# TradeBook Architecture Guide

## System Overview

TradeBook is a high-performance C++ trading engine implementing professional-grade matching logic. This document explains the system architecture and design decisions.

## High-Level Architecture
┌─────────────────────────────────────────────────────────────────────────────┐
│ main.cpp │
│ (Demo & CLI Runner) │
└───────────────────────────────────┬─────────────────────────────────────────┘
│
▼
┌─────────────────────────────────────────────────────────────────────────────┐
│ TradeEngine │
│ ┌─────────────────────────────────────────────────────────────────────┐ │
│ │ processOrder(Order& order) │ │
│ │ ├─ matchLimitOrder() ──► matchOrderAgainstBook() │ │
│ │ ├─ matchMarketOrder() ──► matchOrderAgainstBook() │ │
│ │ ├─ matchIOC() ──► matchOrderAgainstBook() + cancel │ │
│ │ └─ matchFOK() ──► checkLiquidity() + match │ │
│ └─────────────────────────────────────────────────────────────────────┘ │
└───────────────────────────────────┬─────────────────────────────────────────┘
│
▼
┌─────────────────────────────────────────────────────────────────────────────┐
│ OrderBook │
│ ┌─────────────────┐ ┌─────────────────┐ ┌──────────────────────────┐ │
│ │ Bid Book │ │ Ask Book │ │ Lookup Table │ │
│ │ (BUY Orders) │ │ (SELL Orders) │ │ unordered_map<ID,Order> │ │
│ │ │ │ │ │ unordered_map<ID,iter> │ │
│ │ Price 105: [A,B]│ │ Price 106: [C] │ └──────────────────────────┘ │
│ │ Price 104: [D] │ │ Price 107: [E] │ │
│ └─────────────────┘ └─────────────────┘ │
└─────────────────────────────────────────────────────────────────────────────┘
│
┌───────────────┴───────────────┐
│ │
▼ ▼
┌──────────────────────────────────┐ ┌──────────────────────────────────────┐
│ TradeHistory │ │ EngineStatistics │
│ ┌────────────────────────────┐ │ │ ┌──────────────────────────────┐ │
│ │ std::vector<Trade> │ │ │ │ Order Counts │ │
│ │ CSV Export │ │ │ │ Volume & Value │ │
│ │ Trade Summary │ │ │ │ VWAP Calculation │ │
│ └────────────────────────────┘ │ │ │ Performance Metrics │ │
└──────────────────────────────────┘ └──────────────────────────────────────┘

text

## Component Responsibilities

### TradeEngine
- **Primary Role**: Coordinate order matching
- **Key Methods**:
  - `processOrder()` - Main entry point
  - `matchOrderAgainstBook()` - Core matching logic
  - `matchLimitOrder()` - Limit order handling
  - `matchMarketOrder()` - Market order handling
  - `matchIOC()` - IOC handling with cancellation
  - `matchFOK()` - FOK handling with liquidity check

### OrderBook
- **Primary Role**: Store and manage orders
- **Data Structures**:
  - `bids_`: std::map with greater comparator (highest first)
  - `asks_`: std::map (lowest first)
  - `orders_`: unordered_map for O(1) lookup
  - `bidOrderIterators_`: Fast cancellation

### TradeHistory
- **Primary Role**: Track executed trades
- **Features**:
  - Append-only trade storage
  - Trade querying by order ID
  - Summary statistics

### EngineStatistics
- **Primary Role**: Analytics and reporting
- **Features**:
  - Real-time metric collection
  - VWAP calculation
  - Performance benchmarking
  - CSV export

## Data Flow

### Adding a Limit Order
User creates Order

TradeEngine.processOrder() called

matchLimitOrder() invoked

matchOrderAgainstBook() matches against opposite side

Remaining quantity stored in OrderBook

Statistics updated

Trade generated (if any)

text

### Matching Process
Find best opposite-side order

Check if prices cross

Execute trade at resting order price

Update both orders

Remove filled orders

Repeat until order filled or no matches

Store remaining quantity (except IOC/Market)

text

### Cancellation Process
User requests cancellation by OrderId

O(1) lookup in unordered_map

Find order's price level and list iterator

Erase from list

Clean up empty price levels

Update status to CANCELLED

Update statistics

text

## Key Design Patterns

### Strategy Pattern
Different order types (Limit, Market, IOC, FOK) use the same matching engine but with different strategies for handling remaining quantity.

### Observer Pattern
TradeHistory observes trades and updates statistics.

### Factory Pattern
Orders are created through a constructor with validation.

## Data Structure Rationale

| Structure | Why Chosen |
|-----------|------------|
| `std::map` | Automatic sorting for price-time priority |
| `std::list` | FIFO preservation and O(1) insertion/deletion |
| `std::unordered_map` | O(1) average lookup for fast operations |

## Performance Considerations

### Optimizations
- **O(1) Cancellation**: Using stored iterators
- **O(1) Best Bid/Ask**: Using map's first element
- **Const References**: Avoiding unnecessary copies
- **Move Semantics**: Where ownership transfers

### Bottlenecks
1. **Matching multiple price levels** (sweeping)
2. **Memory allocation** for new orders
3. **Price level lookup** (though O(log n) is fast)

## Scalability Analysis

### Current Capacity
- Thousands of orders processed efficiently
- All operations O(log n) or better
- Memory footprint: ~100 bytes per order

### Scaling to Production
Would need:
- Thread safety (mutexes / lock-free)
- Network layer (socket / FIX protocol)
- Persistence (database / file)
- Logging (structured logs)
- Monitoring (metrics / alerts)
- Rate limiting
- Order validation

## Testing Strategy

### Unit Tests
- Each component tested in isolation
- Edge cases covered
- Error conditions tested

### Integration Tests
- End-to-end scenarios
- Multiple order types
- Complex matching scenarios

### Performance Tests
- Benchmark with varying order counts
- Throughput measurement
- Memory usage profiling

## Build System

### CMake Configuration
- Cross-platform (Linux, macOS, Windows)
- C++17 standard
- Warning flags enabled
- Separate test executables

### Dependencies
- C++17 Standard Library only
- No external dependencies

## Security Considerations

While this is a learning project, production systems would need:
- Input validation (already present)
- Rate limiting
- Authentication
- Authorization
- Audit logging
- Data encryption

## Future Architecture Evolution

### Phase 2: Multi-threading
// Thread pool for parallel matching
class ThreadPool {
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
};
Phase 3: Network Layer
// FIX Protocol integration
class FIXEngine {
    void onMessage(const FIXMessage& msg);
    void sendExecution(const Execution& exec);
};
Phase 4: Persistence
// Database-backed order storage
class OrderRepository {
    void save(const Order& order);
    Order load(int orderId);
    void update(const Order& order);
};
