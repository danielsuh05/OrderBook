# depth++
## Overview:
depth++ is an efficient order book written in C++. It is a personal project and is not close to HFT speeds (for obvious reasons). It uses ITCH as the protocol, as this is the most widely used across stock exchanges. I tried to write down as many design choices I made along the way, along with the code that led me to those conclusions. 

## Design Choices:

- I decided against using `strtok_r` for two reasons. One, it edits the underlying string, which means that you must
  make everything non-const. Two, when I tested the performance of `strtok_r` vs. my own implementation of tokenizing a
  string, I found that with the `-O3` flag, my implementation was slightly faster. Without the `-O3` flag, mine was
  slower. So I decided against using the unsafe `strtok_r` and opted for using a `std::string_view` with compiler
  optimizations, which seemed like the best of both worlds because I could keep it const and it was performant. You can
  see my results from the tests below:
    - ITERATIONS = 10,000,000 with `-O3` flag:
        - `strtok_r` time: 3.577117 seconds
        - custom split time: 2.897383 seconds
    - ITERATIONS = 10,000,000 without `-O3` flag:
        - `strtok_r` time: 3.578812 seconds
        - custom split time: 3.903009 seconds
- Error handler is a separate thread for efficiency reasons. Because error handling does not necessarily need to be done
  instantaneously (unless a critical error), we can report and fix the error on a separate thread. We could have
  multiple threads going through the error queue; something to check out in the future. 
- I used `std::boost::spirit::qi::parse` due to performance reasons. Through my testing (`testing/spirit_vs_atoi.cpp`), these are the results I got back:
  - `std::atoi`: 180 ms
  - `boost::qi::parse`: 74 ms
