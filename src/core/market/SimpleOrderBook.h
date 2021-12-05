#pragma once
#include "Level.h"
#include <algorithm>

template <size_t levels = 5>
class SimpleOrderBook
{
public:
    void sync_book(const int64_t update_id, const Json::Value &bid, const Json::Value &ask)
    {
        for (Json::Value::ArrayIndex i = 0; i != std::min(levels, size_t(bid.size())); i++)
        {
            bid_levels[i].price = Price{static_cast<float>(atof(bid[i][0].asString().c_str()))};
            bid_levels[i].quantity = static_cast<float>(atof(bid[i][1].asString().c_str()));
        }
        for (Json::Value::ArrayIndex i = 0; i != std::min(levels, size_t(ask.size())); i++)
        {
            ask_levels[i].price = Price{static_cast<float>(atof(ask[i][0].asString().c_str()))};
            ask_levels[i].quantity = static_cast<float>(atof(ask[i][1].asString().c_str()));
        }
        last_update_id = update_id;
    }

    void print_book()
    {
        std::cout << " Price        |       Shares \n";
        std::cout << "--------------------------------\n";
        std::cout.precision(10);
        for (int i = levels - 1; i > -1; i--)
        {
            std::cout << ask_levels[i].price << "     |      " << ask_levels[i].quantity << "\n";
        }
        std::cout << "--------------------------------\n";
        for (size_t i = 0; i < levels; i++)
        {
            std::cout << bid_levels[i].price << "     |      " << bid_levels[i].quantity << "\n";
        }
    }

private:
    int64_t last_update_id = 0;
    Level bid_levels[levels];
    Level ask_levels[levels];
};
