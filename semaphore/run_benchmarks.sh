#!/bin/bash

# Array of benchmark files
benchmarks=("benchmark1.c" "benchmark2.c" "benchmark3.c")

for file in "${benchmarks[@]}"
do
    # Extract name without .c extension
    name="${file%.*}"

    echo "Compiling and running $file..."
    
    # Compile with pthread support
    gcc -O3 "$file" -o "$name" -lpthread

    if [ $? -eq 0 ]; then
        # Execute the benchmark
        ./"$name"
        # Print the separator
        echo ""
        echo "------------------------------------------"
        echo ""
    else
        echo "Error: Failed to compile $file"
    fi

    # Optional: Remove the binary after running
    rm -f "$name"
done


# chmod +x run_benchmarks.sh