#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline const unsigned is_prime(const unsigned n) {
  if (n == 2 || n == 3)
    return 1;
  if (n % 2 == 0 || n % 3 == 0)
    return 0;
  for (unsigned i = 5; i * i <= n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0)
      return 0;

  return 1;
}

static inline const unsigned arithmetic_derivative(unsigned n) {
  if (n < 2)
    return 0;
  if (is_prime(n))
    return 1;

  const unsigned original_n = n;
  unsigned result = 0;

  for (unsigned i = 2; i * i <= n; i++) {
    if (n % i != 0) {
      continue;
    }

    unsigned exponent = 0;
    do {
      n /= i;
      exponent++;
    } while (n % i == 0);
    result += exponent * original_n / i;
  }

  if (n > 1)
    result += original_n / n;

  return result;
}

int main() {
  const unsigned base = 2;
  const unsigned columns = 32;
  const unsigned max_n = 2048;

  unsigned line_no = 1;

  unsigned shades[base];
  for (unsigned i = 0; i < base; i++)
    shades[i] = (255 / (base - 1)) * i;

  for (unsigned i = 0; i < columns; i++) {
    printf("%-2d", i + 1);
  }
  puts("");

  for (unsigned i = 0; i < max_n; i++) {
    const unsigned adi = arithmetic_derivative(i + 1);
    const unsigned adimk = adi % base;
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", 255, shades[adimk], shades[adimk]);

    if ((i + 1) % columns == 0) {
      printf("%d\n", line_no++);
    }
  }

  return 0;
}
