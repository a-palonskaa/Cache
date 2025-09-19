#!/bin/bash

generate_test() {
    n=$(((RANDOM % 20) + 1))
    m=$((RANDOM % (5*n) + n))
    seq=""
    for ((i=0; i<m; i++)); do
        seq+="$((RANDOM % n + 1)) "
    done
    echo "$n $m $seq"
}

run_compare_generated() {
    amount=$1
    for ((t=0; t<amount; t++)); do
        input=$(generate_test)
        output_2q=$(echo "$input" | ./build/2q)
        output_belady=$(echo "$input" | ./build/belady)
        echo "BELADY: $output_belady 2Q: $output_2q"
    done
}

run_compare() {
    test_dir=$1
    for testfile in "$test_dir"/*.dat; do
        result=$( { time ./build/2q < "$testfile" ;} 2>&1)
        output_twoq=$(echo "$result" | head -n 1)
        twoq_time=$(echo "$result" | awk '/real/ {print $2}')

        result=$( { time ./build/belady < "$testfile" ;} 2>&1)
        output_belady=$(echo "$result" | head -n 1)
        belady_time=$(echo "$result" | awk '/real/ {print $2}')

        echo "[BELADY] hits: $output_belady time: $belady_time"
        echo "[2Q]     hits: $output_twoq time: $twoq_time"
    done
}

cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build ./build

mode=${1}
if [ "$mode" = "generated" ]; then
    run_compare_generated 20
else
    run_compare "./test/testfiles"
fi
