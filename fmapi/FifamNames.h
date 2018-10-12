#pragma once
#include "FifamTypes.h"
#include "Utils.h"
#include "FifamTranslation.h"

class FifamNames {
public:
    static String LimitPersonName(String const &name, UInt maxSize);
    static String LimitName(String const &name, UInt maxSize);
    static String GetClubAbbr(String const &name);
    static FifamTrArray<String> TransformTrArray(FifamTrArray<String> const &ary, Function<String(String const &, UInt)> callback, UInt maxSize);
    static String FindCommonPrefix(Vector<String> const &strings, bool trim = true);
    static String GetLeagueLevelName(Vector<String> const &leagueNames, UInt level);
};
