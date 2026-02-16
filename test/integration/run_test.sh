#!/bin/bash

# Integration test runner for btft shell
# Usage: ./run_test.sh <test_name>

set -e

TEST_NAME="$1"
TEST_DIR="$(dirname "$0")"
BTFT_EXEC="../../build/btft"  # Path to the built btft executable

if [ -z "$TEST_NAME" ]; then
    echo "Usage: $0 <test_name>"
    exit 1
fi

TEST_INPUT_FILE="$TEST_DIR/${TEST_NAME}.input"
TEST_EXPECTED_FILE="$TEST_DIR/${TEST_NAME}.expected"
TEST_OUTPUT_FILE="$TEST_DIR/${TEST_NAME}.output"

if [ ! -f "$TEST_INPUT_FILE" ]; then
    echo "Error: Test input file $TEST_INPUT_FILE not found"
    exit 1
fi

if [ ! -f "$TEST_EXPECTED_FILE" ]; then
    echo "Error: Test expected file $TEST_EXPECTED_FILE not found"
    exit 1
fi

if [ ! -f "$BTFT_EXEC" ]; then
    echo "Error: btft executable not found at $BTFT_EXEC"
    echo "Please build the project first: mkdir build && cd build && cmake .. && make"
    exit 1
fi

echo "Running test: $TEST_NAME"

# For pwd test, we need to run from the test directory
if [ "$TEST_NAME" = "pwd_test" ]; then
    cd "$TEST_DIR"
fi

# Run the test by feeding input to btft and capturing output
"$BTFT_EXEC" < "$TEST_INPUT_FILE" > "$TEST_OUTPUT_FILE" 2>&1

# Compare output with expected result
if diff -u "$TEST_EXPECTED_FILE" "$TEST_OUTPUT_FILE"; then
    echo "✅ Test $TEST_NAME passed"
    rm -f "$TEST_OUTPUT_FILE"
    exit 0
else
    echo "❌ Test $TEST_NAME failed"
    echo "Expected output saved in: $TEST_EXPECTED_FILE"
    echo "Actual output saved in: $TEST_OUTPUT_FILE"
    exit 1
fi
