#pragma once
#include "FifamDbEnvironment.h"

void WriteCountryDef(std::wofstream &out, int id, std::wstring const &name) {
    auto defName = name;
    Utils::Replace(defName, L" ", L"_");
    Utils::Replace(defName, L"-", L"_");
    Utils::Replace(defName, L".", L"_");
    Utils::Replace(defName, L",", L"_");
    Utils::Replace(defName, L"&", L"_");
    Utils::Replace(defName, L"'", L"_");
    Utils::Replace(defName, L"___", L"_");
    Utils::Replace(defName, L"__", L"_");
    if (Utils::EndsWith(defName, L"_"))
        defName.pop_back();
    out << Utils::Format(L"    ENUM_MEMBER(%3d, %s,", id, defName.c_str()) << std::wstring(22 - defName.length(), L' ') <<
        Utils::Format(L"L\"%s\")", name.c_str()) << std::endl;
}

template<typename Game, typename DbType>
class WriteCountries {
public:
    WriteCountries() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        std::wofstream out(Utils::Format(L"CountryNames%d.txt", Game::id()));
        WriteCountryDef(out, 0, L"None");
        for (size_t i = 0; i < 207; i++)
            WriteCountryDef(out, i + 1, FifamTr(db->mCountries[i]->mName));
    }
};
