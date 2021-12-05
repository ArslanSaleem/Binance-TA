#pragma once
#include "../services/ServiceInterface.h"
#include "utils.h"

template <size_t num_of_candles = 14>
class RSI : public ServiceInterface
{
    float value = 0.f;
    int64_t last_timestamp = 0;
    int64_t candle_stick_start_time = 0;
    float candle_gain = 0.f;
    float candle_loss = 0.f;
    float sum_of_gain = 0.f;
    float sum_of_loss = 0.f;
    CircularBuffer<float, num_of_candles> buffer[2];
    Price last_close_price;
    size_t iterator = 0;
    float rsi = 0.f;
    void notify()
    {
        notify(data_provider::last_kline);
    }

public:
    void notify(const Binance::KLine &candleStick)
    {
        if (last_timestamp != candleStick.timestamp)
        {
            if (last_close_price == Price())
            {
                last_close_price = candleStick.close;
            }
            else
            {
                if (last_close_price < candleStick.close)
                {
                    candle_gain += (candleStick.close - last_close_price);
                }
                else if (last_close_price > candleStick.close)
                {
                    candle_loss += (last_close_price - candleStick.close);
                }
            }

            last_timestamp = candleStick.timestamp;
            last_close_price = candleStick.close;
            iterator++;
        }

        if (candle_stick_start_time != candleStick.start_time)
        {
            sum_of_gain += candle_gain - buffer[0].get_oldest_value();
            sum_of_loss += candle_loss - buffer[1].get_oldest_value();
            buffer[0].update(candle_gain);
            buffer[1].update(candle_loss);
            rsi = sum_of_loss <= 0.f ? 1.f : 100.f - (100.f / (1.f + (sum_of_gain / sum_of_loss)));
            candle_gain = 0.f;
            candle_loss = 0.f;
            candle_stick_start_time = candleStick.start_time;
        }
    }
    float get_value()
    {
        return rsi;
    }
};
