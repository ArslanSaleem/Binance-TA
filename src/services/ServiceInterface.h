#pragma once
#include "BinanceObjects.h"

class ServiceInterface
{
public:
    virtual void notify(const KLine &candleStick) = 0;
    virtual float get_value() = 0;
};
