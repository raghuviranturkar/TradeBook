#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Order.h"
#include "Trade.h"

/**
 * @brief Statistics and analytics for the trading engine
 * 
 * Tracks all metrics including orders, trades, volume, and performance.
 * Provides professional reporting and CSV export capabilities.
 */
class EngineStatistics {
public:
    EngineStatistics();
    
    // Record order events
    void recordOrderReceived(const Order& order);
    void recordOrderFilled(const Order& order);
    void recordOrderPartiallyFilled(const Order& order);
    void recordOrderCancelled(const Order& order);
    void recordOrderModified(const Order& order);
    void recordOrderRejected(const Order& order, const std::string& reason);
    
    // Record trade events
    void recordTrade(const Trade& trade);
    
    // Performance timing
    void startTiming();
    void stopTiming();
    void recordOperation(const std::string& operation, long long microseconds);
    
    // Getters
    int getTotalOrders() const;
    int getTotalTrades() const;
    int getTotalVolume() const;
    double getTotalValue() const;
    double getVWAP() const;
    double getAverageTradeSize() const;
    int getLargestTrade() const;
    int getSmallestTrade() const;
    long long getTotalExecutionTime() const;
    double getOrdersPerSecond() const;
    
    // Reports
    std::string generateReport() const;
    std::string generatePerformanceReport() const;
    std::string generateSummary() const;
    void printReport() const;
    void printPerformanceReport() const;
    void printSummary() const;
    
    // CSV Export
    bool exportToCSV(const std::string& filename) const;
    std::string toCSV() const;
    
private:
    // Order counters
    int totalOrdersReceived_;
    int totalBuyOrders_;
    int totalSellOrders_;
    int totalLimitOrders_;
    int totalMarketOrders_;
    int totalIOCOrders_;
    int totalFOKOrders_;
    
    // Execution metrics
    int tradesExecuted_;
    int ordersFilled_;
    int ordersPartiallyFilled_;
    int ordersCancelled_;
    int ordersModified_;
    int ordersRejected_;
    
    // Volume and value
    int totalVolume_;
    double totalValue_;
    int largestTrade_;
    int smallestTrade_;
    int totalTrades_;
    
    // Performance
    std::chrono::high_resolution_clock::time_point startTime_;
    std::chrono::high_resolution_clock::time_point endTime_;
    long long totalExecutionTimeMicros_;
    std::vector<std::pair<std::string, long long>> operationTimes_;
    
    // Helpers
    void updateTradeStats(const Trade& trade);
    void reset();
    std::string formatNumber(int n) const;
    std::string formatDouble(double d) const;
};
