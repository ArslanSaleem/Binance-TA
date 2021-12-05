#pragma once
#include "../services/ServiceInterface.h"
#include "../binance/web_stream_data.h"

template <size_t days = 100>
class Ema : public ServiceInterface
{
    float ema = 0.f;
    float multiplier = 0.f;
    int64_t last_timestamp = 0;

    void notify()
    {
        notify(data_provider::last_kline);
    }

public:
    Ema()
    {
        multiplier = 2.f / (days + 1);
    }

    void notify(const Binance::KLine &candleStick)
    {
        if (last_timestamp != candleStick.timestamp)
        {
            if (ema == 0.f)
            {
                ema = candleStick.close;
            }
            ema = (multiplier * candleStick.close) + (1.f - multiplier) * ema;
            last_timestamp = candleStick.timestamp;
        }
    }

    float get_value()
    {
        return ema;
    }
};
