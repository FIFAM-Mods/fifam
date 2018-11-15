#pragma once
#include "FifamReadWrite.h"
#include "db_map.h"
#include "foom_nation.h"
#include "foom_comp.h"
#include "foom_club.h"
#include "foom_language.h"
#include "foom_region.h"
#include "foom_injury.h"
#include "foom_official.h"
#include "foom_player.h"
#include "foom_non_player.h"
#include "foom_derby.h"
#include "foom_city.h"
#include "foom_stadium.h"
#include "foom_stage_name.h"
#include <iostream>

namespace foom {

struct db {
    Path mDbPath;
    db_map<language> mLanguages;
    db_map<injury> mInjuries;
    db_map<stage_name> mStageNames;
    db_map<stadium> mStadiums;
    db_map<derby> mDerbies;
    db_map<city> mCities;
    db_map<comp> mComps;
    db_map<nation> mNations;
    db_map<region> mRegions;
    db_map<club> mClubs;
    db_map<player> mPlayers;
    db_map<non_player> mNonPlayers;
    db_map<official> mOfficials;

    template<typename T> T *get(Int id) { return nullptr; }
    template<> nation *get<nation>(Int id) { return map_find_ptr(mNations, id); }
    template<> city *get<city>(Int id) { return map_find_ptr(mCities, id); }
    template<> club *get<club>(Int id) { return map_find_ptr(mClubs, id); }
    template<> comp *get<comp>(Int id) { return map_find_ptr(mComps, id); }
    template<> language *get<language>(Int id) { return map_find_ptr(mLanguages, id); }
    template<> injury *get<injury>(Int id) { return map_find_ptr(mInjuries, id); }
    template<> stage_name *get<stage_name>(Int id) { return map_find_ptr(mStageNames, id); }
    template<> stadium *get<stadium>(Int id) { return map_find_ptr(mStadiums, id); }
    template<> derby *get<derby>(Int id) { return map_find_ptr(mDerbies, id); }
    template<> region *get<region>(Int id) { return map_find_ptr(mRegions, id); }
    template<> player *get<player>(Int id) { return map_find_ptr(mPlayers, id); }
    template<> non_player *get<non_player>(Int id) { return map_find_ptr(mNonPlayers, id); }
    template<> official *get<official>(Int id) { return map_find_ptr(mOfficials, id); }

    static Int convert_money(Int value) {
        Int result = (Int)((Double)value * 1.11394);
        Int lastDigit = result % 10;
        if (lastDigit != 0) {
            if (lastDigit <= 2)
                result -= lastDigit;
            else
                result += 10 - lastDigit;
        }
        return result;
    }

    template<> team *get<team>(Int id) {
        team *result = map_find_ptr(mNations, id);
        if (!result)
            result = map_find_ptr(mClubs, id);
        return result;
    }

    template<> person *get<person>(Int id) {
        person *result = map_find_ptr(mPlayers, id);
        if (!result)
            result = map_find_ptr(mNonPlayers, id);
        return result;
    }

    template<typename T>
    void resolve(T *&entity) {
        Int id = (Int)entity;
        if (id == -1)
            entity = nullptr;
        else
            entity = get<T>(id);
    }

    void ReaderCallback(String const &filename, Function<void(FifamReader &)> callback) {
        std::wcout << L"Reading " << filename << L"..." << std::endl;
        FifamReader reader(mDbPath / (filename + L".csv"), 0);
        if (!reader.Available())
            reader.Open(mDbPath / (filename + L".txt"), 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine())
                    callback(reader);
                else
                    reader.SkipLine();
            }
        }
    }

    enum class db_size { Full, Average, Small, Tiny };

    struct cache_entry {
        String filename;

    };

    db(Path const &dbpath, db_size size = db_size::Full) {
        mDbPath = dbpath;
        std::wcout << L"Reading foom database..." << std::endl;
        // read nations
        ReaderCallback(L"fm_nations", [&](FifamReader &reader) {
            nation n;
            reader.ReadLine(n.mID, n.mName, n.mShortName, n.mNickname, n.mThreeLetterName, n.mNationalityName, n.mContinent, IntPtr(n.mCapitalCity), IntPtr(n.mNationalStadium), n.mReputation, n.mAttendance, n.mMinimumAttendance, n.mMaximumAttendance, n.mForegroundColor.r, n.mForegroundColor.g, n.mForegroundColor.b, n.mBackgroundColor.r, n.mBackgroundColor.g, n.mBackgroundColor.b, n.mYearsToGainNationality, n.mFIFAMember, n.mYearAssociationFormed, n.mRankingPoints, n.mYouthRating, n.mEuroCoeff1, n.mEuroCoeff2, n.mEuroCoeff3, n.mEuroCoeff4, n.mEuroCoeff5, n.mEuroCoeff6, n.mEuroCoeff7, n.mEuroCoeff8, n.mEuroCoeff9, n.mEuroCoeff10, n.mEuroCoeff11, n.mPreferredFormation, n.mSecondPreferredFormation, IntPtr(n.mCaptain), IntPtr(n.mViceCaptain));
            mNations[n.mID] = n;
        });
        ReaderCallback(L"fm_nation_kits", [&](FifamReader &reader) {
            Int nationID = -1;
            kit k;
            reader.ReadLine(nationID, k.mKit, k.mType, k.mKitStyle, k.mBackOfShirtStyle, k.mForeground.r, k.mForeground.g, k.mForeground.b, k.mBackground.r, k.mBackground.g, k.mBackground.b, k.mOutline.r, k.mOutline.g, k.mOutline.b, k.mNumberColour.r, k.mNumberColour.g, k.mNumberColour.b, k.mNumberOutlineColour.r, k.mNumberOutlineColour.g, k.mNumberOutlineColour.b, IntPtr(k.mCompetition), k.mYear, k.mAlternativeKitNumber);
            map_find(mNations, nationID).mVecKits.push_back(k);
        });
        ReaderCallback(L"fm_nation_languages", [&](FifamReader &reader) {
            Int nationID = -1;
            nation::language l;
            reader.ReadLine(nationID, IntPtr(l.mLanguage), l.mPercentage);
            map_find(mNations, nationID).mVecLanguages.push_back(l);
        });
        ReaderCallback(L"fm_nation_region_transfer_preferences", [&](FifamReader &reader) {
            Int nationID = -1;
            nation::region_transfer_preference r;
            reader.ReadLine(nationID, IntPtr(r.mRegion), r.mRating, r.mMinimumAge, r.mMaximumAge);
            map_find(mNations, nationID).mVecRegionTransferPreferences.push_back(r);
        });
        // read competitions
        ReaderCallback(L"fm_comps", [&](FifamReader &reader) {
            comp c;
            reader.ReadLine(c.mID, c.mName, c.mShortName, IntPtr(c.mNation), c.mContinent, IntPtr(c.mParentCompetition), c.mReputation, c.mUsesSeatedStadiumsOnly, c.mIsExtinct, c.mType, c.mCompetitionLevel);
            mComps[c.mID] = c;
        });
        ReaderCallback(L"fm_comp_teams", [&](FifamReader &reader) {
            Int compID = -1;
            team *teamID = nullptr;
            reader.ReadLine(compID, IntPtr(teamID));
            map_find(mComps, compID).mVecTeams.push_back(teamID);
        });
        ReaderCallback(L"fm_comp_history", [&](FifamReader &reader) {
            Int compID;
            comp::history h;
            reader.ReadLine(compID, h.mYear, IntPtr(h.mFirstPlaced), IntPtr(h.mSecondPlaced), IntPtr(h.mThirdPlaced), IntPtr(h.mPlayOffWinner), IntPtr(h.mStageName), h.mYearOrder, h.mNoDataForYear, IntPtr(h.mHostStadium));
            map_find(mComps, compID).mVecHistory.push_back(h);
        });
        // read clubs
        ReaderCallback(L"fm_clubs", [&](FifamReader &reader) {
            club c;
            reader.ReadLine(c.mID, c.mName, c.mShortName, c.mSixLetterName, c.mNickname, c.mExtinct, IntPtr(c.mNation), IntPtr(c.mBasedNation), IntPtr(c.mContinentalCupNation), IntPtr(c.mCity), c.mYearFounded, c.mStatus, c.mReputation, c.mLatitude, c.mLongitude, c.mChairmanTitle, c.mOwnershipType, IntPtr(c.mStadium), c.mAttendance, c.mMinimumAttendance, c.mMaximumAttendance, c.mTraining, c.mYouthCoaching, c.mYouthFacilities, c.mYouthRecruitment, c.mYouthImportance, c.mCorporateFacilities, c.mBalance, c.mTransferBudget, c.mSugarDaddy, c.mNumberOfSeasonTicketHolders, IntPtr(c.mDivision), IntPtr(c.mLastDivision), c.mLastPosition, IntPtr(c.mNextDivision), c.mLastPositionForNextDivision, IntPtr(c.mSecondaryDivision), c.mEuroCoeff1, c.mEuroCoeff2, c.mEuroCoeff3, c.mEuroCoeff4, c.mEuroCoeff5, c.mEuroCoeff6, c.mEuroCoeff7, c.mEuroCoeff8, c.mEuroCoeff9, c.mEuroCoeff10, c.mPreferredFormation, c.mSecondPreferredFormation, IntPtr(c.mRecordWinOpposition), c.mRecordWinTG, c.mRecordWinOG, c.mRecordWinYear, IntPtr(c.mRecordDefeatOpposition), c.mRecordDefeatTG, c.mRecordDefeatOG, c.mRecordDefeatYear, IntPtr(c.mRecordAttendanceOpposition), c.mRecordAttendance, c.mRecordAttendanceYear, IntPtr(c.mCaptain), IntPtr(c.mViceCaptain), c.mBadge, c.mForegroundColor.r, c.mForegroundColor.g, c.mForegroundColor.b, c.mBackgroundColor.r, c.mBackgroundColor.g, c.mBackgroundColor.b, c.mSupporterExpectations);
            mClubs[c.mID] = c;
        });
        ReaderCallback(L"fm_club_kits", [&](FifamReader &reader) {
            Int clubID = -1;
            kit k;
            reader.ReadLine(clubID, k.mKit, k.mType, k.mKitStyle, k.mBackOfShirtStyle, k.mForeground.r, k.mForeground.g, k.mForeground.b, k.mBackground.r, k.mBackground.g, k.mBackground.b, k.mOutline.r, k.mOutline.g, k.mOutline.b, k.mNumberColour.r, k.mNumberColour.g, k.mNumberColour.b, k.mNumberOutlineColour.r, k.mNumberOutlineColour.g, k.mNumberOutlineColour.b, IntPtr(k.mCompetition), k.mYear, k.mAlternativeKitNumber);
            map_find(mClubs, clubID).mVecKits.push_back(k);
        });
        ReaderCallback(L"fm_club_incomes", [&](FifamReader &reader) {
            Int clubID = -1;
            club::income i;
            reader.ReadLine(clubID, i.mAmount, i.mIncomeType, i.mStartDate, i.mEndDate, i.mRenewIncome, i.mFixedValue);
            map_find(mClubs, clubID).mVecIncomes.push_back(i);
        });
        ReaderCallback(L"fm_club_team_league_history", [&](FifamReader &reader) {
            Int clubID = -1;
            club::team_league_history h;
            reader.ReadLine(clubID, h.mYear, h.mOrder, IntPtr(h.mDivision), h.mPosition, h.mMaxTeams, h.mPoints, h.mGamesPlayed, h.mGamesWon, h.mGamesDrawn, h.mGamesLost, h.mGoalsFor, h.mGoalsAgainst);
            map_find(mClubs, clubID).mVecTeamLeagueHistory.push_back(h);
        });
        ReaderCallback(L"fm_club_reserve_teams", [&](FifamReader &reader) {
            Int clubID = -1;
            club::reserve_team r;
            reader.ReadLine(clubID, IntPtr(r.mReserveClub), r.mReserveTeamType);
            map_find(mClubs, clubID).mVecReserveTeams.push_back(r);
            if ((Int)r.mReserveClub != -1) {
                club *reserveClub = get<club>((Int)r.mReserveClub);
                if (!reserveClub) {
                    club rc;
                    rc.mID = (Int)r.mReserveClub;
                    rc.mIsReserveDummyClub = true;
                    mClubs[rc.mID] = rc;
                }
            }
        });
        ReaderCallback(L"fm_club_affiliations", [&](FifamReader &reader) {
            Int clubID = -1;
            club::affiliation a;
            reader.ReadLine(clubID, IntPtr(a.mAffiliatedClub), a.mIsMainClub, a.mStartDate, a.mEndDate, a.mAffiliationType);
            map_find(mClubs, clubID).mVecAffiliations.push_back(a);
        });
        ReaderCallback(L"fm_club_rival_clubs", [&](FifamReader &reader) {
            Int clubID = -1;
            club::rival_club r;
            reader.ReadLine(clubID, IntPtr(r.mRivalClub), r.mLevel, r.mReason, r.mClubDontTransferPlayers);
            map_find(mClubs, clubID).mVecRivalClubs.push_back(r);
        });
        ReaderCallback(L"fm_club_regional_divisions", [&](FifamReader &reader) {
            Int clubID = -1;
            club::regional_division r;
            reader.ReadLine(clubID, r.mLevel, IntPtr(r.mDivision), r.mYear);
            map_find(mClubs, clubID).mVecRegionalDivisions.push_back(r);
        });
        ReaderCallback(L"fm_club_buy_back_clauses", [&](FifamReader &reader) {
            Int clubID = -1;
            club::buy_back_clause b;
            reader.ReadLine(clubID, IntPtr(b.mPlayer), IntPtr(b.mFromClub), b.mBuyBackFee);
            map_find(mClubs, clubID).mVecBuyBackClauses.push_back(b);
        });
        // TODO
        // read players
        String playersFile;
        if (size == db_size::Tiny)
            playersFile = L"fm_players_tiny";
        else if (size == db_size::Small)
            playersFile = L"fm_players_small";
        else if (size == db_size::Average)
            playersFile = L"fm_players_average";
        else
            playersFile = L"fm_players";
        UInt playerCounter = 0;
        ReaderCallback(playersFile, [&](FifamReader &reader) {
            player p;
            if (size == db_size::Full)
                reader.ReadLine(p.mID, p.mFirstName, p.mSecondName, p.mCommonName, p.mFullName, p.mEthnicity, p.mHairColour, p.mHairLength, p.mSkinTone, p.mDateOfBirth, IntPtr(p.mNation), IntPtr(p.mLanguage), p.mAgent, p.mDeclaredForNation, p.mInternationalRetirement, p.mInternationalRetirementDate, p.mInternationalApps, p.mInternationalGoals, p.mAdaptability, p.mAmbition, p.mControversy, p.mLoyalty, p.mPressure, p.mProfessionalism, p.mSportsmanship, p.mTemperament, p.mShirtName, p.mCurrentAbility, p.mRecommendedCurrentAbility, p.mPotentialAbility, p.mPotentialAbilityRange, p.mCurrentReputation, p.mHomeReputation, p.mWorldReputation, p.mHeight, p.mWeight, p.mLeftFoot, p.mRightFoot, p.mPosition, p.mGoalkeeper, p.mDefenderLeft, p.mDefenderCentral, p.mDefenderRight, p.mDefensiveMidfielder, p.mWingBackLeft, p.mWingBackRight, p.mMidfielderLeft, p.mMidfielderCentral, p.mMidfielderRight, p.mAttackingMidfielderLeft, p.mAttackingMidfielderCentral, p.mAttackingMidfielderRight, p.mStriker, p.mAggression, p.mAnticipation, p.mBravery, p.mComposure, p.mConcentration, p.mConsistency, p.mVision, p.mDecisions, p.mDetermination, p.mDirtiness, p.mFlair, p.mImportantMatches, p.mLeadership, p.mMovement, p.mPositioning, p.mTeamWork, p.mWorkRate, p.mAcceleration, p.mAgility, p.mBalance, p.mInjuryProneness, p.mJumpingReach, p.mNaturalFitness, p.mPace, p.mStamina, p.mStrength, p.mCorners, p.mCrossing, p.mDribbling, p.mFinishing, p.mFirstTouch, p.mFreeKicks, p.mHeading, p.mLongShots, p.mLongThrows, p.mMarking, p.mPassing, p.mPenaltyTaking, p.mTackling, p.mTechnique, p.mVersatility, p.mAerialAbility, p.mCommandOfArea, p.mCommunication, p.mEccentricity, p.mHandling, p.mKicking, p.mOneOnOnes, p.mReflexes, p.mRushingOut, p.mTendencyToPunch, p.mThrowing, p.mRunsWithBallDownLeft, p.mRunsWithBallDownRight, p.mRunsWithBallThroughTheCentre, p.mGetsIntoOppositionArea, p.mMovesIntoChannels, p.mGetsForwardWheneverPossible, p.mPlaysShortSimplePasses, p.mTriesKillerBallsOften, p.mShootsFromDistance, p.mShootsWithPower, p.mPlacesShots, p.mCurlsBall, p.mLikesToRoundKeeper, p.mLikesToTryToBreakOffsideTrap, p.mArguesWithOfficials, p.mLikesToLobKeeper, p.mPlaysNoThroughBalls, p.mDwellsOnBall, p.mArrivesLateInOppositionArea, p.mTriesToPlayWayOutOfTrouble, p.mStaysBackAtAllTimes, p.mDivesIntoTackles, p.mDoesNotDiveIntoTackles, p.mHitsFreekicksWithPower, p.mRunsWithBallOften, p.mRunsWithBallRarely, p.mAvoidsUsingWeakerFoot, p.mTriesLongRangeFreeKicks, p.mCutsInsideFromBothWings, p.mPlaysOneTwos, p.mDictatesTempo, p.mAttemptsOverheadKicks, p.mKnocksBallPastOpponent, p.mTriesLongRangePasses, p.mLikesToSwitchBallToOtherFlank, p.mComesDeepToGetBall, p.mHugsLine, p.mLooksForPassRatherThanAttemptingToScore, p.mMarksOpponentTightly, p.mPlaysWithBackToGoal, p.mPossessesLongFlatThrow, p.mStopsPlay, p.mTriesFirstTimeShots, p.mUsesLongThrowToStartCounterAttacks, p.mRefrainsFromTakingLongShots, p.mPenaltyBoxPlayer, p.mCutsInsideFromLeftWing, p.mCutsInsideFromRightWing, p.mCrossesEarly, p.mBringBallOutofDefence, p.mIsBasque);
            else
                reader.ReadLineWithSeparator(L'\t', p.mID, p.mFirstName, p.mSecondName, p.mCommonName, p.mFullName, p.mEthnicity, p.mHairColour, p.mHairLength, p.mSkinTone, p.mDateOfBirth, IntPtr(p.mNation), IntPtr(p.mLanguage), p.mAgent, p.mDeclaredForNation, p.mInternationalRetirement, p.mInternationalRetirementDate, p.mInternationalApps, p.mInternationalGoals, p.mAdaptability, p.mAmbition, p.mControversy, p.mLoyalty, p.mPressure, p.mProfessionalism, p.mSportsmanship, p.mTemperament, p.mShirtName, p.mCurrentAbility, p.mRecommendedCurrentAbility, p.mPotentialAbility, p.mPotentialAbilityRange, p.mCurrentReputation, p.mHomeReputation, p.mWorldReputation, p.mHeight, p.mWeight, p.mLeftFoot, p.mRightFoot, p.mPosition, p.mGoalkeeper, p.mDefenderLeft, p.mDefenderCentral, p.mDefenderRight, p.mDefensiveMidfielder, p.mWingBackLeft, p.mWingBackRight, p.mMidfielderLeft, p.mMidfielderCentral, p.mMidfielderRight, p.mAttackingMidfielderLeft, p.mAttackingMidfielderCentral, p.mAttackingMidfielderRight, p.mStriker, p.mAggression, p.mAnticipation, p.mBravery, p.mComposure, p.mConcentration, p.mConsistency, p.mVision, p.mDecisions, p.mDetermination, p.mDirtiness, p.mFlair, p.mImportantMatches, p.mLeadership, p.mMovement, p.mPositioning, p.mTeamWork, p.mWorkRate, p.mAcceleration, p.mAgility, p.mBalance, p.mInjuryProneness, p.mJumpingReach, p.mNaturalFitness, p.mPace, p.mStamina, p.mStrength, p.mCorners, p.mCrossing, p.mDribbling, p.mFinishing, p.mFirstTouch, p.mFreeKicks, p.mHeading, p.mLongShots, p.mLongThrows, p.mMarking, p.mPassing, p.mPenaltyTaking, p.mTackling, p.mTechnique, p.mVersatility, p.mAerialAbility, p.mCommandOfArea, p.mCommunication, p.mEccentricity, p.mHandling, p.mKicking, p.mOneOnOnes, p.mReflexes, p.mRushingOut, p.mTendencyToPunch, p.mThrowing, p.mRunsWithBallDownLeft, p.mRunsWithBallDownRight, p.mRunsWithBallThroughTheCentre, p.mGetsIntoOppositionArea, p.mMovesIntoChannels, p.mGetsForwardWheneverPossible, p.mPlaysShortSimplePasses, p.mTriesKillerBallsOften, p.mShootsFromDistance, p.mShootsWithPower, p.mPlacesShots, p.mCurlsBall, p.mLikesToRoundKeeper, p.mLikesToTryToBreakOffsideTrap, p.mArguesWithOfficials, p.mLikesToLobKeeper, p.mPlaysNoThroughBalls, p.mDwellsOnBall, p.mArrivesLateInOppositionArea, p.mTriesToPlayWayOutOfTrouble, p.mStaysBackAtAllTimes, p.mDivesIntoTackles, p.mDoesNotDiveIntoTackles, p.mHitsFreekicksWithPower, p.mRunsWithBallOften, p.mRunsWithBallRarely, p.mAvoidsUsingWeakerFoot, p.mTriesLongRangeFreeKicks, p.mCutsInsideFromBothWings, p.mPlaysOneTwos, p.mDictatesTempo, p.mAttemptsOverheadKicks, p.mKnocksBallPastOpponent, p.mTriesLongRangePasses, p.mLikesToSwitchBallToOtherFlank, p.mComesDeepToGetBall, p.mHugsLine, p.mLooksForPassRatherThanAttemptingToScore, p.mMarksOpponentTightly, p.mPlaysWithBackToGoal, p.mPossessesLongFlatThrow, p.mStopsPlay, p.mTriesFirstTimeShots, p.mUsesLongThrowToStartCounterAttacks, p.mRefrainsFromTakingLongShots, p.mPenaltyBoxPlayer, p.mCutsInsideFromLeftWing, p.mCutsInsideFromRightWing, p.mCrossesEarly, p.mBringBallOutofDefence, p.mIsBasque);
            mPlayers[p.mID] = p;
            playerCounter++;
            if ((playerCounter % 1000) == 0)
                std::wcout << playerCounter << L"...";
        });
        if (playerCounter >= 1000)
            std::wcout << std::endl;
        ReaderCallback(L"fm_player_club_contracts", [&](FifamReader &reader) {
            Int playerID = -1;
            player::contract c;
            reader.ReadLine(playerID, IntPtr(c.mClub), c.mJob, c.mSecondaryJob, c.mDateJoined, c.mContractExpires, c.mContractType, c.mWage, c.mOnRollingContract, c.mSquadNumber, c.mPreferredSquadNumber, c.mAppearanceFee, c.mGoalBonus, c.mCleanSheetBonus, c.mInternationalCapBonus, c.mYearlyWageRise, c.mPromotionWageRise, c.mRelegationWageDrop, c.mOneYearExtensionAfterLeagueGamesFinalSeason, c.mOptionalContractExtensionByClub, c.mMatchHighestEarnerClause, c.mWillLeaveAtEndOfContract, c.mMinimumFeeReleaseClause, c.mMinimumFeeReleaseClauseExpiryDate, c.mSellOnFeePercentage);
            if (playerID != -1 && c.mClub != (club *)-1)
                map_find(mPlayers, playerID).mContract = c;
        });
        for (UInt i = 0; i < 5; i++) {
            ReaderCallback(Utils::Format(L"fm_player_playing_history_%d_%d", i * 100'000, (i + 1) * 100'000), [&](FifamReader &reader) {
                Int playerID = -1;
                player::playing_history h;
                reader.ReadLine(playerID, h.mYear, h.mOrder, IntPtr(h.mClub), IntPtr(h.mDivision), h.mOnLoan, h.mYouthTeam, h.mApps, h.mGoals, h.mTransferFee);
                map_find(mPlayers, playerID).mVecPlayingHistory.push_back(h);
            });
        }
        ReaderCallback(L"fm_player_languages", [&](FifamReader &reader) {
            Int playerID = -1;
            player::language l;
            reader.ReadLine(playerID, IntPtr(l.mLanguage), l.mProficiency, l.mCannotSpeakLanguage);
            map_find(mPlayers, playerID).mVecLanguages.push_back(l);
        });
        ReaderCallback(L"fm_player_second_nations", [&](FifamReader &reader) {
            Int playerID = -1;
            nation *nation = nullptr;
            reader.ReadLine(playerID, IntPtr(nation));
            map_find(mPlayers, playerID).mVecSecondNations.push_back(nation);
        });
        ReaderCallback(L"fm_player_favourite_clubs", [&](FifamReader &reader) {
            Int playerID = -1;
            player::favourite_club f;
            reader.ReadLine(playerID, IntPtr(f.mClub), f.mReason, f.mLevel);
            map_find(mPlayers, playerID).mVecFavouriteClubs.push_back(f);
        });
        ReaderCallback(L"fm_player_disliked_clubs", [&](FifamReader &reader) {
            Int playerID = -1;
            player::disliked_club d;
            reader.ReadLine(playerID, IntPtr(d.mClub), d.mLevel);
            map_find(mPlayers, playerID).mVecDislikedClubs.push_back(d);
        });
        ReaderCallback(L"fm_player_favourite_people", [&](FifamReader &reader) {
            Int playerID = -1;
            player::favourite_people f;
            reader.ReadLine(playerID, IntPtr(f.mPerson), f.mLevel, f.mReason, f.mPermanent);
            map_find(mPlayers, playerID).mVecFavouritePeople.push_back(f);
        });
        ReaderCallback(L"fm_player_bans", [&](FifamReader &reader) {
            Int playerID = -1;
            player::ban b;
            reader.ReadLine(playerID, b.mBanType, b.mStartDate, b.mEndDate, b.mNumberMatches);
            map_find(mPlayers, playerID).mVecBans.push_back(b);
        });
        for (UInt i = 0; i < 5; i++) {
            ReaderCallback(Utils::Format(L"fm_player_retirements_%d_%d", i * 100'000, (i + 1) * 100'000), [&](FifamReader &reader) {
                Int playerID = -1;
                player::retirement r;
                reader.ReadLine(playerID, r.mType, r.mDate);
                map_find(mPlayers, playerID).mVecRetirements.push_back(r);
            });
        }
        ReaderCallback(L"fm_player_injuries", [&](FifamReader &reader) {
            Int playerID = -1;
            player::injury i;
            reader.ReadLine(playerID, IntPtr(i.mInjury), i.mStartDate, i.mEndDate, i.mFuture, i.mPermanent, i.mSeverity);
            map_find(mPlayers, playerID).mVecInjuries.push_back(i);
        });
        ReaderCallback(L"fm_player_loans", [&](FifamReader &reader) {
            Int playerID = -1;
            player::loan l;
            reader.ReadLine(playerID, IntPtr(l.mClub), l.mStartDate, l.mEndDate, l.mSquadNumber, l.mWage, l.mMonthlyFee, l.mFeeToBuy);
            map_find(mPlayers, playerID).mLoan = l;
        });
        ReaderCallback(L"fm_player_future_transfers", [&](FifamReader &reader) {
            Int playerID = -1;
            player::future_transfer f;
            reader.ReadLine(playerID, IntPtr(f.mClub), f.mTransferDate, f.mContractEndDate, f.mTransferFee, f.mNewJob);
            map_find(mPlayers, playerID).mFutureTransfer = f;
        });
        // read non players
        ReaderCallback(L"fm_non_players", [&](FifamReader &reader) {
            non_player n;
            reader.ReadLine(n.mID, n.mFirstName, n.mSecondName, n.mCommonName, n.mFullName, n.mFemale, n.mDateOfBirth, IntPtr(n.mNation), IntPtr(n.mLanguage), n.mAdaptability, n.mAmbition, n.mControversy, n.mLoyalty, n.mPressure, n.mProfessionalism, n.mSportsmanship, n.mTemperament, n.mJobManager, n.mJobAssistantManager, n.mJobCoach, n.mJobFitnessCoach, n.mJobGkCoach, n.mJobPhysio, n.mJobScout, n.mJobDataAnalyst, n.mJobSportsScientist, n.mJobDirectorOfFootball, n.mJobHeadOfYouth, n.mJobChairman, n.mCurrentAbility, n.mPotentialAbility, n.mCurrentReputation, n.mWorldReputation, n.mHomeReputation, n.mPreferredFormation, n.mSecondPreferredFormation, n.mCoachingStyle, n.mAttacking, n.mDetermination, n.mTacticalKnowledge, n.mBuyingPlayers, n.mHardnessOfTraining, n.mJudgingPlayerAbility, n.mJudgingPlayerPotential, n.mJudgingPlayerData, n.mJudgingTeamData, n.mPresentingData, n.mLevelOfDiscipline, n.mManManagement, n.mMindGames, n.mMotivating, n.mVersatility, n.mSquadRotation, n.mWorkingWithYoungsters, n.mCoachingAttacking, n.mCoachingDefending, n.mCoachingFitness, n.mCoachingGKDistribution, n.mCoachingGKHandling, n.mCoachingGKShotStopping, n.mCoachingMental, n.mCoachingTactical, n.mCoachingTechnical, n.mBusiness, n.mInterference, n.mPatience, n.mResources, n.mPhysiotherapy, n.mSportsScience, n.mSignsALotOfYouthPlayers, n.mSignsYoungPlayersForTheFirstTeam, n.mWillMakeEarlyTacticalChanges, n.mWillFitPlayersIntoPreferredTactic, n.mExpectAttackingFootball, n.mExpectYoungSigningsForTheFirstTeam, n.mWillLookToPlayOutOfDefence);
            mNonPlayers[n.mID] = n;
        });
        ReaderCallback(L"fm_nonplayer_languages", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::language l;
            reader.ReadLine(nonplayerID, IntPtr(l.mLanguage), l.mProficiency, l.mCannotSpeakLanguage);
            map_find(mNonPlayers, nonplayerID).mVecLanguages.push_back(l);
        });
        ReaderCallback(L"fm_nonplayer_second_nations", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            nation *nation = nullptr;
            reader.ReadLine(nonplayerID, IntPtr(nation));
            map_find(mNonPlayers, nonplayerID).mVecSecondNations.push_back(nation);
        });
        ReaderCallback(L"fm_nonplayer_favourite_clubs", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::favourite_club f;
            reader.ReadLine(nonplayerID, IntPtr(f.mClub), f.mReason, f.mLevel);
            map_find(mNonPlayers, nonplayerID).mVecFavouriteClubs.push_back(f);
        });
        ReaderCallback(L"fm_nonplayer_disliked_clubs", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::disliked_club d;
            reader.ReadLine(nonplayerID, IntPtr(d.mClub), d.mLevel);
            map_find(mNonPlayers, nonplayerID).mVecDislikedClubs.push_back(d);
        });
        ReaderCallback(L"fm_nonplayer_club_contracts", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::club_contract c;
            reader.ReadLine(nonplayerID, IntPtr(c.mClub), c.mJob, c.mSecondaryJob, c.mDateJoined, c.mContractExpires, c.mContractType, c.mOnRollingContract);
            if (nonplayerID != -1 && c.mClub != (club *)-1)
                map_find(mNonPlayers, nonplayerID).mClubContract = c;
        });
        ReaderCallback(L"fm_nonplayer_nation_contracts", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::nation_contract c;
            reader.ReadLine(nonplayerID, IntPtr(c.mNation), c.mJob, c.mDateJoined, c.mContractExpires);
            if (nonplayerID != -1 && c.mNation != (nation *)-1)
                map_find(mNonPlayers, nonplayerID).mNationContract = c;
        });

        // read officials
        ReaderCallback(L"fm_officials", [&](FifamReader &reader) {
            official o;
            reader.ReadLine(o.mID, o.mFirstName, o.mSecondName, o.mCommonName, o.mFullName, o.mFemale, IntPtr(o.mNation), o.mCurrentAbility, o.mPotentialAbility, o.mReputation, o.mUEFACategory, o.mFIFACategory, o.mContinentalOfficial, o.mAllowingFlow, o.mDiscipline, o.mImportantMatches, o.mPressure, o.mRefereeing, o.mRunningLine, o.mTimeKeeping);
            mOfficials[o.mID] = o;
        });
        // read regions
        ReaderCallback(L"fm_regions", [&](FifamReader &reader) {
            region r;
            reader.ReadLine(r.mID, r.mName);
            mRegions[r.mID] = r;
        });
        // read cities
        ReaderCallback(L"fm_cities", [&](FifamReader &reader) {
            city c;
            reader.ReadLine(c.mID, c.mName, IntPtr(c.mNation), c.mInhabitants, c.mLatitude, c.mLongitude);
            mCities[c.mID] = c;
        });
        // read stadiums
        ReaderCallback(L"fm_stadiums", [&](FifamReader &reader) {
            stadium s;
            reader.ReadLine(s.mID, s.mName, IntPtr(s.mNation), IntPtr(s.mCity), IntPtr(s.mOwner), s.mCapacity, s.mSeatingCapacity, s.mLatitude, s.mLongitude, s.mUsedByNationalTeam);
            mStadiums[s.mID] = s;
        });
        // read derbies
        ReaderCallback(L"fm_derbies", [&](FifamReader &reader) {
            derby d;
            reader.ReadLine(d.mID, d.mName, d.mShortName, d.mAlternativeName, d.mAlternativeShortName, IntPtr(d.mTeam1), IntPtr(d.mTeam2), d.mWorldReputation, d.mNationalReputation);
            mDerbies[d.mID] = d;
        });
        // read languages
        ReaderCallback(L"fm_languages", [&](FifamReader &reader) {
            language l;
            reader.ReadLine(l.mID, l.mName, IntPtr(l.mNation));
            mLanguages[l.mID] = l;
        });
        // read injuries
        ReaderCallback(L"fm_injuries", [&](FifamReader &reader) {
            injury i;
            reader.ReadLine(i.mID, i.mName, i.mSentenceName, i.mCategory, i.mMinimumDays, i.mExtraDays, i.mInactiveRatio, i.mOccuranceRatio, i.mMatchInjuryPercentage, i.mGoalkeeperPercentage, i.mCentralDefenderPercentage, i.mRecurring, i.mContagious, i.mCause, i.mSeriousness, i.mAgeRule, i.mSeason, i.mTreatmentByPhysion, i.mTreatmentBySpecialist, i.mTreatmentByInjection, i.mTreatmentByIsolation, i.mAllowToPlayWithProtectiveEquipment);
            mInjuries[i.mID] = i;
        });
        // read stage names
        ReaderCallback(L"fm_stagenames", [&](FifamReader &reader) {
            stage_name s;
            reader.ReadLine(s.mID, s.mName, s.mShortName, s.mPluralName);
            mStageNames[s.mID] = s;
        });

        // resolving
        std::wcout << L"Resolving foom database links..." << std::endl;
        for (auto &entry : mNations) {
            nation &n = entry.second;
            resolve(n.mCapitalCity);
            resolve(n.mNationalStadium);
            resolve(n.mCaptain);
            resolve(n.mViceCaptain);
            for (auto &l : n.mVecLanguages)
                resolve(l.mLanguage);
            for (auto &r : n.mVecRegionTransferPreferences)
                resolve(r.mRegion);
            for (auto &k : n.mVecKits)
                resolve(k.mCompetition);
        }
        for (auto &entry : mComps) {
            comp &c = entry.second;
            resolve(c.mNation);
            resolve(c.mParentCompetition);
            for (auto &t : c.mVecTeams)
                resolve(t);
            for (auto &h : c.mVecHistory) {
                resolve(h.mFirstPlaced);
                resolve(h.mSecondPlaced);
                resolve(h.mThirdPlaced);
                resolve(h.mPlayOffWinner);
                resolve(h.mStageName);
                resolve(h.mHostStadium);
            }
        }
        for (auto &entry : mClubs) {
            club &c = entry.second;
            resolve(c.mNation);
            resolve(c.mBasedNation);
            resolve(c.mContinentalCupNation);
            resolve(c.mCity);
            resolve(c.mStadium);
            resolve(c.mDivision);
            resolve(c.mLastDivision);
            resolve(c.mNextDivision);
            resolve(c.mSecondaryDivision);
            resolve(c.mRecordWinOpposition);
            resolve(c.mRecordDefeatOpposition);
            resolve(c.mRecordAttendanceOpposition);
            resolve(c.mCaptain);
            resolve(c.mViceCaptain);

            for (auto &k : c.mVecKits)
                resolve(k.mCompetition);
            for (auto &h : c.mVecTeamLeagueHistory)
                resolve(h.mDivision);
            for (auto &r : c.mVecReserveTeams)
                resolve(r.mReserveClub);
            for (auto &a : c.mVecAffiliations)
                resolve(a.mAffiliatedClub);
            for (auto &r : c.mVecRivalClubs)
                resolve(r.mRivalClub);
            for (auto &r : c.mVecRegionalDivisions)
                resolve(r.mDivision);
            for (auto &b : c.mVecBuyBackClauses) {
                resolve(b.mPlayer);
                resolve(b.mFromClub);
            }
        }
        for (auto &entry : mPlayers) {
            player &p = entry.second;
            resolve(p.mNation);
            resolve(p.mLanguage);
            resolve(p.mContract.mClub);
            resolve(p.mLoan.mClub);
            resolve(p.mFutureTransfer.mClub);
            for (auto &l : p.mVecLanguages)
                resolve(l.mLanguage);
            for (auto &n : p.mVecSecondNations)
                resolve(n);
            for (auto &f : p.mVecFavouriteClubs)
                resolve(f.mClub);
            for (auto &d : p.mVecDislikedClubs)
                resolve(d.mClub);
            for (auto &h : p.mVecPlayingHistory) {
                resolve(h.mClub);
                resolve(h.mDivision);
            }
            for (auto &f : p.mVecFavouritePeople)
                resolve(f.mPerson);
            for (auto &i : p.mVecInjuries)
                resolve(i.mInjury);
            // TODO
        }
        for (auto &entry : mNonPlayers) {
            non_player &n = entry.second;
            resolve(n.mNation);
            resolve(n.mLanguage);
            resolve(n.mClubContract.mClub);
            resolve(n.mNationContract.mNation);
            if (n.mClubContract.mClub)
                n.mClubContract.mClub->mVecContractedNonPlayers.push_back(&n);
            if (n.mNationContract.mNation)
                n.mNationContract.mNation->mVecContractedNonPlayers.push_back(&n);
            for (auto &l : n.mVecLanguages)
                resolve(l.mLanguage);
            for (auto &n : n.mVecSecondNations)
                resolve(n);
            for (auto &f : n.mVecFavouriteClubs)
                resolve(f.mClub);
            for (auto &d : n.mVecDislikedClubs)
                resolve(d.mClub);
        }
        for (auto &entry : mOfficials) {
            official &o = entry.second;
            resolve(o.mNation);
        }
        for (auto &entry : mCities) {
            city &c = entry.second;
            resolve(c.mNation);
        }
        for (auto &entry : mStadiums) {
            stadium &s = entry.second;
            resolve(s.mNation);
            resolve(s.mCity);
            resolve(s.mOwner);
        }
        for (auto &entry : mDerbies) {
            derby &d = entry.second;
            resolve(d.mTeam1);
            resolve(d.mTeam2);
        }
        for (auto &entry : mLanguages) {
            language &l = entry.second;
            resolve(l.mNation);
        }
    }
};

}
