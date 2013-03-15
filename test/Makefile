ITERS_PER_TEST = 1000

OUTPUT_FILE = gettid_seq.times.txt

#set this to TSC for tsc timing, otherwise, clock_gettime
MACROS = TSC
#MACROS = FOO

test: build clean
	sh runner.sh $(ITERS_PER_TEST) $(OUTPUT_FILE) $(TESTS)

build: gettid_seq.c
	gcc -lrt -D $(MACROS) -o gettid_seq.out gettid_seq.c

clean:
	-rm $(OUTPUT_FILE)
