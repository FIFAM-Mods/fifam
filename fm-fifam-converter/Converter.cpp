#include "Converter.h"
#include "FifamCompLeague.h"
#include "FifamNames.h"
#include "FifamUtils.h"
#include "Random.h"

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

void Converter::Convert(UInt gameId, Path const &originalDb, Path const &referenceDb, Bool writeToGameFolder) {
    // read reference database here

    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    mFifamDatabase = new FifamDatabase(gameId, originalDb);
    mFoomDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom");
    ReadAdditionalInfo(L"D:\\Projects\\fifam\\db");

    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // for all affiliated clubs
        for (auto &a : c.mVecAffiliations) {
            if (a.mAffiliatedClub && a.mIsMainClub) {
                if (a.mStartDate < Date(2, 7, CURRENT_YEAR) && (a.mEndDate == Date(1, 1, 1900) || a.mEndDate > Date(1, 7, CURRENT_YEAR))) {
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
                FifamCompID compID = { (UChar)country->mId, FifamCompType::League, i };
                String compName;
                if (div->mName.length() < 64)
                    compName = div->mName;
                else
                    compName = div->mShortName;
                auto league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, compID, compName)->AsLeague();
                league->mCompetitionLevel = div->mLevel;
                league->mLeagueLevel = div->mLevel;
                league->mNumRounds = 2;
                league->mNumSubsAllowed = 3;
                league->mNumTeams = div->mTeams;
                league->mRoundType = FifamRoundID::Group1;
                league->mAttendanceMp = 50;
                league->mTransferMarketMp = 50;
                league->mTeams.resize(div->mTeams);

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
                        club = CreateAndConvertClub(team, mainTeam, country);
                    // put team to league
                    if (club) {
                        league->mTeams[leagueClubCounter].mPtr = club;
                        league->mTeams[leagueClubCounter].mTeamType = teamType;
                        leagueClubCounter++;
                    }
                }

                league->GenerateFixtures();

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
                    if (a->mReputation < b->mReputation) return true;
                    if (b->mReputation < a->mReputation) return false;
                    return false;
                });
                UInt numSpareClubsToAdd = mNumTeamsInLeagueSystem[country->mId - 1];
                if (numSpareClubsToAdd == 0)
                    numSpareClubsToAdd = 8;
                else {
                    if (numSpareClubsToAdd >= 1000)
                        numSpareClubsToAdd = 0;
                    else {
                        numSpareClubsToAdd = numSpareClubsToAdd; // set num of spare teams here
                        if (numSpareClubsToAdd >= 1000)
                            numSpareClubsToAdd = 0;
                    }
                }
                if (numSpareClubsToAdd > spareClubs[country->mId - 1].size())
                    numSpareClubsToAdd = spareClubs[country->mId - 1].size();
                for (UInt i = 0; i < numSpareClubsToAdd; i++) {
                    foom::club *team = spareClubs[country->mId - 1][i];
                    CreateAndConvertClub(team, team, country);
                }
            }
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

FifamClub *Converter::CreateAndConvertClub(foom::club *team, foom::club *mainTeam, FifamCountry *country) {
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

    ConvertClub(club, team, mainTeam, country);

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
    Int prestige = (Int)ceilf((Float)nation->mReputation / 500.0f);
    dst->mNationalTeam.mInternationalPrestige = prestige;
    dst->mNationalTeam.mNationalPrestige = prestige;

    // wins
    dst->mNumWorldCups = (UShort)nation->mConverterData.mWorldCupWins.size();
    dst->mNumWorldCupRunnersUp = (UShort)nation->mConverterData.mWorldCupFinals.size();
    dst->mNumContinentalChampions = (UShort)nation->mConverterData.mEuroCupWins.size();
    dst->mNumContinentalRunnersUp = (UShort)nation->mConverterData.mEuroCupFinals.size();

    ConvertKitsAndColors(&dst->mNationalTeam, nation->mVecKits);
}

void Converter::ConvertClub(FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country) {
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
    // club prestige
    Int prestige = team->mReputation / 450;
    if (prestige > 20)
        prestige = 20;
    dst->mInternationalPrestige = prestige;
    // TODO change this
    dst->mNationalPrestige = prestige;
    // media pressure
    // TODO: read from reference file
    dst->mMediaPressure = FifamClubMediaPressure::Normal;
    // individual TV money

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
    // Fan Base 1st League
    if (team->mAttendance > 0)
        dst->mAverageAttendanceLastSeason = team->mAttendance;
    if (team->mNumberOfSeasonTicketHolders > 0)
        dst->mCountOfSoldSeasonTickets = team->mNumberOfSeasonTicketHolders;

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
    if ((dst->mStadiumSeatsCapacity + dst->mStadiumStandingsCapacity) == 0) {
        // TODO
    }
    // Club facilities
    dst->mClubFacilities = Utils::MapTo((team->mTraining + team->mCorporateFacilities) / 2, 0, 20, 0, 6);
    dst->mYouthCentre = Utils::MapTo(team->mYouthFacilities, 0, 20, 1, 10);
    dst->mYouthBoardingSchool = Utils::MapTo(team->mYouthCoaching, 0, 20, 0, 10);
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

    ConvertKitsAndColors(dst, mainTeam->mVecKits);
}

void Converter::ConvertKitsAndColors(FifamClub *dst, Vector<foom::kit> const &kits) {
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

    const foom::kit *clubClrs = nullptr;
    for (UInt i = 0; i < 5; i++) {
        if (kitSets[i][0]) {
            clubClrs = kitSets[i][0];
            break;
        }
    }

    Color backgroundClr;
    Color foregroundClr;

    // badge and colors
    if (clubClrs) {
        backgroundClr = Color(clubClrs->mBackground.r, clubClrs->mBackground.g, clubClrs->mBackground.b);
        foregroundClr = Color(clubClrs->mForeground.r, clubClrs->mForeground.g, clubClrs->mForeground.b);
        Color outlineClr = Color(clubClrs->mOutline.r, clubClrs->mOutline.g, clubClrs->mOutline.b);
        Color secondColor = foregroundClr;
        if (Color::Distance(backgroundClr, foregroundClr) < 200) {
            if (Color::Distance(backgroundClr, outlineClr) < 200) {
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
    dst->mBackgroundColour = backgroundClr;
    dst->mHeaderColour = foregroundClr;
    dst->mBadge.SetColorDefault(dst->mClubColour);

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
    }
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
