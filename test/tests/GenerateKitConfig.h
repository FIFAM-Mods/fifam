#pragma once
#include "FifamDbEnvironment.h"
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"
#include "Color.h"

Vector<Pair<UInt, ::Color>> shirtNumberColorTable = {
    { 1, { 255, 255, 255 }},
    { 2, { 0, 0, 0 }},
    { 3, { 255, 255, 0 }},
    { 4, { 255, 0, 0 }},
    { 5, { 0, 255, 0 }},
    { 6, { 0, 0, 255 }}
};

Vector<Pair<UInt, ::Color>> shirtNameColorTable = {
    { 1, {241, 242, 245 }},
    { 2, {20, 19, 23 }},
    { 3, {240, 240, 205 }},
    { 4, {240, 240, 128 }},
    { 5, {242, 239, 61 }},
    { 6, {240, 239, 64 }},
    { 7, {241, 223, 54 }},
    { 8, {234, 197, 67 }},
    { 9, {204, 168, 47 }},
    { 10, {241, 176, 46 }},
    { 11, {240, 210, 137 }},
    { 12, {243, 169, 81 }},
    { 13, {222, 127, 59 }},
    { 14, {227, 108, 47 }},
    { 15, {197, 76, 23 }},
    { 16, {221, 78, 64 }},
    { 17, {196, 57, 41 }},
    { 18, {150, 23, 44 }},
    { 19, {238, 60, 49 }},
    { 20, {220, 42, 42 }},
    { 21, {189, 28, 55 }},
    { 22, {181, 27, 55 }},
    { 23, {218, 49, 72 }},
    { 24, {176, 44, 67 }},
    { 25, {150, 18, 40 }},
    { 26, {156, 23, 78 }},
    { 27, {230, 234, 237 }},
    { 28, {254, 213, 220 }},
    { 29, {204, 204, 103 }},
    { 30, {2, 124, 2 }},
    { 31, {119, 13, 3 }},
    { 32, {116, 69, 139 }},
    { 33, {0, 0, 0 }},
    { 34, {255, 255, 0 }},
    { 35, {196, 172, 146 }},
    { 36, {196, 153, 144 }},
    { 37, {168, 133, 114 }},
    { 38, {133, 73, 75 }},
    { 39, {110, 56, 58 }},
    { 40, {78, 36, 37 }},
    { 41, {158, 234, 197 }},
    { 42, {96, 188, 132 }},
    { 43, {1, 153, 169 }},
    { 44, {58, 143, 136 }},
    { 45, {52, 120, 119 }},
    { 46, {145, 147, 60 }},
    { 47, {50, 148, 35 }},
    { 48, {70, 128, 57 }},
    { 49, {27, 77, 58 }},
    { 50, {219, 227, 250 }},
    { 51, {181, 190, 205 }},
    { 52, {160, 170, 176 }},
    { 53, {0, 71, 182 }},
    { 54, {157, 136, 78 }},
    { 55, {1, 2, 81 }},
    { 56, {240, 217, 39 }},
    { 57, {0, 123, 0 }},
    { 58, {125, 177, 211 }},
    { 59, {0, 19, 82 }},
    { 60, {60, 92, 185 }},
    { 61, {44, 65, 143 }},
    { 62, {33, 49, 96 }},
    { 63, {166, 0, 4 }},
    { 64, {15, 31, 77 }},
};

class GenerateKitConfig {
public:
    GenerateKitConfig() {
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        FifamWriter writer("D:\\Games\\FIFA Manager 13\\plugins\\ucp\\kits.csv", 14, FifamVersion());
        writer.WriteLine(L"country,league,team,teamid,kittype,collar,nameplacement,frontnumber,jerseynumbercolor,jerseynamecolor,shortsnumbercolor,shortsnumberhotspotid,namecase,canusecompbadges");
        
        struct team_kit_desc {
            unsigned char collar : 4;
            unsigned char nameplacement : 2;
            unsigned char namecase : 2;

            unsigned char jerseynumbercolor : 4; // 15 - unset
            unsigned char shortsnumbercolor : 4; // 15 - unset

            unsigned char jerseynamecolor : 7; // 127 - unset
            unsigned char frontnumber : 1;

            unsigned char shortsnumberhotspotid : 4; // 15 - unset
            unsigned char canusecompbadges : 1;
            unsigned char used : 1;
        };

        auto GenerateForClub = [&](FifamClub *club, FifamWriter &writer) {
            if (club->mFifaID) {
                FifaTeam *fifaTeam = fifadb->GetTeam(club->mFifaID);
                if (fifaTeam) {
                    String teamid = Utils::Format(L"0x%06X", club->mUniqueID);
                    for (FifaKit *fifaKit : fifaTeam->m_kits) {
                        // teamid - hexadecimal team id
                        // kittype - 0 - home, 1 - away, 2 - gk, 3 - third
                        // collar - 0 - 8                                   4b
                        UChar collar = fifaKit->internal.jerseycollargeometrytype;
                        if (collar > 8) {
                            switch (fifaKit->internal.jerseycollargeometrytype) {
                            case 9:
                                collar = 4;
                                break;
                            case 10:
                            case 19:
                                collar = 5;
                                break;
                            case 11:
                            case 17:
                            case 18:
                                collar = 8;
                                break;
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16:
                            case 21:
                                collar = 7;
                                break;
                            default:
                                collar = 0;
                                break;
                            }
                        }
                        UChar nameplacement = fifaKit->internal.jerseybacknameplacementcode;
                        if (nameplacement > 1)
                            nameplacement = 1;
                        UChar frontnumber = fifaKit->internal.jerseyfrontnumberplacementcode;
                        if (frontnumber > 1)
                            frontnumber = 1;
                        ::Color clrJNum = ::Color(fifaKit->internal.jerseynumbercolorprimr, fifaKit->internal.jerseynumbercolorprimg, fifaKit->internal.jerseynumbercolorprimb);
                        ::Color clrJName = ::Color(fifaKit->internal.jerseynamecolorr, fifaKit->internal.jerseynamecolorg, fifaKit->internal.jerseynamecolorb);
                        Char jerseynumbercolor = clrJNum.FindIndexInTable(shirtNumberColorTable);
                        if (jerseynumbercolor == 0)
                            jerseynumbercolor = -1;
                        Char jerseynamecolor = clrJName.FindIndexInTable(shirtNameColorTable);
                        if (jerseynamecolor == 0)
                            jerseynamecolor = -1;
                        Char shortsnumbercolor = -1;
                        Char shortsnumberhotspotid = -1;
                        UChar namecase = fifaKit->internal.jerseybacknamefontcase;
                        if (namecase > 2)
                            namecase = 0;
                        UChar canusecompbadges = 0;

                        String countryName;
                        if (club->mCountry)
                            countryName = FifamTr(club->mCountry->mName);
                        String leagueName;
                        if (club->mIsNationalTeam)
                            leagueName = L"International";
                        else {
                            auto league = club->GetProperty<FifamCompLeague *>(L"league", nullptr);
                            if (league)
                                leagueName = FifamTr(league->mName);
                        }

                        String teamName = FifamTr(club->mName);
                        if (fifaKit->internal.teamkittypetechid >= 0 && fifaKit->internal.teamkittypetechid <= 3)
                            writer.WriteLine(
                                Quoted(countryName), Quoted(leagueName), Quoted(teamName),
                                teamid, fifaKit->internal.teamkittypetechid, collar, nameplacement, frontnumber, jerseynumbercolor,
                                jerseynamecolor, shortsnumbercolor, shortsnumberhotspotid, namecase, canusecompbadges);
                    }
                }
            }
        };

        for (auto c : db->mCountries) {
            if (c) {
                GenerateForClub(&c->mNationalTeam, writer);

                auto countryComps = c->GetCompetitions(true);
                for (auto comp : countryComps) {
                    if (comp.second->GetDbType() == FifamCompDbType::League) {
                        FifamCompLeague *league = comp.second->AsLeague();
                        for (auto club : league->mTeams) {
                            if (club.IsValid()) {
                                if (club.IsFirstTeam()) {
                                    auto clubLeague = club.mPtr->GetProperty<FifamCompLeague *>(L"league", nullptr);
                                    if (!clubLeague)
                                        club.mPtr->SetProperty<FifamCompLeague *>(L"league", league);
                                }
                            }
                        }
                    }
                }
            }
        }
        for (auto c : db->mClubs)
            GenerateForClub(c, writer);
    }
};
