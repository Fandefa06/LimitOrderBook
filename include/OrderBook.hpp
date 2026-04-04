#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <chrono>
#include "Order.hpp"
#include "Trade.hpp"

constexpr uint32_t MAX_PRICE = 200000; 

struct SpreadSnap {
    uint64_t timestamp;
    uint32_t bid;
    uint32_t ask;

    SpreadSnap(uint64_t ts, uint32_t b, uint32_t a) 
        : timestamp(ts), bid(b), ask(a) {}
};

class OrderBook {
private:
    std::vector<std::deque<Order>> bids; 
    std::vector<std::deque<Order>> asks;
    std::vector<Trade> trades;
    
    // Storage for bid-ask spread history
    std::vector<SpreadSnap> spreadHistory;

    uint32_t bestBid;
    uint32_t bestAsk;

    std::vector<bool> cancelledOrders;
    std::chrono::steady_clock::time_point startTime;

    // Records the current best bid and ask
    void recordSpread(); 

public:
    OrderBook();
    
    void addOrder(Order order);
    void cancelOrder(uint32_t orderId);
    
    void printTrades() const;
    const std::vector<Trade>& getTrades() const { return trades; }
    
    void exportTradesToCSV(const std::string& filename) const; 
    // Exports the recorded spread history to CSV
    void exportSpreadToCSV(const std::string& filename) const;
};