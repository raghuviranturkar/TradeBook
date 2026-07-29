#pragma once

#include <string>
#include <chrono>

namespace TradingConstants {
    // Order Limits
    constexpr int MAX_ORDER_ID = 2147483647;
    constexpr int MIN_ORDER_QUANTITY = 1;
    constexpr int MAX_ORDER_QUANTITY = 1000000;
    constexpr double MIN_PRICE = 0.0001;
    constexpr double MAX_PRICE = 1000000.0;
    
    // Display
    constexpr int PRICE_PRECISION = 2;
    constexpr int QUANTITY_PRECISION = 0;
    
    // Default Values
    constexpr int DEFAULT_TRADE_ID = 1;
    constexpr int DEFAULT_ORDER_ID = 1;
    
    // String Constants
    const std::string APP_NAME = "TradeBook Trading Engine";
    const std::string APP_VERSION = "1.0.0";
}

namespace Validation {
    inline bool isValidPrice(double price) {
        return price >= TradingConstants::MIN_PRICE && 
               price <= TradingConstants::MAX_PRICE;
    }
    
    inline bool isValidQuantity(int quantity) {
        return quantity >= TradingConstants::MIN_ORDER_QUANTITY && 
               quantity <= TradingConstants::MAX_ORDER_QUANTITY;
    }
    
    inline bool isValidOrderId(int orderId) {
        return orderId > 0 && orderId <= TradingConstants::MAX_ORDER_ID;
    }
}
