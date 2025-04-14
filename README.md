# depth++
## Overview:
depth++ is an efficient multi-threaded order book written in C++. It uses ITCH as the protocol, as this is the most widely used across stock exchanges. I tried to write down as many design choices I made along the way below. Has the ability to parse 14.2 million messages per second (reference GitHub repository was 14 million on my machine). Machine is a M1 Pro Macbook with 16 GB of RAM. 

## Design Choices:

- The main design choice here is the use of multiple threads. The three threads are main, buffer, and error. See the below picture for a sketch of what the general workflow of the three threads looks like and how they all communicate with each other.
  - First off, I decided to combine the parsing and order book operations into one thread. The parsing (which basically just reads bytes; very quick operation), would be a lot faster than the order book operations (which involved hashmap lookup, data structure modifications, etc.). I didn't think the overhead of the extra thread would be worth it. You would also need to keep an extra queue of order book operations that need to be done, which needs to be protected by a lock; more work that is not necessary. 
  - I chose to put the buffer and file reads on a separate thread. I wanted the buffer to be doing I/O operations while the main thread is doing the parsing and order book operations. That way, when the main thread is done parsing and updating the order book, it can immediately receive another buffer without having to wait for I/O. The drawback of doing this is you need to `std::memcpy` the buffer, otherwise you would be introducing a race condition where the buffer may be overwritten by the buffer thread while the main thread is trying to parse it. However, this extra overhead is less than the overhead of waiting on I/O and wasting CPU cycles waiting for it to finish. 
  - Error handler is a separate thread for obvious reasons. Because error handling does not necessarily need to be done instantaneously (unless a critical error), we can report and fix the error on a separate thread. 
- Another design choice I had to make was how threads communicated with each other.
  - First, the main/buffer thread to the error thread was relatively simple. The error thread is just a singleton that accepts a `postError()` request and will continuously process them in the background. So I just needed to call this `postError()` every time there was some error that needed to be dealt with.
  - The next relationship was a lot more complicated. The state between the main thread and the buffer thread is very entwined and in the future, I want to declutter this relationship. However, I found that we needed a couple of ways to communicate. 
    - The first thing to do is create the buffer. This is as simple as calling the constructor.
    - Once the buffer is done reading from the file descriptor, it needs to return the buffer back to the main thread. 
    - Then, the main thread needs to copy the memory. Once it has done this, it should notify the buffer thread that it can overwrite the buffer. This is done through `saveAfterPos()`, which in essence copies the leftover memory that was not used because it was too small to the beginning and moves `pos` and `limit` so that the buffer can continue to read.
    - Then, we need a condition variable and a predicate between the main and buffer thread. This is because if the buffer finishes reading first, it needs to wait until the main thread is done with the parsing and order book operations. We need the predicate because otherwise the buffer thread may end up waiting forever as it missed the signal. I don't exactly like this implementation, it's something that I could definitely clean up in the future. 
    - Once the main thread is done parsing and doing order book operations, it will notify the buffer thread that it is done. The buffer thread will then return the buffer to the main thread and the process will continue. 
  - I noted all the relationships and threads in the diagram below, which may be easier to read. 
- Another important decision was how to implement the order book itself.
  - I took heavy insipiration from https://github.com/charles-cooper/itch-order-book/blob/master/order_book.h.  
  - There are several data structures:
    - `Pool`: A large array that keeps track of a "free list." Essentially acts as a memory manager for fixed-sized objects. Allows cache locality and very fast access.
    - `Ordered bids/asks`: Implemented as a `std::vector` instead of an ordered set for reasons described below.
    - `Vector of orders`: Because ITCH chooses to use order IDs that have a relatively low max value, we can use a vector to index into a particular order rather than use a slower hashmap. 
    - `Level`: A struct containing a price and the quantity at that price.
    - `LevelPtr`: A struct containing a price and a pointer (index) into the large pool to obtain the `Level` object.
  - Design choices:
    - The reason I chose to use a simple `std::vector` rather than an ordered set for the asks and bids is because most of the activity occurs at the top of the book. According to my experiments, on average, you need to traverse down less than 20 levels to reach the level of interest. This varies for different stock tickers, but on average, is true. This has less overhead than using an ordered set. 
    - The `Level`/`LevelPtr` approach was also for efficiency reasons. For a `LevelPtr`, you could easily ordered bids and asks according to the price. Once you have the `LevelPtr` object, you can quickly access the underlying `Level` to adjust price or quantity as needed. 
  - Order book differences from original code:
    - I wanted to split the class into the `OrderBookManager` and the `OrderBook` rather than having one overarching `OrderBook` class with static functions. It seemed cleaner to me. 
    - I binary searched for the `LevelPtr` rather than decrement from the top. I found it had better performance for the worst-case where you had to go lower in the order book. 
    - There are other minor differences, but the general idea of the `Pool` is the same. 
- Cache locality should be pretty optimized here. All data structures are contiguous in memory. 
- <strike>I believe this only works on MacOS systems because of the way I convert the ITCH big endian to the host byte order. WIP: convert to some other function that makes it cross-platform.</strike> -> I converted to using `boost::endian`.
