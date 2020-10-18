#include "Converter.h"
#include "ConverterUtil.h"
#include "FifamNames.h"
#include "FifamCompPool.h"

Bool Converter::ProcessScriptWithSpecialFormat(FifamCountry *country, Vector<FifamCompLeague *> &leagues,
    Vector<FifamCompCup *> &cups, Pair<FifamCompLeague *, FifamCompLeague *> &split, Vector<PlayOffInfo *> &playOffs)
{
    if (!mFifamDatabase || !country)
        return false;
    auto countryId = country->mId;
    auto ErrorMsg = [&](String const &message) {
        if (mErrors) {
            Error(L"Error while creating special competition format:\n" + message + L"\nin " + FifamTr(country->mName));
        }
        return false;
    };
    auto GetNextLeagueCupIndex = [&]() {
        UInt result = 9999;
        for (auto &[compId, comp] : mFifamDatabase->mCompMap) {
            if (compId.mRegion.ToInt() == countryId && comp->GetDbType() == FifamCompDbType::Cup && compId.mType == FifamCompType::LeagueCup) {
                if (result == 9999 || compId.mIndex > result)
                    result = compId.mIndex;
            }
        }
        return result == 9999 ? 0 : result + 1;
    };
    auto CreateRound = [&](UShort index, String const &name, UInt level, UInt teams, Vector<FifamCompID> const &predecessors = {},
        Vector<FifamCompID> const &successors = {}, Vector<FifamAbstractInstruction *> const &instructions = {}, Array<UInt, 4> const &bonuses = {},
        UInt legs = 2, UInt subs = 3)
    {
        FifamCompID compRelID = FifamCompID(country->mId, FifamCompType::Relegation, index);
        FifamCompRound *rel = mFifamDatabase->CreateCompetition(FifamCompDbType::Round, compRelID, name)->AsRound();
        rel->mCompetitionLevel = level;
        rel->mNumSubsAllowed = subs;
        rel->mNumTeams = teams;
        rel->mRoundType = FifamRoundID::Final;
        if (legs == 2) {
            rel->m1stLegFlags.Set(FifamBeg::_2ndLeg, true);
            rel->m2ndLegFlags.Set(FifamBeg::WithExtraTime, true);
            rel->m2ndLegFlags.Set(FifamBeg::WithPenalty, true);
            rel->m2ndLegFlags.Set(FifamBeg::_2ndLeg, true);
        }
        else {
            rel->m1stLegFlags.Set(FifamBeg::WithExtraTime, true);
            rel->m1stLegFlags.Set(FifamBeg::WithPenalty, true);
            rel->m1stLegFlags.Set(FifamBeg::_1stLeg, true);
            rel->m1stLegFlags.Set(FifamBeg::End, true);
        }
        for (auto const &i : predecessors) {
            if (!Utils::Contains(rel->mPredecessors, i))
                rel->mPredecessors.push_back(i);
            auto c = mFifamDatabase->GetCompetition(i);
            if (c && !Utils::Contains(c->mSuccessors, rel->mID))
                c->mSuccessors.push_back(rel->mID);
        }
        for (auto const &i : successors) {
            if (!Utils::Contains(rel->mSuccessors, i))
                rel->mSuccessors.push_back(i);
            auto c = mFifamDatabase->GetCompetition(i);
            if (c && !Utils::Contains(c->mPredecessors, rel->mID))
                c->mPredecessors.push_back(rel->mID);
        }
        for (auto const &i : instructions)
            rel->mInstructions.PushBack(i);
        rel->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
        rel->mBonuses = bonuses;
        return rel;
    };
    auto League = [&](UShort index) { return FifamCompID(countryId, FifamCompType::League, index); };
    auto Pool = [&](UShort index) { return FifamCompID(countryId, FifamCompType::Pool, index); };
    auto Relegation = [&](UShort index) { return FifamCompID(countryId, FifamCompType::Relegation, index); };
    auto DivInfo = [&](FifamCompLeague *league) {
        auto div = league->GetProperty<DivisionInfo *>(L"divInfo", nullptr);
        if (!div) {
            static DivisionInfo dummy;
            ErrorMsg(L"DivInfo is not set for the league (" + league->mID.ToStr() + L")");
            return &dummy;
        }
        return div;
    };
    if (countryId == FifamCompRegion::Germany) {
        UInt nextCupIndex = GetNextLeagueCupIndex();
        if (nextCupIndex > 1)
            ErrorMsg(L"Not enough free league cups (next free index is " + Utils::Format(L"%d", nextCupIndex) + L")");
        else {
            FifamCompID youthCupId = FifamCompID(countryId, FifamCompType::LeagueCup, nextCupIndex);
            FifamCompCup *youthCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, youthCupId, L"DFB-Junioren-Vereinspokal")->AsCup();
            youthCup->mRounds.resize(5);
            youthCup->mNumTeams = 21;
            youthCup->mRounds[0].mRoundID = FifamRoundID::_1;
            youthCup->mRounds[0].mTeamsRound = 10;
            youthCup->mRounds[0].mNewTeamsRound = 10;
            youthCup->mRounds[0].mStartBeg = 0;
            youthCup->mRounds[0].mEndBeg = 5;
            youthCup->mRounds[1].mRoundID = FifamRoundID::Last16;
            youthCup->mRounds[1].mTeamsRound = 16;
            youthCup->mRounds[1].mNewTeamsRound = 11;
            youthCup->mRounds[1].mStartBeg = 5;
            youthCup->mRounds[1].mEndBeg = 13;
            youthCup->mRounds[2].mRoundID = FifamRoundID::Quarterfinal;
            youthCup->mRounds[2].mTeamsRound = 8;
            youthCup->mRounds[2].mStartBeg = 13;
            youthCup->mRounds[2].mEndBeg = 17;
            youthCup->mRounds[3].mRoundID = FifamRoundID::Semifinal;
            youthCup->mRounds[3].mTeamsRound = 4;
            youthCup->mRounds[3].mStartBeg = 17;
            youthCup->mRounds[3].mEndBeg = 19;
            youthCup->mRounds[4].mRoundID = FifamRoundID::Final;
            youthCup->mRounds[4].mTeamsRound = 2;
            youthCup->mRounds[4].mStartBeg = 19;
            youthCup->mRounds[4].mEndBeg = 20;
            youthCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(FifamCompID(countryId, FifamCompType::League, 0), 1, 18));
            youthCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(FifamCompID(countryId, FifamCompType::League, 1), 1, 3));
            youthCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
            youthCup->mInstructions.PushBack(new FifamInstruction::CHANGE_TEAM_TYPES(FifamClubTeamType::YouthA));
            youthCup->SetProperty<UChar>(L"min_level", 255);
        }

        FifamCompLeague *bundesliga2 = mFifamDatabase->GetCompetition(League(1))->AsLeague();
        if (!bundesliga2)
            return ErrorMsg(L"Bundesliga 2 is not available");
        FifamCompLeague *liga3 = mFifamDatabase->GetCompetition(League(2))->AsLeague();
        if (!liga3)
            return ErrorMsg(L"Liga 3 is not available");
        Array<FifamCompLeague *, 5> regionalliga;
        enum eRegionalliga { Nord = 0, Nordost = 1, West = 2, Sudwest = 3, Bayern = 4 };
        for (UInt i = 0; i < std::size(regionalliga); i++) {
            regionalliga[i] = mFifamDatabase->GetCompetition(League(3 + i))->AsLeague();
            if (!regionalliga[i])
                return ErrorMsg(L"Regionalliga is not available");
        }
        FifamCompRound *prom3 = mFifamDatabase->GetCompetition(Relegation(2))->AsRound();
        if (!prom3)
            return ErrorMsg(L"Promotion to Liga 3 is not available");
        FifamCompPool *liga3pool = mFifamDatabase->GetCompetition(Pool(2))->AsPool();
        if (!liga3pool)
            return ErrorMsg(L"Liga 3 pool is not available");
        FifamCompPool *regionalligaPool = mFifamDatabase->GetCompetition(Pool(3))->AsPool();
        if (!regionalligaPool)
            return ErrorMsg(L"Regionalliga pool is not available");

        liga3pool->mInstructions.Clear();
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(1)));
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(1), bundesliga2->mNumTeams - DivInfo(bundesliga2)->mRelegated + 1, DivInfo(bundesliga2)->mRelegated));
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(2), DivInfo(liga3)->mPromoted + DivInfo(liga3)->mTotalTeamsPromotionPlayoff + 1, liga3->mNumTeams - (DivInfo(liga3)->mPromoted + DivInfo(liga3)->mTotalTeamsPromotionPlayoff + DivInfo(liga3)->mRelegated + DivInfo(liga3)->mTotalTeamsRelegationPlayoff)));
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(6), 1, 1));
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_CC_SPARE()); // get directly promoted 3 teams from 3 leagues
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(2)));
        liga3pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());

        // promotion to liga 3 round

        prom3->mName[FifamTranslation::German] = L"Aufstieg zur 3. Liga";
        prom3->mPredecessors.clear();
        for (UInt i = 0; i < std::size(regionalliga); i++) {
            prom3->mPredecessors.push_back(League(3 + i));
            if (!Utils::Contains(regionalliga[i]->mSuccessors, Relegation(2)))
                regionalliga[i]->mSuccessors.push_back(Relegation(2));
        }
        prom3->mInstructions.Clear();
        prom3->mInstructions.PushBack(new FifamInstruction::GET_CC_SPARE());
        prom3->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
    }
    /*else if (countryId == FifamCompRegion::Spain) {
        FifamCompLeague *laliga = mFifamDatabase->GetCompetition(League(0))->AsLeague();
        if (!laliga)
            return ErrorMsg(L"LaLiga is not available");
        FifamCompLeague *segunda = mFifamDatabase->GetCompetition(League(1))->AsLeague();
        if (!segunda)
            return ErrorMsg(L"Segunda is not available");
        Array<FifamCompLeague *, 4> segundaB = {};
        for (UInt i = 0; i < std::size(segundaB); i++) {
            segundaB[i] = mFifamDatabase->GetCompetition(League(2 + i))->AsLeague();
            if (!segundaB[i])
                return ErrorMsg(L"Segunda B is not available");
        }
        Array<FifamCompLeague *, 18> tercera = {};
        for (UInt i = 0; i < std::size(tercera); i++) {
            tercera[i] = mFifamDatabase->GetCompetition(League(6 + i))->AsLeague();
            if (!tercera[i])
                return ErrorMsg(L"Tercera is not available");
        }
        FifamCompPool *segundaPool = mFifamDatabase->GetCompetition(Pool(1))->AsPool();
        if (!segundaPool)
            return ErrorMsg(L"Segunda pool is not availablee");
        FifamCompPool *segundaBPool = mFifamDatabase->GetCompetition(Pool(2))->AsPool();
        if (!segundaBPool)
            return ErrorMsg(L"Segunda B pool is not availablee");
        FifamCompPool *terceraPool = mFifamDatabase->GetCompetition(Pool(3))->AsPool();
        if (!terceraPool)
            return ErrorMsg(L"Tercera pool is not availablee");

        enum { LEVEL_3 = 2, LEVEL_4 };
        Vector<FifamCompID> level3leagues;
        for (UInt i = 0; i < 4; i++)
            level3leagues.push_back(League(i + 2));

        // level 2 > level 3
        auto _2bchamp = CreateRound(2, L"2ªB Group Winners Play-Off", LEVEL_3, 4,
            level3leagues,
            { Relegation(3), Relegation(7) },
            {
                new FifamInstruction::GET_TAB_X_TO_Y(League(5), 1, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(4), 1, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(2), 1, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(3), 1, 1)
            });
        auto _2bfinal = CreateRound(3, L"2ª División B Final", LEVEL_3, 2,
            { Relegation(2) },
            {},
            {
                new FifamInstruction::GET_WINNER(Relegation(2))
            });
        auto _2bnonchamp1 = CreateRound(4, L"2ªB Non-champions Play-Off 1", LEVEL_3, 4,
            level3leagues,
            { Relegation(7) },
            {
                new FifamInstruction::GET_TAB_X_TO_Y(League(4), 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(3), 3, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(5), 2, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(2), 3, 1)
            });
        auto _2bnonchamp2 = CreateRound(5, L"2ªB Non-champions Play-Off 2", LEVEL_3, 4,
            level3leagues,
            { Relegation(7) },
            {
                new FifamInstruction::GET_TAB_X_TO_Y(League(3), 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(4), 3, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(2), 2, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(5), 3, 1)
            });
        auto _2bnonchamp3 = CreateRound(6, L"2ªB Non-champions Play-Off 3", LEVEL_3, 4,
            level3leagues,
            { Relegation(7) },
            {
                new FifamInstruction::GET_TAB_X_TO_Y(League(5), 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(2), 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(4), 2, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(3), 2, 1)
            });
        auto _2b_r1 = CreateRound(7, L"2ªB Play-Off R1", LEVEL_3, 8,
            {
                 Relegation(2), Relegation(4),
                 Relegation(5), Relegation(6)
            },
            { Relegation(8) },
            {
                new FifamInstruction::GET_WINNER(Relegation(4)),
                new FifamInstruction::GET_WINNER(Relegation(5)),
                new FifamInstruction::GET_WINNER(Relegation(6)),
                new FifamInstruction::GET_LOSER(Relegation(2)),
            });
        auto _2b_r2 = CreateRound(8, L"2ªB Play-Off R2", LEVEL_3, 4,
            { Relegation(7) },
            {},
            {
                new FifamInstruction::GET_WINNER(Relegation(7)),
            });

        // level 3 > level 2

        auto _2b_relegation = CreateRound(9, L"2ªB Relegation Play-Off", LEVEL_3, 4,
            level3leagues,
            {},
            {
                new FifamInstruction::GET_TAB_X_TO_Y(League(2), segundaB[0]->mNumTeams - 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(5), segundaB[3]->mNumTeams - 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(3), segundaB[1]->mNumTeams - 4, 1),
                new FifamInstruction::GET_TAB_X_TO_Y(League(4), segundaB[2]->mNumTeams - 4, 1)
            });

        // level 4 > level 3

        auto _3champ = CreateRound(10, L"3ª Group Winners Play-Off", LEVEL_4, 18,
            { Pool(16) },
            { Relegation(12) },
            {
                new FifamInstruction::GET_POOL(Pool(16), 0, 18)
            });
        auto _3nonchamp = CreateRound(11, L"3ª Non-champions Play-Off", LEVEL_4, 54,
            { Pool(17), Pool(18), Pool(19) },
            { Relegation(12) },
            {
                new FifamInstruction::GET_POOL(Pool(17), 0, 18),
                new FifamInstruction::GET_POOL(Pool(19), 0, 18),
                new FifamInstruction::GET_POOL(Pool(18), 0, 18)
            });
        auto _3_r1 = CreateRound(12, L"3ª Play-Off R1", LEVEL_4, 36,
            { Relegation(10), Relegation(11) },
            { Relegation(13) },
            {
                new FifamInstruction::GET_WINNER(Relegation(11)),
                new FifamInstruction::GET_LOSER(Relegation(10)),
            });
        auto _3_r2 = CreateRound(13, L"3ª Play-Off R2", LEVEL_4, 18,
            { Relegation(12) },
            {},
            {
                new FifamInstruction::GET_WINNER(Relegation(12))
            });

        Array<FifamCompPool *, 4> pools = {};
        for (UInt i = 0; i < std::size(pools); i++) {
            pools[i] = mFifamDatabase->CreateCompetition(FifamCompDbType::Pool, Pool(16 + i))->AsPool();
            pools[i]->mReserveTeamsAllowed = true;
            pools[i]->mSorting = FifamPoolSorting::Random;
            pools[i]->mNumTeams = 18;
            pools[i]->mSuccessors.push_back(Relegation(i == 0 ? 10 : 11));
            pools[i]->mNumSubsAllowed = 0;
            pools[i]->mCompetitionLevel = 3;
            for (UInt ins = 0; ins < 18; ins++)
                pools[i]->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(ins + 6), i + 1, 1));
        }

        // original pools

        // segunda

        segundaPool->mInstructions.Clear();
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), laliga->mNumTeams - 3 + 1, 3));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(0)));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(1)));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(1), 7, segunda->mNumTeams - (6 + 4)));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(2)));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(8)));
        segundaPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());

        // segunda b
        segundaBPool->mCompConstraints.clear();
        for (UInt i = 0; i < 2; i++)
            segundaBPool->mCompConstraints.push_back(Pool(i));
        segundaBPool->mInstructions.Clear();
        segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(1), segunda->mNumTeams - 4 + 1, 4));
        for (UInt i = 0; i < 5; i++)
            segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(4 + i)));
        for (UInt i = 0; i < 4; i++)
            segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(2 + i), 5, segundaB[i]->mNumTeams - (4 + 5)));
        segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(9)));
        segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(10)));
        segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(13)));
        segundaBPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());

        // tercera
        terceraPool->mCompConstraints.clear();
        for (UInt i = 0; i < 3; i++)
            terceraPool->mCompConstraints.push_back(Pool(i));
        terceraPool->mSuccessors.clear();
        for (UInt i = 0; i < 5; i++)
            terceraPool->mSuccessors.push_back(League(6 + i));
        terceraPool->mInstructions.Clear();
        for (UInt i = 0; i < 4; i++)
            terceraPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(2 + i), segundaB[i]->mNumTeams - 4 + 1, 4));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(9)));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(11)));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(12)));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(Relegation(13)));
        for (UInt i = 0; i < 18; i++)
            terceraPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(6 + i), 5, tercera[i]->mNumTeams - 4 - DivInfo(tercera[i])->mRelegated));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(24), 1, 1));
        terceraPool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());

        for (UInt i = 0; i < std::size(tercera); i++) {
            for (UInt j = 0; j < 4; j++)
                tercera[i]->mSuccessors.push_back(Pool(j + 16));
        }

        FifamCompCup *cup = mFifamDatabase->GetCompetition(FifamCompID(countryId, FifamCompType::FaCup, 0))->AsCup();
        if (cup) {
            cup->mInstructions.Clear();
            cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP(FifamCompID(countryId, FifamCompType::FaCup, 0)));
            cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP(FifamCompID(countryId, FifamCompType::League, 0)));
            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP(FifamCompID(countryId, FifamCompType::FaCup, 0)));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(0, 1, 24));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(1, 1, 24));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(2, 1, 7));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(3, 1, 2));
            cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER(FifamCompID(countryId, FifamCompType::FaCup, 1)));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(4, 1, 24));
            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
        }
    }*/
    else if (countryId == FifamCompRegion::United_States) {
        FifamCompPool *pool = mFifamDatabase->GetCompetition(Pool(0))->AsPool();
        if (!pool)
            return ErrorMsg(L"Pool is not available");
        FifamCompLeague *league = mFifamDatabase->GetCompetition(League(0))->AsLeague();
        if (!league)
            return ErrorMsg(L"First league is not available");
        FifamCompLeague *east = mFifamDatabase->GetCompetition(Relegation(0))->AsLeague();
        if (!east)
            return ErrorMsg(L"First relegation league is not available");
        FifamCompLeague *west = mFifamDatabase->GetCompetition(Relegation(1))->AsLeague();
        if (!west)
            return ErrorMsg(L"Second relegation league is not available");
        UInt nextCupIndex = GetNextLeagueCupIndex();
        if (nextCupIndex > 3)
            return ErrorMsg(L"Not enough free league cups (next free index is " + Utils::Format(L"%d", nextCupIndex) + L")");
        pool->mSorting = FifamPoolSorting::MapLeague;
        FifamCompID mlsCupId = FifamCompID(countryId, FifamCompType::LeagueCup, nextCupIndex);
        league->SetName(L"MLS Supporters' Shield");
        east->SetName(L"MLS Eastern Conference");
        west->SetName(L"MLS Western Conference");
        east->mSuccessors.push_back(Relegation(2));
        west->mSuccessors.push_back(Relegation(3));
        east->mBonuses = { 2000000, 170000, 0, 2000000 };
        west->mBonuses = { 2000000, 170000, 0, 2000000 };

        east->mInstructions.Clear();
        west->mInstructions.Clear();
        east->mInstructions.PushBack(new FifamInstruction::GET_POOL(Pool(0), 0, 12));
        west->mInstructions.PushBack(new FifamInstruction::GET_POOL(Pool(0), 12, 12));

        Array<Array<FifamCompRound *, 3>, 2> playOff = {};

        for (UInt i = 0; i < 2; i++) {
            String cupName = i == 0 ? L"MLS East. Conf. Play-Off" : L"MLS West. Conf. Play-Off";
            playOff[i][0] = CreateRound(i + 2, cupName + L" Round 1", 0, 6,
                { Relegation(i) },
                { Relegation(i + 4) },
                {
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 4, 1),
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 2, 1),
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 3, 1),
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 5, 1),
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 7, 1),
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 6, 1)
                }, 
                { 0, 0, 0, 500000 },
                1
            );
            playOff[i][1] = CreateRound(i + 4, cupName + L" Semi", 0, 4,
                { Relegation(i + 2) },
                { Relegation(i + 6) },
                {
                    new FifamInstruction::GET_TAB_X_TO_Y(Relegation(i), 1, 1),
                    new FifamInstruction::GET_WINNER(Relegation(i + 2))
                },
                { 0, 0, 0, 650000 },
                1
            );
            playOff[i][2] = CreateRound(i + 6, cupName + L" Final", 0, 2,
                { Relegation(i + 4) },
                { mlsCupId },
                {
                    new FifamInstruction::GET_WINNER(Relegation(i + 4))
                },
                { 0, 0, 0, 850000 },
                1
            );
            playOff[i][0]->SetProperty<UChar>(L"max_level", 0);
            playOff[i][1]->SetProperty<UChar>(L"max_level", 0);
            playOff[i][2]->SetProperty<UChar>(L"max_level", 0);
        }
        FifamCompCup *mlsCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, mlsCupId, L"MLS Cup")->AsCup();
        mlsCup->mPredecessors.push_back(Relegation(6));
        mlsCup->mPredecessors.push_back(Relegation(7));
        mlsCup->mRounds.resize(1);
        mlsCup->mNumTeams = 2;
        mlsCup->mRounds[0].mRoundID = FifamRoundID::Final;
        mlsCup->mRounds[0].mTeamsRound = 2;
        mlsCup->mRounds[0].mNewTeamsRound = 2;
        mlsCup->mRounds[0].mStartBeg = 0;
        mlsCup->mRounds[0].mEndBeg = 1;
        mlsCup->mRounds[0].mBonuses = { 0, 2000000, 0, 1000000 };
        mlsCup->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(6)));
        mlsCup->mInstructions.PushBack(new FifamInstruction::GET_WINNER(Relegation(7)));
        mlsCup->mInstructions.PushBack(new FifamInstruction::SHUFFLE_TEAMS());
        mlsCup->SetProperty<UChar>(L"max_level", 0);
        cups.push_back(mlsCup);
    }
    else if (countryId == FifamCompRegion::Russia) {
        UInt nextCupIndex = GetNextLeagueCupIndex();
        if (nextCupIndex > 1)
            return ErrorMsg(L"Not enough free league cups (next free index is " + Utils::Format(L"%d", nextCupIndex) + L")");
        FifamCompID fnlCupId = FifamCompID(countryId, FifamCompType::LeagueCup, nextCupIndex);
        FifamCompID matchPremierCupId = FifamCompID(countryId, FifamCompType::LeagueCup, nextCupIndex + 1);
        FifamCompID pariMatchCupId = FifamCompID(countryId, FifamCompType::LeagueCup, nextCupIndex + 2);
        FifamCompCup *fnlCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, fnlCupId, L"Olimp Kubok FNL")->AsCup();
        fnlCup->mRounds.resize(4);
        fnlCup->mNumTeams = 16;
        fnlCup->mRounds[0].mRoundID = FifamRoundID::Last16;
        fnlCup->mRounds[0].mTeamsRound = 16;
        fnlCup->mRounds[0].mNewTeamsRound = 16;
        fnlCup->mRounds[0].mStartBeg = 0;
        fnlCup->mRounds[0].mEndBeg = 8;
        fnlCup->mRounds[0].mBonuses = { 0, 8000, 0, 0 };
        fnlCup->mRounds[1].mRoundID = FifamRoundID::Quarterfinal;
        fnlCup->mRounds[1].mTeamsRound = 8;
        fnlCup->mRounds[1].mNewTeamsRound = 0;
        fnlCup->mRounds[1].mStartBeg = 8;
        fnlCup->mRounds[1].mEndBeg = 12;
        fnlCup->mRounds[1].mBonuses = { 0, 16000, 0, 0 };
        fnlCup->mRounds[2].mRoundID = FifamRoundID::Semifinal;
        fnlCup->mRounds[2].mTeamsRound = 4;
        fnlCup->mRounds[2].mNewTeamsRound = 0;
        fnlCup->mRounds[2].mStartBeg = 12;
        fnlCup->mRounds[2].mEndBeg = 14;
        fnlCup->mRounds[2].mBonuses = { 0, 32500, 0, 0 };
        fnlCup->mRounds[3].mRoundID = FifamRoundID::Final;
        fnlCup->mRounds[3].mTeamsRound = 2;
        fnlCup->mRounds[3].mNewTeamsRound = 0;
        fnlCup->mRounds[3].mStartBeg = 14;
        fnlCup->mRounds[3].mEndBeg = 15;
        fnlCup->mRounds[3].mBonuses = { 0, 75000, 0, 10000 };
        fnlCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(1, 1, 24));
        fnlCup->SetProperty<UChar>(L"min_level", 1);
        fnlCup->SetProperty<UChar>(L"max_level", 1);
        FifamCompCup *matchPremierCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, matchPremierCupId, L"Fonbet Kubok Match Premier")->AsCup();
        matchPremierCup->mRounds.resize(2);
        matchPremierCup->mNumTeams = 4;
        matchPremierCup->mRounds[0].mRoundID = FifamRoundID::Semifinal;
        matchPremierCup->mRounds[0].mTeamsRound = 4;
        matchPremierCup->mRounds[0].mNewTeamsRound = 4;
        matchPremierCup->mRounds[0].mStartBeg = 0;
        matchPremierCup->mRounds[0].mEndBeg = 2;
        matchPremierCup->mRounds[0].mBonuses = { 0, 0, 0, 40000 };
        matchPremierCup->mRounds[1].mRoundID = FifamRoundID::Final;
        matchPremierCup->mRounds[1].mTeamsRound = 2;
        matchPremierCup->mRounds[1].mNewTeamsRound = 0;
        matchPremierCup->mRounds[1].mStartBeg = 2;
        matchPremierCup->mRounds[1].mEndBeg = 3;
        matchPremierCup->mRounds[1].mBonuses = { 0, 30000, 0, 10000 };
        matchPremierCup->mInstructions.PushBack(new FifamInstruction::GET_INTERNATIONAL_TEAMS(FifamNation::Russia, 4));
        matchPremierCup->SetProperty<UChar>(L"min_level", 0);
        matchPremierCup->SetProperty<UChar>(L"max_level", 0);
        FifamCompCup *pariMatchCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, pariMatchCupId, L"Kubor Parimatch Premier")->AsCup();
        pariMatchCup->mRounds.resize(2);
        pariMatchCup->mNumTeams = 4;
        pariMatchCup->mRounds[0].mRoundID = FifamRoundID::Semifinal;
        pariMatchCup->mRounds[0].mTeamsRound = 4;
        pariMatchCup->mRounds[0].mNewTeamsRound = 4;
        pariMatchCup->mRounds[0].mStartBeg = 0;
        pariMatchCup->mRounds[0].mEndBeg = 2;
        pariMatchCup->mRounds[0].mBonuses = { 0, 0, 0, 20000 };
        pariMatchCup->mRounds[1].mRoundID = FifamRoundID::Final;
        pariMatchCup->mRounds[1].mTeamsRound = 2;
        pariMatchCup->mRounds[1].mNewTeamsRound = 0;
        pariMatchCup->mRounds[1].mStartBeg = 2;
        pariMatchCup->mRounds[1].mEndBeg = 3;
        pariMatchCup->mRounds[1].mBonuses = { 0, 15000, 0, 5000 };
        pariMatchCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(FifamCompID(FifamCompRegion::Russia, FifamCompType::League, 0), 1, 24));
        pariMatchCup->SetProperty<UChar>(L"min_level", 0);
        pariMatchCup->SetProperty<UChar>(L"max_level", 0);
        cups.push_back(fnlCup);
        cups.push_back(matchPremierCup);
        cups.push_back(pariMatchCup);
    }
    else if (countryId == FifamCompRegion::Australia) {
        FifamCompLeague *league = mFifamDatabase->GetCompetition(League(0))->AsLeague();
        if (!league)
            return ErrorMsg(L"First league is not available");
        UInt nextCupIndex = GetNextLeagueCupIndex();
        if (nextCupIndex != 0)
            return ErrorMsg(L"League cup 0 is already used");
        FifamCompID leagueCupId = FifamCompID(countryId, FifamCompType::LeagueCup, 0);
        league->mSuccessors.push_back(leagueCupId);
        FifamCompCup *leagueCup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, leagueCupId, L"A-League Finals series")->AsCup();
        leagueCup->mPredecessors.push_back(League(0));
        leagueCup->mRounds.resize(3);
        leagueCup->mNumTeams = 6;

        leagueCup->mRounds[0].mRoundID = FifamRoundID::_1;
        leagueCup->mRounds[0].mTeamsRound = 4;
        leagueCup->mRounds[0].mNewTeamsRound = 4;
        leagueCup->mRounds[0].mStartBeg = 0;
        leagueCup->mRounds[0].mEndBeg = 2;
        leagueCup->mRounds[0].mBonuses = { 0, 50000, 0, 20000 };
        leagueCup->mRounds[0].mFlags.Set(FifamBeg::NoShuffle, true);

        leagueCup->mRounds[1].mRoundID = FifamRoundID::Semifinal;
        leagueCup->mRounds[1].mTeamsRound = 4;
        leagueCup->mRounds[1].mNewTeamsRound = 2;
        leagueCup->mRounds[1].mStartBeg = 2;
        leagueCup->mRounds[1].mEndBeg = 4;
        leagueCup->mRounds[1].mBonuses = { 0, 100000, 0, 50000 };
        leagueCup->mRounds[1].mFlags.Set(FifamBeg::NoShuffle, true);

        leagueCup->mRounds[2].mRoundID = FifamRoundID::Final;
        leagueCup->mRounds[2].mTeamsRound = 2;
        leagueCup->mRounds[2].mNewTeamsRound = 0;
        leagueCup->mRounds[2].mStartBeg = 4;
        leagueCup->mRounds[2].mEndBeg = 5;
        leagueCup->mRounds[2].mBonuses = { 0, 200000, 0, 100000 };

        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 1, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 2, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 3, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 4, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 6, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 5, 1));
        leagueCup->SetProperty<UChar>(L"max_level", 0);
        cups.push_back(leagueCup);
    }
    else if (countryId == FifamCompRegion::New_Zealand) {
        FifamCompLeague *league = mFifamDatabase->GetCompetition(League(0))->AsLeague();
        if (!league)
            return ErrorMsg(L"League is not available");

        FifamCompID leagueCupID = FifamCompID(countryId, FifamCompType::LeagueCup, 0);

        FifamCompCup *leagueCup = mFifamDatabase->GetCompetition(leagueCupID)->AsCup();
        if (!leagueCup)
            return ErrorMsg(L"League cup is not available");

        if (leagueCup->mRounds.size() != 2)
            return ErrorMsg(L"League cup has incorrect format");

        FifamCompCup *superCup = mFifamDatabase->GetCompetition(FifamCompID(countryId, FifamCompType::SuperCup, 0))->AsCup();
        if (!superCup)
            return ErrorMsg(L"Super cup is not available");

        superCup->mInstructions.Clear();
        superCup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP(leagueCupID));
        superCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(0, 1, 24));
        
        league->mSuccessors.push_back(leagueCupID);

        leagueCup->mPredecessors.push_back(League(0));
        leagueCup->mRounds[0].mFlags.Set(FifamBeg::NoShuffle, true);
        leagueCup->mInstructions.Clear();
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 1, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 2, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 4, 1));
        leagueCup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(League(0), 3, 1));
        leagueCup->SetProperty<UChar>(L"max_level", 0);
        cups.push_back(leagueCup);
    }
    return true;
}

void Converter::ConvertLeagues(UInt gameId) {

    UInt MAX_LEAGUE_NAME_LENGTH = 63;
    UInt MAX_CUP_NAME_LENGTH = 63;
    UInt MAX_POOL_NAME_LENGTH = 58;
    UInt MAX_COMP_NAME_LENGTH = 29;

    if (gameId <= 7) {
        MAX_LEAGUE_NAME_LENGTH = 29;
        MAX_CUP_NAME_LENGTH = 29;
        MAX_POOL_NAME_LENGTH = 29;
        MAX_COMP_NAME_LENGTH = 29;
    }

    FifamWriter *leagueConfigSplit = nullptr;
    FifamWriter *leagueConfigTables = nullptr;

    if (mGenerateLeagueConfigFiles) {
        Path ucpFolder = mOutputGameFolder / L"plugins" / L"ucp";
        create_directories(ucpFolder);
        leagueConfigSplit = new FifamWriter(ucpFolder / L"league_split.csv", 14, FifamVersion());
        leagueConfigTables = new FifamWriter(ucpFolder / L"league_tables.csv", 14, FifamVersion());
        if (leagueConfigSplit->Available())
            leagueConfigSplit->WriteLine(L"CountryId,CountryName");
        if (leagueConfigTables->Available())
            leagueConfigTables->WriteLine(L"CountryId,LeagueIndex,Pro,PPO,Rel,RPO,CountryName");
    }

    for (auto &entry : mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto country = mFifamDatabase->GetCountry(nation.mConverterData.mFIFAManagerID);
        if (country) {
            ConvertNationInfo(country, &nation, gameId);
            country->SetProperty(L"foom::nation", &nation);
            CreateStaffMembersForClub(gameId, &nation, &country->mNationalTeam, true);
            country->mNationalTeam.SetProperty(L"foom::nation", &nation);

            Map<UInt, DivisionInfo *> divLeagueLevels;
            Map<UInt, Vector<DivisionInfo *>> divLeagues;
            Vector<PlayOffInfo *> vecPlayOffs;
            Map<UInt, PlayOffInfo *> playOffs;

            // find all division competitions for this country
            Int maxLevel = -1;
            Bool simulateLeague = false;
            for (DivisionInfo &divInfo : mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    if (divInfo.mPriority >= 1) {
                        simulateLeague = true;
                        if (divInfo.mType == DivisionInfo::League)
                            divLeagues[divInfo.mLevel].push_back(&divInfo);
                        else if (divInfo.mType == DivisionInfo::Level)
                            divLeagueLevels[divInfo.mLevel] = &divInfo;
                        if (divInfo.mLevel > maxLevel)
                            maxLevel = divInfo.mLevel;
                    }
                }
            }

            for (auto &po : mPlayOffs) {
                if (po.mNationID == nation.mID) {
                    vecPlayOffs.push_back(&po);
                    playOffs[po.mID] = &po;
                }
            }

            std::sort(vecPlayOffs.begin(), vecPlayOffs.end(), [](PlayOffInfo *a, PlayOffInfo *b) {
                return a->mID < b->mID;
            });

            country->SetProperty<Bool>(L"SimulateLeague", simulateLeague);

            Vector<FifamCompLeague *> createdLeagues;
            Vector<FifamCompPool *> createdPools;
            Vector<FifamCompCup *> createdCups;
            FifamCompLeague *relLeague[2] = { nullptr, nullptr };

            FifamWriter *leagueWriter = nullptr;

            const UInt MAX_LEAGUE_RATING = 71;

            if (maxLevel >= 0) {

                if (mGenerateLeaguesFiles) {
                    create_directories(L"leagues");
                    leagueWriter = new FifamWriter(L"leagues\\" + FifamTr(country->mName) + L".txt", 14, FifamVersion());
                }

                // sort leagues
                for (auto &e : divLeagues) {
                    if (e.second.size() > 1) {
                        std::sort(e.second.begin(), e.second.end(), [](DivisionInfo *a, DivisionInfo *b) {
                            return a->mOrder < b->mOrder;
                            });
                    }
                }

                // create leagues

                mNextFreeUID[country->mId - 1] = 0x4001;
                mNumTeamsInLeagueSystem[country->mId - 1] = 0;
                Int minLevelWithReserveTeams = -1;
                Bool customLeagues = false;
                Bool leagueSplit = false;

                if (!customLeagues && !mFromFifaDatabase) {
                    for (auto &leagues : divLeagues) {
                        for (DivisionInfo *lg : leagues.second) {
                            Bool leagueCustomFormat = lg->mSplit.first > 0 || lg->mTotalTeamsPromotionPlayoff > 0 || lg->mTotalTeamsRelegationPlayoff > 0;
                            if (!customLeagues && leagueCustomFormat) {
                                customLeagues = true;
                                break;
                            }
                        }
                    }
                }

                UInt leagueIndex = 0;
                for (auto &leagues : divLeagues) {
                    UInt teamCounter = 0;
                    for (DivisionInfo *lg : leagues.second) {
                        if (!nation.mConverterData.mDomesticComps.league)
                            nation.mConverterData.mDomesticComps.league = lg;
                        FifamCompID leagueID = FifamCompID(country->mId, FifamCompType::League, leagueIndex++);
                        String compName;
                        if (lg->mName.length() <= MAX_LEAGUE_NAME_LENGTH)
                            compName = lg->mName;
                        else
                            compName = FifamNames::LimitName(lg->mShortName, MAX_LEAGUE_NAME_LENGTH);
                        FifamCompLeague *league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, leagueID, compName)->AsLeague();

                        lg->mCompID = leagueID;
                        league->SetProperty<DivisionInfo *>(L"divInfo", lg);

                        if (leagueConfigTables && leagueConfigTables->Available()) {
                            if (!mGenerateSpecialScripts || country->mId != FifamNation::Spain)
                                leagueConfigTables->WriteLine(country->mId, leagueID.mIndex, lg->mPromoted, lg->mTotalTeamsPromotionPlayoff, lg->mRelegated, lg->mTotalTeamsRelegationPlayoff, FifamTr(country->mName));
                            else {
                                static Bool mSpainWritten = false;
                                if (!mSpainWritten) {
                                    leagueConfigTables->WriteLine(45, 0, 0, 0, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 1, 2, 4, 4, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 2, 0, 4, 4, 1, L"Spain");
                                    leagueConfigTables->WriteLine(45, 3, 0, 4, 4, 1, L"Spain");
                                    leagueConfigTables->WriteLine(45, 4, 0, 4, 4, 1, L"Spain");
                                    leagueConfigTables->WriteLine(45, 5, 0, 4, 4, 1, L"Spain");
                                    leagueConfigTables->WriteLine(45, 6, 0, 4, 4, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 7, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 8, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 9, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 10, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 11, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 12, 0, 4, 4, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 13, 0, 4, 4, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 14, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 15, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 16, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 17, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 18, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 19, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 20, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 21, 0, 4, 3, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 22, 0, 4, 4, 0, L"Spain");
                                    leagueConfigTables->WriteLine(45, 23, 0, 4, 3, 0, L"Spain");
                                    mSpainWritten = true;
                                }
                            }
                        }

                        // convert - old

                        foom::comp *comp = mFoomDatabase->get<foom::comp>(lg->mID);
                        if (!comp) {
                            if (mErrors) {
                                Error(L"Competition is not available\nCompetitionName: %s\nCompetitionID: %d", lg->mName.c_str(), lg->mID);
                            }
                            continue;
                        }

                        league->mCompetitionLevel = lg->mLevel;
                        league->mLeagueLevel = lg->mLevel;
                        league->mNumSubsAllowed = 3;
                        league->mNumTeams = lg->mTeams;
                        league->mNumRelegatedTeams = customLeagues ? 0 : lg->mRelegated;

                        // rounds (auto-calculation)

                        if (lg->mRounds > 0)
                            league->mNumRounds = lg->mRounds;
                        else if (league->mNumTeams >= 15)
                            league->mNumRounds = 2;
                        else if (league->mNumTeams >= 11)
                            league->mNumRounds = 3;
                        else
                            league->mNumRounds = 4;

                        league->mRoundType = FifamRoundID::Group1;
                        league->mTeams.resize(lg->mTeams);

                        if (lg->mRep <= 20) {
                            if (lg->mRep > 10) {
                                league->mTransferMarketMp = lg->mRep;
                                league->mAttendanceMp = lg->mRep;
                            }
                            else {
                                league->mTransferMarketMp = 10;
                                league->mAttendanceMp = 10;
                            }
                        }
                        else {
                            league->mTransferMarketMp = Utils::Min(200, (Int)((Float)(lg->mRep - 20) * 1.3f));
                            league->mAttendanceMp = Utils::Min(120, (Int)((Float)(lg->mRep - 20) * 0.69f));
                        }

                        comp->mConverterData.mLeague = league;
                        league->SetProperty(L"foom::comp", comp);
                        league->SetProperty(L"foom::id", lg->mID);
                        league->SetProperty(L"foom::reputation", lg->mRep);
                        league->SetProperty(L"startDate", lg->mStartDate);
                        league->SetProperty(L"endDate", lg->mEndDate);
                        league->SetProperty(L"winterBreakStart", lg->mWinterBreakStart);
                        league->SetProperty(L"winterBreakEnd", lg->mWinterBreakEnd);
                        createdLeagues.push_back(league);
                        mLeaguesSystem[country->mId - 1][league->mLeagueLevel].push_back(league);

                        league->mEqualPointsSorting = lg->mSorting;
                        if (lg->mTvBonus >= 0)
                            league->mBonuses[3] = AlignMoneyValue(lg->mTvBonus, 1'000);
                        if (lg->mWinBouns >= 0)
                            league->mBonuses[1] = AlignMoneyValue(lg->mWinBouns, 1'000);
                        if (lg->mPlaceBonus >= 0)
                            league->mBonuses[2] = AlignMoneyValue(lg->mPlaceBonus, 1'000);
                        if (lg->mAttendanceMp > 0)
                            league->mAttendanceMp = lg->mAttendanceMp;
                        if (lg->mTransfersMp > 0)
                            league->mTransferMarketMp = lg->mTransfersMp;
                        if (lg->mNumSubs > 0)
                            league->mNumSubsAllowed = lg->mNumSubs;
                        if (lg->mDomesticPlayers > 0)
                            league->mMinDomesticPlayerCount = lg->mDomesticPlayers;
                        if (lg->mU21Players > 0)
                            league->mMinU21PlayerCount = lg->mU21Players;
                        if (lg->mNonEuSigns > 0)
                            league->mMaxNumberOfNonEUSigns = lg->mNonEuSigns;

                        if (customLeagues)
                            league->mPredecessors.push_back(FifamCompID(country->mId, FifamCompType::Pool, leagues.first));

                        if (lg->mSplit.first > 0) {
                            if (!leagueSplit)
                                leagueSplit = true;

                            league->mSuccessors.push_back(FifamCompID(country->mId, FifamCompType::Relegation, 0));
                            league->mSuccessors.push_back(FifamCompID(country->mId, FifamCompType::Relegation, 1));

                            if (lg->mSplit.second <= 0 && lg->mSplit.first + lg->mSplit.second != lg->mTeams) {
                                if (mErrors) {
                                    Error(L"Incorrect league split format (%s (%d)) - total teams: %d, split: %d/%d",
                                        lg->mName.c_str(), lg->mID, lg->mTeams, lg->mSplit.first, lg->mSplit.second);
                                }
                            }
                            else if (lg->mLevel != 0) {
                                if (mErrors) {
                                    Error(L"League split is possible only on first league level (%s (%d))", lg->mName.c_str(), lg->mID);
                                }
                            }
                            else {
                                String roundNames[2] = { L" Top " + Utils::Format(L"%u", lg->mSplit.first),
                                    L" Bottom " + Utils::Format(L"%u", lg->mSplit.second) };
                                String roundNamesGer[2] = { L" Beste " + Utils::Format(L"%u", lg->mSplit.first),
                                    L" Letzte " + Utils::Format(L"%u", lg->mSplit.second) };
                                for (UInt relId = 0; relId < 2; relId++) {
                                    FifamCompID compRelID = FifamCompID(country->mId, FifamCompType::Relegation, relId);
                                    String relName;
                                    if (lg->mName.length() <= MAX_LEAGUE_NAME_LENGTH - roundNames[relId].size())
                                        relName = lg->mName + roundNames[relId];
                                    else
                                        relName = FifamNames::LimitName(lg->mShortName + roundNames[relId], MAX_LEAGUE_NAME_LENGTH);
                                    relLeague[relId] = mFifamDatabase->CreateCompetition(FifamCompDbType::League, compRelID, relName)->AsLeague();
                                    String relNameGer;
                                    if (lg->mName.length() <= MAX_LEAGUE_NAME_LENGTH - roundNamesGer[relId].size())
                                        relNameGer = lg->mName + roundNamesGer[relId];
                                    else
                                        relNameGer = FifamNames::LimitName(lg->mShortName + roundNamesGer[relId], MAX_LEAGUE_NAME_LENGTH);
                                    relLeague[relId]->mName[FifamTranslation::German] = relNameGer;
                                    relLeague[relId]->mCompetitionLevel = league->mCompetitionLevel;
                                    relLeague[relId]->mLeagueLevel = league->mLeagueLevel;
                                    relLeague[relId]->mNumSubsAllowed = league->mNumSubsAllowed;
                                    relLeague[relId]->mNumTeams = relId == 0 ? lg->mSplit.first : lg->mSplit.second;
                                    relLeague[relId]->mNumRelegatedTeams = 0;
                                    relLeague[relId]->mNumRounds = relId == 0 ? lg->mSplitRounds.first : lg->mSplitRounds.second;
                                    relLeague[relId]->mRoundType = FifamRoundID::Group1;
                                    relLeague[relId]->mTransferMarketMp = 100;
                                    relLeague[relId]->mAttendanceMp = 100;
                                    relLeague[relId]->mTakePoints = true;
                                    relLeague[relId]->mEqualPointsSorting = league->mEqualPointsSorting;
                                    String teamsOverride;
                                    for (UInt to = 0; to < relLeague[relId]->mNumTeams; to++) {
                                        if (to != 0)
                                            teamsOverride += L",";
                                        teamsOverride += Utils::Format(L"%02X%04X", country->mId, to + 1);
                                    }
                                    relLeague[relId]->SetProperty<String>(L"teams_override", teamsOverride);
                                    relLeague[relId]->mPredecessors.push_back(FifamCompID(country->mId, FifamCompType::League, 0));
                                    relLeague[relId]->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                        FifamCompID(country->mId, FifamCompType::League, 0), relId == 1 ? lg->mSplit.first + 1 : 1, relLeague[relId]->mNumTeams));
                                    relLeague[relId]->GenerateFixtures();
                                }

                                if (leagueConfigSplit && leagueConfigSplit->Available())
                                    leagueConfigSplit->WriteLine(country->mId, FifamTr(country->mName));
                            }
                        }

                        if (lg->mTotalTeamsPromotionPlayoff > 0) {
                            if (league->mCompetitionLevel == 0) {
                                if (mErrors) {
                                    Error(L"League promotion play-off is not possible at first league level (%s (%d))", lg->mName.c_str(), lg->mID);
                                }
                            }
                            else {
                                if ((lg->mPromoted + lg->mRelegated + lg->mTotalTeamsPromotionPlayoff + lg->mTotalTeamsRelegationPlayoff) > league->mNumTeams) {
                                    if (mErrors) {
                                        Error(L"Not enough teams in league for promotion/relegation play-off (%s (%d))", lg->mName.c_str(), lg->mID);
                                    }
                                }
                                else {
                                    auto it = playOffs.find(lg->mPromotionID);
                                    if (it == playOffs.end()) {
                                        if (mErrors) {
                                            Error(L"Incorrect league promotion play-off ID (%s (%d)) - %d", lg->mName.c_str(), lg->mID, lg->mPromotionID);
                                        }
                                    }
                                    else {
                                        auto &po = (*it).second;
                                        Bool promotionError = false;
                                        if (po->mIsLeague) {
                                            if (lg->mPromotionPlayoff.size() != 1) {
                                                if (mErrors) {
                                                    Error(L"League promotion play-off should have only 1 play-off group (%s (%d))", lg->mName.c_str(), lg->mID);
                                                }
                                                promotionError = true;
                                            }
                                            else {
                                                lg->mPromotionInfo = po;
                                                UChar tablePos = lg->mPromoted + 1;
                                                for (UInt tpo = 0; tpo < lg->mPromotionPlayoff[0]; tpo++) {
                                                    PlayOffInfo::TeamEntry entry;
                                                    entry.mLevel = league->mCompetitionLevel;
                                                    entry.mPositionIndex = tpo;
                                                    entry.mLeagueIndex = league->mID.mIndex;
                                                    entry.mTablePosition = tablePos++;
                                                    entry.mLeague = league;
                                                    po->mLeague.mTeamEntries.push_back(entry);
                                                }
                                            }
                                        }
                                        else {
                                            if (po->mRounds.size() < lg->mPromotionPlayoff.size()) {
                                                if (mErrors) {
                                                    Error(L"Too many teams in promotion play-off (%s (%d))", lg->mName.c_str(), lg->mID);
                                                }
                                                promotionError = true;
                                            }
                                            else {
                                                lg->mPromotionInfo = po;
                                                UChar tablePos = lg->mPromoted + 1;
                                                for (UInt rpo = 0; rpo < lg->mPromotionPlayoff.size(); rpo++) {
                                                    for (UInt tpo = 0; tpo < lg->mPromotionPlayoff[rpo]; tpo++) {
                                                        PlayOffInfo::TeamEntry entry;
                                                        entry.mLevel = league->mCompetitionLevel;
                                                        entry.mPositionIndex = tpo;
                                                        entry.mLeagueIndex = league->mID.mIndex;
                                                        entry.mTablePosition = tablePos++;
                                                        entry.mLeague = league;
                                                        po->mRounds[lg->mPromotionPlayoff.size() - 1 - rpo].mTeamEntries.push_back(entry);
                                                    }
                                                }
                                            }
                                        }
                                        if (!promotionError) {
                                            if (po->mMinLeagueLevel == -1 || po->mMinLeagueLevel > league->mCompetitionLevel)
                                                po->mMinLeagueLevel = league->mCompetitionLevel;
                                            if (po->mMaxLeagueLevel == -1 || po->mMaxLeagueLevel < league->mCompetitionLevel)
                                                po->mMaxLeagueLevel = league->mCompetitionLevel;
                                            if (po->mPromotionLevel == -1)
                                                po->mPromotionLevel = league->mCompetitionLevel;
                                            else if (po->mPromotionLevel != league->mCompetitionLevel) {
                                                if (mErrors) {
                                                    Error(L"League promotion play-off is present more than one level (%s (%d))", lg->mName.c_str(), lg->mID);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if (lg->mTotalTeamsRelegationPlayoff > 0) {
                            FifamCompLeague *targetLeague = (league->mCompetitionLevel == 0 && relLeague[1]) ? relLeague[1] : league;
                            UInt totalTeamsInPromotionRelegation = lg->mPromoted + lg->mRelegated + lg->mTotalTeamsPromotionPlayoff + lg->mTotalTeamsRelegationPlayoff;
                            if (totalTeamsInPromotionRelegation > targetLeague->mNumTeams) {
                                if (mErrors) {
                                    Error(L"Not enough teams in league for promotion/relegation play-off - %d/%d (in %s (%d))",
                                        totalTeamsInPromotionRelegation, targetLeague->mNumTeams, lg->mName.c_str(), lg->mID);
                                }
                            }
                            else {
                                auto it = playOffs.find(lg->mRelegationID);
                                if (it == playOffs.end()) {
                                    if (mErrors) {
                                        Error(L"Incorrect league relegation play-off ID (%s (%d)) - %d", lg->mName.c_str(), lg->mID, lg->mRelegationID);
                                    }
                                }
                                else {
                                    auto &po = (*it).second;
                                    Bool relegationError = false;
                                    if (po->mIsLeague) {
                                        if (lg->mRelegationPlayoff.size() != 1) {
                                            if (mErrors) {
                                                Error(L"League relegation play-off should have only 1 play-off group (%s (%d))", lg->mName.c_str(), lg->mID);
                                            }
                                            relegationError = true;
                                        }
                                        else {
                                            lg->mRelegationInfo = po;
                                            UChar tablePos = targetLeague->mNumTeams + 1 - lg->mRelegated - lg->mTotalTeamsRelegationPlayoff;
                                            for (UInt tpo = 0; tpo < lg->mRelegationPlayoff[0]; tpo++) {
                                                PlayOffInfo::TeamEntry entry;
                                                entry.mLevel = league->mCompetitionLevel;
                                                entry.mPositionIndex = tpo;
                                                entry.mLeagueIndex = league->mID.mIndex;
                                                entry.mTablePosition = tablePos++;
                                                entry.mLeague = targetLeague;
                                                po->mLeague.mTeamEntries.push_back(entry);
                                            }
                                        }
                                    }
                                    else {
                                        if (po->mRounds.size() < lg->mRelegationPlayoff.size()) {
                                            if (mErrors) {
                                                Error(L"Too many teams in relegation play-off (%s (%d))", lg->mName.c_str(), lg->mID);
                                            }
                                            relegationError = true;
                                        }
                                        else {
                                            lg->mRelegationInfo = po;
                                            UChar tablePos = targetLeague->mNumTeams + 1 - lg->mRelegated - lg->mTotalTeamsRelegationPlayoff;
                                            for (UInt rpo = 0; rpo < lg->mRelegationPlayoff.size(); rpo++) {
                                                for (UInt tpo = 0; tpo < lg->mRelegationPlayoff[rpo]; tpo++) {
                                                    PlayOffInfo::TeamEntry entry;
                                                    entry.mLevel = league->mCompetitionLevel;
                                                    entry.mPositionIndex = tpo;
                                                    entry.mLeagueIndex = league->mID.mIndex;
                                                    entry.mTablePosition = tablePos++;
                                                    entry.mLeague = targetLeague;
                                                    po->mRounds[lg->mRelegationPlayoff.size() - 1 - rpo].mTeamEntries.push_back(entry);
                                                }
                                            }
                                        }
                                    }
                                    if (!relegationError) {
                                        if (po->mMinLeagueLevel == -1 || po->mMinLeagueLevel > league->mCompetitionLevel)
                                            po->mMinLeagueLevel = league->mCompetitionLevel;
                                        if (po->mMaxLeagueLevel == -1 || po->mMaxLeagueLevel < league->mCompetitionLevel)
                                            po->mMaxLeagueLevel = league->mCompetitionLevel;
                                        if (po->mRelegationLevel == -1)
                                            po->mRelegationLevel = league->mCompetitionLevel;
                                        else if (po->mRelegationLevel != league->mCompetitionLevel) {
                                            if (mErrors) {
                                                Error(L"League relegation play-off is present more than on one level (%s (%d))", lg->mName.c_str(), lg->mID);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        league->mInstructions.PushBack(new FifamInstruction::GET_POOL(
                            FifamCompID(country->mId, FifamCompType::Pool, leagues.first), teamCounter, lg->mTeams));

                        // add teams
                        Bool hasReserveTeams = lg->mReserveTeamsAllowed == 1;
                        UInt numLeaguePromotedTeams = 0;
                        UInt numLeagueRelegatedTeams = 0;

                        if (comp->mVecTeams.size() != lg->mTeams) {
                            if (mErrors) {
                                Error(L"Different number of teams in league: %d / %d\nCompetitionName: %s\nCompetitionID: %d",
                                    comp->mVecTeams.size(), lg->mTeams, lg->mName.c_str(), lg->mID);
                            }
                            continue;
                        }

                        if (!comp->mConverterData.mUsesTableData) {
                            for (auto entry : comp->mVecTeams) {
                                foom::club *team = (foom::club *)entry;
                                if (!team)
                                    continue;
                                Int teamRep = team->mReputation;
                                if (teamRep < 0)
                                    teamRep = 0;
                                else if (teamRep > 10'000)
                                    teamRep = 10'000;
                                if (!team->mIsReserveDummyClub) {
                                    if (team->mLastDivision) {
                                        if (team->mLastDivision != comp) {
                                            if (team->mLastDivision->mCompetitionLevel == 0 || team->mLastDivision->mCompetitionLevel > comp->mCompetitionLevel) {
                                                numLeaguePromotedTeams++;
                                                if (team->mLastPosition != 0)
                                                    team->mConverterData.mLeaguePos = 100'000 + team->mLastPosition;
                                                else
                                                    team->mConverterData.mLeaguePos = 100'000 + 1'000 + (10'000 - teamRep);
                                            }
                                            else {
                                                numLeagueRelegatedTeams++;
                                                if (team->mLastPosition != 0)
                                                    team->mConverterData.mLeaguePos = -100'000 - (1'000 - team->mLastPosition);
                                                else
                                                    team->mConverterData.mLeaguePos = -1 - teamRep;
                                            }
                                        }
                                        else {
                                            if (team->mLastPosition != 0)
                                                team->mConverterData.mLeaguePos = team->mLastPosition;
                                            else
                                                team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                                        }
                                    }
                                    else {
                                        numLeaguePromotedTeams++;
                                        team->mConverterData.mLeaguePos = 200'000 + (10'000 - teamRep);
                                    }
                                }
                                else {
                                    team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                                }
                            }
                        }
                        UInt leagueClubCounter = 0;
                        std::sort(comp->mVecTeams.begin(), comp->mVecTeams.end(), [](foom::team *a, foom::team *b) {
                            if (!a) return false;
                            if (!b) return true;
                            return ((foom::club *)a)->mConverterData.mLeaguePos < ((foom::club *)b)->mConverterData.mLeaguePos;
                        });

                        if (leagueWriter && leagueWriter->Available()) {
                            leagueWriter->WriteLine(L";----------------------------------------------------------------------------------------");
                            leagueWriter->WriteLine(Utils::Format(L"LEAGUE %d - %s (%d teams)", lg->mID, lg->mName, lg->mTeams));
                            leagueWriter->WriteLine(L";----------------------------------------------------------------------------------------");
                            for (auto entry : comp->mVecTeams) {
                                foom::club *team = (foom::club *)entry;
                                String teamLine = Utils::Format(L"%-10d - %s", team->mID, team->mName);
                                if (team->mConverterData.mParentClub)
                                    teamLine += L" (RES)";
                                leagueWriter->WriteLine(teamLine);
                            }
                        }

                        for (auto entry : comp->mVecTeams) {
                            foom::club *team = (foom::club *)entry;
                            if (!team) {
                                if (mErrors) {
                                    Error(L"Invalid club pointer in league\nLeague: %s", lg->mName.c_str());
                                }
                                continue;
                            }
                            FifamClub *club = nullptr;
                            foom::club *mainTeam = nullptr; // for reserve teams
                            FifamClubTeamType teamType = FifamClubTeamType::First;
                            Bool createNewClub = false;
                            Bool isExtinct = team->mExtinct;
                            if (team->mConverterData.mParentClub) {
                                isExtinct = team->mConverterData.mParentClub->mExtinct;
                                hasReserveTeams = true;
                            }
                            if (isExtinct) {
                                if (mErrors) {
                                    Error(L"Extinct club in the league\nClub: '%s'\nLeague: '%s'",
                                        team->mName.c_str(), lg->mName.c_str());
                                }
                            }
                            else if (team->mConverterData.mFifamClub) {
                                if (mErrors) {
                                    Message(Utils::Format(L"Team already present in other league\nClub: '%s'\nLeague: '%s'",
                                        team->mName.c_str(), lg->mName.c_str()));
                                }
                                club = (FifamClub *)team->mConverterData.mFifamClub;
                                if (team->mConverterData.mParentClub)
                                    teamType = FifamClubTeamType::Reserve;
                            }
                            else if (team->mConverterData.mParentClub) {
                                FifamClub *parentFifamClub = (FifamClub *)team->mConverterData.mParentClub->mConverterData.mFifamClub;
                                if (!parentFifamClub) {
                                    if (mErrors) {
                                        Error(L"Reserve club appears before the first team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                            team->mConverterData.mParentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                    }
                                }
                                else {
                                    if (!team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB
                                        && team->mConverterData.mParentClub->mNation == &nation)
                                    {
                                        parentFifamClub->SetProperty<Int>(L"res_team_foom_id", team->mID);
                                        club = parentFifamClub;
                                        teamType = FifamClubTeamType::Reserve;
                                        team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB = team;
                                        team->mConverterData.mParentClubInDB = team->mConverterData.mParentClub;
                                    }
                                    else {
                                        foom::club *parentClub = team->mConverterData.mParentClub;
                                        if (!team->mIsReserveDummyClub) {
                                            team->mConverterData.mParentClub->mConverterData.mChildClubs.erase(std::remove_if(
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.begin(),
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.end(),
                                                [=](foom::club::converter_data::child_club const &a) {
                                                    return a.mClub == team;
                                                }), team->mConverterData.mParentClub->mConverterData.mChildClubs.end());
                                            mainTeam = team->mConverterData.mParentClub;
                                            team->mConverterData.mOriginalStoredParentClub = team->mConverterData.mParentClub;
                                            team->mConverterData.mParentClub = nullptr;
                                            createNewClub = true;
                                        }
                                        else {
                                            if (mErrors) {
                                                Error(L"Reserve club can't be created because it's a dummy club\nMainClub: '%s'\nReserveTeamID: %d\nLeague: '%s'",
                                                    team->mConverterData.mParentClub->mName.c_str(), team->mID, lg->mName.c_str());
                                            }
                                        }
                                        if (mErrors) {
                                            //Error(L"Reserve club already present for team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                            //    parentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                        }
                                    }
                                }
                            }
                            else
                                createNewClub = true;
                            if (createNewClub)
                                club = CreateAndConvertClub(gameId, team, mainTeam, country, lg, true);
                            // put team to league
                            if (club) {
                                league->mTeams[leagueClubCounter].mPtr = club;
                                league->mTeams[leagueClubCounter].mTeamType = teamType;
                                club->SetProperty(L"league", league);
                                if (comp->mConverterData.mUsesTableData) {
                                    if (team->mConverterData.mTablePromoted) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                    }
                                    else if (team->mConverterData.mTableRelegated) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                    }
                                }
                                else {
                                    if (team->mConverterData.mLeaguePos >= 100'000) {
                                        if (numLeaguePromotedTeams <= (comp->mVecTeams.size() / 2)) {
                                            if (teamType == FifamClubTeamType::First)
                                                club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                            else
                                                club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                        }
                                    }
                                    else if (team->mConverterData.mLeaguePos < 0) {
                                        if (numLeagueRelegatedTeams <= (comp->mVecTeams.size() / 2)) {
                                            if (teamType == FifamClubTeamType::First)
                                                club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                            else
                                                club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                        }
                                    }
                                }
                                leagueClubCounter++;
                            }
                        }

                        if (hasReserveTeams) {
                            if (minLevelWithReserveTeams == -1 || minLevelWithReserveTeams > league->mLeagueLevel)
                                minLevelWithReserveTeams = league->mLeagueLevel;
                        }

                        bool gotFixtures = false;
                        Date leagueStart, leagueEnd;

                        if (mFixturesPerLeague.count(comp->mID) > 0) {
                            auto const &fixtures = mFixturesPerLeague[comp->mID];
                            UInt leagueTotalMatches = league->GetTotalNumMatches();
                            UInt leagueMatchdaysInRound = league->GetNumMatchdaysInRound();
                            UInt leagueMatchesInMatchday = league->GetNumMatchesInMatchday();
                            if (leagueTotalMatches > 0 && fixtures.size() == leagueTotalMatches) {
                                gotFixtures = true;
                                league->mFixtures.resize(league->mNumRounds * leagueMatchdaysInRound);
                                Map<FifamClubLink, UInt> leagueTeamsMap;
                                for (UInt i = 0; i < league->mTeams.size(); i++)
                                    leagueTeamsMap[league->mTeams[i]] = i + 1;
                                UInt matchId = 0;
                                UInt matchdayId = 0;
                                for (UInt r = 0; r < league->mNumRounds; r++) {
                                    if (!gotFixtures)
                                        break;
                                    for (UInt md = 0; md < leagueMatchdaysInRound; md++) {
                                        if (!gotFixtures)
                                            break;
                                        league->mFixtures[matchdayId].resize(leagueMatchesInMatchday);
                                        for (UInt d = 0; d < leagueMatchesInMatchday; d++) {
                                            if (fixtures.size() <= matchId) {
                                                if (mErrors) {
                                                    Error(L"Incorrect fixtures list size\nLeague: %s", comp->mName.c_str());
                                                }
                                                gotFixtures = false;
                                                break;
                                            }
                                            UInt team1id = 0, team2id = 0;
                                            if (!fixtures[matchId].mTeam1 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam1)) == 0) {
                                                if (mErrors) {
                                                    //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam1 ? GetTeamClubLink(fixtures[matchId].mTeam1).GetTeamName().c_str() : L"none");
                                                }
                                                gotFixtures = false;
                                                break;
                                            }
                                            team1id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam1)];
                                            if (!fixtures[matchId].mTeam2 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam2)) == 0) {
                                                if (mErrors) {
                                                    //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam2 ? GetTeamClubLink(fixtures[matchId].mTeam2).GetTeamName().c_str() : L"none");
                                                }
                                                gotFixtures = false;
                                                break;
                                            }
                                            team2id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam2)];
                                            league->mFixtures[matchdayId][d] = { team1id, team2id };
                                            matchId++;
                                        }
                                        matchdayId++;
                                    }
                                }
                            }
                            //else {
                            //    Error(L"Error fixtures list size\nLeague: %s\n%d/%d", comp->mName.c_str(), fixtures.size(), leagueTotalMatches);
                            //}
                        }

                        if (!gotFixtures)
                            league->GenerateFixtures();
                        league->GenerateCalendar(13, 0, 0, 0);

                        // TODO: translate date to calendar day

                        mNumTeamsInLeagueSystem[country->mId - 1] += lg->mTeams;

                        teamCounter += lg->mTeams;
                    }
                }

                Int relIndex = leagueSplit ? 2 : 0;

                // create relegation rounds
                for (auto &po : vecPlayOffs) {
                    if (!po->mIsLeague && po->mRounds.empty())
                        Error(L"Play-Off without rounds (%d in %s)", po->mID, FifamTr(country->mName).c_str());
                    else if ((po->mPromotionLevel == -1 && po->mRelegationLevel == -1) || po->mMinLeagueLevel == -1 || po->mMaxLeagueLevel == -1)
                        Error(L"Play-Off is not prepared for generation (%d in %s)", po->mID, FifamTr(country->mName).c_str());
                    else if ((!po->mIsLeague && po->mRounds[0].mNewTeams == 0) || (po->mIsLeague && po->mLeague.mTotalTeams == 0))
                        Error(L"Play-Off without teams (%d in %s)", po->mID, FifamTr(country->mName).c_str());
                    else if (po->mMaxLeagueLevel - po->mMinLeagueLevel > 1)
                        Error(L"Play-Off is shared between more than 2 levels (%d in %s)", po->mID, FifamTr(country->mName).c_str());
                    else if (po->mPromotionLevel == po->mRelegationLevel)
                        Error(L"Play-Off is a promotion and a relegation on the same level (%d in %s)", po->mID, FifamTr(country->mName).c_str());
                    else {
                        if (po->mIsLeague) {
                            if (po->mLeague.mTotalTeams != po->mLeague.mTeamEntries.size())
                                Error(L"Number of teams in Play-Off is not the same with number added teams - %d/%d (Play-Off %d in %s)", po->mLeague.mTotalTeams, po->mLeague.mTeamEntries.size(), po->mID, FifamTr(country->mName).c_str());
                            else {
                                FifamCompID compRelID = FifamCompID(country->mId, FifamCompType::Relegation, relIndex++);
                                FifamCompLeague *rel = mFifamDatabase->CreateCompetition(FifamCompDbType::League, compRelID, FifamNames::LimitName(po->mName, MAX_COMP_NAME_LENGTH))->AsLeague();
                                rel->mCompetitionLevel = po->mMaxLeagueLevel;
                                rel->mNumSubsAllowed = po->mSubs;
                                rel->mNumTeams = po->mLeague.mTotalTeams;
                                rel->mNumRounds = 1;
                                rel->mNumRelegatedTeams = 0;
                                rel->mLeagueLevel = rel->mCompetitionLevel;
                                rel->mAttendanceMp = 100;
                                rel->mTransferMarketMp = 100;
                                String teamsOverride;
                                for (UInt to = 0; to < rel->mNumTeams; to++) {
                                    if (to != 0)
                                        teamsOverride += L",";
                                    teamsOverride += Utils::Format(L"%02X%04X", country->mId, to + 1);
                                }
                                rel->SetProperty<String>(L"teams_override", teamsOverride);
                                rel->GenerateFixtures();
                                for (UInt ei = 0; ei < po->mLeague.mTeamEntries.size(); ei++) {
                                    auto const &e = po->mLeague.mTeamEntries[ei];
                                    if (!Utils::Contains(rel->mPredecessors, e.mLeague->mID))
                                        rel->mPredecessors.push_back(e.mLeague->mID);
                                    if (!Utils::Contains(e.mLeague->mSuccessors, rel->mID))
                                        e.mLeague->mSuccessors.push_back(rel->mID);
                                    rel->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(e.mLeague->mID, e.mTablePosition, 1));
                                }
                                rel->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                rel->mInstructions.PushBack(new FifamInstruction::SHUFFLE_TEAMS());
                                rel->SetProperty<UChar>(L"min_level", po->mMinLeagueLevel);
                                rel->SetProperty<UChar>(L"max_level", po->mMaxLeagueLevel);
                                po->mLeague.mFifamLeague = rel;
                            }
                        }
                        else {
                            Int roundTeams = po->mRounds[0].mNewTeams;
                            Bool first = true;
                            Bool previousLosersRound = false;
                            UInt teamsInPreviousRound = 0;
                            for (UInt ir = 0; ir < po->mRounds.size(); ir++) {
                                auto &pr = po->mRounds[ir];
                                if (ir != 0) {
                                    roundTeams /= 2;
                                    roundTeams += pr.mNewTeams;
                                }
                                if (roundTeams % 2 != 0) {
                                    Error(L"Play-Off with incorrect teams count - %d (in %s)", roundTeams, FifamTr(country->mName).c_str());
                                    continue;
                                }
                                if (pr.mNewTeams != pr.mTeamEntries.size()) {
                                    Error(L"Number of teams in Play-Off is not the same with number added teams - %d/%d (Play-Off %d in %s)", roundTeams, pr.mTeamEntries.size(), po->mID, FifamTr(country->mName).c_str());
                                    continue;
                                }

                                FifamRoundID roundType = FifamRoundID::None;
                                if (ir == 0)
                                    roundType = FifamRoundID::_1;
                                else if (ir == 1)
                                    roundType = FifamRoundID::_2;
                                else if (ir == 2)
                                    roundType = FifamRoundID::_3;
                                else if (ir == 3)
                                    roundType = FifamRoundID::_4;
                                else if (ir == 4)
                                    roundType = FifamRoundID::_5;
                                String roundName = po->mName;
                                String roundStageName;
                                if (po->mRounds.size() > 1)
                                    roundStageName = L"Rd " + Utils::Format(L"%u", ir + 1);
                                if (ir == po->mRounds.size() - 1 && roundTeams == 2) {
                                    roundType = FifamRoundID::Final;
                                    if (po->mRounds.size() > 1)
                                        roundStageName = L"Final";
                                }
                                else if (po->mRounds.size() > 1 && ir == po->mRounds.size() - 2 && roundTeams == 4 && po->mRounds[ir + 1].mNewTeams == 0) {
                                    roundType = FifamRoundID::Semifinal;
                                    if (po->mRounds.size() > 1)
                                        roundStageName = L"Semi";
                                }
                                if (!roundStageName.empty())
                                    roundName += L" " + roundStageName;
                                FifamCompID compRelID = FifamCompID(country->mId, FifamCompType::Relegation, relIndex++);
                                FifamCompRound *rel = mFifamDatabase->CreateCompetition(FifamCompDbType::Round, compRelID, FifamNames::LimitName(roundName, MAX_COMP_NAME_LENGTH))->AsRound();
                                rel->mCompetitionLevel = po->mMaxLeagueLevel;
                                rel->mNumSubsAllowed = po->mSubs;
                                rel->mNumTeams = roundTeams;
                                rel->mRoundType = FifamRoundID::Final;/*roundType*/;
                                if (pr.mLegs == 2) {
                                    rel->m1stLegFlags.Set(FifamBeg::_2ndLeg, true);
                                    rel->m2ndLegFlags.Set(FifamBeg::WithExtraTime, true);
                                    rel->m2ndLegFlags.Set(FifamBeg::WithPenalty, true);
                                    rel->m2ndLegFlags.Set(FifamBeg::_2ndLeg, true);
                                }
                                else {
                                    rel->m1stLegFlags.Set(FifamBeg::WithExtraTime, true);
                                    rel->m1stLegFlags.Set(FifamBeg::WithPenalty, true);
                                    rel->m1stLegFlags.Set(FifamBeg::_1stLeg, true);
                                    rel->m1stLegFlags.Set(FifamBeg::End, true);
                                }
                                if (ir != po->mRounds.size() - 1)
                                    rel->mSuccessors.push_back(FifamCompID(country->mId, FifamCompType::Relegation, relIndex));
                                // sort all entries
                                std::sort(pr.mTeamEntries.begin(), pr.mTeamEntries.end(), [](PlayOffInfo::TeamEntry const &a, PlayOffInfo::TeamEntry const &b) {
                                    if (a.mLevel < b.mLevel) return true;
                                    if (a.mLevel > b.mLevel) return false;
                                    if (a.mPositionIndex < b.mPositionIndex) return true;
                                    if (a.mPositionIndex > b.mPositionIndex) return false;
                                    return a.mLeagueIndex <= b.mLeagueIndex;
                                    });
                                if (ir == 0 && pr.mShuffle == PlayOffInfo::Round::ShuffleType::None) // first (entering) round - reverse second part
                                    std::reverse(pr.mTeamEntries.begin() + pr.mTeamEntries.size() / 2, pr.mTeamEntries.end());
                                if (ir == 0) { // first (entering) round - league predecessors
                                    for (auto const &prr : po->mRounds) {
                                        for (auto const &ee : prr.mTeamEntries) {
                                            if (!Utils::Contains(rel->mPredecessors, ee.mLeague->mID))
                                                rel->mPredecessors.push_back(ee.mLeague->mID);
                                            if (!Utils::Contains(ee.mLeague->mSuccessors, rel->mID))
                                                ee.mLeague->mSuccessors.push_back(rel->mID);
                                        }
                                    }
                                }
                                for (UInt ei = 0; ei < pr.mTeamEntries.size(); ei++) {
                                    auto const &e = pr.mTeamEntries[ei];
                                    rel->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(e.mLeague->mID, e.mTablePosition, 1));
                                    if (pr.mShuffle == PlayOffInfo::Round::ShuffleType::HalfTeams && ei == (pr.mTeamEntries.size() / 2 - 1))
                                        rel->mInstructions.PushBack(new FifamInstruction::SHUFFLE_TEAMS());
                                }
                                if (ir != 0) {
                                    FifamCompID prevRoundID = FifamCompID(country->mId, FifamCompType::Relegation, relIndex - 2);
                                    rel->mPredecessors.push_back(prevRoundID);
                                    if (previousLosersRound)
                                        rel->mInstructions.PushBack(new FifamInstruction::GET_LOSER(prevRoundID));
                                    else
                                        rel->mInstructions.PushBack(new FifamInstruction::GET_WINNER(prevRoundID));
                                }
                                rel->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                if (pr.mShuffle == PlayOffInfo::Round::ShuffleType::AllTeams)
                                    rel->mInstructions.PushBack(new FifamInstruction::SHUFFLE_TEAMS());
                                rel->SetProperty<UChar>(L"min_level", po->mMinLeagueLevel);
                                rel->SetProperty<UChar>(L"max_level", po->mMaxLeagueLevel);
                                previousLosersRound = pr.mLosersRound;
                                teamsInPreviousRound = roundTeams;
                                pr.mFifamRound = rel;
                            }
                        }
                    }
                }

                // create league levels
                for (Int i = 0; i <= maxLevel; i++) {
                    DivisionInfo *level = nullptr;
                    Int levelRep = 0;
                    String levelName;
                    auto itLevel = divLeagueLevels.find(i);
                    // if level exists
                    if (itLevel != divLeagueLevels.end()) {
                        level = divLeagueLevels[i];
                        if (level->mName.length() < 29)
                            levelName = level->mName;
                        else
                            levelName = level->mShortName;
                        levelRep = level->mRep;
                    }
                    else { // if level does not exist
                        Vector<String> divsOnLevelNames;
                        auto itLeagues = divLeagues.find(i);
                        if (itLeagues != divLeagues.end()) {
                            for (auto &div : divLeagues[i])
                                divsOnLevelNames.push_back(div->mName);
                            if (!divsOnLevelNames.empty())
                                levelName = FifamNames::GetLeagueLevelName(divsOnLevelNames, i + 1);
                            if (divLeagues[i].size() > 0)
                                levelRep = divLeagues[i][0]->mRep;
                        }
                        else
                            Error(L"No league levels or leagues at level %d in country %s", i, FifamTr(country->mName).c_str());
                    }
                    // setup level information
                    auto &levelNames = country->mLeagueLevelNames.emplace_back();
                    FifamTrSetAll<String>(levelNames, FifamNames::LimitName(levelName, 28));

                    DivisionInfo *firstDivOnLevel = nullptr;

                    auto itFirstLeague = divLeagues.find(i);
                    if (itFirstLeague != divLeagues.end() && divLeagues[i].size() > 0)
                        firstDivOnLevel = divLeagues[i][0];

                    if (firstDivOnLevel) {
                        country->mLeagueLevels[i].mEqualPointsSorting = firstDivOnLevel->mSorting;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = firstDivOnLevel->mForeignersLimit;
                        country->mLeagueLevels[i].mNumRelegatedTeams = customLeagues ? 0 : firstDivOnLevel->mRelegated;
                    }
                    else {
                        country->mLeagueLevels[i].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = 0;
                        country->mLeagueLevels[i].mNumRelegatedTeams = 0;
                    }

                    country->mLeagueLevels[i].mRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));

                    if (country->mLeagueLevels[i].mRating > MAX_LEAGUE_RATING)
                        country->mLeagueLevels[i].mRating = MAX_LEAGUE_RATING;

                    if (i == 0)
                        country->mAveragePlayerRating = country->mLeagueLevels[i].mRating;

                    // setup pool

                    FifamCompID poolID = FifamCompID(country->mId, FifamCompType::Pool, i);
                    FifamCompPool *pool = mFifamDatabase->CreateCompetition(FifamCompDbType::Pool, poolID, FifamNames::LimitName(levelName, MAX_POOL_NAME_LENGTH) + L" Pool")->AsPool();
                    pool->mCompetitionLevel = i;

                    createdPools.push_back(pool);

                    if (level)
                        level->mCompID = poolID;

                    if (i > 0)
                        pool->mCompConstraints.push_back(FifamCompID(country->mId, FifamCompType::Pool, i - 1));


                    if (i > 0) {
                        UInt higherLevel = i - 1;
                        // higher level - losers of relegation play-off
                        for (auto &po : vecPlayOffs) {
                            if (po->mRelegationLevel == higherLevel && po->mPromotionLevel == -1) {
                                if (po->mIsLeague) {
                                    if (po->mLeague.mFifamLeague) {
                                        if (po->mLeague.mNumLosers > 0)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, po->mLeague.mNumWinners + 1, po->mLeague.mNumLosers));
                                    }
                                    else
                                        Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else {
                                    if (!po->mRounds.empty()) {
                                        for (UInt r = 0; r < po->mRounds.size(); r++) {
                                            FifamCompRound *round = po->mRounds[r].mFifamRound;
                                            if (round) {
                                                if (!po->mRounds[r].mLosersRound)
                                                    pool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(round->mID));
                                            }
                                            else
                                                Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                        }
                                    }
                                    else
                                        Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                            }
                        }
                        // relegated from mixed play-off
                        for (auto &po : vecPlayOffs) {
                            if (po->mRelegationLevel == higherLevel && po->mPromotionLevel == i) {
                                if (po->mIsLeague) {
                                    if (po->mLeague.mFifamLeague) {
                                        if (po->mLeague.mNumLosers > 0)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, po->mLeague.mNumWinners + 1, po->mLeague.mNumLosers));
                                    }
                                    else
                                        Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else {
                                    if (!po->mRounds.empty()) {
                                        for (UInt r = 0; r < po->mRounds.size(); r++) {
                                            FifamCompRound *round = po->mRounds[r].mFifamRound;
                                            if (round) {
                                                if (!po->mRounds[r].mLosersRound)
                                                    pool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(round->mID));
                                            }
                                            else
                                                Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                        }
                                    }
                                    else
                                        Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                            }
                        }
                        // higher level - relegated teams
                        if (divLeagues.find(higherLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[higherLevel]) {
                                if (l->mRelegated > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                        l->mCompID, l->mTeams - l->mRelegated + 1, l->mRelegated));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", higherLevel, FifamTr(country->mName).c_str());
                        // promotion losers from this level
                        for (auto &po : vecPlayOffs) {
                            if (po->mRelegationLevel == -1 && po->mPromotionLevel == i) {
                                if (po->mIsLeague) {
                                    if (po->mLeague.mFifamLeague) {
                                        if (po->mLeague.mNumLosers > 0)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, po->mLeague.mNumWinners + 1, po->mLeague.mNumLosers));
                                    }
                                    else
                                        Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else {
                                    if (!po->mRounds.empty()) {
                                        for (UInt r = 0; r < po->mRounds.size(); r++) {
                                            FifamCompRound *round = po->mRounds[r].mFifamRound;
                                            if (round) {
                                                if (!po->mRounds[r].mLosersRound)
                                                    pool->mInstructions.PushBack(new FifamInstruction::GET_LOSER(round->mID));
                                            }
                                            else
                                                Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                        }
                                    }
                                    else
                                        Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                            }
                        }
                    }

                    // this level
                    if (divLeagues.find(i) != divLeagues.end()) {
                        // apply pool sorting if 2 or more leagues on this level
                        if (divLeagues[i].size() > 1)
                            pool->mSorting = FifamPoolSorting::MapLeague;

                        for (DivisionInfo *l : divLeagues[i]) {
                            if (customLeagues)
                                pool->mSuccessors.push_back(l->mCompID);
                            pool->mNumTeams += l->mTeams;
                            if (i == 0 && relLeague[0]) {
                                Int numTeamsFromThisLeague = relLeague[0]->mNumTeams;
                                pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(relLeague[0]->mID, 1, numTeamsFromThisLeague));
                                numTeamsFromThisLeague = relLeague[1]->mNumTeams - (l->mRelegated + l->mTotalTeamsRelegationPlayoff);
                                if (numTeamsFromThisLeague > 0)
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(relLeague[1]->mID, 1, numTeamsFromThisLeague));
                            }
                            else {
                                Int numTeamsFromThisLeague = l->mTeams - (l->mPromoted + l->mRelegated + l->mTotalTeamsPromotionPlayoff + l->mTotalTeamsRelegationPlayoff);
                                if (numTeamsFromThisLeague > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                        l->mCompID, l->mPromoted + l->mTotalTeamsPromotionPlayoff + 1, numTeamsFromThisLeague));
                                }
                            }
                        }
                    }
                    else
                        Error(L"No leagues at level %d\nCountry: %s", i, FifamTr(country->mName).c_str());

                    // relegation winners from this level
                    for (auto &po : vecPlayOffs) {
                        if (po->mRelegationLevel == i && po->mPromotionLevel == -1) {
                            if (po->mIsLeague) {
                                if (po->mLeague.mFifamLeague) {
                                    if (po->mLeague.mNumWinners > 0)
                                        pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, 1, po->mLeague.mNumWinners));
                                }
                                else
                                    Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                            }
                            else {
                                if (!po->mRounds.empty()) {
                                    FifamCompRound *round = po->mRounds.back().mFifamRound;
                                    if (round)
                                        pool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(round->mID));
                                    else
                                        Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else
                                    Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                            }
                        }
                    }
                    if (i != maxLevel) {
                        // next level - promoted teams
                        UInt lowerLevel = i + 1;
                        if (divLeagues.find(lowerLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[lowerLevel]) {
                                if (l->mPromoted > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(l->mCompID, 1, l->mPromoted));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", lowerLevel, FifamTr(country->mName).c_str());
                        // relegation winners from mixed levels play-off
                        for (auto &po : vecPlayOffs) {
                            if (po->mRelegationLevel == i && po->mPromotionLevel == lowerLevel) {
                                if (po->mIsLeague) {
                                    if (po->mLeague.mFifamLeague) {
                                        if (po->mLeague.mNumWinners > 0)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, 1, po->mLeague.mNumWinners));
                                    }
                                    else
                                        Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else {
                                    if (!po->mRounds.empty()) {
                                        for (UInt r = 0; r < po->mRounds.size(); r++) {
                                            FifamCompRound *round = po->mRounds[r].mFifamRound;
                                            if (round) {
                                                if (po->mRounds[r].mLosersRound || r == (po->mRounds.size() - 1))
                                                    pool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(round->mID));
                                            }
                                            else
                                                Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                        }
                                    }
                                    else
                                        Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                            }
                        }
                        // promotion winners from next level
                        for (auto &po : vecPlayOffs) {
                            if (po->mRelegationLevel == -1 && po->mPromotionLevel == lowerLevel) {
                                if (po->mIsLeague) {
                                    if (po->mLeague.mFifamLeague) {
                                        if (po->mLeague.mNumWinners > 0)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(po->mLeague.mFifamLeague->mID, 1, po->mLeague.mNumWinners));
                                    }
                                    else
                                        Error(L"Play-off league has no FifamCompLeague instance\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                                else {
                                    if (!po->mRounds.empty()) {
                                        FifamCompRound *round = po->mRounds.back().mFifamRound;
                                        if (round)
                                            pool->mInstructions.PushBack(new FifamInstruction::GET_WINNER(round->mID));
                                        else
                                            Error(L"Play-off round has no FifamCompRound instance\nCountry: %s", FifamTr(country->mName).c_str());
                                    }
                                    else
                                        Error(L"Play-off has no rounds\nCountry: %s", FifamTr(country->mName).c_str());
                                }
                            }
                        }
                    }
                    else { // CaC league & spare
                        if (leagueIndex < 32) {
                            pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                FifamCompID(country->mId, FifamCompType::League, leagueIndex), 1, 1));
                        }
                    }
                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                }

                if (minLevelWithReserveTeams >= 0) {
                    country->mLeagueLevelWithReserveTeams = minLevelWithReserveTeams;
                    for (auto pool : createdPools) {
                        if (pool->mCompetitionLevel >= minLevelWithReserveTeams)
                            pool->mReserveTeamsAllowed = true;
                    }
                }
                else
                    country->mLeagueLevelWithReserveTeams = maxLevel + 1;

                if (leagueWriter && leagueWriter->Available()) {
                    leagueWriter->WriteLine(L";----------------------------------------------------------------------------------------");
                    leagueWriter->WriteLine(L"NON-LEAGUE CLUBS");
                    leagueWriter->WriteLine(L";----------------------------------------------------------------------------------------");
                    for (auto &entry : mFoomDatabase->mClubs) {
                        auto &c = entry.second;
                        if (!c.mExtinct && c.mNation == &nation && !c.mConverterData.mFifamClub) {
                            String teamLine = Utils::Format(L"%-10d - %s", c.mID, c.mName);
                            if (c.mConverterData.mParentClub)
                                teamLine += L" (RES)";
                            leagueWriter->WriteLine(teamLine);
                        }
                    }
                }

            }
            else {
                country->mLeagueLevels[0].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                country->mLeagueLevels[0].mNumNonEUPlayersAllowed = 0;
                country->mLeagueLevels[0].mNumRelegatedTeams = 0;
                country->mLeagueLevels[0].mRating = 0;

                Int levelRep = -1;

                for (auto &divInfo : mDivisions) {
                    if (divInfo.mNationID == nation.mID && divInfo.mType == DivisionInfo::League && divInfo.mRep > levelRep)
                        levelRep = divInfo.mRep;
                }

                if (levelRep > 0) {
                    country->mAveragePlayerRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));
                    if (country->mAveragePlayerRating > MAX_LEAGUE_RATING)
                        country->mAveragePlayerRating = MAX_LEAGUE_RATING;
                }
            }

            for (auto &e : mLeaguesSystem[country->mId - 1]) {
                for (auto &l : e.second)
                    l->SetProperty<Int>(L"NumLeaguesOnLevel", e.second.size());
            }

            if (mNumTeamsInLeagueSystem[country->mId - 1] > 0) {
                Int cupsMinPriority = 1;
                Int maxFACups = 1;
                Int maxLeagueCups = 4;
                Int maxSuperCups = 1;
                Int numCreatedFACups = 0;
                Int numCreatedLeagueCups = 0;
                Int numCreatedSuperCups = 0;
                if (gameId <= 7)
                    cupsMinPriority = 2;
                Int startMatchdaySuperCup = 3;
                Int startMatchdayFaCup = startMatchdaySuperCup + 2;
                Int startMatchdayLeagueCup = startMatchdayFaCup + 16;

                // find all cups in this country
                for (auto &cupInfo : mCups) {
                    if (cupInfo.mNationID == nation.mID && cupInfo.mPriority >= cupsMinPriority) {
                        Int nextAvailableCompIndex = -1;
                        FifamCompType cupCompType = FifamCompType::LeagueCup;
                        if (cupInfo.mType == CupInfo::FA) {
                            if (numCreatedFACups < maxFACups) {
                                nextAvailableCompIndex = numCreatedFACups;
                                numCreatedFACups++;
                                cupCompType = FifamCompType::FaCup;
                                if (!nation.mConverterData.mDomesticComps.cup)
                                    nation.mConverterData.mDomesticComps.cup = &cupInfo;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::League) {
                            if (numCreatedLeagueCups < maxLeagueCups) {
                                nextAvailableCompIndex = numCreatedLeagueCups;
                                numCreatedLeagueCups++;
                                cupCompType = FifamCompType::LeagueCup;
                                if (!nation.mConverterData.mDomesticComps.leagueCup)
                                    nation.mConverterData.mDomesticComps.leagueCup = &cupInfo;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::Supercup) {
                            if (numCreatedSuperCups < maxSuperCups) {
                                nextAvailableCompIndex = numCreatedSuperCups;
                                cupCompType = FifamCompType::SuperCup;
                                if (!nation.mConverterData.mDomesticComps.supercup)
                                    nation.mConverterData.mDomesticComps.supercup = &cupInfo;
                            }
                        }
                        if (nextAvailableCompIndex != -1) {
                            if (cupInfo.mTemplateType == FifamCupSystemType::None) {
                                Error(L"Unknown cup template type\nCup: %s", cupInfo.mName.c_str());
                                continue;
                            }
                            FifamCupAlloc *cupTemplate = mFifamDatabase->GetCupTemplate(cupInfo.mTemplateType);
                            if (!cupTemplate) {
                                Error(L"Cup template not found\nCup: %s\nTemplate: %s", cupInfo.mName.c_str(), cupInfo.mTemplateType.ToCStr());
                                continue;
                            }
                            FifamCompID cupID = { (UChar)country->mId, cupCompType, (UShort)nextAvailableCompIndex };
                            String cupName;
                            if (cupInfo.mName.length() <= MAX_CUP_NAME_LENGTH)
                                cupName = cupInfo.mName;
                            else
                                cupName = FifamNames::LimitName(cupInfo.mShortName, MAX_CUP_NAME_LENGTH);
                            FifamCompCup *cup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, cupID, cupName)->AsCup();
                            createdCups.push_back(cup);
                            cup->SetProperty(L"foom::id", cupInfo.mID);
                            cup->SetProperty(L"foom::reputation", cupInfo.mReputation);
                            if (cupInfo.mLevel < 0 || cupInfo.mLevel > 15)
                                cup->mCompetitionLevel = 0;
                            else
                                cup->mCompetitionLevel = cupInfo.mLevel;
                            if (cupInfo.mNumSubs <= 0)
                                cup->mNumSubsAllowed = 3;
                            else
                                cup->mNumSubsAllowed = Utils::Clamp(cupInfo.mNumSubs, 1, 7);
                            cup->mCupTemplate = cupInfo.mTemplateType;
                            Int numRounds = 0;
                            Int startRoundIndex = -1;
                            for (Int i = 7; i >= 0; i--) {
                                if (cupInfo.mRoundDesc[i].empty())
                                    break;
                                startRoundIndex = i;
                                numRounds++;
                            }
                            if (numRounds > 0 && startRoundIndex >= 0) {
                                cup->mRounds.resize(numRounds);
                                bool failed = false;
                                Int r = 0;
                                for (Int i = startRoundIndex; i <= 7; i++) {
                                    if (cupInfo.mRoundDesc[i].empty()) {
                                        Error(L"Empty cup description\nCountry: %s\nCup: %s", FifamTr(country->mName).c_str(), cupInfo.mName.c_str());
                                        failed = true;
                                        break;
                                    }
                                    cup->mRounds[r].mFlags.Clear();
                                    for (UInt c = 0; c < cupInfo.mRoundDesc[i].length(); c++) {
                                        if (c == 0) {
                                            if (cupInfo.mRoundDesc[i][c] == '1')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_1stLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '2')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_2ndLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'R')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithReplay, true);
                                            else {
                                                Error(L"Wrong round description in cup\nCup: %s\nRound: %d\nDescription: %s", cupInfo.mName.c_str(), i, cupInfo.mRoundDesc[i].c_str());
                                                failed = true;
                                                break;
                                            }
                                        }
                                        else {
                                            if (cupInfo.mRoundDesc[i][c] == 'E')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithExtraTime, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'P')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithPenalty, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'G')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithGoldenGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'S')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithSilverGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'A')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithoutAwayGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '-' && (c + 1) < cupInfo.mRoundDesc[i].length()) {
                                                if (cupInfo.mRoundDesc[i][c + 1] == 'P')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoPremierLeagueTeams, true);
                                                else if (cupInfo.mRoundDesc[i][c + 1] == 'S')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoShuffle, true);
                                                c++;
                                            }
                                            else if (cupInfo.mRoundDesc[i][c] == '+') {
                                                Int newTeamsInRound = Utils::SafeConvertInt<Int>(cupInfo.mRoundDesc[i].substr(c + 1));
                                                if (newTeamsInRound > 0)
                                                    cup->mRounds[r].mNewTeamsRound = newTeamsInRound;
                                                break;
                                            }
                                        }
                                    }
                                    if (failed)
                                        break;
                                    r++;
                                }
                                if (failed)
                                    cup->mRounds.clear();
                                else if (!cup->mRounds.empty()) {
                                    UInt teamsFromPrevRound = 0;
                                    UInt currBeg = 0;
                                    UInt lastRoundWithAddedTeams = 0;
                                    for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                        cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                        cup->mRounds[r].mTeamsRound = cup->mRounds[r].mNewTeamsRound + teamsFromPrevRound;
                                        teamsFromPrevRound = cup->mRounds[r].mTeamsRound / 2;
                                        cup->mRounds[r].mStartBeg = currBeg;
                                        currBeg += teamsFromPrevRound;
                                        cup->mRounds[r].mEndBeg = currBeg;
                                        if (cup->mRounds[r].mNewTeamsRound)
                                            lastRoundWithAddedTeams = r;
                                    }
                                    UInt numRawRounds = cup->mRounds.size() - lastRoundWithAddedTeams;
                                    // final rounds
                                    Int numFinalRounds = 0;
                                    for (Int r = (Int)cup->mRounds.size() - 1; r >= (Int)lastRoundWithAddedTeams; r--) {
                                        if (numFinalRounds >= 4)
                                            break;
                                        cup->mRounds[r].mRoundID.SetFromInt(15 - numFinalRounds);
                                        numFinalRounds++;
                                    }
                                    Int numRoundsLeft = cup->mRounds.size() - numFinalRounds;
                                    // quali rounds
                                    Int numQualiRounds = numRoundsLeft > 5 ? 8 - numRoundsLeft : 0;
                                    for (Int r = 0; r < numQualiRounds; r++)
                                        cup->mRounds[r].mRoundID.SetFromInt(r + 1);
                                    // other rounds
                                    numRoundsLeft -= numQualiRounds;
                                    for (Int r = 0; r < numRoundsLeft; r++)
                                        cup->mRounds[r + numQualiRounds].mRoundID.SetFromInt(r + 5);
                                }
                            }
                            if (cup->mRounds.empty()) {
                                // use template
                                UInt numRounds = cupTemplate->mRounds.size();
                                cup->mRounds.resize(numRounds);
                                for (UInt i = 0; i < numRounds; i++) {
                                    cup->mRounds[i].mTeamsRound = cupTemplate->mRounds[i]->mTeamsRound;
                                    cup->mRounds[i].mNewTeamsRound = cupTemplate->mRounds[i]->mNewTeamsRound;
                                    cup->mRounds[i].mStartBeg = cupTemplate->mRounds[i]->mStartBeg;
                                    cup->mRounds[i].mEndBeg = cupTemplate->mRounds[i]->mEndBeg;
                                    cup->mRounds[i].mFlags = cupTemplate->mRounds[i]->mFlags;
                                    cup->mRounds[i].mRoundID = cupTemplate->mRounds[i]->mRoundID;
                                }
                            }
                            cup->mNumTeams = 0;
                            if (cupInfo.mBonus <= 0)
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mReputation * cupInfo.mReputation * 75, 1'000);
                            else
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mBonus, 1'000);
                            if (cupInfo.mTvBonus <= 0)
                                cupInfo.mTvBonus = AlignMoneyValue((cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation) / 320, 1'000);
                            else
                                cupInfo.mTvBonus = AlignMoneyValue(cupInfo.mTvBonus, 1'000);
                            UInt numMatchdays = 0;
                            for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                cup->mRounds[r].mBonuses[1] = cupInfo.mBonus;
                                cup->mRounds[r].mBonuses[3] = cupInfo.mTvBonus;
                                UInt roundId = cup->mRounds.size() - r - 1;
                                if (roundId != 0) {
                                    UInt divValue = 1 << roundId;
                                    cup->mRounds[r].mBonuses[1] = AlignMoneyValue(cup->mRounds[r].mBonuses[1] / divValue, 1'000u);
                                    cup->mRounds[r].mBonuses[3] = AlignMoneyValue(cup->mRounds[r].mBonuses[3] / divValue, 1'000u);
                                }
                                if (cup->mRounds[r].mFlags.Check(FifamBeg::_2ndLeg) || cup->mRounds[r].mFlags.Check(FifamBeg::WithReplay))
                                    numMatchdays += 2;
                                else
                                    numMatchdays += 1;
                            }
                            if (!cupInfo.mStructure.empty()) {
                                UInt currentLevel = 0;
                                currentLevel = cupInfo.mMaxLevel;
                                for (auto c : cupInfo.mStructure) {
                                    switch (c) {
                                    case L'-':
                                        if (mLeaguesSystem[country->mId - 1][currentLevel].size() > 0)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(mLeaguesSystem[country->mId - 1][currentLevel][0]->mID, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'|':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(currentLevel, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'W':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                        break;
                                    case L'S':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                        break;
                                    case L'C':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'L':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    case L'R':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'N':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    }
                                }
                            }
                            if (cup->mInstructions.Empty()) {
                                if (cupInfo.mType == CupInfo::Supercup) {
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(0, 1, 24));
                                    if (gameId >= 10)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                }
                                else {
                                    for (auto const &level : mLeaguesSystem[country->mId - 1]) {
                                        if (level.first >= (UInt)cupInfo.mMaxLevel) {
                                            if (level.first == 0 && level.second.size() == 1)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(level.second[0]->mID, 1, 24));
                                            else if (level.second.size() > 0)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(level.first, 1, 24));
                                        }
                                    }
                                    if (cupInfo.mType != CupInfo::League)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                }
                            }
                            if (cup->mRounds.empty()) {
                                Error(L"Cup has no rounds\nCountry: %s\nCup: %s",
                                    FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else if (cup->mNumTeams > 128) {
                                Error(L"Cup has more than 128 teams: %d\nCountry: %s\nCup: %s",
                                    cup->mNumTeams, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else {
                                for (unsigned int ir = 0; ir < cup->mRounds.size(); ir++) {
                                    if (cup->mRounds[ir].mTeamsRound == 0 || (cup->mRounds[ir].mTeamsRound % 2)) {
                                        Error(L"Wrong teams count in cup round: %d\nCountry: %s\nCup: %s\nRound: %d",
                                            cup->mRounds[ir].mTeamsRound, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str(), ir);
                                    }
                                }
                            }
                            //cup->mMathcdays
                            //for (UInt r = 0; r < cup->mRounds.size(); r++) {
                            for (UInt m = 0; m < numMatchdays; m++) {
                                if (cupCompType == FifamCompType::FaCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayFaCup++);
                                else if (cupCompType == FifamCompType::LeagueCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayLeagueCup++);
                                else if (cupCompType == FifamCompType::SuperCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdaySuperCup++);
                            }
                            //}
                            cup->mSecondSeasonMatchdays = cup->mFirstSeasonMatchdays;
                        }
                    }
                }
            }

            Pair<FifamCompLeague *, FifamCompLeague *> splitLeagues = { relLeague[0], relLeague[1] };

            if (!mFromFifaDatabase) {
                if (mGenerateSpecialScripts)
                    ProcessScriptWithSpecialFormat(country, createdLeagues, createdCups, splitLeagues, vecPlayOffs);

                auto ProcessCustomCalendars = [&](Map<UInt, Vector<UShort>> const &calendars, UChar season) {
                    for (auto const &[compId, comp] : mFifamDatabase->mCompMap) {
                        if (compId.mRegion == country->mId) {
                            auto it = calendars.find(compId.ToInt());
                            if (it != calendars.end()) {
                                auto const &calendar = (*it).second;
                                UInt i = 0, numMatches = 0;
                                switch (comp->GetDbType().ToInt()) {
                                case FifamCompDbType::League:
                                    if (comp->AsLeague()->GetNumMatchdays() == calendar.size()) {
                                        if (season == 0)
                                            comp->AsLeague()->mFirstSeasonMatchdays = calendar;
                                        comp->AsLeague()->mSecondSeasonMatchdays = calendar;
                                        comp->SetProperty<Bool>(L"custom_calendar", true);
                                    }
                                    else
                                        Error(L"Unable to assign calendar to league %08X - different matchdays count (%u/%u)", compId.ToInt(), calendar.size(), comp->AsLeague()->GetNumMatchdays());
                                    break;
                                case FifamCompDbType::Cup:
                                    for (i = 0; i < comp->AsCup()->mRounds.size(); i++)
                                        numMatches += (comp->AsCup()->mRounds[i].mFlags.Check(FifamBeg::_2ndLeg) || comp->AsCup()->mRounds[i].mFlags.Check(FifamBeg::WithReplay)) ? 2 : 1;
                                    if (numMatches == calendar.size()) {
                                        if (season == 0)
                                            comp->AsCup()->mFirstSeasonMatchdays = calendar;
                                        comp->AsCup()->mSecondSeasonMatchdays = calendar;
                                        comp->SetProperty<Bool>(L"custom_calendar", true);
                                    }
                                    else
                                        Error(L"Unable to assign calendar to cup %08X - different matchdays count (%u/%u)", compId.ToInt(), calendar.size(), numMatches);
                                    break;
                                case FifamCompDbType::Round:
                                    numMatches = comp->AsRound()->m1stLegFlags.Check(FifamBeg::_2ndLeg) ? 2 : 1;
                                    if (numMatches == calendar.size()) {
                                        if (season == 0) {
                                            if (numMatches == 2)
                                                comp->AsRound()->mFirstSeasonMatchdays = { calendar[0], calendar[1] };
                                            else
                                                comp->AsRound()->mFirstSeasonMatchdays = { calendar[0], 0 };
                                            comp->AsRound()->mSecondSeasonMatchdays = comp->AsRound()->mFirstSeasonMatchdays;
                                        }
                                        else {
                                            if (numMatches == 2)
                                                comp->AsRound()->mSecondSeasonMatchdays = { calendar[0], calendar[1] };
                                            else
                                                comp->AsRound()->mSecondSeasonMatchdays = { calendar[0], 0 };
                                        }
                                        comp->SetProperty<Bool>(L"custom_calendar", true);
                                    }
                                    else
                                        Error(L"Unable to assign calendar to league %08X - different matchdays count (%u/%u)", compId.ToInt(), calendar.size(), numMatches);
                                    break;
                                }
                            }
                        }
                    }
                };

                ProcessCustomCalendars(mCalendarsFirstSeason, 0);
                ProcessCustomCalendars(mCalendarsSecondSeason, 1);
            }

            GenerateCalendar(FifamNation::MakeFromInt(country->mId), mFifamDatabase, createdLeagues, createdCups, splitLeagues, vecPlayOffs);

            delete leagueWriter;
        }
    }

    delete leagueConfigSplit;
    delete leagueConfigTables;

    if (!mFromFifaDatabase) {
    // verify calendars
        for (auto const &[id, data] : mCalendarsFirstSeason) {
            if (!mFifamDatabase->GetCompetition(id))
                Error(L"Unable to assign calendar to competition %08X - competition was not found", id);
        }
        for (auto const &[id, data] : mCalendarsSecondSeason) {
            if (!mFifamDatabase->GetCompetition(id))
                Error(L"Unable to assign calendar to competition %08X - competition was not found", id);
        }
    }

    for (auto const &[compId, comp] : mFifamDatabase->mCompMap) {
        UInt i = 0, numMatches = 0;
        switch (comp->GetDbType().ToInt()) {
        case FifamCompDbType::League:
            if (comp->AsLeague()->mFirstSeasonMatchdays.size() != comp->AsLeague()->GetNumMatchdays() || comp->AsLeague()->mSecondSeasonMatchdays.size() != comp->AsLeague()->GetNumMatchdays())
                Error(L"Incorrect calendar in league %08X", compId.ToInt());
            break;
        case FifamCompDbType::Cup:
            for (i = 0; i < comp->AsCup()->mRounds.size(); i++)
                numMatches += (comp->AsCup()->mRounds[i].mFlags.Check(FifamBeg::_2ndLeg) || comp->AsCup()->mRounds[i].mFlags.Check(FifamBeg::WithReplay)) ? 2 : 1;
            if (comp->AsCup()->mFirstSeasonMatchdays.size() != numMatches || comp->AsCup()->mSecondSeasonMatchdays.size() != numMatches)
                Error(L"Incorrect calendar in cup %08X", compId.ToInt());
            break;
        case FifamCompDbType::Round:
            numMatches = comp->AsRound()->m1stLegFlags.Check(FifamBeg::_2ndLeg) ? 2 : 1;
            if (comp->AsRound()->mFirstSeasonMatchdays[0] == 0 || comp->AsRound()->mSecondSeasonMatchdays[0] == 0)
                Error(L"Incorrect calendar in round %08X", compId.ToInt());
            else if (numMatches == 2 && (comp->AsRound()->mFirstSeasonMatchdays[1] == 0 || comp->AsRound()->mSecondSeasonMatchdays[1] == 0))
                Error(L"Incorrect calendar in round %08X", compId.ToInt());
            break;
        }
    }
}
