#include "Converter.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifamNames.h"
#include "FifamUtils.h"
#include "FifamPlayerGenerator.h"
#include "FifamPlayerLevel.h"
#include "Random.h"

#define DB_SIZE Tiny

Converter::~Converter() {
    delete mReferenceDatabase;
    delete mFifamDatabase;
    delete mFoomDatabase;
}

void Converter::ReadAdditionalInfo(Path const &infoPath) {
    {
        std::wcout << L"Reading fifam-uids.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam-uids.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::club::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, dummy, FootballManagerID, Hexademical(info.mFIFAManagerID), info.mFIFAID);
                    map_find(mFoomDatabase->mClubs, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_countries.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_countries.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::nation::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, dummy, dummy, FootballManagerID, info.mFIFAID, dummy, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mNations, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_languages.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_languages.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::language::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mLanguages, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_regions.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_regions.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::region::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mRegions, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_injuries.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_injuries.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    foom::injury::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(FootballManagerID, info.mFIFAManagerID);
                    map_find(mFoomDatabase->mInjuries, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_divisions.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_divisions.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    DivisionInfo d;
                    reader.ReadLineWithSeparator(L'\t', d.mNationName, d.mNationID, d.mName, d.mShortName, d.mID, d.mIsLvl, d.mLevel, d.mIsTop, d.mParent, d.mTeams, d.mRep, d.mPriority);
                    if (d.mLevel <= 0)
                        d.mLevel = 256;
                    else
                        d.mLevel -= 1;
                    mDivisions.push_back(d);
                }
                else
                    reader.SkipLine();
            }
        }
    }
}

void Converter::Convert(UInt gameId, Bool writeToGameFolder) {

}

void Converter::Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder) {
    // read reference database here

    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    mFifamDatabase = new FifamDatabase(originalGameId, originalDb);
    mFoomDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom", foom::db::db_size::DB_SIZE);
    ReadAdditionalInfo(L"D:\\Projects\\fifam\\db");

    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // for all affiliated clubs
        for (auto &a : c.mVecAffiliations) {
            if (a.mAffiliatedClub && a.mIsMainClub) {
                if (a.mStartDate <= GetCurrentDate() && (a.mEndDate == FmEmptyDate() || a.mEndDate > GetCurrentDate())) {
                    foom::club *child = nullptr;
                    foom::club::converter_data::child_type childType;
                    if (a.mAffiliationType == 4 || a.mAffiliationType == 6 || a.mAffiliationType == 8) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second;
                    }
                    else if (a.mAffiliationType == 5 || a.mAffiliationType == 7) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::third;
                    }
                    if (child) {
                        if (child->mConverterData.mParentClub) {
                            Error(L"Affiliated B/C club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                                child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
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
                if (child) {
                    if (child->mConverterData.mParentClub) {
                        Error(L"Reserve club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                            child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
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
            }
            );

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
        if (c.mConverterData.mParentClub && !c.mConverterData.mChildClubs.empty())
            Error(L"Reserve club has reserve teams\nClub: '%s'", c.mName.c_str());
    }

    for (auto &entry : mFoomDatabase->mComps) {
        auto &c = entry.second;
        // get champion
        for (auto &h : c.mVecHistory) {
            if (h.mYear > 0 && h.mFirstPlaced) {
                if (!h.mFirstPlaced->mIsNation) {
                    foom::club *club = (foom::club *)h.mFirstPlaced;
                    if (c.mID == 1301394 || c.mID == 102415 || c.mID == 127286 || c.mID == 127299 || c.mID == 51002641 || c.mID == 1301417 || c.mID == 127285) // Champions League
                        club->mConverterData.mChampionsLeagueWins.insert(h.mYear);
                    else if (c.mID == 1301396 || c.mID == 317567 || c.mID == 1001959 || c.mID == 12017574 || c.mID == 5623042) // Europa League
                        club->mConverterData.mEuropaLeagueWins.insert(h.mYear);
                    else if (c.mID == 121092) // Club World Cup
                        club->mConverterData.mClubWorldCupWins.insert(h.mYear);
                    else if (c.mID == 1301393) // TOYOTA Cup
                        club->mConverterData.mToyotaCupWins.insert(h.mYear);
                    else if (!c.mIsExtinct && c.mNation && club->mNation == c.mNation) { // domestic competitions
                        // league
                        if (c.mType == 0)
                            club->mConverterData.mLeagueWins.insert(h.mYear);
                        // cup
                        if (c.mType == 2)
                            club->mConverterData.mCupWins.insert(h.mYear);
                        // league cup
                        if (c.mType == 3)
                            club->mConverterData.mLeagueCupWins.insert(h.mYear);
                        // supercup
                        if (c.mType == 5)
                            club->mConverterData.mSuperCupWins.insert(h.mYear);
                    }
                }
                else {
                    foom::nation *nation = (foom::nation *)h.mFirstPlaced;
                    foom::nation *nationRunnerUp = (foom::nation *)h.mSecondPlaced;
                    if (c.mID == 1301385) { // World Cup
                        nation->mConverterData.mWorldCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mWorldCupFinals.insert(h.mYear);
                    }
                    else if (c.mID == 1301388) { // Euro Cup
                        nation->mConverterData.mEuroCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mEuroCupFinals.insert(h.mYear);
                    }
                }
            }
        }
    }

    // convert nations, national teams, leagues
    for (auto &entry : mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto country = mFifamDatabase->GetCountry(nation.mConverterData.mFIFAManagerID);
        if (country) {
            ConvertNationInfo(country, &nation);

            Map<UInt, DivisionInfo *> leagueLevels;
            Vector<DivisionInfo *> comps;
            Vector<DivisionInfo *> unusedComps;

            // find all division competitions for this country
            for (auto &divInfo : mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    Int minPriority = (gameId <= 7) ? 2 : 1;
                    if (divInfo.mPriority >= minPriority) {
                        if (divInfo.mIsLvl)
                            leagueLevels[divInfo.mLevel] = &divInfo;
                        else
                            comps.push_back(&divInfo);
                    }
                    else
                        unusedComps.push_back(&divInfo);
                }
            }
            // sort comps
            std::sort(comps.begin(), comps.end(), [](DivisionInfo *a, DivisionInfo *b) {
                return a->mLevel < b->mLevel;
            });
            // add league levels
            for (UInt i = 0; i < 16; i++) {
                DivisionInfo *level = nullptr;
                String levelName;
                if (leagueLevels.find(i) != leagueLevels.end()) {
                    level = leagueLevels[i];
                    if (level->mName.length() < 29)
                        levelName = level->mName;
                    else
                        levelName = level->mShortName;
                }
                else {
                    Vector<String> divsOnLevelNames;
                    for (UInt c = 0; c < comps.size(); c++) {
                        if (comps[c]->mLevel == i) {
                            if (!level)
                                level = comps[c];
                            divsOnLevelNames.push_back(comps[c]->mName);
                        }
                    }
                    if (!divsOnLevelNames.empty())
                        levelName = FifamNames::GetLeagueLevelName(divsOnLevelNames, i + 1);
                }
                if (level) {
                    auto &levelNames = country->mLeagueLevelNames.emplace_back();
                    FifamTrSetAll<String>(levelNames, FifamNames::LimitName(levelName, 28));
                    country->mLeagueLevels[i].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                    country->mLeagueLevels[i].mNumNonEUPlayersAllowed = 0;
                    country->mLeagueLevels[i].mNumRelegatedTeams = 0;


                    country->mLeagueLevels[i].mRating = (UChar)(roundf((
                        (Float)level->mRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));
                }
                if (i == 0)
                    country->mAveragePlayerRating = country->mLeagueLevels[i].mRating;
            }
            // add leagues
            mNextFreeUID[country->mId - 1] = 0x3001;
            mNumTeamsInLeagueSystem[country->mId - 1] = 0;
            for (UShort i = 0; i < comps.size(); i++) {
                auto div = comps[i];
                FifamCompID leagueID = { (UChar)country->mId, FifamCompType::League, i };
                FifamCompID poolID = { (UChar)country->mId, FifamCompType::Pool, i };
                String compName;
                if (div->mName.length() < 64)
                    compName = div->mName;
                else
                    compName = div->mShortName;
                auto pool = mFifamDatabase->CreateCompetition(FifamCompDbType::Pool, poolID, compName)->AsPool();
                auto league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, leagueID, compName)->AsLeague();
                league->mCompetitionLevel = div->mLevel;
                league->mLeagueLevel = div->mLevel;
                league->mNumRounds = 2;
                league->mNumSubsAllowed = 3;
                league->mNumTeams = div->mTeams;
                league->mRoundType = FifamRoundID::Group1;
                league->mAttendanceMp = 50;
                league->mTransferMarketMp = 50;
                league->mTeams.resize(div->mTeams);

                // instructions
                pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(league, 1, league->mNumTeams));
                if ((i + 1) == comps.size())
                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                league->mInstructions.PushBack(new FifamInstruction::GET_POOL(pool, 0, league->mNumTeams));

                // add teams
                auto comp = mFoomDatabase->get<foom::comp>(div->mID);
                if (!comp)
                    continue;
                UInt leagueClubCounter = 0;

                for (auto entry : comp->mVecTeams) {
                    foom::club *team = (foom::club *)entry;
                    FifamClub *club = nullptr;
                    foom::club *mainTeam = nullptr; // for reserve teams
                    FifamClubTeamType teamType = FifamClubTeamType::First;
                    bool createNewClub = false;
                    bool isExtinct = team->mExtinct;
                    if (team->mConverterData.mParentClub)
                        isExtinct = team->mConverterData.mParentClub->mExtinct;
                    if (isExtinct) {
                        Error(L"Extinct club in the league\nClub: '%s'\nLeague: '%s'",
                            team->mName.c_str(), div->mName.c_str());
                    }
                    else if (team->mConverterData.mFifamClub) {
                        Message(Utils::Format(L"Team already present in other league\nClub: '%s'\nLeague: '%s'",
                            team->mName.c_str(), div->mName.c_str()));
                        club = (FifamClub *)team->mConverterData.mFifamClub;
                        if (team->mConverterData.mParentClub)
                            teamType = FifamClubTeamType::Reserve;
                    }
                    else if (team->mConverterData.mParentClub) {
                        if (!team->mConverterData.mParentClub->mConverterData.mFifamClub) {
                            Error(L"Reserve club appears before the first team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                team->mConverterData.mParentClub->mName.c_str(), team->mName.c_str(), div->mName.c_str());
                        }
                        else {
                            if (!team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB) {
                                club = (FifamClub *)team->mConverterData.mParentClub->mConverterData.mFifamClub;
                                teamType = FifamClubTeamType::Reserve;
                                team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB = team;
                                team->mConverterData.mParentClubInDB = team->mConverterData.mParentClub;
                            }
                            else {
                                team->mConverterData.mParentClub->mConverterData.mChildClubs.erase(std::remove_if(
                                    team->mConverterData.mParentClub->mConverterData.mChildClubs.begin(),
                                    team->mConverterData.mParentClub->mConverterData.mChildClubs.end(),
                                    [=](foom::club::converter_data::child_club const &a) {
                                    return a.mClub == team;
                                }), team->mConverterData.mParentClub->mConverterData.mChildClubs.end());
                                mainTeam = team->mConverterData.mParentClub;
                                team->mConverterData.mParentClub = nullptr;
                                createNewClub = true;
                                //Error(L"Reserve club already present for team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                //    team->mConverterData.mParentClub->mName.c_str(), team->mName.c_str(), div->mName.c_str());
                            }
                        }
                    }
                    else
                        createNewClub = true;
                    if (createNewClub)
                        club = CreateAndConvertClub(team, mainTeam, country, div);
                    // put team to league
                    if (club) {
                        league->mTeams[leagueClubCounter].mPtr = club;
                        league->mTeams[leagueClubCounter].mTeamType = teamType;
                        leagueClubCounter++;
                    }
                }

                league->GenerateFixtures();
                league->GenerateCalendar(13, 0, 0, 0);

                mNumTeamsInLeagueSystem[country->mId - 1] += div->mTeams;
            }
        }
    }

    // create spare teams (non-league teams)
    Array<Vector<foom::club *>, FifamDatabase::NUM_COUNTRIES> spareClubs;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // team is not already added, is active, has associated nation, not a reserve team
        if (!c.mConverterData.mFifamClub && !c.mExtinct && c.mNation && !c.mConverterData.mParentClub) {
            // if proper country ID
            auto country = mFifamDatabase->GetCountry(c.mNation->mConverterData.mFIFAManagerID);
            if (country)
                spareClubs[country->mId - 1].push_back(&c);
        }
    }

    for (auto &country : mFifamDatabase->mCountries) {
        if (country) {
            if (!spareClubs[country->mId - 1].empty()) {
                std::sort(spareClubs[country->mId - 1].begin(), spareClubs[country->mId - 1].end(), [](foom::club *a, foom::club *b) {
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
                UInt numSpareClubsToAdd = mNumTeamsInLeagueSystem[country->mId - 1];
                UInt maxClubs = gameId >= 10 ? 1000 : 500;
                if (numSpareClubsToAdd == 0)
                    numSpareClubsToAdd = 8;
                else {
                    if (numSpareClubsToAdd >= maxClubs)
                        numSpareClubsToAdd = maxClubs;
                    else {
                        numSpareClubsToAdd = numSpareClubsToAdd; // set num of spare teams here
                        if (numSpareClubsToAdd >= maxClubs)
                            numSpareClubsToAdd = maxClubs;
                    }
                }
                //numSpareClubsToAdd = 1023 - mNumTeamsInLeagueSystem[country->mId - 1];
                if (numSpareClubsToAdd > spareClubs[country->mId - 1].size())
                    numSpareClubsToAdd = spareClubs[country->mId - 1].size();
                for (UInt i = 0; i < numSpareClubsToAdd; i++) {
                    foom::club *team = spareClubs[country->mId - 1][i];
                    CreateAndConvertClub(team, team, country, nullptr);
                }
            }
        }
    }

    
    // finish club conversion
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        // team is present in db
        if (team.mConverterData.mFifamClub) {
            FifamClub *dst = (FifamClub *)team.mConverterData.mFifamClub;
            FifamCountry *country = dst->mCountry;
            // friendship club
            foom::club *bestFriendshipClub = nullptr;
            Int bestFriendshipRep = 0;
            // for all affiliated clubs
            for (auto &a : team.mVecAffiliations) {
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
                UInt numRivalClubs = Utils::Min(4, rivalClubs.size());
                for (UInt i = 0; i < numRivalClubs; i++)
                    dst->mRivalClubs.push_back((FifamClub *)(rivalClubs[i]->mRivalClub->mConverterData.mFifamClub));
            }

            // TODO: extract isBasque flag from FooM
            // TODO: reduce unused parameters in fm_players
            // TODO: (not sure) export "days at club" information for players
            // TODO: fix hero calculation
            // TODO: process loans and other things

            // create players
            for (auto &p : team.mVecContractedPlayers) {
                FifamPlayer *player = CreateAndConvertPlayer(p, dst);
                if (player->GetAge(GetCurrentDate()) <= 18)
                    player->mInYouthTeam = true;
                else {
                    if (!dst->mCaptains[0] && p == team.mCaptain) {
                        player->mIsCaptain = true;
                        dst->mCaptains[0] = player;
                    }
                }
                if (p->mContract.mSquadNumber > 0)
                    player->mShirtNumberFirstTeam = p->mContract.mSquadNumber;
            }
            for (auto &childClub : team.mConverterData.mChildClubs) {
                for (auto &p : childClub.mClub->mVecContractedPlayers) {
                    FifamPlayer *player = CreateAndConvertPlayer(p, dst);
                    if (player->GetAge(GetCurrentDate()) <= 18)
                        player->mInYouthTeam = true;
                    else {
                        player->mInReserveTeam = true;
                        if (!dst->mCaptains[1] && p == childClub.mClub->mCaptain)
                            dst->mCaptains[0] = player;
                    }
                }
            }
        }
    }

    // create free agents
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (!p.mConverterData.mFifamPerson && !p.mContract.mClub && p.mCurrentAbility >= 100) {
            FifamPlayer *player = CreateAndConvertPlayer(&p, nullptr);
        }
    }

    // player relations
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
            // TODO: unite with Staff
            if (!p.mVecFavouriteClubs.empty()) {
                Int maxRep = 0;
                FifamClub *bestFavouriteClub = nullptr;
                for (auto &f : p.mVecFavouriteClubs) {
                    if (f.mClub && f.mLevel == 100 && f.mClub->mConverterData.mFifamClub) {
                        if (!bestFavouriteClub || f.mClub->mReputation > maxRep) {
                            bestFavouriteClub = (FifamClub *)f.mClub->mConverterData.mFifamClub;
                            maxRep = f.mClub->mReputation;
                        }
                    }
                }
                if (bestFavouriteClub)
                    player->mFavouriteClub = bestFavouriteClub;
            }
            if (!p.mVecDislikedClubs.empty()) {
                Int maxRep = 0;
                FifamClub *bestDislikedClub = nullptr;
                for (auto &d : p.mVecDislikedClubs) {
                    if (d.mClub && d.mLevel == 100 && d.mClub->mConverterData.mFifamClub) {
                        if (!bestDislikedClub || d.mClub->mReputation > maxRep) {
                            bestDislikedClub = (FifamClub *)d.mClub->mConverterData.mFifamClub;
                            maxRep = d.mClub->mReputation;
                        }
                    }
                }
                if (bestDislikedClub)
                    player->mWouldnSignFor = bestDislikedClub;
            }
        }
    }

    // add players to national teams
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
            UInt numPlayersInNationalTeam = Utils::Min(25, n.size());
            for (UInt i = 0; i < numPlayersInNationalTeam; i++)
                ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mCurrentlyInNationalTeam = true;
        }
    }

    // convert referees
    for (auto &entry : mFoomDatabase->mOfficials) {
        auto &official = entry.second;
        if (!official.mFemale && official.mNation) {
            auto country = mFifamDatabase->GetCountry(official.mNation->mConverterData.mFIFAManagerReplacementID);
            if (country) {
                auto referee = country->AddReferee();
                ConvertReferee(referee, &official);
            }
        }
    }

    FifamVersion version = FifamDatabase::GetGameDbVersion(gameId);
    mFifamDatabase->Write(gameId, version.GetYear(), version.GetNumber(),
        Utils::Format(writeToGameFolder ?
            L"D:\\Games\\FIFA Manager %02d\\database" :
            L"fm_test\\database",
            gameId).c_str());

    delete mFifamDatabase;
    delete mFoomDatabase;
}

FifamClub *Converter::CreateAndConvertClub(foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div) {
    if (!mainTeam)
        mainTeam = team;
    FifamClub *club = mFifamDatabase->CreateClub(country);
    // uid
    club->mUniqueID = team->mConverterData.mFIFAManagerID;
    if (club->mUniqueID != 0) {
        UChar countryId = FifamUtils::GetCountryIDFromClubID(club->mUniqueID);
        if (countryId >= 1 && countryId <= FifamDatabase::NUM_COUNTRIES) {
            if (countryId != country->mId) {
                Error(L"Incorrect Club Country in UniqueID\nClub: '%s'\nUniqueID: %08X\nCountryId: %d\nIncorrectCountryId: %d",
                    team->mName.c_str(), club->mUniqueID, country->mId, countryId);
            }
        }
        else {
            Error(L"Incorrect Club UniqueID\nClub: '%s'\nUniqueID: %08X\nIncorrectCountryId: %d",
                team->mName.c_str(), club->mUniqueID, countryId);
        }
    }
    if (club->mUniqueID == 0)
        club->mUniqueID = (country->mId << 16) | mNextFreeUID[country->mId - 1]++;
    club->mFifaID = team->mConverterData.mFIFAID;

    ConvertClub(club, team, mainTeam, country, div);

    mFifamDatabase->AddClubToMap(club);
    country->mClubsMap[club->mUniqueID] = club;

    team->mConverterData.mFifamClub = club;

    return club;
}

void Converter::ConvertNationInfo(FifamCountry *dst, foom::nation *nation) {
    dst->mAssessmentData[5] = nation->mEuroCoeff1;
    dst->mAssessmentData[4] = nation->mEuroCoeff2;
    dst->mAssessmentData[3] = nation->mEuroCoeff3;
    dst->mAssessmentData[2] = nation->mEuroCoeff4;
    dst->mAssessmentData[1] = nation->mEuroCoeff5;
    dst->mAssessmentData[0] = nation->mEuroCoeff6;
    dst->mFifaRanking = nation->mRankingPoints;
    dst->mYearsForNaturalization = nation->mYearsToGainNationality;
    // national team
    if (nation->mYearAssociationFormed != 0)
        dst->mNationalTeam.mYearOfFoundation = nation->mYearAssociationFormed;

    // wins
    dst->mNumWorldCups = (UShort)nation->mConverterData.mWorldCupWins.size();
    dst->mNumWorldCupRunnersUp = (UShort)nation->mConverterData.mWorldCupFinals.size();
    dst->mNumContinentalChampions = (UShort)nation->mConverterData.mEuroCupWins.size();
    dst->mNumContinentalRunnersUp = (UShort)nation->mConverterData.mEuroCupFinals.size();

    ConvertKitsAndColors(&dst->mNationalTeam, nation->mVecKits, -1);
}

void Converter::ConvertClub(FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div) {
    // name
    if (team->mName.length() < 30)
        FifamTrSetAll(dst->mName, team->mName);
    else
        FifamTrSetAll(dst->mName, FifamNames::LimitName(team->mShortName, 29));
    dst->mName2 = dst->mName;
    // short name
    FifamTrSetAll(dst->mShortName, FifamNames::LimitName(team->mShortName, 10));
    dst->mShortName2 = dst->mShortName;
    // abbr
    FifamTrSetAll(dst->mAbbreviation, FifamNames::GetClubAbbr(team->mShortName));
    // year of foundation & traditional club
    if (team->mYearFounded <= 0)
        dst->mYearOfFoundation = 2000;
    else {
        dst->mYearOfFoundation = team->mYearFounded;
        if ((CURRENT_YEAR - 90) > dst->mYearOfFoundation)
            dst->mTraditionalClub = true;
    }
    // city
    if (team->mCity)
        FifamTrSetAll(dst->mCityName, FifamNames::LimitName(team->mCity->mName, 29));
    else
        dst->mCityName = country->mNationalTeam.mCityName;
    // latitude/longitude
    if (team->mLatitude != 0 && team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;

    // media pressure
    // TODO: read from reference file
    dst->mMediaPressure = FifamClubMediaPressure::Normal;
    // capital
    dst->mInitialCapital = foom::db::convert_money(team->mBalance);
    // transfer budget
    if (team->mTransferBudget > 0)
        dst->mTransferBudget = foom::db::convert_money(team->mTransferBudget);
    // joint-stock company
    dst->mJointStockCompany = team->mOwnershipType == 2; // Public Limited Company
    // rich guy controlled
    dst->mRichGuyControlled = team->mSugarDaddy >= 1 && team->mSugarDaddy <= 3; // Foreground, Background, Underwriter
    // sponsor
    foom::club::income *bestIncome = nullptr;
    Int bestIncomeValue = 0;
    foom::club::income *bestIndividualTvMoneyIncome = nullptr;
    Int bestIndividualTvMoneyValue = 0;
    for (auto &i : team->mVecIncomes) {
        if (i.mIncomeType >= 1 && i.mIncomeType <= 5) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor, Individual TV Deal
            if (i.mAmount > 0 && i.mStartDate.year <= CURRENT_YEAR && i.mEndDate.year > CURRENT_YEAR) { // is valid contract
                UShort duration = i.mEndDate.year - i.mStartDate.year;
                UShort yearsLeft = i.mEndDate.year - CURRENT_YEAR;
                auto amountPerYear = i.mAmount / duration;
                auto amountInGame = amountPerYear * yearsLeft;
                if (i.mIncomeType >= 1 && i.mIncomeType <= 4) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor
                    if (!bestIncome || amountInGame > bestIncomeValue) {
                        bestIncome = &i;
                        bestIncomeValue = amountInGame;
                    }
                }
                else if (i.mIncomeType == 5) { // Individual TV Deal
                    if (!bestIndividualTvMoneyIncome || amountInGame > bestIndividualTvMoneyValue) {
                        bestIndividualTvMoneyIncome = &i;
                        bestIndividualTvMoneyValue = amountInGame;
                    }
                }
            }
        }
    }
    if (bestIncome) {
        UShort duration = bestIncome->mEndDate.year - bestIncome->mStartDate.year;
        if (duration != 0) {
            dst->mSponsorAmount = foom::db::convert_money(bestIncome->mAmount / duration);
            dst->mSponsorDuration = bestIncome->mEndDate.year - CURRENT_YEAR;
            dst->mSpecialSponsor = bestIncome->mRenewIncome && !bestIncome->mFixedValue;
        }
    }
    if (bestIndividualTvMoneyIncome) {
        UShort duration = bestIndividualTvMoneyIncome->mEndDate.year - CURRENT_YEAR;
        if (duration != 0)
            dst->mIndividualTvMoney = foom::db::convert_money(bestIndividualTvMoneyIncome->mAmount / duration);
    }
    // Fans & attendance
    dst->mAverageAttendanceLastSeason = team->mAttendance;
    dst->mMaxAttendance = team->mMaximumAttendance;
    if (team->mNumberOfSeasonTicketHolders > 0)
        dst->mCountOfSoldSeasonTickets = team->mNumberOfSeasonTicketHolders;
    else
        dst->mCountOfSoldSeasonTickets = dst->mAverageAttendanceLastSeason / 2;
    dst->mPotentialFansCount = dst->mAverageAttendanceLastSeason * 2;
    if (team->mReputation > 7'000)
        dst->mPotentialFansCount += (team->mReputation - 7'000) * 50;
    if (dst->mPotentialFansCount > 250'000)
        dst->mPotentialFansCount = 250'000;

    // Stadium
    if (team->mStadium) {
        FifamTrSetAll(dst->mStadiumName, FifamNames::LimitName(team->mStadium->mName, 29));
        if (team->mStadium->mCapacity > 0) {
            Int seatingCapacity = team->mStadium->mSeatingCapacity;
            if (seatingCapacity == 0 || seatingCapacity > team->mStadium->mCapacity)
                seatingCapacity = team->mStadium->mCapacity;
            dst->mStadiumSeatsCapacity = seatingCapacity;
            dst->mStadiumStandingsCapacity = team->mStadium->mCapacity - seatingCapacity;
        }
    }
    if (FifamTr(dst->mStadiumName).empty())
        FifamTrSetAll(dst->mStadiumName, FifamNames::LimitName(FifamTr(dst->mCityName), 21) + L" Stadium");
    if ((dst->mStadiumSeatsCapacity + dst->mStadiumStandingsCapacity) == 0)
        dst->mStadiumSeatsCapacity = Utils::Clamp(dst->mAverageAttendanceLastSeason * 2, 50, 50'000);

    // Club facilities
    Int clubDefaultFacilitiesValue = team->mReputation / 500;
    if (clubDefaultFacilitiesValue < 1)
        clubDefaultFacilitiesValue = 1;
    Int clubDefaultFacilitiesForUnknown = clubDefaultFacilitiesValue;
    if (clubDefaultFacilitiesForUnknown > 1)
        clubDefaultFacilitiesForUnknown -= 1;


    // club facilities is based on training facilities (66,7%) and corporate facilities (33,3%)
    dst->mClubFacilities = Utils::MapTo(team->mTraining + team->mCorporateFacilities / 2, 1, 30, 0, 6);
    Int youthFacilities = team->mYouthFacilities;
    if (youthFacilities <= 0)
        youthFacilities = clubDefaultFacilitiesForUnknown;
    dst->mYouthCentre = Utils::MapTo(youthFacilities, 1, 20, 1, 10);
    Int youthCoaching = team->mYouthCoaching;
    if (youthCoaching <= 0)
        youthCoaching = clubDefaultFacilitiesForUnknown;
    dst->mYouthBoardingSchool = Utils::MapTo(youthCoaching, 1, 20, 0, 10);
    Int trainingGrounds = team->mTraining;
    if (trainingGrounds <= 0)
        trainingGrounds = clubDefaultFacilitiesForUnknown;
    dst->mTrainingGrounds = Utils::MapTo(trainingGrounds, 1, 20, 0, 5);
    Int corporateFacilities = team->mCorporateFacilities;
    if (corporateFacilities <= 0)
        corporateFacilities = 1;
    dst->mMerchandising = Utils::MapTo(corporateFacilities, 1, 20, 0, 5);
    dst->mMedicalDepartment = Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5);
    dst->mMedia = Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5);
    dst->mResearch = Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5);

    // TODO: other facilities
    // TODO: AI strategy

    // Youth players come from
    if (team->mBasedNation)
        dst->mYouthPlayersCountry.SetFromInt(team->mBasedNation->mConverterData.mFIFAManagerReplacementID);
    else
        dst->mYouthPlayersCountry.SetFromInt(country->mId);
    // Youth players are basques
    dst->mYouthPlayersAreBasques = team->is_basque();

    // League history
    for (auto &h : team->mVecTeamLeagueHistory) {
        if (h.mDivision && h.mDivision->mCompetitionLevel >= 1 && h.mDivision->mCompetitionLevel <= 3) {
            UInt level = h.mDivision->mCompetitionLevel - 1;
            dst->mLeagueTotalPoints[level] += h.mPoints;
            dst->mLeagueTotalWins[level] += h.mGamesWon;
            dst->mLeagueTotalDraws[level] += h.mGamesDrawn;
            dst->mLeagueTotalLoses[level] += h.mGamesLost;
            dst->mLeagueTotalGoals[level] += h.mGoalsFor;
            dst->mLeagueTotalGoalsAgainst[level] += h.mGoalsAgainst;
        }
    }
    for (UInt i = 0; i < 3; i++) {
        if (dst->mLeagueTotalPoints[i] < 0)
            dst->mLeagueTotalPoints[i] = 0;
    }
    // Records
    if (team->mRecordAttendance > 0 && team->mRecordAttendanceOpposition && team->mRecordAttendanceYear > 0) {
        dst->mHistory.mRecordAttendance.mAttendance = team->mRecordAttendance;
        dst->mHistory.mRecordAttendance.mOpponentName = FifamNames::LimitName(team->mRecordAttendanceOpposition->mName, 21);
        dst->mHistory.mRecordAttendance.mYear = team->mRecordAttendanceYear;
    }
    if (team->mRecordWinYear > 0 && team->mRecordWinOpposition) {
        dst->mHistory.mRecordHomeWin.mScore1 = team->mRecordWinTG;
        dst->mHistory.mRecordHomeWin.mScore2 = team->mRecordWinOG;
        dst->mHistory.mRecordHomeWin.mOpponentName = FifamNames::LimitName(team->mRecordWinOpposition->mName, 21);
        dst->mHistory.mRecordHomeWin.mYear = team->mRecordWinYear;
    }
    if (team->mRecordDefeatYear > 0 && team->mRecordDefeatOpposition) {
        dst->mHistory.mRecordAwayDefeat.mScore1 = team->mRecordDefeatTG;
        dst->mHistory.mRecordAwayDefeat.mScore2 = team->mRecordDefeatOG;
        dst->mHistory.mRecordAwayDefeat.mOpponentName = FifamNames::LimitName(team->mRecordDefeatOpposition->mName, 21);
        dst->mHistory.mRecordAwayDefeat.mYear = team->mRecordDefeatYear;
    }
    // Win years
    std::copy(team->mConverterData.mLeagueWins.begin(), team->mConverterData.mLeagueWins.end(), std::back_inserter(dst->mHistory.mLeagueWinYears));
    if (dst->mHistory.mLeagueWinYears.size() > 64) {
        dst->mHistory.mLeagueWinYears.resize(64);
        Error("Reached league win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mLeagueWinYears.size());
    }
    std::copy(team->mConverterData.mCupWins.begin(), team->mConverterData.mCupWins.end(), std::back_inserter(dst->mHistory.mCupWinYears));
    if (dst->mHistory.mCupWinYears.size() > 64) {
        dst->mHistory.mCupWinYears.resize(64);
        Error("Reached cup win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mCupWinYears.size());
    }
    std::copy(team->mConverterData.mLeagueCupWins.begin(), team->mConverterData.mLeagueCupWins.end(), std::back_inserter(dst->mHistory.mLeagueCupWinYears));
    if (dst->mHistory.mLeagueCupWinYears.size() > 64) {
        dst->mHistory.mLeagueCupWinYears.resize(64);
        Error("Reached league cup win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mLeagueCupWinYears.size());
    }
    std::copy(team->mConverterData.mSuperCupWins.begin(), team->mConverterData.mSuperCupWins.end(), std::back_inserter(dst->mHistory.mSuperCupsWinYears));
    if (dst->mHistory.mSuperCupsWinYears.size() > 64) {
        dst->mHistory.mSuperCupsWinYears.resize(64);
        Error("Reached supercup win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mSuperCupsWinYears.size());
    }
    std::copy(team->mConverterData.mChampionsLeagueWins.begin(), team->mConverterData.mChampionsLeagueWins.end(), std::back_inserter(dst->mHistory.mChampionsCupWinYears));
    if (dst->mHistory.mChampionsCupWinYears.size() > 64) {
        dst->mHistory.mChampionsCupWinYears.resize(64);
        Error("Reached championsleague win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mChampionsCupWinYears.size());
    }
    std::copy(team->mConverterData.mEuropaLeagueWins.begin(), team->mConverterData.mEuropaLeagueWins.end(), std::back_inserter(dst->mHistory.mEuroTrophyWinYears));
    if (dst->mHistory.mEuroTrophyWinYears.size() > 64) {
        dst->mHistory.mEuroTrophyWinYears.resize(64);
        Error("Reached europaleague win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mEuroTrophyWinYears.size());
    }
    std::copy(team->mConverterData.mToyotaCupWins.begin(), team->mConverterData.mToyotaCupWins.end(), std::back_inserter(dst->mHistory.mWorldChampionshipWinYears));
    if (dst->mHistory.mWorldChampionshipWinYears.size() > 64) {
        dst->mHistory.mWorldChampionshipWinYears.resize(64);
        Error("Reached toyotacup win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mWorldChampionshipWinYears.size());
    }
    std::copy(team->mConverterData.mClubWorldCupWins.begin(), team->mConverterData.mClubWorldCupWins.end(), std::back_inserter(dst->mHistory.mWorldClubChampionshipWinYears));
    if (dst->mHistory.mWorldClubChampionshipWinYears.size() > 64) {
        dst->mHistory.mWorldClubChampionshipWinYears.resize(64);
        Error("Reached worldclubcup win years limit\nClub: '%s'\nCount: %d", team->mName.c_str(), dst->mHistory.mWorldClubChampionshipWinYears.size());
    }

    // calculate undefined attributes
    UInt teamRep = team->mReputation;
    if (teamRep < 50)
        teamRep = 50;

    float teamRepMultiplier = 369.0f;
    UInt euroComponent = 0;
    if (country->mContinent == FifamContinent::Europe) {
        teamRepMultiplier = 400.0f;
        Float euroCoeff = (team->mEuroCoeff1 + team->mEuroCoeff2 + team->mEuroCoeff3 + team->mEuroCoeff4 + team->mEuroCoeff5) / 5.0f;
        euroComponent = (UInt)(floorf(euroCoeff * 0.2f));
    }
    UInt repComponent = 0;
    if (teamRep > 3000)
        repComponent = (UInt)(roundf((Float)(Utils::Max(teamRep - 3000, 500)) / 400.0f));
    dst->mInternationalPrestige = Utils::Min(20, euroComponent + repComponent);
    if (dst->mInternationalPrestige == 1 && teamRep < 1500)
        dst->mInternationalPrestige = 0;

    dst->mNationalPrestige = Utils::Clamp(dst->mInternationalPrestige + 3, 0, 20);

    if (div) {
        if (div->mLevel == 0 && dst->mNationalPrestige < 3)
            dst->mNationalPrestige = 3;
        else if (div->mLevel == 1 && dst->mNationalPrestige < 1)
            dst->mNationalPrestige = 1;
    }

    ConvertKitsAndColors(dst, mainTeam->mVecKits, team->mBadge);
}

void Converter::ConvertKitsAndColors(FifamClub *dst, Vector<foom::kit> const &kits, Int badgeType) {
    enum KitPart { Shirt, Icon, Text, Shorts, Socks };

    const foom::kit *tmpKitSets[5][2] = {};
    const foom::kit *tmpCompKitSets[5][2] = {};

    for (auto &k : kits) {
        if (k.mKit == 0 || k.mKit == 1) {
            if (k.mType >= 1 && k.mType <= 5 && k.mAlternativeKitNumber == 0 && (k.mYear == 0 || k.mYear == CURRENT_YEAR)) {
                if (k.mCompetition) {
                    if (!tmpCompKitSets[k.mType - 1][k.mKit])
                        tmpCompKitSets[k.mType - 1][k.mKit] = &k;
                }
                else {
                    if (!tmpKitSets[k.mType - 1][k.mKit])
                        tmpKitSets[k.mType - 1][k.mKit] = &k;
                }
            }
        }
    }

    const foom::kit *kitSets[5][2] = {};
    for (UInt k = 0; k < 2; k++) {
        for (UInt t = 0; t < 5; t++)
            kitSets[t][k] = tmpKitSets[t][k] ? tmpKitSets[t][k] : tmpCompKitSets[t][k];
    }

    bool textColorsUsed = false;

    const foom::kit *clubClrs = nullptr;
    if (badgeType != 11 && kitSets[2][0]) {
        clubClrs = kitSets[2][0];
        textColorsUsed = true;
    }
    else {
        for (UInt i = 0; i < 5; i++) {
            if (kitSets[i][0]) {
                clubClrs = kitSets[i][0];
                break;
            }
        }
    }

    Color backgroundClr, foregroundClr;

    // badge and colors
    if (clubClrs) {
        backgroundClr = Color(clubClrs->mBackground.r, clubClrs->mBackground.g, clubClrs->mBackground.b);
        foregroundClr = Color(clubClrs->mForeground.r, clubClrs->mForeground.g, clubClrs->mForeground.b);
        Color outlineClr = Color(clubClrs->mOutline.r, clubClrs->mOutline.g, clubClrs->mOutline.b);
        if (textColorsUsed)
            std::swap(foregroundClr, outlineClr);
        Color secondColor = foregroundClr;
        if (Color::Distance(backgroundClr, foregroundClr) < 100) {
            if (Color::Distance(backgroundClr, outlineClr) < 100) {
                auto white = Color::Distance(backgroundClr, { 255, 255, 255 });
                auto black = Color::Distance(backgroundClr, { 0, 0, 0 });
                if (black < white)
                    secondColor = { 255, 255, 255 };
                else
                    secondColor = { 0, 0, 0 };
            }
            else
                secondColor = outlineClr;
        }
        dst->mClubColour.Set(backgroundClr, secondColor);
    }
    else {
        UInt numTeamColors = FifamClub::mTeamColorsTable.size();
        if (numTeamColors > 0) {
            dst->mClubColour.SetFromTable(FifamClub::mTeamColorsTable, Random::Get(0, 27));
            if (Random::Get(1, 100) > 50) {
                backgroundClr = dst->mClubColour.second;
                foregroundClr = dst->mClubColour.first;
            }
            else {
                backgroundClr = dst->mClubColour.first;
                foregroundClr = dst->mClubColour.second;
            }
        }
    }
    dst->mClubColour2 = dst->mClubColour;
    dst->mMerchandiseColour = backgroundClr;
    if (Color::Distance(backgroundClr, Color(255, 255, 255)) < 100) {
        dst->mBackgroundColour = foregroundClr;
        dst->mHeaderColour = backgroundClr;
    }
    else {
        dst->mBackgroundColour = backgroundClr;
        dst->mHeaderColour = foregroundClr;
    }
    dst->mBadge.SetColorDefault(dst->mClubColour);

    if (badgeType != -1) {
        bool inverted = Random::Get(0, 1);
        switch (badgeType) {
        case 0:
            dst->mBadge.SetColor(FifamClubBadgeShape::Ball, 1, 1, inverted);
            break;
        case 1:
        case 13:
            dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, 7, inverted);
            break;
        case 2:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 5, Random::Get(1, 4), inverted);
            break;
        case 3:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 2, 1, inverted);
            break;
        case 4:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 6, 1, inverted);
            break;
        case 5:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 17, 1, inverted);
            break;
        case 6:
            dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, Random::Get(1, 2), false);
            break;
        case 7:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 2, inverted);
            break;
        case 8:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 8, 1, inverted);
            break;
        case 9:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 4, inverted);
            break;
        case 10:
            dst->mBadge.SetColor(FifamClubBadgeShape::Oval, 1, 1, false);
            break;
        case 11:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 10, 1, inverted);
            break;
        case 12:
            dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 9, 1, inverted);
            break;
        }
    }

    // home & away kits
    for (UInt i = 0; i < 2; i++) {
        auto &kit = dst->mKit.mSets[i];
        // kit - number
        if (kitSets[KitPart::Shirt][i])
            kit.mShirtNumberColor = kitSets[KitPart::Shirt][i]->mNumberColour;
        else {
            if (i == 0)
                kit.mShirtNumberColor = foregroundClr;
            else
                kit.mShirtNumberColor = backgroundClr;
        }
        // kit - armband
        kit.mCaptainArmbandColor = kit.mShirtNumberColor;
        // kit - socks
        kit.mSocks = 1;
        if (i == 0)
            kit.mSocksColors = { backgroundClr, backgroundClr };
        else
            kit.mSocksColors = { foregroundClr, foregroundClr };
        if (kitSets[KitPart::Socks][i]) {
            auto &set = kitSets[KitPart::Socks][i];
            switch (set->mKitStyle) {
            case 1:
            case 3:
            case 14:
            case 15:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mBackground };
                break;
            case 2:
            case 4:
            case 5:
            case 7:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 6:
            case 11:
            case 12:
                kit.mSocks = 4;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 8:
            case 9:
            case 13:
                kit.mSocks = 2;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 10:
                kit.mSocks = 3;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            }
        }
        // kit - shorts
        kit.mShorts = 1;
        if (i == 0)
            kit.mShortsColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShortsColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shorts][i]) {
            auto &set = kitSets[KitPart::Shorts][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShorts = 1;
                kit.mShortsColors = { set->mBackground, set->mBackground, set->mBackground };
                break;
            case 2:
                kit.mShorts = 6;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 3:
            case 4:
                kit.mShorts = 7;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 5:
                kit.mShorts = 4;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                kit.mShorts = 3;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            }
        }
        // kit - shirts
        kit.mShirt = 4;
        if (i == 0)
            kit.mShirtColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShirtColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shirt][i]) {
            auto &set = kitSets[KitPart::Shirt][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 2:
                kit.mShirt = 46;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 3:
            case 48:
                if (set->mBackOfShirtStyle == 2) // Plain
                    kit.mShirt = 14;
                else
                    kit.mShirt = 41;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 4:
            case 49:
            case 51:
                kit.mShirt = 48;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 5:
                kit.mShirt = 37;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 6:
                kit.mShirt = 40;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 7:
                kit.mShirt = 49;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 8:
            case 38:
                kit.mShirt = 38;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 9:
                kit.mShirt = 36;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 10:
                kit.mShirt = 30;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 11:
            case 37:
                kit.mShirt = 19;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 12:
            case 55:
                kit.mShirt = 57;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 13:
                kit.mShirt = 21;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 14:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 15:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 16:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 17:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 18:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 19:
                kit.mShirt = 60;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 20:
            case 39:
                kit.mShirt = 39;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 21:
                kit.mShirt = 22;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 22:
            case 23:
            case 31:
            case 32:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 24:
            case 33:
                kit.mShirt = 12;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 25:
                kit.mShirt = 9;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 26:
            case 27:
                kit.mShirt = 52;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 28:
                kit.mShirt = 47;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 29:
                kit.mShirt = 11;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 30:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 34:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 35:
                kit.mShirt = 8;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 36:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mBackground };
                break;
            case 40:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 41:
                kit.mShirt = 63;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mOutline };
                break;
            case 42:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 43:
                kit.mShirt = 13;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 44:
                kit.mShirt = 65;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 45:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 46:
            case 47:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 50:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 52:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 53:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 54:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 56:
                kit.mShirt = 12;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            }
        }

        Color shirtBackColor = FifamKit::GetShirtBackColor(kit.mShirt, kit.mShirtColors[0], kit.mShirtColors[1], kit.mShirtColors[2]);
        if (Color::Distance(kit.mShirtNumberColor, shirtBackColor) < 100) {
            if (Color::Distance(kit.mShirtNumberColor, foregroundClr) < 100)
                kit.mShirtNumberColor = backgroundClr;
            else
                kit.mShirtNumberColor = foregroundClr;
        }
    }
}

Int OriginalAttrValue(Int attr) {
    return (Int)ceilf((Float)attr / 5.0f);
}

Int Converter::ConvertPlayerAttribute(Int attr) {
    Int originalAttr = OriginalAttrValue(attr);
    if (originalAttr == 0)
        return 1;
    if (originalAttr > 20)
        originalAttr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        {  5, 29 }, // 1
        { 30, 39 }, // 2
        { 40, 49 }, // 3
        { 50, 54 }, // 4
        { 55, 59 }, // 5
        { 60, 62 }, // 6
        { 63, 64 }, // 7
        { 64, 65 }, // 8
        { 66, 67 }, // 9
        { 68, 69 }, // 10
        { 70, 71 }, // 11
        { 72, 73 }, // 12
        { 74, 76 }, // 13
        { 77, 79 }, // 14
        { 82, 83 }, // 15
        { 84, 85 }, // 16
        { 86, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    if (fmRatingAry[originalAttr - 1].first == fmRatingAry[originalAttr - 1].second)
        return fmRatingAry[originalAttr - 1].first;
    return Random::Get(fmRatingAry[originalAttr - 1].first, fmRatingAry[originalAttr - 1].second);
}

FifamPlayer *Converter::CreateAndConvertPlayer(foom::player *p, FifamClub *club) {
    if (!p->mNation) {
        Error(L"Player without nation\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *playerCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!playerCountry) {
        Error(L"Player without associated country\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *playerOriginalCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerID);
    FifamPlayer *player = mFifamDatabase->CreatePlayer(club, mPersonIdCounter++);
    player->mIsRealPlayer = true;
    player->mIsBasque = p->mIsBasque;
    // names
    player->mFirstName = FifamNames::LimitPersonName(p->mFirstName, 15);
    player->mLastName = FifamNames::LimitPersonName(p->mSecondName, 19);
    if (!p->mCommonName.empty())
        player->mPseudonym = FifamNames::LimitPersonName(p->mCommonName, (mCurrentGameId > 7) ? 29 : 19);
    // nationality
    Vector<UInt> playerNations;
    if (p->mNation)
        playerNations.push_back(p->mNation->mConverterData.mFIFAManagerReplacementID);
    for (auto &n : p->mVecSecondNations) {
        if (n->mConverterData.mFIFAManagerID != 0 && !Utils::Contains(playerNations, n->mConverterData.mFIFAManagerID))
            playerNations.push_back(n->mConverterData.mFIFAManagerID);
    }
    if (playerNations.empty())
        player->mNationality[0] = FifamNation::England;
    else {
        player->mNationality[0].SetFromInt(playerNations[0]);
        if (playerNations.size() > 1)
            player->mNationality[1].SetFromInt(playerNations[1]);
    }
    // languages
    Vector<Pair<UInt, UInt>> playerLanguages;
    if (p->mLanguage && p->mLanguage->mConverterData.mFIFAManagerID != 0)
        playerLanguages.emplace_back(p->mLanguage->mConverterData.mFIFAManagerID, 99);
    else
        playerLanguages.emplace_back(playerCountry->mLanguages[0].ToInt(), 99);
    for (auto &l : p->mVecLanguages) {
        if (l.mProficiency >= 5 && !l.mCannotSpeakLanguage && l.mLanguage->mConverterData.mFIFAManagerID != 0) {
            if (std::count_if(playerLanguages.begin(), playerLanguages.end(), [&](Pair<UInt, UInt> const &a) {
                return a.first == l.mLanguage->mConverterData.mFIFAManagerID;
            }) == 0)
            {
                playerLanguages.emplace_back(l.mLanguage->mConverterData.mFIFAManagerID, l.mProficiency);
            }
        }
    }
    std::sort(playerLanguages.begin(), playerLanguages.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
        return a.second > b.second;
    });
    UInt numLanguages = Utils::Min(4, playerLanguages.size());
    for (UInt i = 0; i < numLanguages; i++)
        player->mLanguages[i].SetFromInt(playerLanguages[i].first);
    // birthdate
    if (p->mDateOfBirth > FmEmptyDate())
        player->mBirthday = p->mDateOfBirth;
    else
        player->mBirthday = FifamPlayerGenerator::GetRandomPlayerBirthday(CURRENT_YEAR);

    UInt age = player->GetAge(GetCurrentDate());

    // foot
    player->mLeftFoot = Utils::MapTo(p->mLeftFoot, 1, 100, 0, 4);
    player->mRightFoot = Utils::MapTo(p->mRightFoot, 1, 100, 0, 4);
    if (player->mLeftFoot != 4 && player->mRightFoot != 4)
        player->mRightFoot = 4;

    // appearance

    player->mHeight = p->mHeight;
    player->mWeight = p->mWeight;

    UInt randomShoeType = Random::Get(1, 99);
    if (randomShoeType > 66)
        player->mShoeType = FifamShoeType::Blue;
    else if (randomShoeType > 33)
        player->mShoeType = FifamShoeType::White;
    else
        player->mShoeType = FifamShoeType::Black;



    // potential
    UInt maxTalent = 9;
    UChar *potantialAbilityRanges = nullptr;
    static UChar potantialAbilityRanges1to10[9] = { 35, 55, 75, 95, 116, 135, 155, 169, 190 };
    potantialAbilityRanges = potantialAbilityRanges1to10;
    player->mTalent = 0;
    for (UInt i = 0; i < maxTalent; i++) {
        if (p->mPotentialAbility >= potantialAbilityRanges[maxTalent - 1 - i]) {
            player->mTalent = maxTalent - i;
            break;
        }
    }

    Vector<Pair<Int, FifamPlayerPosition>> playerPositions = {
        { p->mGoalkeeper, FifamPlayerPosition::GK },
        { p->mSweeper, FifamPlayerPosition::SW },
        { p->mDefenderLeft, FifamPlayerPosition::LB },
        { p->mDefenderCentral, FifamPlayerPosition::CB },
        { p->mDefenderRight, FifamPlayerPosition::RB },
        { p->mDefensiveMidfielder, FifamPlayerPosition::DM },
        { p->mWingBackLeft, FifamPlayerPosition::LWB },
        { p->mWingBackRight, FifamPlayerPosition::RWB },
        { p->mMidfielderLeft, FifamPlayerPosition::LM },
        { p->mMidfielderCentral, FifamPlayerPosition::CM },
        { p->mMidfielderRight, FifamPlayerPosition::RM },
        { p->mAttackingMidfielderLeft, FifamPlayerPosition::LW },
        { p->mAttackingMidfielderCentral, FifamPlayerPosition::AM },
        { p->mAttackingMidfielderRight, FifamPlayerPosition::RW },
        { p->mStriker, FifamPlayerPosition::ST }
    };
    // position
    auto bestPos = Utils::GetMaxElement<Int, FifamPlayerPosition>(playerPositions);
    if (bestPos.first != 20)
        Error(L"Player has no preferred position\nPlayer: %s\nBest pos: %s (%d)", p->mFullName.c_str(), bestPos.second.ToCStr(), bestPos.first);

    player->mMainPosition = bestPos.second;
    player->mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(player->mMainPosition);
    player->mPositionBias[player->mMainPosition.ToInt()] = 100;
    for (UInt i = 0; i < playerPositions.size(); i++) {
        Int value = playerPositions[i].first;
        Int bias = 40;
        if (value == 20)
            bias = 100;
        else if (value == 19)
            bias = 99;
        else if (value == 18)
            bias = 98;
        else if (value == 17)
            bias = Random::Get(96, 97);
        else if (value == 16)
            bias = Random::Get(94, 95);
        else if (value == 15)
            bias = Random::Get(92, 93);
        else if (value == 14)
            bias = Random::Get(90, 91);
        else if (value == 13)
            bias = Random::Get(86, 89);
        else if (value == 12)
            bias = Random::Get(82, 85);
        else if (value == 11)
            bias = Random::Get(78, 81);
        else if (value == 10)
            bias = Random::Get(74, 77);
        else if (value == 9)
            bias = Random::Get(70, 73);
        else if (value == 8)
            bias = Random::Get(66, 69);
        else if (value == 7)
            bias = Random::Get(62, 65);
        else if (value == 6)
            bias = Random::Get(58, 61);
        else if (value == 5)
            bias = Random::Get(54, 57);
        else if (value == 4)
            bias = Random::Get(50, 53);
        else if (value == 3)
            bias = Random::Get(46, 49);
        else if (value == 2)
            bias = Random::Get(41, 45);
        else if (value == 1)
            bias = 40;
        if (player->mPositionBias[playerPositions[i].second.ToInt()] < bias)
            player->mPositionBias[playerPositions[i].second.ToInt()] = (Float)bias;
    }

    // attributes
    struct ConvertedAttributes {
        Int mAggression = 0;
        Int mAnticipation = 0;
        Int mBravery = 0;
        Int mComposure = 0;
        Int mConcentration = 0;
        Int mConsistency = 0;
        Int mVision = 0;
        Int mDecisions = 0;
        Int mDetermination = 0;
        Int mDirtiness = 0;
        Int mFlair = 0;
        Int mImportantMatches = 0;
        Int mLeadership = 0;
        Int mMovement = 0;
        Int mPositioning = 0;
        Int mTeamWork = 0;
        Int mWorkRate = 0;
        Int mAcceleration = 0;
        Int mAgility = 0;
        Int mBalance = 0;
        Int mInjuryProneness = 0;
        Int mJumpingReach = 0;
        Int mNaturalFitness = 0;
        Int mPace = 0;
        Int mStamina = 0;
        Int mStrength = 0;
        Int mCorners = 0;
        Int mCrossing = 0;
        Int mDribbling = 0;
        Int mFinishing = 0;
        Int mFirstTouch = 0;
        Int mFreeKicks = 0;
        Int mHeading = 0;
        Int mLongShots = 0;
        Int mLongThrows = 0;
        Int mMarking = 0;
        Int mPassing = 0;
        Int mPenaltyTaking = 0;
        Int mTackling = 0;
        Int mTechnique = 0;
        Int mVersatility = 0;
        Int mAerialAbility = 0;
        Int mCommandOfArea = 0;
        Int mCommunication = 0;
        Int mEccentricity = 0;
        Int mHandling = 0;
        Int mKicking = 0;
        Int mOneOnOnes = 0;
        Int mReflexes = 0;
        Int mRushingOut = 0;
        Int mTendencyToPunch = 0;
        Int mThrowing = 0;
    } attr;

    attr.mAggression = ConvertPlayerAttribute(p->mAggression);
    attr.mAnticipation = ConvertPlayerAttribute(p->mAnticipation);
    attr.mBravery = ConvertPlayerAttribute(p->mBravery);
    attr.mComposure = ConvertPlayerAttribute(p->mComposure);
    attr.mConcentration = ConvertPlayerAttribute(p->mConcentration);
    attr.mConsistency = ConvertPlayerAttribute(p->mConsistency);
    attr.mVision = ConvertPlayerAttribute(p->mVision);
    attr.mDecisions = ConvertPlayerAttribute(p->mDecisions);
    attr.mDetermination = ConvertPlayerAttribute(p->mDetermination);
    attr.mDirtiness = ConvertPlayerAttribute(p->mDirtiness);
    attr.mFlair = ConvertPlayerAttribute(p->mFlair);
    attr.mImportantMatches = ConvertPlayerAttribute(p->mImportantMatches);
    attr.mLeadership = ConvertPlayerAttribute(p->mLeadership);
    attr.mMovement = ConvertPlayerAttribute(p->mMovement);
    attr.mPositioning = ConvertPlayerAttribute(p->mPositioning);
    attr.mTeamWork = ConvertPlayerAttribute(p->mTeamWork);
    attr.mWorkRate = ConvertPlayerAttribute(p->mWorkRate);
    attr.mAcceleration = ConvertPlayerAttribute(p->mAcceleration);
    attr.mAgility = ConvertPlayerAttribute(p->mAgility);
    attr.mBalance = ConvertPlayerAttribute(p->mBalance);
    attr.mInjuryProneness = ConvertPlayerAttribute(p->mInjuryProneness);
    attr.mJumpingReach = ConvertPlayerAttribute(p->mJumpingReach);
    attr.mNaturalFitness = ConvertPlayerAttribute(p->mNaturalFitness);
    attr.mPace = ConvertPlayerAttribute(p->mPace);
    attr.mStamina = ConvertPlayerAttribute(p->mStamina);
    attr.mStrength = ConvertPlayerAttribute(p->mStrength);
    attr.mCorners = ConvertPlayerAttribute(p->mCorners);
    attr.mCrossing = ConvertPlayerAttribute(p->mCrossing);
    attr.mDribbling = ConvertPlayerAttribute(p->mDribbling);
    attr.mFinishing = ConvertPlayerAttribute(p->mFinishing);
    attr.mFirstTouch = ConvertPlayerAttribute(p->mFirstTouch);
    attr.mFreeKicks = ConvertPlayerAttribute(p->mFreeKicks);
    attr.mHeading = ConvertPlayerAttribute(p->mHeading);
    attr.mLongShots = ConvertPlayerAttribute(p->mLongShots);
    attr.mLongThrows = ConvertPlayerAttribute(p->mLongThrows);
    attr.mMarking = ConvertPlayerAttribute(p->mMarking);
    attr.mPassing = ConvertPlayerAttribute(p->mPassing);
    attr.mPenaltyTaking = ConvertPlayerAttribute(p->mPenaltyTaking);
    attr.mTackling = ConvertPlayerAttribute(p->mTackling);
    attr.mTechnique = ConvertPlayerAttribute(p->mTechnique);
    attr.mVersatility = ConvertPlayerAttribute(p->mVersatility);
    attr.mAerialAbility = ConvertPlayerAttribute(p->mAerialAbility);
    attr.mCommandOfArea = ConvertPlayerAttribute(p->mCommandOfArea);
    attr.mCommunication = ConvertPlayerAttribute(p->mCommunication);
    attr.mEccentricity = ConvertPlayerAttribute(p->mEccentricity);
    attr.mHandling = ConvertPlayerAttribute(p->mHandling);
    attr.mKicking = ConvertPlayerAttribute(p->mKicking);
    attr.mOneOnOnes = ConvertPlayerAttribute(p->mOneOnOnes);
    attr.mReflexes = ConvertPlayerAttribute(p->mReflexes);
    attr.mRushingOut = ConvertPlayerAttribute(p->mRushingOut);
    attr.mTendencyToPunch = ConvertPlayerAttribute(p->mTendencyToPunch);
    attr.mThrowing = ConvertPlayerAttribute(p->mThrowing);

    //attributes: general
    player->mAttributes.BallControl = (UChar)ceilf((attr.mTechnique + attr.mDribbling) / 2.0f);
    player->mAttributes.Volleys = (UChar)ceilf((attr.mTechnique + attr.mFinishing + attr.mLongShots) / 3.0f);
    player->mAttributes.Dribbling = attr.mDribbling;
    player->mAttributes.Finishing = attr.mFinishing;
    Int shotPowerBaseAttr = attr.mLongShots;
    if (attr.mFinishing > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mFinishing;
    if (attr.mKicking > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mKicking;
    player->mAttributes.ShotPower = Utils::Clamp((UChar)ceilf((shotPowerBaseAttr * 3 + attr.mStrength) / 4.0f) +
        (p->mShootsWithPower ? 10 : 0), 1, 99);
    player->mAttributes.LongShots = attr.mLongShots;
    player->mAttributes.Crossing = attr.mCrossing;
    player->mAttributes.Passing = attr.mPassing;
    player->mAttributes.LongPassing = (UChar)ceilf((attr.mPassing + attr.mVision) / 2.0f);
    player->mAttributes.Header = attr.mHeading;
    player->mAttributes.TacklingGeneral = attr.mTackling;
    player->mAttributes.TackleStanding = (UChar)ceilf((attr.mTackling + attr.mMarking) / 2.0f);
    player->mAttributes.TackleSliding = (UChar)ceilf((attr.mTackling + attr.mDecisions) / 2.0f);
    if (p->mDivesIntoTackles)
        player->mAttributes.TackleSliding += 5;
    else if (p->mDoesNotDiveIntoTackles)
        player->mAttributes.TackleSliding -= 10;
    player->mAttributes.TackleSliding = Utils::Clamp(player->mAttributes.TackleSliding, 1, 99);
    player->mAttributes.ManMarking = attr.mMarking;
    player->mAttributes.Technique = attr.mTechnique;
    player->mAttributes.Creativity = (UChar)ceilf((attr.mFlair + attr.mVision) / 2.0f);
    player->mAttributes.Flair = attr.mFlair;
    player->mAttributes.Touch = attr.mFirstTouch;
    // attributes: physical
    player->mAttributes.Balance = attr.mBalance;
    player->mAttributes.Acceleration = attr.mAcceleration;
    player->mAttributes.Pace = attr.mPace;
    player->mAttributes.Agility = attr.mAgility;
    player->mAttributes.Jumping = attr.mJumpingReach;
    player->mAttributes.Strength = attr.mStrength;
    player->mAttributes.Stamina = attr.mStamina;
    player->mAttributes.WorkRate = attr.mWorkRate;
    player->mAttributes.ForwardRuns = (UChar)ceilf((attr.mMovement + (UChar)ceilf((attr.mPace + attr.mAcceleration) / 2.0f)) / 2.0f);
    // attributes: mental
    player->mAttributes.PosOffensive = attr.mMovement;
    player->mAttributes.PosDefensive = attr.mPositioning;
    player->mAttributes.Vision = attr.mVision;
    player->mAttributes.Reactions = (UChar)ceilf((attr.mAnticipation + attr.mFirstTouch) / 2.0f);
    player->mAttributes.TacticAwareness = attr.mTeamWork;
    player->mAttributes.Aggression = attr.mAggression;
    player->mAttributes.Composure = attr.mComposure;
    player->mAttributes.Consistency = attr.mConsistency;
    player->mAttributes.Leadership = attr.mLeadership;
    player->mAttributes.Anticipation = attr.mAnticipation;
    player->mAttributes.Concentration = attr.mConcentration;
    player->mAttributes.Decision = attr.mDecisions;
    player->mAttributes.Determination = attr.mDetermination;
    player->mAttributes.TeamWork = attr.mTeamWork;
    player->mAttributes.Intimidation = attr.mAggression;
    // atributes : set pieces
    player->mAttributes.FreeKicks = attr.mFreeKicks;
    player->mAttributes.Corners = attr.mCorners;
    player->mAttributes.PenaltyShot = attr.mPenaltyTaking;
    // attributes: goalkeeping
    Int gkJumping = attr.mJumpingReach;
    if (attr.mAerialAbility > gkJumping)
        gkJumping = p->mAerialAbility;
    if (player->mMainPosition == FifamPlayerPosition::GK)
        player->mAttributes.Diving = (UChar)ceilf((attr.mReflexes + gkJumping) / 2.0f);
    else
        player->mAttributes.Diving = attr.mAerialAbility;
    player->mAttributes.GkCrosses = (UChar)ceilf((attr.mPassing + attr.mKicking) / 2.0f);
    player->mAttributes.Handling = attr.mHandling;
    player->mAttributes.Kicking = attr.mKicking;
    player->mAttributes.Positioning = (UChar)ceilf((attr.mRushingOut + attr.mCommandOfArea) / 2.0f);
    player->mAttributes.OneOnOne = attr.mOneOnOnes;
    player->mAttributes.Reflexes = attr.mReflexes;
    player->mAttributes.Punching = attr.mTendencyToPunch;
    player->mAttributes.ShotStopping = (UChar)ceilf((attr.mReflexes + attr.mOneOnOnes) / 2.0f);
    player->mAttributes.Throwing = attr.mThrowing;

    Int attributesBoost = 0;
    if (p->mCurrentAbility >= 195)
        attributesBoost = 7;
    else if (p->mCurrentAbility >= 180)
        attributesBoost = 6;
    else if (p->mCurrentAbility >= 170)
        attributesBoost = 5;
    else if (p->mCurrentAbility >= 160)
        attributesBoost = 4;
    else if (p->mCurrentAbility >= 155)
        attributesBoost = 3;
    else if (p->mCurrentAbility >= 150)
        attributesBoost = 2;
    else if (p->mCurrentAbility >= 145)
        attributesBoost = 1;
    if (attributesBoost > 0) {
        player->ForAllAttributes([&](UChar &attr, FifamPlayerAbilityID const &attrId) {
            if (!FifamPlayerAttributes::IsSetPiecesAttribute(attrId)) {
                if (FifamPlayerAttributes::IsGoalkeeperAttribute(attrId)) {
                    if (player->mMainPosition == FifamPlayerPosition::GK) {
                        attr = Utils::Min(99, attr + attributesBoost);
                    }
                }
                else {
                    if (player->mMainPosition != FifamPlayerPosition::GK) {
                        attr = Utils::Min(99, attr + attributesBoost);
                    }
                }
            }
        });
    }

    Int cfPosFirst = p->mStriker;
    Int cfPosSecond = p->mAttackingMidfielderCentral;
    if (cfPosSecond > cfPosFirst)
        std::swap(cfPosFirst, cfPosSecond);
    if (cfPosFirst == 20) {
        if (cfPosSecond >= 19)
            player->mPositionBias[FifamPlayerPosition::CF] = 100;
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(98, 99);
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(96, 97);
    }
    if (player->mPositionBias[FifamPlayerPosition::CF] == 100) {
        UChar currentLevel = FifamPlayerLevel::GetPlayerLevel(player, FifamPlayerLevel::GetBestStyleForPlayer(player), false);
        auto savedPos = player->mMainPosition;
        player->mMainPosition = FifamPlayerPosition::CF;
        if (FifamPlayerLevel::GetPlayerLevel(player, FifamPlayerLevel::GetBestStyleForPlayer(player), false) <= currentLevel)
            player->mMainPosition = savedPos;

    }
    player->mPlayingStyle = FifamPlayerLevel::GetBestStyleForPlayer(player);

    player->mTacticalEducation = Utils::MapTo(player->mAttributes.TacticAwareness, 1, 99, 0, 7);

    // history


    // contract
    if (club) {
        if (p->mContract.mDateJoined > FmEmptyDate())
            player->mContract.mJoined = p->mContract.mDateJoined;
        else
            player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        if (p->mContract.mWage > 0)
            player->mContract.mBasicSalary = foom::db::convert_money(p->mContract.mWage) * 52;
        if (p->mContract.mContractExpires.year <= CURRENT_YEAR)
            player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR + 1);
        else
            player->mContract.mValidUntil = p->mContract.mContractExpires;

        if (p->mContract.mWillLeaveAtEndOfContract)
            player->mNoContractExtension = true;
        if (p->mContract.mMatchHighestEarnerClause)
            player->mContract.mHighestPaidPlayer = true;
    }
    else {
        player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR + 1);
    }

    // player agent
    if (p->mCurrentAbility >= 100) {
        Int randVal = Random::Get(0, 100);
        if (randVal >= 65) {
            if (randVal >= 90)
                player->mPlayerAgent = FifamPlayerAgent::Lawyer;
            else
                player->mPlayerAgent = FifamPlayerAgent::Agent;
        }
        else
            player->mPlayerAgent = FifamPlayerAgent::Agency;
    }
    else
        player->mPlayerAgent = FifamPlayerAgent::None;

    // national team
    if (p->mInternationalRetirement && p->mInternationalRetirementDate <= GetCurrentDate())
        player->mRetiredFromNationalTeam = true;
    if (!player->mRetiredFromNationalTeam && playerOriginalCountry && p->mDeclaredForNation)
        mNationalTeamPlayers[playerOriginalCountry->mId - 1].push_back(p);
    player->mNationalTeamMatches = p->mInternationalApps;
    player->mNationalTeamGoals = p->mInternationalGoals;

    // experience
    UInt totalExperiencePoints = 0;
    totalExperiencePoints += p->mInternationalApps * 10;
    for (auto &h : p->mVecPlayingHistory) {
        if (!h.mYouthTeam && h.mApps > 0) {
            if (h.mDivision) {
                switch (h.mDivision->mCompetitionLevel) {
                case 1:
                    totalExperiencePoints += h.mApps * 5;
                    break;
                case 2:
                    totalExperiencePoints += h.mApps * 2;
                    break;
                default:
                    totalExperiencePoints += h.mApps;
                }
            }
            else
                totalExperiencePoints += h.mApps;
        }
    }
    if (totalExperiencePoints == 0) {
        if (age > 34)
            totalExperiencePoints = 500;
        else if (age > 30)
            totalExperiencePoints = 375;
        else if (age > 26)
            totalExperiencePoints = 200;
    }
    player->mGeneralExperience = Utils::MapTo(totalExperiencePoints, 0, 3000, 0, 18);

    // character
    if (p->mPressure >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::StrongNerves, true);
    else if (p->mPressure <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::WeakNerves, true);

    if (p->mProfessionalism >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Professionalism, true);
    else if (p->mProfessionalism <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::ScandalProne, true);

    if (p->mAmbition >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Ambition, true);
    else if (p->mAmbition <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::LacksAmbition, true);

    if (p->mAdaptability >= 18)
        player->mCharacter.Set(FifamPlayerCharacter::Adaptability, true);

    if (player->mLanguages[3] != FifamLanguage::None)
        player->mCharacter.Set(FifamPlayerCharacter::LanguageGenius, true);
    else if (player->mLanguages[1] == FifamLanguage::None && p->mAdaptability <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::DoesntLikeNewLang, true);

    if (p->mSportsmanship >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Fairness, true);
    else if (p->mSportsmanship <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::HardMan, true);

    if (OriginalAttrValue(p->mInjuryProneness) >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::InjuryProne, true);
    else if (OriginalAttrValue(p->mInjuryProneness) <= 4) // maybe 3
        player->mCharacter.Set(FifamPlayerCharacter::LittleInjuryProne, true);

    if (p->mControversy >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::MediaDarling, true);
    else if (p->mControversy <= 1)
        player->mCharacter.Set(FifamPlayerCharacter::UninterestedInMedia, true);

    if (p->mLoyalty >= 19) // maybe 17-18
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationHigh, true);
    else if (p->mLoyalty <= 5) // maybe 4
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationLow, true);

    if (p->mHitsFreekicksWithPower)
        player->mCharacter.Set(FifamPlayerCharacter::DrivenFreeKicks, true);

    if (player->mMainPosition == FifamPlayerPosition::GK) {
        if (p->mUsesLongThrowToStartCounterAttacks || OriginalAttrValue(p->mThrowing) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrowOuts, true);
    }
    else {
        if (p->mPossessesLongFlatThrow || OriginalAttrValue(p->mLongThrows) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);

        if (OriginalAttrValue(p->mVersatility) >= 17) // maybe 15-16
            player->mCharacter.Set(FifamPlayerCharacter::Flexibility, true);
        else if (OriginalAttrValue(p->mVersatility) <= 4) // maybe 3
            player->mCharacter.Set(FifamPlayerCharacter::Inflexibility, true);

        Int teamWork = OriginalAttrValue(p->mTeamWork);
        if (teamWork > 0 && teamWork <= 8) {
            Float teamPlayerRating = (Float)p->mCurrentAbility / teamWork;
            if (teamPlayerRating >= 13.4f) // magic
                player->mCharacter.Set(FifamPlayerCharacter::Egoist, true);
        }
    }

    if (!player->mCharacter.Check(FifamPlayerCharacter::Egoist)) {
        if (OriginalAttrValue(p->mTeamWork) >= 16) // maybe 15/17
            player->mCharacter.Set(FifamPlayerCharacter::Teamplayer, true);
    }

    if (OriginalAttrValue(p->mWorkRate) >= 18) // maybe 17/19
        player->mCharacter.Set(FifamPlayerCharacter::HighTrainingWorkRate, true);
    else {
        if (!player->mCharacter.Check(FifamPlayerCharacter::Professionalism)) {
            Int workRate = OriginalAttrValue(p->mWorkRate);
            if (workRate > 0 && workRate <= 8) {
                Float workRateRating = (Float)p->mCurrentAbility / workRate;
                if (workRateRating >= 17.0f) // magic
                    player->mCharacter.Set(FifamPlayerCharacter::LazyInTraining, true);
            }
        }
    }

    if (p->mTemperament >= 20) {
        if (p->mCurrentAbility >= 80)
            player->mCharacter.Set(FifamPlayerCharacter::Composure, true);
    }
    else if (p->mTemperament <= 5)
        player->mCharacter.Set(FifamPlayerCharacter::Temperament, true);

    player->mCharacter.Set(FifamPlayerCharacter::LifestyleIcon, IsIconicPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Introvert, IsIntrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Extrovert, IsExtrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::FansFavorite, IsFansFavouritePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Diva, IsSensitivePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::NeedsAttention, IsInsecurePlayer(p->mID));

    // hero
    UChar heroStatus = 0;
    if (OriginalAttrValue(p->mLeadership) > 17)
        heroStatus += OriginalAttrValue(p->mLeadership) - 17;
    if (OriginalAttrValue(p->mBravery) > 17)
        heroStatus += OriginalAttrValue(p->mBravery) - 17;
    if (player->mGeneralExperience == 18)
        heroStatus += 1;
    if (p->mWorldReputation >= 9500)
        heroStatus += 9;
    else if (p->mWorldReputation >= 8500)
        heroStatus += 3;
    else if (p->mWorldReputation >= 8000)
        heroStatus += 1;
    if (heroStatus > 0)
        player->mHeroStatus = Utils::Clamp(heroStatus, 0, 10);

    // starting conditions

    // retirement
    for (auto &i : p->mVecRetirements) {
        if (i.mType == 1 && i.mDate > GetCurrentDate() && i.mDate.year <= (CURRENT_YEAR + 1)) {
            player->mStartingConditions.mRetirement.Setup();
            break;
        }
    }
    // bans
    for (auto &b : p->mVecBans) {
        // league ban
        if (!player->mStartingConditions.mLeagueBan.mEnabled && b.mNumberMatches > 0 && b.mStartDate == FmEmptyDate() && b.mEndDate == FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 1 || b.mBanType == 3)
                player->mStartingConditions.mLeagueBan.Setup(b.mNumberMatches);
        }
        // ban until
        else if (!player->mStartingConditions.mBanUntil.mEnabled && b.mNumberMatches == 0 && b.mStartDate != FmEmptyDate() && b.mEndDate != FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 1 || b.mBanType == 3 || b.mBanType == 17 || b.mBanType == 18) {
                if (b.mStartDate <= GetCurrentDate() && b.mEndDate > GetCurrentDate())
                    player->mStartingConditions.mBanUntil.Setup(b.mEndDate);
            }
        }
    }

    // injury
    for (auto &i : p->mVecInjuries) {
        if (i.mInjury && i.mStartDate != FmEmptyDate() && i.mStartDate <= GetCurrentDate() && i.mEndDate > GetCurrentDate()) {
            if (i.mInjury->mConverterData.mFIFAManagerID != 0) {
                FifamPlayerInjuryType injuryType = FifamPlayerInjuryType::MakeFromInt(i.mInjury->mConverterData.mFIFAManagerID);
                player->mStartingConditions.mInjury.Setup(i.mStartDate, i.mEndDate, injuryType);
                break;
            }
        }
    }

    // debug
    player->mEmpicsId = p->mID;

    p->mConverterData.mFifamPerson = player;

    return player;
}

FifamStaff *Converter::CreateAndConvertStaff(foom::non_player *p, FifamClub *club) {
    if (!p->mNation) {
        Error(L"Staff without nation\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *staffCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!staffCountry) {
        Error(L"Staff without associated country\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *staffOriginalCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerID);
    FifamStaff *staff = mFifamDatabase->CreateStaff(club, mPersonIdCounter++);
    staff->mFirstName = FifamNames::LimitPersonName(p->mFirstName, 15);
    staff->mLastName = FifamNames::LimitPersonName(p->mSecondName, 19);
    if (!p->mCommonName.empty())
        staff->mPseudonym = FifamNames::LimitPersonName(p->mCommonName, (mCurrentGameId > 7) ? 29 : 19);
    if (p->mNation)
        staff->mNationality[0].SetFromInt(p->mNation->mConverterData.mFIFAManagerReplacementID);
    else
        staff->mNationality[0] = FifamNation::England;
    if (p->mLanguage && p->mLanguage->mConverterData.mFIFAManagerID != 0)
        staff->mLanguages[0].SetFromInt(p->mLanguage->mConverterData.mFIFAManagerID);
    else
        staff->mLanguages[0] = staffCountry->mLanguages[0];
    if (p->mDateOfBirth > FmEmptyDate())
        staff->mBirthday = p->mDateOfBirth;
    else
        staff->mBirthday = FifamPlayerGenerator::GetRandomPlayerBirthday(CURRENT_YEAR);

    // contract
    if (club->mIsNationalTeam) {
        if (p->mNationContract.mDateJoined > FmEmptyDate())
            staff->mJoinedClubDate = p->mNationContract.mDateJoined;
        else
            staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
    }
    else {
        if (p->mClubContract.mDateJoined > FmEmptyDate())
            staff->mJoinedClubDate = p->mClubContract.mDateJoined;
        else
            staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
    }
}

bool Converter::IsIconicPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        735216, // Cristiano Ronaldo
        7458500, // Messi
        19024412, // Neymar
        11133, // Buffon
        703963, // Pirlo
        142173, // Ibrahimovic
        7458272, // Sergio Ramos
        5124470 // Pique
    };
    return Utils::Contains(playerIDs, playerId);
}

bool Converter::IsIntrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        85081880, // Kante
        653054 // Modric
    };
    return Utils::Contains(playerIDs, playerId);
}

bool Converter::IsExtrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        19024412, // Neymar
        8832853, // Marcelo
        71000324, // Konoplyanka
        71081391 // Zinchenko
    };
    return Utils::Contains(playerIDs, playerId);
}

bool Converter::IsInsecurePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        43001238 // Balotelli
    };
    return Utils::Contains(playerIDs, playerId);
}

bool Converter::IsFansFavouritePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        735216, // Cristiano Ronaldo
        19024412, // Neymar
        11133, // Buffon
        142173, // Ibrahimovic
        98028755, // Salah
        76002390, // James
        5132312, // Bale
        78000335, // Luis Suarez
        8832853, // Marcelo
        8435089, // Benzema
        7458272, // Sergio Ramos
        824041, // Iniesta
        85028014, // Pogba
        2114068, // David Luiz
        315542, // Dani Alves
        35002219, // Ozil
        156772, // Kroos
        5124470, // Pique
        67086656, // Griezmann
        14044150, // Dybala
        8833628, // Thiago Silva
        310625, // Kaka
        6700151 // Casillas
    };
    return Utils::Contains(playerIDs, playerId);
}

bool Converter::IsSensitivePlayer(Int playerId) {
    return false;
}

void Converter::ConvertReferee(FifamReferee *dst, foom::official *official) {
    dst->mFirstName = FifamNames::LimitPersonName(official->mFirstName, 19);
    dst->mLastName = FifamNames::LimitPersonName(official->mSecondName, 19);
    if (official->mFIFACategory && official->mContinentalOfficial && official->mCurrentAbility > 140)
        dst->mType = FifamRefereeType::WorldClassReferee;
    else {
        dst->mType = FifamRefereeType::None;
        Vector<Pair<Int, FifamRefereeType>> elements;
        if (official->mPressure != 0) {
            if (official->mContinentalOfficial)
                elements.emplace_back(20 - official->mPressure, FifamRefereeType::Coward);
            elements.emplace_back(official->mPressure, FifamRefereeType::BraveDecisionMaker);
        }
        if (official->mDiscipline != 0)
            elements.emplace_back(official->mDiscipline, FifamRefereeType::SeverePerson);
        if (official->mAllowingFlow != 0)
            elements.emplace_back(official->mAllowingFlow, FifamRefereeType::DoesNotInterfereMuchInTheGame);
        if (!elements.empty()) {
            if (elements.size() == 1) {
                if (elements[0].first >= 15)
                    dst->mType = elements[0].second;
            }
            else {
                std::sort(elements.begin(), elements.end(), [](Pair<Int, FifamRefereeType> const &a, Pair<Int, FifamRefereeType> const &b) {
                    return a.first < b.first;
                });
                Int diff = elements[1].first - elements[0].first;
                if (diff >= 3)
                    dst->mType = elements[0].second;
            }
        }
    }
}
