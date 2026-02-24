#!/bin/bash

# Run all integration tests

# Get the absolute path to the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_INTEGRATION_DIR="$SCRIPT_DIR/integration"

echo "Building btft executable..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake "$PROJECT_ROOT" -DBUILD_TESTS=OFF
make -j$(nproc)
cd "$TEST_INTEGRATION_DIR"

echo "Running integration tests..."

TESTS=(
    "echo_basic"
    "echo_multiple"
    "cat_file"
    "pwd_test"
    "wc_test"
    "env_test"
    "external_env_test"
    "unknown_command_test"
    "grep_file"
)

PASSED=0
FAILED=0

pwd

for test_name in "${TESTS[@]}"; do
    echo "Running test: $test_name"
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
