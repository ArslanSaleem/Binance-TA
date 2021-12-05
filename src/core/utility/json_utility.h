#pragma once
#include <json/json.h>

namespace json::utils
{
    float to_float(const Json::Value &val)
    {
        return static_cast<float>(atof(val.asString().c_str()));
    }
}