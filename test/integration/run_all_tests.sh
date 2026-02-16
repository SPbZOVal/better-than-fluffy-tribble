#!/bin/bash

# Run all integration tests
set -e

BUILD_DIR="../../build"

echo "Building btft executable..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake .. -DBUILD_TESTS=OFF
make -j$(nproc)
cd "../test/integration"

echo "Running integration tests..."

TESTS=(
    "echo_basic"
    "echo_multiple"
    "cat_file"
    "pwd_test"
    "wc_test"
)

PASSED=0
FAILED=0

for test_name in "${TESTS[@]}"; do
    if ./run_test.sh "$test_name"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
done

echo ""
echo "Test Results:"
echo "✅ Passed: $PASSED"
echo "❌ Failed: $FAILED"
echo "Total: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo "All tests passed! 🎉"
    exit 0
else
    echo "Some tests failed! 😞"
    exit 1
fi
