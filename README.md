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
# 🏗 Architecture
### Data Structure Flow
![Architecture Diagram Code](/docs/screenshots/Architecture_Diagram_Code.png)
### Matching Algorithm
![Matching Algorithm Flow Diagram](/docs/screenshots/Matching_Algorithm_Flow_Diagram.png)
### Data Structure Flow
![Data Structure Flow Diagram](/docs/screenshots/Data_Structure_Flow_Diagram.png)
### Trade Execution Lifecycle
![Trade Execution Lifecycle](/docs/screenshots/Trade_Execution_Lifecycle.png)
### Performance Comparison
![Sequence Diagram (Order Flow)](/docs/screenshots/Sequence_Diagram.png)
### Sequence Diagram
![Performance Comparison Diagram](/docs/screenshots/Performance_Comparison_Diagram.png)
---
## 🖼️ Demo

Below is an example of the order book displaying both buy (bids) and sell (asks), followed by a successful trade execution.

```text
========================================
           ORDER BOOK
========================================

  SELL ORDERS (Asks)
  ----------------------------------------
  Price     Qty     Orders    Level
  ----------------------------------------
    106.50      60       1      1
    107.00      50       2      2
    108.00      40       1      3
  ----------------------------------------

  BUY ORDERS (Bids)
  ----------------------------------------
  Price     Qty     Orders    Level
  ----------------------------------------
    106.00      75       1      1
    105.50     125       2      2
    104.00      50       1      3
  ----------------------------------------

  Best Bid     : 106.00
  Best Ask     : 106.50
  Total Orders : 8
========================================

========================================
        TRADE EXECUTED #1
========================================
  Buyer ID  : 4
  Seller ID : 1
  Price     : 105.00
  Quantity  : 30
  Value     : 3150.00
========================================
  Buy Order 4 remaining  : 70
  Sell Order 1 fully filled
========================================
```

### What this demonstrates

- Displays the **sell side (asks)** sorted from the lowest to highest price.
- Displays the **buy side (bids)** sorted from the highest to lowest price.
- Maintains **price-time priority** for order matching.
- Shows the **Best Bid** and **Best Ask** prices.
- Displays the total number of active orders.
- Prints detailed trade execution information, including:
  - Buyer and Seller IDs
  - Execution Price
  - Executed Quantity
  - Total Trade Value
  - Remaining quantity after partial fills
  - Notification when an order is completely filled
---
# TradeBook Screenshots
## 1. [Welcome Banner](/docs/screenshots/01_welcome.txt)

Professional welcome screen showing the trading engine initialization.

---

## 2. [Order Book Display](/docs/screenshots/02_orderbook.txt)

The order book shows both sides (bids and asks) with price, quantity, and order counts.

---

## 3. [Limit Order Execution](/docs/screenshots/03_limit.txt)

Limit order execution with price-time priority matching.

---

## 4. [Market Order Execution](/docs/screenshots/04_market.txt)

Market order sweeping through multiple price levels for immediate execution.

---

## 5. [IOC (Immediate-or-Cancel)](/docs/screenshots/05_ioc.txt)

IOC order execution showing partial fill and immediate cancellation of remaining quantity.

---

## 6. [FOK (Fill-or-Kill)](/docs/screenshots/06_fok.txt)

FOK order handling - either complete fill or complete cancellation (all-or-nothing).

---

## 7. [Statistics Report](/docs/screenshots/07_statistics.txt)

Comprehensive trading session report with order statistics, trade metrics, volume, and VWAP.

---

## 8. [Performance Report](/docs/screenshots/08_performance.txt)

Performance metrics showing execution time and throughput (orders/sec).

---

## 9. [Benchmark Results](/docs/screenshots/09_benchmark.txt)

Stress test benchmark showing performance with varying order counts (100, 1000, 5000, 10000 orders).

---

## 10. [CSV Export](/docs/screenshots/10_csv.txt)

CSV export of statistics for further analysis and reporting.

---

## 11. [Error Handling](/docs/screenshots/11_error_handling.txt)

Professional error messages with clear context for debugging.

---

## 12. [Order Modification](/docs/screenshots/12_modification.txt)

Order modification with detailed before/after display showing price and quantity changes.

---

## 13. [Order Cancellation](/docs/screenshots/13_cancellation.txt)

Order cancellation with confirmation message and status update.

---

## [Full Output](/docs/screenshots/00_full_output.txt)

Complete program output showing all features in action.

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
