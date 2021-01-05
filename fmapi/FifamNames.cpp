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
    String nameUpper = Utils::ToUpper(Utils::GetStringWithoutUnicodeChars(name));
    String abbr;
    UInt abbrSize = 0;
    for (auto &c : nameUpper) {
        if (c != L' ' && !Utils::IsPunctuationMark(c)) {
            abbr += c;
            abbrSize++;
            if (abbrSize == 3)
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
                if (words.back() == L"Group" || words.back() == L"Gr")
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

String FifamNames::GetPersonStringId(UInt gameId, String const &firstName, String const &lastName, String const &pseudonym, FifamDate const &birthday, UInt empicsId) {
    if (empicsId != 0 && gameId <= 9)
        return Utils::Format(L"emp-%d", empicsId);
    String nameId;
    if (!pseudonym.empty())
        nameId = FifamNames::LimitPersonName(pseudonym, gameId > 7 ? 29 : 19);
    else
        nameId = FifamNames::LimitPersonName(lastName, 19) + firstName.substr(0, 2);
    nameId = Utils::GetStringWithoutUnicodeChars(nameId);
    String finalName;
    for (WideChar c : nameId) {
        if (c != L' ' && c != L'.' && c != '_' && c != 0xA0) {
            switch (c) {
            case 0xD8:
                c = L'O';
                break;
            case 0xDE:
            case 0xFE:
                c = L'P';
                break;
            case 0xDF:
                c = L's';
                break;
            case 0xE6:
                c = L'a';
                break;
            case 0xF0:
            case 0xF8:
                c = L'o';
                break;
            case 0x110:
                c = L'D';
                break;
            case 0x111:
                c = L'd';
                break;
            case 0x131:
                c = L'i';
                break;
            case 0x141:
                c = L'L';
                break;
            case 0x142:
                c = L'l';
                break;
            case 0x153:
                c = L'c';
                break;
            case 0x2019:
                c = L'\'';
                break;
            case 0x21A:
                c = L'T';
                break;
            case 0x21B:
                c = L't';
                break;
            }
            finalName += c;
        }
    }
    if (!birthday.IsEmpty())
        finalName += Utils::Format(L"%02d%02d%04d", birthday.day, birthday.month, birthday.year);
    if (empicsId != 0)
        finalName += (gameId >= 13 ? L"-" : L"_") + Utils::Format(L"%d", empicsId);
    return finalName;
}
