#pragma once
#include "FifamDatabase.h"
#include "TextFileTable.h"
#include "FifaFifamIDsEnvironment.h"

class UpdateToFM26Update1 {
public:
    UpdateToFM26Update1() {
        TextFileTable clubsWithoutFoomID;
        clubsWithoutFoomID.AddRow(L"UniqueID", L"Country", L"Name");
        TextFileTable clubsWithoutCity;
        clubsWithoutCity.AddRow(L"UniqueID",L"Country",L"Name",L"Type");
        TextFileTable countriesWithoutFoomID;
        countriesWithoutFoomID.AddRow(L"CountryID", L"CountryName");
        TextFileTable countriesWithoutCapitalCity;
        countriesWithoutCapitalCity.AddRow(L"CountryID", L"CountryName");
        TextFileTable wrongCapitalCity;
        wrongCapitalCity.AddRow(L"CountryID", L"CountryName", L"CityID", L"CityName", L"CityCountryID", L"CityCountryName");
        TextFileTable wrongClubCity;
        wrongClubCity.AddRow(L"UniqueID", L"Name", L"CountryID", L"CountryName", L"CityID", L"CityName", L"CityCountryID",
            L"CityCountryName");
        auto &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        Map<Int, Int> clubCityLinks;
        Set<Int> basquePlayerIDs;
        Array<Int, 208> capitalCity;
        Map<Int, Int> personCityLinks;
        Set<Int> femalePersons;
        for (auto &c : capitalCity)
            c = -1;
        {
            FifamReader r("fm_player_second_nations.csv"); // FoomID, NationID
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    Int playerId = 1, nationId = -1;
                    r.ReadLine(playerId, nationId);
                    if (playerId != -1 && nationId == 114502)
                        basquePlayerIDs.insert(playerId);
                }
                else
                    r.SkipLine();
            }
        }
        {
            FifamReader r("fm_club_cities.csv"); // FoomID, CityID
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    Int foomId = -1, cityId = -1;
                    r.ReadLine(foomId, cityId);
                    if (foomId != -1 && cityId != -1)
                        clubCityLinks[foomId] = cityId;
                }
                else
                    r.SkipLine();
            }
        }
        {
            FifamReader r("fifam_capital_cities.csv"); // FIFAMID, CityID
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    Int countryId = 0, cityId = -1;
                    r.ReadLine(countryId, cityId);
                    if (countryId != 0 && cityId != -1)
                        capitalCity[countryId] = cityId;
                }
                else
                    r.SkipLine();
            }
        }
        {
            FifamReader r("fm_player_cities.csv"); // FoomID, CityID
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    Int personId = -1, cityId = -1;
                    r.ReadLine(personId, cityId);
                    if (personId != -1 && cityId != -1)
                        personCityLinks[personId] = cityId;
                }
                else
                    r.SkipLine();
            }
        }
        {
            Vector<String> filenames = { L"fm_non_player_female.csv", L"fm_official_female" };
            for (auto filename : filenames) {
                FifamReader r(filename); // FoomID
                r.SkipLine();
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        Int personId = -1;
                        r.ReadLine(personId);
                        if (personId != -1)
                            femalePersons.insert(personId);
                    }
                    else
                        r.SkipLine();
                }
            }
        }
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        for (UInt i = 1; i <= FifamDatabase::NUM_COUNTRIES; i++) {
            auto c = db.GetCountry(i);
            if (c) {
                Int foomID = ids.mFifamClubsToFoom[c->mNationalTeam.mUniqueID];
                if (foomID > 0)
                    c->mFootballManagerID = foomID;
                else
                    countriesWithoutFoomID.AddRow(c->mId, FifamTr(c->mName));
                c->mNationalTeam.mCityID = capitalCity[c->mId];
                if (c->mNationalTeam.mCityID == -1)
                    countriesWithoutCapitalCity.AddRow(c->mId, FifamTr(c->mName));
                else {
                    auto city = db.GetCity(c->mNationalTeam.mCityID);
                    if (city) {
                        if (c->mId != city->countryId) {
                            wrongCapitalCity.AddRow(c->mId, FifamTr(c->mName), c->mNationalTeam.mCityID, city->names[0], city->countryId,
                                db.GetCountry(city->countryId) ? FifamTr(db.GetCountry(city->countryId)->mName) : L"");
                        }
                    }
                    else
                        wrongCapitalCity.AddRow(c->mId, FifamTr(c->mName), c->mNationalTeam.mCityID, L"", 0, L"");
                }
            }
        }
        for (auto &c : db.mClubs) {
            c->mCityID = -1;
            String uid = Utils::Format(L"0x%08X", c->mUniqueID);
            String countryName = FifamTr(c->mCountry->mName);
            String name = FifamTr(c->mName);
            Int foomID = -1;
            if (Utils::Contains(ids.mFifamClubsToFoom, c->mUniqueID))
                foomID = ids.mFifamClubsToFoom[c->mUniqueID];
            if (foomID > 0) {
                c->mFootballManagerID = foomID;
                if (Utils::Contains(clubCityLinks, foomID) && clubCityLinks[foomID] != -1) {
                    c->mCityID = clubCityLinks[foomID];
                    if (c->mCityID != -1) {
                        auto city = db.GetCity(c->mCityID);
                        if (city) {
                            if (c->mCountry->mId != city->countryId) {
                                wrongClubCity.AddRow(uid, name, c->mCountry->mId, countryName, c->mCityID, city->names[0], city->countryId,
                                    db.GetCountry(city->countryId) ? FifamTr(db.GetCountry(city->countryId)->mName) : L"");
                            }
                        }
                        else
                            wrongClubCity.AddRow(uid, name, c->mCountry->mId, countryName, c->mCityID, L"", 0, L"");
                    }
                }
                else {
                    Int capitalCityID = capitalCity[c->mCountry->mId];
                    if (capitalCityID == -1)
                        clubsWithoutCity.AddRow(uid, countryName, name, 2);
                    else {
                        c->mCityID = capitalCityID;
                        clubsWithoutCity.AddRow(uid, countryName, name, 3);
                    }
                }
            }
            else {
                clubsWithoutFoomID.AddRow(uid, countryName, name);
                clubsWithoutCity.AddRow(uid, countryName, name, 1);
            }
        }
        for (auto &p : db.mPlayers) {
            if (p->mFootballManagerID >= 0) {
                if (Utils::Contains(personCityLinks, p->mFootballManagerID))
                    p->mBirthCityID = personCityLinks[p->mFootballManagerID];
                if (Utils::Contains(femalePersons, p->mFootballManagerID))
                    p->mIsFemale = true;
                if (Utils::Contains(basquePlayerIDs, p->mFootballManagerID))
                    p->mIsBasque = true;
            }
        }
        for (auto &s : db.mStaffs) {
            if (s->mFootballManagerID >= 0) {
                if (Utils::Contains(personCityLinks, s->mFootballManagerID))
                    s->mBirthCityID = personCityLinks[s->mFootballManagerID];
                if (Utils::Contains(femalePersons, s->mFootballManagerID))
                    s->mIsFemale = true;
            }
        }
        for (auto &r : db.mReferees) {
            if (r->mFootballManagerID >= 0) {
                if (Utils::Contains(femalePersons, r->mFootballManagerID))
                    r->mIsFemale = true;
            }
        }
        clubsWithoutFoomID.WriteUnicodeText(L"fm26u1_clubsWithoutFoomID.txt");
        clubsWithoutCity.WriteUnicodeText(L"fm26u1_clubsWithoutCity.txt");
        countriesWithoutFoomID.WriteUnicodeText(L"fm26u1_countriesWithoutFoomID.txt");
        countriesWithoutCapitalCity.WriteUnicodeText(L"fm26u1_countriesWithoutCapitalCity.txt");
        wrongCapitalCity.WriteUnicodeText(L"fm26u1_wrongCapitalCity.txt");
        wrongClubCity.WriteUnicodeText(L"fm26u1_wrongClubCity.txt");
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
