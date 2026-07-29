#include <iostream>
#include "Side.h"
#include "OrderType.h"
#include "Order.h"
#include "Trade.h"
#include "TradeHistory.h"

/**
 * @brief Simple test to verify that the skeleton compiles
 */
int main() {
    std::cout << "Running skeleton tests...\n";
    
    try {
        // Test Order creation
        Order order(1, Side::BUY, 100.50, 1000);
        std::cout << "✓ Created order: " << order.toString() << "\n";
        
        // Test Trade creation
        Trade trade(1, 1, 2, 100.50, 500);
        std::cout << "✓ Created trade: " << trade.toString() << "\n";
        
        // Test TradeHistory
        TradeHistory history;
        history.addTrade(trade);
        std::cout << "✓ Added trade to history\n";
        std::cout << "  Total trades: " << history.getTradeCount() << "\n";
        std::cout << "  Total volume: " << history.getTotalVolume() << "\n";
        
        std::cout << "\n✓ All tests passed!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed: " << e.what() << "\n";
        return 1;
    }
}