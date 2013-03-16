ITERS = 10

test: build
	./gettid_seq_gettimeofday.out $(ITERS)

build: gettid_seq_gettimeofday.c
	gcc -o gettid_seq_gettimeofday.out gettid_seq_gettimeofday.c
