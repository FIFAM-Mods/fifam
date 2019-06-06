#pragma once
#include "FifamDbEnvironment.h"
#include "FifamUtils.h"

String Capital(String const &str) {
    std::wstring result;
    for (size_t i = 0; i < str.length(); i++) {
        if (i == 0)
            result += toupper(static_cast<unsigned char>(str[i]));
        else
            result += tolower(static_cast<unsigned char>(str[i]));
    }
    return result;
}

class CreateFlagImages {
public:
    CreateFlagImages() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM14, External>>().GetDatabase();

        FifamWriter writer("countries.txt", 14, FifamVersion(), true);
        for (UInt i = 0; i < FifamDatabase::NUM_COUNTRIES; i++) {
            writer.WriteLine(L"|-");
            writer.WriteLine(Utils::Format(L"| %d", i + 1));
            writer.WriteLine(Utils::Format(L"| [[File:%s.png|24px]]", Capital(FifamTr(db->mCountries[i]->mAbbr)).c_str()));
            writer.WriteLine(Utils::Format(L"| %s", FifamTr(db->mCountries[i]->mName).c_str()));
        }
    }
};
