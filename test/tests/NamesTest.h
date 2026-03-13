#pragma once
#include "FifamTypes.h"
#include "FifamNames.h"
#include "Utils.h"
#include "TextFileTable.h"

namespace OldNames {

bool IsVowel(wchar_t wideChar) {
    return wideChar == L'A' || wideChar == L'a' ||
        wideChar == L'E' || wideChar == L'e' ||
        wideChar == L'I' || wideChar == L'i' ||
        wideChar == L'O' || wideChar == L'o' ||
        wideChar == L'U' || wideChar == L'u' ||
        wideChar == L'Ä' || wideChar == L'ä' ||
        wideChar == L'Ö' || wideChar == L'ö' ||
        wideChar == L'Ü' || wideChar == L'ü' ||
        wideChar == L'Y' || wideChar == L'y';
}

String LimitPersonName(String const &name, UInt maxSize) {
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
        if (!IsVowel(result[i]))
            return result.substr(0, i + 1) + L'.';
    }
    return result.substr(0, maxSize);
}

String LimitName(String const &name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    UInt len = name.length();
    if (len <= maxSize)
        return name;
    if (maxSize == 1)
        return name.substr(0, 1);
    if (name[maxSize - 1] != L' ' && !Utils::IsPunctuationMark(name[maxSize - 1])) {
        for (Int i = maxSize - 2; i >= 0; i--) {
            if (!IsVowel(name[i]) && name[i] != ' ' && !Utils::IsPunctuationMark(name[i])) {
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

String GetClubAbbr(String const &name) {
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

String FindCommonPrefix(Vector<String> const &strings, bool trim) {
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

String GetLeagueLevelName(Vector<String> const &leagueNames, UInt level) {
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

}

class NamesTest {
public:
    NamesTest() {
        using namespace std;
        // player names
        {
            UInt NameLimit[] = { 15, 19, 29 };
            TextFileTable f;
            f.ReadUnicodeText("player_names.txt");
            Bool header = true;
            for (auto &row : f.mCells) {
                if (header) {
                    row = { L"FirstName", L"SecondName", L"CommonName",
                            L"LenFirst", L"LenSecond", L"LenCommon",
                            L"OldFirst", L"OldSecond", L"OldCommon",
                            L"NewFirst", L"NewSecond", L"NewCommon" };
                    header = false;
                    continue;
                }
                for (UInt i = 0; i < 3; i++)
                    row.push_back(to_wstring(row[i].size()));
                for (UInt i = 0; i < 3; i++) {
                    if (row[i].size() > NameLimit[i])
                        row.push_back(OldNames::LimitPersonName(row[i], NameLimit[i]));
                    else
                        row.push_back(L"");
                }
                for (UInt i = 0; i < 3; i++) {
                    if (row[i].size() > NameLimit[i])
                        row.push_back(FifamNames::LimitPersonName(row[i], NameLimit[i]));
                    else
                        row.push_back(L"");
                }
            }
            f.WriteUnicodeText("player_names_edited.txt");
        }
        // club names
        {
            UInt NameLimit = 10;
            TextFileTable f;
            f.ReadUnicodeText("club_names.txt");
            Bool header = true;
            for (auto &row : f.mCells) {
                if (header) {
                    row = { L"ShortName", L"Country", L"City", L"Length", L"Old", L"New"};
                    header = false;
                    continue;
                }
                String name = row[0];
                String city = row[2];
                row.push_back(to_wstring(name.size()));
                row.push_back(OldNames::LimitName(name, NameLimit));
                row.push_back(FifamNames::LimitClubName(name, NameLimit, city));
            }
            f.WriteUnicodeText("club_names_edited.txt");
        }
    }
};
