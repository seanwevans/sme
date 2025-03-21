# Sean's Math Experiments

Various mathematical experiments.

## Experiment: Arithmetic Derivatives

`adboxes.c` visualizes the modular spread of the arithmetic derivative. Compile with

```sh
clang -o a adboxes.c
```

## Experiment: Prime Finite Differences

`prime_finite_differences.c` explores forward differences applied to the primes. Compile with

```sh
clang -O3 -march=native -fopenmp -funroll-loops -ffast-math -flto -fuse-ld=gold prime_finite_differences.c -o p -lm
```