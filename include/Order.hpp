#pragma once

#include <cstdint>

enum class OrderSide: uint8_t {
    Buy,
    Sell
};

struct Order {
    uint64_t id;            // ID to locate the specific order
    uint32_t price;         // What's the price for ONE stock 
    uint32_t quantity;      // How many stocks are in the order
    OrderSide side;         // What are we doing, buying or selling??

    Order(uint64_t orderId, uint32_t orderPrice, uint32_t orderQuantity, OrderSide orderSide)
        : id(orderId),
        price(orderPrice),
        quantity(orderQuantity),
        side(orderSide)
        
        {}
};
