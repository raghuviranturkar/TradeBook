# TradeBook Design Decisions

## Overview

This document explains the key design decisions made during the development of TradeBook. Each decision includes the reasoning behind it and alternatives considered.

## 1. Data Structure Choices

### Why std::map for Price Levels?

**Decision**: Use `std::map<double, std::list<Order>>` for price levels

**Reason**: 
- Automatic sorting by price
- O(log n) insertion and lookup
- Bidirectional iteration
- Built-in ordering guarantees

**Alternatives Considered**:
- `std::unordered_map`: Would lose price ordering, requiring additional sorting
- `std::vector` + sorting: Would require O(n log n) resorting
- `std::priority_queue`: Cannot access arbitrary elements for cancellation

**Winner**: `std::map` provides the best balance of features.

### Why std::list for FIFO Queues?

**Decision**: Use `std::list<Order>` at each price level

**Reason**:
- Preserves insertion order (FIFO)
- O(1) insertion at back
- O(1) deletion from front
- O(1) deletion from middle (with iterator)

**Alternatives Considered**:
- `std::queue`: Cannot delete from middle for cancellations
- `std::deque`: O(n) deletion from middle
- `std::vector`: O(n) insertion/deletion

**Winner**: `std::list` provides the needed flexibility.

### Why std::unordered_map for Lookup?

**Decision**: Use `std::unordered_map<OrderId, Order>` for fast lookup

**Reason**:
- O(1) average lookup
- Direct access by order ID
- Fast cancellation and modification

**Alternatives Considered**:
- `std::map`: O(log n) lookup, slower
- `std::vector`: O(n) search
- `std::set`: O(log n) lookup

**Winner**: `std::unordered_map` provides the fastest lookup.

## 2. Matching Algorithm Design

### Price-Time Priority

**Decision**: Implement price-time priority matching

**Reason**:
- Industry standard for exchanges
- Fair to all participants
- Prevents gaming

**How it works**:
1. **Price Priority**: Better prices execute first
2. **Time Priority**: Earlier orders at same price execute first

**Implementation**:
- Bids: Highest price first (std::greater)
- Asks: Lowest price first (std::less)
- FIFO: std::list preserves insertion order

### Shared Matching Engine

**Decision**: Single matching engine for all order types

**Reason**:
- No code duplication
- Consistent behavior
- Easier to maintain
- Centralized logic

**Implementation**:
```cpp
bool matchOrderAgainstBook(Order& order) {
    if (order.getSide() == Side::BUY) {
        return matchBuyOrder(order);
    } else {
        return matchSellOrder(order);
    }
}
3. Order Lifecycle Design
Order Status Enum
Decision: Track order status with enum

Reason:

Clear state management

Easy to understand

Prevents invalid transitions

States:

cpp
enum class OrderStatus {
    ACTIVE,              // In book, waiting
    PARTIALLY_FILLED,    // Some filled, still active
    FILLED,              // Completely filled
    CANCELLED            // User cancelled
};
Status Transitions
text
New Order → ACTIVE → PARTIALLY_FILLED → FILLED
                ↓
             CANCELLED
Modification with Priority Control
Decision: Allow keeping or resetting priority

Reason:

Some users want to preserve priority

Price change should generally lose priority

Configurable behavior

Implementation:

cpp
bool modify(Price newPrice, Quantity newQuantity, bool keepPriority = false);
4. Advanced Order Types
Market Order Design
Decision: Market orders never stored in book

Reason:

Must execute immediately

No price limit

Cancel if insufficient liquidity

Implementation:

Find best opposite price

Execute at that price

Cancel remaining (if any)

IOC Design
Decision: IOC orders never stored in book

Reason:

Immediate execution only

Cancel remaining immediately

Implementation:

Match as much as possible

Cancel unfilled portion

FOK Design
Decision: FOK checks liquidity before execution

Reason:

All-or-nothing requirement

Must not partially fill

Check first, then execute

Implementation:

Scan opposite side for total available quantity

If available >= order quantity, execute

If not, cancel entire order

5. Performance Optimizations
Iterator Storage for O(1) Cancellation
Decision: Store iterators in unordered_map

Reason:

O(1) cancellation

No need to search price levels

Fast removal

Implementation:

cpp
std::unordered_map<OrderId, std::list<Order>::iterator> bidOrderIterators_;
std::unordered_map<OrderId, std::list<Order>::iterator> askOrderIterators_;
Const Correctness
Decision: Use const wherever possible

Reason:

Better compiler optimization

Clearer intent

Prevents accidental modification

Example:

cpp
double getBestBid() const;
const Order* getOrder(OrderId id) const;
Pass by Const Reference
Decision: Pass large objects by const reference

Reason:

Avoids unnecessary copies

Better performance

Clear ownership

Example:

cpp
void addOrder(const Order& order);
bool matchOrder(const Order& order);
6. Error Handling Design
Exception Safety
Decision: Use exceptions for invalid operations

Reason:

Clear error reporting

Prevents silent failures

Easy debugging

Implementation:

cpp
void validate() const {
    if (!isValid()) {
        throw std::invalid_argument("Invalid order");
    }
}
Professional Error Messages
Decision: Provide clear, actionable error messages

Reason:

Easier debugging

Professional appearance

Better user experience

Example:

text
ERROR: Cancel failed
Order ID: 1004
Reason: Order does not exist
7. Statistics Design
VWAP Calculation
Decision: Calculate VWAP on demand

Reason:

O(1) when cached

Accurate for reporting

Standard metric

Formula:

text
VWAP = Σ(Price × Quantity) / Σ(Quantity)
Performance Benchmarking
Decision: Use high_resolution_clock

Reason:

Microsecond precision

Cross-platform

Standard library

Implementation:

cpp
auto start = std::chrono::high_resolution_clock::now();
// ... operation ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
8. Testing Design
Unit Testing Strategy
Decision: Test each component in isolation

Reason:

Pinpoints failures

Easy to debug

Fast execution

Test Categories:

Order tests

OrderBook tests

Matching tests

Lifecycle tests

Test Coverage
Decision: Test edge cases extensively

Reason:

Catches bugs early

Ensures robustness

Builds confidence

Edge Cases:

Empty book

Maximum quantity

Duplicate order IDs

Invalid prices

Already filled/cancelled orders

9. Build System Design
Cross-Platform Build
Decision: Use CMake

Reason:

Cross-platform (Windows, Linux, macOS)

Industry standard

IDE integration

Configuration:

text
cmake_minimum_required(VERSION 3.10)
project(TradeBook VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
Build Script
Decision: Provide build.sh for easy building

Reason:

One-command build

Consistent process

Developer friendly

10. Future Architecture
Scalability Design
Decision: Design for future expansion

Reason:

Easier to extend

Anticipates needs

Professional approach

Future Additions:

Multi-threading

Network layer

Persistence

WebSocket API

Monitoring
