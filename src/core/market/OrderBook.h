#pragma once
#include "BucketSet.h"
#include "enums.h"
#include <json/json.h>
#include <optional>
#include <memory>
#include <utility>

class OrderBook
{
public:
    void sync_book(const int64_t update_id, const Json::Value &bid, const Json::Value &ask)
    {
        bid_levels.clear();
        for (Json::Value::ArrayIndex i = 0; i != bid.size(); i++)
        {
            Price price{static_cast<float>(atof(bid[i][0].asString().c_str()))};
            float qty = static_cast<float>(atof(bid[i][1].asString().c_str()));
            bid_levels.insert(std::make_pair(price, std::make_shared<Level>(Level(price, qty, Side::BID))));
        }
        ask_levels.clear();
        for (Json::Value::ArrayIndex i = 0; i != ask.size(); i++)
        {
            Price price{static_cast<float>(atof(bid[i][0].asString().c_str()))};
            float qty = static_cast<float>(atof(bid[i][1].asString().c_str()));
            ask_levels.insert(std::make_pair(price, std::make_shared<Level>(Level(price, qty, Side::ASK))));
        }
        last_update_id = update_id;
    }

    std::optional<DeltaBook> update_book(const int64_t update_id, const Level &level)
    {
        /* 
            Get updated level and update Book and returns the change of level
        */
        if (update_id <= last_update_id)
        {
            return std::nullopt;
        }

        last_update_id = update_id;
        // TODO - Simplify these conditions
        if (level.side == Side::BID)
        {
            if (level.quantity == 0)
            {
                auto book_level = bid_levels.find(level.price);
                if (book_level == nullptr)
                {
                    return std::nullopt;
                }
                DeltaBook update_msg{level.price, book_level->quantity, DeltaType::DELETE, level.side};
                bid_levels.remove(level.price);
                return update_msg;
            }
            else
            {
                auto book_level = bid_levels.find(level.price);
                if (book_level == nullptr)
                {
                    DeltaType type = DeltaType::ADD_LEVEL;
                    bid_levels.insert(std::make_pair(level.price, std::make_shared<Level>(Level(level.price, level.quantity, level.side))));
                    DeltaBook update_msg{level.price, level.quantity, type, level.side};
                    return update_msg;
                }
                else
                {
                    DeltaType type = DeltaType::ADD;
                    if (book_level->quantity > level.quantity)
                    {
                        type = DeltaType::REDUCE;
                    }

                    DeltaBook update_msg{level.price, std::abs(book_level->quantity - level.quantity), type, level.side};
                    book_level->quantity = level.quantity;
                    return update_msg;
                }
            }
        }
        else if (level.side == Side::ASK)
        {
            if (level.quantity == 0)
            {
                auto book_level = ask_levels.find(level.price);
                if (book_level == nullptr)
                {
                    return std::nullopt;
                }
                DeltaBook update_msg{level.price, std::abs(book_level->quantity - level.quantity), DeltaType::DELETE, level.side};
                ask_levels.remove(level.price);
                return update_msg;
            }
            else
            {
                auto book_level = ask_levels.find(level.price);
                if (book_level == nullptr)
                {
                    DeltaType type = DeltaType::ADD_LEVEL;
                    ask_levels.insert(std::make_pair(level.price, std::make_shared<Level>(Level(level.price, level.quantity, level.side))));
                    DeltaBook update_msg{level.price, level.quantity, type, level.side};
                    return update_msg;
                }
                else
                {
                    DeltaType type = DeltaType::ADD;
                    if (book_level->quantity > level.quantity)
                    {
                        type = DeltaType::REDUCE;
                    }
                    DeltaBook update_msg{level.price, book_level->quantity, type, level.side};
                    book_level->quantity = level.quantity;
                    return update_msg;
                }
            }
        }
        return std::nullopt;
    }

    inline int64_t get_last_update_id() const { return last_update_id; }

private:
    int64_t exchange_time = 0;
    int64_t last_update_id = 0;
    BucketSet<Level> bid_levels;
    BucketSet<Level> ask_levels;
};
