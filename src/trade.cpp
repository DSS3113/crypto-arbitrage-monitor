#include "trade.hpp"

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    os << "Trade{"
       << "price: " << trade.price
       << ", quantity: " << trade.quantity
       << ", timestamp: " << trade.timestamp
       << ", is_buyer_maker: " << (trade.is_buyer_maker ? "true" : "false")
       << "}";
    return os;
}