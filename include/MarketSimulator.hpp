#pragma once
#include "OrderBook.hpp"
#include <random>
#include <iostream>

class MarketSimulator {
public:
    // We added default parameters so you can customize the market behavior from main.cpp
    static void generateRandomOrders(OrderBook& book, 
                                     uint32_t numOrders,
                                     uint32_t minPrice = 90,
                                     uint32_t maxPrice = 110,
                                     uint32_t minQty = 1,
                                     uint32_t maxQty = 15,
                                     bool useFixedSeed = false) { // Set to true for debugging. Set to false to test
        
        // 1. The Seed Engine
        std::random_device rd; // True hardware-based random number (Entropy)
        
        // If useFixedSeed is true, we use 12345. Otherwise, we ask the hardware for a true random seed.
        std::mt19937 gen(useFixedSeed ? 12345 : rd()); 
        
        // 2. The Market Parameters
        std::uniform_int_distribution<uint32_t> priceDist(minPrice, maxPrice);
        std::uniform_int_distribution<uint32_t> qtyDist(minQty, maxQty);
        std::uniform_int_distribution<int> sideDist(0, 1);

        // Progress bar parameters
        uint32_t updateInterval = numOrders / 20; 
        if (updateInterval == 0) updateInterval = 1; 
        const int barWidth = 50; 

        // 3. Generate the flow
        for (uint32_t i = 1; i <= numOrders; ++i) {
            OrderSide side = (sideDist(gen) == 0) ? OrderSide::Bid : OrderSide::Ask;
            book.addOrder(Order(i, priceDist(gen), qtyDist(gen), side));

            // Efficient terminal update (every 5%)
            if (i % updateInterval == 0 || i == numOrders) {
                float progress = static_cast<float>(i) / numOrders;
                int pos = barWidth * progress;

                std::cout << "\r[";
                for (int p = 0; p < barWidth; ++p) {
                    if (p < pos) std::cout << "=";
                    else if (p == pos) std::cout << ">";
                    else std::cout << " ";
                }
                std::cout << "] " << int(progress * 100.0) << "%" << std::flush;
            }
        }
        std::cout << std::endl;
    }
};