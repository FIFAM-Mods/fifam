#pragma once

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
        Spanish,
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
