#include "Converter.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifamCompCup.h"
#include "FifamNames.h"
#include "FifamUtils.h"
#include "FifamPlayerGenerator.h"
#include "FifamPlayerLevel.h"
#include "Random.h"
#include "ConverterUtil.h"
#include "GraphicsConverter.h"
#include "FifaConverter.h"
#include "Fifa07Converter.h"

#define DB_SIZE Full

Int GetIniInt(wchar_t const *key, Int defaultValue = 0) {
    return GetPrivateProfileIntW(L"MAIN", key, defaultValue, L".\\CONFIG.INI");
}

Path GetIniPath(wchar_t const *key, Path const &defaultValue = Path()) {
    wchar_t ResultPath[MAX_PATH];
    GetPrivateProfileStringW(L"MAIN", key, defaultValue.c_str(), ResultPath, MAX_PATH, L".\\CONFIG.INI");
    return ResultPath;
}

void Converter::Convert() {
    UInt originalGameId = GetIniInt(L"ORIGINAL_DB_VERSION", 13);
    Path originalDb = GetIniPath(L"ORIGINAL_DB_FOLDER", L"reference\\dbs\\fm13\\database");
    UInt referenceGameId = GetIniInt(L"REFERENCE_DB_VERSION", 14);
    Path referenceDb = GetIniPath(L"REFERENCE_DB_FOLDER", L"reference\\dbs\\fm14\\database");
    UInt gameId = GetIniInt(L"OUTPUT_GAME_VERSION", 13);
    mOutputGameFolder = GetIniPath(L"OUTPUT_GAME_FOLDER", L"E:\\Games\\FIFA Manager 13");
    mWriteToGameFolder = GetIniInt(L"WRITE_TO_OUTPUT_FOLDER", 1);
    Bool fromFifaDatabase = GetIniInt(L"FROM_FIFA_DB", 0);
    mQuickTest = GetIniInt(L"QUICK_TEST", 1);
    mGenerateLeaguesFiles = GetIniInt(L"GENERATE_LEAGUES_FILES", 0);
    mGenerateLeagueConfigFiles = GetIniInt(L"GENERATE_LEAGUE_CONFIG_FILES", 1);
    mGenerateSpecialScripts = GetIniInt(L"GENERATE_SPECIAL_SCRIPTS", 1);
    Bool EXPORT_FM_FIFA_PLAYERS = GetIniInt(L"EXPORT_FM_FIFA_PLAYERS", 0);
    Bool WRITE_DERBIES = GetIniInt(L"WRITE_DERBIES", 1);
    Bool INCREASE_CONTRACTS = GetIniInt(L"INCREASE_CONTRACTS", 0);
    mTestsOutputFolder = GetIniPath(L"TESTS_OUTPUT_FOLDER", L"tests");
    Path dbPath = GetIniPath(L"DB_FOLDER", L"db");
    Path graphicsPath = GetIniPath(L"FM_GRAPHICS_FOLDER", L"E:\\Documents\\Sports Interactive\\Football Manager 2020\\graphics");
    mContentFolder = GetIniPath(L"CONTENT_FOLDER", L"content");
    mContentArtsFolder = mContentFolder / Utils::Format(L"fm%02d", gameId);
    mFifaAssetsPath = GetIniPath(L"FIFA_GRAPHICS_FOLDER", L"E:\\Projects\\FIFA20");

    Bool CONVERT_CLUB_BADGES = GetIniInt(L"CONVERT_CLUB_BADGES", 0);
    Bool CONVERT_COMP_BADGES = GetIniInt(L"CONVERT_COMP_BADGES", 0);
    Bool CONVERT_COUNTRY_FLAGS = GetIniInt(L"CONVERT_COUNTRY_FLAGS", 0);
    Bool CONVERT_PORTRAITS = GetIniInt(L"CONVERT_PORTRAITS", 0);
    Bool CONVERT_TROPHIES = GetIniInt(L"CONVERT_TROPHIES", 0);
    Bool CONVERT_CITYPICS = GetIniInt(L"CONVERT_CITYPICS", 0);
    Bool CONVERT_STADPICS = GetIniInt(L"CONVERT_STADPICS", 0);
    Bool COPY_LEAGUE_SPLIT_AND_RELEGATION_BADGES = GetIniInt(L"COPY_LEAGUE_SPLIT_AND_RELEGATION_BADGES", 0);
    Bool GRAPHICS_WRITE_TO_GAME_FOLDER = GetIniInt(L"GRAPHICS_WRITE_TO_GAME_FOLDER", 0);
    Bool GRAPHICS_UPDATE_ONLY = GetIniInt(L"GRAPHICS_UPDATE_ONLY", 0);

    Bool REF_DB_FIFA_ID = GetIniInt(L"REF_DB_FIFA_ID", 1);
    Bool REF_DB_SEASON_TICKETS = GetIniInt(L"REF_DB_SEASON_TICKETS", 0);
    Bool REF_DB_AVERAGE_ATTENDANCE = GetIniInt(L"REF_DB_AVERAGE_ATTENDANCE", 0);
    Bool REF_DB_POTENTIAL_FANS_COUNT = GetIniInt(L"REF_DB_POTENTIAL_FANS_COUNT", 0);
    Bool REF_DB_CLUB_HISTORY_RECORDS = GetIniInt(L"REF_DB_CLUB_HISTORY_RECORDS", 1);
    Bool REF_DB_CLUB_HISTORY_WINS = GetIniInt(L"REF_DB_CLUB_HISTORY_WINS", 0);
    Bool REF_DB_CLUB_HISTORY_LEAGUE_STATS = GetIniInt(L"REF_DB_CLUB_HISTORY_LEAGUE_STATS", 0);
    Bool REF_DB_AI_STRATEGY = GetIniInt(L"REF_DB_AI_STRATEGY", 1);
    Bool REF_DB_LANDSCAPE = GetIniInt(L"REF_DB_LANDSCAPE", 1);
    Bool REF_DB_FOUNDATION_YEAR = GetIniInt(L"REF_DB_FOUNDATION_YEAR", 1);
    Bool REF_DB_SETTLEMENT = GetIniInt(L"REF_DB_SETTLEMENT", 1);
    Bool REF_DB_STADIUM_NAME = GetIniInt(L"REF_DB_STADIUM_NAME", 0);
    Bool REF_DB_STADIUM_CAPACITY = GetIniInt(L"REF_DB_STADIUM_CAPACITY", 0);
    Bool REF_DB_WC_EC_STATISTICS = GetIniInt(L"REF_DB_WC_EC_STATISTICS", 0);
    Bool WRITE_DUPLICATE_PLAYER_IDS = GetIniInt(L"WRITE_DUPLICATE_PLAYER_IDS", 0);

    UInt NUM_SPARE_CLUBS_IN_UNPLAYABLE_COUNTRY = GetIniInt(L"NUM_SPARE_CLUBS_IN_UNPLAYABLE_COUNTRY", 24);

    UInt MIN_FREE_AGENT_CA = GetIniInt(L"MIN_FREE_AGENT_CA", 110);
    UInt MIN_SPARE_STAFF_CA = GetIniInt(L"MIN_SPARE_STAFF_CA", 100);
    UInt SPARE_STAFF_FROM_CLUBS = GetIniInt(L"SPARE_STAFF_FROM_CLUBS", 0);
    UInt SPARE_STAFF_FROM_NTS = GetIniInt(L"SPARE_STAFF_FROM_NTS", 0);

    mWarnings = GetIniInt(L"HIDE_WARNINGS", 1) == 0;
    mErrors = GetIniInt(L"HIDE_ERRORS", 0) == 0;
    mLogErrors = GetIniInt(L"LOG_ERRORS", 0) != 0;
    mLogAppearance = GetIniInt(L"LOG_APPEARANCE", 0) != 0;
    mToFifa07Database = GetIniInt(L"TO_FIFA_07_DATABASE", 0);
    mWomen = GetIniInt(L"WOMEN", 0);

    std::error_code ec;
    if (exists(mLogPath, ec))
        remove(mLogPath, ec);
    
    Bool READ_FOOM_PERSONS = !mQuickTest;
    Bool MAKE_CORRECTIONS_FOR_UPDATE = !mQuickTest;
    Bool READ_REFERENCE_DB = true;
    Bool DB_REF_COLORS = false;
    
    mCurrentGameId = gameId;
    mFromFifaDatabase = fromFifaDatabase;
    Path infoPath = dbPath;

    GraphicsConverter graphicsConverter;
    //mFifaDatabase = new FifaDatabase(dbPath / L"fifa");
    //graphicsConverter.DownloadPlayerPortraitsFIFA21(mFifaDatabase, "D:\\FC24_portraits");
    //delete mFifaDatabase;
    //return;

    Bool GENERATE_APPEARANCE_DEFS = GetIniInt(L"GENERATE_APPEARANCE_DEFS", 0) != 0;
    if (GENERATE_APPEARANCE_DEFS) {
        appearanceGenerator.GenerateAppearanceDefs(dbPath, infoPath, mWomen);
        return;
    }

    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    if (mFromFifaDatabase)
        FifamDatabase::mReadingOptions.mGameVersionForScripts = gameId;
    if (!mToFifa07Database)
        mFifamDatabase = new FifamDatabase(originalGameId, originalDb);
    FifamDatabase::mReadingOptions.mGameVersionForScripts = 0;

    if (!mToFifa07Database && READ_REFERENCE_DB) {
        FifamDatabase::mReadingOptions.mReadClubs = true;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = !fromFifaDatabase;
        mReferenceDatabase = new FifamDatabase(referenceGameId, referenceDb);
    }

    if (false && MAKE_CORRECTIONS_FOR_UPDATE) {
        FifamDatabase::mReadingOptions.mReadClubs = true;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = true;

        mPreviousDb = new FifamDatabase(gameId, mOutputGameFolder / L"database text");
        
        for (FifamPlayer *p : mPreviousDb->mPlayers) {
            if (p->mFootballManagerID >= 0)
                mPreviousPlayers[p->mFootballManagerID] = p;
        }
    }

    if (false && DB_REF_COLORS) {
        FifamDatabase::mReadingOptions.mReadClubs = true;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = false;

        auto colorsDb = new FifamDatabase(gameId, Utils::Format(mOutputGameFolder / L"ref_db_colors", gameId));

        for (FifamClub *c : colorsDb->mClubs)
            mRefDbColors[c->mUniqueID] = c->mClubColour;

        delete colorsDb;
    }

    if (!mQuickTest || mFromFifaDatabase || mToFifa07Database)
        mFifaDatabase = new FifaDatabase(dbPath / L"fifa");
    mFoomDatabase = new foom::db(dbPath / L"foom", mWomen ? foom::db::db_gender::Women : foom::db::db_gender::Men,
        fromFifaDatabase? false : READ_FOOM_PERSONS, foom::db::db_size::DB_SIZE);

    if (mFromFifaDatabase)
        FifaConverter::ReadHistoryRatings(infoPath);

    Path faceIDsFilePath;
    if (!mQuickTest) {
        faceIDsFilePath = dbPath / L"FaceIDs.txt";
        if (!exists(faceIDsFilePath)) {
            faceIDsFilePath.clear();
            if (exists(mOutputGameFolder)) {
                faceIDsFilePath = mOutputGameFolder / L"fmdata" / L"eng" / L"FaceIDs.txt";
                if (!exists(faceIDsFilePath))
                    faceIDsFilePath.clear();
            }
        }
        if (!faceIDsFilePath.empty()) {
            FifamReader faceIdsReader(faceIDsFilePath);
            if (faceIdsReader.Available()) {
                UInt faceIdsCount = faceIdsReader.ReadLine<UInt>();
                for (UInt i = 0; i < faceIdsCount; i++) {
                    faceIdsReader.SkipLine();
                    mFaceIDs.insert(faceIdsReader.ReadLine<UInt>());
                }
            }
            faceIdsReader.Close();
        }
    }

    // fix club names
    for (auto &[clubId, club] : mFoomDatabase->mClubs) {
        if (Utils::EndsWith(club.mName, L" S.A.D."))
            club.mName = club.mName.substr(0, club.mName.length() - 7);
    }

    std::wcout << L"Creating reserve clubs..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        for (auto &r : c.mVecReserveTeamsToCreate) {
            if (r.mDivision) {
                UInt newClubId = (*(mFoomDatabase->mClubs.rbegin())).first + 1;
                foom::club newClub;
                newClub.mID = newClubId;
                newClub.mName = c.mName;
                newClub.mShortName = c.mShortName;
                newClub.mIsReserveDummyClub = true;
                newClub.mIsReserveToCreateClub = true;
                newClub.mNation = c.mNation;
                mFoomDatabase->mClubs[newClubId] = newClub;

                foom::club::reserve_team rt;
                rt.mReserveTeamType = r.mReserveTeamType;
                rt.mReserveClub = &mFoomDatabase->mClubs[newClubId];
                c.mVecReserveTeams.push_back(rt);

                r.mDivision->mVecTeams.push_back(&mFoomDatabase->mClubs[newClubId]);
            }
        }
    }

    ReadAdditionalInfo(infoPath, gameId);
    appearanceGenerator.Read(infoPath / L"AppearanceDefs.sav");

    if (!mFromFifaDatabase) {
        Path foomBadgesPath = graphicsPath / L"dvx-logos" / L"clubs" / L"primary" / L"@2x";
        if (exists(foomBadgesPath)) {
            std::wcout << L"Scanning badges folder..." << std::endl;
            for (auto const &i : recursive_directory_iterator(foomBadgesPath)) {
                Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
                if (id > 0 && i.path().extension() == ".png")
                    mAvailableBadges[id] = i.path();
            }
        }
    }

    std::wcout << L"Constructing cities lists..." << std::endl;
    for (auto &[key, city] : mFoomDatabase->mCities) {
        if (city.mNation)
            city.mNation->mConverterData.mCities.push_back(&city);
    }
    for (auto &[key, nation] : mFoomDatabase->mNations) {
        std::sort(nation.mConverterData.mCities.begin(), nation.mConverterData.mCities.end(),
            [](foom::city *a, foom::city *b) {
                return a->mInhabitants > b->mInhabitants;
        });
    }

    if (INCREASE_CONTRACTS) {
        for (auto &entry : mFoomDatabase->mPlayers) {
            auto &p = entry.second;
            if (p.mLoan.mClub) {
                if (p.mLoan.mEndDate < FifamDate(1, 7, CURRENT_YEAR))
                    p.mLoan.mEndDate = FifamDate(30, 6, CURRENT_YEAR + 1);
                //p.mLoan.mEndDate.year += 1;
                //if (p.mLoan.mStartDate.year < CURRENT_YEAR) {
                //    if (p.mLoan.mStartDate.year == (CURRENT_YEAR - 1)) {
                //        if (p.mLoan.mStartDate.month <= 7)
                //            p.mLoan.mStartDate.year += 1;
                //        else
                //            p.mLoan.mStartDate.Set(1, 7, CURRENT_YEAR);
                //    }
                //    else
                //        p.mLoan.mStartDate.year += 1;
                //}
            }
            //if (p.mContract.mClub) {
            //    p.mContract.mContractExpires.year += 1;
            //    p.mContract.mDateJoined.year += 1;
            //}
        }
    }

    std::wcout << L"Finding reserve clubs..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // for all affiliated clubs
        // 22 - Regional Academy
        // 24 - Extinct B or C Club
        for (auto &a : c.mVecAffiliations) {
            if (a.mAffiliatedClub && a.mIsMainClub) {
                if (a.mStartDate <= GetCurrentDate() && (a.mEndDate == FmEmptyDate() || a.mEndDate > GetCurrentDate())) {
                    foom::club *child = nullptr;
                    foom::club::converter_data::child_type childType;
                    if (a.mAffiliationType == 4 || a.mAffiliationType == 6 || a.mAffiliationType == 8) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second;
                    }
                    else if ((a.mAffiliationType == 1 || a.mAffiliationType == 3 || a.mAffiliationType == 21) && a.mPlayersMoveFreely && a.mSameBoard && c.mNation == a.mAffiliatedClub->mNation && c.mDivision != a.mAffiliatedClub->mDivision) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second_affiliated;
                    }
                    else if (a.mAffiliationType == 5 || a.mAffiliationType == 7) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::third;
                    }
                    else if (a.mAffiliationType == 18) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::shared_youth;
                    }
                    else if (a.mAffiliationType == 24) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::extinct_b_or_c;
                    }
                    if (child) {
                        if (child->mConverterData.mParentClub) {
                        #ifdef SHOW_RESERVE_CLUBS_WARNINGS
                            Alert(AL_ERROR, L"Affiliated B/C club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                                child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
                        #endif
                        }
                        else {
                            foom::club::converter_data::child_club childInfo;
                            childInfo.mClub = child;
                            childInfo.mIsAffiliated = true;
                            childInfo.mType = childType;
                            c.mConverterData.mChildClubs.push_back(childInfo);
                            child->mConverterData.mParentClub = &c;
                            child->mConverterData.mChildType = childType;
                        }
                    }
                }
            }
        }
        // for all reserve clubs
        for (auto &r : c.mVecReserveTeams) {
            if (r.mReserveClub) {
                foom::club *child = nullptr;
                foom::club::converter_data::child_type childType;
                if (r.mReserveTeamType == 1 || r.mReserveTeamType == 3 || r.mReserveTeamType == 14 || r.mReserveTeamType == 15 || r.mReserveTeamType == 16) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::second;
                }
                else if (r.mReserveTeamType == 13 || r.mReserveTeamType == 17) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::third;
                }
                else if (r.mReserveTeamType == 9) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u23;
                }
                else if (r.mReserveTeamType == 10 || r.mReserveTeamType == 30) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u21;
                }
                else if (r.mReserveTeamType == 18) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u20;
                }
                else if (r.mReserveTeamType == 11) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u19;
                }
                else if (r.mReserveTeamType == 12) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u18;
                }
                else if (r.mReserveTeamType == 21) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u21_b;
                }
                else if (r.mReserveTeamType == 36) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u19_b;
                }
                else if (r.mReserveTeamType == 37) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u18_b;
                }
                if (child) {
                    if (child->mConverterData.mParentClub) {
                    #ifdef SHOW_RESERVE_CLUBS_WARNINGS
                        Alert(AL_ERROR, L"Reserve club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                            child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
                    #endif
                    }
                    else {
                        foom::club::converter_data::child_club childInfo;
                        childInfo.mClub = child;
                        childInfo.mIsAffiliated = false;
                        childInfo.mType = childType;
                        c.mConverterData.mChildClubs.push_back(childInfo);
                        child->mConverterData.mParentClub = &c;
                        child->mConverterData.mChildType = childType;
                    }
                }
            }
        }
        // sort reserve clubs - by team types (affiliated clubs have bigger priority)
        if (c.mConverterData.mChildClubs.size() > 1) {
            std::sort(c.mConverterData.mChildClubs.begin(), c.mConverterData.mChildClubs.end(),
                [](foom::club::converter_data::child_club const &a, foom::club::converter_data::child_club const &b) {
                if (a.mType < b.mType) return true;
                if (b.mType < a.mType) return false;
                if (a.mIsAffiliated) return true;
                if (b.mIsAffiliated) return false;
                return false;
            });

            // DEBUG
            ///String message = Utils::Format(L"Club '%s' reserve teams:", c.mName.c_str());
            ///for (UInt i = 0; i < c.mConverterData.mChildClubs.size(); i++)
            ///    message += Utils::Format(L"\n%d. '%s'", i + 1, c.mConverterData.mChildClubs[i].mClub->mName.c_str());
            ///Message(message);
        }
    }
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // child club has child clubs
        if (c.mConverterData.mParentClub && !c.mConverterData.mChildClubs.empty()) {
        #ifdef SHOW_RESERVE_CLUBS_WARNINGS
            Alert(AL_ERROR, L"Reserve club has reserve teams\nClub: '%s'", c.mName.c_str());
        #endif
        }
    }

    std::wcout << L"Processing competitions history..." << std::endl;
    for (auto &entry : mFoomDatabase->mComps) {
        auto &c = entry.second;
        if (c.mNation) {
            if (c.mType == 0) // league
                c.mNation->mConverterData.mMainLeagues.push_back(&c);
            else if (c.mType == 2) // cup
                c.mNation->mConverterData.mMainCups.push_back(&c);
            else if (c.mType == 3) // league cup
                c.mNation->mConverterData.mLeagueCups.push_back(&c);
            else if (c.mType == 5) // supercup
                c.mNation->mConverterData.mSuperCups.push_back(&c);
        }
        // get nation champion
        for (auto &h : c.mVecHistory) {
            if (h.mYear > 0 && h.mYear <= CURRENT_YEAR && h.mFirstPlaced) {
                if (h.mFirstPlaced->mIsNation) {
                    foom::nation *nation = (foom::nation *)h.mFirstPlaced;
                    if (nation) {
                        if (nation->mConverterData.mHistoryNation)
                            nation = nation->mConverterData.mHistoryNation;
                        if (c.mID == 1301385) // World Cup
                            nation->mConverterData.mWorldCupWins.insert(h.mYear);
                        else if (nation->mContinent == c.mContinent && (c.mID == 1301388 || c.mID == 1301389 || c.mID == 1301390 || c.mID == 102414 || c.mID == 145509 || c.mID == 129986)) // Main continental international finals
                            nation->mConverterData.mContinentalCupWins.insert(h.mYear);
                    }
                    foom::nation *nationRunnerUp = (foom::nation *)h.mSecondPlaced;
                    if (nationRunnerUp) {
                        if (nationRunnerUp->mConverterData.mHistoryNation)
                            nationRunnerUp = nationRunnerUp->mConverterData.mHistoryNation;
                        if (c.mID == 1301385) // World Cup
                            nationRunnerUp->mConverterData.mWorldCupFinals.insert(h.mYear);
                        else if (nationRunnerUp->mContinent == c.mContinent && (c.mID == 1301388 || c.mID == 1301389 || c.mID == 1301390 || c.mID == 102414 || c.mID == 145509 || c.mID == 129986)) // Main continental international finals
                            nationRunnerUp->mConverterData.mContinentalCupFinals.insert(h.mYear);
                    }
                }
            }
        }
    }

    if (mToFifa07Database) {
        Fifa07Converter fifa07converter;
        fifa07converter.Convert(this, dbPath);
        return;
    }

    const UInt MAX_COMP_NAME_LENGTH = 29;

    // convert nations, national teams, leagues
    std::wcout << L"Converting nations, national teams and leagues..." << std::endl;
    ConvertLeagues(gameId);

    // create spare teams (non-league teams)
    std::wcout << L"Creating spare clubs..." << std::endl;
    Array<Vector<foom::club *>, FifamDatabase::NUM_COUNTRIES> spareClubs;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // team is active, has associated nation, not a reserve team
        if (!c.mExtinct && c.mNation && c.mNation->mConverterData.mSpareClubs.empty() && !c.mConverterData.mParentClub) {
            // fix - set reserve teams for first teams
            if (!c.mConverterData.mChildClubs.empty() && !c.mConverterData.mMainChildClubInDB)
                c.mConverterData.mMainChildClubInDB = c.mConverterData.mChildClubs[0].mClub;
            if (!c.mConverterData.mFifamClub) {
                // if proper country ID
                auto country = mFifamDatabase->GetCountry(c.mNation->mConverterData.mFIFAManagerID);
                if (country)
                    spareClubs[country->mId - 1].push_back(&c);
            }
        }
    }

    for (auto &country : mFifamDatabase->mCountries) {
        UInt maxClubs = gameId >= 10 ? 1000 : 500;
        if (country && country->mClubs.size() < maxClubs) {
            auto nation = country->GetProperty<foom::nation *>(L"foom::nation", nullptr);
            Bool hasPredefinedSpareClubs = nation && !nation->mConverterData.mSpareClubs.empty();
            if (hasPredefinedSpareClubs) {
                for (auto const &c : nation->mConverterData.mSpareClubs) {
                    // team is active, has associated nation, not a reserve team; TODO: review this
                    if (!c->mExtinct && !c->mConverterData.mParentClub) {
                        // fix - set reserve teams for first teams
                        if (!c->mConverterData.mChildClubs.empty() && !c->mConverterData.mMainChildClubInDB)
                            c->mConverterData.mMainChildClubInDB = c->mConverterData.mChildClubs[0].mClub;
                        if (!c->mConverterData.mFifamClub)
                            spareClubs[country->mId - 1].push_back(c);
                    }
                }
            }
            if (!spareClubs[country->mId - 1].empty()) { 
                UInt numSpareClubsToAdd = NUM_SPARE_CLUBS_IN_UNPLAYABLE_COUNTRY; // changed from 7
                if (!hasPredefinedSpareClubs) {
                    std::sort(spareClubs[country->mId - 1].begin(), spareClubs[country->mId - 1].end(), [](foom::club *a, foom::club *b) {
                        // TODO: make reputation over league level
                        UInt teamALeagueLevel = a->mDivision ? a->mDivision->mCompetitionLevel : 0;
                        UInt teamBLeagueLevel = b->mDivision ? b->mDivision->mCompetitionLevel : 0;
                        if (teamALeagueLevel == 0)
                            teamALeagueLevel = 9999;
                        if (teamBLeagueLevel == 0)
                            teamBLeagueLevel = 9999;
                        if (teamALeagueLevel < teamBLeagueLevel) return true;
                        if (teamBLeagueLevel < teamALeagueLevel) return false;
                        if (a->mReputation > b->mReputation) return true;
                        if (b->mReputation > a->mReputation) return false;
                        return false;
                    });

                    //if (gameId <= 7)
                    //    numSpareClubsToAdd = 4;

                    if (mNumTeamsInLeagueSystem[country->mId - 1] > 8) {
                        if (gameId > 7 || !mFromFifaDatabase)
                            numSpareClubsToAdd = mNumTeamsInLeagueSystem[country->mId - 1];
                        else {
                            numSpareClubsToAdd = 0;
                            if (!mLeaguesSystem[country->mId - 1].empty()) {
                                for (auto &ll : (*(mLeaguesSystem[country->mId - 1].rbegin())).second)
                                    numSpareClubsToAdd += ll->mNumTeams / 2;
                            }
                        }
                        if (numSpareClubsToAdd > 100)
                            numSpareClubsToAdd = 100;
                    }
                }
                else
                    numSpareClubsToAdd = maxClubs;
                if ((numSpareClubsToAdd + country->mClubs.size()) >= maxClubs)
                    numSpareClubsToAdd = maxClubs - country->mClubs.size();
                if (numSpareClubsToAdd > spareClubs[country->mId - 1].size())
                    numSpareClubsToAdd = spareClubs[country->mId - 1].size();
                if (mFromFifaDatabase) {
                    if (mNumTeamsInLeagueSystem[country->mId - 1] == 0 && numSpareClubsToAdd == 7)
                        numSpareClubsToAdd = 6;
                }
                for (UInt i = 0; i < numSpareClubsToAdd; i++) {
                    foom::club *team = spareClubs[country->mId - 1][i];
                    CreateAndConvertClub(gameId, team, team, country, nullptr, true);
                }
            }
        }
    }

    // setup club players
    std::wcout << L"Collecting club players..." << std::endl;

    UInt maxAllowedPlayers = 80;
    UInt maxAllowedPlayersWithYouth = 180;
    UInt minPlayersForFirstTeam = 23;
    UInt maxPlayersForFirstTeam = 27;
    UInt minPlayersForReserveTeam1 = 15;
    UInt minPlayersForReserveTeam2 = 20;
    if (gameId <= 7) {
        maxAllowedPlayers = 40;
        maxAllowedPlayersWithYouth = 40;
        minPlayersForFirstTeam = 23;
        maxPlayersForFirstTeam = 25;
        minPlayersForReserveTeam1 = 10;
        minPlayersForReserveTeam2 = 15;
    }
    else if (gameId <= 12) {
        maxAllowedPlayers = 75;
        maxAllowedPlayersWithYouth = 75;
        minPlayersForFirstTeam = 23;
        maxPlayersForFirstTeam = 26;
        minPlayersForReserveTeam1 = 15;
        minPlayersForReserveTeam2 = 20;
    }

    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &player = entry.second;
        auto contractClub = player.mContract.mClub;
        // player must have a contract
        if (contractClub) {
            auto loanClub = player.mLoan.mClub;
            if (loanClub) {
                if (player.mLoan.mEndDate <= GetCurrentDate() || loanClub == contractClub)
                    loanClub = nullptr;
                else if (loanClub == contractClub)
                    loanClub = nullptr;
            }
            if (contractClub->mConverterData.mFifamClub) { // if contracted club exists in the db
                if (loanClub) { // if player is loaned
                    if (loanClub->mConverterData.mFifamClub) { // if his current club exists in the db
                        loanClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub;
                    }
                    else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned club is reserve team
                        player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                        loanClub->mConverterData.mParentClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub;
                    }
                }
                else
                    contractClub->mConverterData.mContractedPlayers.push_back(&player);
            }
            else if (contractClub->mConverterData.mParentClub && contractClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // playing in reserve team
                if (loanClub) { // if player is loaned
                    if (loanClub->mConverterData.mFifamClub) { // if his current club exists in the db
                        loanClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mParentClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub->mConverterData.mParentClub;
                    }
                    else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned club is reserve team
                        player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                        loanClub->mConverterData.mParentClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mParentClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub->mConverterData.mParentClub;
                    }
                }
                else {
                    player.mConverterData.mTeamType = contractClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                    contractClub->mConverterData.mParentClub->mConverterData.mContractedPlayers.push_back(&player);
                }
            }
            else if (loanClub) { // we can create player if he's loaned and his current club exists in the db
                if (loanClub->mConverterData.mFifamClub) { // loaned to first team
                    loanClub->mConverterData.mContractedPlayers.push_back(&player);
                }
                else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned to reserve team
                    player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                    loanClub->mConverterData.mParentClub->mConverterData.mContractedPlayers.push_back(&player);
                }
            }
        }
        // check future transfer
        Date loanEndDate(1, 1, 1900);
        if (player.mConverterData.mLoanedFromClub)
            loanEndDate = player.mLoan.mEndDate;
        if (player.mFutureTransfer.mClub
            && (player.mFutureTransfer.mNewJob == 0 || player.mFutureTransfer.mNewJob == 11)
            && player.mFutureTransfer.mTransferDate > GetCurrentDate()
            && player.mFutureTransfer.mClub != player.mContract.mClub)
        {
            if (player.mFutureTransfer.mTransferDate <= loanEndDate) {
                //Alert(AL_ERROR, L"Player is loaned and transferred at same time\nPlayer: %s\nPlayerID: %d", player.mFullName.c_str(), player.mID);
            }
            else {
                if (player.mFutureTransfer.mClub->mConverterData.mFifamClub) {
                    player.mFutureTransfer.mClub->mConverterData.mFutureSigns.push_back(&player);
                    player.mConverterData.mFutureClub = player.mFutureTransfer.mClub;
                }
                else if (player.mFutureTransfer.mClub->mConverterData.mParentClub && player.mFutureTransfer.mClub->mConverterData.mParentClub->mConverterData.mFifamClub) {
                    player.mFutureTransfer.mClub->mConverterData.mParentClub->mConverterData.mFutureSigns.push_back(&player);
                    player.mConverterData.mFutureClub = player.mFutureTransfer.mClub->mConverterData.mParentClub;
                }
            }
        }
    }

    auto ProcessCompWinners = [=](Map<UShort, foom::team *> winnersMap, foom::nation * baseNation, UInt type) {
        if (!winnersMap.empty()) {
            for (auto we : winnersMap) {
                auto w = we.second;
                if (!w->mIsNation && (!baseNation || ((foom::club *)w)->mNation == baseNation)) {
                    if (type == 0)
                        ((foom::club *)w)->mConverterData.mLeagueWins.insert(we.first);
                    else if (type == 1)
                        ((foom::club *)w)->mConverterData.mCupWins.insert(we.first);
                    auto club = GetTeamClubLink(w);
                    if (club.IsValid()) {
                        if (type == 0)
                            club.mPtr->mHistory.mLeagueWinYears.push_back(we.first);
                        else if (type == 1)
                            club.mPtr->mHistory.mCupWinYears.push_back(we.first);
                        else if (type == 2)
                            club.mPtr->mHistory.mLeagueCupWinYears.push_back(we.first);
                        else if (type == 3)
                            club.mPtr->mHistory.mSuperCupsWinYears.push_back(we.first);
                        else if (type == 4)
                            club.mPtr->mHistory.mChampionsCupWinYears.push_back(we.first);
                        else if (type == 5)
                            club.mPtr->mHistory.mEuroTrophyWinYears.push_back(we.first);
                        else if (type == 7)
                            club.mPtr->mHistory.mWorldClubChampionshipWinYears.push_back(we.first);
                        else if (type == 8)
                            club.mPtr->mHistory.mWorldChampionshipWinYears.push_back(we.first);
                    }
                }
            }
        }
    };

    // finish club conversion
    std::wcout << L"Finishing clubs conversion, sorting players..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        // team is present in db
        if (team.mConverterData.mFifamClub) {
            FifamClub *dst = (FifamClub *)team.mConverterData.mFifamClub;
            FifamCountry *country = dst->mCountry;
            // friendship club
            foom::club *bestFriendshipClub = nullptr;
            if (team.mConverterData.mOriginalStoredParentClub && team.mConverterData.mOriginalStoredParentClub->mConverterData.mFifamClub)
                bestFriendshipClub = team.mConverterData.mOriginalStoredParentClub;
            else {
                Int bestFriendshipRep = 0;
                // for all affiliated clubs
                for (auto &a : team.mVecAffiliations) {
                    // 16 - Good relations
                    // 17 - Likely Friendly
                    if (a.mAffiliatedClub && a.mAffiliatedClub->mConverterData.mFifamClub && (a.mAffiliationType == 1 || a.mAffiliationType == 16)) {
                        FifamClub *affiliatedFifamClub = (FifamClub *)a.mAffiliatedClub->mConverterData.mFifamClub;
                        if (dst->mCountry->mId == affiliatedFifamClub->mCountry->mId) {
                            if (a.mStartDate <= GetCurrentDate() && (a.mEndDate == FmEmptyDate() || a.mEndDate > GetCurrentDate())) {
                                if (!bestFriendshipClub || a.mAffiliatedClub->mReputation > bestFriendshipRep) {
                                    bestFriendshipClub = a.mAffiliatedClub;
                                    bestFriendshipRep = a.mAffiliatedClub->mReputation;
                                }
                            }
                        }
                    }
                }
            }
            if (bestFriendshipClub)
                dst->mPartnershipClub.mPtr = (FifamClub *)bestFriendshipClub->mConverterData.mFifamClub;
            // rival clubs
            Vector<foom::club::rival_club *> rivalClubs;
            for (auto &r : team.mVecRivalClubs) {
                if (r.mRivalClub && r.mRivalClub->mConverterData.mFifamClub) {
                    FifamClub *rivalFifamClub = (FifamClub *)r.mRivalClub->mConverterData.mFifamClub;
                    if (dst->mCountry->mId == rivalFifamClub->mCountry->mId)
                        rivalClubs.push_back(&r);
                }
            }
            if (!rivalClubs.empty()) {
                if (rivalClubs.size() > 1) {
                    std::sort(rivalClubs.begin(), rivalClubs.end(), [](foom::club::rival_club *a, foom::club::rival_club *b) {
                        if (a->mLevel > b->mLevel) return true;
                        if (b->mLevel > a->mLevel) return false;
                        if (a->mRivalClub->mReputation > b->mRivalClub->mReputation) return true;
                        if (b->mRivalClub->mReputation > a->mRivalClub->mReputation) return false;
                        return false;
                    });
                }
                UInt numRivalClubs = Utils::Min(4u, rivalClubs.size());
                for (UInt i = 0; i < numRivalClubs; i++)
                    dst->mRivalClubs.push_back((FifamClub *)(rivalClubs[i]->mRivalClub->mConverterData.mFifamClub));
            }
            // lowest leagues
            Map<UInt, Vector<FifamCompLeague *>> availableLowestLeagues;
            FifamCompLeague *clubLeague = dst->GetProperty<FifamCompLeague *>(L"league", nullptr);
            if (clubLeague && clubLeague->mLeagueLevel > 0 && mLeaguesSystem[country->mId - 1][clubLeague->mLeagueLevel].size() > 1)
                availableLowestLeagues[clubLeague->mLeagueLevel].push_back(clubLeague);
            if (!team.mVecRegionalDivisions.empty()) {
                std::sort(team.mVecRegionalDivisions.begin(), team.mVecRegionalDivisions.end(),
                [](foom::club::regional_division const &a, foom::club::regional_division const &b) {
                    return a.mLevel < b.mLevel;
                });
                for (auto const &r : team.mVecRegionalDivisions) {
                    if (r.mDivision && r.mDivision->mConverterData.mLeague) {
                        FifamCompLeague *league = (FifamCompLeague *)r.mDivision->mConverterData.mLeague;
                        if (league->mID.mRegion.ToInt() == country->mId) {
                            if (league->mLeagueLevel > 0 && mLeaguesSystem[country->mId - 1][league->mLeagueLevel].size() > 1)
                                availableLowestLeagues[league->mLeagueLevel].push_back(league);
                        }
                    }
                }
            }
            for (auto &le : availableLowestLeagues) {
                if (dst->mLowestLeagues.size() >= 3)
                    break;
                if (le.second.size() == 1)
                    dst->mLowestLeagues.push_back(le.second[0]->mID);
                else {
                    for (FifamCompLeague *l : le.second) {
                        if (l && l == clubLeague) {
                            dst->mLowestLeagues.push_back(l->mID);
                            break;
                        }
                    }
                }
            }

            UInt numPlayersInCurrentSquad = team.mConverterData.mContractedPlayers.size()
                + team.mConverterData.mLoanedPlayers.size() + team.mConverterData.mFutureSigns.size();
            UInt numFuturePlayers = team.mConverterData.mFutureSigns.size();
            UInt possibleFutureSquadSize = numPlayersInCurrentSquad + team.mConverterData.mLoanedOutPlayers.size();

            if (possibleFutureSquadSize > maxAllowedPlayers) {
                // create possible future squad
                Vector<foom::player *> possibleFutureSquad;

                if (!team.mConverterData.mContractedPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mContractedPlayers.begin(), team.mConverterData.mContractedPlayers.end());
                if (!team.mConverterData.mLoanedOutPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mLoanedOutPlayers.begin(), team.mConverterData.mLoanedOutPlayers.end());
                if (!team.mConverterData.mLoanedPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mLoanedPlayers.begin(), team.mConverterData.mLoanedPlayers.end());
                if (!team.mConverterData.mFutureSigns.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mFutureSigns.begin(), team.mConverterData.mFutureSigns.end());

                // sort by CA/PA
                std::sort(possibleFutureSquad.begin(), possibleFutureSquad.end(), [](foom::player *a, foom::player *b) {
                    if (a->mCurrentAbility > b->mCurrentAbility) return true;
                    if (b->mCurrentAbility > a->mCurrentAbility) return false;
                    if (a->mPotentialAbility > b->mPotentialAbility) return true;
                    if (b->mPotentialAbility > a->mPotentialAbility) return false;
                    return false;
                });

                for (UInt i = maxAllowedPlayers; i < possibleFutureSquad.size(); i++)
                    possibleFutureSquad[i]->mConverterData.mFitsIntoDbLimit = false;
            }
        }
    }

    std::wcout << L"Calculating clubs national prestige..." << std::endl;
    for (FifamCountry *country : mFifamDatabase->mCountries) {
        if (country && !country->mClubs.empty()) {
            auto clubs = country->mClubs;
            std::sort(clubs.begin(), clubs.end(), [](FifamClub *a, FifamClub *b) {
                foom::club *foomClubA = a->GetProperty<foom::club *>(L"foom::club");
                foom::club *foomClubB = b->GetProperty<foom::club *>(L"foom::club");
                if (foomClubA->mReputation > foomClubB->mReputation) return true;
                if (foomClubB->mReputation > foomClubA->mReputation) return false;
                if (a->mHistory.mLeagueWinYears.size() > b->mHistory.mLeagueWinYears.size()) return true;
                if (b->mHistory.mLeagueWinYears.size() > a->mHistory.mLeagueWinYears.size()) return false;
                return false;
            });
            Int maxRep = clubs[0]->GetProperty<foom::club *>(L"foom::club")->mReputation;
            if (maxRep <= 0)
                maxRep = 1;
            for (UInt i = 0; i < clubs.size(); i++) {
                foom::club *foomClub = clubs[i]->GetProperty<foom::club *>(L"foom::club");
                Int rep = foomClub->mReputation;
                Int natRep = (Int)(9300.0f * ((Float)rep / (Float)maxRep));
                static Pair<Int, UChar> nationalRepTable[20] = {
                    { 9000, 20 },
                    { 8750, 19 },
                    { 8500, 18 },
                    { 8250, 17 },
                    { 8000, 16 },
                    { 7750, 15 },
                    { 7500, 14 },
                    { 7250, 13 },
                    { 7000, 12 },
                    { 6700, 11 },
                    { 6400, 10 },
                    { 6000,  9 },
                    { 5750,  8 },
                    { 5500,  7 },
                    { 5000,  6 },
                    { 4500,  5 },
                    { 4000,  4 },
                    { 3000,  3 },
                    { 2000,  2 },
                    { 1000,  1 }
                };
                Int natPrestige = 0;
                for (auto const &ip : nationalRepTable) {
                    if (natRep >= ip.first) {
                        natPrestige = ip.second;
                        break;
                    }
                }
                Int maxAllowedPrestigeForPlace = 13;
                if (i < 2)
                    maxAllowedPrestigeForPlace = 20;
                else if (i < 3)
                    maxAllowedPrestigeForPlace = 19;
                else if (i < 4)
                    maxAllowedPrestigeForPlace = 18;
                else if (i < 5)
                    maxAllowedPrestigeForPlace = 17;
                else if (i < 6)
                    maxAllowedPrestigeForPlace = 16;
                else if (i < 7)
                    maxAllowedPrestigeForPlace = 15;
                else if (i < 8)
                    maxAllowedPrestigeForPlace = 14;
                if (natPrestige > maxAllowedPrestigeForPlace)
                    natPrestige = maxAllowedPrestigeForPlace;

                if (!foomClub->mConverterData.mParentClub && foomClub->mConverterData.mOriginalStoredParentClub) {
                    if (natPrestige > 16)
                        natPrestige = 2;
                    else if (natPrestige > 11)
                        natPrestige = 1;
                    else
                        natPrestige = 0;
                }

                if (natPrestige > clubs[i]->mNationalPrestige)
                    clubs[i]->mNationalPrestige = natPrestige;
                if (clubs[i]->mNationalPrestige >= 16) {
                    if (clubs[i]->mInternationalPrestige > clubs[i]->mNationalPrestige)
                        clubs[i]->mInternationalPrestige = clubs[i]->mNationalPrestige;
                }
                else if (clubs[i]->mNationalPrestige >= 11 && clubs[i]->mNationalPrestige <= 15) {
                    if (clubs[i]->mInternationalPrestige > (clubs[i]->mNationalPrestige - 1))
                        clubs[i]->mInternationalPrestige = clubs[i]->mNationalPrestige - 1;
                }
                else if (clubs[i]->mNationalPrestige >= 6 && clubs[i]->mNationalPrestige <= 10) {
                    if (clubs[i]->mInternationalPrestige > (clubs[i]->mNationalPrestige - 2))
                        clubs[i]->mInternationalPrestige = clubs[i]->mNationalPrestige - 2;
                }
                else if (clubs[i]->mNationalPrestige >= 4 && clubs[i]->mNationalPrestige <= 5) {
                    if (clubs[i]->mInternationalPrestige > (clubs[i]->mNationalPrestige - 3))
                        clubs[i]->mInternationalPrestige = clubs[i]->mNationalPrestige - 3;
                }
                else if (clubs[i]->mNationalPrestige >= 2 && clubs[i]->mNationalPrestige <= 3) {
                    if (clubs[i]->mInternationalPrestige > 1)
                        clubs[i]->mInternationalPrestige = 1;
                }
                else
                    clubs[i]->mInternationalPrestige = 0;
            }
        }
    }

    std::wcout << L"Processing clubs domestic competitions..." << std::endl;
    for (FifamCountry *country : mFifamDatabase->mCountries) {
        if (country && !country->mClubs.empty()) {
            foom::nation *nation = country->GetProperty<foom::nation *>(L"foom::nation", nullptr);
            if (nation) {
                if (nation->mConverterData.mDomesticComps.league) {
                    
                    auto SetupLeagueWinnersList = [&](Map<UShort, foom::team *> & winners, foom::nation * n, foom::comp * league, Bool oneYearCalendar) {
                        if (league && !league->mVecHistory.empty()) {
                            Int lastPossibleYear = CURRENT_YEAR - oneYearCalendar;
                            Vector<foom::comp::history> h;
                            std::copy_if(league->mVecHistory.begin(), league->mVecHistory.end(), std::back_inserter(h), [&](foom::comp::history const &a) {
                                return a.mYear > 0 && a.mYear <= lastPossibleYear && a.mFirstPlaced;
                                });
                            if (!h.empty()) {
                                std::sort(h.begin(), h.end(), [](foom::comp::history const &a, foom::comp::history const &b) {
                                    if (a.mYear < b.mYear) return true;
                                    if (b.mYear < a.mYear) return false;
                                    if (a.mYearOrder > b.mYearOrder) return true;
                                    if (b.mYearOrder > a.mYearOrder) return false;
                                    return false;
                                    });
                                for (auto &e : h) {
                                    if (winners.find(e.mYear + oneYearCalendar) == winners.end())
                                        winners[e.mYear + oneYearCalendar] = e.mFirstPlaced;
                                }
                            }
                        }
                    };

                    Map<UShort, foom::team *> leagueWinners;
                    DivisionInfo *mainDivInfo = (DivisionInfo *)nation->mConverterData.mDomesticComps.league;
                    foom::comp *mainLeague = mFoomDatabase->get<foom::comp>(mainDivInfo->mID);
                    SetupLeagueWinnersList(leagueWinners, nation, mainLeague, mainDivInfo->mOneYearCalendar);

                    if (leagueWinners.find(CURRENT_YEAR) == leagueWinners.end() && nation->mConverterData.mDomesticComps.leagueWinner)
                        leagueWinners[CURRENT_YEAR] = nation->mConverterData.mDomesticComps.leagueWinner;

                    auto otherLeagues = nation->mConverterData.mMainLeagues;

                    if (otherLeagues.size() > 1) {
                        std::sort(otherLeagues.begin(), otherLeagues.end(), [](foom::comp * a, foom::comp * b) {
                            if (b->mIsExtinct && !a->mIsExtinct) return true;
                            if (a->mIsExtinct && !b->mIsExtinct) return false;
                            Int levelA = a->mCompetitionLevel;
                            if (levelA == 0)
                                levelA = 255;
                            Int levelB = b->mCompetitionLevel;
                            if (levelB == 0)
                                levelB = 255;
                            if (levelA < levelB) return true;
                            if (levelB < levelA) return false;
                            if (a->mReputation > b->mReputation) return true;
                            if (b->mReputation > a->mReputation) return false;
                            if (a->mName.find(L"Clausura") != String::npos && b->mName.find(L"Apertura") != String::npos) return true;
                            if (b->mName.find(L"Clausura") != String::npos && a->mName.find(L"Apertura") != String::npos) return false;
                            return false;
                        });
                        for (foom::comp *l : otherLeagues)
                            SetupLeagueWinnersList(leagueWinners, nation, l, mainDivInfo->mOneYearCalendar);
                    }

                    ProcessCompWinners(leagueWinners, nation, 0);

                    auto SetupCupWinners = [&](foom::nation *n, foom::comp *cup, Bool oneYearCalendar, UInt cupType) {
                        if (cup && !cup->mVecHistory.empty()) {
                            Int lastPossibleYear = CURRENT_YEAR;
                            if (cupType == 3)
                                lastPossibleYear -= 1;
                            else
                                lastPossibleYear -= oneYearCalendar;
                            Vector<foom::comp::history> h;
                            std::copy_if(cup->mVecHistory.begin(), cup->mVecHistory.end(), std::back_inserter(h), [&](foom::comp::history const &a) {
                                return a.mYear > 0 && a.mYear <= lastPossibleYear && a.mFirstPlaced;
                            });
                            if (!h.empty()) {
                                std::sort(h.begin(), h.end(), [](foom::comp::history const &a, foom::comp::history const &b) {
                                    if (a.mYear < b.mYear) return true;
                                    if (b.mYear < a.mYear) return false;
                                    if (a.mYearOrder < b.mYearOrder) return true;
                                    if (b.mYearOrder < a.mYearOrder) return false;
                                    return false;
                                });
                                foom::team *lastWinner = nullptr;
                                foom::team *lastRunnerUp = nullptr;
                                Bool hasLastSeason = false;
                                Map<UShort, foom::team *> winners;
                                for (auto &e : h) {
                                    if (cupType == 3)
                                        winners[e.mYear] = e.mFirstPlaced;
                                    else
                                        winners[e.mYear + oneYearCalendar] = e.mFirstPlaced;
                                    if (e.mYear == lastPossibleYear) {
                                        hasLastSeason = true;
                                        lastWinner = e.mFirstPlaced;
                                        lastRunnerUp = e.mSecondPlaced;
                                    }
                                }
                                if (cupType == 1) {
                                    if (n->mConverterData.mDomesticComps.cupWinner)
                                        lastWinner = n->mConverterData.mDomesticComps.cupWinner;
                                    if (n->mConverterData.mDomesticComps.cupRunnerUp)
                                        lastRunnerUp = n->mConverterData.mDomesticComps.cupRunnerUp;
                                    if (lastWinner) {
                                        if (!lastWinner->mIsNation && ((foom::club *)lastWinner)->mNation == n) {
                                            auto club = GetTeamClubLink(lastWinner);
                                            if (club.IsValid()) {
                                                if (club.mTeamType == FifamClubTeamType::First)
                                                    club.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
                                                else
                                                    club.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
                                            }
                                        }
                                    }
                                    if (lastRunnerUp) {
                                        if (!lastRunnerUp->mIsNation && ((foom::club *)lastRunnerUp)->mNation == n) {
                                            auto club = GetTeamClubLink(lastRunnerUp);
                                            if (club.IsValid()) {
                                                if (club.mTeamType == FifamClubTeamType::First)
                                                    club.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
                                                else
                                                    club.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
                                            }
                                        }
                                    }
                                    if (!hasLastSeason && n->mConverterData.mDomesticComps.cupWinner)
                                        winners[CURRENT_YEAR] = n->mConverterData.mDomesticComps.cupWinner;
                                }
                                ProcessCompWinners(winners, n, cupType);
                            }
                        }
                    };

                    if (nation->mConverterData.mDomesticComps.cup) {
                        CupInfo *info = (CupInfo *)nation->mConverterData.mDomesticComps.cup;
                        SetupCupWinners(nation, mFoomDatabase->get<foom::comp>(info->mID), info->mOneYearCalendar, 1);
                    }
                    if (nation->mConverterData.mDomesticComps.leagueCup) {
                        CupInfo *info = (CupInfo *)nation->mConverterData.mDomesticComps.leagueCup;
                        SetupCupWinners(nation, mFoomDatabase->get<foom::comp>(info->mID), info->mOneYearCalendar, 2);
                    }
                    if (nation->mConverterData.mDomesticComps.supercup) {
                        CupInfo *info = (CupInfo *)nation->mConverterData.mDomesticComps.supercup;
                        SetupCupWinners(nation, mFoomDatabase->get<foom::comp>(info->mID), info->mOneYearCalendar, 3);
                    }
                }
                else {
                    auto ProcessWinYearsForClub = [&](Vector<foom::comp *> const &comps, foom::nation * baseNation, UInt type) {
                        if (!comps.empty())
                            ProcessCompWinners(GetWinnersList(comps, type == 3 || type >= 6), baseNation, type);
                    };
                    if (nation->mConverterData.mFifamCountry) {
                        ProcessWinYearsForClub(nation->mConverterData.mMainLeagues, nation, 0);
                        // cup
                        {
                            ProcessWinYearsForClub(nation->mConverterData.mMainCups, nation, 1);
                        }
                        ProcessWinYearsForClub(nation->mConverterData.mLeagueCups, nation, 2);
                        ProcessWinYearsForClub(nation->mConverterData.mSuperCups, nation, 3);
                    }
                }
            }
        }
    }
    
    std::wcout << L"Creating players and staff..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        // team is present in db
        if (team.mConverterData.mFifamClub && team.mConverterData.mHasFifamSquad) {
            //std::wcout << L"Team: " << team.mID << std::endl;
            UInt howManyPlayersCanIAdditionalyCreate = 0;
            UInt howManyYouthPlayersCanIAdditionalyCreate = 0;
            UInt potentialSquadSize =
                std::count_if(team.mConverterData.mContractedPlayers.begin(), team.mConverterData.mContractedPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mLoanedPlayers.begin(), team.mConverterData.mLoanedPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mLoanedOutPlayers.begin(), team.mConverterData.mLoanedOutPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mFutureSigns.begin(), team.mConverterData.mFutureSigns.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            });
            if (potentialSquadSize < maxAllowedPlayers)
                howManyPlayersCanIAdditionalyCreate = maxAllowedPlayers - potentialSquadSize;
            if (potentialSquadSize < maxAllowedPlayersWithYouth)
                howManyYouthPlayersCanIAdditionalyCreate = maxAllowedPlayersWithYouth - potentialSquadSize;
            FifamClub *dst = (FifamClub *)team.mConverterData.mFifamClub;

            // create players
            for (auto &p : team.mConverterData.mContractedPlayers) {
                if (p->mConverterData.mFitsIntoDbLimit) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    if (p->mConverterData.mTeamType == 1) {
                        player->SetProperty(L"from_res", true);
                        if (p->mContract.mSquadNumber > 0)
                            player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                    }
                    else if (p->mConverterData.mTeamType == 2) {
                        if (player->GetAge(GetCurrentDate()) <= 18)
                            player->mInYouthTeam = true;
                        else {
                            player->SetProperty(L"from_res", true);
                            if (p->mContract.mSquadNumber > 0)
                                player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                        }
                    }
                    else {
                        if (!dst->mCaptains[0] && p == team.mCaptain) {
                            player->mIsCaptain = true;
                            dst->mCaptains[0] = player;
                        }
                        if (p->mContract.mSquadNumber > 0)
                            player->mShirtNumberFirstTeam = p->mContract.mSquadNumber;
                    }
                }
                else {
                    if (FifamUtils::GetAge(p->mDateOfBirth, GetCurrentDate()) <= 18) {
                        if (howManyYouthPlayersCanIAdditionalyCreate > 0) {
                            FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                            player->mInYouthTeam = true;
                            if (howManyPlayersCanIAdditionalyCreate > 0)
                                howManyPlayersCanIAdditionalyCreate--;
                            howManyYouthPlayersCanIAdditionalyCreate--;
                        }
                    }
                    else {
                        if (howManyPlayersCanIAdditionalyCreate > 0) {
                            FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                            if (p->mConverterData.mTeamType == 1 || p->mConverterData.mTeamType == 2) {
                                player->SetProperty(L"from_res", true);
                                if (p->mContract.mSquadNumber > 0)
                                    player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                            }
                            else {
                                if (!dst->mCaptains[0] && p == team.mCaptain) {
                                    player->mIsCaptain = true;
                                    dst->mCaptains[0] = player;
                                }
                                if (p->mContract.mSquadNumber > 0)
                                    player->mShirtNumberFirstTeam = p->mContract.mSquadNumber;
                            }
                            howManyPlayersCanIAdditionalyCreate--;
                            if (howManyYouthPlayersCanIAdditionalyCreate > 0)
                                howManyYouthPlayersCanIAdditionalyCreate--;
                        }
                    }
                }
            }
            for (auto &p : team.mConverterData.mLoanedPlayers) {
                if (p->mConverterData.mFitsIntoDbLimit && p->mConverterData.mTeamType != 2) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    if (p->mConverterData.mTeamType == 1) {
                        player->SetProperty(L"from_res", true);
                        if (p->mLoan.mSquadNumber > 0)
                            player->mShirtNumberReserveTeam = p->mLoan.mSquadNumber;
                    }
                    else {
                        if (!dst->mCaptains[0] && p == team.mCaptain) {
                            player->mIsCaptain = true;
                            dst->mCaptains[0] = player;
                        }
                        if (p->mLoan.mSquadNumber > 0)
                            player->mShirtNumberFirstTeam = p->mLoan.mSquadNumber;
                    }
                    Date loanStartDate = p->mLoan.mStartDate;
                    if (loanStartDate <= FmEmptyDate() || loanStartDate > GetCurrentDate())
                        loanStartDate = GetCurrentDate();
                    else
                        loanStartDate = GetDateAlignedToSeasonStart(loanStartDate, true, false);
                    Date loanEndDate = GetDateAlignedToSeasonEnd(p->mLoan.mEndDate, true, true);
                    Int loanFeeToBuy = -1;
                    if (p->mLoan.mFeeToBuy != -1 && !p->mConverterData.mFutureClub)
                        loanFeeToBuy = foom::db::convert_money(p->mLoan.mFeeToBuy);
                    player->mStartingConditions.mLoan.Setup(loanStartDate, loanEndDate,
                        FifamClubLink((FifamClub *)p->mConverterData.mLoanedFromClub->mConverterData.mFifamClub),
                        p->mLoan.mFeeToBuy == -1 ? -1 : foom::db::convert_money(p->mLoan.mFeeToBuy));
                    if (player->mContract.mValidUntil < loanEndDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(loanEndDate, false, true);;
                }
            }
            // TODO: check if this works
            for (auto &p : team.mConverterData.mFutureSigns) {
                if (p->mConverterData.mFitsIntoDbLimit && !p->mContract.mClub) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    player->mStartingConditions.mFutureJoin.Setup(GetDateAlignedToSeasonEnd(p->mFutureTransfer.mTransferDate, true, true));
                    if (p->mConverterData.mTeamType == 1)
                        player->SetProperty(L"from_res", true);
                    else if (p->mConverterData.mTeamType == 2)
                        player->mInYouthTeam = true;
                    if (player->mContract.mValidUntil <= player->mStartingConditions.mFutureJoin.mDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(player->mStartingConditions.mFutureJoin.mDate, false, true);
                }
            }

            if (MAKE_CORRECTIONS_FOR_UPDATE) {
                for (FifamPlayer *a : dst->mPlayers) {
                    bool levelSet = false;
                    foom::player *foomPlayer = a->GetProperty<foom::player *>(L"foom::player", nullptr);
                    if (foomPlayer) {
                        FifamPlayer *prevPlayer = GetPreviousPlayer(foomPlayer->mID);
                        if (prevPlayer) {
                            a->SetProperty<Int>(L"fifam_level", GetPlayerLevel(prevPlayer, true, gameId));
                            levelSet = true;
                        }
                    }
                    if (!levelSet)
                        a->SetProperty<Int>(L"fifam_level", GetPlayerLevel(a, true, gameId));
                }
            }

            std::sort(dst->mPlayers.begin(), dst->mPlayers.end(), [=](FifamPlayer *a, FifamPlayer *b) {
                auto levelA = 0;
                auto levelB = 0;
                if (MAKE_CORRECTIONS_FOR_UPDATE) {
                    levelA = a->GetProperty<Int>(L"fifam_level", 0);
                    levelB = b->GetProperty<Int>(L"fifam_level", 0);
                }
                else {
                    levelA = GetPlayerLevel(a, true, gameId);
                    levelB = GetPlayerLevel(b, true, gameId);
                }
                if (levelA > levelB) return true;
                if (levelB > levelA) return false;
                if (a->mTalent > b->mTalent) return true;
                if (b->mTalent > a->mTalent) return false;
                return false;
            });

            if (dst->mPlayers.size() > minPlayersForFirstTeam) {

                UInt numYouthPlayers = std::count_if(dst->mPlayers.begin(), dst->mPlayers.end(), [](FifamPlayer *a) {
                    return a->mInYouthTeam;
                });
                UInt numReservePlayers = 0;
                UInt numFirstTeamPlayers = 0;

                UChar minFirstTeamRating = GetPlayerLevel(dst->mPlayers[minPlayersForFirstTeam], true, gameId);
                if (minFirstTeamRating > 0)
                    minFirstTeamRating -= 1;

                for (FifamPlayer *a : dst->mPlayers) {
                    Bool fromReserve = a->GetProperty<Bool>(L"from_res", false);
                    if (!a->mInReserveTeam && !a->mInYouthTeam) {
                        if (!fromReserve && numFirstTeamPlayers < minPlayersForFirstTeam)
                            numFirstTeamPlayers++;
                        else if (!fromReserve && numFirstTeamPlayers < maxPlayersForFirstTeam && GetPlayerLevel(a, true, gameId) >= minFirstTeamRating)
                            numFirstTeamPlayers++;
                        else if (!fromReserve && GetPlayerLevel(a, true, gameId) >= 70)
                            numFirstTeamPlayers++;
                        else {
                            if (numReservePlayers > minPlayersForReserveTeam2) {
                                if (a->GetAge(GetCurrentDate()) <= 18)
                                    a->mInYouthTeam = true;
                                else {
                                    a->mInReserveTeam = true;
                                    numReservePlayers++;
                                }
                            }
                            else if (numReservePlayers > minPlayersForReserveTeam1) {
                                if (a->GetAge(GetCurrentDate()) <= 17)
                                    a->mInYouthTeam = true;
                                else {
                                    a->mInReserveTeam = true;
                                    numReservePlayers++;
                                }
                            }
                            else {
                                a->mInReserveTeam = true;
                                numReservePlayers++;
                            }
                        }
                    }
                }
            }

            Vector<Bool> FirstTeamNumbersUsed(100, false);
            Vector<Bool> ReserveTeamNumbersUsed(100, false);

            auto GetNextAvailableNumber = [](Vector<UChar> const &priorityNumbers, Vector<Bool> &numbers) -> UChar {
                for (UChar i : priorityNumbers) {
                    if (!numbers[i]) {
                        numbers[i] = true;
                        return i;
                    }
                }
                for (UChar i = 12; i <= 99; i++) {
                    if (!numbers[i]) {
                        numbers[i] = true;
                        return i;
                    }
                }
                return 0;
            };

            auto GetNumberForPlayer = [&](FifamPlayer *a, UChar currentNumber, Vector<Bool> &numbers) {
                if (currentNumber && !numbers[currentNumber]) {
                    numbers[currentNumber] = true;
                    return currentNumber;
                }
                UChar pos = a->mMainPosition.ToInt();
                if (pos == FifamPlayerPosition::GK)
                    return GetNextAvailableNumber({ 1, 12, 25, 23, 13, 99 }, numbers);
                else if (pos == FifamPlayerPosition::RB || pos == FifamPlayerPosition::RWB)
                    return GetNextAvailableNumber({ 2, 4, 3, 6, 5 }, numbers);
                else if (pos == FifamPlayerPosition::LB || pos == FifamPlayerPosition::LWB)
                    return GetNextAvailableNumber({ 3, 5, 2, 6, 4 }, numbers);
                else if (pos == FifamPlayerPosition::CB || pos == FifamPlayerPosition::SW)
                    return GetNextAvailableNumber({ 3, 4, 5, 2, 6 }, numbers);
                else if (pos == FifamPlayerPosition::ANC || pos == FifamPlayerPosition::DM)
                    return GetNextAvailableNumber({ 4, 6, 5, 7 }, numbers);
                else if (pos == FifamPlayerPosition::CM)
                    return GetNextAvailableNumber({ 6, 8, 7, 5 }, numbers);
                else if (pos == FifamPlayerPosition::RM || pos == FifamPlayerPosition::LM)
                    return GetNextAvailableNumber({ 7, 11, 10, 8, 6, 17 }, numbers);
                else if (pos == FifamPlayerPosition::AM)
                    return GetNextAvailableNumber({ 10, 7, 11, 8, 6, 20 }, numbers);
                else if (pos == FifamPlayerPosition::RW || pos == FifamPlayerPosition::LW)
                    return GetNextAvailableNumber({ 7, 10, 11, 9, 8, 17 }, numbers);
                else if (pos == FifamPlayerPosition::ST || pos == FifamPlayerPosition::CF)
                    return GetNextAvailableNumber({ 9, 10, 7, 11, 8, 19 }, numbers);
                return GetNextAvailableNumber({}, numbers);
            };

            for (FifamPlayer *a : dst->mPlayers) {
                if (a->mShirtNumberFirstTeam < 0 || a->mShirtNumberFirstTeam > 99)
                    a->mShirtNumberFirstTeam = 0;
                if (a->mShirtNumberReserveTeam < 0 || a->mShirtNumberReserveTeam > 99)
                    a->mShirtNumberReserveTeam = 0;
                if (a->mInYouthTeam) {
                    a->mShirtNumberFirstTeam = 0;
                    a->mShirtNumberReserveTeam = 0;
                }
                else if (a->mInReserveTeam) {
                    if (!a->mShirtNumberReserveTeam && a->mShirtNumberFirstTeam)
                        a->mShirtNumberReserveTeam = a->mShirtNumberFirstTeam;
                    a->mShirtNumberReserveTeam = GetNumberForPlayer(a, a->mShirtNumberReserveTeam, ReserveTeamNumbersUsed);
                    if (a->mShirtNumberFirstTeam) {
                        if (!FirstTeamNumbersUsed[a->mShirtNumberFirstTeam])
                            FirstTeamNumbersUsed[a->mShirtNumberFirstTeam] = true;
                        else
                            a->mShirtNumberFirstTeam = 0;
                    }
                }
                else
                    a->mShirtNumberFirstTeam = GetNumberForPlayer(a, a->mShirtNumberFirstTeam, FirstTeamNumbersUsed);
                // Update (FM 22): fill empty history
                if (a->mHistory.mEntries.empty()) {
                    if (a->mStartingConditions.mLoan.Enabled()) {
                        a->mHistory.mEntries.resize(2);
                        auto &h1 = a->mHistory.mEntries[0];
                        auto &h2 = a->mHistory.mEntries[1];
                        h1.mClub = a->mStartingConditions.mLoan.mLoanedClub;
                        h2.mClub = FifamClubLink(dst);
                        h2.mLoan = true;
                        if (a->mStartingConditions.mLoan.mStartDate > FifamDate(1, 7, CURRENT_YEAR))
                            h2.mStartDate = FifamDate(1, 7, CURRENT_YEAR);
                        else
                            h2.mStartDate = a->mStartingConditions.mLoan.mStartDate;
                        h2.mEndDate = FifamDate(30, 6, CURRENT_YEAR + 1);
                        h2.mStillInThisClub = true;
                        h1.mEndDate.SetDays(h2.mStartDate.GetDays() - 1);
                        if (a->mContract.mJoined < h1.mEndDate)
                            h1.mStartDate = a->mContract.mJoined;
                        else
                            h1.mStartDate = h1.mEndDate.DecreasedByOneYear();
                    }
                    else {
                        auto &h = a->mHistory.mEntries.emplace_back();
                        h.mClub = FifamClubLink(dst);
                        h.mStartDate = a->mContract.mJoined;
                        h.mEndDate = FifamDate(30, 6, CURRENT_YEAR + 1);
                        h.mStillInThisClub = true;
                    }
                }
            }
            CreateStaffMembersForClub(gameId, &team, dst, false);
        }
    }

    std::wcout << L"Processing buy-back clauses..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        for (auto &b : team.mVecBuyBackClauses) {
            if (b.mBuyBackFee > 0 && b.mPlayer && b.mPlayer->mConverterData.mFifamPerson && b.mFromClub && b.mFromClub->mConverterData.mFifamClub) {
                FifamPlayer *player = (FifamPlayer *)b.mPlayer->mConverterData.mFifamPerson;
                if (!player->mContract.mBuyBackClauseEnabled) {
                    player->mContract.mBuyBackClauseEnabled = true;
                    player->mContract.mBuyBackClauseClub = (FifamClub *)b.mFromClub->mConverterData.mFifamClub;
                    player->mContract.mBuyBackClauseCost = foom::db::convert_money(b.mBuyBackFee);
                    if (player->mContract.mValidUntil.year > GetCurrentDate().year + 1)
                        player->mContract.mBuyBackClauseValidUntil = player->mContract.mValidUntil.DecreasedByOneYear();
                    else
                        player->mContract.mBuyBackClauseValidUntil = player->mContract.mValidUntil;
                }
            }
        }
    }

    // create free agents
    std::wcout << L"Creating free agents..." << std::endl;

    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFitsIntoDbLimit) {
            if (!p.mConverterData.mFifamPerson) {
                if (!p.mContract.mClub && (p.mCurrentAbility >= (Int)MIN_FREE_AGENT_CA || (p.mNation && p.mNation->mConverterData.mFIFAManagerID == FifamCompRegion::Liechtenstein)) || Utils::Contains(mFreeAgentsToAdd, (UInt)p.mID)) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, &p, nullptr);
                }
            }
            else {
                if (p.mConverterData.mFutureClub && p.mConverterData.mFutureClub != p.mContract.mClub) {
                    FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
                    FifamDate transferDate = GetDateAlignedToSeasonStart(p.mFutureTransfer.mTransferDate, true, true);
                    FifamDate contractEndDate = GetDateAlignedToSeasonEnd(p.mFutureTransfer.mContractEndDate, true, true);
                    if (contractEndDate < transferDate)
                        contractEndDate = GetDateAlignedToSeasonEnd(transferDate, false, true);
                    else
                        contractEndDate = GetDateAlignedToSeasonEnd(contractEndDate, false, true);
                    player->mStartingConditions.mFutureTransfer.Setup(transferDate, contractEndDate,
                        FifamClubLink((FifamClub *)p.mConverterData.mFutureClub->mConverterData.mFifamClub),
                        p.mFutureTransfer.mTransferFee > 0 ? foom::db::convert_money(p.mFutureTransfer.mTransferFee) : 0);
                    if (player->mContract.mValidUntil <= transferDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(transferDate, false, true);
                }
            }
        }
    }

    // create spare staff
    std::wcout << L"Creating spare managers/staff..." << std::endl;
    for (auto &entry : mFoomDatabase->mNonPlayers) {
        auto &p = entry.second;
        Bool canWorkWithGender = false;
        if (mWomen)
            canWorkWithGender = p.mSideOfGameCurrent == 1 || p.mSideOfGameWilling == 2 || p.mSideOfGameWilling == 3;
        else
            canWorkWithGender = p.mSideOfGameCurrent != 1 && p.mSideOfGameWilling != 2;
        if (canWorkWithGender && IsConvertable(&p, gameId) && !p.mConverterData.mFifamPerson) {
            if ((!p.mClubContract.mClub || SPARE_STAFF_FROM_CLUBS) && (!p.mNationContract.mNation || SPARE_STAFF_FROM_NTS) && p.mCurrentAbility >= (Int)MIN_SPARE_STAFF_CA) {
                auto bestPosition = Utils::GetMaxElementId<Int, FifamClubStaffPosition>({
                    { p.mJobManager, FifamClubStaffPosition::Manager },
                    { p.mJobAssistantManager, FifamClubStaffPosition::AssistantCoach },
                    { p.mJobCoach, FifamClubStaffPosition::AssistantCoach },
                    { p.mJobScout, FifamClubStaffPosition::GeneralScout },
                    { p.mJobDirectorOfFootball, FifamClubStaffPosition::SportsDirector },
                    { p.mJobGkCoach, FifamClubStaffPosition::GoalkeeperCoach },
                    { p.mJobFitnessCoach, FifamClubStaffPosition::FitnessCoach },
                    { p.mJobSportsScientist, FifamClubStaffPosition::TeamDoctor },
                    { p.mJobPhysio, FifamClubStaffPosition::Masseur }
                });
                if (bestPosition.first >= 15 && bestPosition.first >= p.mJobDataAnalyst && bestPosition.first >= p.mJobChairman && bestPosition.first >= p.mJobHeadOfYouth)
                    FifamStaff *staff = CreateAndConvertStaff(&p, nullptr, bestPosition.second, gameId);
            }
        }
    }
    
    auto ProcessPersonFavouriteAndDislikedClubs = [](foom::person *p, FifamPerson *person) {
        if (!p->mVecFavouriteClubs.empty()) {
            Int maxRep = 0;
            FifamClub *bestFavouriteClub = nullptr;
            for (auto &f : p->mVecFavouriteClubs) {
                if (f.mClub && f.mLevel == 100 && f.mClub->mConverterData.mFifamClub) {
                    if (!bestFavouriteClub || f.mClub->mReputation > maxRep) {
                        bestFavouriteClub = (FifamClub *)f.mClub->mConverterData.mFifamClub;
                        maxRep = f.mClub->mReputation;
                    }
                }
            }
            if (bestFavouriteClub)
                person->mFavouriteClub = bestFavouriteClub;
        }
        if (!p->mVecDislikedClubs.empty()) {
            Int maxRep = 0;
            FifamClub *bestDislikedClub = nullptr;
            for (auto &d : p->mVecDislikedClubs) {
                if (d.mClub && d.mLevel == 100 && d.mClub->mConverterData.mFifamClub) {
                    if (!bestDislikedClub || d.mClub->mReputation > maxRep) {
                        bestDislikedClub = (FifamClub *)d.mClub->mConverterData.mFifamClub;
                        maxRep = d.mClub->mReputation;
                    }
                }
            }
            if (bestDislikedClub)
                person->mWouldnSignFor = bestDislikedClub;
        }
    };

    // player relations
    std::wcout << L"Setting up player relations..." << std::endl;
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson) {
            FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
            if (!p.mVecFavouritePeople.empty()) {
                for (auto f : p.mVecFavouritePeople) {
                    if (f.mPerson && f.mPerson->mIsPlayer && f.mPerson->mConverterData.mFifamPerson) {
                        if (f.mReason == 2) { // Brother
                            FifamPlayer *brother = (FifamPlayer *)f.mPerson->mConverterData.mFifamPerson;
                            player->mBrothers.insert(brother);
                            brother->mBrothers.insert(player);

                        }
                        else if (f.mReason == 4) { // Relation
                            FifamPlayer *cousin = (FifamPlayer *)f.mPerson->mConverterData.mFifamPerson;
                            player->mCousins.insert(cousin);
                            cousin->mCousins.insert(player);
                        }
                    }
                }
            }
            ProcessPersonFavouriteAndDislikedClubs(&p, player);
        }
    }
    for (auto &entry : mFoomDatabase->mNonPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson)
            ProcessPersonFavouriteAndDislikedClubs(&p, (FifamStaff *)p.mConverterData.mFifamPerson);
    }

    // process player history and experience
    //std::wcout << L"Processing player history and experience..." << std::endl;
    //for (auto &entry : mFoomDatabase->mPlayers) {
    //    auto &p = entry.second;
    //    if (p.mConverterData.mFifamPerson) {
    //        FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
    //        
    //    }
    //}

    // add players to national teams
    std::wcout << L"Adding players to national teams..." << std::endl;
    for (auto &n : mNationalTeamPlayers) {
        if (!n.empty()) {
            if (n.size() > 1) {
                std::sort(n.begin(), n.end(), [](foom::player *a, foom::player *b) {
                    if (a->mCurrentAbility > b->mCurrentAbility) return true;
                    if (b->mCurrentAbility > a->mCurrentAbility) return false;
                    if (a->mInternationalApps > b->mInternationalApps) return true;
                    if (b->mInternationalApps > a->mInternationalApps) return false;
                    return false;
                });
            }
            UInt numPlayersInNationalTeam = 0;
            UInt numGoalkeepersInNationalTeam = 0;
            for (UInt i = 0; i < n.size(); i++) {
                Bool isGk = ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mMainPosition == FifamPlayerPosition::GK;
                if (isGk) {
                    if (numGoalkeepersInNationalTeam < 3) {
                        ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mCurrentlyInNationalTeam = true;
                        numGoalkeepersInNationalTeam++;
                    }
                }
                else {
                    if (numPlayersInNationalTeam < 22) {
                        ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mCurrentlyInNationalTeam = true;
                        numPlayersInNationalTeam++;
                    }
                }
                if (numGoalkeepersInNationalTeam == 3 && numPlayersInNationalTeam == 22)
                    break;
            }
        }
    }

    // convert referees
    std::wcout << L"Converting referees..." << std::endl;
    Vector<foom::official *> refsSorted;
    for (auto &entry : mFoomDatabase->mOfficials)
        refsSorted.push_back(&entry.second);
    std::sort(refsSorted.begin(), refsSorted.end(), [](foom::official *a, foom::official *b) {
        return a->mCurrentAbility > b->mCurrentAbility;
    });
    for (auto official : refsSorted) {
        if (IsConvertable(official, gameId) && official->mNation) {
            auto country = mFifamDatabase->GetCountry(official->mNation->mConverterData.mFIFAManagerReplacementID);
            if (country && country->mReferees.size() < 256) {
                FifamReferee *referee = country->AddReferee();
                ConvertReferee(referee, official, gameId);
            }
        }
    }

    // FIFA database conversion
    if (mFromFifaDatabase) {
        // referee
        FifaConverter::ConvertReferees(this, mFifamDatabase, mFifaDatabase, gameId);
        Set<FifaPlayer *> attachedPlayers;
        for (auto club : mFifamDatabase->mClubs) {
            if (club->mFifaID != 0) {
                FifaTeam *fifaTeam = mFifaDatabase->GetTeam(club->mFifaID);
                if (fifaTeam && fifaTeam->m_gameId == FifaDatabase::m_lastSupportedGameVersion) {
                    if (fifaTeam->m_manager)
                        FifaConverter::ConvertManager(this, mFifamDatabase, club, fifaTeam->m_manager, gameId);
                    fifaTeam->ForAllPlayersEx([&](FifaPlayer &p, FifaPlayer::Position pos, UChar number) {
                        FifaConverter::ConvertPlayer(this, club, false, fifaTeam, &p, pos, number, gameId);
                        attachedPlayers.insert(&p);
                    });
                }
                if (club->mFifaID == 21) {
                    FifaTeam *resTeam = mFifaDatabase->GetTeam(110679);
                    if (resTeam && resTeam->m_gameId == FifaDatabase::m_lastSupportedGameVersion) {
                        resTeam->ForAllPlayersEx([&](FifaPlayer &p, FifaPlayer::Position pos, UChar number) {
                            FifaConverter::ConvertPlayer(this, club, true, fifaTeam, &p, pos, number, gameId);
                        });
                    }
                }
            }
        }
        mFifaDatabase->ForAllMalePlayers([&](FifaPlayer &p) {
            if (p.m_gameId == FifaDatabase::m_lastSupportedGameVersion && !Utils::Contains(attachedPlayers, &p)) {
                FifaConverter::ConvertPlayer(this, nullptr, false, nullptr, &p, FifaPlayer::POS_RES, 0, gameId);
            }
        });
    }

    // reference database
    if (mReferenceDatabase) {
        // create reference map
        Map<UInt, FifamClub *> referenceClubsByID;
        for (FifamClub *c : mReferenceDatabase->mClubs) {
            if (!REF_DB_FIFA_ID || c->mFifaID)
                referenceClubsByID[REF_DB_FIFA_ID ? c->mFifaID : c->mUniqueID] = c;
        }
        // apply parameters
        for (FifamClub *c : mFifamDatabase->mClubs) {
            if (!REF_DB_FIFA_ID || c->mFifaID) {
                auto it = referenceClubsByID.find(REF_DB_FIFA_ID ? c->mFifaID : c->mUniqueID);
                if (it != referenceClubsByID.end()) {
                    FifamClub *ref = (*it).second;
                    c->mMediaPressure = ref->mMediaPressure;
                    c->mAddress = ref->mAddress;
                    c->mTelephone = ref->mTelephone;
                    c->mWebsiteAndMail = ref->mWebsiteAndMail;
                    c->mFansites = ref->mFansites;
                    c->mFanMembers = ref->mFanMembers;
                    if (c->mTransfersCountry[0] == FifamNation::None)
                        c->mTransfersCountry = ref->mTransfersCountry;
                    else if (c->mTransfersCountry[1] == FifamNation::None) {
                        if (ref->mTransfersCountry[0] != c->mTransfersCountry[0])
                            c->mTransfersCountry[1] = ref->mTransfersCountry[0];
                        else
                            c->mTransfersCountry[1] = ref->mTransfersCountry[1];
                    }
                    c->mMascotName = ref->mMascotName;
                    c->mPlayerInText = ref->mPlayerInText;
                    c->mFanName1 = ref->mFanName1;
                    c->mFanName2 = ref->mFanName2;
                    c->mTermForTeam1 = ref->mTermForTeam1;
                    c->mTermForTeam2 = ref->mTermForTeam2;
                    c->mAbbreviationArticle = ref->mAbbreviationArticle;
                    c->mPlayerInTextArticle = ref->mPlayerInTextArticle;
                    c->mFanName1Article = ref->mFanName1Article;
                    c->mClubNameUsageInPhrase = ref->mClubNameUsageInPhrase;
                    c->mClubNameUsageInPhrase2 = ref->mClubNameUsageInPhrase2;
                    c->mFanName2Article = ref->mFanName2Article;
                    c->mTermForTeam1Article =  ref->mTermForTeam1Article;
                    c->mTermForTeam2Article =  ref->mTermForTeam2Article;

                    if (REF_DB_AI_STRATEGY)
                        c->mAiStrategy = ref->mAiStrategy;
                    if (REF_DB_LANDSCAPE)
                        c->mLandscape = ref->mLandscape;
                    if (REF_DB_SETTLEMENT)
                        c->mSettlement = ref->mSettlement;

                    if (REF_DB_FOUNDATION_YEAR)
                        c->mYearOfFoundation = ref->mYearOfFoundation;

                    if (REF_DB_SEASON_TICKETS)
                        c->mCountOfSoldSeasonTickets = ref->mCountOfSoldSeasonTickets;
                    if (REF_DB_AVERAGE_ATTENDANCE)
                        c->mAverageAttendanceLastSeason = ref->mAverageAttendanceLastSeason;
                    if (REF_DB_POTENTIAL_FANS_COUNT)
                        c->mPotentialFansCount = ref->mPotentialFansCount;

                    if (REF_DB_STADIUM_NAME)
                        c->mStadiumName = ref->mStadiumName;
                    if (REF_DB_STADIUM_CAPACITY) {
                        c->mStadiumSeatsCapacity = ref->mStadiumSeatsCapacity;
                        c->mStadiumStandingsCapacity = ref->mStadiumStandingsCapacity;
                        c->mStadiumVipCapacity = ref->mStadiumVipCapacity;
                    }

                    if (REF_DB_CLUB_HISTORY_RECORDS) {
                        c->mHistory.mRecordAttendance = ref->mHistory.mRecordAttendance;
                        c->mHistory.mRecordHomeWin = ref->mHistory.mRecordHomeWin;
                        c->mHistory.mRecordHomeDefeat = ref->mHistory.mRecordHomeDefeat;
                        c->mHistory.mRecordAwayWin = ref->mHistory.mRecordAwayWin;
                        c->mHistory.mRecordAwayDefeat = ref->mHistory.mRecordAwayDefeat;
                    }
                    if (REF_DB_CLUB_HISTORY_WINS) {
                        c->mHistory.mLeagueWinYears = ref->mHistory.mLeagueWinYears;
                        c->mHistory.mCupWinYears = ref->mHistory.mCupWinYears;
                        c->mHistory.mLeagueCupWinYears = ref->mHistory.mLeagueCupWinYears;
                        c->mHistory.mSuperCupsWinYears = ref->mHistory.mSuperCupsWinYears;
                        c->mHistory.mChampionsCupWinYears = ref->mHistory.mChampionsCupWinYears;
                        c->mHistory.mEuroTrophyWinYears = ref->mHistory.mEuroTrophyWinYears;
                        c->mHistory.mWorldChampionshipWinYears = ref->mHistory.mWorldChampionshipWinYears;
                        c->mHistory.mWorldClubChampionshipWinYears = ref->mHistory.mWorldClubChampionshipWinYears;
                    }
                    if (REF_DB_CLUB_HISTORY_LEAGUE_STATS) {
                        c->mLeagueTotalWins = ref->mLeagueTotalWins;
                        c->mLeagueTotalDraws = ref->mLeagueTotalDraws;
                        c->mLeagueTotalLoses = ref->mLeagueTotalLoses;
                        c->mLeagueTotalPoints = ref->mLeagueTotalPoints;
                        c->mLeagueTotalGoals = ref->mLeagueTotalGoals;
                        c->mLeagueTotalGoalsAgainst = ref->mLeagueTotalGoalsAgainst;
                        c->mLeagueTotalLeadershipsInTable = ref->mLeagueTotalLeadershipsInTable;
                    }
                }
            }
        }
        for (unsigned int c = 1; c <= FifamDatabase::NUM_COUNTRIES; c++) {
            if (c != FifamNation::Anguilla && c != FifamNation::Greenland) {
                FifamCountry *country = mFifamDatabase->GetCountry(c);
                if (c) {
                    FifamCountry *refCountry = mReferenceDatabase->GetCountry(c);
                    if (refCountry) {
                        if (REF_DB_WC_EC_STATISTICS) {
                            country->mNumContinentalChampions = refCountry->mNumContinentalChampions;
                            country->mNumContinentalRunnersUp = refCountry->mNumContinentalRunnersUp;
                            country->mNumWorldCups = refCountry->mNumWorldCups;
                            country->mNumWorldCupRunnersUp = refCountry->mNumWorldCupRunnersUp;
                        }
                    }
                }
            }
        }
    }

    std::wcout << L"Processing continental comps..." << std::endl;
    mFifamDatabase->mRules.mFairnessAwardWinners.clear();
    for (UInt i = 0; i < 6; i++) {
        mFifamDatabase->mRules.mContinentalCupChampions[i].mFirstCup.Clear();
        mFifamDatabase->mRules.mContinentalCupChampions[i].mSecondCup.Clear();
        mFifamDatabase->mRules.mContinentalCupChampions[i].mSuperCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mFirstCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mSecondCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mSuperCup.Clear();
    }

    auto ProcessContinentalComps = [&](Vector<Int> const &compIDs, FifamContinent continent, UInt type) {
        if (continent.ToInt() < 6) {
            if (type == 4)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mFirstCup.Clear();
            else if (type == 5)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSecondCup.Clear();
            else if (type == 6)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSuperCup.Clear();
        }
        Vector<foom::comp *> comps;
        for (Int id : compIDs) {
            foom::comp *comp = mFoomDatabase->get<foom::comp>(id);
            if (comp)
                comps.push_back(comp);
            if (!comps.empty()) {
                auto compWinners = GetWinnersList(comps, type >= 6);
                if (!compWinners.empty()) {
                    ProcessCompWinners(compWinners, nullptr, type);
                    if (continent.ToInt() < 6) {
                        if (type == 4) {
                            UShort lastYear = CURRENT_YEAR;
                            bool yearFound = compWinners.find(lastYear) != compWinners.end();
                            if (!yearFound) {
                                lastYear -= 1;
                                yearFound = compWinners.find(lastYear) != compWinners.end();
                            }
                            if (yearFound) {
                                if (!compWinners[lastYear]->mIsNation)
                                    mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mFirstCup = GetTeamClubLink(compWinners[lastYear]);
                            }
                        }
                        else if (compWinners.find(CURRENT_YEAR) != compWinners.end() && !compWinners[CURRENT_YEAR]->mIsNation) {
                            if (type == 5)
                                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSecondCup = GetTeamClubLink(compWinners[CURRENT_YEAR]);
                            else if (type == 6) {
                                UShort lastYear = CURRENT_YEAR;
                                bool yearFound = compWinners.find(lastYear) != compWinners.end();
                                if (!yearFound) {
                                    lastYear -= 1;
                                    yearFound = compWinners.find(lastYear) != compWinners.end();
                                }
                                if (yearFound) {
                                    if (!compWinners[lastYear]->mIsNation)
                                        mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSuperCup = GetTeamClubLink(compWinners[lastYear]);
                                }
                            }
                        }
                    }
                }
            }
        }

    };

    ProcessContinentalComps({ 1301394 }, FifamContinent::Europe, 4);
    ProcessContinentalComps({ 1301396 }, FifamContinent::Europe, 5);
    ProcessContinentalComps({ 1301397 }, FifamContinent::Europe, 6);

    ProcessContinentalComps({ 102415 }, FifamContinent::SouthAmerica, 4);
    ProcessContinentalComps({ 317567 }, FifamContinent::SouthAmerica, 5);
    ProcessContinentalComps({ 102418 }, FifamContinent::SouthAmerica, 6);

    ProcessContinentalComps({ 51002641, 1301417 }, FifamContinent::NorthAmerica, 4);
    ProcessContinentalComps({ 2000259628, 219740 }, FifamContinent::NorthAmerica, 5);
    ProcessContinentalComps({ 222987 }, FifamContinent::NorthAmerica, 6);

    ProcessContinentalComps({ 127299 }, FifamContinent::Africa, 4);
    ProcessContinentalComps({ 12017574 }, FifamContinent::Africa, 5);
    ProcessContinentalComps({ 131273 }, FifamContinent::Africa, 6);

    ProcessContinentalComps({ 127286 }, FifamContinent::Asia, 4);
    ProcessContinentalComps({ 1001959 }, FifamContinent::Asia, 5);

    ProcessContinentalComps({ 127285 }, FifamContinent::Oceania, 4);

    ProcessContinentalComps({ 121092 }, FifamContinent::None, 7); // Club World Cup
    ProcessContinentalComps({ 1301393 }, FifamContinent::None, 8); // TOYOTA Cup

    for (auto &entry : mFoomDatabase->mComps) {
        auto &c = entry.second;
        if (c.mID == 1301394) { // UEFA Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mFirstCup, c.mName);
        }
        else if (c.mID == 1301396) { // UEFA Europa League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mSecondCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mSecondCup, c.mName);
        }
        else if (c.mID == 1301397) { // UEFA Super Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mSuperCup = GetCompHost(&c, true);
            if (gameId >= 12) {
                if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
                else
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, c.mName);
            }
        }
        else if (c.mID == 1301398) { // UEFA Intertoto Cup
            if (gameId <= 11) {
                if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
                else
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, c.mName);
            }
        }

        else if (c.mID == 102415) { // Copa Bridgestone Libertadores
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup, c.mName);
        }
        else if (c.mID == 317567) { // Copa Total Sudamericana
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mSecondCup, c.mName);
        }
        else if (c.mID == 102418) { // Recopa Santander Sudamericana
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mSuperCup = GetCompHost(&c, true);
        }

        else if (c.mID == 51002641) { // CONCACAF Champions Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::NorthAmerica].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, c.mName);
        }
        else if (c.mID == 2000259628) { // Central American Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::NorthAmerica].mSecondCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mSecondCup, c.mName);
        }
        else if (c.mID == 222987) { // Campeones Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::NorthAmerica].mSuperCup = GetCompHost(&c);
        }

        else if (c.mID == 127299) { // Orange CAF Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mFirstCup, c.mName);
        }
        else if (c.mID == 12017574) { // Total CAF Confederation Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mSecondCup, c.mName);
        }
        else if (c.mID == 131273) { // Total CAF Super Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mSuperCup = GetCompHost(&c, true);
        }

        else if (c.mID == 127286) { // AFC Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Asia].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mFirstCup, c.mName);
        }
        else if (c.mID == 1001959) { // AFC Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Asia].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mSecondCup, c.mName);
        }

        else if (c.mID == 127285) { // Fiji Airways OFC Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Oceania].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Oceania].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Oceania].mFirstCup, c.mName);
        }

        //else if (!c.mIsExtinct && c.mNation && c.mType == 2) { // domestic main cup
        //    FifamClubLink winner, runnerUp;
        //    GetCompWinnerAndRunnerUp(&c, winner, runnerUp, true);
        //    if (runnerUp.IsValid()) {
        //        if (runnerUp.mTeamType == FifamClubTeamType::First)
        //            runnerUp.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
        //        else
        //            runnerUp.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
        //    }
        //    if (winner.IsValid()) {
        //        if (winner.mTeamType == FifamClubTeamType::First)
        //            winner.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
        //        else
        //            winner.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
        //    }
        //}
    }

    //{
    //    std::wcout << L"Processing domestic cupwinners..." << std::endl;
    //    for (FifamCountry *country : mFifamDatabase->mCountries) {
    //        if (country) {
    //            FifamClub *cupWinner = nullptr;
    //            FifamClub *cupRunnerUp = nullptr;
    //            for (FifamClub *club : country->mClubs) {
    //                foom::club *foomclub = club->GetProperty<foom::club *>(L"foom::club", nullptr);
    //                if (foomclub) {
    //                    if (!cupWinner && foomclub->mConverterData.mTableCupWinner)
    //                        cupWinner = club;
    //                    if (!cupRunnerUp && foomclub->mConverterData.mTableCupRunnerUp)
    //                        cupRunnerUp = club;
    //                }
    //            }
    //            if (cupWinner) {
    //                for (FifamClub *club : country->mClubs) {
    //                    
    //                }
    //            }
    //        }
    //    }
    //}

    {
        std::wcout << L"Reading fifam_continental_comps.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_continental_comps.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String continentName, compTypeName;
                    Int hostTeamId = 0;
                    reader.ReadLineWithSeparator(L'\t', continentName, compTypeName, hostTeamId);
                    FifamContinent continent = continentName;
                    if (!continent.GetWasSetFromUnknown() && continent.ToInt() >= 0 && continent.ToInt() <= 5) {
                        FifamCompType compType = compTypeName;
                        if (!compType.GetWasSetFromUnknown()) {
                            if (hostTeamId != 0) {
                                foom::team *hostTeam = mFoomDatabase->get<foom::team>(hostTeamId);
                                if (compType == FifamCompType::ChampionsLeague)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mFirstCup = GetTeamClubLink(hostTeam);
                                else if (compType == FifamCompType::UefaCup)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mSecondCup = GetTeamClubLink(hostTeam);
                                if (compType == FifamCompType::SuperCup)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mSuperCup = GetTeamClubLink(hostTeam);
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
            reader.Close();
        }
    }

    if (!mFifamDatabase->mRules.mContinentalCupChampions[FifamContinent::Europe].mSuperCup.IsValid()) {
        auto scWinner = mFoomDatabase->get<foom::team>(1687);
        if (scWinner)
            mFifamDatabase->mRules.mContinentalCupChampions[FifamContinent::Europe].mSuperCup = GetTeamClubLink(scWinner);
    }
    if (EXPORT_FM_FIFA_PLAYERS) {
        FifamWriter fifaPlayersWriter(L"fm-fifa-players.csv", 14, FifamVersion());
        if (fifaPlayersWriter.Available()) {
            fifaPlayersWriter.WriteLine(L"Country,Level,Club,Name,Birthdate,Nationality,Rating,FoomID,FifaID");
            auto WriteOnePlayer = [&](foom::player *_player, foom::club *_club, Int level, FifaTeam *fifaClub) {
                String countryName;
                if (_club && _club->mNation)
                    countryName = Quoted(_club->mNation->mShortName)();
                String clubName;
                if (_club)
                    clubName = Quoted(_club->mShortName)();
                String fifaIdStr;
                if (_player->mNation && fifaClub) {
                    fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
                        if (fifaIdStr.empty()) {
                            if (fifaPlayer.internal.nationality == _player->mNation->mConverterData.mFIFAID && fifaPlayer.m_birthDate == _player->mDateOfBirth)
                                fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
                        }
                    });
                    //if (fifaIdStr.empty()) {
                    //    String foomPlayerName = Utils::GetQuickName(_player->mFirstName, _player->mSecondName, _player->mCommonName);
                    //    fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
                    //        if (fifaIdStr.empty()) {
                    //            if (fifaPlayer.m_quickName == foomPlayerName)
                    //                fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
                    //        }
                    //    });
                    //}
                }
                String playerName;
                if (!_player->mCommonName.empty())
                    playerName = _player->mCommonName;
                else {
                    if (!_player->mFirstName.empty())
                        playerName = _player->mFirstName;
                    if (!_player->mSecondName.empty()) {
                        if (!playerName.empty())
                            playerName += L" ";
                        playerName += _player->mSecondName;
                    }
                }
                fifaPlayersWriter.WriteLine(
                    countryName,
                    level,
                    Quoted(clubName),
                    Quoted(playerName),
                    _player->mDateOfBirth,
                    _player->mNation ? _player->mNation->mThreeLetterName : L"",
                    _player->mCurrentAbility / 10,
                    _player->mID,
                    fifaIdStr);
            };
            for (auto &entry : mFoomDatabase->mClubs) {
                foom::club &team = entry.second;
                if (!team.mExtinct && team.mNation) {
                    Int leagueLevel = 99;
                    if (team.mDivision && team.mDivision->mCompetitionLevel > 0)
                        leagueLevel = team.mDivision->mCompetitionLevel;

                    FifaTeam *fifaClub = nullptr;
                    if (team.mConverterData.mFIFAID != 0) {
                        UInt fmClubFifaId = team.mConverterData.mFIFAID;
                        switch (fmClubFifaId) {
                        case 39: // Atalanta
                            fmClubFifaId = 115845;
                            break;
                        case 44: // Inter
                            fmClubFifaId = 131682;
                            break;
                        //case 45: // Juventus
                        //    fmClubFifaId = 114153;
                        //    break;
                        case 46: // Lazio
                            fmClubFifaId = 115841;
                            break;
                        case 47: // Milan
                            fmClubFifaId = 131681;
                            break;
                        //case 48: // Napoli
                        //    fmClubFifaId = 116365;
                        //    break;
                        //case 52: // AS Roma
                        //    fmClubFifaId = 114912;
                        //    break;
                        //case 1876: // River Plate
                        //    fmClubFifaId = 114144;
                        //    break;
                        //case 1877: // Boca Juniors
                        //    fmClubFifaId = 114149;
                        //    break;
                        case 190: // Brescia
                            fmClubFifaId = 113973;
                            break;
                        }
                        fifaClub = mFifaDatabase->GetTeam(fmClubFifaId);
                        if (fifaClub && fifaClub->m_gameId != FifaDatabase::m_lastSupportedGameVersion)
                            fifaClub = nullptr;
                    }

                    for (foom::player *p : team.mConverterData.mContractedPlayers)
                        WriteOnePlayer(p, &team, leagueLevel, fifaClub);
                    for (foom::player *p : team.mConverterData.mLoanedPlayers)
                        WriteOnePlayer(p, &team, leagueLevel, fifaClub);

                }
            }
            FifaTeam *freeAgentsClub = mFifaDatabase->GetTeam(111592);
            for (auto &entry : mFoomDatabase->mPlayers) {
                foom::player p = entry.second;
                if (!p.mContract.mClub)
                    WriteOnePlayer(&p, nullptr, 99, freeAgentsClub);
            }
            fifaPlayersWriter.Close();
        }
    }

    if (MAKE_CORRECTIONS_FOR_UPDATE && mPreviousDb)
        UpdateDataFromPreviousDb();

    // TEMPORARY - write staff debug info
    if (false) {
        FifamWriter staffWriter(L"staff_dbg.csv");
        staffWriter.WriteLine(L"Name", L"CA", L"PA");
        for (FifamStaff* staff : mFifamDatabase->mStaffs) {
            if (staff->mPersonType == FifamPersonType::Staff)
                staffWriter.WriteLine(staff->GetName(), staff->GetProperty<UInt>(L"foom::CA"), staff->GetProperty<UInt>(L"foom::PA"));
        }
    }

    mFifamDatabase->mWritingOptions.mWriteAssessment = false;
    mFifamDatabase->mWritingOptions.mWriteFixtures = false;

    Path outputPath = mWriteToGameFolder ? mOutputGameFolder : mTestsOutputFolder;
    Path outputDbPath = outputPath / L"database";

    std::wcout << L"Writing database..." << std::endl;
    mFifamDatabase->Write(gameId, FifamDatabase::GetGameDbVersion(gameId), outputDbPath);

    if (!mFromFifaDatabase) {
        if (WRITE_DERBIES && gameId >= 11) {
            std::wcout << L"Writing derbies..." << std::endl;
            struct derby_info {
                String nameId;
                String name;
                Array<String, 6> translatedNames;
                FifamClubLink club1;
                FifamClubLink club2;
                Int reputation = 0;
            };
            Vector<derby_info> derbies;
            for (auto &entry : mFoomDatabase->mDerbies) {
                foom::derby &d = entry.second;
                if (d.mTeam1 && d.mTeam2) {
                    derby_info i;
                    i.club1 = GetTeamClubLink(d.mTeam1, false); // NOTE: use 'true' for reserve teams
                    if (i.club1.IsValid()) {
                        i.club2 = GetTeamClubLink(d.mTeam2, false); // NOTE: use 'true' for reserve teams
                        if (i.club2.IsValid()) {
                            i.reputation = d.mWorldReputation * 1000 + d.mNationalReputation;
                            i.name = d.mName;
                            i.translatedNames = d.mTranslatedNames;
                            String nameId = Utils::ToUpper(Utils::GetStringWithoutUnicodeChars(d.mName));
                            i.nameId = L"IDS_DERBY_";
                            for (auto c : nameId) {
                                if (c != L'.') {
                                    if (c == L' ' || c == L'-')
                                        i.nameId.push_back(L'_');
                                    else
                                        i.nameId.push_back(c);
                                }
                            }
                            derbies.push_back(i);
                        }
                    }
                }
            }
            std::sort(derbies.begin(), derbies.end(), [](derby_info const &a, derby_info const &b) {
                return a.reputation > b.reputation;
            });
            if (derbies.size() > 999)
                derbies.resize(999);
            FifamWriter derbiesWriter(outputPath / L"fmdata\\ParameterFiles\\Derbies.txt", gameId, FifamVersion(), false);
            if (derbiesWriter.Available()) {
                derbiesWriter.WriteLine(L"BEGIN( DERBIES )");
                derbiesWriter.WriteNewLine();
                derbiesWriter.WriteLine(L"    DERBY_COUNT = " + Utils::Format(L"%d", derbies.size()));
                derbiesWriter.WriteNewLine();
                UInt maxNameIdLength = 0;
                for (UInt i = 0; i < derbies.size(); i++) {
                    UInt len = derbies[i].nameId.length();
                    if (len > maxNameIdLength)
                        maxNameIdLength = len;
                }
                for (UInt i = 0; i < derbies.size(); i++) {
                    derbiesWriter.WriteLine(Utils::Format(L"    DERBY%03d = %-" + std::to_wstring(maxNameIdLength)
                        + L"s 0x%08X, 0x%08X                  // %s, %s",
                        i + 1, String(derbies[i].nameId + L",").c_str(), derbies[i].club1.mPtr->mUniqueID, derbies[i].club2.mPtr->mUniqueID,
                        Utils::GetStringWithoutUnicodeChars(derbies[i].club1.GetTeamName()).c_str(),
                        Utils::GetStringWithoutUnicodeChars(derbies[i].club2.GetTeamName()).c_str()));
                }
                derbiesWriter.WriteNewLine();
                derbiesWriter.Write(L"END");
                derbiesWriter.Close();
            }
            FifamWriter derbyNamesWriter(Utils::Format(outputPath / L"plugins\\ucp\\derbies.tr", gameId),
                gameId, FifamVersion(), true);
            Set<String> writtenDerbyNames;
            if (derbyNamesWriter.Available()) {
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (!Utils::Contains(writtenDerbyNames, derbies[i].nameId)) {
                        derbyNamesWriter.WriteLineWithSeparator(L"|", derbies[i].nameId, derbies[i].name,
                            derbies[i].translatedNames[0], derbies[i].translatedNames[1], derbies[i].translatedNames[2],
                            derbies[i].translatedNames[3], derbies[i].translatedNames[4]);
                        writtenDerbyNames.insert(derbies[i].nameId);
                    }
                }
                derbyNamesWriter.Close();
            }
            FifamWriter derbyNamesWriterRu(Utils::Format(outputPath / L"plugins\\ucp\\derbies_ru.tr", gameId),
                gameId, FifamVersion(), true);
            Set<String> writtenDerbyNamesRu;
            if (derbyNamesWriterRu.Available()) {
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (!Utils::Contains(writtenDerbyNamesRu, derbies[i].nameId)) {
                        if (!derbies[i].translatedNames[5].empty())
                            derbyNamesWriterRu.WriteLineWithSeparator(L"|", derbies[i].nameId, derbies[i].translatedNames[5]);
                        writtenDerbyNamesRu.insert(derbies[i].nameId);
                    }
                }
                derbyNamesWriterRu.Close();
            }
        }

        std::wcout << L"Writing female names..." << std::endl;

        Map<String, Bool> femaleNamesMap;
        Map<String, Bool> femaleSurnamesMap;
        Map<String, Bool> femaleCommonNamesMap;

        // find female names
        for (FifamStaff *staff : mFifamDatabase->mStaffs) {
            auto fmstaff = staff->GetProperty<foom::non_player *>(L"foom::non_player");
            if (fmstaff && fmstaff->mGender) {
                if (!staff->mFirstName.empty())
                    femaleNamesMap[staff->mFirstName] = true;
                if (!staff->mLastName.empty())
                    femaleSurnamesMap[staff->mLastName] = true;
                if (!staff->mPseudonym.empty())
                    femaleCommonNamesMap[staff->mPseudonym] = true;
            }
        }
        for (FifamReferee *ref : mFifamDatabase->mReferees) {
            auto fmofficial = ref->GetProperty<foom::official *>(L"foom::official");
            if (fmofficial && fmofficial->mGender) {
                if (!ref->mFirstName.empty())
                    femaleNamesMap[ref->mFirstName] = true;
                if (!ref->mLastName.empty())
                    femaleSurnamesMap[ref->mLastName] = true;
            }
        }
        // find male names
        for (FifamPlayer *player : mFifamDatabase->mPlayers) {
            auto fmplayer = player->GetProperty<foom::player *>(L"foom::player");
            if (fmplayer && !fmplayer->mGender) {
                if (!player->mFirstName.empty()) {
                    auto it = femaleNamesMap.find(player->mFirstName);
                    if (it != femaleNamesMap.end())
                        (*it).second = false;
                }
                if (!player->mLastName.empty()) {
                    auto it = femaleSurnamesMap.find(player->mLastName);
                    if (it != femaleSurnamesMap.end())
                        (*it).second = false;
                }
                if (!player->mPseudonym.empty()) {
                    auto it = femaleCommonNamesMap.find(player->mPseudonym);
                    if (it != femaleCommonNamesMap.end())
                        (*it).second = false;
                }
            }
        }
        for (FifamStaff *staff : mFifamDatabase->mStaffs) {
            auto fmstaff = staff->GetProperty<foom::non_player *>(L"foom::non_player");
            if (fmstaff && !fmstaff->mGender) {
                if (!staff->mFirstName.empty()) {
                    auto it = femaleNamesMap.find(staff->mFirstName);
                    if (it != femaleNamesMap.end())
                        (*it).second = false;
                }
                if (!staff->mLastName.empty()) {
                    auto it = femaleSurnamesMap.find(staff->mLastName);
                    if (it != femaleSurnamesMap.end())
                        (*it).second = false;
                }
                if (!staff->mPseudonym.empty()) {
                    auto it = femaleCommonNamesMap.find(staff->mPseudonym);
                    if (it != femaleCommonNamesMap.end())
                        (*it).second = false;
                }
            }
        }
        for (FifamReferee *ref : mFifamDatabase->mReferees) {
            auto fmofficial = ref->GetProperty<foom::official *>(L"foom::official");
            if (fmofficial && !fmofficial->mGender) {
                if (!ref->mFirstName.empty()) {
                    auto it = femaleNamesMap.find(ref->mFirstName);
                    if (it != femaleNamesMap.end())
                        (*it).second = false;
                }
                if (!ref->mLastName.empty()) {
                    auto it = femaleSurnamesMap.find(ref->mLastName);
                    if (it != femaleSurnamesMap.end())
                        (*it).second = false;
                }
            }
        }

        FifamWriter femaleNamesWriter(Utils::Format(outputPath / L"fmdata" / L"UCP_FemaleNames.txt", gameId), gameId, FifamVersion(), true);
        for (auto const &[name, write] : femaleNamesMap) {
            if (write)
                femaleNamesWriter.WriteLine(name);
        }
        femaleNamesWriter.Close();
        FifamWriter femaleSurnamesWriter(Utils::Format(outputPath / L"fmdata" / L"UCP_FemaleSurnames.txt", gameId), gameId, FifamVersion(), true);
        for (auto const &[name, write] : femaleSurnamesMap) {
            if (write)
                femaleSurnamesWriter.WriteLine(name);
        }
        femaleSurnamesWriter.Close();
        FifamWriter femaleCommonNamesWriter(Utils::Format(outputPath / L"fmdata" / L"UCP_FemaleCommonNames.txt", gameId), gameId, FifamVersion(), true);
        for (auto const &[name, write] : femaleCommonNamesMap) {
            if (write)
                femaleCommonNamesWriter.WriteLine(name);
        }
        femaleCommonNamesWriter.Close();
    }
    if (WRITE_DUPLICATE_PLAYER_IDS) {
        std::wcout << L"Writing duplicate player ids..." << std::endl;
        Map<String, Set<String>> dupIds;
        for (FifamPlayer* player : mFifamDatabase->mPlayers) {
            auto pp = player->mWriteableStringID.rfind(L'-');
            if (pp != String::npos && Utils::IsNumber(player->mWriteableStringID.substr(pp + 1)))
                dupIds[player->mWriteableStringID.substr(0, pp)].insert(player->mWriteableStringID);
        }
        FifamWriter duplicateIdsWriter(L"duplicate_player_ids.txt");
        duplicateIdsWriter.WriteLineWithSeparator(L'\t', L"BaseID", L"PlayerID");
        for (auto const& [baseId, empicsIds] : dupIds) {
            for (auto const &empicsId : empicsIds)
                duplicateIdsWriter.WriteLineWithSeparator(L'\t', baseId, empicsId);
        }
        duplicateIdsWriter.Close();
    }
    {
        FifamWriter reserveLeaguesWriter(outputPath / L"plugins\\ucp\\reserve_leagues.csv");
        reserveLeaguesWriter.WriteLine(L"CountryId", L"ReserveLevelId", L"CountryName");
        for (UInt countryId = 1; countryId <= FifamDatabase::NUM_COUNTRIES; countryId++) {
            FifamCountry *country = mFifamDatabase->GetCountry(countryId);
            if (country) {
                UChar reserveLevelId = 255;
                for (auto const &[id, c] : mFifamDatabase->mCompMap) {
                    if (id.mType == FifamCompType::Pool && id.mRegion.ToInt() == countryId && c->AsPool() && c->AsPool()->mReserveTeamsAllowed && c->mCompetitionLevel < reserveLevelId)
                        reserveLevelId = c->mCompetitionLevel;
                }
                reserveLeaguesWriter.WriteLine(countryId, (reserveLevelId == 255) ? 255 : (reserveLevelId + 1), FifamTr(country->mName));
            }
        }
    }

#if 1
    if (!mFromFifaDatabase) {
        graphicsConverter.mOnlyUpdates = GRAPHICS_UPDATE_ONLY;
        graphicsConverter.mOutputToGameFolder = GRAPHICS_WRITE_TO_GAME_FOLDER;
        if (CONVERT_COUNTRY_FLAGS) {
            std::wcout << L"Converting country flags..." << std::endl;
            graphicsConverter.ConvertCountryFlags(mFifamDatabase, graphicsPath, mContentFolder, gameId, mOutputGameFolder);
        }
        if (CONVERT_CLUB_BADGES) {
            std::wcout << L"Converting club badges..." << std::endl;
            graphicsConverter.ConvertClubBadges(mFoomDatabase, mAvailableBadges, graphicsPath, mContentFolder, gameId, mOutputGameFolder, 0);
        }
        if (CONVERT_COMP_BADGES) {
            std::wcout << L"Converting competition badges..." << std::endl;
            graphicsConverter.ConvertCompBadges(mFifamDatabase, graphicsPath, mContentFolder, gameId, mOutputGameFolder, 0);
        }
        if (COPY_LEAGUE_SPLIT_AND_RELEGATION_BADGES)
            graphicsConverter.CopyLeagueSplitAndRelegationBadges(mFifamDatabase, mContentFolder, gameId, mOutputGameFolder);
        if (CONVERT_PORTRAITS) {
            std::wcout << L"Converting portraits..." << std::endl;
            graphicsConverter.ConvertPortraits(mFoomDatabase, graphicsPath, mContentFolder, gameId, mOutputGameFolder, 0);
        }
        if (CONVERT_TROPHIES) {
            std::wcout << L"Converting trophies..." << std::endl;
            graphicsConverter.ConvertTrophies(mFifamDatabase, graphicsPath, mContentFolder, gameId, mOutputGameFolder, 0);
        }
        if (CONVERT_CITYPICS)
            graphicsConverter.ConvertCities(mFoomDatabase, mContentFolder, mOutputGameFolder, gameId, mOutputGameFolder, 0);
        if (CONVERT_STADPICS)
            graphicsConverter.ConvertStadiums(mFoomDatabase, mContentFolder, mOutputGameFolder, gameId, mOutputGameFolder, 0, false);
    }
    else {
        if (CONVERT_CLUB_BADGES) {
            std::wcout << L"Converting FIFA club badges..." << std::endl;
            graphicsConverter.ConvertClubBadgesFIFA(mFifamDatabase, mFifaAssetsPath, mContentFolder, gameId, mOutputGameFolder);
        }
        if (CONVERT_COMP_BADGES) {
            std::wcout << L"Converting FIFA competition badges..." << std::endl;
            graphicsConverter.ConvertCompBadgesFIFA(mFifamDatabase, mFifaAssetsPath, mContentFolder, gameId, mOutputGameFolder);
        }
        if (CONVERT_PORTRAITS) {
            std::wcout << L"Converting FIFA player pictures..." << std::endl;
            graphicsConverter.ConvertPlayerPortraitsFIFA(mFifamDatabase, mFifaAssetsPath, mContentFolder, gameId, mOutputGameFolder);
        }
        //graphicsConverter.DownloadPlayerPortraitsFIFA(mFifamDatabase, mOutputGameFolder, gameId);
    }
#endif
}

Converter::~Converter() {
    delete mReferenceDatabase;
    delete mFifamDatabase;
    delete mFoomDatabase;
    delete mFifaDatabase;
    delete mPreviousDb;
}

FifamFormation Converter::ConvertFormationId(Int id) {
    switch (id) {
    case 2:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
        return FifamFormation::_5_3_2_Attacking;
    case 3:
        return FifamFormation::_4_4_2_Normal;
    case 32:
        return FifamFormation::_4_1_4_1;
    case 7:
    case 20:
    case 52:
    case 62:
        return FifamFormation::_4_4_2_Diamond;
    case 8:
        return FifamFormation::_4_5_1_Normal;
    case 12:
        return FifamFormation::_4_3_3_Normal;
    case 14:
        return FifamFormation::_4_4_2_Diamond_Attacking;
    case 15:
        return FifamFormation::_5_4_1;
    case 16:
    case 19:
    case 33:
        return FifamFormation::_4_4_1_1_Defensive;
    case 18:
    case 21:
    case 23:
        return FifamFormation::_4_3_3_Attacking;
    case 22:
    case 24:
        return FifamFormation::_4_4_2_Attacking;
    case 29:
    case 54:
    case 55:
        return FifamFormation::_4_2_4;
    case 25:
    case 34:
    case 60:
        return FifamFormation::_3_5_2;
    case 4:
    case 26:
    case 27:
        return FifamFormation::_4_3_3_Wings;
    case 28:
        return FifamFormation::_4_2_2_1_1_V2;
    case 30:
    case 63:
        return FifamFormation::_5_2_2_1;
    case 31:
    case 35:
    case 68:
        return FifamFormation::_3_4_3_Defensive;
    case 36:
    case 40:
        return FifamFormation::_4_3_3_Defensive;
    case 41:
    case 56:
    case 64:
    case 66:
        return FifamFormation::_5_2_1_2;
    case 42:
    case 59:
    case 61:
    case 67:
        return FifamFormation::_5_2_2_1;
    case 47:
    case 69:
        return FifamFormation::_5_3_2;
    case 65:
        return FifamFormation::_3_4_3_Wings;
    }
    return FifamFormation::None;
}

Int Converter::ConvertFormationIdToCustom(Int id) {
    switch (id) {
    case 2:
        return FMF_FORMATION_5_3_2_WB; // 5-3-2 WB > 5-3-2
    case 3:
        return FMF_FORMATION_4_4_2; // 4-4-2 > 4-4-2
    case 4:
        return FMF_FORMATION_4_3_3_DM_WIDE; // 4-1-4-1 DM Wide > 4-3-3 Wing
    case 7:
        return FMF_FORMATION_4_4_2_DIAMOND_NARROW; // 4-4-2 Diamond Narrow > 4-4-2 ROff
    case 8:
        return FMF_FORMATION_4_5_1; // 4-5-1 > 4-1-4-1 Clas
    case 12:
        return FMF_FORMATION_4_3_2_1_NARROW; // 4-3-2-1 Narrow > 4-3-3 T2
    case 14:
        return FMF_FORMATION_4_3_1_2_NARROW; // 4-3-1-2 Narrow > 4-3-1-2
    case 15:
        return FMF_FORMATION_5_4_1_DIAMOND_WB; // 5-4-1 Diamond WB > 5-4-1
    case 16:
        return FMF_FORMATION_4_4_1_1; // 4-4-1-1 > 4-4-1-1
    case 18:
        return FMF_FORMATION_4_2_3_1_NARROW; // 4-2-3-1 Narrow > 4-5-1 Off
    case 19:
        return FMF_FORMATION_4_1_3_1_1_DM_AM; // 4-1-3-1-1 DM AM > 4-2-2-1-1-V3
    case 20:
        return FMF_FORMATION_4_1_3_2_NARROW; // 4-1-3-2 DM Narrow > 4-4-2 ROff
    case 21:
        return FMF_FORMATION_4_2_3_1_WIDE; // 4-2-3-1 Wide > 4-3-3 Off
    case 22:
        return FMF_FORMATION_4_2_2_2_DM; // 4-2-2-2 DM > 4-4-2 Def
    case 23:
        return FMF_FORMATION_4_2_1_3_DM_WIDE; // 4-2-1-3 DM Wide > 4-3-3 Df V2
    case 24:
        return FMF_FORMATION_4_2_2_2_DM_NARROW; // 4-2-2-2 DM Narrow > 4-2-2-2
    case 25:
        return FMF_FORMATION_3_1_3_1_2_DM; // 3-1-3-1-2 DM > 3-5-2 Sweeper
    case 26:
        return FMF_FORMATION_4_1_2_3_DM_NARROW; // 4-1-2-3 DM Narrow > 4-3-3 Def
    case 27:
        return FMF_FORMATION_4_3_2_1_DM_AM_NARROW; // 4-1-2-3 DM AM Narrow > 4-3-3 T2
    case 28:
        return FMF_FORMATION_4_2_4_DM_WIDE; // 4-2-4 DM Wide > 4-4-2 Double6
    case 29:
        return FMF_FORMATION_4_2_3_1_DM_AM_WIDE; // 4-2-3-1 DM Wide > 4-3-3 Of V2
    case 30:
        return FMF_FORMATION_5_3_2_DM_WB; // 5-1-2-2 DM WB > 5-3-2
    case 31:
        return FMF_FORMATION_3_4_2_1_DM; // 3-4-2-1 DM > 5-4-1
    case 32:
        return FMF_FORMATION_4_1_4_1_DM; // 4-1-4-1 DM > 4-1-4-1 Clas
    case 33:
        return FMF_FORMATION_4_4_1_1_2DM; // 4-4-1-1 2DM > 4-1-4-1 Def (wrong name - must be 4-4-1-1 Def)
    case 34:
        return FMF_FORMATION_3_2_3_2_DM; // 3-2-3-2 DM > 3-5-2
    case 35:
    case 68:
        return FMF_FORMATION_5_2_3_DM_WIDE; // 3-4-3 DM Wide > 5-4-1 F
    case 36:
        return FMF_FORMATION_4_3_3_NARROW; // 4-3-3 Narrow > 4-3-3 T1
    case 40:
        return FMF_FORMATION_4_2_3_1_DM_AM_NARROW; // 4-2-3-1 DM AM Narrow > 4-5-1 Of V2
    case 41:
        return FMF_FORMATION_5_2_1_2_WB; // 5-2-1-2 WB > 5-3-2
    case 42:
        return FMF_FORMATION_5_1_2_2_DM_WB_NARROW; // 5-1-2-2 DM Narrow > 5-3-2
    case 47:
        return FMF_FORMATION_5_2_3_NARROW; // 5-2-3 Narrow > 5-4-1 F
    case 52:
        return FMF_FORMATION_4_1_3_2_DM; // 4-1-3-2 DM > 4-4-2 Rf
    case 54:
        return FMF_FORMATION_4_2_3_1_DM; // 4-2-3-1 DM > 4-5-1 Def
    case 55:
        return FMF_FORMATION_4_2_3_1_0_DM; // 4-2-3-1-0 DM Wide > 4-5-1 Def
    case 56:
        return FMF_FORMATION_5_1_3_1_DM_WB; // 5-1-3-1 DM WB > 5-4-1
    case 57:
        return FMF_FORMATION_5_2_2_1_DM; // 5-2-2-1 DM > 5-4-1
    case 59:
        return FMF_FORMATION_5_2_2_1_WB; // 5-2-2-1 WB > 5-4-1
    case 60:
        return FMF_FORMATION_3_1_4_2_DM; // 3-1-4-2 DM > 3-5-2 F
    case 61:
        return FMF_FORMATION_5_4_1_WB_WIDE; // 5-4-1 WB Wide > 5-4-1 F
    case 62:
        return FMF_FORMATION_4_2_2_2_DM_AM_NARROW;
    case 63:
        return FMF_FORMATION_5_2_2_1_DM_AM;
    case 64:
        return FMF_FORMATION_5_2_1_2_DM_AM;
    case 65:
        return FMF_FORMATION_3_2_5_DM_2AM_WIDE;
    case 66:
        return FMF_FORMATION_5_2_1_2_DM_AM_NARROW;
    case 67:
        return FMF_FORMATION_5_2_2_1_DM_AM_NARROW;
    case 69:
        return FMF_FORMATION_5_3_2_DM_NARROW;
    }
    return 0;
}

void Converter::GenerateNewTeamIDsFile(Path const &outputFilePath, Path const &oldTeamIDsFilePath) {
    if (!mFoomDatabase) {
        Alert(AL_ERROR, L"FoomDatabase is not available");
        return;
    }
    struct TeamDesc {
        String mCountry;
        String mName;
        String mExtinct;
        String mAffiliationType;
        String mMainTeamName;
        UInt mFifaManagerID = 0;
        UInt mFIFAID = 0;
        String mStatus;
        String mLeague;
        UInt mYearOF = 0;
        String mCity;
        UInt mReputation = 0;
        UInt mLeagueLevel = 255;
        Bool mValid = false;
        Bool mNew = false;
    };
    Map<UInt, TeamDesc> uidsMap;
    FifamReader reader(oldTeamIDsFilePath, 0);
    if (reader.Available()) {
        reader.SkipLine();
        Map<UInt, UInt> uniqueUIDsMap;
        while (!reader.IsEof()) {
            if (!reader.EmptyLine()) {
                TeamDesc info;
                Int FootballManagerID = -1;
                reader.ReadLine(info.mCountry, info.mName, info.mExtinct, info.mAffiliationType, info.mMainTeamName,
                    FootballManagerID, Hexadecimal(info.mFifaManagerID), info.mFIFAID, info.mStatus, info.mLeague, info.mYearOF, info.mCity,
                    info.mReputation, info.mLeagueLevel);
                if (info.mFifaManagerID > 0) {
                    auto it = uniqueUIDsMap.find(info.mFifaManagerID);
                    if (it != uniqueUIDsMap.end()) {
                        Alert(AL_ERROR, L"Duplicated Unique ID in old team IDs file: %08X\nClub1: %d\nClub2: %d", info.mFifaManagerID, (*it).second, FootballManagerID);
                    }
                    else {
                        info.mValid = false;
                        uidsMap[FootballManagerID] = info;
                        uniqueUIDsMap[info.mFifaManagerID] = FootballManagerID;
                    }
                }
            }
            else
                reader.SkipLine();
        }
    }
    Array<UInt, FifamDatabase::NUM_COUNTRIES> maxUid = {};
    for (auto [foomId, info] : uidsMap) {
        if (info.mFifaManagerID > 0) {
            UChar region = (info.mFifaManagerID >> 16) & 0xFF;
            if (region >= 1 && region <= FifamDatabase::NUM_COUNTRIES) {
                if (info.mFifaManagerID > maxUid[region - 1])
                    maxUid[region - 1] = info.mFifaManagerID;
            }
        }
    }
    for (auto const &[foomId, club] : mFoomDatabase->mClubs) {
        if (club.mIsReserveDummyClub || club.mIsReserveToCreateClub)
            continue;
        TeamDesc info;
        info.mValid = true;
        foom::nation *n = club.mNation;
        if (!n)
            n = club.mContinentalCupNation;
        if (n)
            info.mCountry = n->mName;
        info.mName = club.mName;
        if (club.mExtinct)
            info.mExtinct = L"Extinct";
        for (auto const &af : club.mVecAffiliations) {
            if (!af.mIsMainClub) {
                if ((af.mAffiliationType >= 2 && af.mAffiliationType <= 8) || af.mAffiliationType == 18 || af.mAffiliationType == 22 || af.mAffiliationType == 24 || af.mAffiliationType == 25) {
                    if (af.mAffiliationType == 2)
                        info.mAffiliationType = L"Sub Team";
                    else if (af.mAffiliationType == 3)
                        info.mAffiliationType = L"Feeder Team";
                    else if (af.mAffiliationType == 4)
                        info.mAffiliationType = L"B Club";
                    else if (af.mAffiliationType == 5)
                        info.mAffiliationType = L"C Club";
                    else if (af.mAffiliationType == 6)
                        info.mAffiliationType = L"2 Club";
                    else if (af.mAffiliationType == 7)
                        info.mAffiliationType = L"3 Club";
                    else if (af.mAffiliationType == 8)
                        info.mAffiliationType = L"II Club";
                    else if (af.mAffiliationType == 18)
                        info.mAffiliationType = L"Shared Youth Team";
                    else if (af.mAffiliationType == 22)
                        info.mAffiliationType = L"Regional Academy";
                    else if (af.mAffiliationType == 24)
                        info.mAffiliationType = L"Extinct B or C Club";
                    else if (af.mAffiliationType == 25)
                        info.mAffiliationType = L"Womens Club";
                    else
                        info.mAffiliationType = L"Unknown";
                    if (af.mAffiliatedClub)
                        info.mMainTeamName = af.mAffiliatedClub->mName;
                    break;
                }
            }
        }
        auto it = uidsMap.find(foomId);
        if (it == uidsMap.end()) {
            if (n) {
                UInt countryId = n->mConverterData.mFIFAManagerID;
                //if (n->mID == 217945)
                //    countryId = 207; // Kosovo
                if (countryId >= 1 && countryId <= FifamDatabase::NUM_COUNTRIES) {
                    info.mFifaManagerID = ++maxUid[countryId - 1];
                    info.mNew = true;
                }
            }
        }
        else {
            info.mFifaManagerID = (*it).second.mFifaManagerID;
            info.mFIFAID = (*it).second.mFIFAID;
        }
        if (club.mStatus == 0)
            info.mStatus = L"Not Set";
        else if (club.mStatus == 1)
            info.mStatus = L"Professional";
        else if (club.mStatus == 2)
            info.mStatus = L"Semi Professional";
        else if (club.mStatus == 3)
            info.mStatus = L"Amateur";
        else if (club.mStatus == 20)
            info.mStatus = L"Always Amateur";
        else
            info.mStatus = L"Unknown";
        if (club.mDivision) {
            info.mLeague = club.mDivision->mName;
            info.mLeagueLevel = club.mDivision->mCompetitionLevel;
            if (info.mLeagueLevel == 0) {
                foom::comp *parent = club.mDivision->mParentCompetition;
                foom::comp *root = club.mDivision;
                while (parent) {
                    root = parent;
                    parent = parent->mParentCompetition;
                }
                info.mLeagueLevel = root->mCompetitionLevel;
            }
        }
        info.mYearOF = club.mYearFounded;
        if (club.mCity)
            info.mCity = club.mCity->mName;
        info.mReputation = club.mReputation;
        uidsMap[foomId] = info;
    }
    FifamWriter clubsUIDsWriter(outputFilePath, 14, FifamVersion());
    clubsUIDsWriter.WriteLine(L"Country,Name,Extinct,AffiliationType,MainTeamName,FootballManagerID,FIFAManagerID,FIFAID,Status,League,YearOF,City,Reputation,LeagueLevel,Valid,IsNew");
    for (auto [foomId, info] : uidsMap) {
        String fifamId;
        if (info.mFifaManagerID != 0)
            fifamId = Utils::Format(L"0x%08X", info.mFifaManagerID);
        clubsUIDsWriter.WriteLine(Quoted(info.mCountry), Quoted(info.mName), info.mExtinct, info.mAffiliationType, Quoted(info.mMainTeamName),
            foomId, fifamId, info.mFIFAID, info.mStatus, Quoted(info.mLeague), info.mYearOF, Quoted(info.mCity), info.mReputation, info.mLeagueLevel,
            info.mValid, info.mNew);
    }
}

Bool Converter::ClubColorsFromBadgeFile(UInt clubId, FifamClubTeamColor &out) {
    Bool result = false;
    String filename = Utils::Format(L"%08X.tga", clubId);
    const String resolution = L"32x32";
    static Path p1 = Path(mOutputGameFolder / L"badges\\clubs") / resolution / filename;
    static Path p2 = Path(mContentArtsFolder / L"badges\\badges\\clubs") / resolution / filename;
    Path p;
    if (!exists(p1)) {
        if (exists(p2)) {
            p = p2;
            result = true;
        }
    }
    else {
        p = p1;
        result = true;
    }
    if (result) {
        out = GraphicsConverter::GetBadgeColor(p);
        return true;
    }
    return false;
}
