#include <iostream>
#include <chrono>
#include "Order.h"

int main() {
    Order order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50);
    
    auto t1 = order.getTimestamp();
    std::cout << "Original timestamp: " << t1.time_since_epoch().count() << "\n";
    
    order.modify(105.00, 50, true);
    auto t2 = order.getTimestamp();
    std::cout << "After modify (keepPriority=true): " << t2.time_since_epoch().count() << "\n";
    std::cout << "Same timestamp? " << (t1 == t2 ? "YES" : "NO") << "\n";
    
    return 0;
}
