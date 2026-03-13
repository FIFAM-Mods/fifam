#pragma once
#include "FifamTypes.h"

/*
# | English   | French  | German     | Italian  | Spanish   | Polish
--|-----------|---------|------------|----------|-----------|--------------
0 | in %1     | en %1   | in %1      | in %1    | en %1     | w kraju %1
1 | in the %1 | à %1    | auf %1     | a %1     | en los %1 | w drużynie %1
2 | on %1     | au %1   | in der %1  | nella %1 | en %1     | w drużynie %1
3 | on the %1 | aux %1  | auf den %1 | nelle %1 | en las %1 | w drużynie %1
4 |           | à la %1 | im %1      | ad %1    |           | im %1
5 |           | à l'%1  | in den %1  | negli %1 |           | in den %1
6 |           |         | in %1      | nel %1   |           | in %1
*/

class FifamTranslation {
public:
    // @since FM07
    enum TranslationId {
        German,
        English,
        French,
        // @since FM04
        Spanish,
        // @since FM05
        Italian,
        // @since FM08
        Polish
    };

    static const UInt NUM_TRANSLATIONS = 6;
};

template<typename T>
using FifamTrArray = Array<T, FifamTranslation::NUM_TRANSLATIONS>;

template<typename T>
T FifamTr(FifamTrArray<T> const &trAry) {
    return trAry[FifamTranslation::English];
}

inline WideChar const *FifamTr_c(FifamTrArray<String> const &trAry) {
    return trAry[FifamTranslation::English].c_str();
}

template<typename T>
void FifamTrSetAll(FifamTrArray<T> &trAry, T const &value) {
    for (T &tr : trAry)
        tr = value;
}

namespace CustomLanguages {

enum TranslationLanguageID {
    TRANSLATIONLANGUAGE_ENG = 0,
    TRANSLATIONLANGUAGE_FRE = 1,
    TRANSLATIONLANGUAGE_GER = 2,
    TRANSLATIONLANGUAGE_ITA = 3,
    TRANSLATIONLANGUAGE_SPA = 4,
    TRANSLATIONLANGUAGE_POL = 5,
    TRANSLATIONLANGUAGE_RUS = 6,
    TRANSLATIONLANGUAGE_UKR = 7,
    TRANSLATIONLANGUAGE_CZE = 8,
    TRANSLATIONLANGUAGE_HUN = 9,
    TRANSLATIONLANGUAGE_POR = 10,
    TRANSLATIONLANGUAGE_TUR = 11,
    TRANSLATIONLANGUAGE_KOR = 12,
    TRANSLATIONLANGUAGE_CHI = 13
};

enum EditorTranslationID {
    EDITORTRANSLATION_GER = 0,
    EDITORTRANSLATION_ENG = 1,
    EDITORTRANSLATION_FRE = 2,
    EDITORTRANSLATION_ITA = 3,
    EDITORTRANSLATION_SPA = 4,
    EDITORTRANSLATION_POL = 5
};

const Int NUM_DEFAULT_TRANSLATION_LANGUAGES = 6;
const Int NUM_CUSTOM_TRANSLATION_LANGUAGES = 8;
const Int NUM_TRANSLATION_LANGUAGES = NUM_DEFAULT_TRANSLATION_LANGUAGES + NUM_CUSTOM_TRANSLATION_LANGUAGES;
extern const Char *TranslationLanguages[NUM_TRANSLATION_LANGUAGES];
extern const WideChar *TranslationLanguagesW[NUM_TRANSLATION_LANGUAGES];
extern TranslationLanguageID EditorLanguageToTranslationLanguage[NUM_DEFAULT_TRANSLATION_LANGUAGES];
extern EditorTranslationID TranslationLanguageToEditorLanguage[NUM_TRANSLATION_LANGUAGES];

Bool IsDefaultTranslationLanguage(Int langID);
Bool IsCustomTranslationLanguage(Int langID);
Int TranslationLanguageIdToCustomLanguageId(Int langID);
Char const *GetTranslationLanguageCode(Int langID);
WideChar const *GetTranslationLanguageCodeW(Int langID);
Int GetTranslationLanguageID(StringA const &name);
Int GetTranslationLanguageID(String const &name);

}
