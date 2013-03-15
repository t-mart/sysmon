N_THREADS = 1000
ITERS = 1000

test: build
	strace -e trace=gettid -T ./gettid_seq.out $(ITERS) 2>&1 | grep --extended-regexp -e "[[:digit:]]\.[[:digit:]]+" --only-matching > seq.times
	strace -e trace=gettid -T ./gettid_omp.out $(N_THREADS) $(ITERS) 2>&1 | grep --extended-regexp -e "[[:digit:]]\.[[:digit:]]+" --only-matching > omp.times

build: gettid_omp.c gettid_seq.c
	gcc -o gettid_seq.out gettid_seq.c
	gcc -fopenmp -o gettid_omp.out gettid_omp.c
