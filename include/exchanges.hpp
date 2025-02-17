// excahnges.hpp

#pragma once

#include "config.hpp"
#include "trade.hpp"

#include <vector>
#include <thread>
#include <sstream>
#include <string>
#include <unordered_map>

class Exchange {
private:
    std::string apiUrl;
public:
    
    virtual ~Exchange() = default;
    virtual std::vector<Trade> fetchTrades(const std::string& symbol) = 0;
    virtual std::vector<Trade> parseTrades(const std::string& response) = 0;
};

class BinanceExchange : public Exchange {
    // Implementation for Binance
private:
    unsigned int req_timeout;
    std::string apiUrl;
public:
    BinanceExchange(const Config& config);
    ~BinanceExchange();
    std::vector<Trade> fetchTrades(const std::string &symbol);
    std::vector<Trade> parseTrades(const std::string& response);
};

class KrakenExchange : public Exchange {
    // Implementation for Kraken
private:
    unsigned int req_timeout;
    std::string apiUrl;
public:
    KrakenExchange(const Config& config);
    ~KrakenExchange();
    std::vector<Trade> fetchTrades(const std::string &symbol);
    std::vector<Trade> parseTrades(const std::string& response);
};

