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
    String nameUpper = Utils::ToUpper(name);
    String abbr;
    UInt abbrSize = 0;
    for (auto &c : nameUpper) {
        if (c != L' ' && !Utils::IsPunctuationMark(c)) {
            abbr += c;
            abbrSize++;
            if (abbrSize == 4)
                break;
        }
    }
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

String FifamNames::GetLeagueLevelName(Vector<String> const & leagueNames, UInt level) {
    String levelName = FindCommonPrefix(leagueNames, true);
    if (!levelName.empty()) {
        UInt l = levelName.find_last_not_of(L" .-");
        if (l != String::npos)
            levelName = levelName.substr(0, l + 1);
        if (!levelName.empty()) {
            auto words = Utils::Split(levelName, L' ', true, true);
            if (!words.empty()) {
                if (words.back() == L"Group")
                    words.pop_back();
                else
                    return levelName;
                levelName = Utils::Join(words, L' ');
                l = levelName.find_last_not_of(L" .-");
                if (l != String::npos)
                    levelName = levelName.substr(0, l + 1);
                if (!levelName.empty()) {
                    UInt l = levelName.find_last_not_of(L" .-");
                    if (l != String::npos)
                        levelName = levelName.substr(0, l + 1);
                    if (!levelName.empty())
                        return levelName;
                }
            }
        }
    }
    return Utils::Format(L"League Level %d", level);
}
