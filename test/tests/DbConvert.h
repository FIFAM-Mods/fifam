#pragma once
#include "FifamDbEnvironment.h"

class DbConvert {
public:
    DbConvert() {
        const UInt SourceGameId = 11;
        const Path SourceDbPath = L"D:\\Games\\FIFA MANAGER 11\\database";
        const UInt SourceDbYear = 2019; // database year

        const Path SourcePortraitsPath = L"D:\\Games\\FIFA MANAGER 11\\portraits\\club\\160x160";
        
        const UInt TargetGameId =  8;
        const Bool DecreaseYears = false;
        const Bool ConvertGraphics = false;

        const Bool WriteToGameFolder = true;

        auto db = new FifamDatabase(SourceGameId, SourceDbPath);

        Int YearsDiff = (1999 + TargetGameId) - SourceDbYear;

        auto UpdateYear = [&](UShort &year) {
            if (year != 0)
                year += YearsDiff;
        };

        auto UpdateYears = [&](Vector<UShort> &years) {
            for (auto &y : years)
                UpdateYear(y);
        };

        auto UpdateDate = [&](FifamDate &date) {
            if (!date.IsEmpty()) {
                date.year += YearsDiff;
                if (date.day == 29 && date.month == 2 && !((date.year + YearsDiff) % 4))
                    date.day = 28;
            }
        };

        auto UpdateCountry = [&](FifamCountry *c) {
            
        };

        auto UpdateClub = [&](FifamClub *c) {
            UpdateYear(c->mYearOfFoundation);
            UpdateYear(c->mHistory.mRecordHomeWin.mYear);
            UpdateYear(c->mHistory.mRecordHomeDefeat.mYear);
            UpdateYear(c->mHistory.mRecordAwayWin.mYear);
            UpdateYear(c->mHistory.mRecordAwayDefeat.mYear);
            UpdateYears(c->mHistory.mLeagueWinYears);
            UpdateYears(c->mHistory.mCupWinYears);
            UpdateYears(c->mHistory.mChampionsCupWinYears);
            UpdateYears(c->mHistory.mEuroTrophyWinYears);
            UpdateYears(c->mHistory.mLeagueCupWinYears);
            UpdateYears(c->mHistory.mSuperCupsWinYears);
            UpdateYears(c->mHistory.mWorldChampionshipWinYears);
            UpdateYears(c->mHistory.mWorldClubChampionshipWinYears);
        };

        auto UpdateStaff = [&](FifamStaff *s) {
            UpdateDate(s->mBirthday);
            UpdateDate(s->mJoinedClubDate);
        };

        auto UpdatePlayer = [&](FifamPlayer *p) {
            UpdateDate(p->mBirthday);
            UpdateDate(p->mContract.mJoined);
            UpdateDate(p->mContract.mValidUntil);
            UpdateDate(p->mContract.mBuyBackClauseValidUntil);

            auto history = p->mHistory.mEntries;
            p->mHistory.mEntries.clear();
            for (auto h : history) {
                UpdateDate(h.mStartDate);
                UpdateDate(h.mEndDate);
                p->mHistory.mEntries.push_back(h);
            }

            if (p->mStartingConditions.mBanUntil.Enabled())
                UpdateDate(p->mStartingConditions.mBanUntil.mDate);
            if (p->mStartingConditions.mFutureJoin.Enabled())
                UpdateDate(p->mStartingConditions.mFutureJoin.mDate);
            if (p->mStartingConditions.mFutureLeave.Enabled())
                UpdateDate(p->mStartingConditions.mFutureLeave.mDate);
            if (p->mStartingConditions.mFutureLoan.Enabled()) {
                UpdateDate(p->mStartingConditions.mFutureLoan.mStartDate);
                UpdateDate(p->mStartingConditions.mFutureLoan.mEndDate);
            }
            if (p->mStartingConditions.mFutureReLoan.Enabled()) {
                UpdateDate(p->mStartingConditions.mFutureReLoan.mStartDate);
                UpdateDate(p->mStartingConditions.mFutureReLoan.mEndDate);
            }
            if (p->mStartingConditions.mFutureTransfer.Enabled()) {
                UpdateDate(p->mStartingConditions.mFutureTransfer.mTransferDate);
                UpdateDate(p->mStartingConditions.mFutureTransfer.mNewContractEndDate);
            }
            if (p->mStartingConditions.mInjury.Enabled()) {
                UpdateDate(p->mStartingConditions.mInjury.mStartDate);
                UpdateDate(p->mStartingConditions.mInjury.mEndDate);
            }
            if (p->mStartingConditions.mLoan.Enabled()) {
                UpdateDate(p->mStartingConditions.mLoan.mStartDate);
                UpdateDate(p->mStartingConditions.mLoan.mEndDate);
            }
        };

        db->SetupWriteableStatus(SourceGameId);
        for (auto p : db->mPlayers) {
            p->SetProperty<String>(L"sourceId", p->mWriteableStringID);
            p->SetProperty<String>(L"sourceId_noEmpics", p->GetStringUniqueId(SourceGameId, false));
        }
        db->ResetWriteableStatus();

        if (DecreaseYears && YearsDiff != 0) {
            for (auto c : db->mCountries) {
                if (c) {
                    UpdateCountry(c);
                    UpdateClub(&c->mNationalTeam);
                }
            }
            for (auto c : db->mClubs) {
                UpdateClub(c);
            }
            for (auto p : db->mPlayers)
                UpdatePlayer(p);
            for (auto s : db->mStaffs)
                UpdateStaff(s);
        }

        Path gamePath = Utils::Format(L"D:\\Games\\FIFA Manager %02d", TargetGameId);

        db->Write(TargetGameId, FifamDatabase::GetGameDbVersion(TargetGameId),
            WriteToGameFolder ? (gamePath / L"database") : L"fm_test\\database");

        if (ConvertGraphics) {
            Path portraitsPath;
            if (TargetGameId > 9)
                portraitsPath = gamePath / L"portraits" / "club" / "160x160";
            else
                portraitsPath = gamePath / L"art" / L"picture";
            create_directories(portraitsPath);
            for (auto p : db->mPlayers) {
                Bool hasPic = false;
                String targetId;
                Path sourcePic = SourcePortraitsPath / (p->GetProperty<String>(L"sourceId") + L".tga");
                Bool noEmpicsId = false;
                if (exists(sourcePic)) {
                    hasPic = true;
                    targetId = p->mWriteableStringID;
                }
                else {
                    sourcePic = SourcePortraitsPath / (p->GetProperty<String>(L"sourceId_noEmpics") + L".tga");
                    if (exists(sourcePic)) {
                        hasPic = true;
                        targetId = p->GetStringUniqueId(TargetGameId, false);
                    }
                }
                if (hasPic)
                    copy(sourcePic, portraitsPath / (targetId + L".tga"), copy_options::overwrite_existing);
            }
        }
    }
};
