#pragma once
#include <thread>
#include "../binance/web_stream_data.h"

class ServiceInterface
{
    std::thread t1;

protected:
    using data_provider = BinanceDataProvider;

public:
    virtual void notify() = 0;
    virtual float get_value() = 0;
    void service_loop()
    {
        while (1)
        {
            notify();
        }
    }
    void init_service_loop()
    {
        t1 = std::thread(&ServiceInterface::service_loop, this);
    }
};