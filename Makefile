CC?=clang
CFLAGS_PFD=-O3 -march=native -fopenmp -funroll-loops -ffast-math -flto -fuse-ld=gold

.PHONY: adboxes bran magoo pfd runs all

adboxes: adboxes.c
	$(CC) -o a adboxes.c

bran: bran.c
	$(CC) -o bran bran.c

magoo: magoo_db.c
	$(CC) -o magoo magoo_db.c

pfd: prime_finite_differences.c
	$(CC) $(CFLAGS_PFD) prime_finite_differences.c -o p -lm

runs: runs.c
	$(CC) -o r runs.c

all: adboxes bran magoo pfd runs
