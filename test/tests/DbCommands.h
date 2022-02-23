#pragma once
#include "FifamDbEnvironment.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifaFifamIDsEnvironment.h"

class DbCommands {
public:
    void UpdateClubLink(FifamClubLink &link, FifamClub *oldClub, FifamClub *newClub, FifamClub *thisClub = nullptr, FifamClub *futureClub = nullptr) {
        if (link.mPtr == oldClub) {
            link.mPtr = newClub;
            if (newClub && (thisClub == newClub || futureClub == newClub))
                link.mPtr = nullptr;
        }
    }

    bool DeleteClub(FifamDatabase *db, UInt clubId, Bool keepPlayers, UInt destinationClub) {
        FifamClub *clubToDel = db->GetClubFromUID(clubId);
        if (!clubToDel)
            return ::Error("Unable to find club with UID %08X", clubId);
        if (clubToDel->mIsNationalTeam)
            return ::Error("National teams are not allowed (%08X)", clubId);
        FifamClub *destClub = nullptr;
        if (keepPlayers && destinationClub != 0) {
            destClub = db->GetClubFromUID(destinationClub);
            if (!destClub)
                return ::Error("Unable to find club with UID %08X", destinationClub);
            if (destClub->mIsNationalTeam)
                return ::Error("National teams are not allowed (%08X)", destinationClub);
        }
        
        for (auto c : db->mClubs) {
            if (c != clubToDel) {
                UpdateClubLink(c->mPartnershipClub, clubToDel, destClub, c);
                for (auto &r : c->mRivalClubs)
                    UpdateClubLink(r, clubToDel, destClub, c);
            }
        }

        for (auto p : db->mPlayers) {
            UpdateClubLink(p->mFavouriteClub, clubToDel, destClub);
            for (auto &r : p->mTransferRumors)
                UpdateClubLink(r, clubToDel, destClub, p->mClub, destClub);
            Bool hasOldClub = false;
            for (auto &h : p->mHistory.mEntries) {
                if (h.mClub.mPtr == clubToDel) {
                    if (destClub) {
                        h.mReserveMatches += h.mMatches;
                        h.mReserveGoals += h.mGoals;
                        h.mMatches = 0;
                        h.mGoals = 0;
                    }
                    hasOldClub = true;
                }
                UpdateClubLink(h.mClub, clubToDel, destClub);
            }
            if (destClub && hasOldClub) {
                auto entries = p->mHistory.mEntries;
                p->mHistory.mEntries.clear();
                for (auto &h : entries) {
                    if (h.mClub.mPtr == destClub && !p->mHistory.mEntries.empty() && p->mHistory.mEntries.back().mClub.mPtr == h.mClub.mPtr) {
                        p->mHistory.mEntries.back().mMatches += h.mMatches;
                        p->mHistory.mEntries.back().mGoals += h.mGoals;
                        p->mHistory.mEntries.back().mReserveMatches += h.mReserveMatches;
                        p->mHistory.mEntries.back().mReserveGoals += h.mReserveGoals;
                    }
                    else
                        p->mHistory.mEntries.push_back(h);
                }
            }
            if (p->mContract.mBuyBackClauseClub.IsValid()) {
                UpdateClubLink(p->mContract.mBuyBackClauseClub, clubToDel, destClub, p->mClub, destClub);
                if (p->mContract.mBuyBackClauseEnabled && !p->mContract.mBuyBackClauseClub.IsValid()) {
                    p->mContract.mBuyBackClauseEnabled = false;
                    p->mContract.mBuyBackClauseCost = 0;
                    p->mContract.mBuyBackClauseValidUntil.MakeEmpty();
                }
            }
            if (p->mStartingConditions.mLoan.mLoanedClub.IsValid()) {
                UpdateClubLink(p->mStartingConditions.mLoan.mLoanedClub, clubToDel, destClub, p->mClub, destClub);
                if (p->mStartingConditions.mLoan.Enabled() && !p->mStartingConditions.mLoan.mLoanedClub.IsValid())
                    p->mStartingConditions.mLoan.Disable();
            }
            if (p->mStartingConditions.mFutureTransfer.mNewClub.IsValid()) {
                UpdateClubLink(p->mStartingConditions.mFutureTransfer.mNewClub, clubToDel, destClub, p->mClub, destClub);
                if (p->mStartingConditions.mFutureTransfer.Enabled() && !p->mStartingConditions.mFutureTransfer.mNewClub.IsValid())
                    p->mStartingConditions.mFutureTransfer.Disable();
            }
            if (p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid()) {
                UpdateClubLink(p->mStartingConditions.mFutureLoan.mLoanedClub, clubToDel, destClub, p->mClub, destClub);
                if (p->mStartingConditions.mFutureLoan.Enabled() && !p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid())
                    p->mStartingConditions.mFutureLoan.Disable();
            }
            if (p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid()) {
                UpdateClubLink(p->mStartingConditions.mFutureReLoan.mLoanedClub, clubToDel, destClub, p->mClub, destClub);
                if (p->mStartingConditions.mFutureReLoan.Enabled() && !p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid())
                    p->mStartingConditions.mFutureReLoan.Disable();
            }
        }

        for (auto s : db->mStaffs) {
            UpdateClubLink(s->mFavouriteClub, clubToDel, destClub);
            UpdateClubLink(s->mWouldnSignFor, clubToDel, destClub, destClub);
        }

        auto comps = clubToDel->mCountry->GetCompetitions();
        Vector<Pair<FifamCompLeague *, UInt>> leaguePlaces;
        for (auto &[compId, comp] : comps) {
            if (comp->GetDbType() == FifamCompDbType::League && comp->mID.mType == FifamCompType::League) {
                FifamCompLeague *league = comp->AsLeague();
                for (UInt i = 0; i < league->mTeams.size(); i++) {
                    if (league->mTeams[i].mPtr == clubToDel || (destClub && league->mTeams[i].mPtr == destClub))
                        leaguePlaces.emplace_back(league, i);
                }
            }
        }
        if (!leaguePlaces.empty()) {
            if (destClub) {
                leaguePlaces[0].first->mTeams[leaguePlaces[0].second] = FifamClubLink(destClub, FifamClubTeamType::First);
                if (leaguePlaces.size() > 1) {
                    leaguePlaces[1].first->mTeams[leaguePlaces[1].second] = FifamClubLink(destClub, FifamClubTeamType::Reserve);
                    for (UInt i = 2; i < leaguePlaces.size(); i++)
                        leaguePlaces[i].first->mTeams[leaguePlaces[i].second].Clear();
                }
            }
            else {
                for (auto &[league, place] : leaguePlaces) {
                    if (league->mTeams[place].mPtr == clubToDel)
                        league->mTeams[place].Clear();
                }
            }
        }
        if (keepPlayers) {
            if (destClub) {
                Vector<Bool> ReserveTeamNumbersUsed(100, false);
                for (auto p : destClub->mPlayers) {
                    if (!p->mInYouthTeam && p->mShirtNumberReserveTeam >= 1 && p->mShirtNumberReserveTeam <= 99)
                        ReserveTeamNumbersUsed[p->mShirtNumberReserveTeam - 1] = true;
                }
                auto GetNextAvailableNumber = [&](Vector<UChar> const &priorityNumbers) -> UChar {
                    for (UChar i : priorityNumbers) {
                        if (!ReserveTeamNumbersUsed[i]) {
                            ReserveTeamNumbersUsed[i] = true;
                            return i;
                        }
                    }
                    for (UChar i = 12; i <= 99; i++) {
                        if (!ReserveTeamNumbersUsed[i]) {
                            ReserveTeamNumbersUsed[i] = true;
                            return i;
                        }
                    }
                    return 0;
                };

                for (auto p : clubToDel->mPlayers) {
                    p->mIsCaptain = false;
                    p->mInReserveTeam = !p->mInYouthTeam;
                    if (!p->mInYouthTeam) {
                        UChar number = p->mShirtNumberFirstTeam;
                        if (number == 0 || number > 99 || ReserveTeamNumbersUsed[number])
                            number = p->mShirtNumberReserveTeam;
                        if (number == 0 || number > 99 || ReserveTeamNumbersUsed[number]) {
                            UChar pos = p->mMainPosition.ToInt();
                            if (pos == FifamPlayerPosition::GK)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 1, 12, 25, 23, 13, 99 });
                            else if (pos == FifamPlayerPosition::RB || pos == FifamPlayerPosition::RWB)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 2, 4, 3, 6, 5 });
                            else if (pos == FifamPlayerPosition::LB || pos == FifamPlayerPosition::LWB)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 3, 5, 2, 6, 4 });
                            else if (pos == FifamPlayerPosition::CB || pos == FifamPlayerPosition::SW)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 3, 4, 5, 2, 6 });
                            else if (pos == FifamPlayerPosition::ANC || pos == FifamPlayerPosition::DM)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 4, 6, 5, 7 });
                            else if (pos == FifamPlayerPosition::CM)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 6, 8, 7, 5 });
                            else if (pos == FifamPlayerPosition::RM || pos == FifamPlayerPosition::LM)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 7, 11, 10, 8, 6, 17 });
                            else if (pos == FifamPlayerPosition::AM)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 10, 7, 11, 8, 6, 20 });
                            else if (pos == FifamPlayerPosition::RW || pos == FifamPlayerPosition::LW)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 7, 10, 11, 9, 8, 17 });
                            else if (pos == FifamPlayerPosition::ST || pos == FifamPlayerPosition::CF)
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({ 9, 10, 7, 11, 8, 19 });
                            else
                                p->mShirtNumberReserveTeam = GetNextAvailableNumber({});
                        }
                        else {
                            p->mShirtNumberReserveTeam = number;
                            ReserveTeamNumbersUsed[number] = true;
                        }
                        p->mShirtNumberFirstTeam = 0;
                    }
                    p->mClub = destClub;
                    if (!p->mHistory.mEntries.empty()) {
                        if (p->mHistory.mEntries.back().mStillInThisClub && p->mHistory.mEntries.back().mClub == destClub && p->mHistory.mEntries.back().mLoan)
                            p->mHistory.mEntries.back().mLoan = false;
                    }
                    destClub->mPlayers.push_back(p);
                }
            }
            else {
                for (auto p : clubToDel->mPlayers) {
                    p->mIsCaptain = false;
                    p->mInReserveTeam = false;
                    p->mInYouthTeam = false;
                    p->mClub = nullptr;
                    if (!p->mHistory.mEntries.empty()) {
                        if (p->mHistory.mEntries.back().mStillInThisClub)
                            p->mHistory.mEntries.back().mStillInThisClub = false;
                    }
                    if (p->mStartingConditions.mLoan.Enabled())
                        p->mStartingConditions.mLoan.Disable();
                    if (p->mStartingConditions.mFutureTransfer.Enabled())
                        p->mStartingConditions.mFutureTransfer.Disable();
                    if (p->mStartingConditions.mFutureLoan.Enabled())
                        p->mStartingConditions.mFutureLoan.Disable();
                    if (p->mStartingConditions.mFutureReLoan.Enabled())
                        p->mStartingConditions.mFutureReLoan.Disable();
                }
            }
            for (auto s : clubToDel->mStaffs)
                s->mClub = nullptr;
        }
        else {
            for (auto p : clubToDel->mPlayers) {
                for (auto b : p->mBrothers)
                    b->mBrothers.erase(p);
                for (auto b : p->mCousins)
                    b->mCousins.erase(p);
                db->mPlayers.erase(p);
                db->mPersonsMap.erase(p->mID);
                delete p;
            }
            for (auto s : clubToDel->mStaffs) {
                db->mStaffs.erase(s);
                db->mPersonsMap.erase(s->mID);
                delete s;
            }
        }
        clubToDel->mCountry->mClubs.erase(std::remove(clubToDel->mCountry->mClubs.begin(), clubToDel->mCountry->mClubs.end(), clubToDel), clubToDel->mCountry->mClubs.end());
        clubToDel->mCountry->mClubsMap.erase(clubToDel->mUniqueID);
        db->mClubs.erase(clubToDel);
        db->mClubsMap.erase(clubToDel->mUniqueID);
        delete clubToDel;
        return true;
    }

    void UpdateCompID(FifamCompID &id, UInt league1id, UInt league2id) {
        if (id.ToInt() == league1id)
            id.SetFromInt(league2id);
        else if (id.ToInt() == league2id)
            id.SetFromInt(league1id);
    }

    bool SwapLeagues(FifamDatabase *db, UInt league1id, UInt league2id) {
        auto league1 = db->GetCompetition(league1id);
        if (!league1)
            return ::Error("Unable to find league with ID %08X", league1id);
        if (league1->GetDbType() != FifamCompDbType::League || league1->mID.mType != FifamCompType::League)
            return ::Error("Competition with ID %08X is not a league", league1id);
        auto league2 = db->GetCompetition(league2id);
        if (!league2)
            return ::Error("Unable to find league with ID %08X", league2id);
        if (league2->GetDbType() != FifamCompDbType::League || league2->mID.mType != FifamCompType::League)
            return ::Error("Competition with ID %08X is not a league", league2id);
        std::swap(league1->mID, league2->mID);
        db->mCompMap[league1->mID] = league1;
        db->mCompMap[league2->mID] = league2;
        for (auto c : db->mClubs) {
            for (auto &id : c->mLowestLeagues)
                UpdateCompID(id, league1id, league2id);
        }
        for (auto &[id, comp] : db->mCompMap) {
            for (auto &id : comp->mPredecessors)
                UpdateCompID(id, league1id, league2id);
            for (auto &id : comp->mSuccessors)
                UpdateCompID(id, league1id, league2id);
            comp->mInstructions.ForAllCompetitionLinks([=](FifamCompID &id, UInt, FifamAbstractInstruction *) {
                UpdateCompID(id, league1id, league2id);
            });
            if (comp->AsPool()) {
                for (auto &id : comp->AsPool()->mCompConstraints)
                    UpdateCompID(id, league1id, league2id);
            }
        }
        return true;
    }

    DbCommands() {
        FifamReader scriptReader("commands.txt");
        if (!scriptReader.Available()) {
            ::Error("Unable to open commands.txt");
            return;
        }

        enum CommandID {
            DELETE_CLUB_KEEP_PLAYERS,     // deleteClubKeepPlayers 000E0001
            DELETE_CLUB_WITH_PLAYERS,     // deleteClubWithPlayers 000E0001
            CONVERT_CLUB_TO_RESERVE_TEAM, // convertClubToReserveTeam 000E0001 000E0002
            SWAP_LEAGUES,                 // swapLeagues 0E010000 0E010001
            UPDATE_CITY_NAMES,            // updateCityNames
            UNDEFINED
        };

        struct Command {
            CommandID commandid = CommandID::UNDEFINED;
            UInt entities[2] = { 0, 0 };
        };

        WideChar const *commandNames[] = {
            L"deleteClubKeepPlayers",
            L"deleteClubWithPlayers",
            L"convertClubToReserveTeam",
            L"swapLeagues",
            L"updateCityNames"
        };

        Vector<Command> commands;
        bool doReturn = false;

        while (!scriptReader.IsEof()) {
            if (!scriptReader.EmptyLine()) {
                auto line = scriptReader.ReadFullLine();
                auto lineParts = Utils::Split(line, L' ');
                if (lineParts.size() >= 1) {
                    Command c;
                    auto commandName = Utils::ToLower(lineParts[0]);
                    for (UInt i = 0; i < std::size(commandNames); i++) {
                        if (commandName == Utils::ToLower(commandNames[i])) {
                            c.commandid = (CommandID)i;
                            break;
                        }
                    }
                    if (c.commandid == DELETE_CLUB_KEEP_PLAYERS || c.commandid == DELETE_CLUB_WITH_PLAYERS) {
                        if (lineParts.size() != 2) {
                            doReturn = true;
                            ::Error(L"Wrong command syntax (%s)", commandName.c_str());
                            break;
                        }
                        c.entities[0] = Utils::SafeConvertInt<UInt>(lineParts[1], true);
                    }
                    else if (c.commandid == CONVERT_CLUB_TO_RESERVE_TEAM || c.commandid == SWAP_LEAGUES) {
                        if (lineParts.size() != 3) {
                            doReturn = true;
                            ::Error(L"Wrong command syntax (%s)", commandName.c_str());
                            break;
                        }
                        c.entities[0] = Utils::SafeConvertInt<UInt>(lineParts[1], true);
                        c.entities[1] = Utils::SafeConvertInt<UInt>(lineParts[2], true);
                    }
                    else if (c.commandid == UPDATE_CITY_NAMES) {

                    }
                    else {
                        doReturn = true;
                        ::Error(L"Unknown command: %s", commandName.c_str());
                        break;
                    }
                    commands.push_back(c);
                }
            }
            else
                scriptReader.SkipLine();
        }

        if (doReturn)
            return;

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        //FifamDatabase::mReadingOptions.mReadPersons = false; // TODO: remove this
        auto db = new FifamDatabase(13, "db");

        UInt numPersons = db->mPlayers.size() + db->mStaffs.size();

        for (auto const &c : commands) {
            if (c.commandid == DELETE_CLUB_KEEP_PLAYERS) {
                wprintf(L"deleteClubKeepPlayers %08X\n", c.entities[0]);
                DeleteClub(db, c.entities[0], true, 0);
            }
            else if (c.commandid == DELETE_CLUB_WITH_PLAYERS) {
                wprintf(L"deleteClubWithPlayers %08X\n", c.entities[0]);
                DeleteClub(db, c.entities[0], false, 0);
            }
            else if (c.commandid == CONVERT_CLUB_TO_RESERVE_TEAM) {
                wprintf(L"convertClubToReserveTeam %08X %08X\n", c.entities[0], c.entities[1]);
                DeleteClub(db, c.entities[0], true, c.entities[1]);
            }
            else if (c.commandid == SWAP_LEAGUES) {
                wprintf(L"swapLeagues %08X %08X\n", c.entities[0], c.entities[1]);
                SwapLeagues(db, c.entities[0], c.entities[1]);
            }
            else if (c.commandid == UPDATE_CITY_NAMES) {
                wprintf(L"updateCityNames\n");
                FifamDatabase::mReadingOptions.mUseCustomFormations = true;
                FifamDatabase::mReadingOptions.mReadPersons = false;
                auto db2 = new FifamDatabase(13, "db2");
                for (auto c : db->mClubs) {
                    auto c2 = db2->GetClubFromUID(c->mUniqueID);
                    if (c2)
                        c->mCityName[FifamTranslation::German] = c2->mCityName[FifamTranslation::German];
                }
            }
        }

        if ((db->mPlayers.size() + db->mStaffs.size()) != numPersons) {
            db->mPersonsMap.clear();
            UInt id = 1;
            for (auto p : db->mPlayers) {
                p->mID = id;
                db->mPersonsMap[id] = p;
                id++;
            }
            for (auto s : db->mStaffs) {
                s->mID = id;
                db->mPersonsMap[id] = s;
                id++;
            }
        }

        db->mWritingOptions.mWriteAssessment = false;
        db->mWritingOptions.mWriteFixtures = false;
        db->Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
