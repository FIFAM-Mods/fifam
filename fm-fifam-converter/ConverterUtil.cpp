#include "ConverterUtil.h"

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

Int OriginalAttrValue(Int attr) {
    return (Int)ceilf((Float)attr / 5.0f);
}
