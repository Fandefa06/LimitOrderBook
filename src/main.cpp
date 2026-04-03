#include <iostream>
#include <chrono> // Added for high-resolution timing
#include "OrderBook.hpp"
#include "MarketSimulator.hpp"

int main() {
    OrderBook myBook;
    const int numOrders = 1000000000; // Define order count here for cleaner calculations

    // ==========================================
    // MODE 1: Manual Debugging (For bug hunting)
    // ==========================================
    /*
    myBook.addOrder(Order(1, 100, 10, OrderSide::Bid));
    myBook.addOrder(Order(2, 105, 5, OrderSide::Bid));
    myBook.addOrder(Order(3, 110, 2, OrderSide::Ask));
    */

    // ==========================================
    // MODE 2: Stress Simulation
    // ==========================================
    std::cout << "--- STARTING MARKET SIMULATION ---\n" << std::endl;
    
    //Start timer
    auto start = std::chrono::high_resolution_clock::now();

    //Execute workload
    MarketSimulator::generateRandomOrders(myBook, numOrders); 

    //Stop timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // View results
    //myBook.print();
    //myBook.printTrades();

    // Print performance metrics
    std::cout << "--- PERFORMANCE METRICS ---" << std::endl;
    std::cout << "Total trades executed: " << myBook.getTrades().size() << std::endl;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Throughput: " << (numOrders / elapsed.count()) << " ops/sec" << std::endl;
    std::cout << "---------------------------" << std::endl;

    return 0;
}