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

void AlignToSeasonStart(FifamDate &date, bool allowHalfSeason, bool forwardDirection) {
    if ((date.month == 7 && date.day == 1) || (allowHalfSeason && date.month == 1 && date.day == 1))
        return;
    if (date.month >= 7) {
        if (forwardDirection) {
            if (allowHalfSeason)
                date.Set(1, 1, date.year + 1);
            else
                date.Set(1, 7, date.year + 1);
        }
        else
            date.Set(1, 7, date.year);
    }
    else {
        if (forwardDirection)
            date.Set(1, 7, date.year);
        else {
            if (allowHalfSeason)
                date.Set(1, 1, date.year);
            else
                date.Set(1, 7, date.year - 1);
        }
    }
}

FifamDate GetDateAlignedToSeasonStart(FifamDate const &date, bool allowHalfSeason, bool forwardDirection) {
    FifamDate result = date;
    AlignToSeasonStart(result, allowHalfSeason, forwardDirection);
    return result;
}

void AlignToSeasonEnd(FifamDate &date, bool allowHalfSeason, bool forwardDirection) {
    if ((date.month == 6 && date.day == 30) || (allowHalfSeason && date.month == 12 && date.day == 31))
        return;
    if (date.month >= 7) {
        if (forwardDirection) {
            if (allowHalfSeason)
                date.Set(31, 12, date.year);
            else
                date.Set(30, 6, date.year + 1);
        }
        else
            date.Set(30, 6, date.year);
    }
    else {
        if (forwardDirection)
            date.Set(30, 6, date.year);
        else {
            if (allowHalfSeason)
                date.Set(31, 12, date.year - 1);
            else
                date.Set(30, 6, date.year - 1);
        }
    }
}

FifamDate GetDateAlignedToSeasonEnd(FifamDate const &date, bool allowHalfSeason, bool forwardDirection) {
    FifamDate result = date;
    AlignToSeasonEnd(result, allowHalfSeason, forwardDirection);
    return result;
}

template<typename T> T AlignMoneyValue(T input, T alignment) {
    if (input > alignment * 3)
        return ((input + (alignment / 2)) / alignment) * alignment;
    return input;
}
