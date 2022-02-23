#include "Converter.h"
#include "FifamNames.h"
#include "FifamUtils.h"
#include "ConverterUtil.h"
#include <fstream>

void Converter::ConvertClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div) {
    // name
    SetNameAndTranslation(dst->mName, team->mName, team->mTranslatedNames, 29, team->mShortName, team->mTranslatedShortNames);
    dst->mName2 = dst->mName;
    // short name
    SetNameAndTranslation(dst->mShortName, team->mShortName, team->mTranslatedShortNames, 10);
    dst->mShortName2 = dst->mShortName;
    // abbr
    String threeLetterName;
    Array<String, 6> threeLetterTranslationNames;
    if (!team->mThreeLetterName.empty()) {
        threeLetterName = team->mThreeLetterName;
        threeLetterTranslationNames = team->mTranslatedThreeLetterNames;
    }
    else if (!team->mOriginalThreeLetterName.empty()) {
        threeLetterName = team->mOriginalThreeLetterName;
        threeLetterTranslationNames = team->mTranslatedOriginalThreeLetterNames;
    }
    else if (!team->mAlternativeThreeLetterName.empty()) {
        threeLetterName = team->mAlternativeThreeLetterName;
        threeLetterTranslationNames = team->mTranslatedAlternativeThreeLetterNames;
    }
    if (!threeLetterName.empty())
        SetNameAndTranslation(dst->mAbbreviation, threeLetterName, threeLetterTranslationNames, 4);
    else
        FifamTrSetAll(dst->mAbbreviation, FifamNames::GetClubAbbr(team->mShortName));
    // year of foundation & traditional club
    if (team->mYearFounded <= 0)
        dst->mYearOfFoundation = 2000;
    else {
        dst->mYearOfFoundation = team->mYearFounded;
        if ((CURRENT_YEAR - 90) > dst->mYearOfFoundation)
            dst->mTraditionalClub = true;
    }
    // city and settlement
    if (team->mCity)
        SetNameAndTranslation(dst->mCityName, team->mCity->mName, team->mCity->mTranslatedNames, 29);
    else
        dst->mCityName = country->mNationalTeam.mCityName;
    // latitude/longitude
    if (team->mLatitude != 0 && team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;

    if (dst->mGeoCoords.mLatitude.ToInt() == 0 && dst->mGeoCoords.mLongitude.ToInt() == 0 && team->mNation) {
        for (auto &city : team->mNation->mConverterData.mCities) {
            if (city->mLatitude != 0 && city->mLongitude != 0) {
                dst->mGeoCoords.SetFromFloat(city->mLatitude, city->mLongitude);
                break;
            }
        }
    }

    // media pressure
    dst->mMediaPressure = FifamClubMediaPressure::Normal;
    // capital
    dst->mInitialCapital = foom::db::convert_money(team->mBalance);
    // transfer budget
    if (team->mTransferBudget > 0)
        dst->mTransferBudget = foom::db::convert_money(team->mTransferBudget);
    // rich guy controlled
    dst->mRichGuyControlled = team->mSugarDaddy >= 1 && team->mSugarDaddy <= 3; // Foreground, Background, Underwriter
    // sponsor
    foom::club::income *bestIncome = nullptr;
    Int bestIncomeValue = 0;
    foom::club::income *bestIndividualTvMoneyIncome = nullptr;
    Int bestIndividualTvMoneyValue = 0;
    for (auto &i : team->mVecIncomes) {
        if (i.mIncomeType >= 1 && i.mIncomeType <= 5) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor, Individual TV Deal; 19 - sleeve sponsor
            if (i.mAmount > 0 && i.mStartDate.year <= CURRENT_YEAR && i.mEndDate.year > CURRENT_YEAR) { // is valid contract
                UShort duration = i.mEndDate.year - i.mStartDate.year;
                UShort yearsLeft = i.mEndDate.year - CURRENT_YEAR;
                auto amountPerYear = i.mAmount / duration;
                auto amountInGame = amountPerYear * yearsLeft;
                if (i.mIncomeType >= 1 && i.mIncomeType <= 4) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor
                    if (!bestIncome || amountInGame > bestIncomeValue) {
                        bestIncome = &i;
                        bestIncomeValue = amountInGame;
                    }
                }
                else if (i.mIncomeType == 5) { // Individual TV Deal
                    if (!bestIndividualTvMoneyIncome || amountInGame > bestIndividualTvMoneyValue) {
                        bestIndividualTvMoneyIncome = &i;
                        bestIndividualTvMoneyValue = amountInGame;
                    }
                }
            }
        }
    }
    if (bestIncome) {
        UShort duration = bestIncome->mEndDate.year - bestIncome->mStartDate.year;
        if (duration != 0) {
            dst->mSponsorAmount = foom::db::convert_money(bestIncome->mAmount / duration);
            dst->mSponsorDuration = bestIncome->mEndDate.year - CURRENT_YEAR;
            dst->mSpecialSponsor = bestIncome->mRenewIncome && !bestIncome->mFixedValue;
        }
    }
    if (bestIndividualTvMoneyIncome) {
        UShort duration = bestIndividualTvMoneyIncome->mEndDate.year - CURRENT_YEAR;
        if (duration != 0)
            dst->mIndividualTvMoney = foom::db::convert_money(bestIndividualTvMoneyIncome->mAmount / duration);
    }
    // Fans & attendance
    dst->mAverageAttendanceLastSeason = team->mAttendance;
    dst->mMaxAttendance = team->mMaximumAttendance;
    if (team->mNumberOfSeasonTicketHolders > 0)
        dst->mCountOfSoldSeasonTickets = team->mNumberOfSeasonTicketHolders;
    else
        dst->mCountOfSoldSeasonTickets = dst->mAverageAttendanceLastSeason / 2;
    Float fanBaseMp = 6.5f;
    static const Float fanBaseLeagueMp[] = { 2.0f, 2.5f, 3.5f, 5.0f };
    if (div && div->mLevel >= 0 && div->mLevel < std::size(fanBaseLeagueMp))
        fanBaseMp = fanBaseLeagueMp[div->mLevel];
    if (dst->mTraditionalClub)
        fanBaseMp += 0.4f;
    dst->mPotentialFansCount = UInt(Float(dst->mAverageAttendanceLastSeason) * fanBaseMp);
    if (team->mReputation > 7'000)
        dst->mPotentialFansCount += (team->mReputation - 7'000) * 50;
    if (dst->mPotentialFansCount > 250'000)
        dst->mPotentialFansCount = 250'000;

    // Stadium
    if (team->mStadium) {
        dst->SetProperty<Int>(L"foom::stad_id", team->mStadium->mID);
        SetNameAndTranslation(dst->mStadiumName, team->mStadium->mName, team->mStadium->mTranslatedNames, 29);
        if (team->mStadium->mCapacity > 0) {
            Int seatingCapacity = team->mStadium->mSeatingCapacity;
            if (seatingCapacity == 0 || seatingCapacity > team->mStadium->mCapacity)
                seatingCapacity = team->mStadium->mCapacity;
            dst->mStadiumSeatsCapacity = seatingCapacity;
            dst->mStadiumStandingsCapacity = team->mStadium->mCapacity - seatingCapacity;
        }
    }
    if (FifamTr(dst->mStadiumName).empty()) {
        for (UInt s = 0; s < dst->mStadiumName.size(); s++) {
            String stadSuffix = (s == FifamTranslation::German) ? L" Stadion" : L" Stadium";
            dst->mStadiumName[s] = FifamNames::LimitName(dst->mCityName[s], 29 - stadSuffix.length()) + stadSuffix;
        }
    }
    if ((dst->mStadiumSeatsCapacity + dst->mStadiumStandingsCapacity) == 0)
        dst->mStadiumSeatsCapacity = Utils::Clamp(dst->mAverageAttendanceLastSeason * 2, 50, 50'000);

    // Club facilities
    Int clubDefaultFacilitiesValue = team->mReputation / 500;
    if (clubDefaultFacilitiesValue < 1)
        clubDefaultFacilitiesValue = 1;
    Int clubDefaultFacilitiesForUnknown = clubDefaultFacilitiesValue;
    if (clubDefaultFacilitiesForUnknown > 1)
        clubDefaultFacilitiesForUnknown -= 1;

    auto FacilityMap = [](Int val, Vector<Int> convMap, Int base = 0) {
        if (!convMap.empty()) {
            for (Int i = convMap.size() - 1; i >= 0; i--) {
                if (val >= convMap[i])
                    return base + i;
            }
        }
        return base;
    };

    // club facilities is based on training facilities (66,7%) and corporate facilities (33,3%)
    dst->mClubFacilities = FacilityMap(team->mTraining + team->mCorporateFacilities / 2, { 0, 6, 11, 16, 21, 25, 28 });
    Int youthFacilities = team->mYouthFacilities;
    if (youthFacilities <= 0)
        youthFacilities = clubDefaultFacilitiesForUnknown;
    dst->mYouthCentre = FacilityMap(youthFacilities, { 0, 5, 8, 10, 12, 14, 16, 18, 19, 20 }, 1);
    Int youthCoaching = team->mYouthCoaching;
    if (youthCoaching <= 0)
        youthCoaching = clubDefaultFacilitiesForUnknown;
    dst->mYouthBoardingSchool = FacilityMap(youthCoaching, { 0, 2, 3, 5, 7, 9, 11, 13, 15, 17, 19 });
    Int trainingGrounds = team->mTraining;
    if (trainingGrounds <= 0)
        trainingGrounds = clubDefaultFacilitiesForUnknown;
    dst->mTrainingGrounds = Utils::MapTo(trainingGrounds, 1, 20, 0, 5);
    Int corporateFacilities = team->mCorporateFacilities;
    if (corporateFacilities <= 0)
        corporateFacilities = 1;
    dst->mMerchandising = Utils::MapTo(corporateFacilities, 1, 20, 0, 5);
    dst->mMedicalDepartment = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mMedia = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mResearch = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);

    // AI strategy
    Float representation = team->mCorporateFacilities >= 15 ? (team->mCorporateFacilities - 14) * 1.5f : 0;
    Float youth = (Float)((team->mTraining >= 6 && team->mYouthImportance >= 14) ? team->mYouthImportance - 13 : 0);
    Float transfers = (Float)(team->mYouthRecruitment >= 14 ? team->mYouthRecruitment - 13 : 0);
    Float incomeOrientation = (Float)((team->mBalance > 100'000 && team->mTransferBudget > 0) ? team->mBalance / team->mTransferBudget : 0);
    Float income = incomeOrientation >= 8 ? Utils::Min(7.0f, roundf(incomeOrientation - 7)) : 0;
    if (representation > 0 || youth > 0 || transfers > 0 || income > 0) {
        dst->mAiStrategy = Utils::GetMaxElementId<Float, FifamClubAiStrategy>({
            { representation, FifamClubAiStrategy::Representation },
            { youth, FifamClubAiStrategy::TrainingAndYouth },
            { transfers, FifamClubAiStrategy::ScoutingAndTransfers },
            { income, FifamClubAiStrategy::Income },
            }).second;
    }
    else
        dst->mAiStrategy = FifamClubAiStrategy::General;

    // settlement
    if (team->mCity && team->mCity->mInhabitants > 0) {
        if (team->mCity->mInhabitants >= 10) // 500'001+
            dst->mSettlement = FifamClubSettlement::City;
        else if (team->mCity->mInhabitants >= 6) // 25'001+
            dst->mSettlement = FifamClubSettlement::Town;
        else
            dst->mSettlement = FifamClubSettlement::Village;
    }
    else {
        if (dst->mStadiumSeatsCapacity <= 50000) {
            if (dst->mStadiumSeatsCapacity > 20000)
                dst->mSettlement = FifamClubSettlement::Town;
            else
                dst->mSettlement = FifamClubSettlement::Village;
        }
        else
            dst->mSettlement = FifamClubSettlement::City;
    }

    // Youth players come from
    if (team->mBasedNation)
        dst->mYouthPlayersCountry.SetFromInt(team->mBasedNation->mConverterData.mFIFAManagerReplacementID);
    else
        dst->mYouthPlayersCountry.SetFromInt(country->mId);
    // Youth players are basques
    dst->mYouthPlayersAreBasques = team->is_basque();

    std::sort(team->mVecVision.begin(), team->mVecVision.end(), [](foom::club::vision const &a, foom::club::vision const &b) {
        return a.mImportance > b.mImportance;
    });
    Vector<UInt> transferCountries;
    for (auto const &v : team->mVecVision) {
        if (v.mType == 101 && v.mNation && v.mImportance >= 0 && !Utils::Contains(transferCountries, v.mNation->mConverterData.mFIFAManagerReplacementID)) // transfer from nation
            transferCountries.push_back(v.mNation->mConverterData.mFIFAManagerReplacementID);
        if (transferCountries.size() == 2)
            break;
    }
    if (transferCountries.size() < 2) {
        if (country->mId == FifamNation::Russia ||
            country->mId == FifamNation::Ukraine ||
            country->mId == FifamNation::Belarus ||
            country->mId == FifamNation::Kazakhstan ||
            country->mId == FifamNation::Azerbaijan)
        {
            if (!Utils::Contains(transferCountries, country->mId))
                transferCountries.push_back(country->mId);
        }
    }
    if (!transferCountries.empty()) {
        dst->mTransfersCountry[0] = FifamNation::MakeFromInt(transferCountries[0]);
        if (transferCountries.size() > 1)
            dst->mTransfersCountry[1] = FifamNation::MakeFromInt(transferCountries[1]);
    }

    // League history
    for (auto &h : team->mVecTeamLeagueHistory) {
        if (h.mDivision && h.mDivision->mCompetitionLevel >= 1 && h.mDivision->mCompetitionLevel <= 3) {
            UInt level = h.mDivision->mCompetitionLevel - 1;
            dst->mLeagueTotalPoints[level] += h.mPoints;
            dst->mLeagueTotalWins[level] += h.mGamesWon;
            dst->mLeagueTotalDraws[level] += h.mGamesDrawn;
            dst->mLeagueTotalLoses[level] += h.mGamesLost;
            dst->mLeagueTotalGoals[level] += h.mGoalsFor;
            dst->mLeagueTotalGoalsAgainst[level] += h.mGoalsAgainst;
        }
    }
    for (UInt i = 0; i < 3; i++) {
        if (dst->mLeagueTotalPoints[i] < 0)
            dst->mLeagueTotalPoints[i] = 0;
    }
    // Records
    if (team->mRecordAttendance > 0 && team->mRecordAttendanceOpposition && team->mRecordAttendanceYear > 0) {
        dst->mHistory.mRecordAttendance.mAttendance = team->mRecordAttendance;
        dst->mHistory.mRecordAttendance.mOpponentName = FifamNames::LimitName(team->mRecordAttendanceOpposition->mName, 20);
        dst->mHistory.mRecordAttendance.mYear = team->mRecordAttendanceYear;
    }
    if (team->mRecordWinYear > 0 && team->mRecordWinOpposition) {
        dst->mHistory.mRecordHomeWin.mScore1 = team->mRecordWinTG;
        dst->mHistory.mRecordHomeWin.mScore2 = team->mRecordWinOG;
        dst->mHistory.mRecordHomeWin.mOpponentName = FifamNames::LimitName(team->mRecordWinOpposition->mName, 20);
        dst->mHistory.mRecordHomeWin.mYear = team->mRecordWinYear;
    }
    if (team->mRecordDefeatYear > 0 && team->mRecordDefeatOpposition) {
        dst->mHistory.mRecordAwayDefeat.mScore1 = team->mRecordDefeatTG;
        dst->mHistory.mRecordAwayDefeat.mScore2 = team->mRecordDefeatOG;
        dst->mHistory.mRecordAwayDefeat.mOpponentName = FifamNames::LimitName(team->mRecordDefeatOpposition->mName, 20);
        dst->mHistory.mRecordAwayDefeat.mYear = team->mRecordDefeatYear;
    }

    // calculate international prestige
    /* OLD FORMULA */
    static Pair<Int, UChar> internationalRep[20] = {
        { 9000, 20 },
        { 8750, 19 },
        { 8550, 18 },
        { 8300, 17 },
        { 8150, 16 },
        { 7900, 15 },
        { 7750, 14 },
        { 7500, 13 },
        { 7300, 12 },
        { 7000, 11 },
        { 6900, 10 },
        { 6600,  9 },
        { 6400,  8 },
        { 6200,  7 },
        { 6000,  6 },
        { 5800,  5 },
        { 5600,  4 },
        { 5000,  3 },
        { 4500,  2 },
        { 4000,  1 }
    };
    dst->mInternationalPrestige = 0;
    for (auto const &ip : internationalRep) {
        if (team->mReputation >= ip.first) {
            dst->mInternationalPrestige = ip.second;
            break;
        }
    }
    
    auto oldIp = dst->mInternationalPrestige;
    // NEW FORMULA
    Float IPLeagueStrength = 0.0f, IPAttendance = 0.0f, IPReputation = 0.0f, IPEuroCoeff = 0.0f, IPInfrastructure = 0.0f;
    if (country && country->mId >= 1 && country->mId <= std::size(mIPCountryStrength)) {
        static Float IPFactorLeagueLevel[] = { 1, 0.75f, 0.5f, 0.25f, 0.1f };
        IPLeagueStrength = IPFactorLeagueLevel[std::size(IPFactorLeagueLevel) - 1];
        if (div && div->mLevel < std::size(IPFactorLeagueLevel))
            IPLeagueStrength = IPFactorLeagueLevel[div->mLevel];
        IPLeagueStrength *= mIPCountryStrength[country->mId - 1];
        auto FindInTable = [](Vector<UInt> const &table, UInt value) -> UInt {
            for (Int v = table.size(); v > 0; v--) {
                if (value >= table[v - 1])
                    return v - 1;
            }
            return 0;
        };
        IPAttendance = Float(FindInTable(
            { 0,1000,1500,2500,5000,6000,7000,8000,10000,12500,15000,18000,22000,27500,35000,42000,48000,52000,58000,64000,70000,76000,82000,88000,94000,100000 },
            team->mAttendance
        )) * 1.5f;
        IPReputation = Float(FindInTable(
            { 0,4000,4500,5000,5500,5800,6000,6200,6400,6600,6800,7000,7250,7500,7750,7900,8150,8300,8550,8750,9000,9250,9500,9750,10000,10250 },
            team->mReputation
        )) * (country->mContinent == FifamContinent::Europe ? 5.0f : 4.0f);
        IPEuroCoeff = Float(FindInTable(
            { 0,10,20,35,50,75,105,140,180,230,280,340,405,475,550,630,715,805,900,1000,1105,1215,1330,1450,1575,1700 },
            UInt(
                team->mEuroCoeff10 * 10.0f + team->mEuroCoeff9 * 9.0f + team->mEuroCoeff8 * 8.0f + team->mEuroCoeff7 * 7.0f + team->mEuroCoeff6 * 6.0f
                + team->mEuroCoeff5 * 5.0f + team->mEuroCoeff4 * 4.0f + team->mEuroCoeff3 * 3.0f + team->mEuroCoeff2 * 2.0f + team->mEuroCoeff1 * 1.0f
            )
        )) * 1.5f;
        IPInfrastructure = Float(FindInTable(
            { 0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100 },
            team->mTraining + team->mYouthCoaching + team->mYouthFacilities + team->mCorporateFacilities
        )) * (country->mContinent == FifamContinent::Europe ? 1.0f : 1.5f);
        if (country->mContinent == FifamContinent::Europe)
            dst->mInternationalPrestige = UInt(roundf((IPLeagueStrength + IPAttendance + IPReputation + IPEuroCoeff + IPInfrastructure) / 10.0f));
        else
            dst->mInternationalPrestige = UInt(roundf((IPLeagueStrength + IPAttendance + IPReputation + IPInfrastructure) / 8.0f));
        dst->mInternationalPrestige = Utils::Clamp(dst->mInternationalPrestige, 0, 20);
    }
    /*
    static std::ofstream ipfile("ip.csv");
    static bool firstTime = true;
    if (firstTime) {
        ipfile << "Country,Level,Club,Reputation,STR,ATT,REP,EUR,INF,OldIP,NewIP,Diff\n";
        firstTime = false;
    }
    StringA ipcountryname;
    if (dst->mCountry)
        ipcountryname = Utils::WtoA(Utils::GetStringWithoutUnicodeChars(FifamTr(dst->mCountry->mName)));
    Int ipleaguelevel = 255;
    if (div)
        ipleaguelevel = div->mLevel + 1;
    ipfile << Utils::Format("\"%s\",%d,\"%s\",%d,%g,%g,%g,%g,%g,%d,%d,%d\n", ipcountryname, ipleaguelevel,
        Utils::WtoA(Utils::GetStringWithoutUnicodeChars(FifamTr(dst->mName))),
        team->mReputation,
        IPLeagueStrength, IPAttendance, IPReputation, IPEuroCoeff, IPInfrastructure,
        oldIp, dst->mInternationalPrestige, Int(dst->mInternationalPrestige) - Int(oldIp));
    */

    // joint-stock company
    dst->mJointStockCompany = dst->mInternationalPrestige > 2 && team->mOwnershipType == 2 /*&& team->mReputation >= 5000*/; // Public Limited Company

    dst->mPreferredFormations[0] = FifamFormation::None;
    dst->mPreferredFormations[1] = FifamFormation::None;

    // custom formations enabled
    //
    //if (team->mPreferredFormation != 0) {
    //    FifamFormation firstFormation = ConvertFormationId(team->mPreferredFormation);
    //    if (firstFormation != FifamFormation::None) {
    //        UChar firstFormationId = firstFormation.GetIdForGame(gameId);
    //        if (firstFormationId != 0) {
    //            dst->mPreferredFormations[0] = firstFormation;
    //            if (team->mSecondPreferredFormation != 0) {
    //                FifamFormation secondFormation = ConvertFormationId(team->mSecondPreferredFormation);
    //                if (secondFormation != FifamFormation::None) {
    //                    UChar secondFormationId = secondFormation.GetIdForGame(gameId);
    //                    if (secondFormationId != 0 && secondFormationId != firstFormationId)
    //                        dst->mPreferredFormations[1] = secondFormation;
    //                }
    //            }
    //        }
    //    }
    //}

    Int firstCustomFormation = ConvertFormationIdToCustom(team->mPreferredFormation);
    if (firstCustomFormation != 0) {
        dst->SetProperty<Int>(L"custom_formation_1", firstCustomFormation);
        Int secondCustomFormation = ConvertFormationIdToCustom(team->mSecondPreferredFormation);
        if (secondCustomFormation != 0 && secondCustomFormation != firstCustomFormation)
            dst->SetProperty<Int>(L"custom_formation_2", secondCustomFormation);
    }

    ConvertClubStadium(dst, gameId);
    Int foomIdForGraphics = team->mID;
    if (!team->mConverterData.mParentClub && team->mConverterData.mOriginalStoredParentClub)
        foomIdForGraphics = team->mConverterData.mOriginalStoredParentClub->mID;
    ConvertKitsAndColors(dst, foomIdForGraphics, mainTeam->mVecKits, team->mBadge, team->mBackgroundColor, team->mForegroundColor, gameId);
}

void Converter::ConvertReserveClub(UInt gameId, FifamClub * dst, foom::club * team, foom::club * mainTeam, FifamCountry * country, DivisionInfo * div) {
    // name
    SetNameAndTranslation(dst->mName, team->mName, team->mTranslatedNames, 29, team->mShortName, team->mTranslatedShortNames);
    SetNameAndTranslation(dst->mShortName, team->mShortName, team->mTranslatedShortNames, 10);
    if (team->mName == mainTeam->mName) {
        String secondTypeName, thirdTypeName;
        if (country && country->mId == FifamNation::Germany) {
            secondTypeName = L"II";
            thirdTypeName = L"III";
        }
        else {
            secondTypeName = L"2";
            thirdTypeName = L"3";
        }
        String teamTypeName = L"Res";
        switch (team->mConverterData.mChildType) {
        case foom::club::converter_data::child_type::second:
        case foom::club::converter_data::child_type::second_affiliated:
        case foom::club::converter_data::child_type::extinct_b_or_c:
        case foom::club::converter_data::child_type::other:
            teamTypeName = secondTypeName;
            break;
        case foom::club::converter_data::child_type::third:
            teamTypeName = thirdTypeName;
            break;
        case foom::club::converter_data::child_type::u23:
            teamTypeName = L"U23";
            break;
        case foom::club::converter_data::child_type::u21:
        case foom::club::converter_data::child_type::u21_b:
            teamTypeName = L"U21";
            break;
        case foom::club::converter_data::child_type::u20:
            teamTypeName = L"U20";
            break;
        case foom::club::converter_data::child_type::u19:
        case foom::club::converter_data::child_type::u19_b:
            teamTypeName = L"U19";
            break;
        case foom::club::converter_data::child_type::u18:
        case foom::club::converter_data::child_type::u18_b:
            teamTypeName = L"U18";
            break;
        }

        teamTypeName = L" " + teamTypeName;
        for (UInt s = 0; s < dst->mName.size(); s++) {
            if (dst->mName[s].length() <= 29 - teamTypeName.length())
                dst->mName[s] = FifamNames::LimitName(dst->mName[s], 29 - teamTypeName.length()) + teamTypeName;
            else
                dst->mName[s] = FifamNames::LimitName(dst->mShortName[s], 29 - teamTypeName.length()) + teamTypeName;
            dst->mShortName[s] = FifamNames::LimitName(dst->mShortName[s], 10 - teamTypeName.length()) + teamTypeName;
        }
    }

    dst->mName2 = dst->mName;
    dst->mShortName2 = dst->mShortName;

    // abbr
    FifamTrSetAll(dst->mAbbreviation, FifamNames::GetClubAbbr(team->mShortName));
    // year of foundation & traditional club
    if (team->mYearFounded <= 0)
        dst->mYearOfFoundation = 2000;
    else
        dst->mYearOfFoundation = team->mYearFounded;

    // city and settlement
    if (team->mCity)
        SetNameAndTranslation(dst->mCityName, team->mCity->mName, team->mCity->mTranslatedNames, 29);
    else if (country)
        dst->mCityName = country->mNationalTeam.mCityName;
    else
        FifamTrSetAll<String>(dst->mCityName, L"City");

    // latitude/longitude
    if (team->mLatitude != 0 || team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else if (country)
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;

    if (dst->mGeoCoords.mLatitude.ToInt() == 0 && dst->mGeoCoords.mLongitude.ToInt() == 0 && team->mNation) {
        for (auto &city : team->mNation->mConverterData.mCities) {
            if (city->mLatitude != 0 && city->mLongitude != 0) {
                dst->mGeoCoords.SetFromFloat(city->mLatitude, city->mLongitude);
                break;
            }
        }
    }

    // Fans & attendance
    dst->mAverageAttendanceLastSeason = team->mAttendance / 100;
    if (dst->mAverageAttendanceLastSeason < 50)
        dst->mAverageAttendanceLastSeason = 50;
    dst->mMaxAttendance = dst->mAverageAttendanceLastSeason;
    team->mNumberOfSeasonTicketHolders = 10;
    dst->mPotentialFansCount = dst->mAverageAttendanceLastSeason;
    if (dst->mPotentialFansCount > 250'000)
        dst->mPotentialFansCount = 250'000;

    // Stadium
    if (FifamTr(dst->mStadiumName).empty()) {
        for (UInt s = 0; s < dst->mStadiumName.size(); s++) {
            String stadSuffix = (s == FifamTranslation::German) ? L" Stadion" : L" Stadium";
            dst->mStadiumName[s] = FifamNames::LimitName(dst->mCityName[s], 29 - stadSuffix.length()) + stadSuffix;
        }
    }
    if (team->mReputation > 8'000)
        dst->mStadiumSeatsCapacity = 3'000;
    else if (team->mReputation > 7'000)
        dst->mStadiumSeatsCapacity = 2'500;
    else if (team->mReputation > 6'000)
        dst->mStadiumSeatsCapacity = 2'000;
    else if (team->mReputation > 5'000)
        dst->mStadiumSeatsCapacity = 1'500;
    else if (team->mReputation > 4'000)
        dst->mStadiumSeatsCapacity = 1'000;
    else if (team->mReputation > 3'000)
        dst->mStadiumSeatsCapacity = 500;
    else if (team->mReputation > 2'000)
        dst->mStadiumSeatsCapacity = 250;
    else
        dst->mStadiumSeatsCapacity = 200;

    // Club facilities
    Int clubDefaultFacilitiesValue = team->mReputation / 500;
    if (clubDefaultFacilitiesValue < 1)
        clubDefaultFacilitiesValue = 1;
    Int clubDefaultFacilitiesForUnknown = clubDefaultFacilitiesValue;
    if (clubDefaultFacilitiesForUnknown > 1)
        clubDefaultFacilitiesForUnknown -= 1;


    // club facilities is based on training facilities (66,7%) and corporate facilities (33,3%)
    dst->mClubFacilities = Utils::MapTo((team->mTraining + team->mCorporateFacilities / 2) / 2, 1, 30, 0, 6);
    Int youthFacilities = team->mYouthFacilities;
    if (youthFacilities <= 0)
        youthFacilities = clubDefaultFacilitiesForUnknown;
    dst->mYouthCentre = Utils::MapTo(youthFacilities / 2, 1, 20, 1, 10);
    Int youthCoaching = team->mYouthCoaching;
    if (youthCoaching <= 0)
        youthCoaching = clubDefaultFacilitiesForUnknown;
    dst->mYouthBoardingSchool = Utils::MapTo(youthCoaching / 2, 1, 20, 0, 10);
    Int trainingGrounds = team->mTraining;
    if (trainingGrounds <= 0)
        trainingGrounds = clubDefaultFacilitiesForUnknown;
    dst->mTrainingGrounds = Utils::MapTo(trainingGrounds / 2, 1, 20, 0, 5);
    Int corporateFacilities = team->mCorporateFacilities;
    if (corporateFacilities <= 0)
        corporateFacilities = 1;
    dst->mMerchandising = 0;
    dst->mMedicalDepartment = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue / 2, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mMedia = 0;
    dst->mResearch = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue / 2, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);

    // AI strategy
    dst->mAiStrategy = FifamClubAiStrategy::TrainingAndYouth;

    // settlement
    if (team->mCity && team->mCity->mInhabitants > 0) {
        if (team->mCity->mInhabitants >= 10) // 500'001+
            dst->mSettlement = FifamClubSettlement::City;
        else if (team->mCity->mInhabitants >= 6) // 25'001+
            dst->mSettlement = FifamClubSettlement::Town;
        else
            dst->mSettlement = FifamClubSettlement::Village;
    } else {
        if (dst->mStadiumSeatsCapacity <= 50000) {
            if (dst->mStadiumSeatsCapacity > 20000)
                dst->mSettlement = FifamClubSettlement::Town;
            else
                dst->mSettlement = FifamClubSettlement::Village;
        } else
            dst->mSettlement = FifamClubSettlement::City;
    }

    // Youth players come from
    if (team->mBasedNation)
        dst->mYouthPlayersCountry.SetFromInt(team->mBasedNation->mConverterData.mFIFAManagerReplacementID);
    else
        dst->mYouthPlayersCountry.SetFromInt(country->mId);
    // Youth players are basques
    dst->mYouthPlayersAreBasques = team->is_basque();

    dst->mInternationalPrestige = 0;

    ConvertKitsAndColors(dst, team->mID, mainTeam->mVecKits, team->mBadge, team->mBackgroundColor, team->mForegroundColor, gameId);
}

void Converter::ConvertClubStadium(FifamClub * dst, UInt gameId) {

    /* SPAIN */

    if (dst->mFifaID == 240) // Vicente Calderon (Madrid)
        dst->mStadiumVenue = 42;
    else if (dst->mFifaID == 241) { // Camp Nou (Barcelona)
        if (gameId != 13)
            dst->mStadiumVenue = 6;
    }
    else if (dst->mFifaID == 243) { // Santiago Bernabeu (Madrid)
        if (gameId >= 10)
            dst->mStadiumVenue = 2;
    }
    else if (dst->mFifaID == 461) { // Mestalla
        if (gameId >= 9 && gameId <= 12)
            dst->mStadiumVenue = 10;
    }

    /* ENGLAND */

    else if (dst->mFifaID == 1) { // Highbury (Arsenal), The Emirates Stadium (London)
        if (gameId <= 7)
            dst->mStadiumVenue = 7;
        else
            dst->mStadiumVenue = 156;
    }
    else if (dst->mFifaID == 5) // Stamford Bridge (London)
        dst->mStadiumVenue = 28;
    else if (dst->mFifaID == 9) // Anfield (Liverpool)
        dst->mStadiumVenue = 13;
    else if (dst->mFifaID == 11) // Old Trafford (Manchester)
        dst->mStadiumVenue = 1;
    else if (dst->mFifaID == 13) // St. James' Park (Newcastle)
        dst->mStadiumVenue = 100;
    else if (dst->mFifaID == 18) { // White Hart Lane (London), New Wembley (London)
        if (gameId >= 8) {
            if (gameId >= 13)
                dst->mStadiumVenue = 116;
            else
                dst->mStadiumVenue = 155;
        }
    }
    else if (dst->mFifaID == 1318) { // New Wembley (London)
        if (gameId >= 8 && gameId <= 12)
            dst->mStadiumVenue = 155;
    }
    else if (dst->mFifaID == 1961) { // Millennium Stadium (Cardiff)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 103;
    }

    /* ITALY */

    else if (dst->mFifaID == 44 || dst->mFifaID == 47) // San Siro (Milan)
        dst->mStadiumVenue = 5;
    else if (dst->mFifaID == 45) { // Delle Alpi (Turin)
        if (gameId <= 10)
            dst->mStadiumVenue = 8;
    }
    else if (dst->mFifaID == 46 || dst->mFifaID == 52) { // Stadio Olimpico (Rome)
        if (gameId >= 8)
            dst->mStadiumVenue = 157;
    }

    /* GERMANY */

    else if (dst->mFifaID == 21) { // Allianz Arena (Munich)
        if (gameId >= 8)
            dst->mStadiumVenue = 137;
    }
    else if (dst->mFifaID == 22) // Signal Iduna Park (Dortmund)
        dst->mStadiumVenue = 9;
    else if (dst->mFifaID == 28) // Imtech Arena (Hamburg)
        dst->mStadiumVenue = 41;
    else if (dst->mFifaID == 32) { // BayArena (Leverkusen)
        if (gameId <= 10)
            dst->mStadiumVenue = 12;
    }
    else if (dst->mFifaID == 34) { // Veltins-Arena (Gelsenkirchen)
        if (gameId >= 8)
            dst->mStadiumVenue = 30;
    }
    else if (dst->mFifaID == 36) { // Gottlieb Daimler Stadion (Stuttgart)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 133;
    }
    else if (dst->mFifaID == 166) { // Olympiastadion Berlin (Berlin)
        if (gameId >= 8)
            dst->mStadiumVenue = 135;
    }
    else if (dst->mFifaID == 485) { // HDI Arena (Hanover)
        if (gameId == 8 || gameId == 9 || gameId >= 11)
            dst->mStadiumVenue = 134;
    }
    else if (dst->mFifaID == 1824) { // Commerzbank Arena (Frankfurt)
        if (gameId == 8 || gameId == 9 || gameId >= 11)
            dst->mStadiumVenue = 138;
    }

    /* FRANCE */

    else if (dst->mFifaID == 66) // Stade Gerland (Lyon)
        dst->mStadiumVenue = 4;
    else if (dst->mFifaID == 73) { // Parc des Princes
        if (gameId != 11)
            dst->mStadiumVenue = 14;
    }
    else if (dst->mFifaID == 64) // Stade Bollaert-Delelis (Lens)
        dst->mStadiumVenue = 16;
    else if (dst->mFifaID == 219) { // Stade Velodrome
        if (gameId != 11)
            dst->mStadiumVenue = 29;
    }

    /* PORTUGAL */

    else if (dst->mFifaID == 234) { // Estadio Jose Alvalade
        if (gameId != 11)
            dst->mStadiumVenue = 107;
    }
    else if (dst->mFifaID == 236) { // Estadio do Dragao
        if (gameId != 11)
            dst->mStadiumVenue = 111;
    }
    else if (dst->mFifaID == 237) { // Estadio Jose Alvalade
        if (gameId != 11)
            dst->mStadiumVenue = 102;
    }
    else if (dst->mFifaID == 1898) { // Estadio Da Bessa (Boavista Porto)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 123;
    }

    /* NETHERLANDS */

    else if (dst->mFifaID == 245) // Amsterdam ArenA (Amsterdam)
        dst->mStadiumVenue = 15;

    /* BELGIUM */

    else if (dst->mFifaID == 229) // Constant Vanden Stock (Brussels)
        dst->mStadiumVenue = 17;

    /* TURKEY */

    else if (dst->mFifaID == 325) { // Ali Sami Yen Stadi (Istanbul)
        if (gameId <= 7)
            dst->mStadiumVenue = 101;
    }

    /* MEXICO */

    else if (dst->mFifaID == 1878) { // Azteca (Mexico City)
        if (gameId >= 8)
            dst->mStadiumVenue = 104;
    }
    else if (dst->mFifaID == 1880) { // Jalisco (Guadalajara)
        if (gameId == 10 || gameId == 11 || gameId == 13 || gameId == 14)
            dst->mStadiumVenue = 163;
    }

    /* USA */

    else if (dst->mFifaID == 697) { // Home Depot Center (Los Angeles)
        if (gameId >= 8 && gameId <= 10)
            dst->mStadiumVenue = 161;
    }

    /* SOUTH KOREA */

    else if (dst->mFifaID == 982) { // Seoul World Cup Stadium (Seoul)
        if (gameId >= 8)
            dst->mStadiumVenue = 108;
    }
    else if (dst->mFifaID == 2056) { // Daegu Stadium (Daegu)
        if (gameId >= 8)
            dst->mStadiumVenue = 110;
    }

}

void Converter::ConvertKitsAndColors(FifamClub * dst, Int foomId, Vector<foom::kit> const &kits, Int badgeType, Color const &teamBackgroundColor, Color const &teamForegroundColor, UInt gameId) {
    enum KitPart { Shirt, Icon, Text, Shorts, Socks };

    const foom::kit *tmpKitSets[5][2] = {};
    const foom::kit *tmpCompKitSets[5][2] = {};

    for (auto &k : kits) {
        if (k.mKit == 0 || k.mKit == 1) {
            if (k.mType >= 1 && k.mType <= 5 && k.mAlternativeKitNumber == 0 && (k.mYear == 0 || k.mYear == CURRENT_YEAR)) {
                if (k.mCompetition) {
                    if (!tmpCompKitSets[k.mType - 1][k.mKit])
                        tmpCompKitSets[k.mType - 1][k.mKit] = &k;
                }
                else {
                    if (!tmpKitSets[k.mType - 1][k.mKit])
                        tmpKitSets[k.mType - 1][k.mKit] = &k;
                }
            }
        }
    }

    const foom::kit *kitSets[5][2] = {};
    for (UInt k = 0; k < 2; k++) {
        for (UInt t = 0; t < 5; t++)
            kitSets[t][k] = tmpKitSets[t][k] ? tmpKitSets[t][k] : tmpCompKitSets[t][k];
    }

    Bool textColorsUsed = false;

    const foom::kit *clubClrs = nullptr;
    if (badgeType != 11 && kitSets[2][0]) {
        clubClrs = kitSets[2][0];
        textColorsUsed = true;
    }
    else {
        for (UInt i = 0; i < 5; i++) {
            if (kitSets[i][0]) {
                clubClrs = kitSets[i][0];
                break;
            }
        }
    }

    // badge and colors

    Color backgroundClr, foregroundClr;

    ClubColor clubColorInfo;
    if (mClubColorsMap.find(foomId) != mClubColorsMap.end())
        clubColorInfo = mClubColorsMap[foomId];
    else
        clubColorInfo.type = ClubColor::DEFAULT;

    auto SetColor_RGB = [&]() {
        backgroundClr = clubColorInfo.rgb[0];
        foregroundClr = clubColorInfo.rgb[1];
        dst->mClubColour.Set(clubColorInfo.rgb[0], clubColorInfo.rgb[1]);
        return true;
    };

    auto SetColor_TEAM = [&](bool forceApply = false) {
        if (forceApply || (teamBackgroundColor != Color(45, 104, 181) && teamForegroundColor != Color(250, 250, 250))) {
            backgroundClr = teamBackgroundColor;
            foregroundClr = teamForegroundColor;
            Color secondColor = foregroundClr;
            if (Color::Distance(backgroundClr, foregroundClr) < 100) {
                auto white = Color::Distance(backgroundClr, { 255, 255, 255 });
                auto black = Color::Distance(backgroundClr, { 0, 0, 0 });
                if (black < white)
                    secondColor = { 255, 255, 255 };
                else
                    secondColor = { 0, 0, 0 };
            }
            dst->mClubColour.Set(backgroundClr, secondColor);
            return true;
        }
        return false;
    };

    auto SetColor_KIT = [&]() {
        if (clubClrs) {
            backgroundClr = Color(clubClrs->mBackground.r, clubClrs->mBackground.g, clubClrs->mBackground.b);
            foregroundClr = Color(clubClrs->mForeground.r, clubClrs->mForeground.g, clubClrs->mForeground.b);
            Color outlineClr = Color(clubClrs->mOutline.r, clubClrs->mOutline.g, clubClrs->mOutline.b);
            if (textColorsUsed)
                std::swap(foregroundClr, outlineClr);
            Color secondColor = foregroundClr;
            if (Color::Distance(backgroundClr, foregroundClr) < 100) {
                if (Color::Distance(backgroundClr, outlineClr) < 100) {
                    if (Color::Distance(backgroundClr, outlineClr) < 100) {
                        auto white = Color::Distance(backgroundClr, { 255, 255, 255 });
                        auto black = Color::Distance(backgroundClr, { 0, 0, 0 });
                        if (black < white)
                            secondColor = { 255, 255, 255 };
                        else
                            secondColor = { 0, 0, 0 };
                    }
                    else
                        secondColor = outlineClr;
                }
                else
                    secondColor = outlineClr;
            }
            dst->mClubColour.Set(backgroundClr, secondColor);
            return true;
        }
        return false;
    };

    bool colorsFound = false;

    if (clubColorInfo.type == ClubColor::RGB)
        colorsFound = SetColor_RGB();
    else if (clubColorInfo.type == ClubColor::TEAM)
        colorsFound = SetColor_TEAM(true);
    else if (clubColorInfo.type == ClubColor::KIT)
        colorsFound = SetColor_KIT();
    else {
        colorsFound = SetColor_KIT();
        if (!colorsFound)
            colorsFound = SetColor_TEAM(false);
    }

    if (!colorsFound) {
        //FifamClubTeamColor badgeColor;
        //if (ClubColorsFromBadgeFile(dst->mUniqueID, badgeColor)) {
        //    dst->mClubColour.SetFromTable(FifamClub::mTeamColorsTable, badgeColor.ToInt());
        //    backgroundClr = dst->mClubColour.second;
        //    foregroundClr = dst->mClubColour.first;
        //}
        auto it = mRefDbColors.find(dst->mUniqueID);
        if (it != mRefDbColors.end()) {
            dst->mClubColour = (*it).second;
            backgroundClr = dst->mClubColour.first;
            foregroundClr = dst->mClubColour.second;
            colorsFound = true;
        }
        if (!colorsFound) {
            //random club colors
            UInt numTeamColors = FifamClub::mTeamColorsTable.size();
            if (numTeamColors > 0) {
                dst->mClubColour.SetFromTable(FifamClub::mTeamColorsTable, Random::Get(0, 27));
                if (Random::Get(1, 100) > 50) {
                    backgroundClr = dst->mClubColour.second;
                    foregroundClr = dst->mClubColour.first;
                }
                else {
                    backgroundClr = dst->mClubColour.first;
                    foregroundClr = dst->mClubColour.second;
                }
            }
        }
    }

    dst->mClubColour2 = dst->mClubColour;
    dst->mMerchandiseColour = backgroundClr;
    if (Color::Distance(backgroundClr, Color(255, 255, 255)) < 100) {
        dst->mBackgroundColour = foregroundClr;
        dst->mHeaderColour = backgroundClr;
    }
    else {
        dst->mBackgroundColour = backgroundClr;
        dst->mHeaderColour = foregroundClr;
    }

    bool hasCustomBadge = false;

    if (mFromFifaDatabase) {
        if (dst->mFifaID != 0 && !dst->mIsNationalTeam) {
            if (exists(mFifaAssetsPath / L"crest" / Utils::Format(L"l%d.dds", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"crest" / Utils::Format(L"%d.png", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"_crest" / Utils::Format(L"l%d.dds", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"_crest" / Utils::Format(L"%d.png", dst->mFifaID)) ||
                exists(Utils::Format(mOutputGameFolder / L"badges\\clubs\\256x256\\%08X.tga", gameId, dst->mUniqueID))
                )
            {
                dst->mBadge.SetBadgePath(L"clubs\\Badge%d\\" + Utils::Format(L"%08X", dst->mUniqueID) + L".tga");
                hasCustomBadge = true;
            }
        }
    }
    else {
        if (gameId <= 9 && foomId != -1) {
            auto badgeIt = mAvailableBadges.find(foomId);
            if (badgeIt != mAvailableBadges.end()) {
                dst->mBadge.SetBadgePath(L"clubs\\Badge%d\\" + Utils::Format(L"%08X", dst->mUniqueID) + L".tga");
                hasCustomBadge = true;
            }
        }
    }

    if (!hasCustomBadge) {
        dst->mBadge.SetColorDefault(dst->mClubColour);

        if (badgeType != -1) {
            Bool inverted = Random::Get(0, 1);
            switch (badgeType) {
            case 0:
                dst->mBadge.SetColor(FifamClubBadgeShape::Ball, 1, 1, inverted);
                break;
            case 1:
            case 13:
                dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, 7, inverted);
                break;
            case 2:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 5, Random::Get(1, 4), inverted);
                break;
            case 3:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 2, 1, inverted);
                break;
            case 4:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 6, 1, inverted);
                break;
            case 5:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 17, 1, inverted);
                break;
            case 6:
                dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, Random::Get(1, 2), false);
                break;
            case 7:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 2, inverted);
                break;
            case 8:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 8, 1, inverted);
                break;
            case 9:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 4, inverted);
                break;
            case 10:
                dst->mBadge.SetColor(FifamClubBadgeShape::Oval, 1, 1, false);
                break;
            case 11:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 10, 1, inverted);
                break;
            case 12:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 9, 1, inverted);
                break;
            }
        }
    }

    // home & away kits
    for (UInt i = 0; i < 2; i++) {
        auto &kit = dst->mKit.mSets[i];
        // kit - number
        if (kitSets[KitPart::Shirt][i])
            kit.mShirtNumberColor = kitSets[KitPart::Shirt][i]->mNumberColour;
        else {
            if (i == 0)
                kit.mShirtNumberColor = foregroundClr;
            else
                kit.mShirtNumberColor = backgroundClr;
        }
        // kit - armband
        kit.mCaptainArmbandColor = kit.mShirtNumberColor;
        // kit - socks
        kit.mSocks = 1;
        if (i == 0)
            kit.mSocksColors = { backgroundClr, backgroundClr };
        else
            kit.mSocksColors = { foregroundClr, foregroundClr };
        if (kitSets[KitPart::Socks][i]) {
            auto &set = kitSets[KitPart::Socks][i];
            switch (set->mKitStyle) {
            case 1:
            case 3:
            case 14:
            case 15:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mBackground };
                break;
            case 2:
            case 4:
            case 5:
            case 7:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 6:
            case 11:
            case 12:
                kit.mSocks = 4;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 8:
            case 9:
            case 13:
                kit.mSocks = 2;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 10:
                kit.mSocks = 3;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            }
        }
        // kit - shorts
        kit.mShorts = 1;
        if (i == 0)
            kit.mShortsColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShortsColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shorts][i]) {
            auto &set = kitSets[KitPart::Shorts][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShorts = 1;
                kit.mShortsColors = { set->mBackground, set->mBackground, set->mBackground };
                break;
            case 2:
                kit.mShorts = 6;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 3:
            case 4:
                kit.mShorts = 7;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 5:
                kit.mShorts = 4;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                kit.mShorts = 3;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            }
        }
        // kit - shirts
        kit.mShirt = 4;
        if (i == 0)
            kit.mShirtColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShirtColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shirt][i]) {
            auto &set = kitSets[KitPart::Shirt][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 2:
                kit.mShirt = 46;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 3:
            case 48:
                if (set->mBackOfShirtStyle == 2) // Plain
                    kit.mShirt = 14;
                else
                    kit.mShirt = 41;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 4:
            case 49:
            case 51:
                kit.mShirt = 48;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 5:
                kit.mShirt = 37;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 6:
                kit.mShirt = 40;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 7:
                kit.mShirt = 49;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 8:
            case 38:
                kit.mShirt = 38;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 9:
                kit.mShirt = 36;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 10:
                kit.mShirt = 30;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 11:
            case 37:
                kit.mShirt = 19;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 12:
            case 55:
                kit.mShirt = 57;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 13:
                kit.mShirt = 21;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 14:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 15:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 16:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 17:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 18:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 19:
                kit.mShirt = 60;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 20:
            case 39:
                kit.mShirt = 39;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 21:
                kit.mShirt = 22;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 22:
            case 23:
            case 31:
            case 32:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 24:
            case 33:
                kit.mShirt = 12;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 25:
                kit.mShirt = 9;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 26:
            case 27:
                kit.mShirt = 52;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 28:
                kit.mShirt = 47;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 29:
                kit.mShirt = 11;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 30:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 34:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 35:
                kit.mShirt = 8;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 36:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mBackground };
                break;
            case 40:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 41:
                kit.mShirt = 63;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mOutline };
                break;
            case 42:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 43:
                kit.mShirt = 13;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 44:
                kit.mShirt = 65;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 45:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 46:
            case 47:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 50:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 52:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 53:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 54:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 56:
                kit.mShirt = 57;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            }
        }

        // (if club has no FIFA kit)
        if (!exists(mOutputGameFolder / (L"data\\kits\\" + Utils::Format(L"%08X", dst->mUniqueID) + L"_h.tga"))
            && !exists(mContentArtsFolder / (L"art_04\\data\\kits\\" + Utils::Format(L"%08X", dst->mUniqueID) + L"_h.tga")))
        {
            Color shirtBackColor = FifamKit::GetShirtBackColor(kit.mShirt, kit.mShirtColors[0], kit.mShirtColors[1], kit.mShirtColors[2]);
            if (Color::Distance(kit.mShirtNumberColor, shirtBackColor) < 100) {
                if (Color::Distance(kit.mShirtNumberColor, foregroundClr) < 100)
                    kit.mShirtNumberColor = backgroundClr;
                else
                    kit.mShirtNumberColor = foregroundClr;
            }
        }
    }
}

FifamClub *Converter::CreateAndConvertClub(UInt gameId, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div, bool convertSquad) {
    if (!mainTeam)
        mainTeam = team;
    FifamClub *club = mFifamDatabase->CreateClub(country);
    team->mConverterData.mHasFifamSquad = convertSquad;
    club->SetProperty(L"foom::club", team);
    // uid
    club->mUniqueID = team->mConverterData.mFIFAManagerID;
    if (club->mUniqueID != 0) {
        UChar countryId = FifamUtils::GetCountryIDFromClubID(club->mUniqueID);
        if (countryId >= 1 && countryId <= FifamDatabase::NUM_COUNTRIES) {
            if (countryId != country->mId) {
                if (mErrors) {
                    Error(L"Incorrect Club Country in UniqueID\nClub: '%s'\nUniqueID: %08X\nCountryId: %d\nIncorrectCountryId: %d",
                        team->mName.c_str(), club->mUniqueID, country->mId, countryId);
                }
            }
        }
        else {
            if (mErrors) {
                Error(L"Incorrect Club UniqueID\nClub: '%s'\nUniqueID: %08X\nIncorrectCountryId: %d",
                    team->mName.c_str(), club->mUniqueID, countryId);
            }
        }
    }
    if (club->mUniqueID == 0)
        club->mUniqueID = (country->mId << 16) | mNextFreeUID[country->mId - 1]++;
    club->mFifaID = team->mConverterData.mFIFAID;

    if (mainTeam != team)
        ConvertReserveClub(gameId, club, team->mIsReserveToCreateClub ? mainTeam : team, mainTeam, country, div);
    else
        ConvertClub(gameId, club, team, mainTeam, country, div);

    {
        auto it = mPenaltyPointsMap.find(team->mID);
        if (it != mPenaltyPointsMap.end()) {
            club->mPenaltyType = FifamClubPenaltyType::Points;
            club->mPenaltyPoints = (*it).second;
        }
    }
    {
        auto it = mNamesMap.find(team->mID);
        if (it != mNamesMap.end()) {
            FifamTrSetAll<String>(club->mName, (*it).second);
            FifamTrSetAll<String>(club->mName2, (*it).second);
        }
    }
    {
        auto it = mShortNamesMap.find(team->mID);
        if (it != mShortNamesMap.end()) {
            FifamTrSetAll<String>(club->mShortName, (*it).second);
            FifamTrSetAll<String>(club->mShortName2, (*it).second);
        }
    }
    {
        auto it = mAbbreviationMap.find(team->mID);
        if (it != mAbbreviationMap.end())
            FifamTrSetAll<String>(club->mAbbreviation, (*it).second);
    }
    {
        auto it = mNamesMap_ger.find(team->mID);
        if (it != mNamesMap_ger.end()) {
            club->mName[FifamTranslation::German] = (*it).second;
            club->mName2[FifamTranslation::German] = (*it).second;
        }
    }
    {
        auto it = mShortNamesMap_ger.find(team->mID);
        if (it != mShortNamesMap_ger.end()) {
            club->mShortName[FifamTranslation::German] = (*it).second;
            club->mShortName2[FifamTranslation::German] = (*it).second;
        }
    }
    {
        auto it = mAbbreviationMap_ger.find(team->mID);
        if (it != mAbbreviationMap_ger.end())
            club->mAbbreviation[FifamTranslation::German] = (*it).second;
    }

    mFifamDatabase->AddClubToMap(club);
    country->mClubsMap[club->mUniqueID] = club;

    team->mConverterData.mFifamClub = club;

    return club;
}
