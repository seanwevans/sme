# Sean's Math Experiments

Various mathematical experiments.

## Experiment: Prime Finite Differences

### Overview

The `prime_finite_differences.c` program explores forward differences applied to prime sequences.

### Compilation

```sh
clang -O3 -march=native -fopenmp -funroll-loops -ffast-math -flto -fuse-ld=gold prime_finite_differences.c -o p -lm
```

### Running

```sh
./prime_finite_differences
```

### Output

The program prints results in a table format:

```
generating primes up to 1073741824... 54400028 primes found

               k               Δ               ∂
               1               2               1
               2              -1               0
               4               1               0
               8              53               3
              16           -3667              -8
              32       450374071              19
              64     -1653207447             -21
             128     -1210083219             -20
             256      -824202257             -20
             512      1728557183              21
            1024     -1563954751             -21
            2048      2093261253              21
            4096      1657561047              21
            8192      1987046059              21
[-21, 21]
```

At the end, it displays the observed range of transformed sums.
