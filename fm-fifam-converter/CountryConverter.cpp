#include "Converter.h"
#include "FifamNames.h"
#include "ConverterUtil.h"

const Bool CONVERT_ASSESSMENT = true;

Int StadNationalTeamUsagePriority(foom::stadium *stad) {
    switch (stad->mUsedByNationalTeam) {
    case 1:
        return 6;
    case 2:
        return 5;
    case 5:
        return 4;
    case 3:
        return 3;
    case 6:
        return 2;
    case 4:
        return 1;
    }
    return 0;
}

void Converter::ConvertNationInfo(FifamCountry *dst, foom::nation *nation, UInt gameId) {

    nation->mConverterData.mFifamCountry = dst;

    if (CONVERT_ASSESSMENT) {
        dst->mAssessmentData[0] = nation->mEuroCoeff6;
        dst->mAssessmentData[1] = nation->mEuroCoeff7;
        dst->mAssessmentData[2] = nation->mEuroCoeff8;
        dst->mAssessmentData[3] = nation->mEuroCoeff9;
        dst->mAssessmentData[4] = nation->mEuroCoeff10;
        dst->mAssessmentData[5] = nation->mEuroCoeff11;
    }
    else {
        dst->mAssessmentData[0] = Float(dst->mAssessmentInfo[0] * 100);
        dst->mAssessmentData[1] = Float(dst->mAssessmentInfo[1] * 100);
        dst->mAssessmentData[2] = Float(dst->mAssessmentInfo[2] * 100);
        dst->mAssessmentData[3] = Float(dst->mAssessmentInfo[3] * 100);
        dst->mAssessmentData[4] = Float(dst->mAssessmentInfo[4] * 100);
        dst->mAssessmentData[5] = Float(dst->mAssessmentInfo[5] * 100);
    }
    dst->mAssessmentInfo[0] = UShort(dst->mAssessmentData[0] / 100.0f);
    dst->mAssessmentInfo[1] = UShort(dst->mAssessmentData[1] / 100.0f);
    dst->mAssessmentInfo[2] = UShort(dst->mAssessmentData[2] / 100.0f);
    dst->mAssessmentInfo[3] = UShort(dst->mAssessmentData[3] / 100.0f);
    dst->mAssessmentInfo[4] = UShort(dst->mAssessmentData[4] / 100.0f);
    dst->mAssessmentInfo[5] = UShort(dst->mAssessmentData[5] / 100.0f);

    dst->mFifaRanking = (Float)nation->mRankingPoints;
    dst->mYearsForNaturalization = Utils::Clamp(nation->mYearsToGainNationality, 0, 30);
    // national team
    if (nation->mYearAssociationFormed != 0)
        dst->mNationalTeam.mYearOfFoundation = nation->mYearAssociationFormed;

    if (nation->mConverterData.mFIFATeamID != 0)
        dst->mNationalTeam.mFifaID = nation->mConverterData.mFIFATeamID;

    // national team stadium
    foom::stadium * nationalStadium = nation->mNationalStadium;
    if (!nationalStadium) {
        Vector<foom::stadium *> stads;
        for (auto &[stadId, stad] : mFoomDatabase->mStadiums) {
            if (stad.mNation == nation)
                stads.push_back(&stad);
        }
        if (!stads.empty()) {
            std::sort(stads.begin(), stads.end(), [](foom::stadium * a, foom::stadium * b) {
                if (StadNationalTeamUsagePriority(a) > StadNationalTeamUsagePriority(b)) return true;
                if (StadNationalTeamUsagePriority(a) < StadNationalTeamUsagePriority(b)) return false;
                if (a->mSeatingCapacity > b->mSeatingCapacity) return true;
                if (a->mSeatingCapacity < b->mSeatingCapacity) return false;
                return false;
                });
            nationalStadium = stads.front();
        }
    }

    if (nationalStadium) {
        dst->SetProperty<Int>(L"foom::stad_id", nationalStadium->mID);
        if (nationalStadium->mOwner)
            dst->SetProperty<foom::team *>(L"foom::stad_owner", nationalStadium->mOwner);
        SetNameAndTranslation(dst->mNationalTeam.mStadiumName, nationalStadium->mName, nationalStadium->mTranslatedNames, 29);
        if (nationalStadium->mCapacity > 0) {
            Int seatingCapacity = nationalStadium->mSeatingCapacity;
            if (seatingCapacity == 0 || seatingCapacity > nationalStadium->mCapacity)
                seatingCapacity = nationalStadium->mCapacity;
            dst->mNationalTeam.mStadiumSeatsCapacity = seatingCapacity;
            dst->mNationalTeam.mStadiumStandingsCapacity = nationalStadium->mCapacity - seatingCapacity;
        }
    }

    // capital city
    if (nation->mCapitalCity) {
        SetNameAndTranslation(dst->mNationalTeam.mCityName, nation->mCapitalCity->mName, nation->mCapitalCity->mTranslatedNames, 29);
        if (nation->mCapitalCity->mLatitude != 0 || nation->mCapitalCity->mLongitude != 0)
            dst->mNationalTeam.mGeoCoords.SetFromFloat(nation->mCapitalCity->mLatitude, nation->mCapitalCity->mLongitude);
    }

    // wins
    dst->mNumWorldCups = (UShort)nation->mConverterData.mWorldCupWins.size();
    dst->mNumWorldCupRunnersUp = (UShort)nation->mConverterData.mWorldCupFinals.size();
    dst->mNumContinentalChampions = (UShort)nation->mConverterData.mContinentalCupWins.size();
    dst->mNumContinentalRunnersUp = (UShort)nation->mConverterData.mContinentalCupFinals.size();

    ConvertClubStadium(&dst->mNationalTeam, gameId);
    ConvertKitsAndColors(&dst->mNationalTeam, -1, nation->mVecKits, -1, nation->mBackgroundColor, nation->mForegroundColor, gameId);
    dst->mNationalTeam.mKit.mSpecialKitId = 0;
    ApplyClubCustomNames(nation->mID, &dst->mNationalTeam);
}
