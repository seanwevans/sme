#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdint.h>

#ifndef LIMIT
#define LIMIT (1u << 30)
#endif

#ifndef K_MAX
#define K_MAX (1u << 13)
#endif

#define TEST_BIT(sv, x)                                                        \
  ((sv[idx_from_odd(x) >> 3] >> ((idx_from_odd(x)) & 7U)) & 1U)

#define SET_BIT(sv, x)                                                         \
  (sv[idx_from_odd(x) >> 3] |= (1U << ((idx_from_odd(x)) & 7U)))

#define SEGMENT_ODDS (1u << 23)
#define LOG_TABLE_SIZE (1 << 20)
static double log_table[LOG_TABLE_SIZE];

static void init_log_table(void) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < LOG_TABLE_SIZE; i++) {
    log_table[i] = log(1.0 + i);
  }
}

static inline unsigned idx_from_odd(unsigned x) { return (x - 1U) >> 1U; }
static inline int sgn(int n) { return (n > 0) - (n < 0); }

static int forward_difference(unsigned int *arr, unsigned len) {
  for (unsigned level = len; level > 1; level--) {
    for (unsigned i = 0; i < level - 1; i++) {
      arr[i] = arr[i] - arr[i + 1];
    }
  }
  return arr[0];
}

static unsigned small_sieve(const unsigned limit, unsigned **out_primes) {
  if (limit < 2U) {
    *out_primes = NULL;
    return 0U;
  }

  char *is_prime = calloc(limit + 1U, sizeof(char));
  if (!is_prime) {
    perror("calloc");
    *out_primes = NULL;
    return 0U;
  }

  for (unsigned i = 2U; i <= limit; i++) {
    is_prime[i] = 1;
  }

  unsigned r = (unsigned)(floor(sqrt(limit)) + 0.5);
  for (unsigned p = 2U; p <= r; p++) {
    if (is_prime[p]) {
      for (unsigned long long j = (unsigned long long)p * p; j <= limit;
           j += p) {
        is_prime[j] = 0;
      }
    }
  }

  unsigned count = 0U;
  for (unsigned i = 2U; i <= limit; i++) {
    if (is_prime[i]) {
      count++;
    }
  }
  *out_primes = malloc(count * sizeof(unsigned));
  if (!(*out_primes)) {
    perror("malloc");
    free(is_prime);
    return 0U;
  }

  unsigned idx = 0U;
  for (unsigned i = 2U; i <= limit; i++) {
    if (is_prime[i]) {
      (*out_primes)[idx++] = i;
    }
  }
  free(is_prime);

  return count;
}

static unsigned segmented_sieve(const unsigned N, unsigned char *sieve,
                                const unsigned *smallPrimes, unsigned nSmall) {
  const size_t totalOddCount = (N + 1U) / 2U;
  const size_t totalBytes = (totalOddCount + 7U) / 8U;
  const size_t chunkSize = (size_t)SEGMENT_ODDS;
  const size_t nChunks = (totalOddCount + chunkSize - 1U) / chunkSize;

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
  for (size_t chunkIdx = 0; chunkIdx < nChunks; chunkIdx++) {
    const size_t chunkOddBegin = chunkIdx * chunkSize;
    const size_t chunkOddEnd = ((chunkOddBegin + chunkSize) < totalOddCount
                                    ? (chunkOddBegin + chunkSize)
                                    : totalOddCount);

    const unsigned chunkValBegin = (unsigned)(2ULL * chunkOddBegin + 1ULL);
    const unsigned chunkValEnd = (unsigned)(2ULL * chunkOddEnd + 1ULL);

    for (unsigned iPrime = 0; iPrime < nSmall; iPrime++) {
      unsigned p = smallPrimes[iPrime];

      if (p == 2U) {
        continue;
      }

      unsigned long long p2 = (unsigned long long)p * p;
      if (p2 > N) {
        break;
      }

      unsigned long long start =
          (p2 > (unsigned long long)chunkValBegin)
              ? p2
              : (((unsigned long long)chunkValBegin + p - 1ULL) / p) * p;
      if ((start & 1ULL) == 0ULL) {
        start += p;
      }

      for (unsigned long long m = start; m < chunkValEnd; m += 2ULL * p) {
        if (m >= 3ULL) {
          sieve[idx_from_odd((unsigned)m) >> 3] |=
              (1U << ((idx_from_odd((unsigned)m)) & 7U));
        }
      }
    }
  }

  unsigned count = 0U;
  if (N >= 2U) {
    count++;
  }
  for (unsigned val = 3U; val <= N; val += 2U) {
    if (!TEST_BIT(sieve, val)) {
      count++;
    }
  }
  return count;
}

static unsigned generate_primes(const unsigned N, unsigned **outPrimes) {
  *outPrimes = NULL;
  if (N < 2U) {
    return 0U;
  }

  unsigned sqrtN = (unsigned)(floor(sqrt((long double)N)));
  if (sqrtN < 2U) {
    sqrtN = 2U;
  }

  unsigned *smallPrimes = NULL;
  unsigned nSmall = small_sieve(sqrtN, &smallPrimes);
  if (!smallPrimes || !nSmall) {
    fprintf(stderr, "Error generating small primes.\n");
    free(smallPrimes);
    return 0U;
  }

  const size_t totalOddCount = (N + 1U) / 2U;
  const size_t totalBytes = (totalOddCount + 7U) / 8U;
  unsigned char *sieve = calloc(totalBytes, 1);
  if (!sieve) {
    fprintf(stderr, "Error: could not allocate main sieve of size %zu.\n",
            totalBytes);
    free(smallPrimes);
    return 0U;
  }

  unsigned count = segmented_sieve(N, sieve, smallPrimes, nSmall);

  *outPrimes = (unsigned *)malloc(count * sizeof(unsigned));
  if (!(*outPrimes)) {
    perror("malloc");
    free(sieve);
    free(smallPrimes);
    return 0U;
  }

  unsigned idx = 0U;
  if (N >= 2U) {
    (*outPrimes)[idx++] = 2U;
  }

  for (unsigned val = 3U; val <= N; val += 2U) {
    if (!TEST_BIT(sieve, val)) {
      (*outPrimes)[idx++] = val;
    }
  }

  free(sieve);
  free(smallPrimes);
  return count;
}

int main(void) {
#ifdef _OPENMP
  int num_procs = omp_get_num_procs();
  omp_set_num_threads(num_procs);
#endif

  const unsigned limit = LIMIT;
  const unsigned k_max = K_MAX;

  if (k_max < 1U || limit < 2U) {
    puts("Invalid configuration, exiting...\n");
    return EXIT_FAILURE;
  }

  init_log_table();

  printf("generating primes up to %u...", limit);
  fflush(stdout);

  unsigned *primes = NULL;
  unsigned nprimes = generate_primes(limit, &primes);
  if (nprimes == 0U) {
    puts(" 0 primes found, exiting...");
    free(primes);

    return EXIT_FAILURE;
  }

  printf(" %u primes found\n\n", nprimes);
  printf("%16s%17s%18s\n", "k", "Δ", "∂");

  static int f_diff[K_MAX + 1];
  static int p_sum[K_MAX + 1];
  int max_seen_ps = 0;
  int min_seen_ps = 0;

#ifdef _OPENMP
#pragma omp parallel
  {
    unsigned *local_buffer = (unsigned *)malloc((k_max + 1) * sizeof(unsigned));
    if (!local_buffer) {
#pragma omp critical
      { perror("malloc"); }
      exit(EXIT_FAILURE);
    }

#pragma omp for schedule(static, 8) reduction(max                              \
                                              : max_seen_ps)                   \
    reduction(min                                                              \
              : min_seen_ps)
    for (unsigned k = 1U; k <= k_max; k++) {
      bool need_output = ((k & (k - 1)) == 0) || (k == k_max);
      if (need_output) {
        memcpy(local_buffer, primes, k * sizeof(unsigned));
        int diff = forward_difference(local_buffer, k);

#pragma omp critical
        {
          f_diff[k] = diff;
          int abs_diff = (diff < 0) ? -diff : diff;
          double log_val = (abs_diff < LOG_TABLE_SIZE) ? log_table[abs_diff]
                                                       : log(1.0 + abs_diff);
          p_sum[k] = (int)(sgn(diff) * log_val);

          if (p_sum[k] > max_seen_ps) {
            max_seen_ps = p_sum[k];
          }
          if (p_sum[k] < min_seen_ps) {
            min_seen_ps = p_sum[k];
          }
        }
      }
    }

    free(local_buffer);
  }
#else
  unsigned *local_buffer = (unsigned *)malloc((k_max + 1) * sizeof(unsigned));
  if (!local_buffer) {
    perror("malloc");
    free(primes);
    return EXIT_FAILURE;
  }
  for (unsigned k = 1U; k <= k_max; k++) {
    bool need_output = ((k & (k - 1)) == 0) || (k == k_max);
    if (need_output) {
      memcpy(local_buffer, primes, k * sizeof(unsigned));
      int diff = forward_difference(local_buffer, k);

      f_diff[k] = diff;
      int abs_diff = (diff < 0) ? -diff : diff;
      double log_val = (abs_diff < LOG_TABLE_SIZE) ? log_table[abs_diff]
                                                   : log(1.0 + abs_diff);
      p_sum[k] = (int)(sgn(diff) * log_val);

      if (p_sum[k] > max_seen_ps) {
        max_seen_ps = p_sum[k];
      }
      if (p_sum[k] < min_seen_ps) {
        min_seen_ps = p_sum[k];
      }
    }
  }
  free(local_buffer);
#endif

  for (unsigned k = 1U; k <= k_max; k++) {
    bool need_output = ((k & (k - 1)) == 0) || (k == k_max);
    if (need_output) {
      printf("%16u%16d%16d\n", k, f_diff[k], p_sum[k]);
    }
  }

  printf("[%d, %d]\n", min_seen_ps, max_seen_ps);

  free(primes);
  return 0;
}
