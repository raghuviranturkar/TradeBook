#include <iostream>
#include "include/Order.h"
#include "include/Trade.h"
#include "include/TradeHistory.h"

int main() {
    std::cout << "Testing core classes...\n\n";
    
    // Create an order
    Order order(1001, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.50, 200);
    order.print();
    
    // Create a trade
    Trade trade(1, 1001, 2001, 105.50, 50);
    trade.print();
    
    // Create history
    TradeHistory history;
    history.addTrade(trade);
    history.printSummary();
    
    return 0;
}
