#!/bin/bash
cat $2 | while read size push_number ref_output output_PC input
do
    echo "$size $push_number $input" >> $1/test/tst.txt
    $1/build/LFU.out 0< $1/test/tst.txt > $1/test/test.txt
    read real_output 0< $1/test/test.txt
    rm $1/test/test.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    read line
    rm $1/test/tst.txt
    if [[ ref_output -eq real_output ]]
    then
        echo "TEST OK!"
        exit 0
    else
        echo "ERROR in test: ref = $ref_output, real = $real_output"
        exit 1
    fi
done
