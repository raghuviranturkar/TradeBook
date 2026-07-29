#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "========================================"
echo "  TradeBook - Build Script"
echo "========================================"

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo -e "${YELLOW}→ Configuring with CMake...${NC}"
cmake .. || {
    echo -e "${RED}❌ CMake configuration failed!${NC}"
    exit 1
}

# Build the project
echo -e "${YELLOW}→ Building project...${NC}"
cmake --build . --config Release || {
    echo -e "${RED}❌ Build failed!${NC}"
    exit 1
}

echo -e "${GREEN}✅ Build successful!${NC}"

# Run the main program
echo ""
echo "========================================"
echo "  Running TradeBook"
echo "========================================"
./TradeBook

# Run tests
echo ""
echo "========================================"
echo "  Running Tests"
echo "========================================"

for test in TradeBookTests OrderBookTests LifecycleTests; do
    if [ -f "./$test" ]; then
        echo -e "${YELLOW}→ Running $test...${NC}"
        ./$test
    else
        echo -e "${RED}❌ $test not found${NC}"
    fi
done

# Run benchmark
if [ -f "./Benchmark" ]; then
    echo ""
    echo "========================================"
    echo "  Running Benchmark"
    echo "========================================"
    ./Benchmark
fi

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}  Build Complete!${NC}"
echo -e "${GREEN}========================================${NC}"
