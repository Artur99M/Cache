#!/bin/bash
cat test/test.txt | while read line number
do
    read line size
    read line push_number
    read line input
    read line ref_output
    echo "$size $push_number $input" >> test/tst$number.txt
    ./LFU 0< test/tst$number.txt > test/test$number.txt
    read real_output 0< test/test$number.txt
    rm test/test$number.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    if [[ ref_output -eq real_output ]]
    then
        echo "TEST $number OK!"
    else
        echo "ERROR in test $number: ref = $ref_output, real = $real_output"
    fi
    read line
    rm test/tst$number.txt
done
