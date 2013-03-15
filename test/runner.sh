#!/bin/bash
for i in {0..100}
do
	./gettid_seq.out $1 >> $2
done
