# PageSim — Demand Paging & Page Replacement Simulator

**Batch:** 13  
**Project number:** 7  
**Project type:** Operating Systems simulation  
**Language:** C  
**Target environment:** Ubuntu / WSL / Linux  
**Algorithms:** FIFO, LRU, Optimal

## 1. Project overview

PageSim is a command-line simulator that demonstrates **demand paging** and three page-replacement algorithms. When a requested page is not in physical memory, a page fault occurs. If all frames are occupied, the selected algorithm chooses a page to evict.

The simulator prints the frame contents after each reference and reports page faults, hits, fault rate, and hit rate. Use `--all` to compare all three algorithms sequentially, or `--parallel` to run FIFO, LRU, and Optimal concurrently in separate POSIX threads.

> This is an educational simulator. It models page-frame decisions; it does not modify the operating system's actual virtual memory.

## 2. Features

- Interactive input for number of frames and page reference string.
- FIFO (First-In, First-Out) replacement.
- LRU (Least Recently Used) replacement.
- Optimal replacement (uses future references; theoretical lower-bound benchmark).
- Per-reference HIT/FAULT table.
- Page-fault count, hit count, fault rate, and hit rate.
- Concurrent comparison using POSIX threads (`pthread`) for the Week 7 concurrency milestone.
- Input validation and a small automated smoke test.
- Build with GCC or `make` on Ubuntu/WSL.

## 3. Requirements

- Ubuntu, WSL Ubuntu, or another Linux distribution.
- GCC compiler and Make.

Install tools on Ubuntu/WSL:

```bash
sudo apt update
sudo apt install build-essential
```

## 4. Build and run

From the project directory:

```bash
make
./pagesim
```

Run all algorithms:

```bash
./pagesim --all
```

Run all algorithms concurrently using POSIX threads (Week 7 concurrency demonstration):

```bash
./pagesim --parallel
```

Hide the detailed frame table:

```bash
./pagesim --all --quiet
```

Show help:

```bash
./pagesim --help
```

You can also compile directly:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/pagesim.c -o pagesim
```

## 5. Sample execution

Example inputs:

```text
Enter number of frames (1-100): 3
Enter page reference string (non-negative integers separated by spaces): 7 0 1 2 0 3 0 4 2 3 0 3 2
```

Choose `--all` to compare FIFO, LRU and Optimal. Exact fault counts are printed by the program, so use the actual output from your run in your lab record/report.

## 6. How it works

1. Read the number of available page frames.
2. Read the page-reference string.
3. For each page request, check whether the page is already loaded.
4. If present, count a **hit**.
5. If absent, count a **page fault**. Load it into an empty frame if one exists.
6. If all frames are full, select a victim according to FIFO, LRU, or Optimal.
7. Update frame metadata and continue.
8. Calculate fault and hit percentages.

### Algorithms

- **FIFO:** Evicts the page that has been in memory the longest.
- **LRU:** Evicts the page that has not been used for the longest time.
- **Optimal:** Evicts the page whose next use is farthest in the future, or that is never used again. This is mainly a comparison benchmark because real systems cannot know the future.

### Formulas

- Page fault rate = (page faults / total references) × 100
- Page hit rate = (page hits / total references) × 100
- Page faults + page hits = total references

## 7. Test

```bash
chmod +x tests/run_tests.sh
make test
```

The smoke test checks sequential and pthread-parallel comparisons and confirms the expected reference count is processed. It is not a substitute for testing additional reference strings.

## 8. Limitations and possible extensions

- Maximum 1,000 page references and 100 frames.
- Page numbers are non-negative integers.
- No hardware page table or actual disk I/O is simulated.
- The pthread feature demonstrates concurrent algorithm simulations; it does not simulate kernel-level scheduling or IPC. Those concepts are not required for this memory-management-focused project.
- Future extensions: random reference-string generator, CSV export, graphical visualization, variable page sizes, and working-set analysis.

## 9. Suggested viva questions

1. **What is demand paging?** Loading a page into memory only when it is requested.
2. **What is a page fault?** An event that occurs when a requested page is not currently in memory.
3. **What is a page frame?** A fixed-size block of physical memory that can hold one page.
4. **What does FIFO do?** Replaces the page that entered memory first.
5. **What does LRU do?** Replaces the page least recently used.
6. **Why is Optimal not directly practical?** It needs knowledge of future page references.
7. **What is thrashing?** Excessive paging that leaves too little time for useful execution.
8. **Can FIFO show Belady's anomaly?** Yes, for some reference strings, adding frames can increase FIFO page faults.
9. **What is a hit?** The requested page is already in a frame.
10. **Why compare algorithms?** To observe how replacement policy changes page-fault counts for the same workload.
