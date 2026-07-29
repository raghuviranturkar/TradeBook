#pragma once

#include <string>

enum class Side {
    BUY,
    SELL
};

inline std::string sideToString(Side side) {
    switch (side) {
        case Side::BUY:  return "BUY";
        case Side::SELL: return "SELL";
        default:         return "UNKNOWN";
    }
}
