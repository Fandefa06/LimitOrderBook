#pragma once
#include <vector>
#include <deque> // Essential for O(1) front removal
#include <algorithm>
#include "Order.hpp"
#include "Trade.hpp"

// We define a maximum possible price for our flat array (e.g., 200,000 cents = $2,000)
constexpr uint32_t MAX_PRICE = 200000; 

class OrderBook {
private:
    // HFT Flat Array: The index is the price.
    // Example: bids[105] contains all buy orders at price 105.
    std::vector<std::deque<Order>> bids; 
    std::vector<std::deque<Order>> asks;
    std::vector<Trade> trades;

    // Trackers to instantly know where the best prices are without scanning the array
    uint32_t bestBid;
    uint32_t bestAsk;

public:
    OrderBook();
    void addOrder(Order order);
    void printTrades() const;
    const std::vector<Trade>& getTrades() const { return trades; }
};