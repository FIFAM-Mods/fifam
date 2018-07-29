#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WriteDbClubsTest {
public:
    WriteDbClubsTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        FifamWriter writer(Utils::Format(L"clubs_%02d.csv", Game::id()), Game::id(), Game::year(), Game::vernum(), Game::unicode());
        if (writer.Available()) {
            writer.Write(
                L"Country name",
                L"UniqueID",
                L"Name",
                L"Name2",
                L"ShortName",
                L"ShortName2",
                L"Abbreviation",
                L"CityName",
                L"PlayerInText",
                L"TermForTeam1",
                L"TermForTeam2",
                L"FanName1",
                L"FanName2",
                L"ClubNameUsageInPhrase",
                L"ClubNameUsageInPhrase2",
                L"AbbreviationArticle",
                L"TermForTeam1Article",
                L"TermForTeam2Article",
                L"FanName1Article",
                L"FanName2Article",
                L"Address",
                L"Telephone",
                L"WebsiteAndMail",
                L"Fansites",
                L"MascotName",
                L"PartnershipClub",
                L"RivalClubs_0",
                L"RivalClubs_1",
                L"RivalClubs_2",
                L"RivalClubs_3",
                L"YearOfFoundation",
                L"InitialCapital",
                L"TransferBudget",
                L"JointStockCompany",
                L"PenaltyFlags",
                L"PenaltyPoints",
                L"PotentialFansCount",
                L"AverageAttendanceLastSeason",
                L"CountOfSoldSeasonTickets",
                L"LeagueTotalPoints_0",
                L"LeagueTotalPoints_1",
                L"LeagueTotalPoints_2",
                L"LeagueTotalWins_0",
                L"LeagueTotalWins_1",
                L"LeagueTotalWins_2",
                L"LeagueTotalDraws_0",
                L"LeagueTotalDraws_1",
                L"LeagueTotalDraws_2",
                L"LeagueTotalLoses_0",
                L"LeagueTotalLoses_1",
                L"LeagueTotalLoses_2",
                L"LeagueTotalGoals_0",
                L"LeagueTotalGoals_1",
                L"LeagueTotalGoals_2",
                L"LeagueTotalGoalsAgainst_0",
                L"LeagueTotalGoalsAgainst_1",
                L"LeagueTotalGoalsAgainst_2",
                L"LeagueTotalLeadershipsInTable_0",
                L"LeagueTotalLeadershipsInTable_1",
                L"LeagueTotalLeadershipsInTable_2",
                L"StadiumVenue",
                L"StadiumCapacity",
                L"StadiumName",
                L"StadiumType",
                L"Flags",
                L"ClubFacilities",
                L"YouthCentre",
                L"YouthBoardingSchool",
                L"MedicalDepartment",
                L"Merchandising",
                L"TrainingGrounds",
                L"Media",
                L"Research",
                L"AiStrategy",
                L"Landscape",
                L"Settlement",
                L"LastSeasonFlags",
                L"Badge",
                L"ClubColour",
                L"MerchandiseColour",
                L"HeaderColour",
                L"BackgroundColour",
                L"Latitude",
                L"Longitude",
                L"NationalPrestige",
                L"InternationalPrestige",
                L"TransfersCountry1",
                L"TransfersCountry2",
                L"YouthPlayersCountry",
                L"FifaID",
                L"LowestLeagues_0",
                L"LowestLeagues_1",
                L"LowestLeagues_2",
                L"LowestLeagues_3",
                L"MediaPressure",
                L"AdditionalFlags",
                L"Unknown_1",
                L"Unknown_2",
                L"Unknown_3",
                L"Unknown_4"
            );
            writer.WriteNewLine();
            for (auto country : db->mCountries) {
                for (auto c : country->mClubs) {
                    writer.Write(
                        Tr(country->mName),
                        Hexademical(c->mUniqueID),
                        Quoted(Tr(c->mName)),
                        Quoted(Tr(c->mName2)),
                        Quoted(Tr(c->mShortName)),
                        Quoted(Tr(c->mShortName2)),
                        Quoted(Tr(c->mAbbreviation)),
                        Quoted(Tr(c->mCityName)),
                        Quoted(Tr(c->mPlayerInText)),
                        Quoted(Tr(c->mTermForTeam1)),
                        Quoted(Tr(c->mTermForTeam2)),
                        Quoted(Tr(c->mFanName1)),
                        Quoted(Tr(c->mFanName2)),
                        Tr(c->mClubNameUsageInPhrase),
                        Tr(c->mClubNameUsageInPhrase2),
                        Tr(c->mAbbreviationArticle),
                        Tr(c->mTermForTeam1Article),
                        Tr(c->mTermForTeam2Article),
                        Tr(c->mFanName1Article),
                        Tr(c->mFanName2Article),
                        Quoted(c->mAddress),
                        Quoted(c->mTelephone),
                        Quoted(c->mWebsiteAndMail),
                        Quoted(c->mFansites),
                        Quoted(c->mMascotName),
                        reinterpret_cast<UInt>(c->mPartnershipClub),
                        reinterpret_cast<UInt>(c->mRivalClubs[0]),
                        reinterpret_cast<UInt>(c->mRivalClubs[1]),
                        reinterpret_cast<UInt>(c->mRivalClubs[2]),
                        reinterpret_cast<UInt>(c->mRivalClubs[3]),
                        c->mYearOfFoundation,
                        c->mInitialCapital,
                        c->mTransferBudget,
                        c->mJointStockCompany,
                        c->mPenaltyFlags,
                        c->mPenaltyPoints,
                        c->mPotentialFansCount,
                        c->mAverageAttendanceLastSeason,
                        c->mCountOfSoldSeasonTickets,
                        c->mLeagueTotalPoints[0],
                        c->mLeagueTotalPoints[1],
                        c->mLeagueTotalPoints[2],
                        c->mLeagueTotalWins[0],
                        c->mLeagueTotalWins[1],
                        c->mLeagueTotalWins[2],
                        c->mLeagueTotalDraws[0],
                        c->mLeagueTotalDraws[1],
                        c->mLeagueTotalDraws[2],
                        c->mLeagueTotalLoses[0],
                        c->mLeagueTotalLoses[1],
                        c->mLeagueTotalLoses[2],
                        c->mLeagueTotalGoals[0],
                        c->mLeagueTotalGoals[1],
                        c->mLeagueTotalGoals[2],
                        c->mLeagueTotalGoalsAgainst[0],
                        c->mLeagueTotalGoalsAgainst[1],
                        c->mLeagueTotalGoalsAgainst[2],
                        c->mLeagueTotalLeadershipsInTable[0],
                        c->mLeagueTotalLeadershipsInTable[1],
                        c->mLeagueTotalLeadershipsInTable[2],
                        c->mStadiumVenue,
                        c->mStadiumCapacity,
                        Tr(c->mStadiumName),
                        c->mStadiumType.ToStr(),
                        c->mFlags,
                        c->mClubFacilities,
                        c->mYouthCentre,
                        c->mYouthBoardingSchool,
                        c->mMedicalDepartment,
                        c->mMerchandising,
                        c->mTrainingGrounds,
                        c->mMedia,
                        c->mResearch,
                        c->mAiStrategy.ToStr(),
                        c->mLandscape.ToStr(),
                        c->mSettlement.ToStr(),
                        c->mLastSeasonFlags,
                        Quoted(c->mBadge.ToStr()),
                        c->mClubColour.ToHexStr(),
                        c->mMerchandiseColour.ToHexStr(),
                        c->mHeaderColour.ToHexStr(),
                        c->mBackgroundColour.ToHexStr(),
                        c->mGeoCoords.mLatitude.ToStr(),
                        c->mGeoCoords.mLongitude.ToStr(),
                        c->mNationalPrestige,
                        c->mInternationalPrestige,
                        c->mTransfersCountry1.ToStr(),
                        c->mTransfersCountry2.ToStr(),
                        c->mYouthPlayersCountry.ToStr(),
                        c->mFifaID,
                        Quoted(c->mLowestLeagues[0].ToStr()),
                        Quoted(c->mLowestLeagues[1].ToStr()),
                        Quoted(c->mLowestLeagues[2].ToStr()),
                        Quoted(c->mLowestLeagues[3].ToStr()),
                        c->mMediaPressure.ToStr(),
                        c->mAdditionalFlags,
                        Quoted(c->Unknown._1),
                        c->Unknown._2,
                        c->Unknown._3,
                        Quoted(c->Unknown._4)
                    );
                    // player history
                    // kit
                    writer.WriteNewLine();
                }
            }
        }
    }
};
