#!/bin/bash

gcc test.c -o test

EXE="./test"
THREADS=(1 2 4 8 16)
STRATEGIES=("EXP_BACKOFF" "RAND_BACKOFF" "FIXED_BACKOFF" "NO_BACKOFF" "DEFAULT_BACKOFF")

# Header
printf "%-15s | %-10s | %-12s\n" "Strategy" "Threads" "Time (s)"
echo "----------------------------------------------------"

# Run Benchmarks
for strategy in "${STRATEGIES[@]}"; do
    for num_threads in "${THREADS[@]}"; do
        result=$($EXE $num_threads $strategy | xargs) 

        printf "%-15s | %-10s | %-12s\n" "$strategy" "$num_threads" "$result"
    done
    echo "----------------------------------------------------"
done
