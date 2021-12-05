#include <grpc++/grpc++.h>

#include "ticker.grpc.pb.h"
#include "binacpp_websocket.h"
#include "binacpp_logger.h"
#include <json/json.h>
#include "../binance/web_stream_data.h"
#include "../binance/BinanceDataProvider.h"
#include "../techical_indicators/Ema.h"
#include "../techical_indicators/RSI.h"
#include "../core/market/OrderBook.h"
#include "../core/market/SimpleOrderBook.h"
#include <string>
#include <thread>

using grpc::ServerContext;
using grpc::Status;
using ticker::Integer;
using ticker::TickData;
using ticker::Ticker;

enum Service
{
    EMA = 0
};

class ServiceLoop
{
    // TODO - Configurator for Services
    // Services list
    // Service lookup
    // implement Service Loop up to register and fetch service

public:
    void init_data_provider()
    {
        t1 = std::thread(&BinanceDataProvider::start_stream);
    }

    void start_service_loop()
    {
        ema_service.init_service_loop();
        rsi_service.init_service_loop();
    }

    float get_value()
    {
        return ema_service.get_value();
    }

    Ema<10> ema_service;
    RSI<14> rsi_service;

private:
    std::thread t1;
};

// Logic and data behind the server's behavior.
class TickerServiceImpl final : public Ticker::Service
{

public:
    TickerServiceImpl()
    {
        service_loop.init_data_provider();
        service_loop.start_service_loop();
    }

private:
    Status EmaTick(ServerContext *context, const Integer *request, TickData *reply) override
    {
        long millis = BinanceDataProvider::last_kline.start_time;
        google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
        timestamp->set_seconds(millis / 1000);
        timestamp->set_nanos((int)((millis % 1000) * 1000000));
        reply->set_allocated_time(timestamp);
        reply->set_price(service_loop.get_value());
        return Status::OK;
    }
    ServiceLoop service_loop;
};
