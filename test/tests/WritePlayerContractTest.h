#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WritePlayerContractTest {
public:
    WritePlayerContractTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        FifamWriter writer(Utils::Format(L"player_contract_%02d.csv", Game::id()), Game::id(), Game::year(), Game::vernum(), Game::unicode());
        if (writer.Available()) {
            writer.WriteLine(
                L"Country",
                L"Club",
                L"PlayerId",
                L"PlayerName",
                L"MatchesInternationalBonus",
                L"SquadRole",
                L"SalaryIncrease",
                L"SalaryDecreaseOnRelegation",
                L"ClauseForeignClub_Enabled",
                L"ClauseForeignClub_Percent",
                L"ClauseHigherLeague_Enabled",
                L"ClauseHigherLeague_Percent",
                L"ClauseRelegation_Enabled",
                L"ClauseRelegation_Percent",
                L"ClauseNoInternational_Enabled",
                L"ClauseNoInternational_Percent",
                L"ClauseNoPromotion_Enabled",
                L"ClauseNoPromotion_Percent",
                L"AutoExtend",
                L"PromisedRoleInTeam",
                L"PromisedFuture",
                L"MatchesAppearanceBonus",
                L"SellOnPercentage",
                L"OptionClub",
                L"OptionPlayer",
                L"HighestPaidPlayer",
                L"ExtendAvoidRelegation",
                L"JobOption",
                L"TransferSigningFee",
                L"BasicSalary",
                L"MatchBonus",
                L"AppearanceBonus",
                L"NoRelegationBonus",
                L"PromotionBonus",
                L"InternationalPremiereBonus",
                L"GoalsCleanSheetBonus",
                L"FixTransferFee",
                L"SellOnFee",
                L"InternationalBonus",
                L"Joined",
                L"ValidUntil",
                L"BuyBackClauseEnabled",
                L"BuyBackClauseCost",
                L"BuyBackClauseClub",
                L"BuyBackClauseValidUntil",
                L"Unknown_1"
            );
            for (auto country : db->mCountries) {
                if (country) {
                    for (auto club : country->mClubs) {
                        for (auto player : club->mPlayers) {
                            writer.WriteLine(
                                Quoted(FifamTr(country->mName)),
                                Quoted(FifamTr(club->mName)),
                                player->mID,
                                Quoted(player->GetName()),
                                player->mContract.mMatchesInternationalBonus,
                                player->mContract.mSquadRole.ToStr(),
                                player->mContract.mSalaryIncrease,
                                player->mContract.mSalaryDecreaseOnRelegation,
                                player->mContract.mClauseForeignClub.mEnabled,
                                player->mContract.mClauseForeignClub.mPercent,
                                player->mContract.mClauseHigherLeague.mEnabled,
                                player->mContract.mClauseHigherLeague.mPercent,
                                player->mContract.mClauseRelegation.mEnabled,
                                player->mContract.mClauseRelegation.mPercent,
                                player->mContract.mClauseNoInternational.mEnabled,
                                player->mContract.mClauseNoInternational.mPercent,
                                player->mContract.mClauseNoPromotion.mEnabled,
                                player->mContract.mClauseNoPromotion.mPercent,
                                player->mContract.mAutoExtend,
                                player->mContract.mPromisedRoleInTeam,
                                player->mContract.mPromisedFuture,
                                player->mContract.mMatchesAppearanceBonus,
                                player->mContract.mSellOnPercentage,
                                player->mContract.mOptionClub,
                                player->mContract.mOptionPlayer,
                                player->mContract.mHighestPaidPlayer,
                                player->mContract.mExtendAvoidRelegation,
                                player->mContract.mJobOption,
                                player->mContract.mSigningFee,
                                player->mContract.mBasicSalary,
                                player->mContract.mMatchBonus,
                                player->mContract.mAppearanceBonus,
                                player->mContract.mNoRelegationBonus,
                                player->mContract.mPromotionBonus,
                                player->mContract.mInternationalPremiereBonus,
                                player->mContract.mGoalsCleanSheetBonus,
                                player->mContract.mFixTransferFee,
                                player->mContract.mSellOnFee,
                                player->mContract.mInternationalBonus,
                                player->mContract.mJoined.ToString(),
                                player->mContract.mValidUntil.ToString(),
                                player->mContract.mBuyBackClauseEnabled,
                                player->mContract.mBuyBackClauseCost,
                                Quoted(player->mContract.mBuyBackClauseClub.GetTeamName()),
                                player->mContract.mBuyBackClauseValidUntil.ToString(),
                                player->mContract.Unknown._1
                            );
                        }
                    }
                }
            }
        }
    }
};
