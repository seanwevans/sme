# Sean's Math Experiments

Various mathematical experiments. Each directory entry is a small C program with
its own focus. The compile commands below assume clang but any C compiler should
work.

## Building with Makefile

Run `make adboxes`, `make bran`, `make magoo`, `make pfd` or `make runs` to
compile individual experiments. `make all` builds every program.

## Experiment: Arithmetic Derivatives

`adboxes.c` visualizes how the arithmetic derivative behaves modulo a chosen
base. The program prints a grid of coloured boxes to the terminal. Each row
represents a range of integers and the shade of the box is determined by the
arithmetic derivative modulo the base.

Compile with:

```sh
clang -o a adboxes.c
```

Run with:

```sh
./a
```

The output is a series of ANSI coloured blocks forming a grid. Every row is
labelled with a line number.

## Experiment: Bran

`bran.c` demonstrates packing eight bytes into a 64‑bit integer and unpacking
them again.

Compile with:

```sh
clang -o bran bran.c
```

Run with:

```sh
./bran
```

Expected output shows the original byte sequence, the packed integer and the
bytes after unpacking.

## Experiment: Magoo DB

`magoo_db.c` is a very small in‑memory CRUD database used for demonstration
purposes. The main program inserts two records, updates one and deletes the
other before printing whether the first record is still active ("1").

Compile with:

```sh
clang -o magoo magoo_db.c
```

Run with:

```sh
./magoo
```

The program prints `1` to indicate the remaining record is active.

## Experiment: Prime Finite Differences

`prime_finite_differences.c` explores forward differences of the prime numbers
and uses OpenMP to parallelise parts of the calculation. It generates primes up
to a configurable limit and prints a table of selected values of `k` with the
corresponding forward difference and a logarithmic sum.

This program requires an OpenMP-capable compiler and the OpenMP runtime to be
installed.

Compile with:

```sh
clang -O3 -march=native -fopenmp -funroll-loops -ffast-math -flto -fuse-ld=gold prime_finite_differences.c -o p -lm
```

Run with:

```sh
./p
```

The output starts with the number of primes generated followed by a table of `k`,
delta and partial sums, ending with the range of the partial sums.

## Experiment: runhash??

`runs.c` iterates over a range of seeds and finds the value that produces the
longest run of equal pseudorandom bits for each seed. It packs these results
into a 64‑bit integer which is printed in hexadecimal form.

Compile with:

```sh
clang -o r runs.c
```

Run with:

```sh
./r
```

The program prints a single 16‑character hexadecimal string representing the
bit pattern.
