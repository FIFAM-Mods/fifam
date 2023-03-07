#pragma once
#include "FifamDbEnvironment.h"
#include "FifaFifamIDsEnvironment.h"

class FixCityLocations {
public:
    void ReaderCallback(String const &filename, Function<void(FifamReader &)> callback) {
        std::wcout << L"Reading " << filename << L"..." << std::endl;
        FifamReader reader(L"D:\\Projects\\fifam\\db\\foom\\" + filename + L".csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine())
                    callback(reader);
                else
                    reader.SkipLine();
            }
        }
    }

    FixCityLocations() {
        auto &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        String d;
        Map<UInt, UInt> clubCity;
        Map<UInt, Pair<Float, Float>> cityCoords;
        ReaderCallback(L"fm_clubs", [&](FifamReader &reader) {
            Int clubId = -1, cityId = -1;
            reader.ReadLine(clubId, d, d, d, d, d, d, d, d, d, d, cityId);
            if (clubId != -1 && cityId != -1)
                clubCity[clubId] = cityId;
        });
        ReaderCallback(L"fm_cities", [&](FifamReader &reader) {
            Int cityId = -1;
            Float latitude = 0.0f, longitude = 0.0f;
            reader.ReadLine(cityId, d, d, d, latitude, longitude);
            if (cityId != -1 && (latitude != 0.0f || longitude != 0.0f))
                cityCoords[cityId] = { latitude, longitude };
        });
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        for (auto [clubId, cityId] : clubCity) {
            if (ids.mFoomClubs.contains(clubId) && cityCoords.contains(cityId)) {
                auto club = db.GetClubFromUID(ids.mFoomClubs[clubId]);
                if (club)
                    club->mGeoCoords.SetFromFloat(cityCoords[cityId].first, cityCoords[cityId].second);
            }
        }
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
