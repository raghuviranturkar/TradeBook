#include "TradeEngine.h"
#include "OrderBook.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <map>
#include <list>

TradeEngine::TradeEngine(OrderBook& orderBook, TradeHistory& tradeHistory)
    : orderBook_(orderBook)
    , tradeHistory_(tradeHistory)
    , nextTradeId_(1) {
}

bool TradeEngine::processOrder(Order& order) {
    if (!order.isActive()) {
        return false;
    }
    
    bool result = false;
    
    // Route to appropriate handler based on order type
    switch (order.getType()) {
        case OrderType::LIMIT:
            result = matchLimitOrder(order);
            break;
        case OrderType::MARKET:
            result = matchMarketOrder(order);
            break;
        default:
            std::cerr << "Unknown order type\n";
            return false;
    }
    
    return result;
}

bool TradeEngine::matchLimitOrder(Order& order) {
    printHeader("LIMIT ORDER", order);
    
    bool traded = matchOrderAgainstBook(order);
    
    // Store remaining if still active
    if (order.isActive()) {
        orderBook_.addOrder(order);
        std::cout << "  Order stored in book with " 
                  << order.getRemainingQuantity() << " remaining\n";
    }
    
    printFooter(order);
    return traded;
}

bool TradeEngine::matchMarketOrder(Order& order) {
    printHeader("MARKET ORDER", order);
    
    int initialQty = order.getRemainingQuantity();
    bool traded = false;
    
    // Market order doesn't have a price - find best available
    if (order.getSide() == Side::BUY) {
        traded = matchBuyMarket(order);
    } else {
        traded = matchSellMarket(order);
    }
    
    // Market orders are never stored - cancel remaining
    if (order.getRemainingQuantity() > 0) {
        std::cout << "\n  ⚠️  Insufficient liquidity: " 
                  << order.getRemainingQuantity() << " remaining\n";
        order.cancel();
        std::cout << "  ❌ Order cancelled\n";
    }
    
    printFooter(order);
    return traded;
}

bool TradeEngine::matchBuyMarket(Order& buyOrder) {
    bool traded = false;
    
    while (buyOrder.isActive()) {
        int sellOrderId;
        
        // Find the best ask (lowest price)
        const auto& asks = orderBook_.getAsks();
        if (asks.empty()) {
            break;
        }
        
        // Get the lowest ask price level
        bool foundMatch = false;
        for (const auto& priceLevel : asks) {
            for (const auto& order : priceLevel.second) {
                if (order.isActive()) {
                    foundMatch = true;
                    sellOrderId = order.getId();
                    break;
                }
            }
            if (foundMatch) break;
        }
        
        if (!foundMatch) {
            break;
        }
        
        const Order* sellOrderPtr = orderBook_.getOrder(sellOrderId);
        if (!sellOrderPtr) {
            break;
        }
        
        Order sellOrder = *sellOrderPtr;
        int tradeQuantity = std::min(buyOrder.getRemainingQuantity(), sellOrder.getRemainingQuantity());
        
        Trade trade = executeTrade(buyOrder, sellOrder, tradeQuantity, sellOrder.getPrice());
        
        buyOrder.reduceQuantity(tradeQuantity);
        
        Order updatedSellOrder = sellOrder;
        updatedSellOrder.reduceQuantity(tradeQuantity);
        
        orderBook_.removeOrder(sellOrderId);
        if (updatedSellOrder.isActive()) {
            orderBook_.addOrder(updatedSellOrder);
        }
        
        tradeHistory_.addTrade(trade);
        traded = true;
        
        std::cout << "  Matched " << tradeQuantity << " @ " 
                  << std::fixed << std::setprecision(2) << trade.getPrice() << "\n";
    }
    
    return traded;
}

bool TradeEngine::matchSellMarket(Order& sellOrder) {
    bool traded = false;
    
    while (sellOrder.isActive()) {
        int buyOrderId;
        
        // Find the best bid (highest price)
        const auto& bids = orderBook_.getBids();
        if (bids.empty()) {
            break;
        }
        
        // Get the highest bid price level
        bool foundMatch = false;
        for (const auto& priceLevel : bids) {
            for (const auto& order : priceLevel.second) {
                if (order.isActive()) {
                    foundMatch = true;
                    buyOrderId = order.getId();
                    break;
                }
            }
            if (foundMatch) break;
        }
        
        if (!foundMatch) {
            break;
        }
        
        const Order* buyOrderPtr = orderBook_.getOrder(buyOrderId);
        if (!buyOrderPtr) {
            break;
        }
        
        Order buyOrder = *buyOrderPtr;
        int tradeQuantity = std::min(sellOrder.getRemainingQuantity(), buyOrder.getRemainingQuantity());
        
        Trade trade = executeTrade(buyOrder, sellOrder, tradeQuantity, buyOrder.getPrice());
        
        sellOrder.reduceQuantity(tradeQuantity);
        
        Order updatedBuyOrder = buyOrder;
        updatedBuyOrder.reduceQuantity(tradeQuantity);
        
        orderBook_.removeOrder(buyOrderId);
        if (updatedBuyOrder.isActive()) {
            orderBook_.addOrder(updatedBuyOrder);
        }
        
        tradeHistory_.addTrade(trade);
        traded = true;
        
        std::cout << "  Matched " << tradeQuantity << " @ " 
                  << std::fixed << std::setprecision(2) << trade.getPrice() << "\n";
    }
    
    return traded;
}

bool TradeEngine::matchIOC(Order& order) {
    printHeader("IOC ORDER", order);
    
    bool traded = matchOrderAgainstBook(order);
    
    // IOC: Cancel any remaining quantity
    if (order.getRemainingQuantity() > 0) {
        std::cout << "  ⚠️  " << order.getRemainingQuantity() 
                  << " remaining - CANCELLED (IOC)\n";
        order.cancel();
    }
    
    printFooter(order);
    return traded;
}

bool TradeEngine::matchFOK(Order& order) {
    printHeader("FOK ORDER", order);
    
    // First check if we can fulfill the entire order
    if (!canFulfillFOK(order)) {
        std::cout << "  ❌ Insufficient liquidity - Order KILLED\n";
        order.cancel();
        printFooter(order);
        return false;
    }
    
    // Can fulfill - execute the match
    std::cout << "  ✅ Sufficient liquidity found - Executing...\n";
    bool traded = matchOrderAgainstBook(order);
    
    // FOK: Should have no remaining quantity after successful match
    if (order.getRemainingQuantity() > 0) {
        std::cout << "  ⚠️  Unexpected remaining quantity - Cancelling\n";
        order.cancel();
    }
    
    printFooter(order);
    return traded;
}

bool TradeEngine::canFulfillFOK(const Order& order) {
    int available = 0;
    return checkFOKLiquidity(order, available);
}

bool TradeEngine::checkFOKLiquidity(const Order& order, int& availableQuantity) {
    availableQuantity = 0;
    
    if (order.getSide() == Side::BUY) {
        // Check ask side for buy FOK
        const auto& asks = orderBook_.getAsks();
        for (const auto& priceLevel : asks) {
            if (priceLevel.first > order.getPrice()) {
                break;  // Price too high
            }
            for (const auto& bookOrder : priceLevel.second) {
                if (bookOrder.isActive()) {
                    availableQuantity += bookOrder.getRemainingQuantity();
                    if (availableQuantity >= order.getRemainingQuantity()) {
                        return true;
                    }
                }
            }
        }
    } else {
        // Check bid side for sell FOK
        const auto& bids = orderBook_.getBids();
        for (const auto& priceLevel : bids) {
            if (priceLevel.first < order.getPrice()) {
                break;  // Price too low
            }
            for (const auto& bookOrder : priceLevel.second) {
                if (bookOrder.isActive()) {
                    availableQuantity += bookOrder.getRemainingQuantity();
                    if (availableQuantity >= order.getRemainingQuantity()) {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

bool TradeEngine::matchOrderAgainstBook(Order& order) {
    if (order.getSide() == Side::BUY) {
        return matchBuyOrder(order);
    } else {
        return matchSellOrder(order);
    }
}

bool TradeEngine::matchBuyOrder(Order& buyOrder) {
    bool traded = false;
    
    while (buyOrder.isActive()) {
        int sellOrderId;
        double executionPrice;
        
        if (!findBestMatchForBuy(buyOrder, sellOrderId, executionPrice)) {
            break;
        }
        
        const Order* sellOrderPtr = orderBook_.getOrder(sellOrderId);
        if (!sellOrderPtr) {
            break;
        }
        
        Order sellOrder = *sellOrderPtr;
        int tradeQuantity = std::min(buyOrder.getRemainingQuantity(), sellOrder.getRemainingQuantity());
        
        Trade trade = executeTrade(buyOrder, sellOrder, tradeQuantity, sellOrder.getPrice());
        
        buyOrder.reduceQuantity(tradeQuantity);
        
        Order updatedSellOrder = sellOrder;
        updatedSellOrder.reduceQuantity(tradeQuantity);
        
        orderBook_.removeOrder(sellOrderId);
        if (updatedSellOrder.isActive()) {
            orderBook_.addOrder(updatedSellOrder);
        }
        
        tradeHistory_.addTrade(trade);
        traded = true;
        
        std::cout << "  Matched " << tradeQuantity << " @ " 
                  << std::fixed << std::setprecision(2) << trade.getPrice() << "\n";
    }
    
    return traded;
}

bool TradeEngine::matchSellOrder(Order& sellOrder) {
    bool traded = false;
    
    while (sellOrder.isActive()) {
        int buyOrderId;
        double executionPrice;
        
        if (!findBestMatchForSell(sellOrder, buyOrderId, executionPrice)) {
            break;
        }
        
        const Order* buyOrderPtr = orderBook_.getOrder(buyOrderId);
        if (!buyOrderPtr) {
            break;
        }
        
        Order buyOrder = *buyOrderPtr;
        int tradeQuantity = std::min(sellOrder.getRemainingQuantity(), buyOrder.getRemainingQuantity());
        
        Trade trade = executeTrade(buyOrder, sellOrder, tradeQuantity, buyOrder.getPrice());
        
        sellOrder.reduceQuantity(tradeQuantity);
        
        Order updatedBuyOrder = buyOrder;
        updatedBuyOrder.reduceQuantity(tradeQuantity);
        
        orderBook_.removeOrder(buyOrderId);
        if (updatedBuyOrder.isActive()) {
            orderBook_.addOrder(updatedBuyOrder);
        }
        
        tradeHistory_.addTrade(trade);
        traded = true;
        
        std::cout << "  Matched " << tradeQuantity << " @ " 
                  << std::fixed << std::setprecision(2) << trade.getPrice() << "\n";
    }
    
    return traded;
}

bool TradeEngine::findBestMatchForBuy(const Order& buyOrder, int& outOrderId, double& outPrice) const {
    const auto& asks = orderBook_.getAsks();
    
    for (const auto& priceLevel : asks) {
        if (priceLevel.first > buyOrder.getPrice()) {
            break;
        }
        for (const auto& order : priceLevel.second) {
            if (order.isActive()) {
                outOrderId = order.getId();
                outPrice = order.getPrice();
                return true;
            }
        }
    }
    return false;
}

bool TradeEngine::findBestMatchForSell(const Order& sellOrder, int& outOrderId, double& outPrice) const {
    const auto& bids = orderBook_.getBids();
    
    for (const auto& priceLevel : bids) {
        if (priceLevel.first < sellOrder.getPrice()) {
            break;
        }
        for (const auto& order : priceLevel.second) {
            if (order.isActive()) {
                outOrderId = order.getId();
                outPrice = order.getPrice();
                return true;
            }
        }
    }
    return false;
}

Trade TradeEngine::executeTrade(Order& buy, Order& sell, int quantity, double price) {
    Trade trade(nextTradeId_++, buy.getId(), sell.getId(), price, quantity);
    return trade;
}

size_t TradeEngine::getTradeCount() const {
    return tradeHistory_.getTradeCount();
}

bool TradeEngine::matchOrder(Order& order) {
    return processOrder(order);
}

void TradeEngine::processIOC(Order& order) {
    // IOC is handled in matchIOC
    // Nothing additional needed
}

void TradeEngine::cancelMarketRemaining(Order& order) {
    // Handled in matchMarketOrder
}

void TradeEngine::printMarketResult(const Order& order, int totalFilled) {
    // Handled in matchMarketOrder
}

// Print helpers
void TradeEngine::printHeader(const std::string& type, const Order& order) {
    std::cout << "\n========================================\n";
    std::cout << "  " << type << "\n";
    std::cout << "========================================\n";
    std::cout << "  " << sideToString(order.getSide()) 
              << " " << orderTypeToString(order.getType())
              << "  Qty: " << order.getRemainingQuantity();
    if (order.getType() == OrderType::LIMIT) {
        std::cout << "  Price: " << std::fixed << std::setprecision(2) << order.getPrice();
    }
    std::cout << "\n  TIF: " << timeInForceToString(order.getTimeInForce()) << "\n";
    std::cout << "========================================\n";
}

void TradeEngine::printFooter(const Order& order) {
    std::cout << "----------------------------------------\n";
    std::cout << "  Final Status: " << Order::statusToString(order.getStatus()) << "\n";
    if (!order.isActive() && !order.isCancelled()) {
        std::cout << "  Result: COMPLETED\n";
    } else if (order.isCancelled()) {
        std::cout << "  Result: CANCELLED\n";
    } else {
        std::cout << "  Result: PARTIALLY FILLED\n";
    }
    std::cout << "========================================\n";
}
