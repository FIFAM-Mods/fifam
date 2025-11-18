#pragma once
#include "FifamDate.h"
#include "FifamTranslation.h"
#include "FifamNames.h"

enum eFMFormation {
    FMF_FORMATION_4_3_2_1_DM_AM_NARROW = 1, // renamed from FMF_FORMATION_4_1_2_3_DM_AM_NARROW in 2026
    FMF_FORMATION_4_1_2_3_DM_NARROW,
    FMF_FORMATION_4_1_2_3_NARROW,
    FMF_FORMATION_4_1_3_1_1_DM_AM,
    FMF_FORMATION_4_1_3_2_DM,
    FMF_FORMATION_4_1_3_2_DM_CF,
    FMF_FORMATION_4_1_3_2_NARROW,
    FMF_FORMATION_4_1_3_2_CF_NARROW,
    FMF_FORMATION_4_1_4_1_DM,
    FMF_FORMATION_4_3_3_DM_WIDE, // renamed from FMF_FORMATION_4_1_4_1_DM_WIDE in 2026
    FMF_FORMATION_4_1_4_1_DM_ASYMMETRIC_AM_L,
    FMF_FORMATION_4_1_4_1_DM_ASYMMETRIC_AM_R,
    FMF_FORMATION_4_2_1_3_DM_WIDE,
    FMF_FORMATION_4_2_2_2_DM,
    FMF_FORMATION_4_2_2_2_DM_CF,
    FMF_FORMATION_4_2_2_2_DM_NARROW,
    FMF_FORMATION_4_2_2_2_DM_CF_NARROW,
    FMF_FORMATION_4_2_2_2_DM_AM_NARROW,
    FMF_FORMATION_4_2_3_1_DM,
    FMF_FORMATION_4_2_3_1_DM_AM_WIDE, // renamed from FMF_FORMATION_4_2_3_1_DM_WIDE in 2026
    FMF_FORMATION_4_2_3_1_DM_CF_WIDE, // added in 2020 public
    FMF_FORMATION_4_2_3_1_DM_AM_NARROW,
    FMF_FORMATION_4_2_3_1_NARROW,
    FMF_FORMATION_4_2_3_1_WIDE,
    FMF_FORMATION_4_2_3_1_DM_CF,
    FMF_FORMATION_4_2_3_1_0_DM,
    FMF_FORMATION_4_2_4_DM_WIDE,
    FMF_FORMATION_4_2_4_DM_CF_WIDE,
    FMF_FORMATION_4_2_4_WIDE,
    FMF_FORMATION_4_2_4_CF_WIDE,
    FMF_FORMATION_4_2_4_0_DM_WIDE,
    FMF_FORMATION_4_3_1_2_NARROW,
    FMF_FORMATION_4_3_2_1_NARROW,
    FMF_FORMATION_4_3_3_NARROW,
    FMF_FORMATION_4_3_3_WIDE, // added in 2020 public
    FMF_FORMATION_4_4_1_1,
    FMF_FORMATION_4_4_1_1_2DM,
    FMF_FORMATION_4_4_1_1_CF, // added in 2020 public
    FMF_FORMATION_4_4_2,
    FMF_FORMATION_4_4_2_CF,
    FMF_FORMATION_4_4_2_DIAMOND_NARROW,
    FMF_FORMATION_4_4_2_DIAMOND_WIDE, // added in 2020 public
    FMF_FORMATION_4_5_1,
    FMF_FORMATION_3_1_3_1_2_DM,
    FMF_FORMATION_3_1_4_2_DM,
    FMF_FORMATION_3_2_3_2_DM,
    FMF_FORMATION_3_4_1_2,
    FMF_FORMATION_3_4_2_1,
    FMF_FORMATION_3_4_2_1_DM,
    FMF_FORMATION_3_4_3,
    FMF_FORMATION_5_2_3_DM_WIDE, // renamed from FMF_FORMATION_3_4_3_DM_WIDE in 2026
    FMF_FORMATION_3_5_2,
    FMF_FORMATION_3_5_2_CF,
    FMF_FORMATION_5_3_2_DM_WB, // renamed from FMF_FORMATION_5_1_2_2_DM_WB in 2026
    FMF_FORMATION_5_1_2_2_DM_WB_CF,
    FMF_FORMATION_5_1_2_2_DM_WB_NARROW,
    FMF_FORMATION_5_1_3_1_DM_WB,
    FMF_FORMATION_5_2_1_2_WB,
    FMF_FORMATION_5_2_2_1_DM,
    FMF_FORMATION_5_2_2_1_WB,
    FMF_FORMATION_5_2_2_1_DM_CF,
    FMF_FORMATION_5_2_2_1_0_DM,
    FMF_FORMATION_5_2_3_NARROW,
    FMF_FORMATION_5_3_2_WB,
    FMF_FORMATION_5_3_2_WB_CF,
    FMF_FORMATION_5_4_1,
    FMF_FORMATION_5_4_1_DIAMOND_WB,
    FMF_FORMATION_5_4_1_WB_WIDE,
    FMF_FORMATION_5_2_2_1_DM_AM, // added in 2026
    FMF_FORMATION_5_2_1_2_DM_AM, // added in 2026
    FMF_FORMATION_3_2_5_DM_2AM_WIDE, // added in 2026
    FMF_FORMATION_5_2_1_2_DM_AM_NARROW, // added in 2026
    FMF_FORMATION_5_2_2_1_DM_AM_NARROW, // added in 2026
    FMF_FORMATION_5_3_2_DM_NARROW // added in 2026
};

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

void ApplyTranslation(FifamTrArray<String> &dst, Array<String, 6> const &translations, UInt nameLimit = 0, Array<String, 6> const &shortNames = {});

void SetNameAndTranslation(FifamTrArray<String> &dst, String const &name, Array<String, 6> const &translations, UInt nameLimit = 0, String const &shortName = String(), Array<String, 6> const &shortNames = {});

Array<String, 6> TranslationArrayFromTrArray(FifamTrArray<String> const &dst);
