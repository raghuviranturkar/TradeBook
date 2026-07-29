#pragma once

#include <vector>
#include <string>
#include "Order.h"
#include "Trade.h"
#include "TradeHistory.h"

class OrderBook;

/**
 * @brief Matching engine supporting all order types
 */
class TradeEngine {
public:
    TradeEngine(OrderBook& orderBook, TradeHistory& tradeHistory);
    
    // Main entry point
    bool processOrder(Order& order);
    bool matchOrder(Order& order);
    size_t getTradeCount() const;
    
private:
    OrderBook& orderBook_;
    TradeHistory& tradeHistory_;
    int nextTradeId_;
    
    // Core matching functions
    bool matchLimitOrder(Order& order);
    bool matchMarketOrder(Order& order);
    bool matchIOC(Order& order);
    bool matchFOK(Order& order);
    
    // Helper functions
    bool matchBuyOrder(Order& buyOrder);
    bool matchSellOrder(Order& sellOrder);
    bool matchBuyMarket(Order& buyOrder);
    bool matchSellMarket(Order& sellOrder);
    
    bool findBestMatchForBuy(const Order& buyOrder, int& outOrderId, double& outPrice) const;
    bool findBestMatchForSell(const Order& sellOrder, int& outOrderId, double& outPrice) const;
    
    Trade executeTrade(Order& buy, Order& sell, int quantity, double price);
    
    // FOK helpers
    bool checkFOKLiquidity(const Order& order, int& availableQuantity);
    bool canFulfillFOK(const Order& order);
    
    // Market order helpers
    void cancelMarketRemaining(Order& order);
    void printMarketResult(const Order& order, int totalFilled);
    
    // IOC helpers
    void processIOC(Order& order);
    
    // Shared matching logic
    bool matchOrderAgainstBook(Order& order);
    
    // Print helpers
    void printHeader(const std::string& type, const Order& order);
    void printFooter(const Order& order);
};
