#pragma once
#include "FifaFifamIDsEnvironment.h"
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
        auto const &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        FifamWriter writer("D:\\Games\\FIFA Manager 25\\plugins\\ucp\\kits.csv", 14, FifamVersion());
        writer.WriteLine(L"country,league,team,teamid,kittype,collar,nameplacement,frontnumber,jerseynumbercolor,jerseynamecolor,jerseynumbersize,jerseynumberoffset,canusecompbadges,canusesponsorlogo");
        
        struct team_kit_desc {
            unsigned char collar : 4;
            unsigned char nameplacement : 2;
            unsigned char frontnumber : 1;
            unsigned char used : 1;

            unsigned char jerseynumbercolor : 4; // 15 - unset
            unsigned char jerseynumbersize : 4; // 0 - default (max), 1 - min, 10 - max

            unsigned char jerseynamecolor : 7; // 127 - unset
            unsigned char canusecompbadges : 1;

            unsigned char jerseynumberoffset : 4; // 0 - min, 15 - max
            unsigned char canusesponsorlogo : 1;
        };

        auto GenerateForClub = [&](UInt fifamID, UInt fifaID, FifaTeam *fifaTeam, FifamWriter &writer) {
            String teamid = Utils::Format(L"0x%06X", fifamID);
            for (FifaKit *fifaKit : fifaTeam->m_kits) {
                String filename = Utils::Format(L"%08X", fifamID);
                if (fifaKit->internal.teamkittypetechid == 0)
                    filename += L"_h";
                else if (fifaKit->internal.teamkittypetechid == 1)
                    filename += L"_a";
                else if (fifaKit->internal.teamkittypetechid == 2)
                    filename += L"_g";
                else if (fifaKit->internal.teamkittypetechid == 3)
                    filename += L"_t";
                if (exists(L"D:\\Projects\\fifam\\content\\fm13\\art_04\\data\\kits\\" + filename + L".tga")) {
                    // teamid - hexadecimal team id
                    // kittype - 0 - home, 1 - away, 2 - gk, 3 - third
                    // collar - 0 - 8                                   4b
                    UChar collar = fifaKit->internal.jerseycollargeometrytype;
                    if (collar > 8) {
                        switch (fifaKit->internal.jerseycollargeometrytype) {
                        case 9:
                        case 25:
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
                        case 20:
                        case 21:
                        case 22:
                        case 24:
                            collar = 7;
                            break;
                        case 23:
                            collar = 0;
                            break;
                        case 26:
                            collar = 3;
                            break;
                        default:
                            collar = 0;
                            break;
                        }
                    }
                    UChar nameplacement = fifaKit->internal.jerseybacknameplacementcode;
                    if (nameplacement > 1)
                        nameplacement = 1;
                    if (nameplacement != 0) {
                        FifamReader posReader(Utils::AtoW("D:\\Projects\\FIFA\\kit\\positions\\kit_" + std::to_string(fifaID) + "_" + std::to_string(fifaKit->internal.teamkittypetechid) + "_0.txt"), 14, false, false);
                        if (posReader.Available()) {
                            posReader.SkipLines(13);
                            if (posReader.ReadLine<Float>() < 0.3f)
                                nameplacement = 2;
                        }
                    }
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
                    UChar canusecompbadges = 1;
                    UChar canusesponsorlogo = 0;
                    UChar jerseynumbersize = 0;
                    UChar jerseynumberoffset = 0;
                    String teamName = fifaTeam->m_name;
                    String countryName = FifamNation::MakeFromInt((fifamID >> 16) & 0xFF).ToStr();
                    String leagueName;
                    if (fifaTeam->m_league)
                        leagueName = fifaTeam->m_league->m_name;
                    if (fifaKit->internal.teamkittypetechid >= 0 && fifaKit->internal.teamkittypetechid <= 3)
                        writer.WriteLine(
                            Quoted(countryName), Quoted(leagueName), Quoted(teamName),
                            teamid, fifaKit->internal.teamkittypetechid, collar, nameplacement, frontnumber, jerseynumbercolor,
                            jerseynamecolor, jerseynumbersize, jerseynumberoffset, canusecompbadges, canusesponsorlogo);
                }
            }
        };

        for (auto const &c : ids.mFifaClubs) {
            auto fifaClub = fifadb->GetTeam(c.first);
            if (fifaClub && fifaClub->m_gameId == FifaDatabase::m_lastSupportedGameVersion)
                GenerateForClub(c.second, c.first, fifaClub, writer);
        }
    }
};
