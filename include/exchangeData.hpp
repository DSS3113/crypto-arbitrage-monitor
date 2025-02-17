// exchangeData.hpp

#pragma once

#include "trade.hpp"

struct ExchangeData {
    std::string symbol;
    std::vector<Trade> trades;
    std::string exchange_name;
};