#!/bin/bash

touch $1/test/test.txt
$1/build/LFU.out 0< $2 > $1/test/test.txt
read real_output 0< $1/test/test.txt
read ref_output 0< $2.ans
rm $1/test/test.txt
# echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
if [[ ref_output -eq real_output ]]
then
    echo "TEST OK!"
    exit 0
else
    echo "ERROR in test: ref = $ref_output, real = $real_output"
    exit 1
fi
