#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"

class CalendarDatabase {
public:
    CalendarDatabase() {
        FifamDatabase::mReadingOptions.mReadClubs = true;
        FifamDatabase::mReadingOptions.mReadCountryCompetitions = true;
        FifamDatabase::mReadingOptions.mReadPersons = false;

        FifamDatabase db(13, L"D:\\Games\\FIFA Manager 13\\database");

        for (FifamClub *c : db.mClubs) {

            c->mYearOfFoundation = 1900;
            c->mTraditionalClub = false;
            c->mNationalPrestige = 1;
            c->mInternationalPrestige = 1;
            c->mMediaPressure = FifamClubMediaPressure::Normal;
            c->mIndividualTvMoney = 0;
            c->mInitialCapital = 0;
            c->mTransferBudget = 0;
            c->mJointStockCompany = false;
            c->mRichGuyControlled = false;
            c->mSponsorAmount = 0;
            c->mSponsorDuration = 0;
            c->mSpecialSponsor = false;
            c->mRivalClubs.clear();
            c->mPartnershipClub.Clear();
            c->mFanMembers = 0;
            c->mAverageAttendanceLastSeason = 0;
            c->mCountOfSoldSeasonTickets = 0;
            c->mPotentialFansCount = 0;
            //
            for (UInt i = 0; i < 2; i++) {
                //c->mKit.mSets[i].mShirt = 0;
                //c->mKit.mSets[i].mShorts = 0;
                //c->mKit.mSets[i].mSocks = 0;
                //c->mKit.mSets[i].mCaptainArmbandColor = Color(0, 0, 0);
                //c->mKit.mSets[i].m
            }

            FifamTrSetAll<String>(c->mStadiumName, L"Stadium");
            c->mStadiumSeatsCapacity = 10'000;
            c->mStadiumStandingsCapacity = 0;

            c->mClubFacilities = 0;
            c->mYouthCentre = 1;
            c->mYouthBoardingSchool = 0;
            c->mAiStrategy = FifamClubAiStrategy::General;
            c->mLandscape = FifamClubLandscape::Flat;
            c->mSettlement = FifamClubSettlement::Town;

            c->mLowestLeagues.clear();

            c->mHistory.mChampionsCupWinYears.clear();
            c->mHistory.mCupWinYears.clear();
            c->mHistory.mEuroTrophyWinYears.clear();
            c->mHistory.mLeagueCupWinYears.clear();
            c->mHistory.mLeagueWinYears.clear();
            c->mHistory.mSuperCupsWinYears.clear();
            c->mHistory.mWorldChampionshipWinYears.clear();
            c->mHistory.mWorldClubChampionshipWinYears.clear();

            c->mHistory.mRecordAttendance.mAttendance = 0;
            c->mHistory.mRecordAttendance.mOpponentName.clear();
            c->mHistory.mRecordAttendance.mScore1 = 0;
            c->mHistory.mRecordAttendance.mScore2 = 0;
            c->mHistory.mRecordAttendance.mYear = 0;

            c->mHistory.mRecordHomeWin.mAttendance = 0;
            c->mHistory.mRecordHomeWin.mOpponentName.clear();
            c->mHistory.mRecordHomeWin.mScore1 = 0;
            c->mHistory.mRecordHomeWin.mScore2 = 0;
            c->mHistory.mRecordHomeWin.mYear = 0;

            c->mHistory.mRecordAwayDefeat.mAttendance = 0;
            c->mHistory.mRecordAwayDefeat.mOpponentName.clear();
            c->mHistory.mRecordAwayDefeat.mScore1 = 0;
            c->mHistory.mRecordAwayDefeat.mScore2 = 0;
            c->mHistory.mRecordAwayDefeat.mYear = 0;

            for (UInt i = 0; i < 3; i++) {
                c->mLeagueTotalDraws[i] = 0;
                c->mLeagueTotalGoals[i] = 0;
                c->mLeagueTotalGoalsAgainst[i] = 0;
                c->mLeagueTotalLeadershipsInTable[i] = 0;
                c->mLeagueTotalLoses[i] = 0;
                c->mLeagueTotalPoints[i] = 0;
                c->mLeagueTotalWins[i] = 0;
            }

            c->mPreferredFormations[0] = FifamFormation::None;
            c->mPreferredFormations[1] = FifamFormation::None;
        }

        db.Write(13, FifamDatabase::GetGameDbVersion(13), L"calendar_database\\database");
    }
};

REGISTER_TEST(CalendarDatabase)
