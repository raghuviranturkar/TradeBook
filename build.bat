@echo off
echo ========================================
echo   TradeBook - Build Script (Windows)
echo ========================================

if not exist build mkdir build
cd build

echo → Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo ❌ CMake configuration failed!
    exit /b 1
)

echo → Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo ❌ Build failed!
    exit /b 1
)

echo ✅ Build successful!

echo.
echo ========================================
echo   Running TradeBook
echo ========================================
Release\TradeBook.exe

echo.
echo ========================================
echo   Running Tests
echo ========================================

if exist Release\TradeBookTests.exe (
    echo → Running TradeBookTests...
    Release\TradeBookTests.exe
)

if exist Release\OrderBookTests.exe (
    echo → Running OrderBookTests...
    Release\OrderBookTests.exe
)

if exist Release\LifecycleTests.exe (
    echo → Running LifecycleTests...
    Release\LifecycleTests.exe
)

if exist Release\Benchmark.exe (
    echo.
    echo ========================================
    echo   Running Benchmark
    echo ========================================
    Release\Benchmark.exe
)

echo.
echo ========================================
echo   Build Complete!
echo ========================================
