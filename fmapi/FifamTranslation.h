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
