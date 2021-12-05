#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include "Level.h"

template <typename PriceLevel = Level>
class BucketSet
{
    // wrapper class which forward calls to std::map, just to conform with
    using bucket_set_t = std::map<Price, std::shared_ptr<PriceLevel>>;

public:
    std::shared_ptr<PriceLevel> find(Price price)
    {
        if (auto it(m_map.find(price)); it != m_map.end()) //C++17
            return it->second;
        else
            return nullptr;
    }

    std::shared_ptr<PriceLevel> next(Price price)
    {
        if (auto it(m_map.upper_bound(price)); it != m_map.end()) //C++17
            return it->second;
        else
            return nullptr;
    }

    std::shared_ptr<PriceLevel> prev(Price price)
    {
        if (auto it(m_map.find(price)); it != m_map.begin()) //C++17
            return (--m_map.find(price))->second;
        else
            return nullptr;
    }

    std::shared_ptr<PriceLevel> insert(typename bucket_set_t::value_type keyValPair)
    {
        // use the convenience of structured bindings offered by C++17
        auto [item, ok] = m_map.insert(keyValPair);
        return ok ? item->second : std::shared_ptr<PriceLevel>();
    }

    void remove(Price price) { m_map.erase(price); }
    void clear() { m_map.clear(); }

    Price minPrice() { return m_map.begin() == m_map.end() ? Price() : m_map.begin()->first; }
    Price maxPrice() { return m_map.rbegin() == m_map.rend() ? Price() : m_map.rbegin()->first; }

    size_t size() const { return m_map.size(); }

private:
    bucket_set_t m_map;
};
