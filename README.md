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
```

# Condition Variable Implementation

A custom implementation of a condition variable synchronization primitive using semaphores and mutexes. This project includes the implementation logic and a benchmark suite to compare its performance against the standard C `pthread_cond_t`.

## Core Functions

* **`init`**: Initializes the condition variable structure, setting the waiter count to zero and preparing the internal semaphore and mutex.
* **`wait`**: Atomically releases the provided external mutex and blocks the calling thread. The mutex is re-acquired before the function returns.
* **`signal`**: Checks if there are any threads currently blocked and resumes exactly one waiting thread.
* **`broadcast`**: Resumes all threads currently waiting on the condition variable by signaling the internal semaphore for each waiter.
* **`destroy`**: Cleans up the internal synchronization primitives to prevent resource leaks.



## Benchmarking

To run the performance benchmarks and compare the custom implementation with the standard C library, you must first ensure the script is executable:

```bash
chmod +x run_benchmarks.sh
./run_benchmarks.sh
