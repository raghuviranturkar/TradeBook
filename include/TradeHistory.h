#pragma once

#include <vector>
#include <string>
#include "Trade.h"

/**
 * @brief Maintains the history of all executed trades
 */
class TradeHistory {
public:
    TradeHistory() = default;
    
    void addTrade(const Trade& trade);
    void clear();
    
    std::vector<Trade> getTrades() const;
    int getTradeCount() const;
    int getTotalVolume() const;
    double getAveragePrice() const;
    
    // Display
    void printTrades() const;
    void printSummary() const;
    std::string toString() const;
    
private:
    std::vector<Trade> trades_;
    mutable bool statsDirty_ = true;
    mutable int totalVolume_ = 0;
    mutable double totalValue_ = 0.0;
    
    void updateStatistics() const;
};
