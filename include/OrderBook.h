#pragma once

#include <map>
#include <list>
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>
#include "Order.h"
#include "Trade.h"
#include "TradeHistory.h"

class TradeEngine;

struct PriceLevel {
    double price;
    int totalQuantity;
    int orderCount;
    
    PriceLevel(double p, int q, int c) : price(p), totalQuantity(q), orderCount(c) {}
};

struct OrderBookSnapshot {
    std::vector<PriceLevel> bids;
    std::vector<PriceLevel> asks;
    std::chrono::system_clock::time_point timestamp;
};

/**
 * @brief Main order book with fast lookup table for order management
 */
class OrderBook {
public:
    OrderBook();
    ~OrderBook();
    
    // Core operations
    bool addOrder(const Order& order);
    bool removeOrder(int orderId);
    bool cancelOrder(int orderId);
    bool modifyOrder(int orderId, double newPrice, int newQuantity, bool keepPriority = false);
    
    // Query methods
    double getBestBid() const;
    double getBestAsk() const;
    double getBestBidPrice() const;
    double getBestAskPrice() const;
    int getVolumeAtPrice(double price, bool isBid) const;
    size_t getOrderCount() const;
    bool isEmpty() const;
    bool isBidEmpty() const;
    bool isAskEmpty() const;
    
    // Order lookup (O(1) using unordered_map)
    bool hasOrder(int orderId) const;
    const Order* getOrder(int orderId) const;
    Order* getMutableOrder(int orderId);
    
    // Price level queries
    std::vector<PriceLevel> getBidLevels(int maxLevels = 10) const;
    std::vector<PriceLevel> getAskLevels(int maxLevels = 10) const;
    
    // Display
    void printBook() const;
    std::string toString() const;
    void printPriceLevels(int maxLevels = 10) const;
    
    // Internal data access (for TradeEngine)
    const std::map<double, std::list<Order>, std::greater<double>>& getBids() const;
    const std::map<double, std::list<Order>>& getAsks() const;
    const TradeHistory& getTradeHistory() const;
    TradeHistory& getMutableTradeHistory();
    
    OrderBookSnapshot getSnapshot() const;
    
private:
    // Price level storage
    std::map<double, std::list<Order>, std::greater<double>> bids_;
    std::map<double, std::list<Order>> asks_;
    
    // Fast lookup by order ID (O(1))
    std::unordered_map<int, typename std::list<Order>::iterator> bidOrderIterators_;
    std::unordered_map<int, typename std::list<Order>::iterator> askOrderIterators_;
    std::unordered_map<int, Order> orders_;  // Primary storage for fast access
    
    TradeHistory tradeHistory_;
    int nextOrderId_;
    
    // Helper methods
    bool validateOrder(const Order& order) const;
    bool addBidOrder(const Order& order);
    bool addAskOrder(const Order& order);
    bool removeBidOrder(int orderId);
    bool removeAskOrder(int orderId);
    void cleanupEmptyPriceLevels();
    
    friend class TradeEngine;
};

void printPriceLevel(const std::pair<double, std::list<Order>>& level, bool isBid);
