#include "Trade.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Trade::Trade(TradeId id, int buyOrderId, int sellOrderId, 
             double price, int quantity)
    : id_(id)
    , buyOrderId_(buyOrderId)
    , sellOrderId_(sellOrderId)
    , price_(price)
    , quantity_(quantity)
    , timestamp_(std::chrono::system_clock::now()) {
    
    if (quantity_ <= 0) {
        throw std::invalid_argument("Trade quantity must be positive");
    }
    if (price_ <= 0) {
        throw std::invalid_argument("Trade price must be positive");
    }
}

TradeId Trade::getId() const { return id_; }
int Trade::getBuyOrderId() const { return buyOrderId_; }
int Trade::getSellOrderId() const { return sellOrderId_; }
double Trade::getPrice() const { return price_; }
int Trade::getQuantity() const { return quantity_; }
double Trade::getValue() const { return price_ * quantity_; }
std::chrono::system_clock::time_point Trade::getTimestamp() const { return timestamp_; }

void Trade::print() const {
    std::cout << toString() << std::endl;
}

std::string Trade::toString() const {
    std::ostringstream oss;
    
    auto time = std::chrono::system_clock::to_time_t(timestamp_);
    std::string timeStr = std::ctime(&time);
    timeStr.pop_back();
    
    oss << "--------------------------------------------\n";
    oss << "Trade ID   : " << id_ << "\n";
    oss << "Buy Order  : " << buyOrderId_ << "\n";
    oss << "Sell Order : " << sellOrderId_ << "\n";
    oss << "Price      : " << std::fixed << std::setprecision(TradingConstants::PRICE_PRECISION) 
        << price_ << "\n";
    oss << "Quantity   : " << quantity_ << "\n";
    oss << "Value      : " << std::fixed << std::setprecision(TradingConstants::PRICE_PRECISION) 
        << getValue() << "\n";
    oss << "Time       : " << timeStr << "\n";
    oss << "--------------------------------------------";
    
    return oss.str();
}
