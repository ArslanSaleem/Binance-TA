#pragma once
#include <iostream>
#include "../market/Price.h"
#include "../market/Level.h"

namespace Binance
{
    // Data Feed of Binance
    struct KLine
    {
        int64_t timestamp = 0;
        int64_t start_time = 0;
        int64_t end_time = 0;
        float high = 0.f;
        float low = 0.f;
        float open = 0.f;
        float close = 0.f;
        float volume = 0.f;
        std::size_t num_of_trades = 0;
        bool is_kline_closed = false;
        std::size_t first_trade_id = 0;
        std::size_t last_trade_id = 0;
        float buyer_volume = 0.f;
        float seller_volume = 0.f;

        friend std::ostream &operator<<(std::ostream &out, const KLine &d)
        {
            out << "Event Time: " << d.timestamp << std::endl;
            out << "Start Time: " << d.start_time << std::endl;
            out << "End Time: " << d.end_time << std::endl;
            out << "High Price: " << d.high << std::endl;
            out << "Low Price: " << d.low << std::endl;
            out << "Open Price: " << d.open << std::endl;
            out << "Close Price: " << d.close << std::endl;
            out << "Volume: " << d.volume << std::endl;
            out << "Num of Trades: " << d.num_of_trades << std::endl;
            out << "First Trade Id: " << d.first_trade_id << std::endl;
            out << "Last Trade Id: " << d.last_trade_id << std::endl;
            out << "Buy Volume: " << d.buyer_volume << std::endl;
            out << "Sell Volume: " << d.seller_volume << std::endl;

            return out;
        }
    };

    struct Ticker
    {
        int64_t update_id;
        Price bid_price;
        float bid_quantity = 0.f;
        Price ask_price;
        float ask_quantity = 0.f;
        Ticker() = default;
        Ticker(const int64_t update_id_, const Price bid_price_, const float bid_quantity_, const Price ask_price_, const float ask_quantity_)
            : update_id(update_id_), bid_price(bid_price_), bid_quantity(bid_quantity_), ask_price(ask_price_), ask_quantity(ask_quantity_) {}
    };

    struct TradeMessage
    {
        int64_t time = 0;
        int64_t event_time = 0;
        int64_t id = 0;
        Price price;
        float quantity = 0.f;
        int64_t buy_order_id = 0.f;
        int64_t sell_order_id = 0.f;
        bool is_buyer_maket_maker = false;
        TradeMessage() = default;
        TradeMessage(const int64_t id_, const int64_t time_, const int64_t event_time_, const Price price_, const float quantity_, const int64_t buy_order_id_, const int64_t sell_order_id_,
                     const bool is_buyer_market_maker_) : id(id_), time(time_), event_time(event_time_), price(price_), quantity(quantity_), buy_order_id(buy_order_id_), sell_order_id(sell_order_id_),
                                                          is_buyer_maket_maker(is_buyer_market_maker_) {}
    };

    struct AggregatedTrade
    {
        int64_t time = 0;
        int64_t event_time = 0;
        Price price;
        float quantity = 0.f;
        int64_t first_trade_id = 0;
        int64_t last_trade_id = 0;
        bool is_buyer_market_maker = false;
        AggregatedTrade() = default;
        AggregatedTrade(const int64_t time_, const int64_t event_time_, const Price price_, const float quantity_, const int64_t first_trade_id_,
                        const int64_t last_trade_id_, const bool is_buyer_market_maker_) : time(time_), event_time(event_time_), price(price_), quantity(quantity_),
                                                                                           first_trade_id(first_trade_id_), last_trade_id(last_trade_id_), is_buyer_market_maker(is_buyer_market_maker_) {}
    };

    struct DeltaBook
    {
        int64_t last_update_id = 0; // Last Update ID
        std::vector<Level> bid;     // Bid side updates
        std::vector<Level> ask;     // Ask side updates
    };

    struct MiniTickerStream
    {
        int64_t time = 0; // Event time
        Price open;       // Open price
        Price close;      // Close price
        Price high;       // High price
        Price low;        // Low price
        float v = 0.f;    // Total traded base asset volume
        float q = 0.f;    // Total traded quote asset volume
    };

}