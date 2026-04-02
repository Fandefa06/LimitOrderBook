#include <iostream>
#include "Order.hpp"
int main(){

    Order BuyOrder(1, 100, 10, OrderSide::Buy);
    std::cout << "Orden creada: ID " << BuyOrder.id << std::endl;



    return 0;
}

