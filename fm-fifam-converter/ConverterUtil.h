#pragma once
#include "FifamDate.h"

template<typename T>
T BestFrom_Avg(Vector<T> values, UInt num, Bool round = false) {
    if (values.empty())
        return 0;
    if (values.size() == 1)
        return values[0];
    std::sort(values.begin(), values.end(), std::greater<T>());
    if (num == 1)
        return values[0];
    T result = values[0];
    for (UInt i = 1; i < Utils::Min(num, values.size()); i++)
        result += values[i];
    if (!round)
        return result / num;
    return (T)((Int)roundf((Float)result / num));
}

void AlignToSeasonStart(FifamDate &date, bool allowHalfSeason, bool forwardDirection);

FifamDate GetDateAlignedToSeasonStart(FifamDate const &date, bool allowHalfSeason, bool forwardDirection);

void AlignToSeasonEnd(FifamDate &date, bool allowHalfSeason, bool forwardDirection);

FifamDate GetDateAlignedToSeasonEnd(FifamDate const &date, bool allowHalfSeason, bool forwardDirection);

template<typename T> T AlignMoneyValue(T input, T alignment) {
    if (input > alignment * 3)
        return ((input + (alignment / 2)) / alignment) * alignment;
    return input;
}

Int OriginalAttrValue(Int attr);
