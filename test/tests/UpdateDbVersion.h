#pragma once
#include "FifamDatabase.h"

class UpdateDbVersion {
public:
    unsigned int GetCreator(Int footballManagerId, Set<Int> const &ids) {
        if (footballManagerId >= 0 && Utils::Contains(ids, footballManagerId))
            return 2;
        return 0;
    }

    void ConvertDbTo20130E(FifamDatabase &db, FifamDatabase &initialDb) {
        Set<Int> initialPlayersIds;
        for (auto p : initialDb.mPlayers) {
            if (p->mFootballManagerID >= 0)
                initialPlayersIds.insert(p->mFootballManagerID);
        }
        Set<Int> initialStaffIds;
        for (auto s : initialDb.mStaffs) {
            if (s->mFootballManagerID >= 0)
                initialStaffIds.insert(s->mFootballManagerID);
        }
        Set<Int> initialRefereeIds;
        for (auto r : initialDb.mReferees) {
            if (r->mFootballManagerID >= 0)
                initialRefereeIds.insert(r->mFootballManagerID);
        }
        db.SetupWriteableStatus(13);
        Map<UInt, Vector<FifamPlayer *>> fifaIds;
        for (FifamPlayer *p : db.mPlayers) {
            p->mCreator = GetCreator(p->mFootballManagerID, initialPlayersIds);
            if (p->mEmpicsId != 0) {
                auto pp = p->mWriteableStringID.rfind(L'-');
                if (pp == String::npos || !Utils::IsNumber(p->mWriteableStringID.substr(pp + 1)))
                    p->mEmpicsId = 0;
            }
            if (!p->mComment.empty()) {
                auto commentParams = Utils::Split(p->mComment, L';', true, true, false);
                for (auto const &commentParam : commentParams) {
                    auto paramPair = Utils::Split(commentParam, L':', true, true, false);
                    if (paramPair.size() == 2) {
                        auto paramName = Utils::ToLower(paramPair[0]);
                        if (paramName == L"fifaid") {
                            p->mFifaID = Utils::SafeConvertInt<UInt>(paramPair[1]);
                            fifaIds[p->mFifaID].push_back(p);
                        }
                    }
                }
                p->mComment.clear();
            }
        }
        for (auto s : db.mStaffs)
            s->mCreator = GetCreator(s->mFootballManagerID, initialStaffIds);
        for (auto r : db.mReferees)
            r->mCreator = GetCreator(r->mFootballManagerID, initialRefereeIds);
        Vector<String> duplicatedPlayers;
        for (auto const &[fifaId, players] : fifaIds) {
            if (players.size() > 1) {
                for (auto const &p : players)
                    duplicatedPlayers.push_back(Utils::Format(L"%u - %s (%s, %u)", fifaId, p->GetName(), p->mWriteableStringID, p->mFootballManagerID));
            }
        }
        if (!duplicatedPlayers.empty()) {
            wcout << "Duplicated players:" << endl;
            for (auto const &s : duplicatedPlayers)
                wcout << s << endl;
        }
    }
    
    void CopyInjuries(FifamDatabase &db, FifamDatabase &initialDb) {
        FifamWriter log("log_injuries.txt");
        Map<Int, FifamPlayer *> initialPlayers;
        for (auto p : initialDb.mPlayers) {
            if (p->mFootballManagerID >= 0)
                initialPlayers[p->mFootballManagerID] = p;
        }
        for (auto p : db.mPlayers) {
            if (p->mFootballManagerID >= 0 && Utils::Contains(initialPlayers, p->mFootballManagerID)) {
                if (!p->mStartingConditions.mInjury.Enabled()) {
                    auto &injury = initialPlayers[p->mFootballManagerID]->mStartingConditions.mInjury;
                    if (injury.Enabled()) {
                        //if (injury.mStartDate == FifamDate(30, 6, 2023)) {
                            p->mStartingConditions.mInjury.Setup(injury.mStartDate, injury.mEndDate, injury.mType);
                            log.WriteLine(Utils::Format(L"Re-added injury (%s, %s-%s) for player %s (%d)", injury.mType.ToStr(),
                                injury.mStartDate.ToString(), injury.mEndDate.ToString(), p->GetName(), p->mFootballManagerID));
                        //}
                    }
                }
                else {
                    auto &injury = initialPlayers[p->mFootballManagerID]->mStartingConditions.mInjury;
                    if (injury.Enabled() && (
                        p->mStartingConditions.mInjury.mStartDate != injury.mStartDate ||
                        p->mStartingConditions.mInjury.mEndDate != injury.mEndDate ||
                        p->mStartingConditions.mInjury.mType != injury.mType
                        )
                        )
                    {
                        String dateWasModified;
                        if (p->mStartingConditions.mInjury.mStartDate != injury.mStartDate ||
                            p->mStartingConditions.mInjury.mEndDate != injury.mEndDate
                            )
                        {
                            dateWasModified = L" - date was modified";
                        }
                        log.WriteLine(Utils::Format(L"Injury was changed (%s, %s-%s) for player %s (%d)%s",
                            injury.mType.ToStr(), injury.mStartDate.ToString(), injury.mEndDate.ToString(), p->GetName(),
                            p->mFootballManagerID, dateWasModified));
                    }
                }
            }
        }
    }
    
    void CopyKitSettings(FifamDatabase &db, FifamDatabase &kitsDb) {
        FifamWriter log("log_teamkits.txt");
        for (UChar i = 0; i < db.NUM_COUNTRIES; i++) {
            if (db.mCountries[i] && kitsDb.mCountries[i]) {
                db.mCountries[i]->mNationalTeam.mKit = kitsDb.mCountries[i]->mNationalTeam.mKit;
                log.WriteLine(L"Updated kit for " + FifamTr(db.mCountries[i]->mNationalTeam.mName));
            }
            if (db.mCountries[i]->mId == FifamNation::Ukraine) {
                for (auto c : db.mCountries[i]->mClubs) {
                    auto oldClub = kitsDb.GetClubFromUID(c->mUniqueID);
                    if (oldClub) {
                        c->mKit = oldClub->mKit;
                        log.WriteLine(L"Updated kit for " + FifamTr(c->mName));
                    }
                }
            }
        }
    }

    void ConvertDbTo20130D(FifamDatabase &db) {
        db.SetupWriteableStatus(13);
        for (FifamPlayer *p : db.mPlayers) {
            if (p->mFifaID != 0)
                p->mComment = L"FIFAID:" + Utils::Format(L"%d", p->mFifaID);
        }
    }

    UpdateDbVersion() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        FifamDatabase initialDb(13, "db_initial");
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase kitsDb(13, "db_kits");
        CopyKitSettings(db, kitsDb);
        CopyInjuries(db, initialDb);
        ConvertDbTo20130E(db, initialDb);
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
