#!/bin/bash
for i in {0..1000}
do
	./gettid_seq.out $1 >> $2
done
