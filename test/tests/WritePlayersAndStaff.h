#pragma once
#include "FifamDatabase.h"
#include "FifamCompLeague.h"
#include "FifamPlayerLevel.h"

class WritePlayersAndStaff {
public:
    WritePlayersAndStaff() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        db.SetupWriteableStatus(13, false);
        Map<FifamClub *, FifamCompLeague *> clubLeague;
        for (auto const &[id, c] : db.mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid() && t.IsFirstTeam())
                        clubLeague[t.mPtr] = l;
                }
            }
        }
        FifamWriter writer(L"players_and_staff.txt");
        if (writer.Available()) {
            writer.WriteLineWithSeparator(L'\t',
                L"Country",
                L"Club",
                L"Creator",
                L"ID",
                L"StringID",
                L"FoomID",
                L"FifaID",
                L"EmpicsId",
                L"Name",
                L"LastName",
                L"FirstName",
                L"Pseudonym",
                L"Nickname",
                L"Position",
                L"Level",
                L"Talent",
                L"Nationality1",
                L"Nationality2",
                L"Language1",
                L"Language2",
                L"Language3",
                L"Language4",
                L"Birthday",
                L"InReserveTeam",
                L"InYouthTeam",
                L"IsNaturalised",
                L"IsBasque",
                L"IsRealPlayer",
                L"SpecialFace",
                L"Height",
                L"Weight",
                L"NTMatches",
                L"NTGoals"
            );
            for (auto p : db.mPlayers) {
                String club, country;
                Int league = 255;
                if (p->mClub) {
                    club = FifamTr(p->mClub->mName);
                    country = FifamTr(p->mClub->mCountry->mName);
                    if (clubLeague.find(p->mClub) != clubLeague.end())
                        league = clubLeague[p->mClub]->mLeagueLevel + 1;
                }
                writer.WriteLineWithSeparator(L'\t',
                    country,
                    club,
                    p->mCreator,
                    p->mID,
                    p->mWriteableStringID,
                    p->mFootballManagerID,
                    p->mFifaID,
                    p->mEmpicsId,
                    p->GetName(),
                    p->mLastName,
                    p->mFirstName,
                    p->mPseudonym,
                    p->mNickname,
                    p->mMainPosition.ToStr(),
                    p->GetLevel(),
                    p->mTalent,
                    p->mNationality[0].ToStr(),
                    p->mNationality[1].ToStr(),
                    p->mLanguages[0].ToStr(),
                    p->mLanguages[1].ToStr(),
                    p->mLanguages[2].ToStr(),
                    p->mLanguages[3].ToStr(),
                    p->mBirthday.ToString(),
                    p->mInReserveTeam,
                    p->mInYouthTeam,
                    p->mIsNaturalised,
                    p->mIsBasque,
                    p->mIsRealPlayer,
                    p->mSpecialFace,
                    p->mHeight,
                    p->mWeight,
                    p->mNationalTeamMatches,
                    p->mNationalTeamGoals
                );
            }
            for (auto p : db.mStaffs) {
                String club, country;
                Int league = 255;
                if (p->mClub) {
                    club = FifamTr(p->mClub->mName);
                    country = FifamTr(p->mClub->mCountry->mName);
                    if (clubLeague.find(p->mClub) != clubLeague.end())
                        league = clubLeague[p->mClub]->mLeagueLevel + 1;
                }
                UInt level = 0;
                if (p->mClubPosition == FifamClubStaffPosition::Manager || p->mClubPosition == FifamClubStaffPosition::ChiefExec || p->mClubPosition == FifamClubStaffPosition::President)
                    level = p->GetManagerLevel() * 99 / 15;
                else
                    level = p->GetStaffLevel();
                writer.WriteLineWithSeparator(L'\t',
                    country,
                    club,
                    p->mCreator,
                    p->mID,
                    p->mWriteableStringID,
                    p->mFootballManagerID,
                    0,
                    0,
                    p->GetName(),
                    p->mLastName,
                    p->mFirstName,
                    p->mPseudonym,
                    p->mNickname,
                    p->mClubPosition.ToStr(),
                    level,
                    p->mTalent,
                    p->mNationality[0].ToStr(),
                    p->mNationality[1].ToStr(),
                    p->mLanguages[0].ToStr(),
                    p->mLanguages[1].ToStr(),
                    p->mLanguages[2].ToStr(),
                    p->mLanguages[3].ToStr(),
                    p->mBirthday.ToString(),
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0
                );
            }
        }
    }
};
