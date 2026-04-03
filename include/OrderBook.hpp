#pragma once
#include <vector>
#include <algorithm>
#include "Order.hpp"

class OrderBook {
private:
    std::vector<Order> bids;
    std::vector<Order> asks;

public:
    void addOrder(Order order);

    void print() const;
    
    //Getters so that we can read form the main
    const std::vector<Order>& getBids() const { return bids; }
    const std::vector<Order>& getAsks() const { return asks; }
};