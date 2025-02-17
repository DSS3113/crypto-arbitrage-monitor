// threadedArbitrageSystem.cpp

#include "threadedArbitrageSystem.hpp"

ThreadedArbitrageSystem::ThreadedArbitrageSystem() : 
    config(Config::getInstance()),
    kraken(config),
    binance(config),
    last_status_update(std::chrono::steady_clock::now()) {}

void ThreadedArbitrageSystem::reset_status_counters() {
    symbols_processed = 0;
    opportunities_found = 0;
    last_status_update = std::chrono::steady_clock::now();
}

void ThreadedArbitrageSystem::update_status() {
    std::lock_guard<std::mutex> lock(status_mutex);
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_status_update).count();
    auto elapsed_limit = config.getFeedbackInterval();
    
    // Update every elapsed_limit seconds if no opportunities found
    if (elapsed >= elapsed_limit && opportunities_found == 0) {
        Utils::safe_print("Status: Checked " + std::to_string(symbols_processed) + 
                         " trading pairs in the last " + std::to_string(elapsed) + 
                         " seconds. No arbitrage opportunities found.");
        reset_status_counters();
    }
}

void ThreadedArbitrageSystem::fetch_exchange_data(const std::string& symbol) {
    auto kraken_trades = kraken.fetchTrades(symbol);
    auto binance_trades = binance.fetchTrades(symbol);

    if (!kraken_trades.empty()) {
        exchange_data_queue.push({symbol, kraken_trades, "Kraken"});
    }
    if (!binance_trades.empty()) {
        exchange_data_queue.push({symbol, binance_trades, "Binance"});
    }
    
    symbols_processed++;
}

void ThreadedArbitrageSystem::process_exchange_data() {
    std::unordered_map<std::string, ExchangeData> symbol_data;
    ExchangeData data;

    while (running) {
        if (exchange_data_queue.try_pop(data)) {
            auto& current_data = symbol_data[data.symbol];
            
            if (current_data.exchange_name.empty()) {
                current_data = std::move(data);
            } else {
                const auto& data1 = current_data;
                const auto& data2 = data;

                auto opportunities = analyzer.findOpportunities(
                    data.symbol,
                    data1.trades,
                    data1.exchange_name,
                    data2.trades,
                    data2.exchange_name
                );

                if (!opportunities.empty()) {
                    opportunities_found += opportunities.size();
                    for (auto& opp : opportunities) {
                        opportunity_queue.push(std::move(opp));
                    }
                }

                symbol_data.erase(data.symbol);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void ThreadedArbitrageSystem::process_opportunities() {
    ArbitrageOpportunity opp;
    while (running) {
        if (opportunity_queue.try_pop(opp)) {
            if (analyzer.isViableOpportunity(opp)) {
                Utils::safe_print("Found viable opportunity: " + std::to_string(opp.profit_percentage) + 
                         "% profit between " + opp.buy_exchange + " and " + opp.sell_exchange + 
                         " for " + opp.symbol);
            }
        } else {
            update_status();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void ThreadedArbitrageSystem::start() {
    reset_status_counters();
    const int num_analysis_threads = config.getNumThreads();
    
    for (int i = 0; i < num_analysis_threads; i++) {
        workers.emplace_back(&ThreadedArbitrageSystem::process_exchange_data, this);
    }

    workers.emplace_back(&ThreadedArbitrageSystem::process_opportunities, this);

    Utils::safe_print("Starting arbitrage system - monitoring " + 
                     std::to_string(config.getSymbols().size()) + " trading pairs...");

    while (running) {
        for (const auto& symbol : config.getSymbols()) {
            fetch_exchange_data(symbol);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ThreadedArbitrageSystem::stop() {
    running = false;
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

ThreadedArbitrageSystem::~ThreadedArbitrageSystem() {
    stop();
}