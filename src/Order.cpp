#include "Order.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Order::Order(OrderId id, Side side, OrderType type, TimeInForce tif,
             Price price, Quantity quantity)
    : id_(id)
    , side_(side)
    , type_(type)
    , timeInForce_(tif)
    , price_(price)
    , quantity_(quantity)
    , remainingQuantity_(quantity)
    , status_(OrderStatus::ACTIVE)
    , timestamp_(std::chrono::system_clock::now())
    , lastModified_(std::chrono::system_clock::now()) {
    validate();
}

void Order::validate() const {
    if (!Validation::isValidOrderId(id_)) {
        throw std::invalid_argument("Invalid order ID: " + std::to_string(id_));
    }
    if (!Validation::isValidQuantity(quantity_)) {
        throw std::invalid_argument(
            "Invalid quantity: " + std::to_string(quantity_) + 
            ". Must be between " + std::to_string(TradingConstants::MIN_ORDER_QUANTITY) +
            " and " + std::to_string(TradingConstants::MAX_ORDER_QUANTITY)
        );
    }
    if (type_ == OrderType::LIMIT && !Validation::isValidPrice(price_)) {
        throw std::invalid_argument("Invalid limit price: " + std::to_string(price_));
    }
    if (type_ == OrderType::MARKET && price_ < 0) {
        throw std::invalid_argument("Market order price cannot be negative");
    }
}

OrderId Order::getId() const { return id_; }
Side Order::getSide() const { return side_; }
OrderType Order::getType() const { return type_; }
TimeInForce Order::getTimeInForce() const { return timeInForce_; }
Price Order::getPrice() const { return price_; }
Quantity Order::getQuantity() const { return quantity_; }
Quantity Order::getRemainingQuantity() const { return remainingQuantity_; }
OrderStatus Order::getStatus() const { return status_; }
std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp_; }
std::chrono::system_clock::time_point Order::getLastModified() const { return lastModified_; }

bool Order::isFilled() const { return status_ == OrderStatus::FILLED; }
bool Order::isActive() const { 
    return status_ == OrderStatus::ACTIVE || status_ == OrderStatus::PARTIALLY_FILLED; 
}
bool Order::isCancelled() const { return status_ == OrderStatus::CANCELLED; }
bool Order::isPartiallyFilled() const { return status_ == OrderStatus::PARTIALLY_FILLED; }

void Order::reduceQuantity(Quantity qty) {
    if (qty <= 0) {
        throw std::invalid_argument("Reduction quantity must be positive");
    }
    if (qty > remainingQuantity_) {
        throw std::invalid_argument(
            "Cannot reduce by " + std::to_string(qty) + 
            ". Only " + std::to_string(remainingQuantity_) + " remaining"
        );
    }
    remainingQuantity_ -= qty;
    updateStatus();
    updateLastModified();
}

void Order::cancel() {
    if (status_ == OrderStatus::FILLED) {
        throw std::logic_error("Cannot cancel a filled order");
    }
    if (status_ == OrderStatus::CANCELLED) {
        throw std::logic_error("Order already cancelled");
    }
    status_ = OrderStatus::CANCELLED;
    remainingQuantity_ = 0;
    updateLastModified();
}

bool Order::modify(Price newPrice, Quantity newQuantity, bool keepPriority) {
    // Cannot modify filled or cancelled orders
    if (status_ == OrderStatus::FILLED || status_ == OrderStatus::CANCELLED) {
        return false;
    }
    
    // Validate new values
    if (!Validation::isValidQuantity(newQuantity)) {
        return false;
    }
    if (type_ == OrderType::LIMIT && !Validation::isValidPrice(newPrice)) {
        return false;
    }
    
    bool priceChanged = (newPrice != price_);
    bool quantityChanged = (newQuantity != quantity_);
    
    if (!priceChanged && !quantityChanged) {
        return true;  // No actual change
    }
    
    // Update values
    price_ = newPrice;
    quantity_ = newQuantity;
    
    // Fix remaining quantity
    if (remainingQuantity_ > quantity_) {
        remainingQuantity_ = quantity_;
    }
    
    // CRITICAL FIX: Only update timestamp if price changed AND not keeping priority
    if (priceChanged && !keepPriority) {
        timestamp_ = std::chrono::system_clock::now();
    }
    // If keepPriority is true, timestamp stays the SAME
    
    updateStatus();
    updateLastModified();
    return true;
}

void Order::updateStatus() {
    if (status_ == OrderStatus::CANCELLED) {
        return;
    }
    
    if (remainingQuantity_ == 0) {
        status_ = OrderStatus::FILLED;
    } else if (remainingQuantity_ < quantity_) {
        status_ = OrderStatus::PARTIALLY_FILLED;
    } else {
        status_ = OrderStatus::ACTIVE;
    }
}

void Order::updateLastModified() {
    lastModified_ = std::chrono::system_clock::now();
}

std::string Order::statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::ACTIVE:           return "ACTIVE";
        case OrderStatus::PARTIALLY_FILLED: return "PARTIALLY_FILLED";
        case OrderStatus::FILLED:           return "FILLED";
        case OrderStatus::CANCELLED:        return "CANCELLED";
        default:                            return "UNKNOWN";
    }
}

void Order::print() const {
    std::cout << toString() << std::endl;
}

std::string Order::toString() const {
    std::ostringstream oss;
    auto time = std::chrono::system_clock::to_time_t(timestamp_);
    std::string timeStr = std::ctime(&time);
    timeStr.pop_back();
    
    auto modTime = std::chrono::system_clock::to_time_t(lastModified_);
    std::string modTimeStr = std::ctime(&modTime);
    modTimeStr.pop_back();
    
    oss << "--------------------------------------------\n";
    oss << "Order ID : " << id_ << "\n";
    oss << "Side     : " << sideToString(side_) << "\n";
    oss << "Type     : " << orderTypeToString(type_) << "\n";
    oss << "Price    : " << std::fixed << std::setprecision(TradingConstants::PRICE_PRECISION) 
        << price_ << "\n";
    oss << "Quantity : " << quantity_ << "\n";
    oss << "Remaining: " << remainingQuantity_ << "\n";
    oss << "Status   : " << statusToString(status_) << "\n";
    oss << "TIF      : " << timeInForceToString(timeInForce_) << "\n";
    oss << "Created  : " << timeStr << "\n";
    oss << "Modified : " << modTimeStr << "\n";
    oss << "--------------------------------------------";
    return oss.str();
}
