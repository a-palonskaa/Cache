#!/bin/bash

declare -a twoq_tests=(
    "4 5 1 2 3 4 5:0"
    "4 8 1 1 1 1 1 1 1 1:7"
    "5 5 1 2 3 4 5:0"
    "20 40 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10 1 2 3 4 5 6 7 8 9 10:25"
    "20 45 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25:5"
)

declare -a belady_tests=(
    "3 7 1 2 1 3 1 2 3:4"
    "2 5 1 2 3 4 5:0"
    "3 8 1 1 1 1 1 1 1 1:7"
    "4 10 1 2 3 4 1 2 5 1 2 3:5"
    "1 6 1 2 1 3 1 4:2"
    "0 5 1 2 3 4 5:0"
    "3 12 1 2 3 5 3 1 5 6 8 2 1 9:4"
    "4 12 1 2 3 4 1 2 5 1 2 4 3 4:7"
)

run_tests() {
    binary=$1
    name=$2
    shift 2
    tests=("$@")
    successful=true

    for test in "${tests[@]}"; do
        IFS=':' read -r input expected_output <<< "$test"
        actual_output=$(echo "$input" | "$binary")

        if [ "$actual_output" != "$expected_output" ]; then
            echo "TEST FAILED ($name)
                    INPUT: $input
                    EXPECTED: $expected_output
                    ACTUAL: $actual_output"
            successful=false
        fi
    done

    if $successful; then
        echo "[$name] TESTS ARE PASSED"
    fi
}

cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build ./build
run_tests ./build/2q 2q "${twoq_tests[@]}"
run_tests ./build/belady belady "${belady_tests[@]}"
