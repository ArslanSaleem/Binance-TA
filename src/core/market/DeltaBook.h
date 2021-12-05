#pragma once
#include "Price.h"
#include "enums.h"

struct DeltaBook
{
    Price price;
    float quantity = 0.f;
    DeltaType type = DeltaType::UNKNOWN;
    Side side = Side::UNKNOWN;
    DeltaBook() = default;
    DeltaBook(const Price price_, const float quantity_, const DeltaType type_, const Side side_) : price(price_), quantity(quantity_), type(type_), side(side_) {}
};
