#pragma once

#include <chrono>
#include <string>
#include "Side.h"
#include "OrderType.h"
#include "Constants.h"

using OrderId = int;
using Price = double;
using Quantity = int;

/**
 * @brief Represents the current status of an order
 */
enum class OrderStatus {
    ACTIVE,              // Order is active in the book
    PARTIALLY_FILLED,    // Partially filled, still active
    FILLED,              // Completely filled
    CANCELLED            // Cancelled by user
};

class Order {
public:
    Order() = default;
    Order(OrderId id, Side side, OrderType type, TimeInForce tif, 
          Price price, Quantity quantity);
    
    // Getters
    OrderId getId() const;
    Side getSide() const;
    OrderType getType() const;
    TimeInForce getTimeInForce() const;
    Price getPrice() const;
    Quantity getQuantity() const;
    Quantity getRemainingQuantity() const;
    OrderStatus getStatus() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::chrono::system_clock::time_point getLastModified() const;
    
    // Status checks
    bool isFilled() const;
    bool isActive() const;
    bool isCancelled() const;
    bool isPartiallyFilled() const;
    
    // Operations
    void reduceQuantity(Quantity qty);
    void cancel();
    bool modify(Price newPrice, Quantity newQuantity, bool keepPriority = false);
    void updateStatus();
    
    // Display
    void print() const;
    std::string toString() const;
    static std::string statusToString(OrderStatus status);
    
private:
    OrderId id_ = 0;
    Side side_ = Side::BUY;
    OrderType type_ = OrderType::LIMIT;
    TimeInForce timeInForce_ = TimeInForce::GTC;
    Price price_ = 0.0;
    Quantity quantity_ = 0;
    Quantity remainingQuantity_ = 0;
    OrderStatus status_ = OrderStatus::ACTIVE;
    std::chrono::system_clock::time_point timestamp_ = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point lastModified_ = std::chrono::system_clock::now();
    
    void validate() const;
    void updateLastModified();
};
// Add TimeInForce support to Order
// Make sure TimeInForce enum is included
