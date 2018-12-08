#pragma once
#include "FifamDbEnvironment.h"
#include "FifamUtils.h"

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
                L"Newspaper",
                L"MascotName",
                L"PartnershipClub",
                L"RivalClubs_0",
                L"RivalClubs_1",
                L"RivalClubs_2",
                L"RivalClubs_3",
                L"YearOfFoundation",
                L"InitialCapital",
                L"TransferBudget",
                L"SponsorAmount",
                L"SponsorDuration",
                L"SpecialSponsor",
                L"IndividualTvMoney",
                L"JointStockCompany",
                L"PenaltyType",
                L"PenaltyPoints",
                L"PotentialFansCount",
                L"AverageAttendanceLastSeason",
                L"CountOfSoldSeasonTickets",
                L"FanMembers",
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
                L"StadiumSeatsCapacity",
                L"StadiumStandingsCapacity",
                L"StadiumVipCapacity",
                L"StadiumName",
                L"StadiumType",
                L"StadiumBigMainStand",
                L"CanBeDeletedInEditor",
                L"UnknownFlag_2",
                L"UnknownFlag_4",
                L"UnknownFlag_8",
                L"UnknownFlag_16",
                L"UnknownFlag_32",
                L"UnknownFlag_64",
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
                L"FirstTeamLastSeasonLeague",
                L"FirstTeamLastSeasonCup",
                L"ReserveTeamLastSeasonLeague",
                L"ReserveTeamLastSeasonCup",
                L"DoesNotTakePartInITCup",
                L"Badge",
                L"ClubColour",
                L"ClubColour2",
                L"MerchandiseColour",
                L"HeaderColour",
                L"BackgroundColour",
                L"Latitude",
                L"Longitude",
                L"NationalPrestige",
                L"InternationalPrestige",
                L"TransfersCountry_0",
                L"TransfersCountry_1",
                L"YouthPlayersCountry",
                L"FifaID",
                L"LowestLeagues_0",
                L"LowestLeagues_1",
                L"LowestLeagues_2",
                L"LowestLeagues_3",
                L"MediaPressure",
                L"TraditionalClub",
                L"CantPlayInLeague",
                L"RichGuyControlled",
                L"YouthPlayersAreBasques",
                L"PreferredFormation_0",
                L"PreferredFormation_1",
                L"Captain_0",
                L"Captain_1",
                L"Captain_2",
                L"Unknown_2",
                L"Unknown_4",
                L"LeagueWinYears",
                L"CupWinYears",
                L"SuperCupsWinYears",
                L"LeagueCupWinYears",
                L"EuroTrophyWinYears",
                L"ChampionsCupWinYears",
                L"WorldChampionshipWinYears",
                L"WorldClubChampionshipWinYears",
                L"RecordHomeWin",
                L"RecordHomeDefeat",
                L"RecordAwayWin",
                L"RecordAwayDefeat",
                L"RecordAttendance",
                L"SpecialKitId",
                L"KitHomeShirt",
                L"KitHomeShorts",
                L"KitHomeSocks",
                L"KitHomeShirtColor_0",
                L"KitHomeShirtColor_1",
                L"KitHomeShirtColor_2",
                L"KitHomeSleevesColor_0",
                L"KitHomeSleevesColor_1",
                L"KitHomeSleevesColor_2",
                L"KitHomeShortsColor_0",
                L"KitHomeShortsColor_1",
                L"KitHomeShortsColor_2",
                L"KitHomeSocksColor_0",
                L"KitHomeSocksColor_1",
                L"KitHomeCaptainArmbandColor",
                L"KitHomeShirtNumberColor",
                L"KitHomeBadgePosition",
                L"KitAwayShirt",
                L"KitAwayShorts",
                L"KitAwaySocks",
                L"KitAwayShirtColor_0",
                L"KitAwayShirtColor_1",
                L"KitAwayShirtColor_2",
                L"KitAwaySleevesColor_0",
                L"KitAwaySleevesColor_1",
                L"KitAwaySleevesColor_2",
                L"KitAwayShortsColor_0",
                L"KitAwayShortsColor_1",
                L"KitAwayShortsColor_2",
                L"KitAwaySocksColors_0",
                L"KitAwaySocksColors_1",
                L"KitAwayCaptainArmbandColor",
                L"KitAwayShirtNumberColor",
                L"KitAwayBadgePosition"
            );
            writer.WriteNewLine();
            auto WriteClub = [](FifamWriter &writer, FifamCountry *country, FifamClub *c) {
                writer.Write(
                    FifamTr(country->mName),
                    Hexadecimal(c->mUniqueID),
                    Quoted(FifamTr(c->mName)),
                    Quoted(FifamTr(c->mName2)),
                    Quoted(FifamTr(c->mShortName)),
                    Quoted(FifamTr(c->mShortName2)),
                    Quoted(FifamTr(c->mAbbreviation)),
                    Quoted(FifamTr(c->mCityName)),
                    Quoted(FifamTr(c->mPlayerInText)),
                    Quoted(FifamTr(c->mTermForTeam1)),
                    Quoted(FifamTr(c->mTermForTeam2)),
                    Quoted(FifamTr(c->mFanName1)),
                    Quoted(FifamTr(c->mFanName2)),
                    FifamTr(c->mClubNameUsageInPhrase),
                    FifamTr(c->mClubNameUsageInPhrase2),
                    FifamTr(c->mAbbreviationArticle),
                    FifamTr(c->mTermForTeam1Article),
                    FifamTr(c->mTermForTeam2Article),
                    FifamTr(c->mFanName1Article),
                    FifamTr(c->mFanName2Article),
                    Quoted(c->mAddress),
                    Quoted(c->mTelephone),
                    Quoted(c->mWebsiteAndMail),
                    Quoted(c->mFansites),
                    Quoted(c->mNewspaper),
                    Quoted(c->mMascotName),
                    Quoted(c->mPartnershipClub.GetTeamName()),
                    Quoted(c->mRivalClubs[0].GetTeamName()),
                    Quoted(c->mRivalClubs[1].GetTeamName()),
                    Quoted(c->mRivalClubs[2].GetTeamName()),
                    Quoted(c->mRivalClubs[3].GetTeamName()),
                    c->mYearOfFoundation,
                    c->mInitialCapital,
                    c->mTransferBudget,
                    c->mSponsorAmount,
                    c->mSponsorDuration,
                    c->mSpecialSponsor,
                    c->mIndividualTvMoney,
                    c->mJointStockCompany,
                    c->mPenaltyType.ToStr(),
                    c->mPenaltyPoints,
                    c->mPotentialFansCount,
                    c->mAverageAttendanceLastSeason,
                    c->mCountOfSoldSeasonTickets,
                    c->mFanMembers,
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
                    c->mStadiumSeatsCapacity,
                    c->mStadiumStandingsCapacity,
                    c->mStadiumVipCapacity,
                    Quoted(FifamTr(c->mStadiumName)),
                    c->mStadiumType.ToStr(),
                    c->mStadiumBigMainStand,
                    c->mCanBeDeletedInEditor,
                    c->Unknown.flags._2,
                    c->Unknown.flags._4,
                    c->Unknown.flags._8,
                    c->Unknown.flags._16,
                    c->Unknown.flags._32,
                    c->Unknown.flags._64,
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
                    c->mFirstTeamLastSeasonInfo.mLeague.ToStr(),
                    c->mFirstTeamLastSeasonInfo.mCup.ToStr(),
                    c->mReserveTeamLastSeasonInfo.mLeague.ToStr(),
                    c->mReserveTeamLastSeasonInfo.mCup.ToStr(),
                    c->mDoesNotTakePartInITCup,
                    Quoted(c->mBadge.ToStr()),
                    FifamClubTeamColor::MakeFromInt(c->mClubColour.FindIndexInTable(FifamClub::mTeamColorsTable)).ToStr(),
                    FifamClubTeamColor::MakeFromInt(c->mClubColour2.FindIndexInTable(FifamClub::mTeamColorsTable)).ToStr(),
                    FifamClubMerchandiseColor::MakeFromInt(c->mMerchandiseColour.FindIndexInTable(FifamClub::mMerchandiseColorsTable)).ToStr(),
                    FifamClubInterfaceColor08::MakeFromInt(c->mHeaderColour.FindIndexInTable(FifamClub::m08InterfaceColorsTable)).ToStr(),
                    FifamClubInterfaceColor08::MakeFromInt(c->mBackgroundColour.FindIndexInTable(FifamClub::m08InterfaceColorsTable)).ToStr(),
                    c->mGeoCoords.mLatitude.ToStr(),
                    c->mGeoCoords.mLongitude.ToStr(),
                    c->mNationalPrestige,
                    c->mInternationalPrestige,
                    c->mTransfersCountry[0].ToStr(),
                    c->mTransfersCountry[1].ToStr(),
                    c->mYouthPlayersCountry.ToStr(),
                    c->mFifaID,
                    Quoted(c->mLowestLeagues[0].ToStr()),
                    Quoted(c->mLowestLeagues[1].ToStr()),
                    Quoted(c->mLowestLeagues[2].ToStr()),
                    Quoted(c->mLowestLeagues[3].ToStr()),
                    c->mMediaPressure.ToStr(),
                    c->mTraditionalClub,
                    c->mCantPlayInLeague,
                    c->mRichGuyControlled,
                    c->mYouthPlayersAreBasques,
                    c->mPreferredFormations[0].ToStr(),
                    c->mPreferredFormations[1].ToStr(),
                    FifamUtils::GetPlayerName(c->mCaptains[0]),
                    FifamUtils::GetPlayerName(c->mCaptains[1]),
                    FifamUtils::GetPlayerName(c->mCaptains[2]),
                    c->Unknown._2,
                    Quoted(c->Unknown._4)
                );
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mLeagueWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mCupWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mSuperCupsWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mLeagueCupWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mEuroTrophyWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mChampionsCupWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mWorldChampionshipWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteArray(c->mHistory.mWorldClubChampionshipWinYears, L' ');
                writer.WriteOne(L",");
                writer.WriteOne(Quoted(Utils::Format(L"%d:%d - %s - %d", c->mHistory.mRecordHomeWin.mScore1,
                                                                         c->mHistory.mRecordHomeWin.mScore2,
                                                                         c->mHistory.mRecordHomeWin.mOpponentName.c_str(),
                                                                         c->mHistory.mRecordHomeWin.mYear)));
                writer.WriteOne(L",");
                writer.WriteOne(Quoted(Utils::Format(L"%d:%d - %s - %d", c->mHistory.mRecordHomeDefeat.mScore1,
                                                                         c->mHistory.mRecordHomeDefeat.mScore2,
                                                                         c->mHistory.mRecordHomeDefeat.mOpponentName.c_str(),
                                                                         c->mHistory.mRecordHomeDefeat.mYear)));
                writer.WriteOne(L",");
                writer.WriteOne(Quoted(Utils::Format(L"%d:%d - %s - %d", c->mHistory.mRecordAwayWin.mScore1,
                                                                         c->mHistory.mRecordAwayWin.mScore2,
                                                                         c->mHistory.mRecordAwayWin.mOpponentName.c_str(),
                                                                         c->mHistory.mRecordAwayWin.mYear)));
                writer.WriteOne(L",");
                writer.WriteOne(Quoted(Utils::Format(L"%d:%d - %s - %d", c->mHistory.mRecordAwayDefeat.mScore1,
                                                                         c->mHistory.mRecordAwayDefeat.mScore2,
                                                                         c->mHistory.mRecordAwayDefeat.mOpponentName.c_str(),
                                                                         c->mHistory.mRecordAwayDefeat.mYear)));
                writer.WriteOne(L",");
                writer.WriteOne(Quoted(Utils::Format(L"%d - %s - %d", c->mHistory.mRecordAttendance.mAttendance,
                                                                      c->mHistory.mRecordAttendance.mOpponentName.c_str(),
                                                                      c->mHistory.mRecordAttendance.mYear)));
                writer.WriteOne(L",");
                writer.Write(c->mKit.mSpecialKitId,
                    c->mKit.mSets[FifamKit::Home].mShirt,
                    c->mKit.mSets[FifamKit::Home].mShorts,
                    c->mKit.mSets[FifamKit::Home].mSocks,
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShirtColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShirtColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShirtColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mSleevesColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mSleevesColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mSleevesColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShortsColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShortsColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShortsColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mSocksColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mSocksColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mCaptainArmbandColor.FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamShirtNumberColor::MakeFromInt(c->mKit.mSets[FifamKit::Home].mShirtNumberColor.FindIndexInTable(FifamKit::mShirtNumberColorTable)).ToStr(),
                    c->mKit.mSets[FifamKit::Home].mBadgePosition.ToStr(),
                    c->mKit.mSets[FifamKit::Away].mShirt,
                    c->mKit.mSets[FifamKit::Away].mShorts,
                    c->mKit.mSets[FifamKit::Away].mSocks,
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShirtColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShirtColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShirtColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mSleevesColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mSleevesColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mSleevesColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShortsColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShortsColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShortsColors[2].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mSocksColors[0].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mSocksColors[1].FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamKitColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mCaptainArmbandColor.FindIndexInTable(FifamKit::mKitColorTable)).ToStr(),
                    FifamShirtNumberColor::MakeFromInt(c->mKit.mSets[FifamKit::Away].mShirtNumberColor.FindIndexInTable(FifamKit::mShirtNumberColorTable)).ToStr(),
                    c->mKit.mSets[FifamKit::Away].mBadgePosition.ToStr()
                );
                writer.WriteNewLine();
            };
            for (auto country : db->mCountries) {
                WriteClub(writer, country, &country->mNationalTeam);
                for (auto c : country->mClubs)
                    WriteClub(writer, country, c);
            }
        }
    }
};
