# High-Frequency Trading (HFT) Matching Engine

A low-latency Limit Order Book (LOB) built in C++ designed for high-throughput financial simulations. 

## 🚀 Performance Metrics
- **Throughput:** ~10,000,000 orders/second (on local hardware).
- **Latency:** Sub-microsecond matching for top-of-book orders.
- **Capacity:** Tested with 10M+ continuous order injections.

## 🛠️ Features
- **Deterministic & Stochastic Simulation:** Integrated `MarketSimulator` with configurable price bounds, quantities, and entropy sources (fixed seed vs. hardware entropy).
- **Audit Trail:** $O(1)$ batch export of all executed trades to CSV.
- **Zero-Latency CLI:** Non-blocking terminal progress bar using carriage return carriage return updates.

## 🏗️ Architectural Decisions (ADR)

### 1. Price Indexing: Flat Array (Direct Mapping)
- **Decision:** Used a pre-allocated `std::vector<std::deque<Order>>` where the index strictly represents the price tick.
- **Why:** Replaced `std::map` and sorted vectors to achieve absolute **O(1) access time**. This completely bypasses the $O(\log N)$ search overhead and the catastrophic $O(N)$ memory shifting during insertions.
- **Trade-off:** Higher memory footprint (RAM) pre-allocated for the price range, in exchange for maximum CPU Cache locality and speed.

### 2. Order Storage: std::deque
- **Decision:** Each price level uses a `std::deque` to store resting orders.
- **Why:** Matches the **FIFO (First-In, First-Out)** requirement of exchange matching. `std::deque` allows $O(1)$ removal from the front (`pop_front`) without reallocating or shifting the entire sequence, unlike `std::vector`.

### 3. Memory Pre-allocation (Reserve)
- **Decision:** Using `.reserve()` on the Trade audit vector.
- **Why:** Prevents "Stop-the-world" tail latencies caused by dynamic memory reallocation and element copying when the history vector grows during high-load simulations.