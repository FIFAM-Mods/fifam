#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"
#include <fstream>

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

class WriteCountries {
public:
    WriteCountries() {
        FifamDatabase db(13, "db");

        std::wofstream out(L"CountryNames.txt");
        WriteCountryDef(out, 0, L"None");
        for (size_t i = 0; i < 207; i++)
            WriteCountryDef(out, i + 1, FifamTr(db.mCountries[i]->mName));
    }
};

REGISTER_TEST(WriteCountries)
