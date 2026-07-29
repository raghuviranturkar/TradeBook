#pragma once

#include <string>

enum class OrderType {
    LIMIT,
    MARKET
};

enum class TimeInForce {
    GTC,  // Good 'Til Canceled
    IOC,  // Immediate-or-Cancel
    FOK   // Fill-or-Kill
};

inline std::string orderTypeToString(OrderType type) {
    switch (type) {
        case OrderType::LIMIT:  return "LIMIT";
        case OrderType::MARKET: return "MARKET";
        default:                return "UNKNOWN";
    }
}

inline std::string timeInForceToString(TimeInForce tif) {
    switch (tif) {
        case TimeInForce::GTC:  return "GTC";
        case TimeInForce::IOC:  return "IOC";
        case TimeInForce::FOK:  return "FOK";
        default:                return "UNKNOWN";
    }
}
