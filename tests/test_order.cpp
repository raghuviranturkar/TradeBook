#include <iostream>
#include <stdexcept>
#include "../include/Order.h"
#include "../include/Trade.h"
#include "../include/TradeHistory.h"

void printTestHeader(const std::string& testName) {
    std::cout << "\n========================================\n";
    std::cout << "  TEST: " << testName << "\n";
    std::cout << "========================================\n";
}

void printTestResult(bool passed, const std::string& message) {
    if (passed) {
        std::cout << "✅ PASSED: " << message << "\n";
    } else {
        std::cout << "❌ FAILED: " << message << "\n";
    }
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  ORDER CLASS UNIT TESTS\n";
    std::cout << "========================================\n";
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Create valid limit order
    printTestHeader("Create Valid Limit Order");
    try {
        Order order(1001, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.50, 200);
        order.print();
        printTestResult(true, "Successfully created valid limit order");
        passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Failed to create valid order: " + std::string(e.what()));
    }
    total++;
    
    // Test 2: Create valid market order
    printTestHeader("Create Valid Market Order");
    try {
        Order order(1002, Side::SELL, OrderType::MARKET, TimeInForce::IOC, 0, 100);
        order.print();
        printTestResult(true, "Successfully created valid market order");
        passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Failed to create market order: " + std::string(e.what()));
    }
    total++;
    
    // Test 3: Invalid quantity (too small)
    printTestHeader("Invalid Quantity - Too Small");
    try {
        Order order(1003, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 0);
        printTestResult(false, "Should have thrown exception for invalid quantity");
    } catch (const std::invalid_argument& e) {
        printTestResult(true, "Correctly rejected invalid quantity: " + std::string(e.what()));
        passed++;
    }
    total++;
    
    // Test 4: Invalid quantity (too large)
    printTestHeader("Invalid Quantity - Too Large");
    try {
        Order order(1004, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 2000000);
        printTestResult(false, "Should have thrown exception for invalid quantity");
    } catch (const std::invalid_argument& e) {
        printTestResult(true, "Correctly rejected oversized quantity: " + std::string(e.what()));
        passed++;
    }
    total++;
    
    // Test 5: Invalid price (negative)
    printTestHeader("Invalid Price - Negative");
    try {
        Order order(1005, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, -10, 100);
        printTestResult(false, "Should have thrown exception for negative price");
    } catch (const std::invalid_argument& e) {
        printTestResult(true, "Correctly rejected negative price: " + std::string(e.what()));
        passed++;
    }
    total++;
    
    // Test 6: Reduce quantity (partial fill)
    printTestHeader("Reduce Quantity - Partial Fill");
    try {
        Order order(1006, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 200);
        order.print();
        std::cout << "\n→ Reducing quantity by 75...\n";
        order.reduceQuantity(75);
        std::cout << "→ Remaining: " << order.getRemainingQuantity() << "\n";
        order.print();
        printTestResult(order.getRemainingQuantity() == 125, 
                       "Correctly reduced quantity to 125");
        passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Failed to reduce quantity: " + std::string(e.what()));
    }
    total++;
    
    // Test 7: Reduce quantity (full fill)
    printTestHeader("Reduce Quantity - Full Fill");
    try {
        Order order(1007, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 100);
        std::cout << "\n→ Reducing quantity by 100 (full fill)...\n";
        order.reduceQuantity(100);
        printTestResult(order.isFilled(), "Order correctly marked as filled");
        passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Failed to fully fill order: " + std::string(e.what()));
    }
    total++;
    
    // Test 8: Reduce quantity (exceeding remaining)
    printTestHeader("Reduce Quantity - Exceeding Remaining");
    try {
        Order order(1008, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 150);
        std::cout << "\n→ Attempting to reduce by 200 (exceeds remaining 150)...\n";
        order.reduceQuantity(200);
        printTestResult(false, "Should have thrown exception for exceeding remaining");
    } catch (const std::invalid_argument& e) {
        printTestResult(true, "Correctly rejected excessive reduction: " + std::string(e.what()));
        passed++;
    }
    total++;
    
    // Test 9: Reduce quantity (zero)
    printTestHeader("Reduce Quantity - Zero");
    try {
        Order order(1009, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100, 100);
        std::cout << "\n→ Attempting to reduce by 0...\n";
        order.reduceQuantity(0);
        printTestResult(false, "Should have thrown exception for zero reduction");
    } catch (const std::invalid_argument& e) {
        printTestResult(true, "Correctly rejected zero reduction: " + std::string(e.what()));
        passed++;
    }
    total++;
    
    // Test 10: Getters
    printTestHeader("Getter Methods");
    try {
        Order order(1010, Side::SELL, OrderType::LIMIT, TimeInForce::FOK, 99.99, 500);
        bool allValid = true;
        allValid = allValid && (order.getId() == 1010);
        allValid = allValid && (order.getSide() == Side::SELL);
        allValid = allValid && (order.getType() == OrderType::LIMIT);
        allValid = allValid && (order.getTimeInForce() == TimeInForce::FOK);
        allValid = allValid && (order.getPrice() == 99.99);
        allValid = allValid && (order.getQuantity() == 500);
        allValid = allValid && (order.getRemainingQuantity() == 500);
        allValid = allValid && !order.isFilled();
        allValid = allValid && order.isActive();
        
        printTestResult(allValid, "All getters returned correct values");
        if (allValid) passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Getter test failed: " + std::string(e.what()));
    }
    total++;
    
    // Test 11: Create trade
    printTestHeader("Trade Creation");
    try {
        Trade trade(1, 1001, 2001, 105.50, 50);
        trade.print();
        printTestResult(true, "Trade created successfully");
        passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "Trade creation failed: " + std::string(e.what()));
    }
    total++;
    
    // Test 12: TradeHistory
    printTestHeader("TradeHistory Operations");
    try {
        TradeHistory history;
        
        // Add trades
        history.addTrade(Trade(1, 1001, 2001, 105.50, 50));
        history.addTrade(Trade(2, 1002, 2002, 106.00, 75));
        history.addTrade(Trade(3, 1003, 2003, 104.75, 100));
        
        bool allValid = true;
        allValid = allValid && (history.getTradeCount() == 3);
        allValid = allValid && (history.getTotalVolume() == 225);
        allValid = allValid && (history.getAveragePrice() > 105.0);
        
        history.printTrades();
        history.printSummary();
        
        printTestResult(allValid, "TradeHistory operations successful");
        if (allValid) passed++;
    } catch (const std::exception& e) {
        printTestResult(false, "TradeHistory test failed: " + std::string(e.what()));
    }
    total++;
    
    // Summary
    std::cout << "\n========================================\n";
    std::cout << "  TEST SUMMARY\n";
    std::cout << "========================================\n";
    std::cout << "  Passed: " << passed << "/" << total << " tests\n";
    std::cout << "  Success Rate: " << (passed * 100 / total) << "%\n";
    std::cout << "========================================\n\n";
    
    return (passed == total) ? 0 : 1;
}
