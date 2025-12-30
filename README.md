# Semaphore Implementation

A implementation of a semaphore synchronization primitive.
There are also some benchmark results atached.

## Core Functions

* **`init`**: Initializes the semaphore with a specified starting value.
* **`wait`**: Decrements the semaphore value. If the value is zero, the calling thread blocks.
* **`post`**: Increments the semaphore value and signals any waiting threads to resume.



## Benchmarking

To run the performance benchmarks, you must first ensure the script is executable:

```bash
chmod +x run_benchmarks.sh
./run_benchmarks.sh
