cat test/test.txt | while read line number
do
    read line size
    read line push_number
    read line input
    read line ref_output
    echo "$number $size $push_number $input" >> test/tstp$number.txt
    ./perfect_cache 0< test/tstp$number.txt > test/testp$number.txt
    # echo "number = $number, size = $size, push_number = $push_number, input = $input, ref_output = $ref_output, real_output = $real_output"
    read line
    read output 0< test/testp$number.txt
    echo "PERFECT CACHE TEST #$number return $output"
    rm test/tstp$number.txt test/testp$number.txt
done
