# Design Decisions - Day 1

## Data Structure Choices

### 1. Price Levels: `std::map<double, std::queue<int>>`

**Why map?**
- Need automatic sorting by price for price-time priority
- O(log n) insertion and lookup
- Bidirectional iteration for best price finding

**Why queue at each price level?**
- FIFO for time priority
- O(1) insertion and removal
- No random access needed

### 2. Order Storage: `std::unordered_map<int, Order>`

**Why unordered_map?**
- O(1) lookup by order ID for cancellations/modifications
- No ordering requirement for order IDs
- Efficient memory usage

### 3. Trade History: `std::vector<Trade>`

**Why vector?**
- Sequential access for displaying history
- Cache-friendly
- Supports efficient appending

## Class Responsibilities

| Class | Responsibility |
|-------|---------------|
| Order | Represents a single order with all attributes |
| Trade | Represents an executed trade |
| OrderBook | Manages bid/ask books and coordinates trading |
| TradeEngine | Implements matching algorithm |
| TradeHistory | Stores and queries trade history |

## Algorithm Decisions

### Price-Time Priority
1. Price priority: Higher bids/lower asks first
2. Time priority: Earlier orders at same price first
3. Implemented using map for price and queues for time

### Matching Algorithm
1. Find best opposite order
2. Check if prices cross
3. Execute trade for minimum quantity
4. Update both orders
5. Remove filled orders
6. Repeat