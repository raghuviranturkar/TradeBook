#include "OrderBook.h"
#include "TradeEngine.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

OrderBook::OrderBook() : nextOrderId_(1) {}
OrderBook::~OrderBook() = default;

bool OrderBook::addOrder(const Order& order) {
    if (!validateOrder(order)) {
        return false;
    }
    
    Order orderCopy = order;
    
    if (order.getSide() == Side::BUY) {
        return addBidOrder(orderCopy);
    } else {
        return addAskOrder(orderCopy);
    }
}

bool OrderBook::addBidOrder(const Order& order) {
    auto& bidList = bids_[order.getPrice()];
    bidList.push_back(order);
    auto it = bidList.end();
    --it;
    bidOrderIterators_[order.getId()] = it;
    orders_[order.getId()] = order;
    return true;
}

bool OrderBook::addAskOrder(const Order& order) {
    auto& askList = asks_[order.getPrice()];
    askList.push_back(order);
    auto it = askList.end();
    --it;
    askOrderIterators_[order.getId()] = it;
    orders_[order.getId()] = order;
    return true;
}

bool OrderBook::removeOrder(int orderId) {
    if (removeBidOrder(orderId)) {
        return true;
    }
    if (removeAskOrder(orderId)) {
        return true;
    }
    return false;
}

bool OrderBook::removeBidOrder(int orderId) {
    auto it = bidOrderIterators_.find(orderId);
    if (it == bidOrderIterators_.end()) {
        return false;
    }
    
    double price = it->second->getPrice();
    auto& bidList = bids_[price];
    bidList.erase(it->second);
    bidOrderIterators_.erase(orderId);
    orders_.erase(orderId);
    
    if (bidList.empty()) {
        bids_.erase(price);
    }
    return true;
}

bool OrderBook::removeAskOrder(int orderId) {
    auto it = askOrderIterators_.find(orderId);
    if (it == askOrderIterators_.end()) {
        return false;
    }
    
    double price = it->second->getPrice();
    auto& askList = asks_[price];
    askList.erase(it->second);
    askOrderIterators_.erase(orderId);
    orders_.erase(orderId);
    
    if (askList.empty()) {
        asks_.erase(price);
    }
    return true;
}

bool OrderBook::cancelOrder(int orderId) {
    // Find the order in the lookup table
    auto it = orders_.find(orderId);
    if (it == orders_.end()) {
        std::cout << "❌ Order " << orderId << " not found\n";
        return false;
    }
    
    Order& order = it->second;
    
    try {
        // Cancel the order (this will update status)
        order.cancel();
    } catch (const std::logic_error& e) {
        std::cout << "❌ Cannot cancel: " << e.what() << "\n";
        return false;
    }
    
    // Remove from the book
    bool removed = removeOrder(orderId);
    
    if (removed) {
        std::cout << "\n========================================\n";
        std::cout << "  ORDER CANCELLED\n";
        std::cout << "========================================\n";
        std::cout << "  Order ID : " << orderId << "\n";
        std::cout << "  Side     : " << sideToString(order.getSide()) << "\n";
        std::cout << "  Price    : " << std::fixed << std::setprecision(2) << order.getPrice() << "\n";
        std::cout << "  Remaining: " << order.getRemainingQuantity() << "\n";
        std::cout << "  Status   : " << Order::statusToString(order.getStatus()) << "\n";
        std::cout << "========================================\n";
        return true;
    }
    
    return false;
}

bool OrderBook::modifyOrder(int orderId, double newPrice, int newQuantity, bool keepPriority) {
    // Find the order in the lookup table
    auto it = orders_.find(orderId);
    if (it == orders_.end()) {
        std::cout << "❌ Order " << orderId << " not found\n";
        return false;
    }
    
    Order& order = it->second;
    
    // Store old values for comparison
    double oldPrice = order.getPrice();
    int oldQuantity = order.getQuantity();
    
    // Try to modify the order
    if (!order.modify(newPrice, newQuantity, keepPriority)) {
        std::cout << "❌ Order modification failed\n";
        return false;
    }
    
    // If price changed and we're removing from book, need to reinsert
    bool priceChanged = (oldPrice != newPrice);
    bool removed = false;
    
    if (priceChanged) {
        // Remove from current price level
        removed = removeOrder(orderId);
        if (removed) {
            // Add back with new price
            addOrder(order);
        }
    } else if (oldQuantity != newQuantity) {
        // Quantity changed but price same - update in place
        // The order is already updated in orders_, we just need to update the iterator
        if (order.getSide() == Side::BUY) {
            auto bit = bidOrderIterators_.find(orderId);
            if (bit != bidOrderIterators_.end()) {
                *bit->second = order;
            }
        } else {
            auto ait = askOrderIterators_.find(orderId);
            if (ait != askOrderIterators_.end()) {
                *ait->second = order;
            }
        }
    }
    
    std::cout << "\n========================================\n";
    std::cout << "  ORDER MODIFIED\n";
    std::cout << "========================================\n";
    std::cout << "  Order ID : " << orderId << "\n";
    std::cout << "  Side     : " << sideToString(order.getSide()) << "\n";
    std::cout << "  Old Price: " << std::fixed << std::setprecision(2) << oldPrice << "\n";
    std::cout << "  New Price: " << std::fixed << std::setprecision(2) << newPrice << "\n";
    std::cout << "  Old Qty  : " << oldQuantity << "\n";
    std::cout << "  New Qty  : " << newQuantity << "\n";
    std::cout << "  Remaining: " << order.getRemainingQuantity() << "\n";
    std::cout << "  Status   : " << Order::statusToString(order.getStatus()) << "\n";
    if (priceChanged) {
        std::cout << "  Note     : Price changed - order reinserted\n";
    }
    std::cout << "========================================\n";
    
    return true;
}

// Query methods
double OrderBook::getBestBid() const {
    if (bids_.empty()) return 0.0;
    return bids_.begin()->first;
}

double OrderBook::getBestAsk() const {
    if (asks_.empty()) return 0.0;
    return asks_.begin()->first;
}

double OrderBook::getBestBidPrice() const {
    return getBestBid();
}

double OrderBook::getBestAskPrice() const {
    return getBestAsk();
}

int OrderBook::getVolumeAtPrice(double price, bool isBid) const {
    if (isBid) {
        auto it = bids_.find(price);
        if (it == bids_.end()) return 0;
        int total = 0;
        for (const auto& order : it->second) {
            total += order.getRemainingQuantity();
        }
        return total;
    } else {
        auto it = asks_.find(price);
        if (it == asks_.end()) return 0;
        int total = 0;
        for (const auto& order : it->second) {
            total += order.getRemainingQuantity();
        }
        return total;
    }
}

const Order* OrderBook::getOrder(int orderId) const {
    auto it = orders_.find(orderId);
    if (it != orders_.end()) {
        return &it->second;
    }
    return nullptr;
}

Order* OrderBook::getMutableOrder(int orderId) {
    auto it = orders_.find(orderId);
    if (it != orders_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool OrderBook::hasOrder(int orderId) const {
    return orders_.find(orderId) != orders_.end();
}

bool OrderBook::isEmpty() const {
    return bids_.empty() && asks_.empty();
}

bool OrderBook::isBidEmpty() const {
    return bids_.empty();
}

bool OrderBook::isAskEmpty() const {
    return asks_.empty();
}

size_t OrderBook::getOrderCount() const {
    return orders_.size();
}

std::vector<PriceLevel> OrderBook::getBidLevels(int maxLevels) const {
    std::vector<PriceLevel> levels;
    int count = 0;
    for (const auto& level : bids_) {
        if (count >= maxLevels) break;
        int totalQty = 0;
        for (const auto& order : level.second) {
            totalQty += order.getRemainingQuantity();
        }
        levels.emplace_back(level.first, totalQty, static_cast<int>(level.second.size()));
        count++;
    }
    return levels;
}

std::vector<PriceLevel> OrderBook::getAskLevels(int maxLevels) const {
    std::vector<PriceLevel> levels;
    int count = 0;
    for (const auto& level : asks_) {
        if (count >= maxLevels) break;
        int totalQty = 0;
        for (const auto& order : level.second) {
            totalQty += order.getRemainingQuantity();
        }
        levels.emplace_back(level.first, totalQty, static_cast<int>(level.second.size()));
        count++;
    }
    return levels;
}

const std::map<double, std::list<Order>, std::greater<double>>& OrderBook::getBids() const {
    return bids_;
}

const std::map<double, std::list<Order>>& OrderBook::getAsks() const {
    return asks_;
}

const TradeHistory& OrderBook::getTradeHistory() const {
    return tradeHistory_;
}

TradeHistory& OrderBook::getMutableTradeHistory() {
    return tradeHistory_;
}

OrderBookSnapshot OrderBook::getSnapshot() const {
    OrderBookSnapshot snapshot;
    snapshot.timestamp = std::chrono::system_clock::now();
    auto bidLevels = getBidLevels(10);
    for (const auto& level : bidLevels) {
        snapshot.bids.push_back(level);
    }
    auto askLevels = getAskLevels(10);
    for (const auto& level : askLevels) {
        snapshot.asks.push_back(level);
    }
    return snapshot;
}

bool OrderBook::validateOrder(const Order& order) const {
    if (hasOrder(order.getId())) {
        std::cerr << "Error: Order ID " << order.getId() << " already exists\n";
        return false;
    }
    if (!order.isActive()) {
        std::cerr << "Error: Order " << order.getId() << " is not active\n";
        return false;
    }
    return true;
}

void OrderBook::cleanupEmptyPriceLevels() {
    for (auto it = bids_.begin(); it != bids_.end(); ) {
        if (it->second.empty()) {
            it = bids_.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = asks_.begin(); it != asks_.end(); ) {
        if (it->second.empty()) {
            it = asks_.erase(it);
        } else {
            ++it;
        }
    }
}

void OrderBook::printBook() const {
    std::cout << toString();
}

void OrderBook::printPriceLevels(int maxLevels) const {
    std::cout << "\n========================================\n";
    std::cout << "  PRICE LEVELS (Top " << maxLevels << " each side)\n";
    std::cout << "========================================\n\n";
    
    auto bidLevels = getBidLevels(maxLevels);
    auto askLevels = getAskLevels(maxLevels);
    
    std::cout << "  Bid Levels (Highest to Lowest):\n";
    for (const auto& level : bidLevels) {
        std::cout << "    Price: " << std::fixed << std::setprecision(2) << level.price
                  << " | Qty: " << level.totalQuantity 
                  << " | Orders: " << level.orderCount << "\n";
    }
    std::cout << "\n  Ask Levels (Lowest to Highest):\n";
    for (const auto& level : askLevels) {
        std::cout << "    Price: " << std::fixed << std::setprecision(2) << level.price
                  << " | Qty: " << level.totalQuantity 
                  << " | Orders: " << level.orderCount << "\n";
    }
    std::cout << "========================================\n";
}

std::string OrderBook::toString() const {
    std::ostringstream oss;
    oss << "\n========================================\n";
    oss << "           ORDER BOOK\n";
    oss << "========================================\n\n";
    oss << "  SELL ORDERS (Asks)\n";
    oss << "  ----------------------------------------\n";
    oss << "  Price     Qty     Orders\n";
    oss << "  ----------------------------------------\n";
    
    if (asks_.empty()) {
        oss << "  (No sell orders)\n";
    } else {
        for (const auto& priceLevel : asks_) {
            int totalQty = 0;
            for (const auto& order : priceLevel.second) {
                totalQty += order.getRemainingQuantity();
            }
            oss << "  " << std::fixed << std::setprecision(2) 
                << std::setw(8) << priceLevel.first
                << "  " << std::setw(6) << totalQty
                << "  " << std::setw(6) << priceLevel.second.size() << "\n";
        }
    }
    oss << "  ----------------------------------------\n\n";
    oss << "  BUY ORDERS (Bids)\n";
    oss << "  ----------------------------------------\n";
    oss << "  Price     Qty     Orders\n";
    oss << "  ----------------------------------------\n";
    
    if (bids_.empty()) {
        oss << "  (No buy orders)\n";
    } else {
        for (const auto& priceLevel : bids_) {
            int totalQty = 0;
            for (const auto& order : priceLevel.second) {
                totalQty += order.getRemainingQuantity();
            }
            oss << "  " << std::fixed << std::setprecision(2) 
                << std::setw(8) << priceLevel.first
                << "  " << std::setw(6) << totalQty
                << "  " << std::setw(6) << priceLevel.second.size() << "\n";
        }
    }
    oss << "  ----------------------------------------\n\n";
    oss << "  Best Bid  : " << std::fixed << std::setprecision(2) << getBestBid() << "\n";
    oss << "  Best Ask  : " << std::fixed << std::setprecision(2) << getBestAsk() << "\n";
    oss << "  Total Orders : " << getOrderCount() << "\n";
    oss << "========================================\n";
    return oss.str();
}
