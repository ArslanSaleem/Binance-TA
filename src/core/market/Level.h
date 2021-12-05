#pragma once
#include "DeltaBook.h"
#include "Price.h"

struct Level
{
    Price price;
    float quantity = 0.0;
    Side side = Side::UNKNOWN;
    Level() = default;
    Level(const Price price_, float quantity_, const Side side_) : price(price_), quantity(quantity_), side(side_) {}
    static Level create_level(const Json::Value &price, const Json::Value &qty, const Side &side)
    {
        Price price_{static_cast<float>(atof(price.asString().c_str()))};
        float qty_ = static_cast<float>(atof(qty.asString().c_str()));
        return Level(price_, qty_, side);
    }
    static std::shared_ptr<Level> create_level_ptr(const Json::Value &price, const Json::Value &qty, const Side &side)
    {
        Price price_{static_cast<float>(atof(price.asString().c_str()))};
        float qty_ = static_cast<float>(atof(qty.asString().c_str()));
        return std::make_shared<Level>(Level(price_, qty_, side));
    }
};
