cmake_minimum_required(VERSION 3.10)
project(TradeBook VERSION 1.0.0)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Cross-platform settings
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Include directories
include_directories(include)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Main executable
add_executable(TradeBook
    src/main.cpp
    src/Order.cpp
    src/Trade.cpp
    src/TradeHistory.cpp
    src/OrderBook.cpp
    src/TradeEngine.cpp
    src/EngineStatistics.cpp
)

target_compile_options(TradeBook PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX->
)

# Benchmark executable
add_executable(Benchmark
    benchmarks/benchmark.cpp
    src/Order.cpp
    src/Trade.cpp
    src/TradeHistory.cpp
    src/OrderBook.cpp
    src/TradeEngine.cpp
    src/EngineStatistics.cpp
)

target_compile_options(Benchmark PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic -O2>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX- /O2>
)

# Test executables
add_executable(TradeBookTests
    tests/test_matching.cpp
    src/Order.cpp
    src/Trade.cpp
    src/TradeHistory.cpp
    src/OrderBook.cpp
    src/TradeEngine.cpp
    src/EngineStatistics.cpp
)

target_compile_options(TradeBookTests PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX->
)

add_executable(OrderBookTests
    tests/test_orderbook.cpp
    src/Order.cpp
    src/Trade.cpp
    src/TradeHistory.cpp
    src/OrderBook.cpp
    src/TradeEngine.cpp
    src/EngineStatistics.cpp
)

target_compile_options(OrderBookTests PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX->
)

add_executable(LifecycleTests
    tests/test_lifecycle.cpp
    src/Order.cpp
    src/Trade.cpp
    src/TradeHistory.cpp
    src/OrderBook.cpp
    src/TradeEngine.cpp
    src/EngineStatistics.cpp
)

target_compile_options(LifecycleTests PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX->
)

# Testing
enable_testing()

add_test(NAME TradeBookTests COMMAND TradeBookTests)
add_test(NAME OrderBookTests COMMAND OrderBookTests)
add_test(NAME LifecycleTests COMMAND LifecycleTests)

# Cross-platform build instructions
if(NOT WIN32)
    add_custom_target(run
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TradeBook
        DEPENDS TradeBook
        COMMENT "Running TradeBook"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()
