#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(Order order) {
    if (order.side == OrderSide::Bid) {
        while (!asks.empty() && order.price >= asks.front().price && order.quantity > 0) {
            Order& bestAsk = asks.front();
            
            if (order.quantity >= bestAsk.quantity) {
                order.quantity -= bestAsk.quantity;
                asks.erase(asks.begin());
            } else {
                bestAsk.quantity -= order.quantity;
                order.quantity = 0;
            }
        }
        
        if (order.quantity > 0) {
            auto it = std::lower_bound(bids.begin(), bids.end(), order, 
                [](const Order& a, const Order& b) { return a.price > b.price; });
            bids.insert(it, order);
        }
    } 
    else {
        while (!bids.empty() && order.price <= bids.front().price && order.quantity > 0) {
            Order& bestBid = bids.front();
            
            if (order.quantity >= bestBid.quantity) {
                order.quantity -= bestBid.quantity;
                bids.erase(bids.begin());
            } else {
                bestBid.quantity -= order.quantity;
                order.quantity = 0;
            }
        }

        if (order.quantity > 0) {
            auto it = std::lower_bound(asks.begin(), asks.end(), order, 
                [](const Order& a, const Order& b) { return a.price < b.price; });
            asks.insert(it, order);
        }
    }
}

void OrderBook::print() const {
    std::cout << "\n--- ASK SIDE (Sellers) ---" << std::endl;
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << it->price << " @ " << it->quantity << std::endl;
    }
    
    std::cout << "--- BID SIDE (Buyers) ---" << std::endl;
    for (const auto& order : bids) {
        std::cout << order.price << " @ " << order.quantity << std::endl;
    }
    std::cout << "--------------------------\n" << std::endl;
}