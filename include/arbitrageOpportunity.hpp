// arbitrageOpportunity.hpp

#pragma once

#include <string>
#include <iostream>

struct ArbitrageOpportunity {
    std::string symbol;
    std::string buy_exchange;
    std::string sell_exchange;
    double buy_price;
    double sell_price;
    double profit_percentage;
    
    friend std::ostream& operator<<(std::ostream& os, const ArbitrageOpportunity& opp);
};