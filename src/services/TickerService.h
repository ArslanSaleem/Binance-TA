#include <grpc++/grpc++.h>

#include "ticker.grpc.pb.h"
#include "binacpp_websocket.h"
#include <json/json.h>
#include "BinanceObjects.h"
#include "techical_indicators/Ema.h"
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
    static EmaService ema_service;

    std::thread t1;

    static int ws_klines_onData(Json::Value &json_result)
    {

        KLine kline_data{
            json_result["E"].asInt64(),                                         // Event time
            json_result["k"]["t"].asInt64(),                                    // Kline start time
            json_result["k"]["T"].asInt64(),                                    // Kline close time
            static_cast<float>(atof(json_result["k"]["h"].asString().c_str())), // high Price
            static_cast<float>(atof(json_result["k"]["l"].asString().c_str())), // low Price
            static_cast<float>(atof(json_result["k"]["o"].asString().c_str())), // open Price
            static_cast<float>(atof(json_result["k"]["c"].asString().c_str())), // close Price
            static_cast<float>(atof(json_result["k"]["q"].asString().c_str())), // volume
            json_result["k"]["n"].asUInt64(),                                   // number of trades
            json_result["k"]["x"].asBool(),                                     // is candlestick close
            json_result["k"]["f"].asUInt64(),                                   // first trade id
            json_result["k"]["L"].asUInt64(),                                   // last trade id
            static_cast<float>(atof(json_result["k"]["Q"].asString().c_str())),
            static_cast<float>(atof(json_result["k"]["q"].asString().c_str())) - static_cast<float>(atof(json_result["k"]["Q"].asString().c_str())),
        };
        ema_service.notify(kline_data);
        lastKline = kline_data;
    }

public:
    static KLine lastKline;
    void start_service_loop()
    {
        // Start Event thread
        // Klines/Candlestick update via websocket
        int (*func)(Json::Value &) = &ServiceLoop::ws_klines_onData;
        BinaCPP_websocket::init();
        BinaCPP_websocket::connect_endpoint(func, "/ws/btcbusd@kline_1m");
        t1 = std::thread(&BinaCPP_websocket::enter_event_loop);
    }
    float get_value()
    {
        return ema_service.get_value();
    }
};

// map<long, map<string, double> > ServiceLoop::klinesCache;
EmaService ServiceLoop::ema_service = EmaService(100);
KLine ServiceLoop::lastKline;

// Logic and data behind the server's behavior.
class TickerServiceImpl final : public Ticker::Service
{
public:
    TickerServiceImpl()
    {

        service_loop.start_service_loop();
    }

private:
    Status EmaTick(ServerContext *context, const Integer *request, TickData *reply) override
    {
        long millis = service_loop.lastKline.timestamp;
        google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();

        timestamp->set_seconds(millis / 1000);
        timestamp->set_nanos((int)((millis % 1000) * 1000000));
        std::cout << "ioioioi\n";
        std::cout << service_loop.lastKline.start_time << "\n";
        std::cout << service_loop.lastKline.end_time << "\n";
        std::cout << service_loop.lastKline.timestamp << "\n";

        // google::protobuf::Timestamp *timestamp = google::protobuf::Timestamp()
        // .newBuilder().setSeconds(millis / 1000).setNanos((int)((millis % 1000) * 1000000)).build();

        reply->set_allocated_time(timestamp);
        reply->set_price(service_loop.get_value());
        return Status::OK;
    }
    ServiceLoop service_loop;
};
