#include <iostream>
#include <cassert>
#include "../include/OrderBook.h"
#include "../include/Order.h"

void printTestResult(bool passed, const std::string& testName) {
    if (passed) {
        std::cout << "✅ PASSED: " << testName << "\n";
    } else {
        std::cout << "❌ FAILED: " << testName << "\n";
    }
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  ORDERBOOK UNIT TESTS (DAY 3)\n";
    std::cout << "========================================\n\n";
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Add buy order
    {
        OrderBook book;
        Order order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50);
        bool result = book.addOrder(order);
        printTestResult(result, "Add buy order");
        if (result) passed++;
        total++;
    }
    
    // Test 2: Add sell order
    {
        OrderBook book;
        Order order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50);
        bool result = book.addOrder(order);
        printTestResult(result, "Add sell order");
        if (result) passed++;
        total++;
    }
    
    // Test 3: Multiple buy prices (sorted correctly)
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book.addOrder(Order(3, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 102.00, 20));
        
        bool result = (book.getBestBid() == 105.00);
        printTestResult(result, "Multiple buy prices - best bid correct");
        if (result) passed++;
        total++;
    }
    
    // Test 4: Multiple sell prices (sorted correctly)
    {
        OrderBook book;
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 95.00, 30));
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 98.00, 20));
        
        bool result = (book.getBestAsk() == 95.00);
        printTestResult(result, "Multiple sell prices - best ask correct");
        if (result) passed++;
        total++;
    }
    
    // Test 5: Same price FIFO
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 30));
        book.addOrder(Order(3, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 20));
        
        const auto& bids = book.getBids();
        bool result = false;
        if (!bids.empty()) {
            const auto& orders = bids.begin()->second;
            if (orders.size() == 3) {
                auto it = orders.begin();
                result = (it->getId() == 1 && 
                         std::next(it)->getId() == 2 && 
                         std::next(std::next(it))->getId() == 3);
            }
        }
        printTestResult(result, "Same price - FIFO order preserved");
        if (result) passed++;
        total++;
    }
    
    // Test 6: Best bid when empty
    {
        OrderBook book;
        bool result = (book.getBestBid() == 0.0);
        printTestResult(result, "Best bid on empty book returns 0.0");
        if (result) passed++;
        total++;
    }
    
    // Test 7: Best ask when empty
    {
        OrderBook book;
        bool result = (book.getBestAsk() == 0.0);
        printTestResult(result, "Best ask on empty book returns 0.0");
        if (result) passed++;
        total++;
    }
    
    // Test 8: Remove order
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        
        bool result = book.removeOrder(2);
        result = result && (book.getOrderCount() == 1);
        result = result && (book.getBestBid() == 100.00);
        printTestResult(result, "Remove order - order removed correctly");
        if (result) passed++;
        total++;
    }
    
    // Test 9: Remove non-existent order
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        
        bool result = !book.removeOrder(999);
        printTestResult(result, "Remove non-existent order returns false");
        if (result) passed++;
        total++;
    }
    
    // Test 10: Get volume at price
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 30));
        book.addOrder(Order(3, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 20));
        
        int volume = book.getVolumeAtPrice(100.00, true);
        bool result = (volume == 80);
        printTestResult(result, "Get volume at price - correct total");
        if (result) passed++;
        total++;
    }
    
    // Test 11: Empty book check
    {
        OrderBook book;
        bool result = book.isEmpty();
        printTestResult(result, "Empty book - isEmpty returns true");
        if (result) passed++;
        total++;
    }
    
    // Test 12: Non-empty book check
    {
        OrderBook book;
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        bool result = !book.isEmpty();
        printTestResult(result, "Non-empty book - isEmpty returns false");
        if (result) passed++;
        total++;
    }
    
    // Summary
    std::cout << "\n========================================\n";
    std::cout << "  TEST SUMMARY\n";
    std::cout << "========================================\n";
    std::cout << "  Passed: " << passed << "/" << total << " tests\n";
    std::cout << "  Success Rate: " << (passed * 100 / total) << "%\n";
    std::cout << "========================================\n\n";
    
    return (passed == total) ? 0 : 1;
}
