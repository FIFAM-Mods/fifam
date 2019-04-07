#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"
#include "FifaDatabase.h"
#include "AppearanceGenerator.h"
#include "FifamCompLeague.h"
#include "FifamCompCup.h"
#include "FifamCompRound.h"

const UShort CURRENT_YEAR = 2018;

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
    FifaDatabase *mFifaDatabase = nullptr;
    UInt mCurrentGameId = FifamDatabase::LATEST_GAME_VERSION;
    foom::db *mFoomDatabase = nullptr;

    struct DivisionInfo {
        enum Type {
            League,
            Level,
        };

        Int mNationID = -1;
        String mName;
        String mShortName;
        Int mID = -1;
        Type mType = League;
        Int mLevel = 0;
        Int mTeams = 0;
        Int mRep = 0;
        Int mOrder = 0;
        Int mPriority = 0;
        Int mRounds = -1;
        Int mPromoted = 0;
        Int mRelegated = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
        Int mWinterBreakStart = 0;
        Int mWinterBreakEnd = 0;
        Int mNumSubs = 3;
        Int mForeignersLimit = 0;
        Int mNonEuSigns = 0;
        Int mDomesticPlayers = 0;
        Int mU21Players = 0;
        Int mReserveTeamsAllowed = -1;
        FifamEqualPointsSorting mSorting;
        Int mTvBonus = -1;
        Int mWinBouns = -1;
        Int mPlaceBonus = -1;
        Int mAttendanceMp = 0;
        Int mTransfersMp = 0;
        FifamCompID mCompID;
    };

    struct CupInfo {
        enum Type {
            FA = 0,
            League = 1,
            Supercup = 2
        };
        Int mNationID = -1;
        String mName;
        String mShortName;
        Int mID = -1;
        Int mLevel = 0;
        Type mType = League;
        Int mReputation = 0;
        Int mPriority = 0;
        FifamCupSystemType mTemplateType;
        Array<String, 8> mRoundDesc;
        String mStructure;
        Int mMaxLevel = 0;
        Int mNumSubs = 0;
        Int mBonus = 0;
        Int mTvBonus = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
    };

    struct FixtureInfo {
        Date mDate;
        foom::club *mTeam1 = 0;
        foom::club *mTeam2 = 0;
    };

    struct ClubColor {
        enum ClubColorType {
            DEFAULT,
            TEAM,
            KIT,
            RGB
        };
        ClubColorType type = ClubColorType::DEFAULT;
        Array<Color, 2> rgb;
    };

    Vector<DivisionInfo> mDivisions;
    Vector<CupInfo> mCups;
    Map<UInt, Vector<FixtureInfo>> mFixturesPerLeague;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNextFreeUID = {};
    Array<Map<UInt, Vector<FifamCompLeague *>>, FifamDatabase::NUM_COUNTRIES> mLeaguesSystem;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNumTeamsInLeagueSystem = {};
    Array<Vector<foom::player *>, FifamDatabase::NUM_COUNTRIES> mNationalTeamPlayers;
    UInt mPersonIdCounter = 1;
    Map<Int, Path> mAvailableBadges;
    AppearanceGenerator appearanceGenerator;
    Map<Int, ClubColor> mClubColorsMap;
    Map<Int, Int> mPenaltyPointsMap;
    Map<Int, String> mAbbreviationMap;
    
    Date GetCurrentDate() { return Date(1, 7, CURRENT_YEAR); }
    Date FmEmptyDate() { return Date(1, 1, 1900); }

    void ReadAdditionalInfo(Path const &infoPath, UInt gameId);
    void Convert(UInt gameId, Bool writeToGameFolder);
    void Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder);

    Int ConvertPlayerAttribute(Int attr, UInt gameId = 14);
    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation, UInt gameId);
    void ConvertClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertReserveClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertClubStadium(FifamClub *dst, UInt gameId);
    FifamClub *CreateAndConvertClub(UInt gameId, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div, bool convertSquad);
    void ConvertReferee(FifamReferee *dst, foom::official *official);
    void ConvertKitsAndColors(FifamClub *dst, Int foomId, Vector<foom::kit> const &kits, Int badgeType, Color const &teamBackgroundColor,
        Color const &teamForegroundColor, UInt gameId);
    FifamPlayer *CreateAndConvertPlayer(UInt gameId, foom::player *p, FifamClub *club);
    FifamStaff *CreateAndConvertStaff(foom::non_player *p, FifamClub *club, FifamClubStaffPosition position);
    void ConvertPersonAttributes(FifamPerson *person, foom::person *p);
    void CreateStaffMembersForClub(UInt gameId, foom::team *team, FifamClub *dst, Bool isNationalTeam);
    UChar GetPlayerLevelFromCA(Int ca);

    Bool IsConvertable(foom::person *p, UInt gameId);
    Bool IsConvertable(foom::official *o, UInt gameId);
    
    Bool IsIconicPlayer(Int playerId);
    Bool IsIntrovertPlayer(Int playerId);
    Bool IsExtrovertPlayer(Int playerId);
    Bool IsInsecurePlayer(Int playerId);
    Bool IsFansFavouritePlayer(Int playerId);
    Bool IsSensitivePlayer(Int playerId);

    UChar GetPlayerLevel(FifamPlayer *player, Bool includeExperience, UInt gameId);
    UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool includeExperience, UInt gameId);

    Bool IsPlayerRetiredFromNationalTeam(Int playerId);

    FifamClubLink GetCompWinner(foom::comp *c, Bool checkPreviousSeason = false);
    void GetCompWinnerAndRunnerUp(foom::comp *c, FifamClubLink &outWinner, FifamClubLink &outRunnerUp, Bool checkPreviousSeason = false);
    FifamClubLink GetCompHost(foom::comp *c, Bool checkPreviousSeason = false);

    FifamClubLink GetTeamClubLink(foom::team *t, Bool allowReserveTeam = true);

    Map<UShort, foom::team *> GetWinnersList(Vector<foom::comp *> const &inputComps, bool isSupercup = false);

    FifamFormation ConvertFormationId(Int id);

    Bool GenerateCalendar(FifamNation const &countryId, FifamDatabase *database, Vector<FifamCompLeague *> const &leagues, Vector<FifamCompCup *> const &cups);

    FifamPlayer *CreateAndConvertFifaPlayer(UInt gameId, FifaPlayer *p, FifamClub *club);

    /*
    void GenerateMaleFemaleNames() {
        const UInt MIN_FREQUENCY = 2;

        Map<FifamLanguage, Map<String, UInt>> maleNames;
        Map<FifamLanguage, Map<String, UInt>> femaleNames;
        Map<FifamLanguage, Map<String, UInt>> surnames;
        Map<String, Set<FifamLanguage>> maleNameUsage;
        Map<String, Set<FifamLanguage>> femaleNameUsage;
        Map<String, Set<FifamLanguage>> surnameUsage;

        auto ProcessPersons = [&](auto &dbmap) {
            for (auto &[k, p] : dbmap) {
                FifamLanguage languageId = FifamLanguage::None;
                if (p.mNation && p.mNation->mConverterData.mFIFAManagerID > 0 && p.mNation->mConverterData.mFIFAManagerID < FifamDatabase::NUM_COUNTRIES) {
                    FifamCountry *country = mFifamDatabase->GetCountry(p.mNation->mConverterData.mFIFAManagerID);
                    if (country) {
                        switch (country->mId) {
                        case FifamNation::Liberia:
                        case FifamNation::Zambia:
                        case FifamNation::Lesotho:
                        case FifamNation::Swaziland:
                        case FifamNation::Madagascar:
                        case FifamNation::Burkina_Faso:
                        case FifamNation::Namibia:
                        case FifamNation::Zimbabwe:
                        case FifamNation::Somalia:
                        case FifamNation::Niger:
                        case FifamNation::Mali:
                        case FifamNation::Rwanda:
                        case FifamNation::Malawi:
                        case FifamNation::Botswana:
                        case FifamNation::Senegal:
                        case FifamNation::South_Africa:
                        case FifamNation::Cape_Verde_Islands:
                        case FifamNation::Congo:
                        case FifamNation::Angola:
                        case FifamNation::Central_African_Rep:
                        case FifamNation::Djibouti:
                        case FifamNation::Guinea_Bissau:
                        case FifamNation::Mozambique:
                        case FifamNation::Chad:
                        case FifamNation::Mauritius:
                        case FifamNation::Sierra_Leone:
                        case FifamNation::Gabon:
                        case FifamNation::Ghana:
                        case FifamNation::Togo:
                        case FifamNation::Burundi:
                        case FifamNation::Benin:
                        case FifamNation::Equatorial_Guinea:
                        case FifamNation::Eritrea:
                        case FifamNation::Guinea:
                        case FifamNation::Nigeria:
                            languageId = FifamLanguage::African;
                            break;
                        case FifamNation::Albania:
                            languageId = FifamLanguage::Albanian;
                            break;
                        case FifamNation::Ethiopia:
                            languageId = FifamLanguage::Amharic;
                            break;
                        case FifamNation::Egypt:
                        case FifamNation::Bahrain:
                        case FifamNation::Jordan:
                        case FifamNation::Kuwait:
                        case FifamNation::Lebanon:
                        case FifamNation::Maldives:
                        case FifamNation::Palestinian_Authority:
                        case FifamNation::Saudi_Arabia:
                        case FifamNation::United_Arab_Emirates:
                        case FifamNation::Yemen:
                        case FifamNation::Iraq:
                        case FifamNation::Tunisia:
                        case FifamNation::Mauritania:
                        case FifamNation::Oman:
                        case FifamNation::Syria:
                        case FifamNation::Libya:
                        case FifamNation::Sudan:
                        case FifamNation::Morocco:
                        case FifamNation::Algeria:
                        case FifamNation::Qatar:
                            languageId = FifamLanguage::Arabic;
                            break;
                        case FifamNation::Armenia:
                            languageId = FifamLanguage::Armenian;
                            break;
                        case FifamNation::Bosnia_Herzegovina:
                            languageId = FifamLanguage::Bosnian;
                            break;
                        case FifamNation::Bulgaria:
                            languageId = FifamLanguage::Bulgarian;
                            break;
                        case FifamNation::Azerbaijan:
                            languageId = FifamLanguage::Azerbaijani;
                            break;
                        case FifamNation::China_PR:
                            languageId = FifamLanguage::Chinese;
                            break;
                        case FifamNation::Croatia:
                            languageId = FifamLanguage::Croatian;
                            break;
                        case FifamNation::Czech_Republic:
                            languageId = FifamLanguage::Czech;
                            break;
                        case FifamNation::Denmark:
                            languageId = FifamLanguage::Danish;
                            break;
                        case FifamNation::Netherlands:
                            languageId = FifamLanguage::Dutch;
                            break;
                        case FifamNation::England:
                            languageId = FifamLanguage::English;
                            break;
                        case FifamNation::Estonia:
                            languageId = FifamLanguage::Estonian;
                            break;
                        case FifamNation::Finland:
                            languageId = FifamLanguage::Finnish;
                            break;
                        case FifamNation::France:
                            languageId = FifamLanguage::French;
                            break;
                        case FifamNation::Georgia:
                            languageId = FifamLanguage::Georgian;
                            break;
                        case FifamNation::Germany:
                            languageId = FifamLanguage::German;
                            break;
                        case FifamNation::Greece:
                            languageId = FifamLanguage::Greek;
                            break;
                        case FifamNation::Israel:
                            languageId = FifamLanguage::Hebrew;
                            break;
                        case FifamNation::India:
                            languageId = FifamLanguage::Hindi;
                            break;
                        case FifamNation::Hungary:
                            languageId = FifamLanguage::Hungarian;
                            break;
                        case FifamNation::Iceland:
                            languageId = FifamLanguage::Icelandic;
                            break;
                        case FifamNation::Italy:
                            languageId = FifamLanguage::Italian;
                            break;
                        case FifamNation::Japan:
                            languageId = FifamLanguage::Japanese;
                            break;
                        case FifamNation::Tanzania:
                        case FifamNation::Kenya:
                            languageId = FifamLanguage::Kiswahili;
                            break;
                        case FifamNation::Korea_DPR:
                        case FifamNation::Korea_Republic:
                            languageId = FifamLanguage::Korean;
                            break;
                        case FifamNation::Latvia:
                            languageId = FifamLanguage::Lettish;
                            break;
                        case FifamNation::Lithuania:
                            languageId = FifamLanguage::Lithuanian;
                            break;
                        case FifamNation::FYR_Macedonia:
                            languageId = FifamLanguage::Macedonian;
                            break;
                        case FifamNation::Brunei_Darussalam:
                        case FifamNation::Malaysia:
                            languageId = FifamLanguage::Malay;
                            break;
                        case FifamNation::Malta:
                            languageId = FifamLanguage::Maltese;
                            break;
                        case FifamNation::Norway:
                            languageId = FifamLanguage::Norwegian;
                            break;
                        case FifamNation::Iran:
                            languageId = FifamLanguage::Persian;
                            break;
                        case FifamNation::Poland:
                            languageId = FifamLanguage::Polish;
                            break;
                        case FifamNation::Brazil:
                        case FifamNation::Portugal:
                            languageId = FifamLanguage::Portuguese;
                            break;
                        case FifamNation::Romania:
                            languageId = FifamLanguage::Romanian;
                            break;
                        case FifamNation::Russia:
                            languageId = FifamLanguage::Russian;
                            break;
                        case FifamNation::Serbia:
                            languageId = FifamLanguage::Serbian;
                            break;
                        case FifamNation::Slovakia:
                            languageId = FifamLanguage::Slovak;
                            break;
                        case FifamNation::Slovenia:
                            languageId = FifamLanguage::Slovenian;
                            break;
                        case FifamNation::Spain:
                            languageId = FifamLanguage::Spanish;
                            break;
                        case FifamNation::Thailand:
                            languageId = FifamLanguage::Thai;
                            break;
                        case FifamNation::Turkey:
                            languageId = FifamLanguage::Turkish;
                            break;
                        case FifamNation::Ukraine:
                            languageId = FifamLanguage::Ukrainian;
                            break;
                        case FifamNation::Vietnam:
                            languageId = FifamLanguage::Vietnamese;
                            break;
                        case FifamNation::Sweden:
                            languageId = FifamLanguage::Swedish;
                            break;
                        case FifamNation::Afghanistan:
                            languageId = FifamLanguage::Afghani;
                            break;
                        case FifamNation::Indonesia:
                            languageId = FifamLanguage::Bahasa;
                            break;
                        case FifamNation::Myanmar:
                            languageId = FifamLanguage::Burmese;
                            break;
                        case FifamNation::Kazakhstan:
                            languageId = FifamLanguage::Kazakh;
                            break;
                        case FifamNation::Cambodia:
                            languageId = FifamLanguage::Khmer;
                            break;
                        case FifamNation::Laos:
                            languageId = FifamLanguage::Laotian;
                            break;
                        case FifamNation::Mongolia:
                            languageId = FifamLanguage::Mongolian;
                            break;
                        case FifamNation::Bhutan:
                        case FifamNation::Nepal:
                            languageId = FifamLanguage::Nepali;
                            break;
                        case FifamNation::Philippines:
                            languageId = FifamLanguage::Pilipino;
                            break;
                        case FifamNation::American_Samoa:
                        case FifamNation::Cook_Islands:
                        case FifamNation::Fiji:
                        case FifamNation::Papua_New_Guinea:
                        case FifamNation::Samoa:
                        case FifamNation::Solomon_Islands:
                        case FifamNation::Tahiti:
                        case FifamNation::Tonga:
                        case FifamNation::Vanuatu:
                            languageId = FifamLanguage::Polynesian;
                            break;
                        case FifamNation::Bangladesh:
                        case FifamNation::Pakistan:
                            languageId = FifamLanguage::Punjabi;
                            break;
                        case FifamNation::Sri_Lanka:
                            languageId = FifamLanguage::Sinhala;
                            break;
                        case FifamNation::Turkmenistan:
                            languageId = FifamLanguage::Turkmenian;
                            break;
                        case FifamNation::Uzbekistan:
                            languageId = FifamLanguage::Uzbek;
                            break;
                        case FifamNation::Wales:
                            languageId = FifamLanguage::Welsh;
                            break;
                        case FifamNation::Scotland:
                            languageId = FifamLanguage::Scottish;
                            break;
                        case FifamNation::Ireland:
                        case FifamNation::Northern_Ireland:
                            languageId = FifamLanguage::Irish;
                            break;
                        }
                    }
                }

                if (languageId != FifamLanguage::None) {
                    if (p.mFemale) {
                        if (!p.mFirstName.empty() && p.mFirstName.find(L' ') == String::npos) {
                            femaleNames[languageId][p.mFirstName]++;
                            femaleNameUsage[p.mFirstName].insert(languageId);
                        }
                    }
                    else {
                        if (!p.mFirstName.empty() && p.mFirstName.find(L' ') == String::npos) {
                            maleNames[languageId][p.mFirstName]++;
                            maleNameUsage[p.mFirstName].insert(languageId);
                        }
                        if (!p.mSecondName.empty()) {
                            auto np = Utils::Split(p.mSecondName, L' ');
                            if (np.size() == 1 || (np.size() == 2 && np[0].length() <= 3)) {
                                surnames[languageId][p.mSecondName]++;
                                surnameUsage[p.mSecondName].insert(languageId);
                            }
                        }
                    }
                }
            }
        };

        ProcessPersons(mFoomDatabase->mPlayers);
        ProcessPersons(mFoomDatabase->mNonPlayers);
        ProcessPersons(mFoomDatabase->mOfficials);

        Map<FifamLanguage, Vector<Pair<String, UInt>>> finalMaleNames;
        Map<FifamLanguage, Vector<Pair<String, UInt>>> finalFemaleNames;
        Map<FifamLanguage, Vector<Pair<String, UInt>>> finalSurnames;

        auto GenerateFinalMap = [](Map<FifamLanguage, Map<String, UInt>> &names, Map<String, Set<FifamLanguage>> &usage, Map<FifamLanguage, Vector<Pair<String, UInt>>> &finalmap) {
            for (auto &[name, languages] : usage) {

            }
        };

        GenerateFinalMap(maleNames, maleNameUsage, finalMaleNames);
        GenerateFinalMap(femaleNames, femaleNameUsage, finalFemaleNames);
        GenerateFinalMap(surnames, surnameUsage, finalSurnames);
    }
    */
};
