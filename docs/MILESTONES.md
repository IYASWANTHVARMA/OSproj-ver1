# 10-Week Milestone Plan — PageSim

The weights below match the supplied milestone schedule and total 100%.

| Week | Stage | Main student deliverable | Weight | PageSim evidence |
|---:|---|---|---:|---|
| 1 | Environment & Project Setup | Ubuntu/WSL + software/library/tool installation | 5% | `README.md`, GCC/Make setup commands |
| 2 | Problem Understanding & Design | Requirements, architecture, OS concepts, design | 8% | `docs/PROJECT_REPORT.md` sections 1–6 |
| 3 | Basic Prototype | Skeleton/code structure + basic functionality | 10% | Interactive input and initial simulation |
| 4 | Core Module – I | First major OS functionality | 10% | Page-reference processing, hit/fault detection, FIFO |
| 5 | Core Module – II | Second major functionality | 12% | LRU and Optimal policies |
| 6 | Integration | Integrate modules | 12% | Unified CLI, `--all`, statistics and error handling |
| 7 | Concurrency / OS Concepts | Threads, IPC, scheduling, memory, as applicable | 10% | Implemented pthread-based parallel comparison: FIFO, LRU, and Optimal run in separate worker threads with independent simulation state. Memory concepts are implemented throughout. IPC and CPU scheduling are outside the scope of this memory-management simulator. |
| 8 | Testing & Debugging | Test cases, debugging, Valgrind/GDB/strace | 10% | `tests/run_tests.sh`, testing plan in report |
| 9 | Optimization & Documentation | Performance, robustness, documentation | 8% | C implementation, validation, README and report |
| 10 | Final Demo & Viva | Working project + presentation + viva | 15% | Runnable program, comparison output, report and viva questions |
| **Total** |  |  | **100%** |  |

## Suggested weekly evidence checklist

- **Week 1:** Screenshot of `gcc --version`, `make --version`, and project folder.
- **Week 2:** Submit problem statement, objectives, architecture, and algorithm flow.
- **Week 3:** Demonstrate input and basic page-hit/page-fault logic.
- **Week 4:** Demonstrate FIFO and explain when replacement begins.
- **Week 5:** Demonstrate LRU and Optimal; compare the fault counts.
- **Week 6:** Demonstrate the integrated `--all` option and statistics.
- **Week 7:** Run `./pagesim --parallel`; explain pthread creation/join, independent worker state, demand paging, frames, and page faults. IPC and CPU scheduling are outside the scope of this memory-management project.
- **Week 8:** Run `make test`; optionally run `valgrind ./pagesim --all` after compiling.
- **Week 9:** Complete report, input validation, and reproducible sample run.
- **Week 10:** Present the problem, architecture, algorithm differences, actual results, limitations, and viva answers.

## Demo command

```bash
make
./pagesim --all       # sequential comparison
./pagesim --parallel  # concurrent pthread comparison
```

Use a consistent reference string when comparing algorithms, and paste your actual program output into the report.
