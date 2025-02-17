// config.cpp
#include "config.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << filename << std::endl;
            return false;
        }

        json j;
        file >> j;

        auto exchangesFromJson = j["exchanges"];
        for (const auto& [exchange, details] : exchangesFromJson.items()) {
            apiUrls[exchange] = details["api_url"];
            req_timeouts[exchange] = details["req_timeout"];
        }

        auto symbolsFromJson = j["symbols"];
        for (const auto& symbol : symbolsFromJson) {
            symbols.insert(symbol);
        }

        num_threads = j["num_threads"];
        feedback_interval= j["feedback_interval"];
        min_profit_percentage = j["min_profit_percentage"];
        trading_fees = j["trading_fees"];
        min_trade_size = j["min_trade_size"];

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        return false;
    }
}

std::string Config::getApiUrl(const std::string& exchange_name) const {
    auto it = apiUrls.find(exchange_name);
    if (it == apiUrls.end()) {
        throw std::runtime_error("No API URL found for exchange: " + exchange_name);
    }
    return it->second;
}

const std::unordered_set<std::string>& Config::getSymbols() const {
    return symbols;
}

unsigned int Config::getReqTimeout(const std::string& exchange_name) const {
    auto it = req_timeouts.find(exchange_name);
    if (it == req_timeouts.end()) {
        throw std::runtime_error("No timeout setting found for exchange: " + exchange_name);
    }
    return it->second;
}

unsigned int Config::getNumThreads() const {
    return num_threads;
}

unsigned int Config::getFeedbackInterval() const {
    return feedback_interval;
}

double Config::getMinProfitPercentage() const {
    return min_profit_percentage;
}

double Config::getTradingFees() const {
    return trading_fees;
}

double Config::getMinTradeSize() const { 
    return min_trade_size;
}