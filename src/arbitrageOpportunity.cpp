#include "arbitrageOpportunity.hpp"

std::ostream& operator<<(std::ostream& os, const ArbitrageOpportunity& opp) {
    os << "ArbitrageOpportunity{"
       << "symbol: " << opp.symbol
       << ", buy: " << opp.buy_exchange << " @ " << opp.buy_price
       << ", sell: " << opp.sell_exchange << " @ " << opp.sell_price
       << ", profit: " << opp.profit_percentage << "%"
       << "}";
    return os;
}