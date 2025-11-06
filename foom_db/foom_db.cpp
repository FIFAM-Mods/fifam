#include "foom_db.h"
#include <iostream>
#include "ProgressBar.h"

Int foom::db::convert_money(Int value) {
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

void foom::db::ReaderCallback(String const &filename, Function<void(FifamReader &)> callback) {
    std::wcout << L"Reading " << filename << L"..." << std::endl;
    FifamReader reader(mDbPath / (filename + L".csv"), 0);
    if (!reader.Available())
        reader.Open(mDbPath / (filename + L".txt"), 0);
    if (reader.Available()) {
        reader.SkipLine();
        ProgressBar pb((reader.GetSize() > 0) ? (reader.GetSize() - 1) : 0);
        while (!reader.IsEof()) {
            if (!reader.EmptyLine())
                callback(reader);
            else
                reader.SkipLine();
            pb.Step();
        }
    }
    else
        std::wcout << L"Can't read " << filename << std::endl;
}

#define MAP_SET_ENTITY_ATTR(entitymap, entityid, entityattr, value) auto ent = map_find_ptr(entitymap, entityid); if (ent) ent->entityattr = value
#define MAP_ADD_ENTITY_ATTR(entitymap, entityid, entityattr, value) auto ent = map_find_ptr(entitymap, entityid); if (ent) ent->entityattr.push_back(value)

foom::db::db(Path const &dbpath, db_gender gender, bool readPersons, db_size playersCount) {
    mDbPath = dbpath;
    mWomen = gender == db_gender::Women;
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
        MAP_ADD_ENTITY_ATTR(mNations, nationID, mVecKits, k);
    });
    ReaderCallback(L"fm_nation_languages", [&](FifamReader &reader) {
        Int nationID = -1;
        nation::nation_language l;
        reader.ReadLine(nationID, IntPtr(l.mLanguage), l.mPercentage);
        MAP_ADD_ENTITY_ATTR(mNations, nationID, mVecLanguages, l);
    });
    ReaderCallback(L"fm_nation_region_transfer_preferences", [&](FifamReader &reader) {
        Int nationID = -1;
        nation::region_transfer_preference r;
        reader.ReadLine(nationID, IntPtr(r.mRegion), r.mRating, r.mMinimumAge, r.mMaximumAge);
        MAP_ADD_ENTITY_ATTR(mNations, nationID, mVecRegionTransferPreferences, r);
    });
    // read competitions
    ReaderCallback(L"fm_comps", [&](FifamReader &reader) {
        comp c;
        reader.ReadLine(c.mID, c.mGender, c.mName, c.mShortName, IntPtr(c.mNation), c.mContinent, IntPtr(c.mParentCompetition), c.mReputation, c.mUsesSeatedStadiumsOnly, c.mIsExtinct, c.mType, c.mCompetitionLevel);
        mComps[c.mID] = c;
    });
    ReaderCallback(L"fm_comp_teams", [&](FifamReader &reader) {
        Int compID = -1;
        team *teamID = nullptr;
        reader.ReadLine(compID, IntPtr(teamID));
        MAP_ADD_ENTITY_ATTR(mComps, compID, mVecTeams, teamID);
    });
    ReaderCallback(L"fm_comp_history", [&](FifamReader &reader) {
        Int compID;
        comp::history h;
        reader.ReadLine(compID, h.mYear, IntPtr(h.mFirstPlaced), IntPtr(h.mSecondPlaced), IntPtr(h.mThirdPlaced), IntPtr(h.mPlayOffWinner), IntPtr(h.mStageName), h.mYearOrder, h.mNoDataForYear, IntPtr(h.mHostStadium), h.mNoTakePlace, h.mNotFinished, h.mStartYear, h.mEndYear);
        MAP_ADD_ENTITY_ATTR(mComps, compID, mVecHistory, h);
    });
    ReaderCallback(L"fm_comp_history_additional", [&](FifamReader &reader) {
        Int compID;
        comp::history h;
        reader.ReadLine(compID, h.mYear, IntPtr(h.mFirstPlaced), IntPtr(h.mSecondPlaced), IntPtr(h.mThirdPlaced), IntPtr(h.mPlayOffWinner), IntPtr(h.mStageName), h.mYearOrder, h.mNoDataForYear, IntPtr(h.mHostStadium));
        comp *c = get<comp>(compID);
        if (c) {
            bool isPresent = false;
            for (auto &e : c->mVecHistory) {
                if (e.mYear == h.mYear) {
                    isPresent = true;
                    break;
                }
            }
            if (!isPresent)
                c->mVecHistory.push_back(h);
        }
    });
    // read clubs
    ReaderCallback(L"fm_clubs", [&](FifamReader &reader) {
        club c;
        Bool isNation;
        reader.ReadLine(c.mID, isNation, c.mGender, c.mName, c.mShortName, c.mSixLetterName, c.mThreeLetterName, c.mAlternativeThreeLetterName, c.mNickname, c.mExtinct, IntPtr(c.mNation), IntPtr(c.mBasedNation), IntPtr(c.mContinentalCupNation), IntPtr(c.mCity), c.mYearFounded, c.mStatus, c.mReputation, c.mLatitude, c.mLongitude, c.mChairmanTitle, c.mOwnershipType, IntPtr(c.mStadium), c.mAttendance, c.mMinimumAttendance, c.mMaximumAttendance, c.mTraining, c.mYouthCoaching, c.mYouthFacilities, c.mYouthRecruitment, c.mYouthImportance, c.mCorporateFacilities, c.mBalance, c.mTransferBudget, c.mSugarDaddy, c.mNumberOfSeasonTicketHolders, IntPtr(c.mDivision), IntPtr(c.mLastDivision), c.mLastPosition, IntPtr(c.mNextDivision), c.mLastPositionForNextDivision, IntPtr(c.mSecondaryDivision), c.mEuroCoeff1, c.mEuroCoeff2, c.mEuroCoeff3, c.mEuroCoeff4, c.mEuroCoeff5, c.mEuroCoeff6, c.mEuroCoeff7, c.mEuroCoeff8, c.mEuroCoeff9, c.mEuroCoeff10, c.mPreferredFormation, c.mSecondPreferredFormation, IntPtr(c.mRecordWinOpposition), c.mRecordWinTG, c.mRecordWinOG, c.mRecordWinYear, IntPtr(c.mRecordDefeatOpposition), c.mRecordDefeatTG, c.mRecordDefeatOG, c.mRecordDefeatYear, IntPtr(c.mRecordAttendanceOpposition), c.mRecordAttendance, c.mRecordAttendanceYear, IntPtr(c.mCaptain), IntPtr(c.mViceCaptain), c.mBadge, c.mForegroundColor.r, c.mForegroundColor.g, c.mForegroundColor.b, c.mBackgroundColor.r, c.mBackgroundColor.g, c.mBackgroundColor.b, c.mSupporterExpectations, c.mInstituteClub, c.mScoutingSystem, c.mShirtForeground.r, c.mShirtForeground.g, c.mShirtForeground.b, c.mShirtBackground.r, c.mShirtBackground.g, c.mShirtBackground.b, c.mAwayShirtForeground.r, c.mAwayShirtForeground.g, c.mAwayShirtForeground.b, c.mAwayShirtBackground.r, c.mAwayShirtBackground.g, c.mAwayShirtBackground.b, IntPtr(c.mLocalRegion));
        if ((Bool)c.mGender == mWomen)
            mClubs[c.mID] = c;
    });
    ReaderCallback(L"fm_club_kits", [&](FifamReader &reader) {
        Int clubID = -1;
        kit k;
        reader.ReadLine(clubID, k.mKit, k.mType, k.mKitStyle, k.mBackOfShirtStyle, k.mForeground.r, k.mForeground.g, k.mForeground.b, k.mBackground.r, k.mBackground.g, k.mBackground.b, k.mOutline.r, k.mOutline.g, k.mOutline.b, k.mNumberColour.r, k.mNumberColour.g, k.mNumberColour.b, k.mNumberOutlineColour.r, k.mNumberOutlineColour.g, k.mNumberOutlineColour.b, IntPtr(k.mCompetition), k.mYear, k.mAlternativeKitNumber);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecKits, k);
    });
    ReaderCallback(L"fm_club_incomes", [&](FifamReader &reader) {
        Int clubID = -1;
        club::income i;
        reader.ReadLine(clubID, i.mAmount, i.mIncomeType, i.mStartDate, i.mEndDate, i.mRenewIncome, i.mFixedValue);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecIncomes, i);
    });
    ReaderCallback(L"fm_club_team_league_history", [&](FifamReader &reader) {
        Int clubID = -1;
        club::team_league_history h;
        reader.ReadLine(clubID, h.mYear, h.mOrder, IntPtr(h.mDivision), h.mPosition, h.mMaxTeams, h.mPoints, h.mGamesPlayed, h.mGamesWon, h.mGamesDrawn, h.mGamesLost, h.mGoalsFor, h.mGoalsAgainst);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecTeamLeagueHistory, h);
    });
    ReaderCallback(L"fm_club_reserve_teams", [&](FifamReader &reader) {
        Int clubID = -1;
        club::reserve_team r;
        reader.ReadLine(clubID, IntPtr(r.mReserveClub), r.mReserveTeamType, r.mDisabled, r.mNewTeam, r.mName, r.mShortName, r.m6LetterName, r.mReputation, IntPtr(r.mStadium), r.mAttendance, r.mMinAttendance, r.mMaxAttendance, IntPtr(r.mDivision), IntPtr(r.mSecondaryDivision), IntPtr(r.mLastDivision), r.mLastPosition, IntPtr(r.mNextDivision), IntPtr(r.mManager));
        auto c = get<club>(clubID);
        if (c) {
            c->mVecReserveTeams.push_back(r);
            if ((Int)r.mReserveClub != -1 && !r.mDisabled) {
                club *reserveClub = get<club>((Int)r.mReserveClub);
                if (!reserveClub) {
                    club rc;
                    rc.mID = (Int)r.mReserveClub;
                    rc.mIsReserveDummyClub = true;
                    rc.mName = r.mName;
                    rc.mShortName = r.mShortName;
                    rc.mSixLetterName = r.m6LetterName;
                    rc.mReputation = r.mReputation;
                    rc.mStadium = r.mStadium;
                    rc.mAttendance = r.mAttendance;
                    rc.mMinimumAttendance = r.mMinAttendance;
                    rc.mMaximumAttendance = r.mMaxAttendance;
                    rc.mDivision = r.mDivision;
                    rc.mSecondaryDivision = r.mSecondaryDivision;
                    rc.mLastDivision = r.mLastDivision;
                    rc.mLastPosition = r.mLastPosition;
                    rc.mNextDivision = r.mNextDivision;
                    rc.mReserveManager = r.mManager;
                    mClubs[rc.mID] = rc;
                }
            }
        }
    });
    ReaderCallback(L"fm_club_reserve_teams_to_create", [&](FifamReader &reader) {
        Int clubID = -1;
        club::reserve_team_to_create r;
        reader.ReadLine(clubID, r.mReserveTeamType, IntPtr(r.mDivision), IntPtr(r.mStadium), r.mAttendance, r.mMinAttendance, r.mMaxAttendance);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecReserveTeamsToCreate, r);
    });
    ReaderCallback(L"fm_club_affiliations", [&](FifamReader &reader) {
        Int clubID = -1;
        club::affiliation a;
        reader.ReadLine(clubID, IntPtr(a.mAffiliatedClub), a.mIsMainClub, a.mReason, a.mStartDate, a.mAffiliationType, a.mPlayersMoveFreely, a.mSameBoard, a.mPermanent, a.mCannotPlayInSameDivision);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecAffiliations, a);
    });
    ReaderCallback(L"fm_club_rival_clubs", [&](FifamReader &reader) {
        Int clubID = -1;
        club::rival_club r;
        reader.ReadLine(clubID, IntPtr(r.mRivalClub), r.mLevel, r.mReason, r.mClubDontTransferPlayers);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecRivalClubs, r);
    });
    ReaderCallback(L"fm_club_regional_divisions", [&](FifamReader &reader) {
        Int clubID = -1;
        club::regional_division r;
        reader.ReadLine(clubID, r.mLevel, IntPtr(r.mDivision), r.mYear);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecRegionalDivisions, r);
    });
    ReaderCallback(L"fm_club_buy_back_clauses", [&](FifamReader &reader) {
        Int clubID = -1;
        club::buy_back_clause b;
        reader.ReadLine(clubID, IntPtr(b.mPlayer), IntPtr(b.mFromClub), b.mBuyBackFee, b.mStartDate, b.mEndDate);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecBuyBackClauses, b);
    });
    ReaderCallback(L"fm_club_nation_transfer_preferences", [&](FifamReader &reader) {
        Int clubID = -1;
        club::nation_transfer_preference p;
        reader.ReadLine(clubID, IntPtr(p.mNation), p.mLevel, p.mMinAge, p.mMaxAge);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecNationTransferPreferences, p);
    });
    ReaderCallback(L"fm_club_nation_newgen_specifications", [&](FifamReader &reader) {
        Int clubID = -1;
        club::nation_newgen_specification s;
        reader.ReadLine(clubID, IntPtr(s.mFirstNation), IntPtr(s.mSecondNation), s.mPercentage, s.mIgnoreSecondNationalityForNames);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecNationNewgenSpecifications, s);
    });
    ReaderCallback(L"fm_club_new_clubs", [&](FifamReader &reader) {
        Int clubID = -1;
        club *n;
        reader.ReadLine(clubID, IntPtr(n));
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecNewClubs, n);
    });
    ReaderCallback(L"fm_club_vision", [&](FifamReader &reader) {
        Int clubID = -1;
        club::vision v;
        reader.ReadLine(clubID, v.mType, v.mImportance, v.mStartYear, v.mEndYear, v.mMaxAge, v.mMinLength, v.mIsOngoing, IntPtr(v.mCompetition), IntPtr(v.mNation), v.mWorldRepClassification);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecVision, v);
    });
    ReaderCallback(L"fm_club_retired_numbers", [&](FifamReader &reader) {
        Int clubID = -1;
        club::retired_number n;
        reader.ReadLine(clubID, IntPtr(n.mPlayer), n.mNumber, n.mReason);
        MAP_ADD_ENTITY_ATTR(mClubs, clubID, mVecRetiredNumbers, n);
    });
    if (readPersons) {
        // read players
        String playersFile;
        if (playersCount == db_size::Tiny)
            playersFile = L"fm_players_tiny";
        else if (playersCount == db_size::Small)
            playersFile = L"fm_players_small";
        else if (playersCount == db_size::Average)
            playersFile = L"fm_players_average";
        else
            playersFile = L"fm_players";
        ReaderCallback(playersFile, [&](FifamReader &reader) {
            player p;
            if (playersCount == db_size::Full)
                reader.ReadLine(p.mID, p.mFirstName, p.mSecondName, p.mCommonName, p.mFullName, p.mGender, p.mEthnicity, p.mHairColour, p.mHairLength, p.mSkinTone, p.mDateOfBirth, p.mEarliestDOB, IntPtr(p.mNation), IntPtr(p.mLanguage), p.mAgent, p.mDeclaredForNation, IntPtr(p.mDeclaredForNationYouth), p.mNationalityInfo, p.mInternationalRetirement, p.mInternationalRetirementDate, p.mInternationalApps, p.mInternationalGoals, p.mAdaptability, p.mAmbition, p.mControversy, p.mLoyalty, p.mPressure, p.mProfessionalism, p.mSportsmanship, p.mTemperament, p.mShirtName, p.mCurrentAbility, p.mRecommendedCurrentAbility, p.mPotentialAbility, p.mPerceivedPotentialAbility, p.mPeakCA, p.mPeakPA, p.mOriginalCA, p.mOriginalPA, p.mCurrentReputation, p.mHomeReputation, p.mWorldReputation, p.mHeight, p.mBodyType, p.mLeftFoot, p.mRightFoot, p.mPosition, p.mGoalkeeper, p.mDefenderLeft, p.mDefenderCentral, p.mDefenderRight, p.mDefensiveMidfielder, p.mWingBackLeft, p.mWingBackRight, p.mMidfielderLeft, p.mMidfielderCentral, p.mMidfielderRight, p.mAttackingMidfielderLeft, p.mAttackingMidfielderCentral, p.mAttackingMidfielderRight, p.mStriker, p.mAggression, p.mAnticipation, p.mBravery, p.mComposure, p.mConcentration, p.mConsistency, p.mVision, p.mDecisions, p.mDetermination, p.mDirtiness, p.mFlair, p.mImportantMatches, p.mLeadership, p.mMovement, p.mPositioning, p.mTeamWork, p.mWorkRate, p.mAcceleration, p.mAgility, p.mBalance, p.mInjuryProneness, p.mJumpingReach, p.mNaturalFitness, p.mPace, p.mStamina, p.mStrength, p.mCorners, p.mCrossing, p.mDribbling, p.mFinishing, p.mFirstTouch, p.mFreeKicks, p.mHeading, p.mLongShots, p.mLongThrows, p.mMarking, p.mPassing, p.mPenaltyTaking, p.mTackling, p.mTechnique, p.mVersatility, p.mAerialAbility, p.mCommandOfArea, p.mCommunication, p.mEccentricity, p.mHandling, p.mKicking, p.mOneOnOnes, p.mReflexes, p.mRushingOut, p.mTendencyToPunch, p.mThrowing, p.mRunsWithBallDownLeft, p.mRunsWithBallDownRight, p.mRunsWithBallThroughTheCentre, p.mGetsIntoOppositionArea, p.mMovesIntoChannels, p.mGetsForwardWheneverPossible, p.mPlaysShortSimplePasses, p.mTriesKillerBallsOften, p.mShootsFromDistance, p.mShootsWithPower, p.mPlacesShots, p.mCurlsBall, p.mLikesToRoundKeeper, p.mLikesToTryToBreakOffsideTrap, p.mArguesWithOfficials, p.mLikesToLobKeeper, p.mPlaysNoThroughBalls, p.mDwellsOnBall, p.mArrivesLateInOppositionArea, p.mTriesToPlayWayOutOfTrouble, p.mStaysBackAtAllTimes, p.mDivesIntoTackles, p.mDoesNotDiveIntoTackles, p.mHitsFreekicksWithPower, p.mRunsWithBallOften, p.mRunsWithBallRarely, p.mAvoidsUsingWeakerFoot, p.mTriesLongRangeFreeKicks, p.mCutsInsideFromBothWings, p.mPlaysOneTwos, p.mDictatesTempo, p.mAttemptsOverheadKicks, p.mKnocksBallPastOpponent, p.mTriesLongRangePasses, p.mLikesToSwitchBallToOtherFlank, p.mComesDeepToGetBall, p.mHugsLine, p.mLooksForPassRatherThanAttemptingToScore, p.mMarksOpponentTightly, p.mPlaysWithBackToGoal, p.mPossessesLongFlatThrow, p.mStopsPlay, p.mTriesFirstTimeShots, p.mUsesLongThrowToStartCounterAttacks, p.mRefrainsFromTakingLongShots, p.mPenaltyBoxPlayer, p.mCutsInsideFromLeftWing, p.mCutsInsideFromRightWing, p.mCrossesEarly, p.mBringBallOutofDefence, IntPtr(p.mNationOfBirth), IntPtr(p.mRegionOfBirth), IntPtr(p.mCityOfBirth), p.mWearsHijab, p.mSlowRetirement);
            else
                reader.ReadLineWithSeparator(L'\t', p.mID, p.mFirstName, p.mSecondName, p.mCommonName, p.mFullName, p.mGender, p.mEthnicity, p.mHairColour, p.mHairLength, p.mSkinTone, p.mDateOfBirth, p.mEarliestDOB, IntPtr(p.mNation), IntPtr(p.mLanguage), p.mAgent, p.mDeclaredForNation, IntPtr(p.mDeclaredForNationYouth), p.mNationalityInfo, p.mInternationalRetirement, p.mInternationalRetirementDate, p.mInternationalApps, p.mInternationalGoals, p.mAdaptability, p.mAmbition, p.mControversy, p.mLoyalty, p.mPressure, p.mProfessionalism, p.mSportsmanship, p.mTemperament, p.mShirtName, p.mCurrentAbility, p.mRecommendedCurrentAbility, p.mPotentialAbility, p.mPerceivedPotentialAbility, p.mPeakCA, p.mPeakPA, p.mOriginalCA, p.mOriginalPA, p.mCurrentReputation, p.mHomeReputation, p.mWorldReputation, p.mHeight, p.mBodyType, p.mLeftFoot, p.mRightFoot, p.mPosition, p.mGoalkeeper, p.mDefenderLeft, p.mDefenderCentral, p.mDefenderRight, p.mDefensiveMidfielder, p.mWingBackLeft, p.mWingBackRight, p.mMidfielderLeft, p.mMidfielderCentral, p.mMidfielderRight, p.mAttackingMidfielderLeft, p.mAttackingMidfielderCentral, p.mAttackingMidfielderRight, p.mStriker, p.mAggression, p.mAnticipation, p.mBravery, p.mComposure, p.mConcentration, p.mConsistency, p.mVision, p.mDecisions, p.mDetermination, p.mDirtiness, p.mFlair, p.mImportantMatches, p.mLeadership, p.mMovement, p.mPositioning, p.mTeamWork, p.mWorkRate, p.mAcceleration, p.mAgility, p.mBalance, p.mInjuryProneness, p.mJumpingReach, p.mNaturalFitness, p.mPace, p.mStamina, p.mStrength, p.mCorners, p.mCrossing, p.mDribbling, p.mFinishing, p.mFirstTouch, p.mFreeKicks, p.mHeading, p.mLongShots, p.mLongThrows, p.mMarking, p.mPassing, p.mPenaltyTaking, p.mTackling, p.mTechnique, p.mVersatility, p.mAerialAbility, p.mCommandOfArea, p.mCommunication, p.mEccentricity, p.mHandling, p.mKicking, p.mOneOnOnes, p.mReflexes, p.mRushingOut, p.mTendencyToPunch, p.mThrowing, p.mRunsWithBallDownLeft, p.mRunsWithBallDownRight, p.mRunsWithBallThroughTheCentre, p.mGetsIntoOppositionArea, p.mMovesIntoChannels, p.mGetsForwardWheneverPossible, p.mPlaysShortSimplePasses, p.mTriesKillerBallsOften, p.mShootsFromDistance, p.mShootsWithPower, p.mPlacesShots, p.mCurlsBall, p.mLikesToRoundKeeper, p.mLikesToTryToBreakOffsideTrap, p.mArguesWithOfficials, p.mLikesToLobKeeper, p.mPlaysNoThroughBalls, p.mDwellsOnBall, p.mArrivesLateInOppositionArea, p.mTriesToPlayWayOutOfTrouble, p.mStaysBackAtAllTimes, p.mDivesIntoTackles, p.mDoesNotDiveIntoTackles, p.mHitsFreekicksWithPower, p.mRunsWithBallOften, p.mRunsWithBallRarely, p.mAvoidsUsingWeakerFoot, p.mTriesLongRangeFreeKicks, p.mCutsInsideFromBothWings, p.mPlaysOneTwos, p.mDictatesTempo, p.mAttemptsOverheadKicks, p.mKnocksBallPastOpponent, p.mTriesLongRangePasses, p.mLikesToSwitchBallToOtherFlank, p.mComesDeepToGetBall, p.mHugsLine, p.mLooksForPassRatherThanAttemptingToScore, p.mMarksOpponentTightly, p.mPlaysWithBackToGoal, p.mPossessesLongFlatThrow, p.mStopsPlay, p.mTriesFirstTimeShots, p.mUsesLongThrowToStartCounterAttacks, p.mRefrainsFromTakingLongShots, p.mPenaltyBoxPlayer, p.mCutsInsideFromLeftWing, p.mCutsInsideFromRightWing, p.mCrossesEarly, p.mBringBallOutofDefence, IntPtr(p.mNationOfBirth), IntPtr(p.mRegionOfBirth), IntPtr(p.mCityOfBirth), p.mWearsHijab, p.mSlowRetirement);
            if ((Bool)p.mGender == mWomen && p.mOriginalCA > 0) // skip unknown players
                mPlayers[p.mID] = p;
        });
        ReaderCallback(L"fm_player_club_contracts", [&](FifamReader &reader) {
            Int playerID = -1;
            player::contract c;
            reader.ReadLine(playerID, IntPtr(c.mClub), c.mJob, c.mSecondaryJob, c.mDateJoined, c.mContractExpires, c.mContractType, c.mWage, c.mOnRollingContract, c.mSquadStatus, c.mSquadNumber, c.mPreferredSquadNumber, c.mAppearanceFee, c.mGoalBonus, c.mCleanSheetBonus, c.mInternationalCapBonus, c.mYearlyWageRise, c.mPromotionWageRise, c.mRelegationWageDrop, c.mOneYearExtensionAfterLeagueGamesFinalSeason, c.mOptionalContractExtensionByClub, c.mMatchHighestEarnerClause, c.mWillLeaveAtEndOfContract, c.mMinimumFeeReleaseClause, c.mMinimumFeeReleaseClauseExpiryDate, c.mSellOnFeePercentage);
            if (playerID != -1 && c.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)c.mClub)) {
                MAP_SET_ENTITY_ATTR(mPlayers, playerID, mContract, c);
            }
        });
        for (UInt i = 0; i < 10; i++) {
            ReaderCallback(Utils::Format(L"fm_player_playing_history_%d_%d", i * 100'000, (i + 1) * 100'000), [&](FifamReader &reader) {
                Int playerID = -1;
                player::playing_history h;
                reader.ReadLine(playerID, h.mYear, h.mOrder, IntPtr(h.mClub), IntPtr(h.mDivision), h.mDateJoined, h.mDateLeft, h.mOnLoan, h.mYouthTeam, h.mApps, h.mGoals, h.mTransferFee);
                if (!Utils::Contains(mClubs, (Int)h.mClub))
                    *(Int *)(&h.mClub) = -1;
                if (!Utils::Contains(mComps, (Int)h.mDivision))
                    *(Int *)(&h.mDivision) = -1;
                MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecPlayingHistory, h);
            });
        }
        ReaderCallback(L"fm_player_languages", [&](FifamReader &reader) {
            Int playerID = -1;
            player::language l;
            reader.ReadLine(playerID, IntPtr(l.mLanguage), l.mProficiency, l.mCannotSpeakLanguage);
            MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecLanguages, l);
        });
        ReaderCallback(L"fm_player_second_nations", [&](FifamReader &reader) {
            Int playerID = -1;
            player::second_nation s;
            reader.ReadLine(playerID, IntPtr(s.mNation), s.mInfo);
            MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecSecondNations, s);
        });
        ReaderCallback(L"fm_player_favourite_clubs", [&](FifamReader &reader) {
            Int playerID = -1;
            player::favourite_club f;
            reader.ReadLine(playerID, IntPtr(f.mClub), f.mReason, f.mLevel);
            if (playerID != -1 && f.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)f.mClub)) {
                MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecFavouriteClubs, f);
            }
        });
        ReaderCallback(L"fm_player_disliked_clubs", [&](FifamReader &reader) {
            Int playerID = -1;
            player::disliked_club d;
            reader.ReadLine(playerID, IntPtr(d.mClub), d.mLevel);
            if (playerID != -1 && d.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)d.mClub)) {
                MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecDislikedClubs, d);
            }
        });
        ReaderCallback(L"fm_player_favourite_people", [&](FifamReader &reader) {
            Int playerID = -1;
            player::favourite_people f;
            reader.ReadLine(playerID, IntPtr(f.mPerson), f.mLevel, f.mReason, f.mPermanent);
            if (playerID != -1 && f.mPerson != (person *)-1 && (Utils::Contains(mPlayers, (Int)f.mPerson) || Utils::Contains(mNonPlayers, (Int)f.mPerson))) {
                MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecFavouritePeople, f);
            }
        });
        ReaderCallback(L"fm_player_bans", [&](FifamReader &reader) {
            Int playerID = -1;
            player::ban b;
            reader.ReadLine(playerID, b.mBanType, b.mStartDate, b.mEndDate, b.mNumberMatches);
            MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecBans, b);
        });
        for (UInt i = 0; i < 10; i++) {
            ReaderCallback(Utils::Format(L"fm_player_retirements_%d_%d", i * 100'000, (i + 1) * 100'000), [&](FifamReader &reader) {
                Int playerID = -1;
                player::retirement r;
                reader.ReadLine(playerID, r.mType, r.mDate);
                MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecRetirements, r);
            });
        }
        ReaderCallback(L"fm_player_injuries", [&](FifamReader &reader) {
            Int playerID = -1;
            player::injury i;
            reader.ReadLine(playerID, IntPtr(i.mInjury), i.mStartDate, i.mEndDate, i.mFuture, i.mPermanent, i.mSeverity);
            MAP_ADD_ENTITY_ATTR(mPlayers, playerID, mVecInjuries, i);
        });
        ReaderCallback(L"fm_player_loans", [&](FifamReader &reader) {
            Int playerID = -1;
            player::loan l;
            reader.ReadLine(playerID, IntPtr(l.mClub), l.mStartDate, l.mEndDate, l.mSquadNumber, l.mWage, l.mMonthlyFee, l.mFeeToBuy);
            if (playerID != -1 && l.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)l.mClub)) {
                MAP_SET_ENTITY_ATTR(mPlayers, playerID, mLoan, l);
            }
        });
        ReaderCallback(L"fm_player_future_transfers", [&](FifamReader &reader) {
            Int playerID = -1;
            player::future_transfer f;
            reader.ReadLine(playerID, IntPtr(f.mClub), f.mTransferDate, f.mContractEndDate, f.mTransferFee, f.mNewJob);
            if (playerID != -1 && f.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)f.mClub)) {
                MAP_SET_ENTITY_ATTR(mPlayers, playerID, mFutureTransfer, f);
            }
        });
        // read non players
        ReaderCallback(L"fm_non_players", [&](FifamReader &reader) {
            non_player n;
            String dummy; // TODO: remove this
            reader.ReadLine(n.mID, n.mFirstName, n.mSecondName, n.mCommonName, n.mFullName, n.mGender, n.mDateOfBirth, IntPtr(n.mNation), IntPtr(n.mLanguage), n.mAdaptability, n.mAmbition, n.mControversy, n.mLoyalty, n.mPressure, n.mProfessionalism, n.mSportsmanship, n.mTemperament, n.mJobManager, n.mJobAssistantManager, n.mJobCoach, n.mJobFitnessCoach, n.mJobGkCoach, n.mJobPhysio, n.mJobScout, n.mJobDataAnalyst, n.mJobSportsScientist, n.mJobDirectorOfFootball, n.mJobHeadOfYouth, n.mJobChairman, n.mJobTechnicalDirector, n.mJobHeadOfPlayerDevelopment, n.mJobSetPieceCoach, n.mCurrentAbility, n.mPotentialAbility, n.mOriginalCA, n.mOriginalPA, n.mRecommendedCA, n.mRecommendedJobRole, n.mCurrentReputation, n.mWorldReputation, n.mHomeReputation, n.mPreferredFormation, n.mSecondPreferredFormation, n.mCoachingStyle, n.mAttacking, n.mDetermination, n.mTacticalKnowledge, n.mBuyingPlayers, n.mHardnessOfTraining, n.mJudgingPlayerAbility, n.mJudgingPlayerPotential, n.mAnalysingData, n.mLevelOfDiscipline, n.mManManagement, n.mMindGames, n.mMotivating, n.mVersatility, n.mSquadRotation, n.mWorkingWithYoungsters, n.mCoachingAttacking, n.mCoachingDefending, n.mCoachingFitness, n.mCoachingGoalkeeping, n.mCoachingMental, n.mCoachingTactical, n.mCoachingTechnical, n.mCoachingSetPieces, n.mBusiness, n.mNegotiations, n.mInterference, n.mPatience, n.mResources, n.mPhysiotherapy, n.mSportsScience, n.mSignsALotOfYouthPlayers, n.mSignsYoungPlayersForTheFirstTeam, n.mWillMakeEarlyTacticalChanges, n.mWillFitPlayersIntoPreferredTactic, n.mWillLookToPlayOutOfDefence, n.mSideOfGameWilling, n.mSideOfGameCurrent);
            mNonPlayers[n.mID] = n;
        });
        ReaderCallback(L"fm_nonplayer_languages", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::language l;
            reader.ReadLine(nonplayerID, IntPtr(l.mLanguage), l.mProficiency, l.mCannotSpeakLanguage);
            MAP_ADD_ENTITY_ATTR(mNonPlayers, nonplayerID, mVecLanguages, l);
        });
        ReaderCallback(L"fm_nonplayer_second_nations", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::second_nation s;
            reader.ReadLine(nonplayerID, IntPtr(s.mNation), s.mInfo);
            MAP_ADD_ENTITY_ATTR(mNonPlayers, nonplayerID, mVecSecondNations, s);
        });
        ReaderCallback(L"fm_nonplayer_favourite_clubs", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::favourite_club f;
            reader.ReadLine(nonplayerID, IntPtr(f.mClub), f.mReason, f.mLevel);
            if (nonplayerID != -1 && f.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)f.mClub)) {
                MAP_ADD_ENTITY_ATTR(mNonPlayers, nonplayerID, mVecFavouriteClubs, f);
            }
        });
        ReaderCallback(L"fm_nonplayer_disliked_clubs", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::disliked_club d;
            reader.ReadLine(nonplayerID, IntPtr(d.mClub), d.mLevel);
            if (nonplayerID != -1 && d.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)d.mClub)) {
                MAP_ADD_ENTITY_ATTR(mNonPlayers, nonplayerID, mVecDislikedClubs, d);
            }
        });
        ReaderCallback(L"fm_nonplayer_club_contracts", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::club_contract c;
            reader.ReadLine(nonplayerID, IntPtr(c.mClub), c.mJob, c.mSecondaryJob, c.mDateJoined, c.mContractExpires, c.mContractType, c.mOnRollingContract, c.mSquadStatus);
            if (nonplayerID != -1 && c.mClub != (club *)-1 && Utils::Contains(mClubs, (Int)c.mClub)) {
                MAP_SET_ENTITY_ATTR(mNonPlayers, nonplayerID, mClubContract, c);
            }
        });
        ReaderCallback(L"fm_nonplayer_nation_contracts", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::nation_contract c;
            reader.ReadLine(nonplayerID, IntPtr(c.mNation), c.mJob, c.mDateJoined, c.mContractExpires);
            if (nonplayerID != -1 && c.mNation != (nation *)-1) {
                MAP_SET_ENTITY_ATTR(mNonPlayers, nonplayerID, mNationContract, c);
            }
        });
        ReaderCallback(L"fm_non_player_days_at_club_nation", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            non_player::days_at_club_or_nation d;
            reader.ReadLine(nonplayerID, IntPtr(d.mClubOrNation), d.mDays);
            if ((Utils::Contains(mClubs, (Int)d.mClubOrNation) || Utils::Contains(mNations, (Int)d.mClubOrNation)) && nonplayerID != -1 && d.mClubOrNation != (nation *)-1) {
                MAP_ADD_ENTITY_ATTR(mNonPlayers, nonplayerID, mDaysAtClubOrNation, d);
            }
        });
        ReaderCallback(L"fm_non_player_side_of_game", [&](FifamReader &reader) {
            Int nonplayerID = -1;
            Int SideOfGameWilling = -1, SideOfGameCurrent = -1;
            reader.ReadLine(nonplayerID, SideOfGameWilling, SideOfGameCurrent);
            if (nonplayerID != -1 && SideOfGameWilling != -1) {
                MAP_SET_ENTITY_ATTR(mNonPlayers, nonplayerID, mSideOfGameWilling, SideOfGameWilling);
            }
            if (nonplayerID != -1 && SideOfGameCurrent != -1) {
                MAP_SET_ENTITY_ATTR(mNonPlayers, nonplayerID, mSideOfGameCurrent, SideOfGameCurrent);
            }
        });
    }

    // read officials
    ReaderCallback(L"fm_officials", [&](FifamReader &reader) {
        official o;
        reader.ReadLine(o.mID, o.mFirstName, o.mSecondName, o.mCommonName, o.mFullName, o.mGender, IntPtr(o.mNation), o.mCurrentAbility, o.mPotentialAbility, o.mReputation, o.mUEFACategory, o.mFIFACategory, o.mContinentalOfficial, o.mAllowingFlow, o.mDiscipline, o.mImportantMatches, o.mPressure, o.mRefereeing, o.mRunningLine, o.mTimeKeeping);
        mOfficials[o.mID] = o;
    });

    // read regions
    ReaderCallback(L"fm_regions", [&](FifamReader &reader) {
        region r;
        reader.ReadLine(r.mID, r.mName);
        mRegions[r.mID] = r;
    });
    // read local regions
    ReaderCallback(L"fm_local_regions", [&](FifamReader &reader) {
        local_region r;
        reader.ReadLine(r.mID, r.mName, IntPtr(r.mNation), r.mPopulation, r.mWeather);
        mLocalRegions[r.mID] = r;
    });
    ReaderCallback(L"fm_local_region_cities", [&](FifamReader &reader) {
        Int regionID = -1;
        city *cityID = nullptr;
        reader.ReadLine(regionID, IntPtr(cityID));
        if (regionID != -1 && cityID != (city *)-1 && Utils::Contains(mCities, (Int)cityID)) {
            MAP_ADD_ENTITY_ATTR(mLocalRegions, regionID, mVecCities, cityID);
        }
    });
    ReaderCallback(L"fm_local_region_languages", [&](FifamReader &reader) {
        Int regionID = -1;
        local_region::local_region_language l;
        reader.ReadLine(regionID, IntPtr(l.mLanguage), l.mPercentage);
        if (regionID != -1 && l.mLanguage != (language *)-1 && Utils::Contains(mLanguages, (Int)l.mLanguage)) {
            MAP_ADD_ENTITY_ATTR(mLocalRegions, regionID, mVecLanguages, l);
        }
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
        reader.ReadLine(s.mID, s.mName, IntPtr(s.mNation), IntPtr(s.mCity), IntPtr(s.mOwner), s.mCapacity, s.mSeatingCapacity, s.mLatitude, s.mLongitude, s.mUsedByNationalTeam, s.mEnvironment);
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
        reader.ReadLine(i.mID, i.mName, i.mSentenceName, i.mClassification, i.mSubClassification, i.mMinimumDays, i.mExtraDays, i.mMinimumDaysWomen, i.mExtraDaysWomen, i.mInactiveRatio, i.mOccuranceRatio, i.mMatchInjuryPercentage, i.mGoalkeeperPercentage, i.mCentralDefenderPercentage, i.mRecurring, i.mContagious, i.mCause, i.mSeriousness, i.mAgeRule, i.mSeason, i.mTreatmentByPhysion, i.mTreatmentBySpecialist, i.mTreatmentByInjection, i.mTreatmentByIsolation, i.mAllowToPlayWithProtectiveEquipment);
        mInjuries[i.mID] = i;
    });
    // read stage names
    ReaderCallback(L"fm_stagenames", [&](FifamReader &reader) {
        stage_name s;
        reader.ReadLine(s.mID, s.mName, s.mShortName, s.mPluralName);
        mStageNames[s.mID] = s;
    });
    // read names
    Array<String, 6> translations = { L"fr", L"de", L"it", L"es", L"pl", L"ru" };
    for (UInt tr = 0; tr < std::size(translations); tr++) {
        ReaderCallback(L"names_cities_" + translations[tr], [&](FifamReader &reader) {
            Int id = -1; String name;
            reader.ReadLine(id, name);
            if (Utils::Contains(mCities, id))
                mCities[id].mTranslatedNames[tr] = name;
        });
        //ReaderCallback(L"names_clubs_" + translations[tr], [&](FifamReader &reader) {
        //    Int id = -1; String name; String shortName; String threeLetterNameOriginal; String threeLetterName; String threeLetterNameAlternative; String nickname;
        //    reader.ReadLine(id, name, shortName, threeLetterName, threeLetterNameAlternative, nickname);
        //    if (mClubs.contains(id)) {
        //        mClubs[id].mTranslatedNames[tr] = name;
        //        mClubs[id].mTranslatedShortNames[tr] = shortName;
        //        mClubs[id].mTranslatedNames[tr] = name;
        //        mClubs[id].mTranslatedThreeLetterNames[tr] = threeLetterName;
        //        mClubs[id].mTranslatedAlternativeThreeLetterNames[tr] = threeLetterNameAlternative;
        //        mClubs[id].mTranslatedNicknames[tr] = nickname;
        //    }
        //});
        //ReaderCallback(L"names_comps_" + translations[tr], [&](FifamReader &reader) {
        //    Int id = -1; String name; String shortName;
        //    reader.ReadLine(id, name, shortName);
        //    if (mComps.contains(id)) {
        //        mComps[id].mTranslatedNames[tr] = name;
        //        mComps[id].mTranslatedShortNames[tr] = shortName;
        //    }
        //});
        ReaderCallback(L"names_derbies_" + translations[tr], [&](FifamReader &reader) {
            Int id = -1; String name; String shortName; String alternativeName; String alternativeShortName;
            reader.ReadLine(id, name, shortName, alternativeName, alternativeShortName);
            if (Utils::Contains(mDerbies, id)) {
                mDerbies[id].mTranslatedNames[tr] = name;
                mDerbies[id].mTranslatedShortNames[tr] = shortName;
                mDerbies[id].mTranslatedNames[tr] = name;
                mDerbies[id].mTranslatedAlternativeNames[tr] = alternativeName;
                mDerbies[id].mTranslatedAlternativeShortNames[tr] = alternativeShortName;
            }
        });
        ReaderCallback(L"names_nations_" + translations[tr], [&](FifamReader &reader) {
            Int id = -1; String name; String shortName; String nickname; String threeLetterName; String nationalityName;
            reader.ReadLine(id, name, shortName, nickname, threeLetterName, nationalityName);
            if (Utils::Contains(mNations, id)) {
                mNations[id].mTranslatedNames[tr] = name;
                mNations[id].mTranslatedShortNames[tr] = shortName;
                mNations[id].mTranslatedNames[tr] = name;
                mNations[id].mTranslatedNicknames[tr] = nickname;
                mNations[id].mTranslatedThreeLetterNames[tr] = threeLetterName;
                mNations[id].mTranslatedNationalityName[tr] = nationalityName;
            }
        });
        ReaderCallback(L"names_stadiums_" + translations[tr], [&](FifamReader &reader) {
            Int id = -1; String name;
            reader.ReadLine(id, name);
            if (Utils::Contains(mStadiums, id))
                mStadiums[id].mTranslatedNames[tr] = name;
        });
    }

    // resolving
    std::wcout << L"Resolving foom database links..." << std::endl;
    for (auto &entry : mNations) {
        nation &n = entry.second;
        resolve(n.mCapitalCity);
        resolve(n.mNationalStadium);
        resolve(n.mCaptain);
        resolve(n.mViceCaptain);
        resolve(n.mNationalTeamMen);
        resolve(n.mNationalTeamWomen);
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
        resolve(c.mLocalRegion);
        resolve(c.mReserveManager);

        for (auto &k : c.mVecKits)
            resolve(k.mCompetition);
        for (auto &h : c.mVecTeamLeagueHistory)
            resolve(h.mDivision);
        for (auto &r : c.mVecReserveTeams) {
            resolve(r.mReserveClub);
            resolve(r.mStadium);
            resolve(r.mDivision);
            resolve(r.mSecondaryDivision);
            resolve(r.mNextDivision);
            resolve(r.mManager);
        }
        for (auto &r : c.mVecReserveTeamsToCreate) {
            resolve(r.mDivision);
            resolve(r.mStadium);
        }
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
        for (auto &v : c.mVecVision) {
            resolve(v.mCompetition);
            resolve(v.mNation);
        }
        for (auto &n : c.mVecRetiredNumbers)
            resolve(n.mPlayer);
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
        for (auto &s : p.mVecSecondNations)
            resolve(s.mNation);
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
        for (auto &s : n.mVecSecondNations)
            resolve(s.mNation);
        for (auto &f : n.mVecFavouriteClubs)
            resolve(f.mClub);
        for (auto &d : n.mVecDislikedClubs)
            resolve(d.mClub);
        for (auto &d : n.mDaysAtClubOrNation) {
            auto ptr = reinterpret_cast<club *>(d.mClubOrNation);
            resolve(ptr);
            if (ptr)
                d.mClubOrNation = ptr;
            else {
                auto nat_ptr = reinterpret_cast<nation *>(d.mClubOrNation);
                resolve(nat_ptr);
                if (nat_ptr)
                    d.mClubOrNation = nat_ptr;
            }
        }
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
    for (auto &entry : mLocalRegions) {
        local_region &r = entry.second;
        resolve(r.mNation);
        for (auto &c : r.mVecCities)
            resolve(c);
        for (auto &l : r.mVecLanguages)
            resolve(l.mLanguage);
    }

    // swap rgb
    /*
    auto swaprgb = [](Color &clr) {
    std::swap(clr.r, clr.b);
    };

    for (auto &entry : mNations) {
    nation &n = entry.second;
    swaprgb(n.mForegroundColor);
    swaprgb(n.mBackgroundColor);
    for (auto &k : n.mVecKits) {
    swaprgb(k.mBackground);
    swaprgb(k.mForeground);
    swaprgb(k.mOutline);
    swaprgb(k.mNumberColour);
    swaprgb(k.mNumberOutlineColour);
    }
    }
    for (auto &entry : mClubs) {
    club &c = entry.second;
    swaprgb(c.mForegroundColor);
    swaprgb(c.mBackgroundColor);
    for (auto &k : c.mVecKits) {
    swaprgb(k.mBackground);
    swaprgb(k.mForeground);
    swaprgb(k.mOutline);
    swaprgb(k.mNumberColour);
    swaprgb(k.mNumberOutlineColour);
    }
    }
    */
}
