# PageSim: Demand Paging and Page Replacement Simulator
## Operating Systems Mini Project Report

**Batch:** 13  
**Project No.:** 7  
**Project title:** PageSim — Demand Paging & Page Replacement  
**Student name / roll number:** Fill in your own details  
**Department / section:** Fill in  
**Guide / faculty:** Fill in  
**Academic year:** Fill in

---

## Abstract

PageSim is a C-based educational simulator for understanding demand paging and page-replacement strategies in operating systems. It accepts a page-reference string and a number of memory frames, then simulates FIFO, LRU, or Optimal replacement. For each reference, the program identifies a hit or page fault and displays the frame state. It then calculates page-fault and page-hit totals and their rates. A pthread-based mode also runs the three independent policy simulations concurrently, demonstrating OS threads and concurrency. The project provides a practical way to compare algorithms using the same workload and supports learning about virtual memory, locality, and replacement decisions.

## 1. Problem statement

When a process refers to a page that is not currently in physical memory, the operating system must handle a page fault and bring the page into memory. If all page frames are occupied, a page-replacement policy selects a page to remove. Different policies can produce different numbers of page faults. This project simulates and compares common policies without changing the host operating system's memory.

## 2. Objectives

- Demonstrate demand paging and page-fault handling conceptually.
- Implement FIFO, LRU, and Optimal page replacement.
- Display the frame contents after each page reference.
- Calculate hits, faults, hit rate, and fault rate.
- Compare replacement policies for the same input.
- Build and test the program on Ubuntu/WSL using GCC.
- Demonstrate concurrency by running independent replacement-policy simulations in POSIX threads.

## 3. Scope

The project is a user-space simulation. It models a fixed number of frames and a supplied reference string. It does not create real page tables, handle hardware interrupts, or perform actual swap/disk I/O.

## 4. Requirements

### Hardware
A standard desktop or laptop capable of running Linux or Windows with WSL.

### Software
- Ubuntu/WSL or another Linux environment
- GCC C compiler
- GNU Make (optional but recommended)
- Terminal

## 5. Design and architecture

**Input module**
- Reads frame count.
- Reads non-negative integer page references.
- Validates the input.

**Simulation module**
- Checks whether each page is already loaded.
- Counts a hit if it is present.
- Counts a fault if it is absent.
- Uses an empty frame when available.
- Calls the selected replacement policy when all frames are full.

**Replacement-policy module**
- FIFO: uses load order.
- LRU: uses last-used reference position.
- Optimal: searches the remaining reference string for each resident page's next use.

**Concurrency module**
- Creates one POSIX thread per replacement algorithm in `--parallel` mode.
- Each thread has independent frame metadata and computes its own page-fault count.
- The main thread joins workers and prints results after all workers finish.

**Statistics module**
- Computes total hits and faults.
- Computes hit and fault percentages.
- Prints a summary and, optionally, an algorithm comparison.

## 6. Algorithm / procedure

1. Start the program.
2. Enter the number of frames.
3. Enter the page-reference string.
4. Select FIFO, LRU, Optimal, or run all three.
5. Process references from left to right.
6. If the page is already in a frame, mark HIT.
7. Otherwise mark FAULT and load it into an empty frame if possible.
8. If no frame is empty, select and replace a victim page using the selected policy.
9. Print the frame table and update statistics.
10. Display totals and rates.
11. Stop.

## 7. Algorithm explanation

### FIFO
The page that has been resident for the longest time is replaced first. It is simple, but can exhibit Belady's anomaly.

### LRU
The page whose most recent access is farthest in the past is replaced. It uses recent history as an approximation of near-future use.

### Optimal
The page whose next use is farthest in the future is replaced. It provides a theoretical benchmark for the minimum possible number of faults for a known reference string, but real operating systems cannot generally know future references.

## 8. Build and execution

```bash
sudo apt update
sudo apt install build-essential
make
./pagesim --all       # sequential algorithm comparison
./pagesim --parallel  # run FIFO, LRU and Optimal in pthreads
```

To run without the step-by-step frame table:

```bash
./pagesim --all --quiet
```

## 9. Testing plan

| Test | Input / action | Expected behavior |
|---|---|---|
| T1 | Valid frame count and reference string | Simulation completes |
| T2 | A page is referenced twice while resident | Second access is a hit |
| T3 | More unique pages than available frames | Replacement occurs after frames fill |
| T4 | `--all` option | FIFO, LRU and Optimal summaries appear |
| T4b | `--parallel` option | pthread workers complete and all three fault counts appear |
| T5 | Frame count 0 or greater than 100 | Input is rejected |
| T6 | Negative or non-numeric page | Input is rejected |
| T7 | Empty reference string | Input is rejected |
| T8 | `--quiet` option | Summary appears without step-by-step table |

Run the included smoke test using `make test`. It tests both sequential and pthread-parallel modes.

## 10. Results and analysis

Record the actual output from your own run below. Do not copy guessed results; the simulator prints the values for the exact input you use.

| Algorithm | Page faults | Page hits | Fault rate | Hit rate |
|---|---:|---:|---:|---:|
| FIFO | Fill from output | Fill from output | Fill from output | Fill from output |
| LRU | Fill from output | Fill from output | Fill from output | Fill from output |
| Optimal | Fill from output | Fill from output | Fill from output | Fill from output |

**Analysis prompts**
- Which algorithm had the fewest faults for your reference string?
- How did the number of frames affect the fault count?
- Why does Optimal act as a theoretical benchmark?
- Did FIFO and LRU produce the same result? Explain using the access pattern.

## 11. Limitations

- Simulates policy decisions rather than actual kernel memory management.
- Threads demonstrate concurrent user-space simulations; IPC and CPU scheduling are not implemented because the selected project focuses on virtual-memory page replacement.
- Uses a fixed number of frames.
- Optimal relies on future references and is not directly implementable as a general real-time policy.
- The reference string is entered manually.

## 12. Concurrency milestone (Week 7)

The `--parallel` option launches three POSIX worker threads. Each worker runs one replacement algorithm using the same read-only reference string and its own local frame arrays. The main thread waits with `pthread_join()` before displaying the combined results. This demonstrates thread creation, independent thread state, concurrent execution, and synchronization at completion. It does not claim to implement inter-process communication or a CPU scheduler.

## 13. Future enhancements

- Generate reference strings automatically.
- Export the comparison table to CSV.
- Add a graphical frame-by-frame visualization.
- Support working-set and Clock/Second-Chance algorithms.
- Add more automated unit tests.

## 15. Milestone traceability (10-week plan)

The project follows the milestone stages and weightages supplied for the course. The total is 100%.

| Week | Stage | Main student deliverable | Weight | Project evidence |
|---:|---|---|---:|---|
| 1 | Environment & Project Setup | Ubuntu/WSL + software/library/tool installation | 5% | README setup and build instructions |
| 2 | Problem Understanding & Design | Requirements, architecture, OS concepts, design | 8% | Problem statement, objectives, design and algorithms in this report |
| 3 | Basic Prototype | Skeleton/code structure + basic functionality | 10% | Interactive CLI, validated input, basic simulation |
| 4 | Core Module – I | First major OS functionality | 10% | Demand paging model, page-hit/page-fault detection, FIFO |
| 5 | Core Module – II | Second major functionality | 12% | LRU and Optimal replacement policies |
| 6 | Integration | Integrate modules | 12% | Integrated CLI, sequential comparison, statistics and error handling |
| 7 | Concurrency / OS Concepts | Threads, IPC, scheduling, memory, etc. as applicable | 10% | POSIX pthreads run FIFO, LRU and Optimal in separate worker threads; memory-management concepts are explained. IPC and CPU scheduling are outside this project's memory-management scope. |
| 8 | Testing & Debugging | Test cases, debugging, Valgrind/GDB/strace | 10% | Automated smoke tests and test plan; Valgrind/GDB/strace can be used for further inspection |
| 9 | Optimization & Documentation | Performance, robustness, documentation | 8% | Compiler warnings enabled, input validation, README and report |
| 10 | Final Demo & Viva | Working project + presentation + viva | 15% | Runnable source, presentation outline, results template and viva questions |
| **Total** |  |  | **100%** |  |

## 14. Conclusion

PageSim demonstrates how demand paging and replacement algorithms operate using a clear, repeatable simulation. It provides frame-by-frame output and quantitative statistics so that FIFO, LRU, and Optimal can be compared on the same input. The project reinforces core operating-system concepts and is suitable for a student demonstration and viva.

## 15. Viva questions and short answers

1. **What is virtual memory?** A memory-management technique that gives processes a logical address space beyond the immediately available physical memory.
2. **What is demand paging?** Loading a page only when it is needed.
3. **What is a page fault?** A trap/event when the requested page is not resident in memory.
4. **What is page replacement?** Selecting a resident page to remove when a new page must be loaded and no frame is free.
5. **What is FIFO?** Replaces the earliest-loaded resident page.
6. **What is LRU?** Replaces the page least recently accessed.
7. **What is Optimal?** Replaces the page whose next use is farthest away.
8. **Why is Optimal useful?** It provides a comparison benchmark for page-fault counts.
9. **What is a page hit?** A reference to a page already in memory.
10. **What is thrashing?** A condition in which paging activity becomes excessive and useful execution falls.
