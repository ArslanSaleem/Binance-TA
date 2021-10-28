#pragma once
#include "../ServiceInterface.h"
#include "../BinanceObjects.h"

class EmaService : ServiceInterface
{
    float ema = 0.f;
    float multiplier = 0.f;
    size_t no_of_days = 100;

public:
    EmaService(const size_t no_of_days_) : no_of_days(no_of_days_)
    {
        multiplier = 2.f / (no_of_days + 1);
    }
    void notify(const KLine &candleStick)
    {
        if (ema == 0.f)
        {
            ema = candleStick.close;
        }
        ema = (multiplier * candleStick.close) + (1.f - multiplier) * ema;
    }

    float get_value()
    {
        return ema;
    }
};
