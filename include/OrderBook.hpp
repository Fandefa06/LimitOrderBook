#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "Order.hpp"
#include "Trade.hpp"

constexpr uint32_t MAX_PRICE = 200000; 

class OrderBook {
private:
    std::vector<std::deque<Order>> bids; 
    std::vector<std::deque<Order>> asks;
    std::vector<Trade> trades;

    uint32_t bestBid;
    uint32_t bestAsk;

    // NEW: The Soft Delete Ledger
    // Maps Order ID to a boolean (true if cancelled)
    std::vector<bool> cancelledOrders;
    
    // NEW: High-resolution timestamp for latency and burst analysis
    std::chrono::steady_clock::time_point startTime;

public:
    OrderBook();
    
    void addOrder(Order order);
    void cancelOrder(uint32_t orderId); // NEW: Cancellation method
    
    void printTrades() const;
    const std::vector<Trade>& getTrades() const { return trades; }
    void exportTradesToCSV(const std::string& filename) const; 
};