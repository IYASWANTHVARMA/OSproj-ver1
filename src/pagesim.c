/*
 * PageSim: Demand Paging and Page Replacement Simulator
 * OS Concepts: page faults, demand paging, FIFO, LRU, Optimal replacement.
 * Build: gcc -std=c11 -Wall -Wextra -pedantic src/pagesim.c -o pagesim
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#define MAX_REFS 1000
#define MAX_FRAMES 100
#define LINE_SIZE 8192

typedef enum { FIFO = 1, LRU = 2, OPTIMAL = 3 } Algorithm;

static const char *algorithm_name(Algorithm a) {
    switch (a) {
        case FIFO: return "FIFO";
        case LRU: return "LRU";
        case OPTIMAL: return "Optimal";
        default: return "Unknown";
    }
}

static int find_page(const int frames[], int frame_count, int page) {
    for (int i = 0; i < frame_count; i++)
        if (frames[i] == page) return i;
    return -1;
}

static int choose_victim(Algorithm alg, const int frames[], int frame_count,
                         const int refs[], int ref_count, int current,
                         const int last_used[], const int loaded_at[]) {
    if (alg == FIFO) {
        int victim = 0;
        for (int i = 1; i < frame_count; i++)
            if (loaded_at[i] < loaded_at[victim]) victim = i;
        return victim;
    }
    if (alg == LRU) {
        int victim = 0;
        for (int i = 1; i < frame_count; i++)
            if (last_used[i] < last_used[victim]) victim = i;
        return victim;
    }

    int victim = 0;
    int farthest = -1;
    for (int i = 0; i < frame_count; i++) {
        int next_use = INT_MAX;
        for (int j = current + 1; j < ref_count; j++) {
            if (refs[j] == frames[i]) {
                next_use = j;
                break;
            }
        }
        if (next_use > farthest) {
            farthest = next_use;
            victim = i;
        }
    }
    return victim;
}

static int simulate(const int refs[], int ref_count, int frame_count, Algorithm alg,
                    int verbose, int print_summary) {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int loaded_at[MAX_FRAMES];
    int faults = 0, hits = 0, next_load_time = 0;

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        last_used[i] = -1;
        loaded_at[i] = -1;
    }

    if (verbose) {
        printf("\nAlgorithm: %s | Frames: %d\n", algorithm_name(alg), frame_count);
        printf("Step | Page | Frame contents (empty = -) | Result\n");
        printf("-----+------+----------------------------+--------\n");
    }

    for (int i = 0; i < ref_count; i++) {
        int page = refs[i];
        int slot = find_page(frames, frame_count, page);
        int fault = (slot == -1);

        if (fault) {
            faults++;
            int empty = -1;
            for (int k = 0; k < frame_count; k++) {
                if (frames[k] == -1) { empty = k; break; }
            }
            if (empty != -1) {
                slot = empty;
            } else {
                slot = choose_victim(alg, frames, frame_count, refs, ref_count,
                                     i, last_used, loaded_at);
            }
            frames[slot] = page;
            loaded_at[slot] = next_load_time++;
        } else {
            hits++;
        }
        last_used[slot] = i;

        if (verbose) {
            printf("%4d | %4d | ", i + 1, page);
            for (int k = 0; k < frame_count; k++) {
                if (frames[k] == -1) printf(" - ");
                else printf("%2d ", frames[k]);
            }
            printf("| %s\n", fault ? "FAULT" : "HIT");
        }
    }

    double fault_rate = ref_count ? (100.0 * faults / ref_count) : 0.0;
    double hit_rate = ref_count ? (100.0 * hits / ref_count) : 0.0;
    if (print_summary) {
        printf("\n%s summary\n", algorithm_name(alg));
        printf("Page references : %d\n", ref_count);
        printf("Page faults     : %d\n", faults);
        printf("Page hits       : %d\n", hits);
        printf("Fault rate      : %.2f%%\n", fault_rate);
        printf("Hit rate        : %.2f%%\n", hit_rate);
    }
    return faults;
}

typedef struct {
    const int *refs;
    int ref_count;
    int frame_count;
    Algorithm alg;
    int faults;
} WorkerArgs;

/* Each worker owns its frame arrays, so the simulations do not share mutable state. */
static void *run_algorithm_thread(void *arg) {
    WorkerArgs *work = (WorkerArgs *)arg;
    work->faults = simulate(work->refs, work->ref_count, work->frame_count,
                            work->alg, 0, 0);
    return NULL;
}

static int run_parallel_comparison(const int refs[], int ref_count, int frame_count) {
    pthread_t threads[3];
    WorkerArgs work[3] = {
        {refs, ref_count, frame_count, FIFO, 0},
        {refs, ref_count, frame_count, LRU, 0},
        {refs, ref_count, frame_count, OPTIMAL, 0}
    };
    int created = 0;
    for (int i = 0; i < 3; i++) {
        int rc = pthread_create(&threads[i], NULL, run_algorithm_thread, &work[i]);
        if (rc != 0) {
            fprintf(stderr, "Could not create worker thread for %s (error %d).\n",
                    algorithm_name(work[i].alg), rc);
            for (int j = 0; j < created; j++) pthread_join(threads[j], NULL);
            return 1;
        }
        created++;
    }
    for (int i = 0; i < created; i++) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "Could not join worker thread %d (error %d).\n", i, rc);
            return 1;
        }
    }
    printf("\n========== PARALLEL COMPARISON (pthreads) ==========\n");
    printf("Each algorithm ran in its own worker thread.\n");
    printf("Reference count: %d | Frames: %d\n", ref_count, frame_count);
    for (int i = 0; i < 3; i++)
        printf("%-8s page faults: %d\n", algorithm_name(work[i].alg), work[i].faults);
    printf("Optimal is a theoretical benchmark because it uses future references.\n");
    return 0;
}

static int parse_int(const char *s, int *out) {
    char *end = NULL;
    long value = strtol(s, &end, 10);
    if (s == end || *end != '\0' || value < 0 || value > INT_MAX) return 0;
    *out = (int)value;
    return 1;
}

static int read_reference_string(int refs[], int *count) {
    char line[LINE_SIZE];
    printf("Enter page reference string (non-negative integers separated by spaces): ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    char *token = strtok(line, " ,\t\r\n");
    *count = 0;
    while (token) {
        int value;
        if (!parse_int(token, &value)) {
            fprintf(stderr, "Invalid page number: %s\n", token);
            return 0;
        }
        if (*count >= MAX_REFS) {
            fprintf(stderr, "Too many references (maximum %d).\n", MAX_REFS);
            return 0;
        }
        refs[(*count)++] = value;
        token = strtok(NULL, " ,\t\r\n");
    }
    if (*count == 0) {
        fprintf(stderr, "Reference string cannot be empty.\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int refs[MAX_REFS], ref_count = 0, frame_count = 0;
    int run_all = 0, verbose = 1, parallel = 0;
    Algorithm alg = FIFO;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s [--all] [--parallel] [--quiet]\n", argv[0]);
        printf("Interactive PageSim: demand paging and page replacement.\n");
        printf("Options: --all   run FIFO, LRU and Optimal\n");
        printf("         --parallel run all three policies concurrently using pthreads\n");
        printf("         --quiet suppress per-reference frame table\n");
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--all") == 0) run_all = 1;
        else if (strcmp(argv[i], "--parallel") == 0) { run_all = 1; parallel = 1; }
        else if (strcmp(argv[i], "--quiet") == 0) verbose = 0;
        else {
            fprintf(stderr, "Unknown option: %s (try --help)\n", argv[i]);
            return 1;
        }
    }

    printf("=============================================\n");
    printf(" PageSim: Demand Paging & Page Replacement\n");
    printf("=============================================\n");
    printf("Maximum: %d references, %d frames\n", MAX_REFS, MAX_FRAMES);

    printf("Enter number of frames (1-%d): ", MAX_FRAMES);
    char line[LINE_SIZE];
    if (!fgets(line, sizeof(line), stdin)) return 1;
    char *end = NULL;
    long fc = strtol(line, &end, 10);
    while (end && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) end++;
    if (end == line || (end && *end != '\0') || fc < 1 || fc > MAX_FRAMES) {
        fprintf(stderr, "Invalid frame count. Enter a number from 1 to %d.\n", MAX_FRAMES);
        return 1;
    }
    frame_count = (int)fc;

    if (!read_reference_string(refs, &ref_count)) return 1;

    if (run_all && parallel) {
        if (run_parallel_comparison(refs, ref_count, frame_count) != 0) return 1;
    } else if (run_all) {
        int fifo_faults = simulate(refs, ref_count, frame_count, FIFO, verbose, 1);
        int lru_faults = simulate(refs, ref_count, frame_count, LRU, verbose, 1);
        int opt_faults = simulate(refs, ref_count, frame_count, OPTIMAL, verbose, 1);
        printf("\n========== COMPARISON ==========\n");
        printf("FIFO faults    : %d\n", fifo_faults);
        printf("LRU faults     : %d\n", lru_faults);
        printf("Optimal faults : %d\n", opt_faults);
        printf("Note: Optimal is a theoretical benchmark; it requires future knowledge.\n");
    } else {
        printf("\nChoose algorithm:\n1. FIFO\n2. LRU\n3. Optimal\nSelection: ");
        if (!fgets(line, sizeof(line), stdin)) return 1;
        int choice;
        if (!parse_int(strtok(line, " \t\r\n"), &choice) || choice < 1 || choice > 3) {
            fprintf(stderr, "Invalid algorithm selection.\n");
            return 1;
        }
        alg = (Algorithm)choice;
        simulate(refs, ref_count, frame_count, alg, verbose, 1);
    }
    return 0;
}
