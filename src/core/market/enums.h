#pragma once

enum class DeltaType
{
    ADD,    // New Volume Added
    REDUCE, // Volume reduced but level exists
    TRADE,  // Volume Traded
    DELETE, // Level Completely deleted
    ADD_LEVEL,
    UNKNOWN
};

enum class Side
{
    BID,
    ASK,
    UNKNOWN
};
