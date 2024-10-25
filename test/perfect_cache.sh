#!/bin/bash
cat $1 | while read size push_number ref_output input
do
    echo "$size $push_number $input" >> test/tstp.txt
    ./perfect_cache 0< test/tstp.txt > test/testp.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    read output 0< test/testp.txt
    echo "PERFECT CACHE TEST return $output"
    rm test/tstp.txt test/testp.txt
done
