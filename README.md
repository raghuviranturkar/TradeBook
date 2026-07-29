# TradeBook

<div align="center">

# ⚡ TradeBook
### High-Performance C++ Trading Order Matching Engine

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-3.10+-green)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen)
![Tests](https://img.shields.io/badge/Tests-50%2B%20Passing-success)

*A professional-grade trading engine implementing price-time priority matching, multiple order types, analytics, and benchmarking.*

</div>

---

## 📖 Overview

TradeBook is a production-inspired C++ trading engine that simulates the core matching logic used by modern financial exchanges.

### ✨ Highlights

- 🚀 Limit, Market, IOC and FOK Orders
- ⚖️ Price-Time Priority Matching
- ⚡ O(1) Order Lookup
- 📈 VWAP & Trading Statistics
- 📊 Performance Benchmarking
- 🧪 50+ Unit Tests
- 📁 CSV Export
- 🛠 Modern C++17 Architecture

---
# TradeBook Screenshots
## 1. Welcome Banner
![Welcome Banner](screenshots/01_welcome.txt)

Professional welcome screen showing the trading engine initialization.

---

## 2. Order Book Display
![Order Book](screenshots/02_orderbook.txt)

The order book shows both sides (bids and asks) with price, quantity, and order counts.

---

## 3. Limit Order Execution
![Limit Order](screenshots/03_limit.txt)

Limit order execution with price-time priority matching.

---

## 4. Market Order Execution
![Market Order](screenshots/04_market.txt)

Market order sweeping through multiple price levels for immediate execution.

---

## 5. IOC (Immediate-or-Cancel)
![IOC](screenshots/05_ioc.txt)

IOC order execution showing partial fill and immediate cancellation of remaining quantity.

---

## 6. FOK (Fill-or-Kill)
![FOK](screenshots/06_fok.txt)

FOK order handling - either complete fill or complete cancellation (all-or-nothing).

---

## 7. Statistics Report
![Statistics Report](screenshots/07_statistics.txt)

Comprehensive trading session report with order statistics, trade metrics, volume, and VWAP.

---

## 8. Performance Report
![Performance Report](screenshots/08_performance.txt)

Performance metrics showing execution time and throughput (orders/sec).

---

## 9. Benchmark Results
![Benchmark](screenshots/09_benchmark.txt)

Stress test benchmark showing performance with varying order counts (100, 1000, 5000, 10000 orders).

---

## 10. CSV Export
![CSV Export](screenshots/10_csv.txt)

CSV export of statistics for further analysis and reporting.

---

## 11. Error Handling
![Error Handling](screenshots/11_error_handling.txt)

Professional error messages with clear context for debugging.

---

## 12. Order Modification
![Order Modification](screenshots/12_modification.txt)

Order modification with detailed before/after display showing price and quantity changes.

---

## 13. Order Cancellation
![Order Cancellation](screenshots/13_cancellation.txt)

Order cancellation with confirmation message and status update.

---

## Full Output
![Full Output](screenshots/00_full_output.txt)

Complete program output showing all features in action.

---

## Architecture Diagrams

### High-Level Architecture
![Architecture Diagram](screenshots/Architecture_Diagram_Code.png)

Complete system architecture showing all components and their interactions.

### Data Structure Flow
![Data Structure Flow](screenshots/Data_Structure_Flow_Diagram.png)

How orders flow through the data structures.

### Matching Algorithm
![Matching Algorithm](screenshots/Matching_Algorithm_Flow_Diagram.png)

The core matching algorithm flow chart.

### Trade Execution Lifecycle
![Trade Execution Lifecycle](screenshots/Trade_Execution_Lifecycle.png)

Order lifecycle from creation to execution or cancellation.

### Performance Comparison
![Performance Comparison](screenshots/Performance_Comparison_Diagram.png)

Performance comparison across different order types.

### Sequence Diagram
![Sequence Diagram](screenshots/Sequence_Diagram.png)

Sequence diagram showing interaction between components.

---

## Screenshot Summary

| # | Screenshot | Description |
|---|------------|-------------|
| 1 | [01_welcome.txt](/docs/screenshots/01_welcome.txt) | Welcome banner |
| 2 | [02_orderbook.txt](/docs/screenshots/02_orderbook.txt) | Order book display |
| 3 | [03_limit.txt](/docs/screenshots/03_limit.txt) | Limit order execution |
| 4 | [04_market.txt](/docs/screenshots/04_market.txt) | Market order execution |
| 5 | [05_ioc.txt](/docs/screenshots/05_ioc.txt) | IOC order handling |
| 6 | [06_fok.txt](/docs/screenshots/06_fok.txt) | FOK order handling |
| 7 | [07_statistics.txt](/docs/screenshots/07_statistics.txt) | Statistics report |
| 8 | [08_performance.txt](/docs/screenshots/08_performance.txt) | Performance report |
| 9 | [09_benchmark.txt](/docs/screenshots/09_benchmark.txt) | Benchmark results |
| 10 | [10_csv.txt](/docs/screenshots/10_csv.txt) | CSV export |
| 11 | [11_error_handling.txt](/docs/screenshots/11_error_handling.txt) | Error handling |
| 12 | [12_modification.txt](/docs/screenshots/12_modification.txt) | Order modification |
| 13 | [13_cancellation.txt](/docs/screenshots/13_cancellation.txt) | Order cancellation |
| 14 | [00_full_output.txt](/docs/screenshots/00_full_output.txt) | Complete output |

---
---
# 🏗 Architecture
![Architecture Diagram Code](/docs/screenshots/Architecture_Diagram_Code.png)
![Matching Algorithm Flow Diagram](/docs/screenshots/Matching_Algorithm_Flow_Diagram.png)
![Data Structure Flow Diagram](/docs/screenshots/Data_Structure_Flow_Diagram.png)
![Trade Execution Lifecycle](/docs/screenshots/Trade_Execution_Lifecycle.png)
![Performance Comparison Diagram](/docs/screenshots/Performance_Comparison_Diagram.png)
![Sequence Diagram (Order Flow)](/docs/screenshots/Performance_Comparison_Diagram.png)

---

# 📂 Project Structure

```text
TradeBook/
├── benchmarks/         
├── docs/               
│   ├── Architecture.md
│   ├── Complexity.md
│   ├── Design.md
│   ├── FutureImprovements.md
│   ├── screenshots.md
│   └── screenshots/    
├── exports/            
├── include/            
│   ├── Constants.h
│   ├── EngineStatistics.h
│   ├── Order.h
│   ├── OrderBook.h
│   ├── OrderType.h
│   ├── Side.h
│   ├── Trade.h
│   ├── TradeEngine.h
│   └── TradeHistory.h
├── src/              
│   ├── EngineStatistics.cpp
│   ├── main.cpp
│   ├── Order.cpp
│   ├── OrderBook.cpp
│   ├── Trade.cpp
│   ├── TradeEngine.cpp
│   └── TradeHistory.cpp
├── tests/             
│   ├── test_lifecycle.cpp
│   ├── test_matching.cpp
│   ├── test_order.cpp
│   ├── test_orderbook.cpp
│   └── test_skeleton.cpp
├── .gitignore
├── build.bat          
├── build.sh          
├── CMakeLists.txt     
├── LICENSE           
└── README.md         
```

---

# ⚙ Data Structures

| Component | STL Container | Complexity |
|-----------|---------------|-----------:|
| Price Levels | std::map | O(log n) |
| FIFO Queue | std::list | O(1) |
| Order Lookup | std::unordered_map | O(1) |
| Trade History | std::vector | O(1) append |

---

# 📊 Performance

| Operation | Complexity |
|-----------|-----------:|
| Add Order | O(log n) |
| Cancel Order | O(1) |
| Modify Order | O(log n) |
| Best Bid/Ask | O(1) |
| Match Order | O(k log n) |

---

# 🚀 Features

- ✅ Limit Orders
- ✅ Market Orders
- ✅ IOC Orders
- ✅ FOK Orders
- ✅ Partial Fills
- ✅ Order Modification
- ✅ Order Cancellation
- ✅ Price-Time Priority
- ✅ VWAP
- ✅ Engine Statistics
- ✅ Benchmarking
- ✅ CSV Export
- ✅ Professional Logging
- ✅ Memory Optimized

---

# 📸 Sample Output

```text
====================================
TRADE EXECUTED

Buyer : 4
Seller: 1
Price : 105
Qty   : 30

Remaining Buy Qty : 70

====================================
```

---

# 🛠 Build

```bash
git clone https://github.com/yourusername/TradeBook.git
cd TradeBook
mkdir build
cd build
cmake ..
cmake --build .
```

Run:

```bash
./TradeBook
```

---

# 🔮 Future Work

- Multi-threaded Matching
- FIX Protocol
- WebSocket API
- REST API
- Persistent Storage
- Risk Management
- Iceberg Orders
- Stop Orders

---

# 👨‍💻 Author

Built as a 10-day systems programming project to demonstrate:

- Object-Oriented Design
- Data Structures
- Algorithms
- Performance Engineering
- Modern C++17
- Software Architecture

---

<div align="center">

**⭐ If you like this project, consider starring the repository!**

*"The best way to understand a system is to build it."*

</div>
