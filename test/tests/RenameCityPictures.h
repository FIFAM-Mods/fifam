#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "Random.h"

class RenameCityPictures {
public:
    RenameCityPictures() {
        Path cityPicturesPath = L"D:\\cities\\615x461"; // TODO
        Path outputPath = L"E:\\Games\\FIFA Manager 13\\cities\\615x461"; // TODO
        if (!exists(outputPath))
            create_directories(outputPath);
        FifamTranslation::TranslationId translation = FifamTranslation::English;
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase* db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        for (FifamClub *c : db->mClubs) {
            if (!c->mCityName[translation].empty()) {
                Path cityPicFilename = cityPicturesPath / (c->mCityName[translation] + L".jpg");
                if (exists(cityPicFilename))
                    copy(cityPicFilename, outputPath / Utils::Format(L"%08X.jpg", c->mUniqueID), copy_options::overwrite_existing);
            }
        }
        delete db;
    }
};
