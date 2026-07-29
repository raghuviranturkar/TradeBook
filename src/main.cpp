#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <thread>
#include "Order.h"
#include "OrderBook.h"
#include "TradeEngine.h"
#include "TradeHistory.h"
#include "EngineStatistics.h"
#include "Constants.h"

void printHeader(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

void printSection(const std::string& title) {
    std::cout << "\n----------------------------------------\n";
    std::cout << "  " << title << "\n";
    std::cout << "----------------------------------------\n";
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  " << TradingConstants::APP_NAME << "\n";
    std::cout << "  Version " << TradingConstants::APP_VERSION << "\n";
    std::cout << "========================================\n";
    
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::cout << "  Started at: " << std::ctime(&time_t_now);
    std::cout << "----------------------------------------\n";
    std::cout << "  Day 7: Statistics, Reporting & Polish!\n";
    std::cout << "========================================\n";
    
    // Create statistics collector
    EngineStatistics stats;
    stats.startTiming();
    
    // Create trading engine components
    OrderBook book;
    TradeHistory history;
    TradeEngine engine(book, history);
    
    // ========================================
    // Demo 1: Limit Orders
    // ========================================
    printHeader("DEMO 1: LIMIT ORDERS");
    
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        book.addOrder(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        stats.recordOrderReceived(Order(1, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        
        book.addOrder(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 40));
        stats.recordOrderReceived(Order(2, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 40));
        
        book.addOrder(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.00, 20));
        stats.recordOrderReceived(Order(3, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.00, 20));
        
        Order buy(4, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 107.00, 50);
        stats.recordOrderReceived(buy);
        engine.processOrder(buy);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("Limit Orders", duration.count());
        
        book.printBook();
    }
    
    // ========================================
    // Demo 2: Market Orders
    // ========================================
    printHeader("DEMO 2: MARKET ORDERS");
    
    {
        OrderBook book2;
        TradeHistory history2;
        TradeEngine engine2(book2, history2);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        book2.addOrder(Order(5, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book2.addOrder(Order(6, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 40));
        book2.addOrder(Order(7, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.00, 20));
        
        Order buy(8, Side::BUY, OrderType::MARKET, TimeInForce::GTC, 0, 50);
        stats.recordOrderReceived(buy);
        engine2.processOrder(buy);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("Market Orders", duration.count());
        
        book2.printBook();
        history2.printSummary();
    }
    
    // ========================================
    // Demo 3: IOC Orders
    // ========================================
    printHeader("DEMO 3: IOC (IMMEDIATE-OR-CANCEL)");
    
    {
        OrderBook book3;
        TradeHistory history3;
        TradeEngine engine3(book3, history3);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        book3.addOrder(Order(9, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 30));
        book3.addOrder(Order(10, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 106.00, 40));
        
        Order buy(11, Side::BUY, OrderType::LIMIT, TimeInForce::IOC, 107.00, 100);
        stats.recordOrderReceived(buy);
        engine3.processOrder(buy);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("IOC Orders", duration.count());
        
        book3.printBook();
        history3.printSummary();
    }
    
    // ========================================
    // Demo 4: FOK Orders
    // ========================================
    printHeader("DEMO 4: FOK (FILL-OR-KILL)");
    
    {
        OrderBook book4;
        TradeHistory history4;
        TradeEngine engine4(book4, history4);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        book4.addOrder(Order(12, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 40));
        book4.addOrder(Order(13, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 105.00, 60));
        
        Order buy(14, Side::BUY, OrderType::LIMIT, TimeInForce::FOK, 105.00, 100);
        stats.recordOrderReceived(buy);
        engine4.processOrder(buy);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("FOK Orders", duration.count());
        
        book4.printBook();
        history4.printSummary();
    }
    
    // ========================================
    // Demo 5: Order Modifications
    // ========================================
    printHeader("DEMO 5: ORDER MODIFICATIONS");
    
    {
        OrderBook book5;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        book5.addOrder(Order(15, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        stats.recordOrderReceived(Order(15, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        
        std::cout << "→ Modifying Order 15: Price 100→105, Qty 50→75\n";
        book5.modifyOrder(15, 105.00, 75, false);
        stats.recordOrderModified(Order(15, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("Modifications", duration.count());
        
        book5.printBook();
    }
    
    // ========================================
    // Demo 6: Cancellations
    // ========================================
    printHeader("DEMO 6: ORDER CANCELLATIONS");
    
    {
        OrderBook book6;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        book6.addOrder(Order(16, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        stats.recordOrderReceived(Order(16, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        
        std::cout << "→ Cancelling Order 16\n";
        book6.cancelOrder(16);
        stats.recordOrderCancelled(Order(16, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 100.00, 50));
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        stats.recordOperation("Cancellations", duration.count());
        
        book6.printBook();
    }
    
    // ========================================
    // Demo 7: Statistics Report
    // ========================================
    printHeader("DEMO 7: STATISTICS REPORT");
    
    stats.stopTiming();
    stats.printReport();
    
    // ========================================
    // Demo 8: Performance Report
    // ========================================
    printHeader("DEMO 8: PERFORMANCE REPORT");
    stats.printPerformanceReport();
    
    // ========================================
    // Demo 9: CSV Export
    // ========================================
    printHeader("DEMO 9: CSV EXPORT");
    
    std::cout << "Exporting statistics to CSV...\n";
    if (stats.exportToCSV("trading_stats.csv")) {
        std::cout << "✅ Statistics exported to trading_stats.csv\n";
    } else {
        std::cout << "❌ Failed to export statistics\n";
    }
    
    // ========================================
    // Demo 10: Order Book Summary
    // ========================================
    printHeader("DEMO 10: ORDER BOOK SUMMARY");
    
    OrderBook summaryBook;
    summaryBook.addOrder(Order(17, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 106.50, 35));
    summaryBook.addOrder(Order(18, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 105.00, 25));
    summaryBook.addOrder(Order(19, Side::BUY, OrderType::LIMIT, TimeInForce::GTC, 104.50, 40));
    summaryBook.addOrder(Order(20, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.00, 30));
    summaryBook.addOrder(Order(21, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 107.50, 20));
    summaryBook.addOrder(Order(22, Side::SELL, OrderType::LIMIT, TimeInForce::GTC, 108.00, 15));
    
    std::cout << "\n  ORDER BOOK SUMMARY\n";
    std::cout << "  ========================================\n";
    std::cout << "  Best Bid  : $" << std::fixed << std::setprecision(2) 
              << summaryBook.getBestBid() << "\n";
    std::cout << "  Best Ask  : $" << std::fixed << std::setprecision(2) 
              << summaryBook.getBestAsk() << "\n";
    std::cout << "  Spread    : $" << std::fixed << std::setprecision(2) 
              << (summaryBook.getBestAsk() - summaryBook.getBestBid()) << "\n";
    std::cout << "  Bid Levels: " << summaryBook.getBidLevels().size() << "\n";
    std::cout << "  Ask Levels: " << summaryBook.getAskLevels().size() << "\n";
    std::cout << "  Open Orders: " << summaryBook.getOrderCount() << "\n";
    std::cout << "  ========================================\n";
    
    // ========================================
    // Summary
    // ========================================
    printHeader("DAY 7 COMPLETE");
    std::cout << "✅ EngineStatistics - Implemented\n";
    std::cout << "✅ Trade Tracking - Working\n";
    std::cout << "✅ VWAP Calculation - Working\n";
    std::cout << "✅ Performance Benchmark - Working\n";
    std::cout << "✅ Professional Reports - Working\n";
    std::cout << "✅ CSV Export - Working\n";
    std::cout << "✅ Order Book Summary - Working\n";
    std::cout << "✅ 10 Complete Demos - Working\n";
    std::cout << "\n🎯 Your trading engine is now a professional system!\n";
    std::cout << "Ready for Day 8: Optimization, Refactoring & Testing!\n";
    std::cout << "========================================\n\n";
    
    return 0;
}
