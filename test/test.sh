#!/bin/bash
cat $1 | while read size push_number ref_output input
do
    echo "$size $push_number $input" >> test/tst.txt
    ./LFU.out 0< test/tst.txt > test/test.txt
    read real_output 0< test/test.txt
    rm test/test.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    if [[ ref_output -eq real_output ]]
    then
        echo "TEST OK!"
    else
        echo "ERROR in test: ref = $ref_output, real = $real_output"
        exit 1
    fi
    read line
    rm test/tst.txt
done
