#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(Order order) {
    if (order.side == OrderSide::Bid) {
        // Match against existing asks
        while (!asks.empty() && order.price >= asks.front().price && order.quantity > 0) {
            Order& bestAsk = asks.front();
            
            if (order.quantity >= bestAsk.quantity) {
                order.quantity -= bestAsk.quantity; // Decrease incoming
                asks.erase(asks.begin());           // Full fill: remove ask
            } else {
                bestAsk.quantity -= order.quantity; // Partial fill: update ask
                order.quantity = 0;                 // Incoming finished
            }
        }
        
        // If not fully filled, add to bids
        if (order.quantity > 0) {
            auto it = std::lower_bound(bids.begin(), bids.end(), order, 
                [](const Order& a, const Order& b) { return a.price > b.price; }); // High to low
            bids.insert(it, order);
        }
    } 
    else {
        // Match against existing bids
        while (!bids.empty() && order.price <= bids.front().price && order.quantity > 0) {
            Order& bestBid = bids.front();
            
            if (order.quantity >= bestBid.quantity) {
                order.quantity -= bestBid.quantity; // Decrease incoming
                bids.erase(bids.begin());           // Full fill: remove bid
            } else {
                bestBid.quantity -= order.quantity; // Partial fill: update bid
                order.quantity = 0;                 // Incoming finished
            }
        }

        // If not fully filled, add to asks
        if (order.quantity > 0) {
            auto it = std::lower_bound(asks.begin(), asks.end(), order, 
                [](const Order& a, const Order& b) { return a.price < b.price; }); // Low to high
            asks.insert(it, order);
        }
    }
}

void OrderBook::print() const {
    std::cout << "\n--- ASK SIDE (Sellers) ---" << std::endl;
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << it->price << " @ " << it->quantity << std::endl; // High prices top
    }
    
    std::cout << "--- BID SIDE (Buyers) ---" << std::endl;
    for (const auto& order : bids) {
        std::cout << order.price << " @ " << order.quantity << std::endl; // High prices top
    }
    std::cout << "--------------------------\n" << std::endl;
}