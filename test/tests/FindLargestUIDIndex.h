#pragma once
#include "FifamDbEnvironment.h"
#include "FifaFifamIDsEnvironment.h"

class FindLargestUIDIndex {
public:
    FindLargestUIDIndex() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
        FifamDatabase db(14, "D:\\Games\\FIFA Manager 14\\database");
        UInt largestIndex = 0;
        FifamClub *largestIndexClub = nullptr;
        for (FifamClub *club : db.mClubs) {
            if ((club->mUniqueID & 0xFFFF) > largestIndex) {
                largestIndex = club->mUniqueID & 0xFFFF;
                largestIndexClub = club;
            }
        }
        std::wstring clubName;
        if (largestIndexClub) {
            clubName = FifamTr(largestIndexClub->mName) + L" - " + Utils::Format(L"%08X", largestIndexClub->mUniqueID);
        }
        std::wcout << L"Largest UID index: " << Utils::Format(L"%04X", largestIndex) << " (" << clubName << ")";
    }
};
