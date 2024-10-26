#!/bin/bash
cat $2 | while read size push_number ref_output output_PC input
do
    echo "$size $push_number $input" >> $1/test/tstp.txt
    $1/build/PC.out > $1/test/testp.txt 0< $1/test/tstp.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    read output 0< $1/test/testp.txt
    echo "PERFECT CACHE TEST return $output"
    rm $1/test/tstp.txt $1/test/testp.txt
    if [[ output -ne output_PC ]]
    then
        echo "TEST FAIL: real = $output, ref = $output_PC"
        exit 1
    else
        echo "TEST OK!"
        exit 0
    fi
done
