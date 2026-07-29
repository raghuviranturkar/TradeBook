# TradeBook Design Decisions

## Overview

This document explains the major design decisions behind **TradeBook**, the reasoning for each choice, alternatives that were considered, and why the final approach was selected.

---

# 1. Data Structure Choices

## `std::map` for Price Levels

**Decision**

Use:

```cpp
std::map<double, std::list<Order>>
```

### Why?

- Automatic price ordering
- O(log n) insertion
- O(log n) lookup
- Bidirectional iteration
- Perfect for price-time priority

### Alternatives

| Alternative | Why Not? |
|------------|----------|
| `std::unordered_map` | No price ordering |
| `std::vector` | Resorting required |
| `std::priority_queue` | Cannot cancel arbitrary orders |

**Winner:** `std::map`

---

## `std::list` for FIFO Queues

- Preserves FIFO
- O(1) insert at back
- O(1) erase using iterator

| Alternative | Issue |
|------------|-------|
| queue | Can't erase middle |
| deque | Middle erase O(n) |
| vector | Expensive insert/erase |

**Winner:** `std::list`

---

## `std::unordered_map` for Lookup

Provides average **O(1)** lookup for:

- Cancellation
- Modification
- Order retrieval

---

# 2. Matching Engine

## Price-Time Priority

1. Better price executes first.
2. Earlier order executes first within the same price.

### Shared Matching Engine

All order types reuse the same matching logic.

```cpp
bool matchOrderAgainstBook(Order& order){
    return order.getSide()==Side::BUY ?
        matchBuyOrder(order):
        matchSellOrder(order);
}
```

Benefits:

- No duplicated logic
- Easier maintenance
- Consistent behavior

---

# 3. Order Lifecycle

```text
NEW
 |
ACTIVE
 |
+----------------------+
|                      |
v                      v
PARTIALLY_FILLED   CANCELLED
 |
v
FILLED
```

```cpp
enum class OrderStatus{
    ACTIVE,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};
```

---

# 4. Advanced Order Types

## Market

- Never stored
- Executes immediately
- Remaining quantity cancelled

## IOC

- Execute immediately
- Cancel remaining

## FOK

- Check liquidity first
- Execute only if complete fill possible
- Otherwise cancel

---

# 5. Performance Optimizations

## Iterator Cache

```cpp
std::unordered_map<OrderId,
    std::list<Order>::iterator>
```

Provides O(1) cancellation.

### Additional Optimizations

- Const correctness
- Pass by const reference
- Move semantics where appropriate

---

# 6. Error Handling

Use validation and exceptions.

```cpp
void validate() const{
    if(!isValid())
        throw std::invalid_argument("Invalid order");
}
```

Example output

```text
ERROR
Cancel failed

Order ID : 1004
Reason   : Order does not exist
```

---

# 7. Statistics

## VWAP

```text
VWAP =
Σ(Price × Quantity)
-------------------
Σ(Quantity)
```

Benchmarks use:

```cpp
std::chrono::high_resolution_clock
```

---

# 8. Testing Strategy

- Order Tests
- Matching Tests
- Lifecycle Tests
- Integration Tests
- Performance Tests

Edge cases include:

- Empty book
- Duplicate IDs
- Invalid prices
- Filled orders
- Cancelled orders

---

# 9. Build System

- C++17
- CMake
- Cross-platform
- Warning flags enabled

```cmake
cmake_minimum_required(VERSION 3.10)
project(TradeBook VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
```

---

# 10. Future Roadmap

- Multi-threading
- FIX Protocol
- Persistence
- REST API
- WebSocket API
- Monitoring
- Risk Management

---

## Summary

The guiding principles behind TradeBook were:

- Correctness
- Performance
- Maintainability
- Extensibility
- Clean Architecture
- Modern C++17
