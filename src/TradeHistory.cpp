#include "TradeHistory.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void TradeHistory::addTrade(const Trade& trade) {
    trades_.push_back(trade);
    statsDirty_ = true;
}

void TradeHistory::clear() {
    trades_.clear();
    statsDirty_ = true;
    totalVolume_ = 0;
    totalValue_ = 0.0;
}

std::vector<Trade> TradeHistory::getTrades() const {
    return trades_;
}

int TradeHistory::getTradeCount() const {
    return static_cast<int>(trades_.size());
}

int TradeHistory::getTotalVolume() const {
    updateStatistics();
    return totalVolume_;
}

double TradeHistory::getAveragePrice() const {
    updateStatistics();
    if (totalVolume_ == 0) return 0.0;
    return totalValue_ / totalVolume_;
}

void TradeHistory::updateStatistics() const {
    if (!statsDirty_) return;
    
    totalVolume_ = 0;
    totalValue_ = 0.0;
    
    for (const Trade& trade : trades_) {
        totalVolume_ += trade.getQuantity();
        totalValue_ += trade.getValue();
    }
    
    statsDirty_ = false;
}

void TradeHistory::printTrades() const {
    std::cout << toString();
}

void TradeHistory::printSummary() const {
    std::cout << "--------------------------------------------\n";
    std::cout << "Trade History Summary\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Total Trades : " << getTradeCount() << "\n";
    std::cout << "Total Volume : " << getTotalVolume() << "\n";
    std::cout << "Avg Price    : " << std::fixed << std::setprecision(2) 
              << getAveragePrice() << "\n";
    std::cout << "--------------------------------------------\n";
}

std::string TradeHistory::toString() const {
    std::ostringstream oss;
    oss << "\n============================================\n";
    oss << "  TRADE HISTORY (" << trades_.size() << " trades)\n";
    oss << "============================================\n\n";
    
    if (trades_.empty()) {
        oss << "  No trades executed yet.\n";
    } else {
        for (const Trade& trade : trades_) {
            oss << trade.toString() << "\n\n";
        }
    }
    
    oss << "============================================\n";
    oss << "Summary:\n";
    oss << "  Total Trades : " << getTradeCount() << "\n";
    oss << "  Total Volume : " << getTotalVolume() << "\n";
    oss << "  Average Price: " << std::fixed << std::setprecision(2) 
        << getAveragePrice() << "\n";
    oss << "============================================\n";
    
    return oss.str();
}
