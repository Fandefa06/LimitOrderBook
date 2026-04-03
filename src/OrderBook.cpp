#include "OrderBook.hpp"
#include <iostream>

OrderBook::OrderBook() : bestBid(0), bestAsk(MAX_PRICE) {
    // We pre-allocate the entire price range. This costs a few MB of RAM but gives extreme speed.
    bids.resize(MAX_PRICE);
    asks.resize(MAX_PRICE);
    trades.reserve(1000000);
}

void OrderBook::addOrder(Order order) {
    if (order.side == OrderSide::Bid) {
        
        // 1. Match against existing asks
        while (order.quantity > 0 && bestAsk <= order.price) {
            std::deque<Order>& askQueue = asks[bestAsk];

            while (!askQueue.empty() && order.quantity > 0) {
                Order& bestAskOrder = askQueue.front();
                
                uint32_t matchQty = std::min(order.quantity, bestAskOrder.quantity);
                trades.emplace_back(order.id, bestAskOrder.id, bestAsk, matchQty);

                if (order.quantity >= bestAskOrder.quantity) {
                    order.quantity -= bestAskOrder.quantity;
                    askQueue.pop_front(); // O(1) removal because it's a deque
                } else {
                    bestAskOrder.quantity -= order.quantity;
                    order.quantity = 0;
                }
            }

            // 2. If the current best ask level is depleted, find the next one
            if (askQueue.empty()) {
                while (bestAsk < MAX_PRICE && asks[bestAsk].empty()) {
                    bestAsk++;
                }
                if (bestAsk == MAX_PRICE) break; // No more sellers left
            }
        }
        
        // 3. If not fully filled, sit in the book
        if (order.quantity > 0) {
            bids[order.price].push_back(order);
            // Update the tracker if this new bid is the best we've seen
            if (order.price > bestBid) {
                bestBid = order.price;
            }
        }
    } 
    else {
        // MATCHING FOR ASKS
        while (order.quantity > 0 && bestBid >= order.price && bestBid > 0) {
            std::deque<Order>& bidQueue = bids[bestBid];

            while (!bidQueue.empty() && order.quantity > 0) {
                Order& bestBidOrder = bidQueue.front();
                
                uint32_t matchQty = std::min(order.quantity, bestBidOrder.quantity);
                trades.emplace_back(bestBidOrder.id, order.id, bestBid, matchQty);

                if (order.quantity >= bestBidOrder.quantity) {
                    order.quantity -= bestBidOrder.quantity;
                    bidQueue.pop_front();
                } else {
                    bestBidOrder.quantity -= order.quantity;
                    order.quantity = 0;
                }
            }

            // If the current best bid level is depleted, scan downwards
            if (bidQueue.empty()) {
                while (bestBid > 0 && bids[bestBid].empty()) {
                    bestBid--;
                }
                if (bestBid == 0) break; // No more buyers left
            }
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            // Update tracker if this new ask is lower than the current best
            if (order.price < bestAsk) {
                bestAsk = order.price;
            }
        }
    }
}