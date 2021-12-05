#pragma once
#include <json/json.h>
#include "../utility/json_utility.h"

struct Price
{
    float value = 0.f;
    Price() = default;
    Price(const float val) : value(val) {}
    Price(const Json::Value &val)
    {
        value = json::utils::to_float(val);
    }
    operator float() const { return value; }
    Price operator+(Price &obj)
    {
        Price temp;
        temp.value = this->value + obj.value;
        return temp;
    }
    Price operator-(Price &obj)
    {
        Price temp;
        temp.value = this->value - obj.value;
        return temp;
    }
    bool operator>(Price &other) const { return this->value > other.value; }
    bool operator<(Price &other) const { return this->value > other.value; }
    bool operator==(const Price &other) const { return this->value == other.value; }
    bool operator!=(const Price &other) const { return this->value == other.value; }
};
