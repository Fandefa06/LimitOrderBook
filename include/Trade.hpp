#pragma once
#include <cstdint>
#include <chrono>

struct Trade {

    uint64_t buyerId;                                   // Who bought
    uint64_t sellerId;                                  // Who sold
    uint32_t price;                                     // Execution price
    uint32_t quantity;                                  // Executed quantity
    std::chrono::steady_clock::time_point timestamp;    // Precise moment

    
    Trade(uint64_t bId, uint64_t sId, uint32_t p, uint32_t q)
        : buyerId(bId), sellerId(sId), price(p), quantity(q),
          timestamp(std::chrono::steady_clock::now()) {}
};