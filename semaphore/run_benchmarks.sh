#!/bin/bash

BLUE='\033[0;34m'
NC='\033[0m'



echo -e "${BLUE}The first benchmark treats the semaphore like a mutex${NC}"
gcc "benchmark1.c" -o "benchmark1" 
echo "Compiling benchmark..."
./"benchmark1"
echo -e "\n"
rm -f "benchmark1"

echo -e "${BLUE}The second benchmark lets the semaphore have more active threads${NC}"
gcc "benchmark2.c" -o "benchmark2" 
echo "Compiling benchmark..."
./"benchmark2"
echo -e "\n"
rm -f "benchmark2"

echo -e "${BLUE}The third benchmark is similar to the second but avoids compiler optimizations${NC}"
gcc "benchmark3.c" -o "benchmark3" 
echo "Compiling benchmark..."
./"benchmark3"
echo -e "\n"
rm -f "benchmark3"



# chmod +x run_benchmarks.sh