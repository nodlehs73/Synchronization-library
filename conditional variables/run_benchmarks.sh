#!/bin/bash

BLUE='\033[0;34m'
NC='\033[0m'

clear

echo -e "${BLUE}The benchmark creates threads that wait for a conditional variable, frees all of them with a Broadcast\nand after that creates again the whole number of threads and frees them one by one with Signals${NC}"
gcc "benchmark.c" -o "benchmark1" 
echo "Compiling benchmark..."
./"benchmark1"
echo -e "\n"
rm -f "benchmark1"

# chmod +x run_benchmarks.sh