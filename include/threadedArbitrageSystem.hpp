// threadedArbitrageSystem.hpp

#pragma once

#include "config.hpp"
#include "arbitrageAnalyzer.hpp"
#include "exchanges.hpp"
#include "threadSafeQueue.hpp"
#include "exchangeData.hpp"
#include "safeOutput.hpp"
#include <thread>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <chrono>

class ThreadedArbitrageSystem {
private:
    Config& config;
    ArbitrageAnalyzer analyzer;
    KrakenExchange kraken;
    BinanceExchange binance;
    ThreadSafeQueue<ExchangeData> exchange_data_queue;
    ThreadSafeQueue<ArbitrageOpportunity> opportunity_queue;
    std::atomic<bool> running{true};
    std::vector<std::thread> workers;
    
    // Status tracking
    std::atomic<int> symbols_processed{0};
    std::atomic<int> opportunities_found{0};
    std::chrono::steady_clock::time_point last_status_update;
    std::mutex status_mutex;

    void fetch_exchange_data(const std::string& symbol);
    void process_exchange_data();
    void process_opportunities();
    void update_status();
    void reset_status_counters();

public:
    ThreadedArbitrageSystem();
    ~ThreadedArbitrageSystem();
    
    void start();
    void stop();

    // Prevent copying and moving
    ThreadedArbitrageSystem(const ThreadedArbitrageSystem&) = delete;
    ThreadedArbitrageSystem& operator=(const ThreadedArbitrageSystem&) = delete;
    ThreadedArbitrageSystem(ThreadedArbitrageSystem&&) = delete;
    ThreadedArbitrageSystem& operator=(ThreadedArbitrageSystem&&) = delete;
};