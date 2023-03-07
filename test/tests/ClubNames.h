#pragma once
#include "FifamDbEnvironment.h"

class ClubNames {
public:
    ClubNames() {
        if (exists("club_names.csv")) {
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "db");
            FifamReader reader(L"club_names.csv", 13, FifamDatabase::GetGameDbVersion(13));
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (reader.EmptyLine())
                    reader.SkipLine();
                else {
                    String d;
                    UInt clubId = 0;
                    FifamTrArray<String> name, shortName, abbr, town, stadium;
                    reader.ReadLine(d, Hexadecimal(clubId),
                        name[0], name[1], name[2], name[3], name[4], name[5],
                        shortName[0], shortName[1], shortName[2], shortName[3], shortName[4], shortName[5],
                        abbr[0], abbr[1], abbr[2], abbr[3], abbr[4], abbr[5],
                        town[0], town[1], town[2], town[3], town[4], town[5],
                        stadium[0], stadium[1], stadium[2], stadium[3], stadium[4], stadium[5]
                    );
                    if (clubId != 0) {
                        FifamClub *c = nullptr;

                        if ((clubId & 0xFFFF) == 0xFFFF) {
                            UChar countryId = (clubId >> 16) & 0xFF;
                            if (countryId >= 1 && countryId <= 207)
                                c = &db.mCountries[countryId - 1]->mNationalTeam;
                        }
                        else
                            c = db.GetClubFromUID(clubId);
                        if (c) {
                            c->mName = name;
                            c->mName2 = c->mName;
                            c->mShortName = shortName;
                            c->mShortName2 = c->mShortName;
                            c->mAbbreviation = abbr;
                            c->mCityName = town;
                            c->mStadiumName = stadium;
                        }
                        else
                            ::Error("Can't find club %08X", clubId);
                    }
                }
            }
            db.mWritingOptions.mWriteFixtures = false;
            db.mWritingOptions.mWriteExternalScripts = false;
            db.Write(13, FifamDatabase::GetGameDbVersion(13), "db");
        }
        else {
            FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
            FifamDatabase::mReadingOptions.mReadPersons = false;
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "db");
            FifamWriter writer(L"club_names.csv", 13, FifamDatabase::GetGameDbVersion(13));
            writer.WriteLine(L"Country,ClubUID,Name_DE,Name_EN,Name_FR,Name_ES,Name_IT,Name_PL,Short_DE,Short_EN,Short_FR,Short_ES,Short_IT,Short_PL,Abbr_DE,Abbr_EN,Abbr_FR,Abbr_ES,Abbr_IT,Abbr_PL,Town_DE,Town_EN,Town_FR,Town_ES,Town_IT,Town_PL,Stad_DE,Stad_EN,Stad_FR,Stad_ES,Stad_IT,Stad_PL");
            Map<UInt, FifamClub *> clubs;
            for (FifamClub *c : db.mClubs)
                clubs[c->mUniqueID] = c;
            for (FifamCountry *c : db.mCountries)
                clubs[c->mNationalTeam.mUniqueID] = &c->mNationalTeam;
            for (auto const &[id, c] : clubs) {
                writer.WriteOne(FifamTr(c->mCountry->mName));
                writer.WriteOne(L',');
                writer.WriteOne(Utils::Format(L"0x%06X", id));
                writer.WriteOne(L',');
                writer.WriteTranslationArray(c->mName);
                writer.WriteOne(L',');
                writer.WriteTranslationArray(c->mShortName);
                writer.WriteOne(L',');
                writer.WriteTranslationArray(c->mAbbreviation);
                writer.WriteOne(L',');
                writer.WriteTranslationArray(c->mCityName);
                writer.WriteOne(L',');
                writer.WriteTranslationArray(c->mStadiumName);
                writer.WriteNewLine();
            }
        }
    }
};
