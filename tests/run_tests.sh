#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
cd "$ROOT"
make pagesim >/dev/null
OUTPUT="$(printf '3\n7 0 1 2 0 3 0 4 2 3 0 3 2\n' | ./pagesim --all --quiet)"
printf '%s\n' "$OUTPUT" | grep -q 'FIFO faults'
printf '%s\n' "$OUTPUT" | grep -q 'LRU faults'
printf '%s\n' "$OUTPUT" | grep -q 'Optimal faults'
printf '%s\n' "$OUTPUT" | grep -q 'Page references : 13'
PARALLEL_OUTPUT="$(printf '3\n7 0 1 2 0 3 0 4 2 3 0 3 2\n' | ./pagesim --parallel)"
printf '%s\n' "$PARALLEL_OUTPUT" | grep -q 'PARALLEL COMPARISON'
printf '%s\n' "$PARALLEL_OUTPUT" | grep -q 'FIFO.*page faults'
printf '%s\n' "$PARALLEL_OUTPUT" | grep -q 'LRU.*page faults'
printf '%s\n' "$PARALLEL_OUTPUT" | grep -q 'Optimal.*page faults'
echo "PASS: sequential and pthread-parallel comparison tests"
