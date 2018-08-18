#pragma once

#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class RulesTest {
public:
    RulesTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();
        for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
            FifamContinent continent;
            continent.SetFromInt(i);
            std::wcout << continent.ToStr() << std::endl;
            
            std::wcout << L"  " << FifamTr(db->mRules.mContinentalCupNames[i].mFirstCup) << L": " <<
                db->mRules.mContinentalCupChampions[i].mFirstCup.GetTeamName() << L" (" <<
                db->mRules.mContinentalCupStadiums[i].mFirstCup.GetTeamName() << L")" << std::endl;

            std::wcout << L"  " << FifamTr(db->mRules.mContinentalCupNames[i].mSecondCup) << L": " <<
                db->mRules.mContinentalCupChampions[i].mSecondCup.GetTeamName() << L" (" <<
                db->mRules.mContinentalCupStadiums[i].mSecondCup.GetTeamName() << L")" << std::endl;

            if (i == 0) {
                std::wcout << L"  " << FifamTr(db->mRules.mEuropeanSuperCupName) << L": " <<
                    db->mRules.mContinentalCupChampions[i].mSuperCup.GetTeamName() << L" (" <<
                    db->mRules.mContinentalCupStadiums[i].mSuperCup.GetTeamName() << L")" << std::endl;
            }
        }
        std::wcout << L"Fairness teams: ";
        for (UInt i = 0; i < 3; i++) {
            if (i != 0)
                std::wcout << L",";
            std::wcout << db->mRules.mFairnessAwardWinners[i].GetTeamName();
        }
        std::wcout << std::endl;
        std::wcout << L"Points for home win: " << db->mRules.mPointsForHomeWin << std::endl;
        std::wcout << L"Points for away win: " << db->mRules.mPointsForAwayWin << std::endl;
        std::wcout << L"Bonus point for win against champion: " << db->mRules.mBonusPointForWinAgainstChampion << std::endl;
        std::wcout << L"Bonus point for more than 3 goals: " << db->mRules.mBonusPointForMoreThan3Goals << std::endl;
        std::wcout << L"Unknown bonus: " << db->mRules.Unknown._3 << std::endl;
        std::wcout << L"Num subs in friendly matches: " << db->mRules.mNumSubsInFriendlyMatches << std::endl;
        std::wcout << L"International friendlies first season: ";
        for (UInt i = 0; i < 16; i++) {
            if (i != 0)
                std::wcout << L",";
            std::wcout << db->mRules.mInternationalFriendliesFirstSeason[i];
        }
        std::wcout << std::endl;
        std::wcout << L"International friendlies second season: ";
        for (UInt i = 0; i < 16; i++) {
            if (i != 0)
                std::wcout << L",";
            std::wcout << db->mRules.mInternationalFriendliesSecondSeason[i];
        }
        std::wcout << std::endl;
        std::wcout << L"Unknown teams: " << db->mRules.Unknown._1.GetTeamName() << L"," << db->mRules.Unknown._2.GetTeamName();
        std::wcout << std::endl;
    }
};
