#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "FifamNames.h"
#include "FifamCompLeague.h"

class CompareDB {
public:
    static String NationName(FifamDatabase *db, FifamNation const &id) {
        if (id.ToInt() >= 1 && id.ToInt() <= FifamDatabase::NUM_COUNTRIES && db->mCountries[id.ToInt() - 1])
            return FifamTr(db->mCountries[id.ToInt() - 1]->mName);
        return String();
    }

    CompareDB() {
        UInt const gameVersion = 13;
        Path dbPath1 = L"db1";
        Path dbPath2 = L"db2";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *db1 = new FifamDatabase(gameVersion, dbPath1);
        FifamDatabase *db2 = new FifamDatabase(gameVersion, dbPath2);

        db1->SetupWriteableStatus(gameVersion);
        db2->SetupWriteableStatus(gameVersion);

        Map<FifamClub *, FifamCompLeague *> clubLeague1;
        Map<FifamClub *, FifamCompLeague *> clubLeague2;

        for (auto const &[id, c] : db1->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid())
                        clubLeague1[t.mPtr] = l;
                }
            }
        }
        for (auto const &[id, c] : db2->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid())
                        clubLeague2[t.mPtr] = l;
                }
            }
        }

        {
            FifamWriter w(L"clubs_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db1->mClubsMap) {
                if (!db2->mClubsMap.contains(id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        {
            FifamWriter w(L"clubs_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db2->mClubsMap) {
                if (!db1->mClubsMap.contains(id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db2->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db1->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!referees.contains(id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db1->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!referees.contains(id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db2->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db1->mStaffs) {
                if (!staffs.contains(s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague1.contains(s->mClub))
                            clubLeagueLevel = clubLeague1[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (!staffs.contains(s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague2.contains(s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (!players.contains(p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague1.contains(p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (!players.contains(p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            FifamWriter w(L"foom_players_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (!p->mEmpicsId) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers) {
                if (p->mEmpicsId != 0)
                    players[p->mEmpicsId] = p;
            }
            FifamWriter w(L"foom_players_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (p->mEmpicsId != 0 && !players.contains(p->mEmpicsId)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague1.contains(p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mEmpicsId != 0)
                    players[p->mEmpicsId] = p;
            }
            FifamWriter w(L"foom_players_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mEmpicsId != 0 && !players.contains(p->mEmpicsId)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mEmpicsId != 0)
                    players[p->mEmpicsId] = p;
            }
            FifamWriter w(L"foom_players_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"newid", L"oldid", "change_type", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                try {
                    if (p->mEmpicsId != 0 && players.contains(p->mEmpicsId)) {
                        FifamPlayer *oldp = players[p->mEmpicsId];
                        if (p->mWriteableStringID != oldp->mWriteableStringID) {
                            UInt clubLeagueLevel = 255;
                            String clubName, clubCountry;
                            if (p->mClub) {
                                clubName = FifamTr(p->mClub->mName);
                                clubCountry = FifamTr(p->mClub->mCountry->mName);
                                if (clubLeague2.contains(p->mClub))
                                    clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                            }
                            String namePart1 = p->mWriteableStringID.substr(0, p->mWriteableStringID.size() - 8);
                            String datePart1 = p->mWriteableStringID.substr(p->mWriteableStringID.size() - 8);
                            String namePart2 = oldp->mWriteableStringID.substr(0, oldp->mWriteableStringID.size() - 8);
                            String datePart2 = oldp->mWriteableStringID.substr(oldp->mWriteableStringID.size() - 8);
                            String changeType;
                            if (namePart1 != namePart2)
                                changeType = L"name";
                            if (datePart1 != datePart2) {
                                if (changeType.empty())
                                    changeType = L"date";
                                else
                                    changeType += L"+date";
                            }
                            w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, oldp->mWriteableStringID, changeType, p->mEmpicsId, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                                p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                        }
                    }
                }
                catch (exception e) {
                    ::Error("%s\n%d", e.what(), p->mEmpicsId);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mEmpicsId != 0)
                    players[p->mEmpicsId] = p;
            }
            FifamWriter w(L"foom_players_changed_level.txt");
            w.WriteLineWithSeparator(L'\t', L"empicsid", L"name", L"birthdate", L"nationality", L"clubname",
                L"clubcountry", "clubleaguelevel", L"position", L"oldposition", L"positionchange", L"level", L"oldlevel", L"levelchange", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mEmpicsId != 0 && players.contains(p->mEmpicsId)) {
                    FifamPlayer *oldp = players[p->mEmpicsId];
                    if (p->GetLevel() != oldp->GetLevel()) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (clubLeague2.contains(p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mEmpicsId, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), oldp->mMainPosition.ToStr(), p->mMainPosition != oldp->mMainPosition,
                            p->GetLevel(), oldp->GetLevel(), p->GetLevel() - oldp->GetLevel(), p->mTalent);
                    }
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mEmpicsId != 0)
                    players[p->mEmpicsId] = p;
            }
            FifamWriter w(L"foom_players_changed_talent.txt");
            w.WriteLineWithSeparator(L'\t', L"empicsid", L"name", L"birthdate", L"nationality", L"clubname", L"clubcountry",
                "clubleaguelevel", L"position", L"level", L"oldlevel", L"talent", L"oldtalent", L"talentchange");
            for (auto const &p : db2->mPlayers) {
                if (p->mEmpicsId != 0 && players.contains(p->mEmpicsId)) {
                    FifamPlayer *oldp = players[p->mEmpicsId];
                    if (p->mTalent != oldp->mTalent) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (clubLeague2.contains(p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mEmpicsId, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), oldp->GetLevel(), p->mTalent, oldp->mTalent,
                            p->mTalent - oldp->mTalent);
                    }
                }
            }
        }
    }
};
