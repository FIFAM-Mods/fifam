#include "Fifa07Converter.h"
#include "Converter.h"
#include "Magick++.h"
#include "FifamNames.h"

using namespace Magick;

enum class FifaCountryId {
    No_Country_Set = 0,
    Albania = 1,
    Andorra = 2,
    Armenia = 3,
    Austria = 4,
    Azerbaijan = 5,
    Belarus = 6,
    Belgium = 7,
    Bosnia_and_Herzegovina = 8,
    Bulgaria = 9,
    Croatia = 10,
    Cyprus = 11,
    Czech_Republic = 12,
    Denmark = 13,
    England = 14,
    Montenegro = 15,
    Faroe_Islands = 16,
    Finland = 17,
    France = 18,
    North_Macedonia = 19,
    Georgia = 20,
    Germany = 21,
    Greece = 22,
    Hungary = 23,
    Iceland = 24,
    Republic_of_Ireland = 25,
    Israel = 26,
    Italy = 27,
    Latvia = 28,
    Liechtenstein = 29,
    Lithuania = 30,
    Luxembourg = 31,
    Malta = 32,
    Moldova = 33,
    Holland = 34,
    Northern_Ireland = 35,
    Norway = 36,
    Poland = 37,
    Portugal = 38,
    Romania = 39,
    Russia = 40,
    San_Marino = 41,
    Scotland = 42,
    Slovakia = 43,
    Slovenia = 44,
    Spain = 45,
    Sweden = 46,
    Switzerland = 47,
    Turkey = 48,
    Ukraine = 49,
    Wales = 50,
    Serbia = 51,
    Argentina = 52,
    Bolivia = 53,
    Brazil = 54,
    Chile = 55,
    Colombia = 56,
    Ecuador = 57,
    Paraguay = 58,
    Peru = 59,
    Uruguay = 60,
    Venezuela = 61,
    Anguilla = 62,
    Antigua_and_Barbuda = 63,
    Aruba = 64,
    Bahamas = 65,
    Barbados = 66,
    Belize = 67,
    Bermuda = 68,
    British_Virgin_Islands = 69,
    Canada = 70,
    Cayman_Islands = 71,
    Costa_Rica = 72,
    Cuba = 73,
    Dominica = 74,
    El_Salvador = 76,
    Grenada = 77,
    Guatemala = 78,
    Guyana = 79,
    Haiti = 80,
    Honduras = 81,
    Jamaica = 82,
    Mexico = 83,
    Montserrat = 84,
    Curacao = 85,
    Nicaragua = 86,
    Panama = 87,
    Puerto_Rico = 88,
    St_Kitts_and_Nevis = 89,
    St_Lucia = 90,
    St_Vincent_and_the_Grenadines = 91,
    Suriname = 92,
    Trinidad_and_Tobago = 93,
    Turks_and_Caicos_Islands = 94,
    United_States = 95,
    US_Virgin_Islands = 96,
    Algeria = 97,
    Angola = 98,
    Benin = 99,
    Botswana = 100,
    Burkina_Faso = 101,
    Burundi = 102,
    Cameroon = 103,
    Cape_Verde_Islands = 104,
    Central_African_Republic = 105,
    Chad = 106,
    Congo = 107,
    Cote_d_Ivoire = 108,
    Djibouti = 109,
    Congo_DR = 110,
    Egypt = 111,
    Equatorial_Guinea = 112,
    Eritrea = 113,
    Ethiopia = 114,
    Gabon = 115,
    Gambia = 116,
    Ghana = 117,
    Guinea = 118,
    Guinea_Bissau = 119,
    Kenya = 120,
    Lesotho = 121,
    Liberia = 122,
    Libya = 123,
    Madagascar = 124,
    Malawi = 125,
    Mali = 126,
    Mauritania = 127,
    Mauritius = 128,
    Morocco = 129,
    Mozambique = 130,
    Namibia = 131,
    Niger = 132,
    Nigeria = 133,
    Rwanda = 134,
    Sao_Tome_e_Principe = 135,
    Senegal = 136,
    Seychelles = 137,
    Sierra_Leone = 138,
    Somalia = 139,
    South_Africa = 140,
    Sudan = 141,
    Eswatini = 142,
    Tanzania = 143,
    Togo = 144,
    Tunisia = 145,
    Uganda = 146,
    Zambia = 147,
    Zimbabwe = 148,
    Afghanistan = 149,
    Bahrain = 150,
    Bangladesh = 151,
    Bhutan = 152,
    Brunei_Darussalam = 153,
    Cambodia = 154,
    China_PR = 155,
    Guam = 157,
    Hong_Kong = 158,
    India = 159,
    Indonesia = 160,
    Iran = 161,
    Iraq = 162,
    Japan = 163,
    Jordan = 164,
    Kazakhstan = 165,
    Korea_DPR = 166,
    Korea_Republic = 167,
    Kuwait = 168,
    Kyrgyzstan = 169,
    Laos = 170,
    Lebanon = 171,
    Macau = 172,
    Malaysia = 173,
    Maldives = 174,
    Mongolia = 175,
    Myanmar = 176,
    Nepal = 177,
    Oman = 178,
    Pakistan = 179,
    Palestine = 180,
    Philippines = 181,
    Qatar = 182,
    Saudi_Arabia = 183,
    Singapore = 184,
    Sri_Lanka = 185,
    Syria = 186,
    Tajikistan = 187,
    Thailand = 188,
    Turkmenistan = 189,
    United_Arab_Emirates = 190,
    Uzbekistan = 191,
    Vietnam = 192,
    Yemen = 193,
    American_Samoa = 194,
    Australia = 195,
    Cook_Islands = 196,
    Fiji = 197,
    New_Zealand = 198,
    Papua_New_Guinea = 199,
    Samoa = 200,
    Solomon_Islands = 201,
    Tahiti = 202,
    Tonga = 203,
    Vanuatu = 204,
    Gibraltar = 205,
    Greenland = 206,
    Dominican_Republic = 207,
    Estonia = 208,
    Timor_Leste = 212,
    Chinese_Taipei = 213,
    Comoros = 214,
    New_Caledonia = 215,
    South_Sudan = 218,
    Kosovo = 219,
};

enum class SpeechLanguageId {
    English = 0,
    French = 1,
    German = 2,
    Italian = 3,
    Spanish = 4,
    BrazilPortuguese = 5,
    Korean = 7,
    Dutch = 8,
    Danish = 9,
    Swedish = 10,
    Norwegian = 11,
    Portuguese = 12,
    Other = 13
};

UInt Fifa07Converter::FoomCountryToFifaId(foom::nation *n) {
    if (n->mConverterData.mFIFAID != 0)
        return n->mConverterData.mFIFAID;
    for (auto const &fmn : mConverter->mFoomDatabase->mNations) {
        if (fmn.second.mConverterData.mFIFAManagerID == n->mConverterData.mFIFAManagerReplacementID && fmn.second.mConverterData.mFIFAID != 0)
            return fmn.second.mConverterData.mFIFAID;
    }
    ::Error(L"Unable to find FIFA ID for country %s", n->mName.c_str());
    return 0;
}

void Fifa07Converter::Convert(Converter *c, Path const &dbPath) {
    std::wcout << "Converting to FIFA07 database" << std::endl;
    mConverter = c;
    Path inPath = dbPath / "fifa07";
    Path outPath = inPath / "output";
    Path outDbPath = outPath / "fifa_db";
    Path outGraphicsPath = outPath / "fifa_db";
    Path contentPath = R"(E:\Projects\fifam\content\fifa07)";
    Path contentOutput = contentPath / "output";
    Path contentTemplate = contentPath / "template";
    Vector<Path> teamBadgesDirs = {
        //R"(E:\Projects\fifam\content\fm13\update_badges_clean2\badges\clubs\256x256)",
        //R"(E:\Projects\fifam\content\fm13\update_badges_clean\badges\clubs\256x256)",
        R"(E:\Projects\fifam\content\fm13\badges_clean\badges\clubs\256x256)"
    };
    Vector<Path> leagueBadgesDirs = {
        //R"(E:\Projects\fifam\content\fm13\update_badges_2\badges\Leagues\256x256)",
        //R"(E:\Projects\fifam\content\fm13\update_badges\badges\Leagues\256x256)",
        R"(E:\Projects\fifam\content\fm13\badges\badges\Leagues\256x256)"
    };
    Path minikitsDir = R"(E:\Projects\fifam\content\fm13\art_05\data\minikits)";
    Fifa07Database::m_currentGameVersion = 7;
    //mLeagues.push_back(Fifa07League(76, L"Rest of World", 4, 0, 12));
    //mLeagues.push_back(Fifa07League(78, L"International", 5, 0, 1));
    //mLeagues.push_back(Fifa07League(2000, L"FIFA Career Saved Teams", 6, 0, 1));
    //mLeagues.push_back(Fifa07League(2028, L"World League", 7, 0, 1));
    UInt fifaLeagueNextId = 3'000;
    UInt fifaTeamNextId = 150'000;
    Set<UInt> foomTeamIdsInCountryLeagues;
    for (auto &entry : c->mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto countryId = nation.mConverterData.mFIFAID;
        if (countryId != 0) {
            Map<UInt, Converter::DivisionInfo *> divLeagueLevels;
            Map<UInt, Vector<Converter::DivisionInfo *>> divLeagues;
            Vector<Converter::PlayOffInfo *> vecPlayOffs;
            Map<UInt, Converter::PlayOffInfo *> playOffs;
            // find all division competitions for this country
            Int maxLevel = -1;
            for (Converter::DivisionInfo &divInfo : c->mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    if (divInfo.mPriority >= 1) {
                        if (divInfo.mType == Converter::DivisionInfo::League)
                            divLeagues[divInfo.mLevel].push_back(&divInfo);
                        else if (divInfo.mType == Converter::DivisionInfo::Level)
                            divLeagueLevels[divInfo.mLevel] = &divInfo;
                        if (divInfo.mLevel > maxLevel)
                            maxLevel = divInfo.mLevel;
                    }
                }
            }
            for (auto &po : c->mPlayOffs) {
                if (po.mNationID == nation.mID) {
                    vecPlayOffs.push_back(&po);
                    playOffs[po.mID] = &po;
                }
            }
            std::sort(vecPlayOffs.begin(), vecPlayOffs.end(), [](Converter::PlayOffInfo *a, Converter::PlayOffInfo *b) {
                return a->mID < b->mID;
            });
            if (maxLevel >= 0) {
                // sort leagues
                for (auto &e : divLeagues) {
                    if (e.second.size() > 1) {
                        std::sort(e.second.begin(), e.second.end(), [](Converter::DivisionInfo *a, Converter::DivisionInfo *b) {
                            return a->mOrder < b->mOrder;
                        });
                    }
                }
                // create leagues
                Int minLevelWithReserveTeams = -1;
                Bool customLeagues = false;
                Bool leagueSplit = false;

                if (!customLeagues) {
                    for (auto &leagues : divLeagues) {
                        for (Converter::DivisionInfo *lg : leagues.second) {
                            Bool leagueCustomFormat = lg->mSplit.first > 0 || lg->mTotalTeamsPromotionPlayoff > 0 || lg->mTotalTeamsRelegationPlayoff > 0;
                            if (!customLeagues && leagueCustomFormat) {
                                customLeagues = true;
                                break;
                            }
                        }
                    }
                }

                UInt leagueIndex = 0;

                for (auto &leagues : divLeagues) {
                    UInt teamCounter = 0;
                    for (Converter::DivisionInfo *lg : leagues.second) {
                        foom::comp *comp = c->mFoomDatabase->get<foom::comp>(lg->mID);
                        if (!comp) {
                            //if (c->mErrors) {
                            //    Error(L"Competition is not available\nCompetitionName: %s\nCompetitionID: %d", lg->mName.c_str(), lg->mID);
                            //}
                            continue;
                        }
                        if (!nation.mConverterData.mDomesticComps.league)
                            nation.mConverterData.mDomesticComps.league = lg;
                        Fifa07League fifa07league;
                        fifa07league.SetProperty("NationalityName", nation.mNationalityName);
                        fifa07league.SetProperty("CountryName", nation.mName);
                        UInt fifamId = (nation.mConverterData.mFIFAManagerReplacementID << 24) | 0x10000 | leagueIndex;
                        if (c->mFifamCompIdToFifa07LeagueId.contains(fifamId))
                            fifa07league.leagueid = c->mFifamCompIdToFifa07LeagueId[fifamId];
                        else
                            fifa07league.leagueid = fifaLeagueNextId++;
                        fifa07league.countryid = countryId;
                        if (lg->mName.length() > 29)
                            fifa07league.leaguename = lg->mShortName;
                        else
                            fifa07league.leaguename = lg->mName;
                        if (fifa07league.leaguename.length() > 29)
                            fifa07league.leaguename = FifamNames::LimitName(fifa07league.leaguename, 29);
                        fifa07league.leagueprestige = Utils::MapTo(comp->mReputation, 0, 9999, 0, 20);
                        fifa07league.level = lg->mLevel + 1;

                        for (auto entry : comp->mVecTeams) {
                            foom::club *team = (foom::club *)entry;
                            if (!team) {
                                if (c->mErrors) {
                                    ::Error(L"Invalid club pointer in league\nLeague: %s", lg->mName.c_str());
                                }
                                continue;
                            }
                            UInt fifamTeamId = team->mConverterData.mFIFAManagerID;
                            foom::club *mainTeam = nullptr; // for reserve teams
                            Bool noTeam = false;
                            Bool isExtinct = team->mExtinct;
                            if (team->mConverterData.mParentClub)
                                isExtinct = team->mConverterData.mParentClub->mExtinct;
                            if (isExtinct) {
                                if (c->mErrors) {
                                    ::Error(L"Extinct club in the league\nClub: '%s'\nLeague: '%s'",
                                        team->mName.c_str(), lg->mName.c_str());
                                }
                            }
                            else if (foomTeamIdsInCountryLeagues.contains(team->mID)) {
                                if (c->mErrors) {
                                    Message(Utils::Format(L"Team already present in other league\nClub: '%s'\nLeague: '%s'",
                                        team->mName.c_str(), lg->mName.c_str()));
                                }
                            }
                            Fifa07Team fifa07team;
                            FifaTeam *fifaTeam = nullptr;
                            if (team->mConverterData.mFIFAID != 0) {
                                fifa07team.teamid = team->mConverterData.mFIFAID;
                                fifaTeam = c->mFifaDatabase->GetTeam(team->mConverterData.mFIFAID);
                            }
                            else
                                fifa07team.teamid = fifaTeamNextId++;
                            fifa07team.teamname = team->mShortName;
                            if (fifa07team.teamname.length() > 29)
                                fifa07team.teamname = FifamNames::LimitName(fifa07team.teamname, 29);
                            fifa07team.SetProperty("FullName", team->mName);
                            fifa07team.generickitid = fifa07team.teamid;
                            fifa07team.balltype = 11; // adjust later
                            fifa07team.adboardid = 1; // adjust later
                            fifa07team.stadiumid = 1; // adjust later
                            fifa07team.genericbanner = 1; // ?
                            fifa07team.trainingstadiumid = 38; // 38 or 39
                            switch (countryId) {
                            case (int)FifaCountryId::Antigua_and_Barbuda:
                            case (int)FifaCountryId::Australia:
                            case (int)FifaCountryId::Bahamas:
                            case (int)FifaCountryId::Barbados:
                            case (int)FifaCountryId::Belize:
                            case (int)FifaCountryId::Canada:
                            case (int)FifaCountryId::Dominica:
                            case (int)FifaCountryId::England:
                            case (int)FifaCountryId::Grenada:
                            case (int)FifaCountryId::Guyana:
                            case (int)FifaCountryId::Jamaica:
                            case (int)FifaCountryId::New_Zealand:
                            case (int)FifaCountryId::Northern_Ireland:
                            case (int)FifaCountryId::Republic_of_Ireland:
                            case (int)FifaCountryId::Scotland:
                            case (int)FifaCountryId::St_Kitts_and_Nevis:
                            case (int)FifaCountryId::St_Lucia:
                            case (int)FifaCountryId::St_Vincent_and_the_Grenadines:
                            case (int)FifaCountryId::Trinidad_and_Tobago:
                            case (int)FifaCountryId::United_States:
                            case (int)FifaCountryId::Wales:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::English;
                                break;
                            case (int)FifaCountryId::Belgium:
                            case (int)FifaCountryId::Benin:
                            case (int)FifaCountryId::Burkina_Faso:
                            case (int)FifaCountryId::Burundi:
                            case (int)FifaCountryId::Central_African_Republic:
                            case (int)FifaCountryId::Chad:
                            case (int)FifaCountryId::Comoros:
                            case (int)FifaCountryId::Congo:
                            case (int)FifaCountryId::Congo_DR:
                            case (int)FifaCountryId::Cameroon:
                            case (int)FifaCountryId::Djibouti:
                            case (int)FifaCountryId::France:
                            case (int)FifaCountryId::Gabon:
                            case (int)FifaCountryId::Guinea:
                            case (int)FifaCountryId::Haiti:
                            case (int)FifaCountryId::Madagascar:
                            case (int)FifaCountryId::Mali:
                            case (int)FifaCountryId::Niger:
                            case (int)FifaCountryId::Rwanda:
                            case (int)FifaCountryId::Senegal:
                            case (int)FifaCountryId::Seychelles:
                            case (int)FifaCountryId::Togo:
                            case (int)FifaCountryId::Vanuatu:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::French;
                            case (int)FifaCountryId::Austria:
                            case (int)FifaCountryId::Germany:
                            case (int)FifaCountryId::Liechtenstein:
                            case (int)FifaCountryId::Switzerland:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::German;
                            case (int)FifaCountryId::Italy:
                            case (int)FifaCountryId::San_Marino:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Italian;
                            case (int)FifaCountryId::Argentina:
                            case (int)FifaCountryId::Bolivia:
                            case (int)FifaCountryId::Colombia:
                            case (int)FifaCountryId::Chile:
                            case (int)FifaCountryId::Costa_Rica:
                            case (int)FifaCountryId::Cuba:
                            case (int)FifaCountryId::Dominican_Republic:
                            case (int)FifaCountryId::Ecuador:
                            case (int)FifaCountryId::El_Salvador:
                            case (int)FifaCountryId::Equatorial_Guinea:
                            case (int)FifaCountryId::Guatemala:
                            case (int)FifaCountryId::Honduras:
                            case (int)FifaCountryId::Mexico:
                            case (int)FifaCountryId::Nicaragua:
                            case (int)FifaCountryId::Panama:
                            case (int)FifaCountryId::Spain:
                            case (int)FifaCountryId::Paraguay:
                            case (int)FifaCountryId::Peru:
                            case (int)FifaCountryId::Uruguay:
                            case (int)FifaCountryId::Venezuela:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Spanish;
                            case (int)FifaCountryId::Brazil:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::BrazilPortuguese;
                            case (int)FifaCountryId::Korea_DPR:
                            case (int)FifaCountryId::Korea_Republic:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Korean;
                            case (int)FifaCountryId::Aruba:
                            case (int)FifaCountryId::Curacao:
                            case (int)FifaCountryId::Holland:
                            case (int)FifaCountryId::Suriname:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Dutch;
                            case (int)FifaCountryId::Denmark:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Danish;
                            case (int)FifaCountryId::Sweden:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Swedish;
                            case (int)FifaCountryId::Norway:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Norwegian;
                            case (int)FifaCountryId::Angola:
                            case (int)FifaCountryId::Cape_Verde_Islands:
                            case (int)FifaCountryId::Guinea_Bissau:
                            case (int)FifaCountryId::Mozambique:
                            case (int)FifaCountryId::Portugal:
                            case (int)FifaCountryId::Sao_Tome_e_Principe:
                            case (int)FifaCountryId::Timor_Leste:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Portuguese;
                            default:
                                fifa07team.speechcountryid = (int)SpeechLanguageId::Other;
                                break;
                            }
                            fifa07team.opponent = 1; // adjus later
                            fifa07team.assetid = fifa07team.teamid;
                            fifa07team.teamchemistry = 50; // adjust later
                            fifa07team.overallrating = 50; // adjust later
                            if (team->mBasedNation && FoomCountryToFifaId(team->mBasedNation) != 0)
                                fifa07team.signsyouthplayersfrom = FoomCountryToFifaId(team->mBasedNation);
                            else
                                fifa07team.signsyouthplayersfrom = countryId;

                            fifa07team.signsplayersfromprimary = 0;
                            fifa07team.signsplayersfromsecondary = 0;
                            if (fifaTeam) {
                                fifa07team.initialtransferbudget = 0;
                                fifa07team.internationalprestige = 0;
                                fifa07team.domesticprestige = 0;
                            }
                            else {
                                fifa07team.initialtransferbudget = 0;
                                fifa07team.internationalprestige = 0;
                                fifa07team.domesticprestige = 0;
                            }
                            if (fifa07team.internationalprestige >= 15)
                                fifa07team.homecrowdsize = 3;
                            else if (fifa07team.internationalprestige >= 9)
                                fifa07team.homecrowdsize = 2;
                            else
                                fifa07team.homecrowdsize = 1;
                            if (fifaTeam) {
                                fifa07team.teamprimarycolor = 0;
                                fifa07team.teamsecondarycolor = 0;
                            }
                            else {
                                fifa07team.teamprimarycolor = 0;
                                fifa07team.teamsecondarycolor = 0;
                            }
                            fifa07team.defaultformationid = 0;

                            fifa07team.SetProperty("FoomTeam", team);
                            fifa07team.SetProperty("FifamID", fifamTeamId);

                            Path teamLogoPath;
                            String teamLogoName = Utils::Format(L"%08X.tga", fifamTeamId);
                            for (auto const &p : teamBadgesDirs) {
                                if (exists(p / teamLogoName)) {
                                    teamLogoPath = p / teamLogoName;
                                    break;
                                }
                            }
                            if (teamLogoPath.empty() && team->mConverterData.mParentClub) {
                                UInt fifamMainTeamId = team->mConverterData.mParentClub->mConverterData.mFIFAManagerID;
                                teamLogoName = Utils::Format(L"%08X.tga", fifamMainTeamId);
                                for (auto const &p : teamBadgesDirs) {
                                    if (exists(p / teamLogoName)) {
                                        teamLogoPath = p / teamLogoName;
                                        break;
                                    }
                                }
                            }
                            if (!teamLogoPath.empty()) {
                                Image teamLogo(teamLogoPath.string());
                                teamLogo.resize(Magick::Geometry(200 - 12, 200 - 12));
                                teamLogo.extent(Magick::Geometry(256, 256, -6, -6), Magick::Color(0, 0, 0, 0));
                                Path teamDir = contentOutput / "logos" / Utils::Format("t%d", fifa07team.teamid);
                                create_directories(teamDir);
                                teamLogo.write((teamDir / Utils::Format("1   @t%d.tga", fifa07team.teamid)).string());
                                copy(contentTemplate / "teamlogo" / "t1.apt", teamDir / Utils::Format("t%d.apt", fifa07team.teamid), copy_options::overwrite_existing);
                                copy(contentTemplate / "teamlogo" / "t1.const", teamDir / Utils::Format("t%d.const", fifa07team.teamid), copy_options::overwrite_existing);
                                copy(contentTemplate / "teamlogo" / "t1.o", teamDir / Utils::Format("t%d.o", fifa07team.teamid), copy_options::overwrite_existing);
                            }

                            Array<Char, 2> kitTypes = { 'h', 'a' };
                            for (UChar k = 0; k < 2; k++) {
                                Path teamKitPath = minikitsDir / Utils::Format(L"%08X_%c.png", fifamTeamId, kitTypes[k]);
                                if (exists(teamKitPath)) {
                                    Image teamKit(teamKitPath.string());
                                    teamKit.extent(Magick::Geometry(256, 256, 29, 10), Magick::Color(0, 0, 0, 0));
                                    Path teamKitDir = contentOutput / "kits" / Utils::Format("j%d_%05d", k, fifa07team.teamid);
                                    create_directories(teamKitDir);
                                    teamKit.write((teamKitDir / Utils::Format("1   @j%d_%05d.tga", k, fifa07team.teamid)).string());
                                    copy(contentTemplate / "teamkit" / "j0_00001.apt", teamKitDir / Utils::Format("j%d_%05d.apt", k, fifa07team.teamid), copy_options::overwrite_existing);
                                    copy(contentTemplate / "teamkit" / "j0_00001.const", teamKitDir / Utils::Format("j%d_%05d.const", k, fifa07team.teamid), copy_options::overwrite_existing);
                                    copy(contentTemplate / "teamkit" / "j0_00001.o", teamKitDir / Utils::Format("j%d_%05d.o", k, fifa07team.teamid), copy_options::overwrite_existing);
                                }
                            }

                            mTeams.push_back(fifa07team);
                            fifa07league.teams.push_back(fifa07team.teamid);
                        }
                        fifa07league.SetProperty("FoomComp", comp);
                        fifa07league.SetProperty("Div", lg);
                        fifa07league.SetProperty("FifamID", fifamId);

                        Image leagueBackground((contentTemplate / "leaguebg.tga").string());
                        Path leagueLogoPath;
                        String leagueLogoName = Utils::Format(L"%08X.tga", fifamId);
                        for (auto const &p : leagueBadgesDirs) {
                            if (exists(p / leagueLogoName)) {
                                leagueLogoPath = p / leagueLogoName;
                                break;
                            }
                        }
                        if (!leagueLogoPath.empty()) {
                            Image leagueLogo(leagueLogoPath.string());
                            leagueBackground.composite(leagueLogo, 154, 9, MagickCore::OverCompositeOp);
                            Image smallLeagueLogo(leagueLogo);
                            smallLeagueLogo.resize(Magick::Geometry(50, 50));
                            leagueBackground.composite(smallLeagueLogo, 17, 65, MagickCore::OverCompositeOp);
                            Path leagueBgDir = contentOutput / "lglogos" / Utils::Format("%d", fifa07league.leagueid);
                            create_directories(leagueBgDir);
                            leagueBackground.write((leagueBgDir / Utils::Format("1   @%d.tga", fifa07league.leagueid)).string());
                            copy(contentTemplate / "leaguebg" / "1.apt", leagueBgDir / Utils::Format("%d.apt", fifa07league.leagueid), copy_options::overwrite_existing);
                            copy(contentTemplate / "leaguebg" / "1.const", leagueBgDir / Utils::Format("%d.const", fifa07league.leagueid), copy_options::overwrite_existing);
                            copy(contentTemplate / "leaguebg" / "1.o", leagueBgDir / Utils::Format("%d.o", fifa07league.leagueid), copy_options::overwrite_existing);
                            leagueLogo.resize(Magick::Geometry(200 - 12, 200 - 12));
                            leagueLogo.extent(Magick::Geometry(256, 256, -6, -6), Magick::Color(0, 0, 0, 0));
                            Path leagueDir = contentOutput / "logos" / Utils::Format("l%d", fifa07league.leagueid);
                            create_directories(leagueDir);
                            leagueLogo.write((leagueDir / Utils::Format("1   @l%d.tga", fifa07league.leagueid)).string());
                            copy(contentTemplate / "leaguelogo" / "l1.apt", leagueDir / Utils::Format("l%d.apt", fifa07league.leagueid), copy_options::overwrite_existing);
                            copy(contentTemplate / "leaguelogo" / "l1.const", leagueDir / Utils::Format("l%d.const", fifa07league.leagueid), copy_options::overwrite_existing);
                            copy(contentTemplate / "leaguelogo" / "l1.o", leagueDir / Utils::Format("l%d.o", fifa07league.leagueid), copy_options::overwrite_existing);
                        }

                        if (!fifa07league.teams.empty()) {
                            std::sort(fifa07league.teams.begin(), fifa07league.teams.end());
                            mLeagues.push_back(fifa07league);
                        }
                        if (lg->mSplit.first != 0)
                            leagueIndex += 2;
                        else
                            leagueIndex += 1;
                    }
                }
            }
        }
    }
    create_directories(outDbPath);
    std::sort(mLeagues.begin(), mLeagues.end(), [](Fifa07League const &a, Fifa07League const &b) {
        return a.leagueid < b.leagueid;
    });
    std::sort(mTeams.begin(), mTeams.end(), [](Fifa07Team const &a, Fifa07Team const &b) {
        return a.teamid < b.teamid;
    });

    Fifa07DataFile fifa07file;
    if (fifa07file.Open(outDbPath / "leagues.txt", Fifa07DataFile::Write, L"leagueid	leaguename	level	countryid	leagueprestige")) {
        for (auto &l : mLeagues) {
            Fifa07DataFile::Line line;
            fifa07file.BeforeLine(line);
            l.Parse(line);
            fifa07file.NextLine(line);
        }
        fifa07file.Close();
    }
    if (fifa07file.Open(outDbPath / "teams.txt", Fifa07DataFile::Write, Fifa07Database::m_currentGameVersion == 7 ?
        L"teamid	teamname	generickitid	balltype	adboardid	stadiumid	genericbanner	trainingstadiumid	speechcountryid	opponent	assetid	teamchemistry	overallrating	signsyouthplayersfrom	signsplayersfromprimary	signsplayersfromsecondary	initialtransferbudget	internationalprestige	domesticprestige	homecrowdsize	teamprimarycolor	teamsecondarycolor" :
        L"teamid	teamname	generickitid	balltype	adboardid	stadiumid	genericbanner	trainingstadiumid	speechcountryid	opponent	assetid	teamchemistry	overallrating	signsyouthplayersfrom	signsplayersfromprimary	signsplayersfromsecondary	initialtransferbudget	internationalprestige	domesticprestige	homecrowdsize	teamprimarycolor	teamsecondarycolor	defaultformationid"))
    {
        for (auto &t : mTeams) {
            Fifa07DataFile::Line line;
            fifa07file.BeforeLine(line);
            t.Parse(line);
            fifa07file.NextLine(line);
        }
        fifa07file.Close();
    }
    if (fifa07file.Open(outDbPath / "leagueteamlinks.txt", Fifa07DataFile::Write, L"leagueid	teamid")) {
        for (auto &l : mLeagues) {
            for (auto teamid : l.teams) {
                Fifa07DataFile::Line line;
                fifa07file.BeforeLine(line);
                line >> l.leagueid >> teamid;
                fifa07file.NextLine(line);
            }
        }
        fifa07file.Close();
    }

    // nation names - localized
    // stadium names - localized
    FifamWriter translation(outPath / "names.tr");
    for (auto &l : mLeagues) {
        translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 2, l.leaguename);
        translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 3, l.leaguename);
        translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 4, l.GetProperty<String>("NationalityName") + L" Leagues");
        translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 5, l.GetProperty<String>("CountryName"));
    }
    for (auto &t : mTeams) {
        translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 0, t.GetProperty<String>("FullName"));
        translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 1, t.teamname);
        translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 2, t.teamname);
    }
    translation.Close();
}
