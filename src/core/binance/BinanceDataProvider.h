#pragma once
#include "web_stream_data.h"
#include "../market/OrderBook.h"
#include "../market/SimpleOrderBook.h"
#include "binance_api_path_gen.h"

class BinanceDataProvider
{
public:
    static void start_stream()
    {
        // Start Event thread
        // Klines/Candlestick update via websocket
        int (*kline)(Json::Value &) = &BinanceDataProvider::ws_klines_onData;
        int (*depth)(Json::Value &) = &BinanceDataProvider::ws_depth_data;
        int (*ticker)(Json::Value &) = &BinanceDataProvider::ws_ticker_data;
        int (*trade_data)(Json::Value &) = &BinanceDataProvider::ws_trade_message_data;
        int (*agg_trade_data)(Json::Value &) = &BinanceDataProvider::ws_agg_trade_message_data;
        int (*partial_order_book)(Json::Value &) = &BinanceDataProvider::ws_partial_order_book;
        int (*mini_ticker_stream)(Json::Value &) = &BinanceDataProvider::ws_mini_ticker_stream;

        BinaCPP_websocket::init();
        BinaCPP_websocket::connect_endpoint(kline, "/ws/btcbusd@kline_1m");
        BinaCPP_websocket::connect_endpoint(depth, "/ws/btcbusd@depth@100ms");
        BinaCPP_websocket::connect_endpoint(partial_order_book, "/ws/btcbusd@depth5@100ms");
        BinaCPP_websocket::connect_endpoint(ticker, "/ws/btcbusd@bookTicker");
        BinaCPP_websocket::connect_endpoint(trade_data, "/ws/btcbusd@trade");
        BinaCPP_websocket::connect_endpoint(agg_trade_data, "/ws/btcbusd@aggTrade");
        BinaCPP_websocket::connect_endpoint(mini_ticker_stream, "/ws/btcbusd@miniTicker");
        BinaCPP_websocket::enter_event_loop();
    }

    static Binance::KLine last_kline;
    static SimpleOrderBook<5> order_book;
    static Binance::Ticker tick_data;
    static Binance::TradeMessage last_trade_message;
    static Binance::AggregatedTrade last_agg_trade_message;
    static Binance::DeltaBook delta_book;
    static Binance::MiniTickerStream mini_ticker_data;

private:
    static int ws_klines_onData(Json::Value &json_result)
    {
        Binance::KLine kline_data{
            json_result["E"].asInt64(),                   // Event time
            json_result["k"]["t"].asInt64(),              // Kline start time
            json_result["k"]["T"].asInt64(),              // Kline close time
            json::utils::to_float(json_result["k"]["h"]), // high Price
            json::utils::to_float(json_result["k"]["l"]), // low Price
            json::utils::to_float(json_result["k"]["o"]), // open Price
            json::utils::to_float(json_result["k"]["c"]), // close Price
            json::utils::to_float(json_result["k"]["q"]), // volume
            json_result["k"]["n"].asUInt64(),             // number of trades
            json_result["k"]["x"].asBool(),               // is candlestick close
            json_result["k"]["f"].asUInt64(),             // first trade id
            json_result["k"]["L"].asUInt64(),             // last trade id
            json::utils::to_float(json_result["k"]["Q"]),
            json::utils::to_float(json_result["k"]["q"]) - json::utils::to_float(json_result["k"]["Q"])};
        last_kline = kline_data;
    }

    static int ws_depth_data(Json::Value &json_result)
    {

        delta_book.bid.clear();
        delta_book.ask.clear();
        delta_book.last_update_id = json_result["u"].asInt64();
        Json::Value &bid_side = json_result["b"];
        for (Json::Value::ArrayIndex i = 0; i != bid_side.size(); i++)
        {
            delta_book.bid.emplace_back(Level(Price(bid_side[i][0]), json::utils::to_float(bid_side[i][1]), Side::BID));
        }
        Json::Value &ask_side = json_result["a"];
        for (Json::Value::ArrayIndex i = 0; i != ask_side.size(); i++)
        {
            delta_book.bid.emplace_back(Level(Price(ask_side[i][0]), json::utils::to_float(ask_side[i][1]), Side::ASK));
        }
        // [Deprecated] Used in case maintaining orderbook based on events
        // auto last_update_id = order_book.get_last_update_id();
        // int64_t u = json_result["u"].asInt64();
        // int64_t U = json_result["U"].asInt64();

        // if (u <= last_update_id)
        // {
        //     return 0;
        // }
        // if (u > last_update_id)
        // {
        //     Json::Value &bidSide = json_result["b"];
        //     for (Json::Value::ArrayIndex i = 0; i != bidSide.size(); i++)
        //     {
        //         Price price{static_cast<float>(atof(bidSide[i][0].asString().c_str()))};
        //         float qty = static_cast<float>(atof(bidSide[i][1].asString().c_str()));
        //         order_book.update_book(u, Level(price, qty, Side::BID));
        //     }
        //     Json::Value &askSide = json_result["a"];
        //     for (Json::Value::ArrayIndex i = 0; i != askSide.size(); i++)
        //     {
        //         Price price{static_cast<float>(atof(askSide[i][0].asString().c_str()))};
        //         float qty = static_cast<float>(atof(askSide[i][1].asString().c_str()));
        //         order_book.update_book(u, Level(price, qty, Side::ASK));
        //     }
        // }
        // order_book.iterate_book();
    }

    static int ws_ticker_data(Json::Value &json_result)
    {
        float bid_qty = static_cast<float>(atof(json_result["B"].asString().c_str()));
        float ask_qty = static_cast<float>(atof(json_result["A"].asString().c_str()));
        tick_data = Binance::Ticker(json_result["u"].asInt64(), Price(json_result["b"]), bid_qty, Price(json_result["a"]), ask_qty);
    }

    static int ws_trade_message_data(Json::Value &json_result)
    {
        last_trade_message = Binance::TradeMessage(json_result["t"].asInt64(), json_result["T"].asInt64(), json_result["E"].asInt64(), Price(json_result["p"]), json::utils::to_float(json_result["q"]),
                                                   json_result["b"].asInt64(), json_result["a"].asInt64(), json_result["m"].asBool());
    }

    static int ws_agg_trade_message_data(Json::Value &json_result)
    {
        last_agg_trade_message = Binance::AggregatedTrade(json_result["T"].asInt64(), json_result["E"].asInt64(), Price(json_result["p"]), json::utils::to_float(json_result["q"]),
                                                          json_result["f"].asInt64(), json_result["l"].asInt64(), json_result["m"].asBool());
    }

    static int ws_partial_order_book(Json::Value &json_result)
    {
        order_book.sync_book(json_result["lastUpdateId"].asInt64(), json_result["bids"], json_result["asks"]);
        // order_book.print_book();
    }

    static int ws_mini_ticker_stream(Json::Value &json_result)
    {
        mini_ticker_data.time = json_result["E"].asInt64();
        mini_ticker_data.open = Price(json_result["o"]);
        mini_ticker_data.close = Price(json_result["c"]);
        mini_ticker_data.high = Price(json_result["h"]);
        mini_ticker_data.low = Price(json_result["l"]);
        mini_ticker_data.v = json::utils::to_float(json_result["v"]);
        mini_ticker_data.q = json::utils::to_float(json_result["q"]);
    }
};

Binance::KLine BinanceDataProvider::last_kline;
SimpleOrderBook<5> BinanceDataProvider::order_book;
Binance::Ticker BinanceDataProvider::tick_data;
Binance::TradeMessage BinanceDataProvider::last_trade_message;
Binance::AggregatedTrade BinanceDataProvider::last_agg_trade_message;
Binance::DeltaBook BinanceDataProvider::delta_book;
Binance::MiniTickerStream BinanceDataProvider::mini_ticker_data;
