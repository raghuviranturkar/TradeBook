#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>
#include "../include/OrderBook.h"
#include "../include/TradeEngine.h"
#include "../include/TradeHistory.h"
#include "../include/EngineStatistics.h"

void printBenchmarkHeader(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

void runBenchmark(int numOrders) {
    printBenchmarkHeader("STRESS TEST: " + std::to_string(numOrders) + " ORDERS");
    
    OrderBook book;
    TradeHistory history;
    TradeEngine engine(book, history);
    EngineStatistics stats;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sideDist(0, 1);
    std::uniform_int_distribution<> priceDist(100, 110);
    std::uniform_int_distribution<> qtyDist(1, 100);
    
    std::cout << "Generating " << numOrders << " orders...\n";
    
    stats.startTiming();
    auto start = std::chrono::high_resolution_clock::now();
    
    int orderId = 1;
    int tradesExecuted = 0;
    int errors = 0;
    
    for (int i = 0; i < numOrders; ++i) {
        Side side = (sideDist(gen) == 0) ? Side::BUY : Side::SELL;
        double price = priceDist(gen);
        int quantity = qtyDist(gen);
        
        // Randomly choose order type
        int type = i % 4;
        OrderType orderType = OrderType::LIMIT;
        TimeInForce tif = TimeInForce::GTC;
        
        if (type == 1 && i > 100) {
            orderType = OrderType::MARKET;
            price = 0;  // Market orders don't have price
        } else if (type == 2 && i > 200) {
            orderType = OrderType::LIMIT;
            tif = TimeInForce::IOC;
        } else if (type == 3 && i > 300) {
            orderType = OrderType::LIMIT;
            tif = TimeInForce::FOK;
        }
        
        try {
            Order order(orderId++, side, orderType, tif, price, quantity);
            stats.recordOrderReceived(order);
            engine.processOrder(order);
            tradesExecuted++;
        } catch (const std::exception& e) {
            errors++;
        }
        
        // Periodically cancel random orders
        if (i > 0 && i % 50 == 0) {
            int cancelId = (orderId > 1) ? (orderId - 1) : 1;
            if (book.hasOrder(cancelId)) {
                book.cancelOrder(cancelId);
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    stats.stopTiming();
    
    std::cout << "\n  RESULTS\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  Orders Processed    : " << numOrders << "\n";
    std::cout << "  Trades Executed     : " << tradesExecuted << "\n";
    std::cout << "  Errors              : " << errors << "\n";
    std::cout << "  Execution Time      : " << duration.count() / 1000.0 << " ms\n";
    std::cout << "  Orders/sec          : " 
              << std::fixed << std::setprecision(2) 
              << (numOrders / (duration.count() / 1000000.0)) << "\n";
    std::cout << "  Avg Order Time      : " 
              << std::fixed << std::setprecision(4) 
              << (duration.count() / numOrders) << " μs\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  Final Book Size     : " << book.getOrderCount() << " orders\n";
    std::cout << "  Best Bid            : $" << std::fixed << std::setprecision(2) 
              << book.getBestBid() << "\n";
    std::cout << "  Best Ask            : $" << std::fixed << std::setprecision(2) 
              << book.getBestAsk() << "\n";
    std::cout << "  Spread              : $" << std::fixed << std::setprecision(2) 
              << (book.getBestAsk() - book.getBestBid()) << "\n";
    std::cout << "  ========================================\n";
}

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  TRADEBOOK BENCHMARK SUITE\n";
    std::cout << "========================================\n";
    
    // Run benchmarks with different sizes
    runBenchmark(100);
    runBenchmark(1000);
    runBenchmark(5000);
    runBenchmark(10000);
    
    std::cout << "\n========================================\n";
    std::cout << "  BENCHMARK COMPLETE\n";
    std::cout << "========================================\n";
    
    return 0;
}
