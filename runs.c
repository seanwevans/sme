#include <stdio.h>
#include <stdlib.h>

#define M 2
#define N 255
#define S 0
#define E 64

int find_longest_run(unsigned int seed) {
  srand(seed);
  int current_run = 1;
  int max_run = 1;
  int previous_number = rand() % M;
  int number_with_longest_run = previous_number;

  for (int i = 1; i < N; i++) {
    int current_number = rand() % M;
    if (current_number == previous_number) {
      current_run++;
      if (current_run > max_run) {
        max_run = current_run;
        number_with_longest_run = current_number;
      }
    } else {
      current_run = 1;
    }
    previous_number = current_number;
  }

  return number_with_longest_run;
}

int is_little_endian() {
  unsigned int x = 1;
  return *((unsigned char *)&x) == 1;
}

unsigned long long get_64bit_number(unsigned long long number) {
  if (is_little_endian())
    return number;

  unsigned long long reversed = 0;
  for (int i = 0; i < 8; i++)
    reversed |= ((number >> (i * 8)) & 0xFF) << ((7 - i) * 8);

  return reversed;
}

int main() {
  unsigned long long bit_representation = 0;
  unsigned int seed = S;

  for (; seed <= E; seed++) {
    unsigned int run_number = find_longest_run(seed);
    bit_representation |= ((unsigned long long)run_number << seed);
  }

  printf("%016llx\n", get_64bit_number(bit_representation));
  return 0;
}
