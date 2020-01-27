#!/bin/bash

FILES="bench_txt_unsorted/*"

for F in $FILES
do
    mips-parser/bin/parser $F "bench_bins/#"
done

echo -e "Finished.\n"
