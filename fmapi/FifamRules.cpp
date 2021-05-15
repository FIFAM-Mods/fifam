#include "FifamRules.h"
#include "FifamUtils.h"
#include "FifamDbWriteable.h"
#include "FifamNames.h"

void FifamRules::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"RULES_AND_MISC")) {
        if (reader.ReadVersion()) {
            if (!reader.IsVersionGreaterOrEqual(0x2005, 0x05)) {
                reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mFirstCup);
                reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mSecondCup);
                if (reader.IsVersionGreaterOrEqual(0x2004, 0x00)) {
                    reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup);
                    if (reader.IsVersionGreaterOrEqual(0x2004, 0x04))
                        reader.ReadLineTranslationArray(mEuropeanSuperCupName);
                }
                if (reader.IsVersionGreaterOrEqual(0x2004, 0x07)) {
                    reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mFirstCupDup);
                    reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mSecondCupDup);
                    reader.ReadLineTranslationArray(mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCupDup);
                    reader.ReadLineTranslationArray(mEuropeanSuperCupNameDup);
                }
                else {
                    mContinentalCupNames[FifamContinent::Europe].mFirstCupDup = mContinentalCupNames[FifamContinent::Europe].mFirstCup;
                    mContinentalCupNames[FifamContinent::Europe].mSecondCupDup = FifamNames::TransformTrArray(mContinentalCupNames[FifamContinent::Europe].mSecondCup, FifamNames::LimitName, 17);
                    mEuropeanSuperCupNameDup = FifamNames::TransformTrArray(mEuropeanSuperCupName, FifamNames::LimitName, 17);
                }
                FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[FifamContinent::Europe].mFirstCup, reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[FifamContinent::Europe].mSecondCup, reader.ReadLine<UInt>());
                if (reader.IsVersionGreaterOrEqual(0x2004, 0x00))
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[FifamContinent::SouthAmerica].mFirstCup, reader.ReadLine<UInt>());
                if (reader.GetGameId() >= 5) {
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[FifamContinent::Europe].mFirstCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[FifamContinent::Europe].mSecondCup, reader.ReadLine<UInt>());
                }
                else {
                    FifamUtils::SaveStadiumIDToPtr(mEuropeCup1Stadium, reader.ReadLine<UInt>());
                    FifamUtils::SaveStadiumIDToPtr(mEuropeCup2Stadium, reader.ReadLine<UInt>());
                }
                if (reader.IsVersionGreaterOrEqual(0x2004, 0x00)) {
                    if (reader.GetGameId() >= 5)
                        FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[FifamContinent::SouthAmerica].mFirstCup, reader.ReadLine<UInt>());
                    else
                        FifamUtils::SaveStadiumIDToPtr(mSouthAmericaCup1Stadium, reader.ReadLine<UInt>());
                }
            }
            else {
                for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                    reader.ReadLineTranslationArray(mContinentalCupNames[i].mFirstCup);
                    reader.ReadLineTranslationArray(mContinentalCupNames[i].mSecondCup);
                }
                reader.ReadLineTranslationArray(mEuropeanSuperCupName);
                if (reader.GetGameId() <= 7) {
                    for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                        reader.ReadLineTranslationArray(mContinentalCupNames[i].mFirstCupDup);
                        reader.ReadLineTranslationArray(mContinentalCupNames[i].mSecondCupDup);
                    }
                    reader.ReadLineTranslationArray(mEuropeanSuperCupNameDup);
                }
                else {
                    reader.SkipLines(13);
                    for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                        mContinentalCupNames[i].mFirstCupDup = mContinentalCupNames[i].mFirstCup;
                        mContinentalCupNames[i].mSecondCupDup = FifamNames::TransformTrArray(mContinentalCupNames[i].mSecondCup, FifamNames::LimitName, 17);
                    }
                    mEuropeanSuperCupNameDup = FifamNames::TransformTrArray(mEuropeanSuperCupName, FifamNames::LimitName, 17);
                }
                for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[i].mFirstCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[i].mFirstCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[i].mSecondCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[i].mSecondCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupChampions[i].mSuperCup, reader.ReadLine<UInt>());
                    FifamUtils::SaveClubIDToClubLink(mContinentalCupStadiums[i].mSuperCup, reader.ReadLine<UInt>());
                }
            }
            FifamUtils::SaveClubIDToClubLink(Unknown._1, reader.ReadLine<UInt>());
            FifamUtils::SaveClubIDToClubLink(Unknown._2, reader.ReadLine<UInt>());
            UInt pointsForHomeWin = reader.ReadLine<UInt>();
            if (pointsForHomeWin == 0)
                mPointsForHomeWin = 2;
            else if (pointsForHomeWin == 2)
                mPointsForHomeWin = 4;
            else
                mPointsForHomeWin = 3;
            UInt pointsForAwayWin = reader.ReadLine<UInt>();
            if (pointsForAwayWin == 0)
                mPointsForAwayWin = 2;
            else if (pointsForAwayWin == 2)
                mPointsForAwayWin = 4;
            else
                mPointsForAwayWin = 3;
            mBonusPointForWinAgainstChampion = reader.ReadLine<UInt>() == 0;
            mBonusPointForMoreThan3Goals = reader.ReadLine<UInt>() == 0;
            Unknown._3 = reader.ReadLine<UInt>() == 0;
            mNumSubsInFriendlyMatches = reader.ReadLine<UInt>();
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x0F)) {
                mFairnessAwardWinners.resize(3);
                for (UInt i = 0; i < 3; i++)
                    FifamUtils::SaveClubIDToClubLink(mFairnessAwardWinners[i], reader.ReadLine<UInt>());
            }
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x07)) {
                for (UInt i = 0; i < 16; i++) {
                    mInternationalFriendliesFirstSeason[i] = reader.ReadLine<UShort>();
                    mInternationalFriendliesSecondSeason[i] = reader.ReadLine<UShort>();
                }
            }
        }
        reader.ReadEndIndex(L"RULES_AND_MISC");
    }
}

void FifamRules::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"RULES_AND_MISC");
    writer.WriteVersion();
    if (!writer.IsVersionGreaterOrEqual(0x2005, 0x05)) {
        writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mFirstCup);
        writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mSecondCup);
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x00)) {
            writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup);
            if (writer.IsVersionGreaterOrEqual(0x2004, 0x04))
                writer.WriteLineTranslationArray(mEuropeanSuperCupName);
        }
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x07)) {
            writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mFirstCupDup);
            writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::Europe].mSecondCupDup);
            writer.WriteLineTranslationArray(mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCupDup);
            writer.WriteLineTranslationArray(mEuropeanSuperCupNameDup);
        }
        writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[FifamContinent::Europe].mFirstCup));
        writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[FifamContinent::Europe].mSecondCup));
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x00))
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[FifamContinent::SouthAmerica].mFirstCup));
        if (writer.GetGameId() >= 5) {
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[FifamContinent::Europe].mFirstCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[FifamContinent::Europe].mSecondCup));
        }
        else {
            writer.WriteLine(FifamUtils::GetWriteableID(mEuropeCup1Stadium));
            writer.WriteLine(FifamUtils::GetWriteableID(mEuropeCup2Stadium));
        }
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x00)) {
            if (writer.GetGameId() >= 5)
                writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[FifamContinent::SouthAmerica].mFirstCup));
            else
                writer.WriteLine(FifamUtils::GetWriteableID(mSouthAmericaCup1Stadium));
        }
    }
    else {
        if (writer.GetGameId() <= 7) {
            for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                writer.WriteLineTranslationArray(mContinentalCupNames[i].mFirstCupDup);
                writer.WriteLineTranslationArray(mContinentalCupNames[i].mSecondCupDup);
            }
            writer.WriteLineTranslationArray(mEuropeanSuperCupNameDup);
        }
        else {
            for (UInt i = 0; i < 2; i++) {
                for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
                    writer.WriteLineTranslationArray(mContinentalCupNames[i].mFirstCup);
                    writer.WriteLineTranslationArray(mContinentalCupNames[i].mSecondCup);
                }
                writer.WriteLineTranslationArray(mEuropeanSuperCupName);
            }
        }
        for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[i].mFirstCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[i].mFirstCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[i].mSecondCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[i].mSecondCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupChampions[i].mSuperCup));
            writer.WriteLine(FifamUtils::GetWriteableID(mContinentalCupStadiums[i].mSuperCup));
        }
    }
    writer.WriteLine(FifamUtils::GetWriteableID(Unknown._1));
    writer.WriteLine(FifamUtils::GetWriteableID(Unknown._2));
    UInt pointsForHomeWin = 1;
    if (mPointsForHomeWin == 2)
        pointsForHomeWin = 0;
    else if (mPointsForHomeWin == 4)
        pointsForHomeWin = 2;
    writer.WriteLine(pointsForHomeWin);
    UInt pointsForAwayWin = 1;
    if (mPointsForAwayWin == 2)
        pointsForAwayWin = 0;
    else if (mPointsForAwayWin == 4)
        pointsForAwayWin = 2;
    writer.WriteLine(pointsForAwayWin);
    writer.WriteLine(mBonusPointForWinAgainstChampion == 0);
    writer.WriteLine(mBonusPointForMoreThan3Goals == 0);
    writer.WriteLine(Unknown._3 == 0);
    writer.WriteLine(mNumSubsInFriendlyMatches);
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x0F)) {
        auto fairnessAwardWinners = FifamUtils::MakeWriteableIDsList(mFairnessAwardWinners);
        for (UInt i = 0; i < 3; i++)
            writer.WriteLine(fairnessAwardWinners[i]);
    }
    if (writer.IsVersionGreaterOrEqual(0x2009, 0x07)) {
        for (UInt i = 0; i < 16; i++) {
            writer.WriteLine(mInternationalFriendliesFirstSeason[i]);
            writer.WriteLine(mInternationalFriendliesSecondSeason[i]);
        }
    }
    writer.WriteEndIndex(L"RULES_AND_MISC");
}
