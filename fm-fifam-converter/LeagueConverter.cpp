#include "Converter.h"
#include "ConverterUtil.h"
#include "FifamNames.h"
#include "FifamCompPool.h"

void Converter::ConvertLeagues(UInt gameId) {

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

    for (auto &entry : mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto country = mFifamDatabase->GetCountry(nation.mConverterData.mFIFAManagerID);
        if (country) {
            ConvertNationInfo(country, &nation, gameId);
            country->SetProperty(L"foom::nation", &nation);
            CreateStaffMembersForClub(gameId, &nation, &country->mNationalTeam, true);
            country->mNationalTeam.SetProperty(L"foom::nation", &nation);

            Map<UInt, DivisionInfo *> divLeagueLevels;
            Map<UInt, Vector<DivisionInfo *>> divLeagues;

            // find all division competitions for this country
            Int maxLevel = -1;
            Bool simulateLeague = false;
            for (auto &divInfo : mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    Int minPriority = (gameId <= 7) ? 4 : 1;
                    if (divInfo.mPriority >= minPriority) {
                        simulateLeague = true;
                        if (divInfo.mType == DivisionInfo::League)
                            divLeagues[divInfo.mLevel].push_back(&divInfo);
                        else if (divInfo.mType == DivisionInfo::Level)
                            divLeagueLevels[divInfo.mLevel] = &divInfo;
                        if (divInfo.mLevel > maxLevel)
                            maxLevel = divInfo.mLevel;
                    }
                    else if (divInfo.mPriority >= 1)
                        simulateLeague = true;
                }
            }

            country->SetProperty<Bool>(L"SimulateLeague", simulateLeague);

            Vector<FifamCompLeague *> createdLeagues;
            Vector<FifamCompPool *> createdPools;
            Vector<FifamCompCup *> createdCups;

            if (maxLevel >= 0) {

                //create_directories(L"leagues");
                //FifamWriter leagueWriter(L"leagues\\" + FifamTr(country->mName) + L".txt", 14, FifamVersion());

                bool countryUsesSpecialScript =
                    country->mId == FifamNation::England ||
                    country->mId == FifamNation::Germany ||
                    country->mId == FifamNation::Scotland ||
                    country->mId == FifamNation::Spain;

                // sort leagues
                for (auto &e : divLeagues) {
                    if (e.second.size() > 1) {
                        std::sort(e.second.begin(), e.second.end(), [](DivisionInfo * a, DivisionInfo * b) {
                            return a->mOrder < b->mOrder;
                            });
                    }
                }

                // create leagues

                mNextFreeUID[country->mId - 1] = 0x4001;
                mNumTeamsInLeagueSystem[country->mId - 1] = 0;
                Int minLevelWithReserveTeams = -1;

                UInt leagueIndex = 0;
                for (auto &leagues : divLeagues) {
                    UInt teamCounter = 0;
                    for (DivisionInfo *lg : leagues.second) {
                        if (!nation.mConverterData.mDomesticComps.league)
                            nation.mConverterData.mDomesticComps.league = lg;
                        FifamCompID leagueID = FifamCompID(country->mId, FifamCompType::League, leagueIndex++);
                        String compName;
                        if (lg->mName.length() <= MAX_LEAGUE_NAME_LENGTH)
                            compName = lg->mName;
                        else
                            compName = FifamNames::LimitName(lg->mShortName, MAX_LEAGUE_NAME_LENGTH);
                        FifamCompLeague * league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, leagueID, compName)->AsLeague();

                        lg->mCompID = leagueID;

                        // convert - old

                        foom::comp * comp = mFoomDatabase->get<foom::comp>(lg->mID);
                        if (!comp) {
                            Error(L"Competition is not available\nCompetitionName: %s\nCompetitionID: %d", lg->mName.c_str(), lg->mID);
                            continue;
                        }

                        league->mCompetitionLevel = lg->mLevel;
                        league->mLeagueLevel = lg->mLevel;
                        league->mNumSubsAllowed = 3;
                        league->mNumTeams = lg->mTeams;
                        league->mNumRelegatedTeams = lg->mRelegated;

                        // rounds (auto-calculation)

                        if (lg->mRounds > 0)
                            league->mNumRounds = lg->mRounds;
                        else if (league->mNumTeams >= 15)
                            league->mNumRounds = 2;
                        else if (league->mNumTeams >= 11)
                            league->mNumRounds = 3;
                        else
                            league->mNumRounds = 4;

                        league->mRoundType = FifamRoundID::Group1;
                        league->mTeams.resize(lg->mTeams);

                        if (comp->mReputation <= 20) {
                            if (comp->mReputation > 10) {
                                league->mTransferMarketMp = comp->mReputation;
                                league->mAttendanceMp = comp->mReputation;
                            }
                            else {
                                league->mTransferMarketMp = 10;
                                league->mAttendanceMp = 10;
                            }
                        }
                        else {
                            league->mTransferMarketMp = Utils::Min(200, (Int)((Float)(comp->mReputation - 20) * 1.3f));
                            league->mAttendanceMp = Utils::Min(120, (Int)((Float)(comp->mReputation - 20) * 0.69f));
                        }
                        league->mFreeAgentsCanBeTranferredAtAnyTime = true;
                        league->mForeignFreeAgentsCanBeTransferredAtAnyTime = true;
                        league->mShortTermLoansTotalLimit = 15;
                        league->mShortTermLoansSimultaneosLimit = 15;
                        league->mLongTermLoansTotalLimit = 15;
                        league->mLongTermLoansSimultaneosLimit = 15;
                        league->mOverallTotalLoansLimit = 15;
                        league->mOverallTotalSimultaneosLoansLimit = 15;
                        league->mLoanedPlayersPerSeason = 15;
                        league->mLoanPlayerPerSquad = 15;
                        league->mSimLoanOtherLeague = 15;
                        league->mSimLoanSameLeague = 15;
                        league->mLoanOtherLeagueCount = 15;
                        league->mLoanSameLeagueCount = 15;

                        comp->mConverterData.mLeague = league;
                        league->SetProperty(L"foom::comp", comp);
                        league->SetProperty(L"foom::id", lg->mID);
                        league->SetProperty(L"foom::reputation", lg->mRep);
                        league->SetProperty(L"startDate", lg->mStartDate);
                        league->SetProperty(L"endDate", lg->mEndDate);
                        league->SetProperty(L"winterBreakStart", lg->mWinterBreakStart);
                        league->SetProperty(L"winterBreakEnd", lg->mWinterBreakEnd);
                        createdLeagues.push_back(league);
                        mLeaguesSystem[country->mId - 1][league->mLeagueLevel].push_back(league);

                        league->mEqualPointsSorting = lg->mSorting;
                        if (lg->mTvBonus >= 0)
                            league->mBonuses[3] = AlignMoneyValue(lg->mTvBonus, 1'000);
                        if (lg->mWinBouns >= 0)
                            league->mBonuses[1] = AlignMoneyValue(lg->mWinBouns, 1'000);
                        if (lg->mPlaceBonus >= 0)
                            league->mBonuses[2] = AlignMoneyValue(lg->mPlaceBonus, 1'000);
                        if (lg->mAttendanceMp > 0)
                            league->mAttendanceMp = lg->mAttendanceMp;
                        if (lg->mTransfersMp > 0)
                            league->mTransferMarketMp = lg->mTransfersMp;
                        if (lg->mNumSubs > 0)
                            league->mNumSubsAllowed = lg->mNumSubs;
                        if (lg->mDomesticPlayers > 0)
                            league->mMinDomesticPlayerCount = lg->mDomesticPlayers;
                        if (lg->mU21Players > 0)
                            league->mMinU21PlayerCount = lg->mU21Players;
                        if (lg->mNonEuSigns > 0)
                            league->mMaxNumberOfNonEUSigns = lg->mNonEuSigns;

                        league->mInstructions.PushBack(new FifamInstruction::GET_POOL(
                            FifamCompID(country->mId, FifamCompType::Pool, leagues.first), teamCounter, lg->mTeams));

                        // add teams
                        Bool hasReserveTeams = lg->mReserveTeamsAllowed == 1;
                        UInt numLeaguePromotedTeams = 0;
                        UInt numLeagueRelegatedTeams = 0;

                        if (comp->mVecTeams.size() != lg->mTeams) {
                            Error(L"Different number of teams in league: %d / %d\nCompetitionName: %s\nCompetitionID: %d",
                                comp->mVecTeams.size(), lg->mTeams, lg->mName.c_str(), lg->mID);
                            continue;
                        }

                        if (!comp->mConverterData.mUsesTableData) {
                            for (auto entry : comp->mVecTeams) {
                                foom::club *team = (foom::club *)entry;
                                if (!team)
                                    continue;
                                Int teamRep = team->mReputation;
                                if (teamRep < 0)
                                    teamRep = 0;
                                else if (teamRep > 10'000)
                                    teamRep = 10'000;
                                if (!team->mIsReserveDummyClub) {
                                    if (team->mLastDivision) {
                                        if (team->mLastDivision != comp) {
                                            if (team->mLastDivision->mCompetitionLevel == 0 || team->mLastDivision->mCompetitionLevel > comp->mCompetitionLevel) {
                                                numLeaguePromotedTeams++;
                                                if (team->mLastPosition != 0)
                                                    team->mConverterData.mLeaguePos = 100'000 + team->mLastPosition;
                                                else
                                                    team->mConverterData.mLeaguePos = 100'000 + 1'000 + (10'000 - teamRep);
                                            }
                                            else {
                                                numLeagueRelegatedTeams++;
                                                if (team->mLastPosition != 0)
                                                    team->mConverterData.mLeaguePos = -100'000 - (1'000 - team->mLastPosition);
                                                else
                                                    team->mConverterData.mLeaguePos = -1 - teamRep;
                                            }
                                        }
                                        else {
                                            if (team->mLastPosition != 0)
                                                team->mConverterData.mLeaguePos = team->mLastPosition;
                                            else
                                                team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                                        }
                                    }
                                    else {
                                        numLeaguePromotedTeams++;
                                        team->mConverterData.mLeaguePos = 200'000 + (10'000 - teamRep);
                                    }
                                }
                                else {
                                    team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                                }
                            }
                        }
                        UInt leagueClubCounter = 0;
                        std::sort(comp->mVecTeams.begin(), comp->mVecTeams.end(), [](foom::team * a, foom::team * b) {
                            if (!a) return false;
                            if (!b) return true;
                            return ((foom::club *)a)->mConverterData.mLeaguePos < ((foom::club *)b)->mConverterData.mLeaguePos;
                        });

                        //if (leagueWriter.Available()) {
                        //    leagueWriter.WriteLine(L";----------------------------------------------------------------------------------------");
                        //    leagueWriter.WriteLine(Utils::Format(L"LEAGUE %d - %s (%d teams)", lg->mID, lg->mName, lg->mTeams));
                        //    leagueWriter.WriteLine(L";----------------------------------------------------------------------------------------");
                        //    for (auto entry : comp->mVecTeams) {
                        //        foom::club *team = (foom::club *)entry;
                        //        String teamLine = Utils::Format(L"%-10d - %s", team->mID, team->mName);
                        //        if (team->mConverterData.mParentClub)
                        //            teamLine += L" (RES)";
                        //        leagueWriter.WriteLine(teamLine);
                        //    }
                        //}

                        for (auto entry : comp->mVecTeams) {
                            foom::club *team = (foom::club *)entry;
                            if (!team) {
                                Error(L"Invalid club pointer in league\nLeague: %s", lg->mName.c_str());
                                continue;
                            }
                            FifamClub *club = nullptr;
                            foom::club *mainTeam = nullptr; // for reserve teams
                            FifamClubTeamType teamType = FifamClubTeamType::First;
                            Bool createNewClub = false;
                            Bool isExtinct = team->mExtinct;
                            if (team->mConverterData.mParentClub) {
                                isExtinct = team->mConverterData.mParentClub->mExtinct;
                                hasReserveTeams = true;
                            }
                            if (isExtinct) {
                                Error(L"Extinct club in the league\nClub: '%s'\nLeague: '%s'",
                                    team->mName.c_str(), lg->mName.c_str());
                            }
                            else if (team->mConverterData.mFifamClub) {
                                Message(Utils::Format(L"Team already present in other league\nClub: '%s'\nLeague: '%s'",
                                    team->mName.c_str(), lg->mName.c_str()));
                                club = (FifamClub *)team->mConverterData.mFifamClub;
                                if (team->mConverterData.mParentClub)
                                    teamType = FifamClubTeamType::Reserve;
                            }
                            else if (team->mConverterData.mParentClub) {
                                if (!team->mConverterData.mParentClub->mConverterData.mFifamClub) {
                                    Error(L"Reserve club appears before the first team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                        team->mConverterData.mParentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                }
                                else {
                                    if (!team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB
                                        && team->mConverterData.mParentClub->mNation == &nation)
                                    {
                                        club = (FifamClub *)team->mConverterData.mParentClub->mConverterData.mFifamClub;
                                        teamType = FifamClubTeamType::Reserve;
                                        team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB = team;
                                        team->mConverterData.mParentClubInDB = team->mConverterData.mParentClub;
                                    }
                                    else {
                                        foom::club *parentClub = team->mConverterData.mParentClub;
                                        if (!team->mIsReserveDummyClub) {
                                            team->mConverterData.mParentClub->mConverterData.mChildClubs.erase(std::remove_if(
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.begin(),
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.end(),
                                                [=](foom::club::converter_data::child_club const &a) {
                                                    return a.mClub == team;
                                                }), team->mConverterData.mParentClub->mConverterData.mChildClubs.end());
                                            mainTeam = team->mConverterData.mParentClub;
                                            team->mConverterData.mOriginalStoredParentClub = team->mConverterData.mParentClub;
                                            team->mConverterData.mParentClub = nullptr;
                                            createNewClub = true;
                                        }
                                        else {
                                            Error(L"Reserve club can't be created because it's a dummy club\nMainClub: '%s'\nReserveTeamID: %d\nLeague: '%s'",
                                                team->mConverterData.mParentClub->mName.c_str(), team->mID, lg->mName.c_str());
                                        }
                                        //Error(L"Reserve club already present for team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                        //    parentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                    }
                                }
                            }
                            else
                                createNewClub = true;
                            if (createNewClub)
                                club = CreateAndConvertClub(gameId, team, mainTeam, country, lg, true);
                            // put team to league
                            if (club) {
                                league->mTeams[leagueClubCounter].mPtr = club;
                                league->mTeams[leagueClubCounter].mTeamType = teamType;
                                club->SetProperty(L"league", league);
                                if (comp->mConverterData.mUsesTableData) {
                                    if (team->mConverterData.mTablePromoted) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                    }
                                    else if (team->mConverterData.mTableRelegated) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                    }
                                }
                                else {
                                    if (team->mConverterData.mLeaguePos >= 100'000) {
                                        if (numLeaguePromotedTeams <= (comp->mVecTeams.size() / 2)) {
                                            if (teamType == FifamClubTeamType::First)
                                                club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                            else
                                                club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                        }
                                    }
                                    else if (team->mConverterData.mLeaguePos < 0) {
                                        if (numLeagueRelegatedTeams <= (comp->mVecTeams.size() / 2)) {
                                            if (teamType == FifamClubTeamType::First)
                                                club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                            else
                                                club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                        }
                                    }
                                }
                                leagueClubCounter++;
                            }
                        }

                        if (hasReserveTeams) {
                            if (minLevelWithReserveTeams == -1 || minLevelWithReserveTeams > league->mLeagueLevel)
                                minLevelWithReserveTeams = league->mLeagueLevel;
                        }

                        bool gotFixtures = false;
                        Date leagueStart, leagueEnd;

                        if (mFixturesPerLeague.count(comp->mID) > 0) {
                            auto const &fixtures = mFixturesPerLeague[comp->mID];
                            UInt leagueTotalMatches = league->GetTotalNumMatches();
                            UInt leagueMatchdaysInRound = league->GetNumMatchdaysInRound();
                            UInt leagueMatchesInMatchday = league->GetNumMatchesInMatchday();
                            if (leagueTotalMatches > 0 && fixtures.size() == leagueTotalMatches) {
                                gotFixtures = true;
                                league->mFixtures.resize(league->mNumRounds * leagueMatchdaysInRound);
                                Map<FifamClubLink, UInt> leagueTeamsMap;
                                for (UInt i = 0; i < league->mTeams.size(); i++)
                                    leagueTeamsMap[league->mTeams[i]] = i + 1;
                                UInt matchId = 0;
                                UInt matchdayId = 0;
                                for (UInt r = 0; r < league->mNumRounds; r++) {
                                    if (!gotFixtures)
                                        break;
                                    for (UInt md = 0; md < leagueMatchdaysInRound; md++) {
                                        if (!gotFixtures)
                                            break;
                                        league->mFixtures[matchdayId].resize(leagueMatchesInMatchday);
                                        for (UInt d = 0; d < leagueMatchesInMatchday; d++) {
                                            if (fixtures.size() <= matchId) {
                                                Error(L"Incorrect fixtures list size\nLeague: %s", comp->mName.c_str());
                                                gotFixtures = false;
                                                break;
                                            }
                                            UInt team1id = 0, team2id = 0;
                                            if (!fixtures[matchId].mTeam1 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam1)) == 0) {
                                                //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam1 ? GetTeamClubLink(fixtures[matchId].mTeam1).GetTeamName().c_str() : L"none");
                                                gotFixtures = false;
                                                break;
                                            }
                                            team1id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam1)];
                                            if (!fixtures[matchId].mTeam2 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam2)) == 0) {
                                                //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam2 ? GetTeamClubLink(fixtures[matchId].mTeam2).GetTeamName().c_str() : L"none");
                                                gotFixtures = false;
                                                break;
                                            }
                                            team2id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam2)];
                                            league->mFixtures[matchdayId][d] = { team1id, team2id };
                                            matchId++;
                                        }
                                        matchdayId++;
                                    }
                                }
                            }
                            //else {
                            //    Error(L"Error fixtures list size\nLeague: %s\n%d/%d", comp->mName.c_str(), fixtures.size(), leagueTotalMatches);
                            //}
                        }

                        if (!gotFixtures)
                            league->GenerateFixtures();
                        league->GenerateCalendar(13, 0, 0, 0);

                        // TODO: translate date to calendar day

                        mNumTeamsInLeagueSystem[country->mId - 1] += lg->mTeams;

                        teamCounter += lg->mTeams;
                    }
                }

                // create league levels
                for (Int i = 0; i <= maxLevel; i++) {
                    DivisionInfo *level = nullptr;
                    Int levelRep = 0;
                    String levelName;
                    auto itLevel = divLeagueLevels.find(i);
                    // if level exists
                    if (itLevel != divLeagueLevels.end()) {
                        level = divLeagueLevels[i];
                        if (level->mName.length() < 29)
                            levelName = level->mName;
                        else
                            levelName = level->mShortName;
                        levelRep = level->mRep;
                    }
                    else { // if level does not exist
                        Vector<String> divsOnLevelNames;
                        auto itLeagues = divLeagues.find(i);
                        if (itLeagues != divLeagues.end()) {
                            for (auto &div : divLeagues[i])
                                divsOnLevelNames.push_back(div->mName);
                            if (!divsOnLevelNames.empty())
                                levelName = FifamNames::GetLeagueLevelName(divsOnLevelNames, i + 1);
                            if (divLeagues[i].size() > 0)
                                levelRep = divLeagues[i][0]->mRep;
                        }
                        else
                            Error(L"No league levels or leagues at level %d in country %s", i, FifamTr(country->mName).c_str());
                    }
                    // setup level information
                    auto &levelNames = country->mLeagueLevelNames.emplace_back();
                    FifamTrSetAll<String>(levelNames, FifamNames::LimitName(levelName, 28));

                    DivisionInfo *firstDivOnLevel = nullptr;

                    auto itFirstLeague = divLeagues.find(i);
                    if (itFirstLeague != divLeagues.end() && divLeagues[i].size() > 0)
                        firstDivOnLevel = divLeagues[i][0];

                    if (firstDivOnLevel) {
                        country->mLeagueLevels[i].mEqualPointsSorting = firstDivOnLevel->mSorting;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = firstDivOnLevel->mForeignersLimit;
                        country->mLeagueLevels[i].mNumRelegatedTeams = firstDivOnLevel->mRelegated;
                    }
                    else {
                        country->mLeagueLevels[i].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = 0;
                        country->mLeagueLevels[i].mNumRelegatedTeams = 0;
                    }

                    country->mLeagueLevels[i].mRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));

                    if (i == 0)
                        country->mAveragePlayerRating = country->mLeagueLevels[i].mRating;

                    // setup pool

                    FifamCompID poolID = FifamCompID(country->mId, FifamCompType::Pool, i);
                    FifamCompPool * pool = mFifamDatabase->CreateCompetition(FifamCompDbType::Pool, poolID, FifamNames::LimitName(levelName, MAX_POOL_NAME_LENGTH) + L" Pool")->AsPool();
                    pool->mCompetitionLevel = i;

                    createdPools.push_back(pool);

                    if (level)
                        level->mCompID = poolID;

                    if (i > 0)
                        pool->mCompConstraints.push_back(FifamCompID(country->mId, FifamCompType::Pool, i - 1));

                    // higher level - relegated teams
                    if (i > 0) {
                        UInt higherLevel = i - 1;
                        if (divLeagues.find(higherLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[higherLevel]) {
                                if (l->mRelegated > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                        l->mCompID, l->mTeams - l->mRelegated + 1, l->mRelegated));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", higherLevel, FifamTr(country->mName).c_str());
                    }

                    // this level
                    if (divLeagues.find(i) != divLeagues.end()) {
                        // apply pool sorting if 2 or more leagues on this level
                        if (divLeagues[i].size() > 1)
                            pool->mSorting = FifamPoolSorting::MapLeague;

                        for (DivisionInfo *l : divLeagues[i]) {
                            pool->mNumTeams += l->mTeams;
                            Int numTeamsFromThisLeague = l->mTeams - (l->mPromoted + l->mRelegated);
                            if (numTeamsFromThisLeague > 0) {
                                pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                    l->mCompID, l->mPromoted + 1, numTeamsFromThisLeague));
                            }
                        }
                    }
                    else
                        Error(L"No leagues at level %d\nCountry: %s", i, FifamTr(country->mName).c_str());

                    // next level - promoted teams
                    if (i != maxLevel) {
                        UInt lowerLevel = i + 1;
                        if (divLeagues.find(lowerLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[lowerLevel]) {
                                if (l->mPromoted > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(l->mCompID, 1, l->mPromoted));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", lowerLevel, FifamTr(country->mName).c_str());
                    }
                    else { // CaC league & spare
                        pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                            FifamCompID(country->mId, FifamCompType::League, leagueIndex), 1, 1));
                        pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                    }
                }

                if (minLevelWithReserveTeams >= 0) {
                    country->mLeagueLevelWithReserveTeams = minLevelWithReserveTeams;
                    for (auto pool : createdPools) {
                        if (pool->mCompetitionLevel >= minLevelWithReserveTeams)
                            pool->mReserveTeamsAllowed = true;
                    }
                }
                else
                    country->mLeagueLevelWithReserveTeams = maxLevel + 1;

                //if (leagueWriter.Available()) {
                //    leagueWriter.WriteLine(L";----------------------------------------------------------------------------------------");
                //    leagueWriter.WriteLine(L"NON-LEAGUE CLUBS");
                //    leagueWriter.WriteLine(L";----------------------------------------------------------------------------------------");
                //    for (auto &entry : mFoomDatabase->mClubs) {
                //        auto &c = entry.second;
                //        if (!c.mExtinct && c.mNation == &nation && !c.mConverterData.mFifamClub) {
                //            String teamLine = Utils::Format(L"%-10d - %s", c.mID, c.mName);
                //            if (c.mConverterData.mParentClub)
                //                teamLine += L" (RES)";
                //            leagueWriter.WriteLine(teamLine);
                //        }
                //    }
                //}

            }
            else {
                country->mLeagueLevels[0].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                country->mLeagueLevels[0].mNumNonEUPlayersAllowed = 0;
                country->mLeagueLevels[0].mNumRelegatedTeams = 0;
                country->mLeagueLevels[0].mRating = 0;

                Int levelRep = -1;

                for (auto &divInfo : mDivisions) {
                    if (divInfo.mNationID == nation.mID && divInfo.mType == DivisionInfo::League && divInfo.mRep > levelRep)
                        levelRep = divInfo.mRep;
                }

                if (levelRep > 0) {
                    country->mAveragePlayerRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));
                }
            }

            for (auto &e : mLeaguesSystem[country->mId - 1]) {
                for (auto &l : e.second)
                    l->SetProperty<Int>(L"NumLeaguesOnLevel", e.second.size());
            }

            if (mNumTeamsInLeagueSystem[country->mId - 1] > 0) {
                Int cupsMinPriority = 1;
                Int maxFACups = 1;
                Int maxLeagueCups = 4;
                Int maxSuperCups = 1;
                Int numCreatedFACups = 0;
                Int numCreatedLeagueCups = 0;
                Int numCreatedSuperCups = 0;
                if (gameId <= 7)
                    cupsMinPriority = 2;
                Int startMatchdaySuperCup = 3;
                Int startMatchdayFaCup = startMatchdaySuperCup + 2;
                Int startMatchdayLeagueCup = startMatchdayFaCup + 16;

                // find all cups in this country
                for (auto &cupInfo : mCups) {
                    if (cupInfo.mNationID == nation.mID && cupInfo.mPriority >= cupsMinPriority) {
                        Int nextAvailableCompIndex = -1;
                        FifamCompType cupCompType = FifamCompType::LeagueCup;
                        if (cupInfo.mType == CupInfo::FA) {
                            if (numCreatedFACups < maxFACups) {
                                nextAvailableCompIndex = numCreatedFACups;
                                numCreatedFACups++;
                                cupCompType = FifamCompType::FaCup;
                                if (!nation.mConverterData.mDomesticComps.cup)
                                    nation.mConverterData.mDomesticComps.cup = &cupInfo;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::League) {
                            if (numCreatedLeagueCups < maxLeagueCups) {
                                nextAvailableCompIndex = numCreatedLeagueCups;
                                numCreatedLeagueCups++;
                                cupCompType = FifamCompType::LeagueCup;
                                if (!nation.mConverterData.mDomesticComps.leagueCup)
                                    nation.mConverterData.mDomesticComps.leagueCup = &cupInfo;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::Supercup) {
                            if (numCreatedSuperCups < maxSuperCups) {
                                nextAvailableCompIndex = numCreatedSuperCups;
                                cupCompType = FifamCompType::SuperCup;
                                if (!nation.mConverterData.mDomesticComps.supercup)
                                    nation.mConverterData.mDomesticComps.supercup = &cupInfo;
                            }
                        }
                        if (nextAvailableCompIndex != -1) {
                            if (cupInfo.mTemplateType == FifamCupSystemType::None) {
                                Error(L"Unknown cup template type\nCup: %s", cupInfo.mName.c_str());
                                continue;
                            }
                            FifamCupAlloc *cupTemplate = mFifamDatabase->GetCupTemplate(cupInfo.mTemplateType);
                            if (!cupTemplate) {
                                Error(L"Cup template not found\nCup: %s\nTemplate: %s", cupInfo.mName.c_str(), cupInfo.mTemplateType.ToCStr());
                                continue;
                            }
                            FifamCompID cupID = { (UChar)country->mId, cupCompType, (UShort)nextAvailableCompIndex };
                            String cupName;
                            if (cupInfo.mName.length() <= MAX_CUP_NAME_LENGTH)
                                cupName = cupInfo.mName;
                            else
                                cupName = FifamNames::LimitName(cupInfo.mShortName, MAX_CUP_NAME_LENGTH);
                            FifamCompCup * cup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, cupID, cupName)->AsCup();
                            createdCups.push_back(cup);
                            cup->SetProperty(L"foom::id", cupInfo.mID);
                            cup->SetProperty(L"foom::reputation", cupInfo.mReputation);
                            if (cupInfo.mLevel < 0 || cupInfo.mLevel > 15)
                                cup->mCompetitionLevel = 0;
                            else
                                cup->mCompetitionLevel = cupInfo.mLevel;
                            if (cupInfo.mNumSubs <= 0)
                                cup->mNumSubsAllowed = 3;
                            else
                                cup->mNumSubsAllowed = Utils::Clamp(cupInfo.mNumSubs, 1, 7);
                            cup->mCupTemplate = cupInfo.mTemplateType;
                            Int numRounds = 0;
                            Int startRoundIndex = -1;
                            for (Int i = 7; i >= 0; i--) {
                                if (cupInfo.mRoundDesc[i].empty())
                                    break;
                                startRoundIndex = i;
                                numRounds++;
                            }
                            if (numRounds > 0 && startRoundIndex >= 0) {
                                cup->mRounds.resize(numRounds);
                                bool failed = false;
                                Int r = 0;
                                for (Int i = startRoundIndex; i <= 7; i++) {
                                    if (cupInfo.mRoundDesc[i].empty()) {
                                        Error(L"Empty cup description\nCountry: %s\nCup: %s", FifamTr(country->mName).c_str(), cupInfo.mName.c_str());
                                        failed = true;
                                        break;
                                    }
                                    cup->mRounds[r].mFlags.Clear();
                                    for (UInt c = 0; c < cupInfo.mRoundDesc[i].length(); c++) {
                                        if (c == 0) {
                                            if (cupInfo.mRoundDesc[i][c] == '1')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_1stLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '2')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_2ndLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'R')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithReplay, true);
                                            else {
                                                Error(L"Wrong round description in cup\nCup: %s\nRound: %d\nDescription: %s", cupInfo.mName.c_str(), i, cupInfo.mRoundDesc[i].c_str());
                                                failed = true;
                                                break;
                                            }
                                        }
                                        else {
                                            if (cupInfo.mRoundDesc[i][c] == 'E')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithExtraTime, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'P')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithPenalty, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'G')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithGoldenGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'S')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithSilverGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'A')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithoutAwayGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '-' && (c + 1) < cupInfo.mRoundDesc[i].length()) {
                                                if (cupInfo.mRoundDesc[i][c + 1] == 'P')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoPremierLeagueTeams, true);
                                                else if (cupInfo.mRoundDesc[i][c + 1] == 'S')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoShuffle, true);
                                                c++;
                                            }
                                            else if (cupInfo.mRoundDesc[i][c] == '+') {
                                                Int newTeamsInRound = Utils::SafeConvertInt<Int>(cupInfo.mRoundDesc[i].substr(c + 1));
                                                if (newTeamsInRound > 0)
                                                    cup->mRounds[r].mNewTeamsRound = newTeamsInRound;
                                                break;
                                            }
                                        }
                                    }
                                    if (failed)
                                        break;
                                    r++;
                                }
                                if (failed)
                                    cup->mRounds.clear();
                                else if (!cup->mRounds.empty()) {
                                    UInt teamsFromPrevRound = 0;
                                    UInt currBeg = 0;
                                    UInt lastRoundWithAddedTeams = 0;
                                    for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                        cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                        cup->mRounds[r].mTeamsRound = cup->mRounds[r].mNewTeamsRound + teamsFromPrevRound;
                                        teamsFromPrevRound = cup->mRounds[r].mTeamsRound / 2;
                                        cup->mRounds[r].mStartBeg = currBeg;
                                        currBeg += teamsFromPrevRound;
                                        cup->mRounds[r].mEndBeg = currBeg;
                                        if (cup->mRounds[r].mNewTeamsRound)
                                            lastRoundWithAddedTeams = r;
                                    }
                                    UInt numRawRounds = cup->mRounds.size() - lastRoundWithAddedTeams;
                                    // final rounds
                                    Int numFinalRounds = 0;
                                    for (Int r = (Int)cup->mRounds.size() - 1; r >= (Int)lastRoundWithAddedTeams; r--) {
                                        if (numFinalRounds >= 4)
                                            break;
                                        cup->mRounds[r].mRoundID.SetFromInt(15 - numFinalRounds);
                                        numFinalRounds++;
                                    }
                                    Int numRoundsLeft = cup->mRounds.size() - numFinalRounds;
                                    // quali rounds
                                    Int numQualiRounds = numRoundsLeft > 5 ? 8 - numRoundsLeft : 0;
                                    for (Int r = 0; r < numQualiRounds; r++)
                                        cup->mRounds[r].mRoundID.SetFromInt(r + 1);
                                    // other rounds
                                    numRoundsLeft -= numQualiRounds;
                                    for (Int r = 0; r < numRoundsLeft; r++)
                                        cup->mRounds[r + numQualiRounds].mRoundID.SetFromInt(r + 5);
                                }
                            }
                            if (cup->mRounds.empty()) {
                                // use template
                                UInt numRounds = cupTemplate->mRounds.size();
                                cup->mRounds.resize(numRounds);
                                for (UInt i = 0; i < numRounds; i++) {
                                    cup->mRounds[i].mTeamsRound = cupTemplate->mRounds[i]->mTeamsRound;
                                    cup->mRounds[i].mNewTeamsRound = cupTemplate->mRounds[i]->mNewTeamsRound;
                                    cup->mRounds[i].mStartBeg = cupTemplate->mRounds[i]->mStartBeg;
                                    cup->mRounds[i].mEndBeg = cupTemplate->mRounds[i]->mEndBeg;
                                    cup->mRounds[i].mFlags = cupTemplate->mRounds[i]->mFlags;
                                    cup->mRounds[i].mRoundID = cupTemplate->mRounds[i]->mRoundID;
                                }
                            }
                            cup->mNumTeams = 0;
                            if (cupInfo.mBonus <= 0)
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mReputation * cupInfo.mReputation * 75, 1'000);
                            else
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mBonus, 1'000);
                            if (cupInfo.mTvBonus <= 0)
                                cupInfo.mTvBonus = AlignMoneyValue((cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation) / 320, 1'000);
                            else
                                cupInfo.mTvBonus = AlignMoneyValue(cupInfo.mTvBonus, 1'000);
                            UInt numMatchdays = 0;
                            for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                cup->mRounds[r].mBonuses[1] = cupInfo.mBonus;
                                cup->mRounds[r].mBonuses[3] = cupInfo.mTvBonus;
                                UInt roundId = cup->mRounds.size() - r - 1;
                                if (roundId != 0) {
                                    UInt divValue = 1 << roundId;
                                    cup->mRounds[r].mBonuses[1] = AlignMoneyValue(cup->mRounds[r].mBonuses[1] / divValue, 1'000u);
                                    cup->mRounds[r].mBonuses[3] = AlignMoneyValue(cup->mRounds[r].mBonuses[3] / divValue, 1'000u);
                                }
                                if (cup->mRounds[r].mFlags.Check(FifamBeg::_2ndLeg) || cup->mRounds[r].mFlags.Check(FifamBeg::WithReplay))
                                    numMatchdays += 2;
                                else
                                    numMatchdays += 1;
                            }
                            if (!cupInfo.mStructure.empty()) {
                                UInt currentLevel = 0;
                                currentLevel = cupInfo.mMaxLevel;
                                for (auto c : cupInfo.mStructure) {
                                    switch (c) {
                                    case L'-':
                                        if (mLeaguesSystem[country->mId - 1][currentLevel].size() > 0)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(mLeaguesSystem[country->mId - 1][currentLevel][0]->mID, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'|':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(currentLevel, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'W':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                        break;
                                    case L'S':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                        break;
                                    case L'C':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'L':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    case L'R':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'N':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    }
                                }
                            }
                            if (cup->mInstructions.Empty()) {
                                if (cupInfo.mType == CupInfo::Supercup) {
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                    if (gameId >= 10)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(0, 1, 24));
                                }
                                else {
                                    for (auto const &level : mLeaguesSystem[country->mId - 1]) {
                                        if (level.first >= (UInt)cupInfo.mMaxLevel) {
                                            if (level.first == 0 && level.second.size() == 1)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(level.second[0]->mID, 1, 24));
                                            else if (level.second.size() > 0)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(level.first, 1, 24));
                                        }
                                    }
                                    if (cupInfo.mType != CupInfo::League)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                }
                            }
                            if (cup->mRounds.empty()) {
                                Error(L"Cup has no rounds\nCountry: %s\nCup: %s",
                                    FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else if (cup->mNumTeams > 128) {
                                Error(L"Cup has more than 128 teams: %d\nCountry: %s\nCup: %s",
                                    cup->mNumTeams, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else {
                                for (unsigned int ir = 0; ir < cup->mRounds.size(); ir++) {
                                    if (cup->mRounds[ir].mTeamsRound == 0 || (cup->mRounds[ir].mTeamsRound % 2)) {
                                        Error(L"Wrong teams count in cup round: %d\nCountry: %s\nCup: %s\nRound: %d",
                                            cup->mRounds[ir].mTeamsRound, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str(), ir);
                                    }
                                }
                            }
                            //cup->mMathcdays
                            //for (UInt r = 0; r < cup->mRounds.size(); r++) {
                            for (UInt m = 0; m < numMatchdays; m++) {
                                if (cupCompType == FifamCompType::FaCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayFaCup++);
                                else if (cupCompType == FifamCompType::LeagueCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayLeagueCup++);
                                else if (cupCompType == FifamCompType::SuperCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdaySuperCup++);
                            }
                            //}
                            cup->mSecondSeasonMatchdays = cup->mFirstSeasonMatchdays;
                        }
                    }
                }
            }
            GenerateCalendar(FifamNation::MakeFromInt(country->mId), mFifamDatabase, createdLeagues, createdCups);
        }
    }

}
