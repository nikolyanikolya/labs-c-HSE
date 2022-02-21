#!/bin/bash
set -euo pipefail
[ "$#" -ge 1 ] || (echo "Expected at least one argument: command to run" && exit 1)

FAIL=0

for TEST in test-data/*.in; do
    TEST=$(basename "$TEST" .in)

    echo "===== $TEST ====="
    echo "Cleaning up..."
    rm -f test-data/*.out
    echo "    ok"

    echo "Running..."
    RET=0
    "$@" <"test-data/$TEST.in" >"test-data/$TEST.out" 2>"test-data/$TEST.err" || RET=$?
    echo "    exited with code $RET"
    if [ "$RET" != 0 ]; then FAIL=1; fi

    echo "Checking stdout..."
    diff "test-data/$TEST.expected" "test-data/$TEST.out" && echo "    ok" || FAIL=1

    echo "Ensuring stderr is empty..."
    diff /dev/null "test-data/$TEST.err" && echo "    ok" || FAIL=1

    echo "Checking remaining files..."
    CHECKED_COUNT=1
    for F in test-data/$TEST.*.expected; do
        F=$(basename "$F" .expected)
        echo "    $F"
        diff "test-data/$F.expected" "test-data/$F.out" && echo "        ok" || FAIL=1
        ((CHECKED_COUNT += 1))
    done

    echo "Ensuring no stray fiels..."
    ([ "$CHECKED_COUNT" == "$(ls test-data/*.out | wc -l)" ] && echo "    ok") || (FAIL=1 && ls test-data/*.out)
done
echo "FAIL=$FAIL"
exit $FAIL
