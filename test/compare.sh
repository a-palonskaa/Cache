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

run_compare() {
    amount=$1
    for ((t=0; t<amount; t++)); do
        input=$(generate_test)
        output_2q=$(echo "$input" | ./build/2q)
        output_belady=$(echo "$input" | ./build/belady)
        echo "BELADY: $output_belady 2Q: $output_2q"
    done
}

cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build ./build
run_compare 20
