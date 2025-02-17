// config.hpp

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class Config {
public:
    static Config& getInstance();
    bool loadFromFile(const std::string& filename);
    std::string getApiUrl(const std::string& exchange_name) const;
    unsigned int getReqTimeout(const std::string& exchange_name) const;
    const std::unordered_set<std::string>& getSymbols() const;
    unsigned int getNumThreads() const;
    unsigned int getFeedbackInterval() const;
    double getMinProfitPercentage() const;
    double getTradingFees() const;
    double getMinTradeSize() const;

private:
    Config() = default;  // Singleton pattern
    std::unordered_map<std::string, std::string> apiUrls;
    std::unordered_set<std::string> symbols;
    std::unordered_map<std::string, unsigned int> req_timeouts;
    unsigned int num_threads = 4;
    unsigned int feedback_interval = 10;
    double min_profit_percentage = 0.5;
    double trading_fees = 0.1;
    double min_trade_size =  0.001;
};