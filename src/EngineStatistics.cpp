#include "EngineStatistics.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>

EngineStatistics::EngineStatistics()
    : totalOrdersReceived_(0)
    , totalBuyOrders_(0)
    , totalSellOrders_(0)
    , totalLimitOrders_(0)
    , totalMarketOrders_(0)
    , totalIOCOrders_(0)
    , totalFOKOrders_(0)
    , tradesExecuted_(0)
    , ordersFilled_(0)
    , ordersPartiallyFilled_(0)
    , ordersCancelled_(0)
    , ordersModified_(0)
    , ordersRejected_(0)
    , totalVolume_(0)
    , totalValue_(0.0)
    , largestTrade_(0)
    , smallestTrade_(INT_MAX)
    , totalTrades_(0)
    , totalExecutionTimeMicros_(0) {
}

void EngineStatistics::reset() {
    totalOrdersReceived_ = 0;
    totalBuyOrders_ = 0;
    totalSellOrders_ = 0;
    totalLimitOrders_ = 0;
    totalMarketOrders_ = 0;
    totalIOCOrders_ = 0;
    totalFOKOrders_ = 0;
    tradesExecuted_ = 0;
    ordersFilled_ = 0;
    ordersPartiallyFilled_ = 0;
    ordersCancelled_ = 0;
    ordersModified_ = 0;
    ordersRejected_ = 0;
    totalVolume_ = 0;
    totalValue_ = 0.0;
    largestTrade_ = 0;
    smallestTrade_ = INT_MAX;
    totalTrades_ = 0;
    totalExecutionTimeMicros_ = 0;
    operationTimes_.clear();
}

void EngineStatistics::recordOrderReceived(const Order& order) {
    totalOrdersReceived_++;
    
    if (order.getSide() == Side::BUY) {
        totalBuyOrders_++;
    } else {
        totalSellOrders_++;
    }
    
    if (order.getType() == OrderType::LIMIT) {
        totalLimitOrders_++;
    } else {
        totalMarketOrders_++;
    }
    
    if (order.getTimeInForce() == TimeInForce::IOC) {
        totalIOCOrders_++;
    } else if (order.getTimeInForce() == TimeInForce::FOK) {
        totalFOKOrders_++;
    }
}

void EngineStatistics::recordOrderFilled(const Order&) {
    ordersFilled_++;
}

void EngineStatistics::recordOrderPartiallyFilled(const Order&) {
    ordersPartiallyFilled_++;
}

void EngineStatistics::recordOrderCancelled(const Order&) {
    ordersCancelled_++;
}

void EngineStatistics::recordOrderModified(const Order&) {
    ordersModified_++;
}

void EngineStatistics::recordOrderRejected(const Order&, const std::string&) {
    ordersRejected_++;
}

void EngineStatistics::recordTrade(const Trade& trade) {
    tradesExecuted_++;
    totalTrades_++;
    updateTradeStats(trade);
}

void EngineStatistics::updateTradeStats(const Trade& trade) {
    int qty = trade.getQuantity();
    double price = trade.getPrice();
    double value = qty * price;
    
    totalVolume_ += qty;
    totalValue_ += value;
    
    if (qty > largestTrade_) {
        largestTrade_ = qty;
    }
    if (qty < smallestTrade_) {
        smallestTrade_ = qty;
    }
}

void EngineStatistics::startTiming() {
    startTime_ = std::chrono::high_resolution_clock::now();
}

void EngineStatistics::stopTiming() {
    endTime_ = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime_ - startTime_);
    totalExecutionTimeMicros_ = duration.count();
}

void EngineStatistics::recordOperation(const std::string& operation, long long microseconds) {
    operationTimes_.push_back({operation, microseconds});
}

int EngineStatistics::getTotalOrders() const { return totalOrdersReceived_; }
int EngineStatistics::getTotalTrades() const { return totalTrades_; }
int EngineStatistics::getTotalVolume() const { return totalVolume_; }
double EngineStatistics::getTotalValue() const { return totalValue_; }

double EngineStatistics::getVWAP() const {
    if (totalVolume_ == 0) return 0.0;
    return totalValue_ / totalVolume_;
}

double EngineStatistics::getAverageTradeSize() const {
    if (totalTrades_ == 0) return 0.0;
    return static_cast<double>(totalVolume_) / totalTrades_;
}

int EngineStatistics::getLargestTrade() const { return largestTrade_; }

int EngineStatistics::getSmallestTrade() const { 
    return (smallestTrade_ == INT_MAX) ? 0 : smallestTrade_; 
}

long long EngineStatistics::getTotalExecutionTime() const {
    return totalExecutionTimeMicros_;
}

double EngineStatistics::getOrdersPerSecond() const {
    if (totalExecutionTimeMicros_ == 0) return 0.0;
    return (static_cast<double>(totalOrdersReceived_) / totalExecutionTimeMicros_) * 1000000.0;
}

std::string EngineStatistics::generateReport() const {
    std::ostringstream oss;
    
    oss << "\n========================================\n";
    oss << "      TRADING SESSION REPORT\n";
    oss << "========================================\n\n";
    
    oss << "  ORDERS\n";
    oss << "  ----------------------------------------\n";
    oss << "  Total Orders Received : " << formatNumber(totalOrdersReceived_) << "\n";
    oss << "  Buy Orders            : " << formatNumber(totalBuyOrders_) << "\n";
    oss << "  Sell Orders           : " << formatNumber(totalSellOrders_) << "\n";
    oss << "  Limit Orders          : " << formatNumber(totalLimitOrders_) << "\n";
    oss << "  Market Orders         : " << formatNumber(totalMarketOrders_) << "\n";
    oss << "  IOC Orders            : " << formatNumber(totalIOCOrders_) << "\n";
    oss << "  FOK Orders            : " << formatNumber(totalFOKOrders_) << "\n";
    oss << "  ----------------------------------------\n";
    oss << "  Filled Orders         : " << formatNumber(ordersFilled_) << "\n";
    oss << "  Partially Filled      : " << formatNumber(ordersPartiallyFilled_) << "\n";
    oss << "  Cancelled Orders      : " << formatNumber(ordersCancelled_) << "\n";
    oss << "  Modified Orders       : " << formatNumber(ordersModified_) << "\n";
    oss << "  Rejected Orders       : " << formatNumber(ordersRejected_) << "\n";
    oss << "  ----------------------------------------\n\n";
    
    oss << "  TRADES\n";
    oss << "  ----------------------------------------\n";
    oss << "  Total Trades          : " << formatNumber(totalTrades_) << "\n";
    oss << "  Total Volume          : " << formatNumber(totalVolume_) << "\n";
    oss << "  Total Value           : $" << formatDouble(totalValue_) << "\n";
    oss << "  VWAP                  : $" << formatDouble(getVWAP()) << "\n";
    oss << "  Avg Trade Size        : " << formatDouble(getAverageTradeSize()) << "\n";
    oss << "  Largest Trade         : " << formatNumber(largestTrade_) << "\n";
    oss << "  Smallest Trade        : " << formatNumber(getSmallestTrade()) << "\n";
    oss << "  ----------------------------------------\n\n";
    
    oss << "  PERFORMANCE\n";
    oss << "  ----------------------------------------\n";
    oss << "  Execution Time        : " << totalExecutionTimeMicros_ / 1000.0 << " ms\n";
    oss << "  Orders/sec            : " << formatDouble(getOrdersPerSecond()) << "\n";
    oss << "  ----------------------------------------\n";
    oss << "========================================\n";
    
    return oss.str();
}

std::string EngineStatistics::generatePerformanceReport() const {
    std::ostringstream oss;
    
    oss << "\n========================================\n";
    oss << "      PERFORMANCE REPORT\n";
    oss << "========================================\n\n";
    
    oss << "  Operation Timing\n";
    oss << "  ----------------------------------------\n";
    oss << "  Total Execution   : " << totalExecutionTimeMicros_ / 1000.0 << " ms\n";
    oss << "  Orders/sec        : " << formatDouble(getOrdersPerSecond()) << "\n";
    oss << "  ----------------------------------------\n";
    
    if (!operationTimes_.empty()) {
        oss << "\n  Operation Breakdown\n";
        oss << "  ----------------------------------------\n";
        for (const auto& op : operationTimes_) {
            oss << "  " << std::setw(15) << op.first 
                << " : " << op.second / 1000.0 << " ms\n";
        }
        oss << "  ----------------------------------------\n";
    }
    
    oss << "========================================\n";
    return oss.str();
}

std::string EngineStatistics::generateSummary() const {
    std::ostringstream oss;
    oss << "📊 Orders: " << totalOrdersReceived_ 
        << " | Trades: " << totalTrades_
        << " | Volume: " << totalVolume_
        << " | VWAP: $" << std::fixed << std::setprecision(2) << getVWAP();
    return oss.str();
}

void EngineStatistics::printReport() const {
    std::cout << generateReport();
}

void EngineStatistics::printPerformanceReport() const {
    std::cout << generatePerformanceReport();
}

void EngineStatistics::printSummary() const {
    std::cout << generateSummary() << "\n";
}

bool EngineStatistics::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << toCSV();
    file.close();
    return true;
}

std::string EngineStatistics::toCSV() const {
    std::ostringstream oss;
    
    oss << "Metric,Value\n";
    oss << "Total Orders," << totalOrdersReceived_ << "\n";
    oss << "Buy Orders," << totalBuyOrders_ << "\n";
    oss << "Sell Orders," << totalSellOrders_ << "\n";
    oss << "Limit Orders," << totalLimitOrders_ << "\n";
    oss << "Market Orders," << totalMarketOrders_ << "\n";
    oss << "IOC Orders," << totalIOCOrders_ << "\n";
    oss << "FOK Orders," << totalFOKOrders_ << "\n";
    oss << "Filled Orders," << ordersFilled_ << "\n";
    oss << "Partially Filled," << ordersPartiallyFilled_ << "\n";
    oss << "Cancelled Orders," << ordersCancelled_ << "\n";
    oss << "Modified Orders," << ordersModified_ << "\n";
    oss << "Rejected Orders," << ordersRejected_ << "\n";
    oss << "Total Trades," << totalTrades_ << "\n";
    oss << "Total Volume," << totalVolume_ << "\n";
    oss << "Total Value," << std::fixed << std::setprecision(2) << totalValue_ << "\n";
    oss << "VWAP," << std::fixed << std::setprecision(2) << getVWAP() << "\n";
    oss << "Average Trade Size," << std::fixed << std::setprecision(2) << getAverageTradeSize() << "\n";
    oss << "Largest Trade," << largestTrade_ << "\n";
    oss << "Smallest Trade," << getSmallestTrade() << "\n";
    oss << "Execution Time (ms)," << totalExecutionTimeMicros_ / 1000.0 << "\n";
    oss << "Orders/sec," << std::fixed << std::setprecision(2) << getOrdersPerSecond() << "\n";
    
    return oss.str();
}

std::string EngineStatistics::formatNumber(int n) const {
    std::string s = std::to_string(n);
    int pos = s.length() - 3;
    while (pos > 0) {
        s.insert(pos, ",");
        pos -= 3;
    }
    return s;
}

std::string EngineStatistics::formatDouble(double d) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << d;
    return oss.str();
}
