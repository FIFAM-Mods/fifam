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

#define DB_SIZE Full
const Bool READ_FOOM_PERSONS = true;
const Bool MAKE_CORRECTIONS_FOR_UPDATE = true;

void Converter::Convert(UInt gameId, Bool writeToGameFolder) {

}

void Converter::Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder, Bool fromFifaDatabase) {
    mCurrentGameId = gameId;
    mFromFifaDatabase = fromFifaDatabase;
    // read reference database here
    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;

    mFifamDatabase = new FifamDatabase(originalGameId, originalDb);

    FifamDatabase::mReadingOptions.mReadClubs = true;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = !fromFifaDatabase;

    mReferenceDatabase = new FifamDatabase(referenceGameId, referenceDb);

    mFifaDatabase = new FifaDatabase(L"D:\\Projects\\fifam\\db\\fifa");
    mFoomDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom", fromFifaDatabase? false : READ_FOOM_PERSONS, foom::db::db_size::DB_SIZE);

    Path infoPath = L"D:\\Projects\\fifam\\db";
    Path graphicsPath = L"D:\\Documents\\Sports Interactive\\Football Manager 2019\\graphics";
    Path contentPath = L"D:\\Projects\\fifam\\content";

    ReadAdditionalInfo(infoPath, gameId);
    appearanceGenerator.Read(infoPath / L"AppearanceDefs.sav");

    if (!mFromFifaDatabase) {
        std::wcout << L"Scanning badges folder..." << std::endl;
        for (auto const &i : recursive_directory_iterator(graphicsPath / L"dvx-logos" / L"clubs" / L"primary" / L"@2x")) {
            Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
            if (id > 0 && i.path().extension() == ".png")
                mAvailableBadges[id] = i.path();
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
                mFoomDatabase->mClubs[newClubId] = newClub;

                foom::club::reserve_team rt;
                rt.mReserveTeamType = r.mReserveTeamType;
                rt.mReserveClub = &mFoomDatabase->mClubs[newClubId];
                c.mVecReserveTeams.push_back(rt);

                r.mDivision->mVecTeams.push_back(&mFoomDatabase->mClubs[newClubId]);
            }
        }
    }

    if (MAKE_CORRECTIONS_FOR_UPDATE) {
        for (auto &entry : mFoomDatabase->mPlayers) {
            auto &p = entry.second;
            if (p.mLoan.mClub) {
                p.mLoan.mEndDate.year += 1;
                if (p.mLoan.mStartDate.year < CURRENT_YEAR) {
                    if (p.mLoan.mStartDate.year == (CURRENT_YEAR - 1)) {
                        if (p.mLoan.mStartDate.month <= 7)
                            p.mLoan.mStartDate.year += 1;
                        else
                            p.mLoan.mStartDate.Set(1, 7, CURRENT_YEAR);
                    }
                    else
                        p.mLoan.mStartDate.year += 1;
                }
            }
            if (p.mContract.mClub) {
                p.mContract.mContractExpires.year += 1;
                p.mContract.mDateJoined.year += 1;
            }
        }
    }

    std::wcout << L"Finding reserve clubs..." << std::endl;
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
                    else if ((a.mAffiliationType == 1 || a.mAffiliationType == 3 || a.mAffiliationType == 21) && a.mPlayersMoveFreely && a.mSameBoard) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second_affiliated;
                    }
                    else if (a.mAffiliationType == 5 || a.mAffiliationType == 7) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::third;
                    }
                    if (child) {
                        if (child->mConverterData.mParentClub) {
                        #ifdef SHOW_RESERVE_CLUBS_WARNINGS
                            Error(L"Affiliated B/C club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
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
                        Error(L"Reserve club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
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
            Error(L"Reserve club has reserve teams\nClub: '%s'", c.mName.c_str());
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
                    foom::nation *nationRunnerUp = (foom::nation *)h.mSecondPlaced;
                    if (c.mID == 1301385) { // World Cup
                        nation->mConverterData.mWorldCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mWorldCupFinals.insert(h.mYear);
                    }
                    else if (c.mType == 11) { // Main continental international finals
                        nation->mConverterData.mContinentalCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mContinentalCupFinals.insert(h.mYear);
                    }
                }
            }
        }
    }

    UInt MAX_LEAGUE_NAME_LENGTH = 63;
    UInt MAX_CUP_NAME_LENGTH = 63;
    UInt MAX_POOL_NAME_LENGTH = 58;
    UInt MAX_COMP_NAME_LENGTH = 29;

    if (gameId <= 7) {
        MAX_LEAGUE_NAME_LENGTH = 29;
        MAX_CUP_NAME_LENGTH = 29;
        MAX_POOL_NAME_LENGTH = 29;
        MAX_COMP_NAME_LENGTH = 29;
    }

    // convert nations, national teams, leagues
    std::wcout << L"Converting nations, national teams and leagues..." << std::endl;
    ConvertLeagues(gameId);

    // create spare teams (non-league teams)
    std::wcout << L"Creating spare clubs..." << std::endl;
    Array<Vector<foom::club *>, FifamDatabase::NUM_COUNTRIES> spareClubs;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // team is active, has associated nation, not a reserve team
        if (!c.mExtinct && c.mNation && !c.mConverterData.mParentClub) {
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
                UInt numSpareClubsToAdd = 8; // changed from 7
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
                else if (country->GetProperty<Bool>(L"SimulateLeague")
                    || country->mId == FifamNation::Canada || country->mId == FifamNation::Cameroon
                    || country->mId == FifamNation::Cote_d_Ivoire || country->mId == FifamNation::Ghana)
                {
                    numSpareClubsToAdd = 8;
                }
                if ((numSpareClubsToAdd + country->mClubs.size()) >= maxClubs)
                    numSpareClubsToAdd = maxClubs - country->mClubs.size();
                if (numSpareClubsToAdd > spareClubs[country->mId - 1].size())
                    numSpareClubsToAdd = spareClubs[country->mId - 1].size();
                for (UInt i = 0; i < numSpareClubsToAdd; i++) {
                    foom::club *team = spareClubs[country->mId - 1][i];
                    CreateAndConvertClub(gameId, team, team, country, nullptr, mNumTeamsInLeagueSystem[country->mId - 1] == 0);
                }
            }
        }
    }

    // setup club players
    std::wcout << L"Collecting club players..." << std::endl;

    UInt maxAllowedPlayers = 80;
    UInt maxAllowedPlayersWithYouth = 180;
    UInt minPlayersForFirstTeam = 25;
    UInt maxPlayersForFirstTeam = 29;
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
            if (loanClub && (player.mLoan.mEndDate <= GetCurrentDate() || loanClub == contractClub))
                loanClub = nullptr;
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
                //Error(L"Player is loaned and transferred at same time\nPlayer: %s\nPlayerID: %d", player.mFullName.c_str(), player.mID);
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
                        if (l == clubLeague) {
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

            std::sort(dst->mPlayers.begin(), dst->mPlayers.end(), [=](FifamPlayer *a, FifamPlayer *b) {
                auto levelA = GetPlayerLevel(a, true, gameId);
                auto levelB = GetPlayerLevel(b, true, gameId);
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
                if (!p.mContract.mClub && p.mCurrentAbility >= 110) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, &p, nullptr);
                }
            }
            else {
                if (p.mConverterData.mFutureClub) {
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
        if (IsConvertable(&p, gameId) && !p.mConverterData.mFifamPerson) {
            if (!p.mClubContract.mClub && !p.mNationContract.mNation && p.mCurrentAbility >= 100) {
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
                    FifamStaff *staff = CreateAndConvertStaff(&p, nullptr, bestPosition.second);
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
    std::wcout << L"Processing player history and experience..." << std::endl;
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson) {
            FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
            // history - validate years
            for (auto &h : p.mVecPlayingHistory) {
                if (h.mYear <= 0) {
                    Error(L"Wrong date in player history\nPlayer: %s\nPlayerID: %d", p.mFullName.c_str(), p.mID);
                    h.mYear = 1970;
                }
            };
            // history - sort history entries
            std::sort(p.mVecPlayingHistory.begin(), p.mVecPlayingHistory.end(), [](foom::player::playing_history const &a, foom::player::playing_history const &b) {
                if (a.mYear < b.mYear) return true;
                if (b.mYear < a.mYear) return true;
                return a.mOrder < b.mOrder;
            });

            // history - separate by years
            Map<Int, Vector<foom::player::playing_history *>> playerHistoryMap;
            for (auto &h : p.mVecPlayingHistory) {
                if (h.mYear <= CURRENT_YEAR && !h.mYouthTeam && h.mClub) { // ignore 'future' records and youth team
                    auto historyClub = GetTeamClubLink(h.mClub);
                    if (historyClub.IsValid() || h.mClub->mConverterData.mAdditionalHistoryReserveTeamType != 1)
                        playerHistoryMap[h.mYear].push_back(&h);
                }
            }

            auto GetPlayerHistorySeasonStartDate = [=](Int year) {
                if (year == CURRENT_YEAR)
                    return FifamDate(1, 1, CURRENT_YEAR);
                return FifamDate(1, 7, year);
            };

            auto GetPlayerHistorySeasonEndDate = [=](Int year, bool hasCurrentYear) {
                if (year == CURRENT_YEAR)
                    return FifamDate(30, 6, CURRENT_YEAR);
                else if (year == (CURRENT_YEAR - 1) && hasCurrentYear)
                    return FifamDate(31, 12, year);
                return FifamDate(30, 6, year + 1);
            };

            // history - fill player history
            foom::player::playing_history *lastEntry = nullptr;
            Vector<FifamPlayerHistoryEntry> history;
            bool hasCurrent = playerHistoryMap.count(CURRENT_YEAR) > 0;
            Int lastTransferFee = 0;
            for (auto const &e : playerHistoryMap) {
                UInt entryIndex = 0;
                for (auto &h : e.second) {
                    FifamPlayerHistoryEntry *last = nullptr;
                    if (!history.empty())
                        last = &history.back();
                    FifamPlayerHistoryEntry *curr = nullptr;
                    FifamClubLink historyClub;
                    if (h->mClub)
                        historyClub = GetTeamClubLink(h->mClub);
                    if (!historyClub.IsValid()) {
                        if (h->mClub->mConverterData.mAdditionalHistoryParentTeam && h->mClub->mConverterData.mAdditionalHistoryReserveTeamType == 0) {
                            if (h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub) {
                                historyClub = FifamClubLink(
                                    (FifamClub *)(h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub), FifamClubTeamType::Reserve);
                            }
                        }
                    }
                    bool createNewEntry = true;
                    bool sameClub = false;

                    if (last) {
                        if (historyClub.mPtr && historyClub.mPtr == last->mClub.mPtr) {
                            sameClub = true;
                            if (h->mOnLoan == last->mLoan)
                                createNewEntry = false;
                        }
                        else if (lastEntry && lastEntry->mClub && h->mClub) {
                            if (h->mClub == lastEntry->mClub || h->mClub->mConverterData.mParentClub == lastEntry->mClub || lastEntry->mClub->mConverterData.mParentClub == h->mClub) {
                                sameClub = true;
                                if (h->mOnLoan == last->mLoan)
                                    createNewEntry = false;
                            }
                        }
                    }
                    Date startDate = GetPlayerHistorySeasonStartDate(h->mYear);
                    Date endDate = GetPlayerHistorySeasonEndDate(h->mYear, hasCurrent);
                    if (e.second.size() > 1 && endDate.month != 12) {
                        if (entryIndex == (e.second.size() - 1))
                            startDate.Set(1, 1, endDate.year);
                        else
                            endDate.Set(31, 12, endDate.year - 1);
                    }
                    if (!createNewEntry) {
                        curr = last;
                        curr->mEndDate = endDate;
                    }
                    else {
                        curr = &history.emplace_back();
                        curr->mClub = FifamClubLink(historyClub.mPtr);
                        curr->mStartDate = startDate;
                        curr->mEndDate = endDate;
                        curr->mLoan = h->mOnLoan;
                    }
                    if (!h->mYouthTeam) {
                        bool firstTeam = true;
                        if (historyClub.IsValid())
                            firstTeam = historyClub.mTeamType == FifamClubTeamType::First;
                        else
                            firstTeam = !h->mClub || !h->mClub->mConverterData.mParentClub;
                        if (firstTeam) { // MAYBE: also check division level?
                            curr->mMatches += h->mApps;
                            curr->mGoals += h->mGoals;
                        }
                        else {
                            curr->mReserveMatches += h->mApps;
                            curr->mReserveGoals += h->mGoals;
                        }
                    }
                    if (h->mOnLoan) {
                        if (h->mTransferFee > 0)
                            curr->mTransferFee = foom::db::convert_money(h->mTransferFee);
                    }
                    else if (lastTransferFee > 0)
                        curr->mTransferFee = foom::db::convert_money(lastTransferFee);
                    if (!h->mOnLoan)
                        lastTransferFee = h->mTransferFee;
                    if (sameClub && !curr->mLoan && last->mLoan) {
                        //Int lastFee = last->mTransferFee;
                        //last->mTransferFee = curr->mTransferFee;
                        //curr->mTransferFee = lastFee;
                    }
                    lastEntry = h;
                    entryIndex++;
                }
            }

            if (!history.empty()) {
                if (player->mClub) {
                    FifamPlayerHistoryEntry &last = history.back();
                    if (last.mClub.mPtr != player->mClub || last.mLoan != player->mStartingConditions.mLoan.Enabled()) {
                        FifamPlayerHistoryEntry &curr = history.emplace_back();
                        curr.mClub = FifamClubLink(player->mClub);
                        curr.mStartDate = FifamDate(1, 7, CURRENT_YEAR);
                        curr.mEndDate = FifamDate(1, 7, CURRENT_YEAR);
                        curr.mLoan = player->mStartingConditions.mLoan.Enabled();
                        if (!curr.mLoan && lastTransferFee > 0)
                            curr.mTransferFee = foom::db::convert_money(lastTransferFee);
                    }
                    history.back().mStillInThisClub = true;
                }
            }

            // NOTE: check this
            if (p.mCurrentAbility >= 0) {
                for (auto const &h : history)
                    player->mHistory.mEntries.insert(h);
            }

            // national/international experience
            UInt totalLeagueMatches = 0;
            for (auto &careerEntry : history)
                totalLeagueMatches += careerEntry.mMatches + careerEntry.mReserveMatches;
            UInt totalInternationalMatches = player->mNationalTeamMatches;
            if (totalLeagueMatches > 450)
                player->mNationalExperience = 6;
            else if (totalLeagueMatches > 350)
                player->mNationalExperience = 5;
            else if (totalLeagueMatches > 250)
                player->mNationalExperience = 4;
            else if (totalLeagueMatches > 150)
                player->mNationalExperience = 3;
            else if (totalLeagueMatches > 50)
                player->mNationalExperience = 2;
            else if (totalLeagueMatches > 20)
                player->mNationalExperience = 1;
            else
                player->mNationalExperience = 0;

            if (p.mInternationalApps > 85)
                player->mInternationalExperience = 4;
            else if (p.mInternationalApps >= 55)
                player->mInternationalExperience = 3;
            else if (p.mInternationalApps >= 30)
                player->mInternationalExperience = 2;
            else if (p.mInternationalApps >= 10)
                player->mInternationalExperience = 1;
        }
    }

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
    std::wcout << L"Converting referess..." << std::endl;
    for (auto &entry : mFoomDatabase->mOfficials) {
        auto &official = entry.second;
        if (IsConvertable(&official, gameId) && official.mNation) {
            auto country = mFifamDatabase->GetCountry(official.mNation->mConverterData.mFIFAManagerReplacementID);
            if (country) {
                FifamReferee *referee = country->AddReferee();
                ConvertReferee(referee, &official);
            }
        }
    }

    // FIFA database conversion
    if (mFromFifaDatabase) {
        // referee
        FifaConverter::ConvertReferees(mFifamDatabase, mFifaDatabase);
        Set<FifaPlayer *> attachedPlayers;
        for (auto club : mFifamDatabase->mClubs) {
            if (club->mFifaID != 0) {
                FifaTeam *fifaTeam = mFifaDatabase->GetTeam(club->mFifaID);
                if (fifaTeam) {
                    if (fifaTeam->m_manager)
                        FifaConverter::ConvertManager(mFifamDatabase, club, fifaTeam->m_manager);
                    fifaTeam->ForAllPlayersEx([&](FifaPlayer &p, FifaPlayer::Position pos, UChar number) {
                        FifaConverter::ConvertPlayer(this, club, fifaTeam, &p, pos, number);
                        attachedPlayers.insert(&p);
                    });
                }
            }
        }
        mFifaDatabase->ForAllMalePlayers([&](FifaPlayer &p) {
            if (!Utils::Contains(attachedPlayers, &p)) {
                FifaConverter::ConvertPlayer(this, nullptr, nullptr, &p, FifaPlayer::POS_RES, 0);
            }
        });
    }

    // reference database
    if (mReferenceDatabase) {
        // create reference map
        Map<UInt, FifamClub *> referenceClubsByFifaID;
        for (FifamClub *c : mReferenceDatabase->mClubs) {
            if (c->mFifaID)
                referenceClubsByFifaID[c->mFifaID] = c;
        }
        // apply parameters
        for (FifamClub *c : mFifamDatabase->mClubs) {
            if (c->mFifaID) {
                auto it = referenceClubsByFifaID.find(c->mFifaID);
                if (it != referenceClubsByFifaID.end()) {
                    FifamClub *ref = (*it).second;
                    c->mMediaPressure = ref->mMediaPressure;
                    c->mAddress = ref->mAddress;
                    c->mTelephone = ref->mTelephone;
                    c->mWebsiteAndMail = ref->mWebsiteAndMail;
                    c->mFansites = ref->mFansites;
                    c->mFanMembers = ref->mFanMembers;
                    c->mAiStrategy = ref->mAiStrategy;
                    c->mLandscape = ref->mLandscape;
                    c->mSettlement = ref->mSettlement;
                    c->mTransfersCountry = ref->mTransfersCountry;
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

        else if (c.mID == 51002641) { // Scotiabank CONCACAF Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::NorthAmerica].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, c.mName);
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
    //{
    //    FifamWriter fifaPlayersWriter(L"fm-fifa-players.csv", 14, 0, 0);
    //    if (fifaPlayersWriter.Available()) {
    //        fifaPlayersWriter.WriteLine(L"Country,Level,Club,Name,Birthdate,Nationality,Rating,FoomID,FifaID");
    //        auto WriteOnePlayer = [&](foom::player *_player, foom::club *_club, Int level, FifaTeam *fifaClub) {
    //            String countryName;
    //            if (_club && _club->mNation)
    //                countryName = Quoted(_club->mNation->mShortName)();
    //            String clubName;
    //            if (_club)
    //                clubName = Quoted(_club->mShortName)();
    //            String fifaIdStr;
    //            if (_player->mNation && fifaClub) {
    //                fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
    //                    if (fifaIdStr.empty()) {
    //                        if (fifaPlayer.internal.nationality == _player->mNation->mConverterData.mFIFAID && fifaPlayer.m_birthDate == _player->mDateOfBirth)
    //                            fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
    //                    }
    //                });
    //                if (fifaIdStr.empty()) {
    //                    String foomPlayerName = Utils::GetQuickName(_player->mFirstName, _player->mSecondName, _player->mCommonName);
    //                    fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
    //                        if (fifaIdStr.empty()) {
    //                            if (fifaPlayer.m_quickName == foomPlayerName)
    //                                fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
    //                        }
    //                    });
    //                }
    //            }
    //            String playerName;
    //            if (!_player->mCommonName.empty())
    //                playerName = _player->mCommonName;
    //            else {
    //                if (!_player->mFirstName.empty())
    //                    playerName = _player->mFirstName;
    //                if (!_player->mSecondName.empty()) {
    //                    if (!playerName.empty())
    //                        playerName += L" ";
    //                    playerName += _player->mSecondName;
    //                }
    //            }
    //            fifaPlayersWriter.WriteLine(
    //                countryName,
    //                level,
    //                clubName,
    //                Quoted(playerName),
    //                _player->mDateOfBirth,
    //                _player->mNation ? _player->mNation->mThreeLetterName : L"",
    //                _player->mCurrentAbility / 10,
    //                _player->mID,
    //                fifaIdStr);
    //        };
    //        for (auto &entry : mFoomDatabase->mClubs) {
    //            foom::club &team = entry.second;
    //            if (!team.mExtinct && team.mNation) {
    //                Int leagueLevel = 99;
    //                if (team.mDivision && team.mDivision->mCompetitionLevel > 0)
    //                    leagueLevel = team.mDivision->mCompetitionLevel;
    //
    //                FifaTeam *fifaClub = nullptr;
    //                if (team.mConverterData.mFIFAID != 0)
    //                    fifaClub = mFifaDatabase->GetTeam(team.mConverterData.mFIFAID);
    //
    //                for (foom::player *p : team.mConverterData.mContractedPlayers)
    //                    WriteOnePlayer(p, &team, leagueLevel, fifaClub);
    //                for (foom::player *p : team.mConverterData.mLoanedPlayers)
    //                    WriteOnePlayer(p, &team, leagueLevel, fifaClub);
    //
    //            }
    //        }
    //        FifaTeam *freeAgentsClub = mFifaDatabase->GetTeam(111592);
    //        for (auto &entry : mFoomDatabase->mPlayers) {
    //            foom::player p = entry.second;
    //            if (!p.mContract.mClub)
    //                WriteOnePlayer(&p, nullptr, 99, freeAgentsClub);
    //        }
    //        fifaPlayersWriter.Close();
    //    }
    //}

    std::wcout << L"Writing database..." << std::endl;
    mFifamDatabase->Write(gameId, FifamDatabase::GetGameDbVersion(gameId),
        Utils::Format(writeToGameFolder ?
            L"D:\\Games\\FIFA Manager %02d\\database" :
            L"fm_test\\database",
            gameId).c_str());

    if (!mFromFifaDatabase) {
        if (gameId >= 11 && writeToGameFolder) {
            std::wcout << L"Writing derbies..." << std::endl;
            struct derby_info {
                String nameId;
                String name;
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
            FifamWriter derbiesWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\fmdata\\Parameter File - Derbies.txt", gameId),
                gameId, FifamVersion(), false);
            if (derbiesWriter.Available()) {
                derbiesWriter.WriteLine(L"BEGIN( DERBIES )");
                derbiesWriter.WriteNewLine();
                derbiesWriter.WriteLine(L"    DERBY_COUNT = " + Utils::Format(L"%d", derbies.size()));
                derbiesWriter.WriteNewLine();
                UInt maxNameIdLength = 0;
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
                    UInt len = derbies[i].nameId.length();
                    if (len > maxNameIdLength)
                        maxNameIdLength = len;
                }
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
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
            FifamWriter derbyNamesWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\plugins\\derbies.tr", gameId),
                gameId, FifamVersion(), true);
            Set<String> writtenDerbyNames;
            if (derbyNamesWriter.Available()) {
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
                    if (!Utils::Contains(writtenDerbyNames, derbies[i].nameId)) {
                        derbyNamesWriter.WriteLine(derbies[i].nameId + L"|" + derbies[i].name);
                        writtenDerbyNames.insert(derbies[i].nameId);
                    }
                }
                derbyNamesWriter.Close();
            }
        }

        std::wcout << L"Writing female persons..." << std::endl;

        FifamWriter femaleStaffWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database\\FemaleStaff.sav", gameId), gameId, FifamVersion(), true);
        femaleStaffWriter.WriteLine(L"FirstName,LastName,Pseudonym,Birthdate,Nationality,Job,ID");
        for (FifamStaff *staff : mFifamDatabase->mStaffs) {
            auto fmstaff = staff->GetProperty<foom::non_player *>(L"foom::non_player");
            if (fmstaff && fmstaff->mFemale) {
                Date birthdate = staff->mBirthday;
                femaleStaffWriter.WriteLine(Quoted(staff->mFirstName), Quoted(staff->mLastName), Quoted(staff->mPseudonym), birthdate, staff->mNationality[0],
                    staff->mClubPosition, staff->mWriteableStringID);
            }
        }

        FifamWriter femaleRefereeWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database\\FemaleReferee.sav", gameId), gameId, FifamVersion(), true);
        femaleRefereeWriter.WriteLine(L"FirstName,LastName,Pseudonym,Birthdate,Nationality,Job,ID");
        for (FifamReferee *ref : mFifamDatabase->mReferees) {
            auto fmofficial = ref->GetProperty<foom::official *>(L"foom::official");
            if (fmofficial && fmofficial->mFemale) {
                femaleRefereeWriter.WriteLine(Quoted(ref->mFirstName), Quoted(ref->mLastName), String(), Date(), ref->mCountry->mId,
                    0, FifamNames::GetPersonStringId(gameId, ref->mFirstName, ref->mLastName, String(), Date(), 0));
            }
        }
    }

#if 0
    GraphicsConverter graphicsConverter;
    if (!mFromFifaDatabase) {
        //graphicsConverter.mOnlyUpdates = true;
        //std::wcout << L"Converting club badges..." << std::endl;
        //graphicsConverter.ConvertClubBadges(mFoomDatabase, mAvailableBadges, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting portraits..." << std::endl;
        //graphicsConverter.ConvertPortraits(mFoomDatabase, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting competition badges..." << std::endl;
        //graphicsConverter.ConvertCompBadges(mFifamDatabase, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting trophies..." << std::endl;
        //graphicsConverter.ConvertTrophies(mFifamDatabase, graphicsPath, contentPath, gameId, 0);
        //graphicsConverter.ConvertCities(mFoomDatabase, L"D:\\Downloads", L"D:\\Games\\FIFA Manager 13", gameId, 0);
        //graphicsConverter.ConvertStadiums(mFoomDatabase, L"D:\\Downloads", L"D:\\Games\\FIFA Manager 13", gameId, 0, false);
    }
    else {
        std::wcout << L"Converting FIFA club badges..." << std::endl;
        graphicsConverter.ConvertClubBadgesFIFA(mFifamDatabase, mFifaAssetsPath, contentPath, gameId);
        std::wcout << L"Converting FIFA competition badges..." << std::endl;
        graphicsConverter.ConvertCompBadgesFIFA(mFifamDatabase, mFifaAssetsPath, contentPath, gameId);
    }
#endif

    delete mReferenceDatabase;
    mReferenceDatabase = nullptr;
    delete mFifamDatabase;
    mFifamDatabase = nullptr;
    delete mFoomDatabase;
    mFoomDatabase = nullptr;
    delete mFifaDatabase;
    mFifaDatabase = nullptr;
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
        return FifamFormation::_5_2_2_1;
    case 31:
    case 35:
        return FifamFormation::_3_4_3_Defensive;
    case 36:
    case 40:
        return FifamFormation::_4_3_3_Defensive;
    case 41:
    case 56:
        return FifamFormation::_5_2_1_2;
    case 42:
    case 59:
    case 61:
        return FifamFormation::_5_2_2_1;
    case 47:
        return FifamFormation::_5_3_2;
    }
    return FifamFormation::None;
}
