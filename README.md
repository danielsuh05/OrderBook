Design Choices:
- I decided against using `strtok_r` for two reasons. One, it edits the underlying string, which means that you must make everything non-const. Two, when I tested the performance of `strtok_r` vs. my own implementation of tokenizing a string, I found that with the `-O3` flag, my implementation was slightly faster. Without the `-O3` flag, mine was slower. So I decided against using the unsafe `strtok_r` and opted for using a `std::string_view` with compiler optimizations, which seemed like the best of both worlds because I could keep it const and it was performant. You can see my results from the tests below:
  - `ITERATIONS = 10000000` with `-O3` flag:
    - `strtok_r` time: 3.577117 seconds
    - custom split time: 2.897383 seconds
  - `ITERATIONS = 10000000` without `-O3` flag:
      - `strtok_r` time: 3.578812 seconds
      - custom split time: 3.903009 seconds
- 