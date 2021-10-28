#include <grpc++/grpc++.h>

#include "ticker.grpc.pb.h"
#include "binacpp_websocket.h"
#include <json/json.h>
#include "BinanceObjects.h"

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
    static map<long, map<string, double> > klinesCache;

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
        std::cout << "*************************************************\n";
        std::cout << kline_data;
    }

public:
    void start_service_loop()
    {
        // Start Event thread
        // Klines/Candlestick update via websocket
        int (*func)(Json::Value &) = &ServiceLoop::ws_klines_onData;
        BinaCPP_websocket::init();
        BinaCPP_websocket::connect_endpoint(func, "/ws/btcbusd@kline_1m");
        BinaCPP_websocket::connect_endpoint(func, "/ws/btcbusd@kline_5m");
        BinaCPP_websocket::enter_event_loop();
    }
};

map<long, map<string, double> > ServiceLoop::klinesCache;

// Logic and data behind the server's behavior.
class TickerServiceImpl final : public Ticker::Service
{
    Status EmaTick(ServerContext *context, const Integer *request, TickData *reply) override
    {
        google::protobuf::Timestamp *timestamp = new google::protobuf::Timestamp();
        timestamp->set_seconds(10000);
        timestamp->set_nanos(100);
        reply->set_allocated_time(timestamp);
        return Status::OK;
    }
};
