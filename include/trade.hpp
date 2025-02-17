// trade.hpp

#pragma once

#include <iostream>

struct Trade {
    double price;
    double quantity;
    long long timestamp;
    bool is_buyer_maker;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
};