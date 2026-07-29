#pragma once

#include <chrono>
#include <string>
#include "Constants.h"

using TradeId = int;

/**
 * @brief Represents an executed trade between two orders
 */
class Trade {
public:
    Trade(TradeId id, int buyOrderId, int sellOrderId, 
          double price, int quantity);
    
    // Getters
    TradeId getId() const;
    int getBuyOrderId() const;
    int getSellOrderId() const;
    double getPrice() const;
    int getQuantity() const;
    double getValue() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    
    // Display
    void print() const;
    std::string toString() const;
    
private:
    TradeId id_;
    int buyOrderId_;
    int sellOrderId_;
    double price_;
    int quantity_;
    std::chrono::system_clock::time_point timestamp_;
};
