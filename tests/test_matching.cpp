#include <iostream>
#include <cassert>
#include "../include/OrderBook.h"
#include "../include/TradeEngine.h"
#include "../include/TradeHistory.h"

void printTestResult(bool passed, const std::string& testName) {
    if (passed) {
        std::cout << "✅ PASSED: " << testName << "\n";
    } else {
        std::cout << "❌ FAILED: " << testName << "\n";
    }
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  MATCHING ENGINE TESTS (DAY 4)\n";
    std::cout << "========================================\n\n";
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Exact match
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        Order buy(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50);
        engine.processOrder(buy);
        
        bool result = book.isEmpty() && history.getTradeCount() == 1;
        printTestResult(result, "Exact match - both orders filled");
        if (result) passed++;
        total++;
    }
    
    // Test 2: Partial fill
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 40));
        Order buy(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 100);
        engine.processOrder(buy);
        
        const Order* remaining = book.getOrder(2);
        bool result = remaining && remaining->getRemainingQuantity() == 60;
        result = result && history.getTradeCount() == 1;
        result = result && !book.isEmpty();
        printTestResult(result, "Partial fill - remaining quantity correct");
        if (result) passed++;
        total++;
    }
    
    // Test 3: Multiple price levels (Buy order matching against sells)
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 20));
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.00, 10));
        
        Order buy(4, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 107.00, 100);
        engine.processOrder(buy);
        
        bool result = history.getTradeCount() == 3;
        result = result && book.hasOrder(4);
        const Order* remaining = book.getOrder(4);
        result = result && remaining && remaining->getRemainingQuantity() == 40;
        printTestResult(result, "Multiple price levels - buy matches sells correctly");
        if (result) passed++;
        total++;
    }
    
    // Test 4: No match (price too low)
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 108.00, 50));
        Order buy(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 107.00, 50);
        engine.processOrder(buy);
        
        bool result = history.getTradeCount() == 0;
        result = result && book.hasOrder(2);
        const Order* stored = book.getOrder(2);
        result = result && stored && stored->getRemainingQuantity() == 50;
        printTestResult(result, "No match - order stored correctly");
        if (result) passed++;
        total++;
    }
    
    // Test 5: FIFO at same price
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 30));
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 25));
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 20));
        
        Order buy(4, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50);
        engine.processOrder(buy);
        
        bool result = history.getTradeCount() == 2;
        result = result && !book.hasOrder(1);
        result = result && book.hasOrder(2);
        if (result) {
            const Order* remaining = book.getOrder(2);
            result = remaining && remaining->getRemainingQuantity() == 5;
        }
        result = result && book.hasOrder(3);
        if (result) {
            const Order* remaining = book.getOrder(3);
            result = result && remaining && remaining->getRemainingQuantity() == 20;
        }
        
        printTestResult(result, "FIFO at same price - orders matched in order");
        if (result) passed++;
        total++;
    }
    
    // Test 6: Multiple matches with remaining
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 100.00, 40));
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 101.00, 30));
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 102.00, 20));
        
        Order buy(4, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 102.00, 50);
        engine.processOrder(buy);
        
        bool result = history.getTradeCount() == 2;
        result = result && !book.hasOrder(1);
        result = result && book.hasOrder(2);
        if (result) {
            const Order* remaining = book.getOrder(2);
            result = remaining && remaining->getRemainingQuantity() == 20;
        }
        result = result && book.hasOrder(3);
        if (result) {
            const Order* remaining = book.getOrder(3);
            result = result && remaining && remaining->getRemainingQuantity() == 20;
        }
        result = result && !book.hasOrder(4);
        
        printTestResult(result, "Multiple matches with remaining - correct");
        if (result) passed++;
        total++;
    }
    
    // Test 7: Trade generation
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        Order buy(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30);
        engine.processOrder(buy);
        
        auto trades = history.getTrades();
        bool result = trades.size() == 1;
        if (result) {
            const Trade& t = trades[0];
            result = (t.getPrice() == 105.00) && 
                     (t.getQuantity() == 30) &&
                     (t.getBuyOrderId() == 2) &&
                     (t.getSellOrderId() == 1);
        }
        printTestResult(result, "Trade generation - correct trade details");
        if (result) passed++;
        total++;
    }
    
    // Test 8: Best bid updated after fills
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 20));
        book.addOrder(Order(3, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 104.00, 50));
        book.addOrder(Order(4, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 103.00, 30));
        
        Order buy(5, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 106.00, 25);
        engine.processOrder(buy);
        
        bool result = book.getBestBid() == 104.00;
        printTestResult(result, "Best bid updated correctly");
        if (result) passed++;
        total++;
    }
    
    // Test 9: Best ask updated after fills
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 104.00, 20));
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 50));
        
        Order sell(4, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 25);
        engine.processOrder(sell);
        
        bool result = book.getBestAsk() == 106.00;
        printTestResult(result, "Best ask updated correctly");
        if (result) passed++;
        total++;
    }
    
    // Test 10: Empty price levels removed
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 50));
        Order buy(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 50);
        engine.processOrder(buy);
        
        const auto& asks = book.getAsks();
        bool result = asks.find(105.00) == asks.end();
        printTestResult(result, "Empty price levels removed");
        if (result) passed++;
        total++;
    }
    
    // Test 11: Sell order matching with existing buys (simple)
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 50));
        Order sell(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30);
        engine.processOrder(sell);
        
        bool result = history.getTradeCount() == 1;
        result = result && book.hasOrder(1);
        
        const Order* remaining = book.getOrder(1);
        result = result && remaining && remaining->getRemainingQuantity() == 20;
        printTestResult(result, "Sell order matching - correct");
        if (result) passed++;
        total++;
    }
    
    // Test 12: Multiple sell orders matching - FIXED
    {
        OrderBook book;
        TradeHistory history;
        TradeEngine engine(book, history);
        
        // Add buy orders at different price levels
        book.addOrder(Order(1, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book.addOrder(Order(2, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 104.00, 20));
        book.addOrder(Order(3, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 103.00, 10));
        
        // Sell order comes in at 103
        Order sell(4, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 103.00, 45);
        engine.processOrder(sell);
        
        // Should match 30 from buy at 105, then 15 from buy at 104
        // Total: 2 trades, sell fully filled
        bool result = history.getTradeCount() == 2;
        
        // Check trades were created with correct prices
        auto trades = history.getTrades();
        bool pricesCorrect = true;
        if (trades.size() == 2) {
            std::vector<double> prices;
            for (const auto& t : trades) {
                prices.push_back(t.getPrice());
            }
            std::sort(prices.begin(), prices.end());
            pricesCorrect = (prices[0] == 104.00 && prices[1] == 105.00);
        } else {
            pricesCorrect = false;
        }
        
        result = result && pricesCorrect;
        
        // Check sell order is fully filled
        result = result && !book.hasOrder(4);
        
        // Check buy order at 105 is fully filled
        result = result && !book.hasOrder(1);
        
        // Check buy order at 104 has 5 remaining (20 - 15 = 5)
        const Order* remainingBuy104 = book.getOrder(2);
        result = result && remainingBuy104 && remainingBuy104->getRemainingQuantity() == 5;
        
        // Check buy order at 103 is untouched (10 remaining)
        const Order* remainingBuy103 = book.getOrder(3);
        result = result && remainingBuy103 && remainingBuy103->getRemainingQuantity() == 10;
        
        printTestResult(result, "Multiple sell orders matching - correct");
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
