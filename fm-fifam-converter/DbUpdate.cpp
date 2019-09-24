#include "Converter.h"

FifamClub *Converter::GetPreviousClub(UInt id) {
    return mPreviousDb->GetClubFromUID(id);
}

FifamPlayer *Converter::GetPreviousPlayer(UInt id) {
    auto it = mPreviousPlayers.find(id);
    if (it != mPreviousPlayers.end())
        return (*it).second;
    return nullptr;
}

void Converter::UpdateDataFromPreviousDb() {
    for (FifamPlayer *p : mFifamDatabase->mPlayers) {
        FifamPlayer *oldP = GetPreviousPlayer(p->mEmpicsId);
        if (oldP) {
            // talent
            p->mTalent = oldP->mTalent;
            // experience
            p->mGeneralExperience = oldP->mGeneralExperience;
            p->mInternationalExperience = oldP->mInternationalExperience;
            p->mNationalExperience = oldP->mNationalExperience;
            // character
            p->mCharacter = oldP->mCharacter;
            // position bias
            p->mPositionBias = oldP->mPositionBias;
            // main position
            p->mMainPosition = oldP->mMainPosition;
            // abilities
            p->mAttributes = oldP->mAttributes;
            // appearance
            p->mAppearance = oldP->mAppearance;
            // NT stats
            p->mNationalTeamGoals = oldP->mNationalTeamGoals;
            p->mNationalTeamMatches = oldP->mNationalTeamMatches;
            p->mCurrentlyInNationalTeam = oldP->mCurrentlyInNationalTeam;
            p->mRetiredFromNationalTeam = oldP->mRetiredFromNationalTeam;
            // agent
            p->mPlayerAgent = oldP->mPlayerAgent;
            // other
            p->mPlayingStyle = oldP->mPlayingStyle;
            p->mShoeType = oldP->mShoeType;
        }
    }
    for (FifamClub *c : mFifamDatabase->mClubs) {
        FifamClub *oldC = GetPreviousClub(c->mUniqueID);
        if (oldC) {
            FifamTrSetAll(c->mName, FifamTr(oldC->mName));
            FifamTrSetAll(c->mShortName, FifamTr(oldC->mShortName));
            FifamTrSetAll(c->mShortName2, FifamTr(oldC->mShortName2));
            FifamTrSetAll(c->mAbbreviation, FifamTr(oldC->mAbbreviation));
            FifamTrSetAll(c->mCityName, FifamTr(oldC->mCityName));
            FifamTrSetAll(c->mPlayerInText, FifamTr(oldC->mPlayerInText));
            FifamTrSetAll(c->mFanName1, FifamTr(oldC->mFanName1));
            FifamTrSetAll(c->mFanName2, FifamTr(oldC->mFanName2));
            FifamTrSetAll(c->mFanName1Article, FifamTr(oldC->mFanName1Article));
            FifamTrSetAll(c->mFanName2Article, FifamTr(oldC->mFanName2Article));
            c->mMascotName = oldC->mMascotName;
            c->mFanMembers = oldC->mFanMembers;
            c->mMaxAttendance = oldC->mMaxAttendance;
            c->mPotentialFansCount = oldC->mPotentialFansCount;
            c->mAverageAttendanceLastSeason = oldC->mAverageAttendanceLastSeason;
            c->mCountOfSoldSeasonTickets = oldC->mCountOfSoldSeasonTickets;
            c->mTraditionalClub = oldC->mTraditionalClub;
            c->mInternationalPrestige = oldC->mInternationalPrestige;
            c->mNationalPrestige = oldC->mNationalPrestige;
            c->mMediaPressure = oldC->mMediaPressure;
            c->mIndividualTvMoney = oldC->mIndividualTvMoney;
            c->mInitialCapital = oldC->mInitialCapital;
            if (c->mInitialCapital > 0)
                c->mTransferBudget = c->mInitialCapital / 2;
            else
                c->mTransferBudget = 0;
            c->mJointStockCompany = oldC->mJointStockCompany;
            c->mRichGuyControlled = oldC->mRichGuyControlled;
            c->mAddress = oldC->mAddress;
            c->mTelephone = oldC->mTelephone;
            c->mWebsiteAndMail = oldC->mWebsiteAndMail;
            c->mFansites = oldC->mFansites;

            // stadium
            FifamTrSetAll(c->mStadiumName, FifamTr(oldC->mStadiumName));
            c->mStadiumSeatsCapacity = oldC->mStadiumSeatsCapacity;
            c->mStadiumStandingsCapacity = oldC->mStadiumStandingsCapacity;
            c->mStadiumVipCapacity = oldC->mStadiumVipCapacity;
            c->mStadiumVenue = oldC->mStadiumVenue;
            c->mStadiumType = oldC->mStadiumType;
            c->mClubFacilities = oldC->mClubFacilities;
            c->mYouthBoardingSchool = oldC->mYouthBoardingSchool;
            c->mYouthCentre = oldC->mYouthCentre;
            c->mAiStrategy = oldC->mAiStrategy;
            c->mSettlement = oldC->mSettlement;
            c->mLandscape = oldC->mLandscape;

            // SPONSOR - TODO (need to update from foom)
            //c->mSponsorAmount = oldC->mSponsorAmount;
            //c->mSponsorDuration = oldC->mSponsorDuration;
            //c->mSpecialSponsor = oldC->mSpecialSponsor;

            c->mGeoCoords = oldC->mGeoCoords;
            c->mYouthPlayersAreBasques = oldC->mYouthPlayersAreBasques;
            c->mYouthPlayersCountry = oldC->mYouthPlayersCountry;
            c->mTransfersCountry = oldC->mTransfersCountry;

            // kit
            c->mKit = oldC->mKit;
            // badge
            c->mClubColour = oldC->mClubColour;
            c->mClubColour2 = oldC->mClubColour2;
            c->mMerchandiseColour = oldC->mMerchandiseColour;
            c->mHeaderColour = oldC->mHeaderColour;
            c->mBackgroundColour = oldC->mBackgroundColour;
        }
    }
}
