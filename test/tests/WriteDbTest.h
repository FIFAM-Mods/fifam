#pragma once
#include "FifamDbEnvironment.h"

template<typename From, typename To, bool WriteToGameFolder = true>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<From, Default>>().GetDatabase();
        for (auto c : db->mClubs) {
            if (c->mCountry) {
                auto countryId = FifamNation::MakeFromInt(c->mCountry->mId);
                if (countryId == FifamNation::Russia ||
                    countryId == FifamNation::Ukraine ||
                    countryId == FifamNation::Belarus ||
                    countryId == FifamNation::Kazakhstan ||
                    countryId == FifamNation::Azerbaijan)
                {
                    if (c->mTransfersCountry[0] == FifamNation::None)
                        c->mTransfersCountry[0] = countryId;
                    else if (c->mTransfersCountry[1] == FifamNation::None && c->mTransfersCountry[0] != countryId)
                        c->mTransfersCountry[1] = countryId;
                }
            }
        }
        db->Write(To::id(), FifamVersion(To::year(), To::vernum()),
            Utils::Format(WriteToGameFolder ?
                L"E:\\Games\\FIFA Manager %02d\\database corr3":
                L"fm_test\\database",
                To::id()).c_str());
    }
};
