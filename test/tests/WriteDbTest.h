#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        FifamWriter writer(Utils::Format(L"country_misc_%02d.csv", Game::id()), Game::id(), Game::year(), Game::vernum(), Game::unicode());
        if (writer.Available()) {
            writer.Write(
                L"Id",
                L"Name",
                L"OriginalLeagueSystem",
                L"FA_Name",
                L"Language_0",
                L"Language_1",
                L"Language_2",
                L"Language_3",
                L"LanguageFrequency_0",
                L"LanguageFrequency_1",
                L"LanguageFrequency_2",
                L"LanguageFrequency_3",
                L"LeagueLevelWithReserveTeams",
                L"Unknown_1",
                L"FA_PresidentFirstName",
                L"FA_PresidentLastName",
                L"FA_PresidentBirthDate",
                L"Unknown_2",
                L"Unknown_3",
                L"Unknown_4",
                L"Unknown_5",
                L"Unknown_6",
                L"Unknown_7",
                L"YellowCardsLeagueRule",
                L"YellowCardsLeagueAfterYelRed",
                L"YellowCardsCupRule",
                L"YellowCardsCupAfterYelRed",
                L"Unknown_8",
                L"Unknown_9",
                L"RedCardBanFixedDuration",
                L"RedCardRule",
                L"YellowCardsBanForAllComp",
                L"SecondYellowCardBanForAllComp",
                L"RedCardsBanForAllComp",
                L"SecondYellowCardLeagueState_0",
                L"SecondYellowCardLeagueState_1",
                L"SecondYellowCardLeagueState_2",
                L"SecondYellowCardLeagueState_3",
                L"SecondYellowCardLeagueState_4"
            );
            writer.Write(L",");
            for (UInt i = 0; i < 16; i++) {
                writer.Write(
                    Utils::Format(L"LeagueLevels_%u_EqualPointsSorting", i),
                    Utils::Format(L"LeagueLevels_%u_mNumNonEUPlayersAllowed", i),
                    Utils::Format(L"LeagueLevels_%u_mNumRelegatedTeams", i),
                    Utils::Format(L"LeagueLevels_%u_mRating", i)
                );
                writer.Write(L",");
            }


            writer.Write(
                L"CupSystemType",
                L"CupHomeAdvantageForTeamsInLowerLeagues",
                L"CupCanPlayersBeCupTied",
                L"CupFinalStadium",
                L"SuperCupType",
                L"LeagueCupSystemType_0",
                L"LeagueCupSystemType_1",
                L"LeagueCupSystemType_2",
                L"LeagueCupSystemType_3",
                L"YearsForNaturalization",
                L"Unknown_10_0",
                L"Unknown_10_1",
                L"Unknown_11",
                L"Unknown_12",
                L"Unknown_13",
                L"Unknown_14",
                L"Flags",
                L"Unknown_15",
                L"AmateurRule",
                L"Climate",
                L"AssessmentInfo_0",
                L"AssessmentInfo_1",
                L"AssessmentInfo_2",
                L"AssessmentInfo_3",
                L"AssessmentInfo_4",
                L"AssessmentInfo_5",
                L"ProbabilityForOverseasTransfers",
                L"Unknown_16",
                L"Unknown_17",
                L"AveragePlayerRating",
                L"Unknown_18",
                L"AppearanceHairColor",
                L"AppearanceHairStyle_0",
                L"AppearanceHairStyle_1",
                L"AppearanceBeard",
                L"AppearanceFaceType_0",
                L"AppearanceFaceType_1",
                L"AppearanceFaceType_2",
                L"AppearanceFaceType_3",
                L"AverageHeight",
                L"AverageWeight",
                L"AppearanceData_size",
                L"AppearanceData_data",
                L"PreferredTransfersTerritory",
                L"FifaRanking",
                L"Unknown_19",
                L"NumContinentalChampions",
                L"NumWorldCups",
                L"NumContinentalRunnersUp",
                L"NumWorldCupRunnersUp",
                L"Notes"
            );

            writer.WriteNewLine();
            for (auto c : db->mCountries) {
                writer.Write(
                    c->mId,
                    Tr(c->mName),
                    c->mOriginalLeagueSystem,
                    c->mFA_Name,
                    c->mLanguages[0].ToStr(),
                    c->mLanguages[1].ToStr(),
                    c->mLanguages[2].ToStr(),
                    c->mLanguages[3].ToStr(),
                    c->mLanguageFrequency[0],
                    c->mLanguageFrequency[1],
                    c->mLanguageFrequency[2],
                    c->mLanguageFrequency[3],
                    c->mLeagueLevelWithReserveTeams,
                    c->Unknown._1,
                    c->mFA_PresidentFirstName,
                    c->mFA_PresidentLastName,
                    c->mFA_PresidentBirthDate.ToString(),
                    c->Unknown._2,
                    c->Unknown._3,
                    c->Unknown._4,
                    c->Unknown._5,
                    c->Unknown._6,
                    c->Unknown._7,
                    c->mYellowCardsLeagueRule.ToStr(),
                    c->mYellowCardsLeagueAfterYelRed,
                    c->mYellowCardsCupRule.ToStr(),
                    c->mYellowCardsCupAfterYelRed,
                    c->Unknown._8,
                    c->Unknown._9,
                    c->mRedCardBanFixedDuration,
                    c->mRedCardRule.ToStr(),
                    c->mYellowCardsBanForAllComp,
                    c->mSecondYellowCardBanForAllComp,
                    c->mRedCardsBanForAllComp,
                    c->mSecondYellowCardLeagueState[0],
                    c->mSecondYellowCardLeagueState[1],
                    c->mSecondYellowCardLeagueState[2],
                    c->mSecondYellowCardLeagueState[3],
                    c->mSecondYellowCardLeagueState[4]
                );
                writer.Write(L",");
                for (UInt i = 0; i < 16; i++) {
                    writer.Write(
                        c->mLeagueLevels[i].mEqualPointsSorting.ToStr(),
                        c->mLeagueLevels[i].mNumNonEUPlayersAllowed,
                        c->mLeagueLevels[i].mNumRelegatedTeams,
                        c->mLeagueLevels[i].mRating
                    );
                    writer.Write(L",");
                }
                writer.Write(
                    c->mCupSystemType.ToStr(),
                    c->mCupHomeAdvantageForTeamsInLowerLeagues,
                    c->mCupCanPlayersBeCupTied,
                    c->mCupFinalStadium,
                    c->mSuperCupType.ToStr(),
                    c->mLeagueCupSystemType[0].ToStr(),
                    c->mLeagueCupSystemType[1].ToStr(),
                    c->mLeagueCupSystemType[2].ToStr(),
                    c->mLeagueCupSystemType[3].ToStr(),
                    c->mYearsForNaturalization,
                    c->Unknown._10[0],
                    c->Unknown._10[1],
                    c->Unknown._11,
                    c->Unknown._12,
                    c->Unknown._13,
                    c->Unknown._14,
                    c->mFlags.ToInt(),
                    c->Unknown._15,
                    c->mAmateurRule.ToStr(),
                    c->mClimate.ToStr(),
                    c->mAssessmentInfo[0],
                    c->mAssessmentInfo[1],
                    c->mAssessmentInfo[2],
                    c->mAssessmentInfo[3],
                    c->mAssessmentInfo[4],
                    c->mAssessmentInfo[5],
                    c->mProbabilityForOverseasTransfers,
                    c->Unknown._16,
                    c->Unknown._17,
                    c->mAveragePlayerRating,
                    c->Unknown._18,
                    c->mAppearanceHairColor,
                    c->mAppearanceHairStyle[0],
                    c->mAppearanceHairStyle[1],
                    c->mAppearanceBeard,
                    c->mAppearanceFaceType[0],
                    c->mAppearanceFaceType[1],
                    c->mAppearanceFaceType[2],
                    c->mAppearanceFaceType[3],
                    c->mAverageHeight,
                    c->mAverageWeight,
                    c->mAppearanceData.size()
                );
                writer.Write(L",");
                for (UInt i = 0; i < c->mAppearanceData.size(); i++) {
                    writer.Write(L"[");
                    writer.Write(c->mAppearanceData[i].mAppearanceType);
                    writer.Write(L":");
                    writer.Write(c->mAppearanceData[i].mDistributionProbability);
                    writer.Write(L"]");
                }
                writer.Write(L",");
                writer.Write(
                    c->mPreferredTransfersTerritory.ToStr(),
                    c->mFifaRanking,
                    c->Unknown._19,
                    c->mNumContinentalChampions,
                    c->mNumWorldCups,
                    c->mNumContinentalRunnersUp,
                    c->mNumWorldCupRunnersUp,
                    c->mNotes
                );
                writer.WriteNewLine();
            }
        }


        //db->Write(Game::id(), 0x2007, 0xC, L"fm_test\\test_db");
    }
};
