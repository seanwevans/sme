# Sean's Math Experiments

Various mathematical experiments.

## Building with Makefile

Run `make adboxes`, `make bran`, `make magoo`, `make pfd` or `make runs` to
compile individual experiments. `make all` builds every program.

## Experiment: Arithmetic Derivatives

`adboxes.c` visualizes the modular spread of the arithmetic derivative. Compile with

```sh
clang -o a adboxes.c
```

## Experiment: Bran

`bran.c` is the simplest serialization. Compile with
```sh
clang -o bran bran.c
```

## Experiment: Magoo DB

`magoo_db.c` is the simplest possible in memory CRUD database. Compile with
```sh
clang -o magoo magoo_db.c
```

## Experiment: Prime Finite Differences

`prime_finite_differences.c` explores forward differences applied to the primes. Compile with

```sh
clang -O3 -march=native -fopenmp -funroll-loops -ffast-math -flto -fuse-ld=gold prime_finite_differences.c -o p -lm
```

## Experiment: runhash??

I don't really remember what `runs.c` is supposed to do. Some kind of hash maybe? Compile with
```sh
clang -o r runs.c
```
