// arbitrageAnalyzer.cpp

#include "arbitrageAnalyzer.hpp"
#include "arbitrageOpportunity.hpp"

ArbitrageAnalyzer::ArbitrageAnalyzer() : 
    config(Config::getInstance()) {}

std::vector<ArbitrageOpportunity> ArbitrageAnalyzer::findOpportunities(
    const std::string& symbol,
    const std::vector<Trade>& exchange1_trades,
    const std::string& exchange1_name,
    const std::vector<Trade>& exchange2_trades,
    const std::string& exchange2_name,
    double min_profit_percent
) {
    min_profit_percent = config.getMinProfitPercentage();
    std::vector<ArbitrageOpportunity> opportunities;

    if (exchange1_trades.empty() || exchange2_trades.empty()) {
        return opportunities;
    }

    // Get latest prices from each exchange
    double price1 = exchange1_trades[0].price;
    double price2 = exchange2_trades[0].price;

    // Calculate potential profit percentages both ways
    double profit1_2 = ((price2 - price1) / price1) * 100;
    double profit2_1 = ((price1 - price2) / price2) * 100;

    // Check for opportunities in both directions
    if (profit1_2 > min_profit_percent) {
        opportunities.push_back({
            symbol,
            exchange1_name,
            exchange2_name,
            price1,
            price2,
            profit1_2
        });
    }

    if (profit2_1 > min_profit_percent) {
        opportunities.push_back({
            symbol,
            exchange2_name,
            exchange1_name,
            price2,
            price1,
            profit2_1
        });
    }

    return opportunities;
}


bool ArbitrageAnalyzer::isViableOpportunity(
    const ArbitrageOpportunity& opp, 
    double trading_fees,
    double min_trade_size
) {
    trading_fees = config.getTradingFees();
    min_trade_size = config.getMinTradeSize();

    // Factor in trading fees
    double net_profit = opp.profit_percentage - (2 * trading_fees);  // Two trades

    // Consider minimum trade size
    double min_cost = min_trade_size * opp.buy_price;

    return net_profit > 0 && min_cost < 1000;  // viability threshold
}