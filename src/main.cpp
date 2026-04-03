#include <iostream>
#include "OrderBook.hpp"

int main() {
    OrderBook myBook;
    
    myBook.addOrder(Order(1, 100, 10, OrderSide::Bid));     //One bid
    myBook.addOrder(Order(2, 105, 5, OrderSide::Bid));      //Another bid
    myBook.addOrder(Order(3, 110, 2, OrderSide::Ask));      //One sale

    myBook.print();

    myBook.addOrder(Order(4, 115, 3, OrderSide::Bid));       //Bid finishing the previous sale and going with the rest of the bids

    //We check if it is printed
    myBook.print();

    return 0;
}

