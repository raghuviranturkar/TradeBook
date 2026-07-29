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

```text
                    +------------------+
                    |   Trade Engine   |
                    +---------+--------+
                              |
      +-----------+-----------+------------+
      |           |                        |
+-----------+ +-----------+      +------------------+
| OrderBook | |Statistics |      |  Trade History   |
+-----+-----+ +-----+-----+      +---------+--------+
      |             |                       |
+-----------+   +---------+           +-----------+
| Bid / Ask |   | Reports |           | CSV Export|
+-----------+   +---------+           +-----------+
```

---

# 📂 Project Structure

```text
TradeBook/
├── include/
├── src/
├── tests/
├── benchmarks/
├── docs/
├── exports/
├── README.md
├── CMakeLists.txt
├── LICENSE
└── .gitignore
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
