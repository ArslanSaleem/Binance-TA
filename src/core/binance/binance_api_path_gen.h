#pragma once
#include <sstream>

class BinanceApiPathGen
{
    static std::string get_kline_path(const std::string symbol, const uint32_t mins)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@kline_" << mins << "m";
        return str_stream.str();
    }

    static std::string get_depth_path(const std::string symbol, const uint32_t time)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@depth@" << time << "ms";
        return str_stream.str();
    }

    static std::string get_partial_book_path(const std::string symbol, const uint32_t levels, const uint32_t time)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@depth" << levels << "@" << time << "ms";
        return str_stream.str();
    }
    static std::string get_book_ticker_path(const std::string symbol)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@bookTicker";
        return str_stream.str();
    }

    static std::string get_trade_data_path(const std::string symbol)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@trade";
        return str_stream.str();
    }

    static std::string get_agg_trade_data_path(const std::string symbol)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@aggTrade";
        return str_stream.str();
    }

    static std::string get_mini_ticker_path(const std::string symbol)
    {
        std::stringstream str_stream;
        str_stream << "/ws/" << symbol << "@miniTicker";
        return str_stream.str();
    }
};
