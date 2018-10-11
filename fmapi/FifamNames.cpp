#include "FifamNames.h"

String FifamNames::LimitPersonName(String const & name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    UInt len = name.length();
    if (len <= maxSize)
        return name;
    auto nameParts = Utils::Split(name, L' ', true, true);
    if (nameParts.empty())
        return String();
    String result = nameParts.back();
    if (maxSize == 1)
        return result.substr(0, 1);
    if (result.size() <= maxSize) {
        if (nameParts.size() > 1) {
            for (Int i = (Int)nameParts.size() - 2; i >= 0; i--) {
                if (result.size() + 1 + nameParts[i].size() > maxSize)
                    break;
                result = nameParts[i] + L' ' + result;
            }
        }
        return result;
    }
    for (Int i = maxSize - 2; i >= 0; i--) {
        if (!Utils::IsVowel(result[i]))
            return result.substr(0, i + 1) + L'.';
    }
    return result.substr(0, maxSize);
}

String FifamNames::LimitName(String const &name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    UInt len = name.length();
    if (len <= maxSize)
        return name;
    if (maxSize == 1)
        return name.substr(0, 1);
    if (name[maxSize - 1] != L' ' && !Utils::IsPunctuationMark(name[maxSize - 1])) {
        for (Int i = maxSize - 2; i >= 0; i--) {
            if (!Utils::IsVowel(name[i]) && name[i] != ' ' && !Utils::IsPunctuationMark(name[i])) {
                String newName = name.substr(0, i + 1);
                if (i != (maxSize - 2)) {
                    if (name[i + 1] != ' ')
                        newName += L'.';
                }
                else
                    newName += L'.';
                return newName;
            }
        }
    }
    auto result = name.substr(0, maxSize);
    Utils::Trim(result);
    return result;
}

String FifamNames::GetClubAbbr(String const &name) {
    String abbr = Utils::ToUpper(name);
    if (abbr.length() <= 4)
        return abbr;
    abbr = abbr.substr(0, 4);
    Utils::Trim(abbr);
    return abbr;
}

FifamTrArray<String> FifamNames::TransformTrArray(FifamTrArray<String> const &ary, Function<String(String const &, UInt)> callback, UInt maxSize) {
    FifamTrArray<String> result;
    for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++)
        result[i] = callback(ary[i], maxSize);
    return result;
}

String FifamNames::FindCommonPrefix(Vector<String> const & strings, bool trim) {
    if (strings.empty())
        return String();
    String commonPrefix;
    UInt index = 0;
    bool stop = false;
    while (1) {
        if (strings[0].length() <= index)
            break;
        for (UInt i = 0; i < strings.size(); i++) {
            if (strings[i].length() <= index || strings[i][index] != strings[0][index]) {
                stop = true;
                break;
            }
        }
        if (stop)
            break;
        commonPrefix += strings[0][index];
        index++;
    }
    if (trim)
        Utils::Trim(commonPrefix);
    return commonPrefix;
}
