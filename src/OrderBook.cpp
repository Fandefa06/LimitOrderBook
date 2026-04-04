#include "OrderBook.hpp"
#include <iostream>
#include <fstream>      
#include <filesystem>   

namespace fs = std::filesystem;

OrderBook::OrderBook() : bestBid(0), bestAsk(MAX_PRICE) {
    // Record the exact engine initialization time (T=0)
    startTime = std::chrono::steady_clock::now(); 

    // Pre-allocate memory to guarantee O(1) execution time
    bids.resize(MAX_PRICE);
    asks.resize(MAX_PRICE);
    trades.reserve(1000000);
    
    // Pre-allocate 100M+ bits for lazy cancellation tracking (~12.5 MB of RAM)
    cancelledOrders.resize(100000500, false); 
}

void OrderBook::addOrder(Order order) {
    
    // Check if the incoming order itself was cancelled before arriving (edge case)
    if (cancelledOrders[order.id]) return;

    if (order.side == OrderSide::Bid) {
        
        while (order.quantity > 0 && bestAsk <= order.price) {
            std::deque<Order>& askQueue = asks[bestAsk];

            // --- LAZY CANCELLATION CLEANUP ---
            // Remove cancelled orders sitting at the front of the queue
            while (!askQueue.empty() && cancelledOrders[askQueue.front().id]) {
                askQueue.pop_front();
            }

            while (!askQueue.empty() && order.quantity > 0) {
                Order& bestAskOrder = askQueue.front();
                
                uint32_t matchQty = std::min(order.quantity, bestAskOrder.quantity);
                trades.emplace_back(order.id, bestAskOrder.id, bestAsk, matchQty);

                if (order.quantity >= bestAskOrder.quantity) {
                    order.quantity -= bestAskOrder.quantity;
                    askQueue.pop_front(); 
                } else {
                    bestAskOrder.quantity -= order.quantity;
                    order.quantity = 0;
                }
                
                // CLEANUP AFTER PARTIAL FILL
                // Just in case the next order in line is also cancelled
                while (!askQueue.empty() && cancelledOrders[askQueue.front().id]) {
                    askQueue.pop_front();
                }
            }

            if (askQueue.empty()) {
                while (bestAsk < MAX_PRICE && asks[bestAsk].empty()) {
                    bestAsk++;
                }
                if (bestAsk == MAX_PRICE) break; 
            }
        }
        
        if (order.quantity > 0) {
            bids[order.price].push_back(order);
            if (order.price > bestBid) bestBid = order.price;
        }
    } 
    else {
        // MATCHING FOR ASKS
        while (order.quantity > 0 && bestBid >= order.price && bestBid > 0) {
            std::deque<Order>& bidQueue = bids[bestBid];

            // --- LAZY CANCELLATION CLEANUP ---
            while (!bidQueue.empty() && cancelledOrders[bidQueue.front().id]) {
                bidQueue.pop_front();
            }

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
                
                // CLEANUP AFTER PARTIAL FILL
                while (!bidQueue.empty() && cancelledOrders[bidQueue.front().id]) {
                    bidQueue.pop_front();
                }
            }

            if (bidQueue.empty()) {
                while (bestBid > 0 && bids[bestBid].empty()) {
                    bestBid--;
                }
                if (bestBid == 0) break; 
            }
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            if (order.price < bestAsk) bestAsk = order.price;
        }
    }
    // Record L1 state after order processing
    recordSpread();
}

void OrderBook::cancelOrder(uint32_t orderId) {
    cancelledOrders[orderId] = true;
    // Record L1 state after cancellation
    recordSpread();
}

void OrderBook::recordSpread() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime);
    
    // We only record if it's a valid spread
    if (bestBid > 0 && bestAsk < MAX_PRICE) {
        spreadHistory.emplace_back(duration.count(), bestBid, bestAsk);
    }
}

void OrderBook::exportTradesToCSV(const std::string& filename) const {
    // 1. Detect current working directory
    fs::path current = fs::current_path();
    
    // 2. If running from 'build' folder, move up to the project root
    if (current.filename() == "build") {
        current = current.parent_path();
    }

    // 3. Define the 'output' directory at the root level
    fs::path outputDir = current / "output";

    // 4. Create the directory if it does not exist
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    fs::path fullPath = outputDir / filename;
    std::ofstream file(fullPath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file at: " << fullPath << std::endl;
        return;
    }

    // Write CSV headers including the new Microseconds column
    file << "Microseconds,BuyerID,SellerID,Price,Quantity\n";
    
    for (const auto& t : trades) {
        // Calculate elapsed time in microseconds since engine initialization
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t.timestamp - startTime);
        
        file << duration.count() << "," 
             << t.buyerId << "," 
             << t.sellerId << "," 
             << t.price << "," 
             << t.quantity << "\n";
    }

    file.close();
    std::cout << "File successfully saved to: " << fullPath << std::endl;
}

void OrderBook::exportSpreadToCSV(const std::string& filename) const {
    // Detect and setup path
    fs::path current = fs::current_path();
    if (current.filename() == "build") current = current.parent_path();
    fs::path outputDir = current / "output";
    if (!fs::exists(outputDir)) fs::create_directories(outputDir);

    fs::path fullPath = outputDir / filename;
    std::ofstream file(fullPath);

    if (!file.is_open()) return;

    // Header for L1 spread data
    file << "Microseconds,BestBid,BestAsk\n";
    for (const auto& s : spreadHistory) {
        file << s.timestamp << "," << s.bid << "," << s.ask << "\n";
    }
    file.close();
    std::cout << "Spread data saved to: " << fullPath << std::endl;
}