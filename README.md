# High-Frequency Trading (HFT) Matching Engine

A low-latency Limit Order Book (LOB) built in C++ designed for high-throughput financial simulations. 

## 🚀 Performance Metrics
- **Throughput:** ~10,000,000 orders/second (on local hardware).
- **Latency:** Sub-microsecond matching for top-of-book orders.
- **Capacity:** Tested with continuous 10M+ order injections (generating ~1.7GB datasets).

## 🛠️ Features
- **Deterministic & Stochastic Simulation:** Integrated `MarketSimulator` with configurable price bounds and hardware entropy.
- **Order Cancellation:** Instant $O(1)$ removal of orders from the book.
- **Automated Data Management:** Smart directory detection and creation for simulation outputs.
- **Zero-Latency CLI:** Non-blocking terminal progress bar using carriage return updates.

## 🏗️ Architectural Decisions (ADR)

### 1. Price Indexing: Flat Array (Direct Mapping)
- **Decision:** Used a pre-allocated `std::vector<std::deque<Order>>` where the index represents the price tick.
- **Why:** Achieves absolute **O(1) access time**, bypassing $O(\log N)$ search overhead and $O(N)$ memory shifts.
- **Trade-off:** Optimized for CPU Cache locality over RAM footprint.

### 2. Order Cancellation: Lazy Deletion ($O(1)$)
- **Decision:** Implemented a "Soft Delete" ledger using `std::unordered_map<uint32_t, bool>`.
- **Why:** Instead of searching for an order to remove it ($O(N)$), we mark it as cancelled in a hash table. The matching engine ignores these orders only when they reach the top of the book. 
- **Result:** Constant time cancellation regardless of the book size.

### 3. Smart Output Management (`std::filesystem`)
- **Decision:** Automated path resolution using C++20 `std::filesystem`.
- **Why:** The engine detects its execution context (root vs. build folder) and ensures all CSV exports are stored in a dedicated `/output` directory at the project root.

### 4. Memory Pre-allocation (Reserve)
- **Decision:** Using `.reserve()` on the Trade audit vector.
- **Why:** Prevents "Stop-the-world" tail latencies caused by dynamic memory reallocation during high-load simulations.

## 📊 Scalability Note
The engine is capable of generating massive datasets (e.g., **1.7GB+ CSV files**). It is highly recommended to use specialized tools like **Modern CSV**, **Pandas**, or **Datagrip** to analyze results, as standard spreadsheet software like Excel will exceed its row limits.

## 💻 Usage
You can customize the simulation parameters directly in `src/main.cpp`:

```cpp
const uint32_t numOrders = 300000; 
const uint32_t cancelPercent = 15; // Probability to cancel an order (0-100)

MarketSimulator::generateRandomOrders(myBook, numOrders, cancelPercent);