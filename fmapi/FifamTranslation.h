#pragma once

class FifamTranslation {
public:
    enum TranslationId {
        German,
        English,
        French,
        Spanish,
        Italian,
        Polish
    };

    static const unsigned int NUM_TRANSLATIONS = 6;
};
