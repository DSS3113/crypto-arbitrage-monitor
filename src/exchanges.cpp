#include "exchanges.hpp"
#include "config.hpp"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <ctime>

#include <fstream>
#include <iostream>

 // Convert symbol format e.g. "BTC/USDT" to "BTCUSDT"
std::string formatSymbol(const std::string& symbol) {
    std::string formatted_symbol = symbol;
    formatted_symbol.erase(
        std::remove(formatted_symbol.begin(), formatted_symbol.end(), '/'), 
        formatted_symbol.end()
    );

    return formatted_symbol;
}

BinanceExchange::BinanceExchange(const Config& config) {
    apiUrl = config.getApiUrl("binance");
    req_timeout = config.getReqTimeout("binance");
}

KrakenExchange::KrakenExchange(const Config& config) {
    apiUrl = config.getApiUrl("kraken");
    req_timeout = config.getReqTimeout("kraken");
}

std::vector<Trade> BinanceExchange::parseTrades(const std::string& response) {
    std::vector<Trade> trades;
    try {
        auto j = nlohmann::json::parse(response);
        
        // Binance returns an array of trades
        for (const auto& trade : j) {
            Trade t;
            t.price = std::stod(trade["price"].get<std::string>());
            t.quantity = std::stod(trade["qty"].get<std::string>());
            t.timestamp = trade["time"].get<long long>();
            t.is_buyer_maker = trade["isBuyerMaker"].get<bool>();
            trades.push_back(t);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing Binance response: " << e.what() << std::endl;
    }
    return trades;
}

std::vector<Trade> KrakenExchange::parseTrades(const std::string& response) {
    std::vector<Trade> trades;
    try {
        auto j = nlohmann::json::parse(response);
        
        // Check for errors
        if (!j["error"].empty()) {
            throw std::runtime_error("Kraken API error: " + j["error"].dump());
        }

        // Kraken returns XBTUSDT for BTC/USDT
        const auto& trades_array = j["result"]["XBTUSDT"];
        
        for (const auto& trade : trades_array) {
            Trade t;
            t.price = std::stod(trade[0].get<std::string>());  // Price is first element
            t.quantity = std::stod(trade[1].get<std::string>());  // Quantity is second
            t.timestamp = static_cast<long long>(trade[2].get<double>() * 1000); // Convert to ms
            t.is_buyer_maker = (trade[3] == "b");  // "b" for buy, "s" for sell
            trades.push_back(t);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing Kraken response: " << e.what() << std::endl;
    }
    return trades;
}

std::vector<Trade> BinanceExchange::fetchTrades(const std::string& symbol) {
    std::string formatted_symbol = formatSymbol(symbol);
    
    // Make the request
    auto response = cpr::Get(cpr::Url{apiUrl},
        cpr::Parameters{
            {"symbol", formatted_symbol}, 
            {"limit", "200"}
        },
        cpr::Header{
            {"Accept", "application/json"}
        },
        cpr::Timeout{req_timeout});

    if (response.error) {
        std::cerr << "Error: " << response.error.message << std::endl;
        return {};
    }

    if (response.status_code != 200) {
        std::cerr << "Request failed with status code: " << response.status_code << std::endl;
        std::cerr << "Response body: " << response.text << std::endl;
        return {};
    }

    // std::cout << "Success!" << std::endl;

    try {
        return parseTrades(response.text);
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<Trade> KrakenExchange::fetchTrades(const std::string& symbol) {
    std::string formatted_symbol = formatSymbol(symbol);
    
    time_t timestampNow = std::time(nullptr);
    std::stringstream timestampNowSs;
    timestampNowSs << timestampNow-100;
    std::string timestampNowStr = timestampNowSs.str();

    // Make the request
    auto response = cpr::Get(cpr::Url{apiUrl},
        cpr::Parameters{
            {"pair", formatted_symbol},
            {"since", timestampNowStr},
            {"limit", "200"}
        },
        cpr::Header{
            {"Accept", "application/json"}
        },
        cpr::Timeout{req_timeout});

    if (response.error) {
        std::cerr << "Error: " << response.error.message << std::endl;
        return {};
    }

    if (response.status_code != 200) {
        std::cerr << "Request failed with status code: " << response.status_code << std::endl;
        std::cerr << "Response body: " << response.text << std::endl;
        return {};
    }

    // std::cout << "Success!" << std::endl;

    try {
        return parseTrades(response.text);
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return {};
    }
}

BinanceExchange::~BinanceExchange() {
    // std::cout<< "works" << std::endl;
}

KrakenExchange::~KrakenExchange() {
    // std::cout<< "works" << std::endl;
}