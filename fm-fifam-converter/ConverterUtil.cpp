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

Int TranslationLanguageFmToTranslationArrayID(Int language) {
    switch (language) {
    case FifamTranslation::French:
        return 0;
    case FifamTranslation::German:
        return 1;
    case FifamTranslation::Italian:
        return 2;
    case FifamTranslation::Spanish:
        return 3;
    case FifamTranslation::Polish:
        return 4;
    }
    return -1;
}

Int TranslationArrayIDToTranslationLanguageFm(Int index) {
    switch (index) {
    case 0:
        return FifamTranslation::French;
    case 1:
        return FifamTranslation::German;
    case 2:
        return FifamTranslation::Italian;
    case 3:
        return FifamTranslation::Spanish;
    case 4:
        return FifamTranslation::Polish;
    }
    return -1;
}

void ApplyTranslation(FifamTrArray<String> &dst, Array<String, 6> const &translations, UInt nameLimit, Array<String, 6> const &shortNames) {
    for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
        Int index = TranslationLanguageFmToTranslationArrayID(i);
        if (index != -1 && !translations[index].empty()) {
            String &d = dst[i];
            String const &t = translations[index];
            String const &s = shortNames[i];
            if (nameLimit == 0 || t.length() <= nameLimit)
                d = t;
            else {
                if (!s.empty()) {
                    if (s.length() <= nameLimit)
                        d = s;
                    else
                        d = FifamNames::LimitName(s, nameLimit);
                }
                else
                    d = FifamNames::LimitName(t, nameLimit);
            }
        }
    }
}

void SetNameAndTranslation(FifamTrArray<String> &dst, String const &name, Array<String, 6> const &translations, UInt nameLimit, String const &shortName, Array<String, 6> const &shortNames) {
    if (nameLimit == 0 || name.length() <= nameLimit)
        FifamTrSetAll(dst, name);
    else {
        if (!shortName.empty()) {
            if (shortName.length() <= nameLimit)
                FifamTrSetAll(dst, shortName);
            else
                FifamTrSetAll(dst, FifamNames::LimitName(shortName, nameLimit));
        }
        else
            FifamTrSetAll(dst, FifamNames::LimitName(name, nameLimit));
    }
    ApplyTranslation(dst, translations, nameLimit, shortNames);
}

Array<String, 6> TranslationArrayFromTrArray(FifamTrArray<String> const &dst) {
    Array<String, 6> result;
    for (UInt i = 0; i < result.size(); i++) {
        Int language = TranslationArrayIDToTranslationLanguageFm(i);
        if (language != -1 && language < Int(dst.size()))
            result[i] = dst[language];
    }
    return result;
}
