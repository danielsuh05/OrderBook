//
// Created by Daniel Suh on 3/27/25.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string_view>

#define ITERATIONS 10000000

// prevent optimization on loops
volatile size_t strtok_r_count = 0;
volatile size_t custom_split_count = 0;

void test_strtok_r_no_dup(char *input) {
  char *saveptr = nullptr;
  char *token = strtok_r(input, " ", &saveptr);
  while (token != nullptr) {
    strtok_r_count += std::strlen(token);
    token = strtok_r(nullptr, " ", &saveptr);
  }
}

void custom_split_no_dup(std::string_view s) {
  size_t start = 0;
  while (start < s.size()) {
    while (start < s.size() && s[start] == ' ') start++;

    if (start >= s.size()) break;

    size_t end = start;
    while (end < s.size() && s[end] != ' ') end++;

    custom_split_count += end - start;

    start = end;
  }
}

int main() {
  const char test_string[] =
      "This is a test string to be tokenized using "
      "different methods repeatedly.";
  const size_t len = std::strlen(test_string) + 1;

  char *buffer1 = new char[len];
  char *buffer2 = new char[len];

  clock_t start, end;
  double time_strtok_r = 0, time_custom = 0;

  for (int i = 0; i < ITERATIONS; i++) {
    std::memcpy(buffer1, test_string, len);
    start = clock();
    test_strtok_r_no_dup(buffer1);
    end = clock();
    time_strtok_r += ((double)(end - start)) / CLOCKS_PER_SEC;
  }
  std::printf("strtok_r time: %f seconds\n", time_strtok_r);

  for (int i = 0; i < ITERATIONS; i++) {
    std::memcpy(buffer2, test_string, len);
    start = clock();
    custom_split_no_dup(buffer2);
    end = clock();
    time_custom += ((double)(end - start)) / CLOCKS_PER_SEC;
  }
  std::printf("custom split time: %f seconds\n", time_custom);

  delete[] buffer1;
  delete[] buffer2;
  return 0;
}
