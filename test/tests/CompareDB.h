#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "FifamNames.h"
#include "FifamCompLeague.h"

String FixPersonName(String const &name, UInt gameId) {
    String result;
    static Set<WideChar> restrictedChars = { L',', L'|', L':', L'<', L'>', L'/', L'\\', L'?', L'*', 0x327, 0x301, 0x308, 0x30C, 0x200E };
    for (auto c : name) {
        if (!Utils::Contains(restrictedChars, c)) {
            if (c == L'Ị')
                result += L'I';
            else if (c == L'ị')
                result += L'i';
            else if (c == L'Ọ')
                result += L'O';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'þ')
                result += L"th";
            else if (c == L'Ə')
                result += L'A';
            else if (c == L'ə')
                result += L'a';
            else if (c == L'ü')
                result += L'u';
            else if (c == L'ứ')
                result += L'u';
            else if (c == L'Ș')
                result += L'S';
            else if (c == L'ș')
                result += L's';
            else if (c == L'ë')
                result += L'e';
            else if (c == L'с')
                result += L'c';
            else if (c == L'´')
                result += L'\'';
            else if (gameId >= 13)
                result += c;
            else {
                if (c == L'Ț')
                    result += L"T";
                else if (c == L'ț')
                    result += L"t";
                else if (c == L'ð')
                    result += L'd';
                else
                    result += c;
            }
        }
    }
    return result;
}

class CompareDB {
public:
    static String NationName(FifamDatabase *db, FifamNation const &id) {
        if (id.ToInt() >= 1 && id.ToInt() <= FifamDatabase::NUM_COUNTRIES && db->mCountries[id.ToInt() - 1])
            return FifamTr(db->mCountries[id.ToInt() - 1]->mName);
        return String();
    }

    static String ClubName(FifamClubLink const &c) {
        if (c.IsValid())
            return L"\"" + (c.mPtr->mName[FifamTranslation::English]) + L"\"";
        return L"\"(NONE)\"";
    }

    static String ClubName(FifamClub *c) {
        if (c)
            return L"\"" + (c->mName[FifamTranslation::English]) + L"\"";
        return L"\"(NONE)\"";
    }

    static String ClubNameAndNation(FifamClub *c) {
        if (c)
            return L"\"" + FifamTr(c->mName) + L" (" + FifamTr(c->mCountry->mName) +  L")\"";
        return L"\"(NONE)\"";
    }

    static UInt ClubID(FifamClubLink const &c) {
        if (c.IsValid())
            return c.mPtr->mUniqueID;
        return 0;
    }

    CompareDB() {
        UInt const gameVersion = 13;
        Path dbPath1 = L"db1";
        Path dbPath2 = L"db2";
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *db1 = new FifamDatabase(gameVersion, dbPath1);
        FifamDatabase *db2 = new FifamDatabase(gameVersion, dbPath2);
        db1->SetupWriteableStatus(gameVersion, false);
        Vector<Pair<FifamPlayer *, UInt>> newEmpicsIDs;
        Vector<Pair<FifamPlayer *, UInt>> removedEmpicsIDs;
        db2->SetupWriteableStatus(gameVersion, true, newEmpicsIDs, removedEmpicsIDs);

        Map<FifamClub *, FifamCompLeague *> clubLeague1;
        Map<FifamClub *, FifamCompLeague *> clubLeague2;

        for (auto const &[id, c] : db1->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid() && t.IsFirstTeam())
                        clubLeague1[t.mPtr] = l;
                }
            }
        }
        for (auto const &[id, c] : db2->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid() && t.IsFirstTeam())
                        clubLeague2[t.mPtr] = l;
                }
            }
        }
        Vector<Pair<String, String>> renPlayersAndStaff;
        Vector<Pair<String, String>> renXXL;
        Vector<Pair<String, String>> renReferees;
        // empics IDs
        {
            FifamWriter w(L"empics_id_new.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &[p, id] : newEmpicsIDs) {
                UInt clubLeagueLevel = 255;
                String clubName, clubCountry;
                if (p->mClub) {
                    clubName = FifamTr(p->mClub->mName);
                    clubCountry = FifamTr(p->mClub->mCountry->mName);
                    if (Utils::Contains(clubLeague1, p->mClub))
                        clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                }
                w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->mEmpicsId, p->GetName(),
                    p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname, p->mBirthday.ToString(),
                    NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(),
                    p->mTalent);
            }
        }
        {
            FifamWriter w(L"empics_id_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"empicsid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &[p, id] : removedEmpicsIDs) {
                UInt clubLeagueLevel = 255;
                String clubName, clubCountry;
                if (p->mClub) {
                    clubName = FifamTr(p->mClub->mName);
                    clubCountry = FifamTr(p->mClub->mCountry->mName);
                    if (Utils::Contains(clubLeague1, p->mClub))
                        clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                }
                w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->mEmpicsId, p->GetName(),
                    p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname, p->mBirthday.ToString(),
                    NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(),
                    p->mTalent);
            }
        }
        // clubs
        {
            FifamWriter w(L"clubs_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db1->mClubsMap) {
                if (!Utils::Contains(db2->mClubsMap, id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        {
            FifamWriter w(L"clubs_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db2->mClubsMap) {
                if (!Utils::Contains(db1->mClubsMap, id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        // referees
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db2->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db1->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!Utils::Contains(referees, id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db1->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!Utils::Contains(referees, id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<Int, FifamReferee *> referees;
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID >= 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID >= 0 && !Utils::Contains(referees, r->mFootballManagerID)) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        {
            Map<Int, FifamReferee *> referees;
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID >= 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID >= 0 && Utils::Contains(referees, r->mFootballManagerID)) {
                    auto ro = referees[r->mFootballManagerID];
                    String id1 = FifamNames::GetPersonStringId(gameVersion, ro->mFirstName, ro->mLastName, String(), Date(), 0);
                    String id2 = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    if (id1 != id2) {
                        w.WriteLineWithSeparator(L'\t', id1, id2, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                        renReferees.emplace_back(id1, id2);
                    }
                }
            }
        }
        {
            FifamWriter w(L"foom_referee_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID < 0) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        {
            Map<Int, FifamReferee *> referees;
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID >= 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID >= 0 && !Utils::Contains(referees, r->mFootballManagerID)) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        {
            FifamWriter w(L"referee_created.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mCreator != 2) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mCreator, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        // staff
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db2->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db1->mStaffs) {
                if (!Utils::Contains(staffs, s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague1, s->mClub))
                            clubLeagueLevel = clubLeague1[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (!Utils::Contains(staffs, s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<Int, FifamStaff *> staffs;
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID >= 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID >= 0 && !Utils::Contains(staffs, s->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague1, s->mClub))
                            clubLeagueLevel = clubLeague1[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<Int, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID >= 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID >= 0 && !Utils::Contains(staffs, s->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<Int, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID >= 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID >= 0 && Utils::Contains(staffs, s->mFootballManagerID)) {
                    FifamStaff *olds = staffs[s->mFootballManagerID];
                    if (s->mWriteableStringID != olds->mWriteableStringID) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (s->mClub) {
                            clubName = FifamTr(s->mClub->mName);
                            clubCountry = FifamTr(s->mClub->mCountry->mName);
                            if (Utils::Contains(clubLeague2, s->mClub))
                                clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                        }
                        UInt level = 0;
                        if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                            level = s->GetManagerLevel() * 99 / 15;
                        else
                            level = s->GetStaffLevel();
                        w.WriteLineWithSeparator(L'\t', olds->mWriteableStringID, s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                            s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                        renPlayersAndStaff.emplace_back(olds->mWriteableStringID, s->mWriteableStringID);
                    }
                }
            }
        }
        {
            FifamWriter w(L"foom_staff_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID < 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            FifamWriter w(L"staff_created.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mCreator != 2) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->mCreator, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        // players
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (!Utils::Contains(players, p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague1, p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (!Utils::Contains(players, p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            FifamWriter w(L"foom_players_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID < 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<Int, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID >= 0 && !Utils::Contains(players, p->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague1, p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<Int, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID >= 0 && !Utils::Contains(players, p->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            auto SplitStringID = [](String const &stringID, UInt empicsID, String &outName, String &outDate, String &outEmpicsID) {
                outEmpicsID.clear();
                UInt empicsIDLength = 0;
                if (empicsID != 0) {
                    String empicsIDStr = Utils::Format(L"-%d", empicsID);
                    if (Utils::EndsWith(stringID, empicsIDStr)) {
                        outEmpicsID = empicsIDStr.substr(1);
                        empicsIDLength = empicsIDStr.length();
                    }
                }
                outName = stringID.substr(0, stringID.size() - 8 - empicsIDLength);
                outDate = stringID.substr(stringID.size() - 8 - empicsIDLength, 8);
            };
            Map<Int, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", "change_type", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                try {
                    if (p->mFootballManagerID >= 0 && Utils::Contains(players, p->mFootballManagerID)) {
                        FifamPlayer *oldp = players[p->mFootballManagerID];
                        if (p->mWriteableStringID != oldp->mWriteableStringID) {
                            UInt clubLeagueLevel = 255;
                            String clubName, clubCountry;
                            if (p->mClub) {
                                clubName = FifamTr(p->mClub->mName);
                                clubCountry = FifamTr(p->mClub->mCountry->mName);
                                if (Utils::Contains(clubLeague2, p->mClub))
                                    clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                            }
                            String name1, name2, date1, date2, empics1, empics2;
                            SplitStringID(p->mWriteableStringID, p->mEmpicsId, name1, date1, empics1);
                            SplitStringID(oldp->mWriteableStringID, oldp->mEmpicsId, name1, date1, empics1);
                            Vector<String> changes;
                            if (name1 != name2)
                                changes.push_back(L"name");
                            if (date1 != date2)
                                changes.push_back(L"date");
                            if (empics1 != empics2)
                                changes.push_back(L"empics");
                            w.WriteLineWithSeparator(L'\t', oldp->mWriteableStringID, p->mWriteableStringID, Utils::Join(changes, L'+') , p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                                p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                            renPlayersAndStaff.emplace_back(oldp->mWriteableStringID, p->mWriteableStringID);
                            renXXL.emplace_back(oldp->mWriteableStringID, p->mWriteableStringID);
                        }
                    }
                }
                catch (exception e) {
                    ::Error("%s\n%d", e.what(), p->mFootballManagerID);
                }
            }
        }
        {
            Map<Int, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_level.txt");
            w.WriteLineWithSeparator(L'\t', L"foomid", L"creator", L"name", L"birthdate", L"nationality", L"clubname",
                L"clubcountry", "clubleaguelevel", L"position", L"oldposition", L"positionchange", L"level", L"oldlevel", L"levelchange", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID >= 0 && Utils::Contains(players, p->mFootballManagerID)) {
                    FifamPlayer *oldp = players[p->mFootballManagerID];
                    if (p->GetLevel() != oldp->GetLevel()) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (Utils::Contains(clubLeague2, p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mFootballManagerID, p->mCreator, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), oldp->mMainPosition.ToStr(), p->mMainPosition != oldp->mMainPosition,
                            p->GetLevel(), oldp->GetLevel(), p->GetLevel() - oldp->GetLevel(), p->mTalent);
                    }
                }
            }
        }
        {
            Map<Int, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_talent.txt");
            w.WriteLineWithSeparator(L'\t', L"foomid", L"creator", L"name", L"birthdate", L"nationality", L"clubname", L"clubcountry",
                "clubleaguelevel", L"position", L"level", L"oldlevel", L"talent", L"oldtalent", L"talentchange");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID >= 0 && Utils::Contains(players, p->mFootballManagerID)) {
                    FifamPlayer *oldp = players[p->mFootballManagerID];
                    if (p->mTalent != oldp->mTalent) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (Utils::Contains(clubLeague2, p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mFootballManagerID, p->mCreator, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), oldp->GetLevel(), p->mTalent, oldp->mTalent,
                            p->mTalent - oldp->mTalent);
                    }
                }
            }
        }
        {
            FifamWriter w(L"players_created.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"creator", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mCreator != 2) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (Utils::Contains(clubLeague2, p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->mCreator, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            FifamWriter w(L"_rename_players_and_staff.bat");
            for (auto const &r : renPlayersAndStaff)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
        {
            FifamWriter w(L"_rename_xxl.bat");
            for (auto const &r : renXXL)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
        {
            FifamWriter w(L"_rename_referees.bat");
            for (auto const &r : renReferees)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
        {
            FifamWriter w(L"_copy_players_and_staff.bat");
            w.WriteLine(L"mkdir renamed");
            for (auto const& r : renPlayersAndStaff)
                w.WriteLine(L"copy /y " + r.first + L".png renamed\\" + r.second + L".png");
        }
        {
            FifamWriter w(L"_copy_xxl.bat");
            w.WriteLine(L"mkdir renamed");
            for (auto const& r : renXXL)
                w.WriteLine(L"copy /y " + r.first + L".png renamed\\" + r.second + L".png");
        }
        {
            FifamWriter w(L"_copy_referees.bat");
            w.WriteLine(L"mkdir renamed");
            for (auto const& r : renReferees)
                w.WriteLine(L"copy /y " + r.first + L".png renamed\\" + r.second + L".png");
        }

        FifamWriter dupWriter(L"duplicates.csv");
        dupWriter.WriteLine(L"Id", L"Type", L"Name1", L"Club1", L"Name2", L"Club2");
        Map<Int, FifamReferee *> mapReferees;
        for (auto const &r : db2->mReferees) {
            if (r->mFootballManagerID >= 0)
                mapReferees[r->mFootballManagerID] = r;
        }
        Map<Int, FifamStaff *> mapStaffs;
        for (auto const &s : db2->mStaffs) {
            if (s->mFootballManagerID >= 0) {
                if (Utils::Contains(mapReferees, s->mFootballManagerID)) {
                    FifamReferee *r = mapReferees[s->mFootballManagerID];
                    dupWriter.WriteLine(s->mFootballManagerID, L"staff-referee",
                        s->GetName(), ClubNameAndNation(s->mClub), r->mFirstName + L" " + r->mLastName, FifamTr(r->mCountry->mName));
                }
                mapStaffs[s->mFootballManagerID] = s;
            }
        }
        Map<Int, FifamPlayer *> mapPlayers;
        for (auto const &p : db2->mPlayers) {
            if (p->mFootballManagerID >= 0) {
                if (Utils::Contains(mapReferees, p->mFootballManagerID)) {
                    FifamReferee *r = mapReferees[p->mFootballManagerID];
                    dupWriter.WriteLine(p->mFootballManagerID, L"player-referee",
                        p->GetName(), ClubNameAndNation(p->mClub), r->mFirstName + L" " + r->mLastName, FifamTr(r->mCountry->mName));
                }
                if (Utils::Contains(mapStaffs, p->mFootballManagerID)) {
                    FifamStaff *s = mapStaffs[p->mFootballManagerID];
                    dupWriter.WriteLine(p->mFootballManagerID, L"player-staff",
                        p->GetName(), ClubNameAndNation(p->mClub), s->GetName(), ClubNameAndNation(s->mClub));
                }
                mapPlayers[p->mFootballManagerID] = p;
            }
        }
        dupWriter.Close();

        Path playerPortraitsPath = "portraits\\club\\160x160";
        Path refereePortraitsPath = "portraits\\Referees\\160x160";
        create_directories(playerPortraitsPath);
        create_directories(refereePortraitsPath);
        auto ConvertPortrait = [](Path const &portraitPath, String const &fileName, Path const &outFolder, FifamWriter &logWriter) {
            try {
                Image portraitImg(portraitPath.string());
                if (portraitImg.baseRows() >= 150 && portraitImg.baseColumns() >= 150) {
                    if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                        UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                        portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    }
                    portraitImg.filterType(FilterType::LanczosFilter);
                    portraitImg.resize(Geometry(160, 160));
                    portraitImg.defineValue("png", "color-type", "6");
                    portraitImg.write((outFolder / (fileName + L".png")).string());
                }
            }
            catch (std::exception e) {
                logWriter.WriteLine(portraitPath, fileName, Utils::AtoW(e.what()));
            }
        };
        FifamWriter portraitsWriter(L"portraits_errors.csv");
        portraitsWriter.WriteLine(L"Path", L"StringID", L"Reason");
        for (auto const &i : directory_iterator("faces")) {
            auto p = i.path();
            if (p.extension() == ".png") {
                Int id = -1;
                try {
                    id = static_cast<Int>(std::stoull(p.stem().string(), 0, 10));
                }
                catch (...) {}
                if (id != -1) {
                    if (Utils::Contains(mapPlayers, id))
                        ConvertPortrait(p, mapPlayers[id]->mWriteableStringID, playerPortraitsPath, portraitsWriter);
                    else if (Utils::Contains(mapStaffs, id))
                        ConvertPortrait(p, mapStaffs[id]->mWriteableStringID, playerPortraitsPath, portraitsWriter);
                    else if (Utils::Contains(mapReferees, id)) {
                        auto r = mapReferees[id];
                        ConvertPortrait(p, FifamNames::GetPersonStringId(13, r->mFirstName, r->mLastName, String(), Date(), 0),
                            refereePortraitsPath, portraitsWriter);
                    }
                }
            }
        }
        portraitsWriter.Close();
    }
};
