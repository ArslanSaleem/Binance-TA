#pragma once

struct KLine
{
    long timestamp = 0;
    long start_time = 0;
    long end_time = 0;
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
