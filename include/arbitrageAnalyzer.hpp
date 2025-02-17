// arbitrageAnalyzer.hpp

#pragma once

#include "arbitrageOpportunity.hpp"
#include "config.hpp"
#include "trade.hpp"

#include <string>
#include <vector>
#include <iostream>


class ArbitrageAnalyzer {
private:
    Config& config;

public:
    ArbitrageAnalyzer();
    std::vector<ArbitrageOpportunity> findOpportunities(
        const std::string& symbol,
        const std::vector<Trade>& exchange1_trades,
        const std::string& exchange1_name,
        const std::vector<Trade>& exchange2_trades,
        const std::string& exchange2_name,
        double min_profit_percent = 0.5
    );
    // Advanced analysis considering fees, liquidity, etc.
    bool isViableOpportunity(const ArbitrageOpportunity& opp, 
                            double trading_fees = 0.1,  // 0.1% per trade
                            double min_trade_size = 0.001);
};