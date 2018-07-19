#pragma once

class FifamTranslation {
public:
    // @since FM07
    enum TranslationId {
        German,
        English,
        French,
        Spanish,
        Italian,
        // @since FM08
        Polish
    };

    static const unsigned int NUM_TRANSLATIONS = 6;
};

template<typename T>
using TrArray = Array<T, FifamTranslation::NUM_TRANSLATIONS>;

template<typename T>
T Tr(TrArray<T> const &trAry) {
    return trAry[FifamTranslation::English];
}

inline wchar_t const *Tr_c(TrArray<String> const &trAry) {
    return trAry[FifamTranslation::English].c_str();
}
