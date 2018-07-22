#include "FifamCountry.h"
#include "FifamDatabase.h"
#include "Error.h"

FifamCountry::FifamCountry(UInt id, FifamDatabase *db) {
    mId = id;
    mDatabase = db;
}

FifamReferee *FifamCountry::AddReferee() {
    FifamReferee *referee = new FifamReferee;
    mReferees.push_back(referee);
    if (mDatabase)
        mDatabase->mReferees.insert(referee);
    return referee;
}

FifamStadium *FifamCountry::AddStadium() {
    FifamStadium *stadium = new FifamStadium;
    mStadiums.push_back(stadium);
    if (mDatabase)
        mDatabase->mStadiums.insert(stadium);
    return stadium;
}

FifamSponsor *FifamCountry::AddSponsor() {
    FifamSponsor *sponsor = new FifamSponsor;
    mSponsors.push_back(sponsor);
    if (mDatabase)
        mDatabase->mSponsors.insert(sponsor);
    return sponsor;
}

void FifamCountry::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"COUNTRY")) {

        reader.ReadVersion();

        if (reader.GetGameId() < 11) {
            if (reader.ReadStartIndex(L"COMPETITIONPART"))
                reader.ReadEndIndex(L"COMPETITIONPART");
        }

        if (reader.ReadStartIndex(L"COMPETITION")) {
            // TODO
            reader.ReadEndIndex(L"COMPETITION");
        }

        if (reader.ReadStartIndex(L"CLUBS")) {
            // TODO
            reader.ReadEndIndex(L"CLUBS");
        }

        if (reader.ReadStartIndex(L"REFEREES")) {
            auto numReferees = reader.ReadLine<UInt>();
            for (UInt i = 0; i < numReferees; i++)
                AddReferee()->Read(reader);
            reader.ReadEndIndex(L"REFEREES");
        }

        if (reader.GetGameId() < 11) {
            if (reader.ReadStartIndex(L"SPONSORS")) {
                auto numStadiums = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numStadiums; i++)
                    AddSponsor()->Read(reader);
                reader.ReadEndIndex(L"SPONSORS");
            }
        }

        if (reader.ReadStartIndex(L"STAFFS"))
            reader.ReadEndIndex(L"STAFFS");

        if (reader.GetGameId() < 11) {
            if (reader.ReadStartIndex(L"STADIUMS")) {
                auto numStadiums = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numStadiums; i++)
                    AddStadium()->Read(reader);
                reader.ReadEndIndex(L"STADIUMS");
            }
        }

        if (reader.ReadStartIndex(L"ADDMANAGER")) {
            // TODO
            reader.ReadEndIndex(L"ADDMANAGER");
        }

        if (reader.ReadStartIndex(L"PLAYERPOOL")) {
            // TODO
            reader.ReadEndIndex(L"PLAYERPOOL");
        }

        String countryMiscSectionName;
        if (reader.GetGameId() < 11)
            countryMiscSectionName = L"COUNTRY_MIST";
        else
            countryMiscSectionName = L"COUNTRY_MISC";

        if (reader.ReadStartIndex(countryMiscSectionName)) {
            if (reader.GetGameId() < 11) {
                reader.ReadLine<String>();
                reader.ReadLine<String>();
                reader.ReadLine<String>();
                reader.ReadLine(mOriginalLeagueSystem);
            }
            reader.ReadLine(mFA_Name);
            reader.ReadLineArray(mLanguages);
            reader.ReadLineArray(mLanguageFrequency);
            reader.ReadLine(mLeagueLevelWithReserveTeams);
            reader.ReadLine(Unknown._1);
            if (reader.GetGameId() < 11) {
                reader.ReadLine(mFA_PresidentFirstName);
                reader.ReadLine(mFA_PresidentLastName);
                reader.ReadLine(mFA_PresidentBirthDate);
            }
            if (!reader.IsVersionGreaterOrEqual(0x2007, 0x17)) {
                reader.ReadLine(Unknown._2);
                reader.ReadLine(Unknown._3);
                reader.ReadLine(Unknown._4);
                reader.ReadLine(Unknown._5);
                reader.ReadLine(Unknown._6);
                reader.ReadLine(Unknown._7);
            }
            UChar yelCardsRule;
            reader.ReadLine(yelCardsRule);
            mYellowCardsLeagueRule.SetFromInt(yelCardsRule & 0x7F);
            mYellowCardsLeagueAfterYelRed = (yelCardsRule & 0x80) == 0x80;
            reader.ReadLine(yelCardsRule);
            mYellowCardsCupRule.SetFromInt(yelCardsRule & 0x7F);
            mYellowCardsCupAfterYelRed = (yelCardsRule & 0x80) == 0x80;
            UChar redCardOptions = reader.ReadLine<UChar>();
            if (redCardOptions & 1)
                Unknown._8 = true;
            if (redCardOptions & 2)
                Unknown._9 = true;
            if (redCardOptions & 16)
                mRedCardBanFixedDuration = true;
            reader.ReadLine(mRedCardRule);
            UChar validForAllCompsFlags = reader.ReadLine<UChar>();
            if (reader.GetGameId() < 8) {
                if (validForAllCompsFlags & 1)
                    mYellowCardsBanForAllComp = true;
                if (validForAllCompsFlags & 2)
                    mSecondYellowCardBanForAllComp = true;
            }
            if (validForAllCompsFlags & 4)
                mRedCardsBanForAllComp = true;
            UChar secondYellowCardLeaguesState = reader.ReadLine<UChar>();
            if (secondYellowCardLeaguesState & 1)
                mSecondYellowCardLeagueState[0] = true;
            if (secondYellowCardLeaguesState & 2)
                mSecondYellowCardLeagueState[1] = true;
            if (secondYellowCardLeaguesState & 4)
                mSecondYellowCardLeagueState[2] = true;
            if (secondYellowCardLeaguesState & 8)
                mSecondYellowCardLeagueState[3] = true;
            if (secondYellowCardLeaguesState & 16)
                mSecondYellowCardLeagueState[4] = true;
            UInt numLeagueLevels = reader.IsVersionGreaterOrEqual(0x2007, 0x13) ? 16 : 5;
            for (UInt i = 0; i < numLeagueLevels; i++) {
                reader.ReadLine(mLeagueLevels[i].mEqualPointsSorting);
                reader.ReadLine(mLeagueLevels[i].mNumNonEUPlayersAllowed);
                reader.ReadLine(mLeagueLevels[i].mNumRelegatedTeams);
                reader.ReadLine(mLeagueLevels[i].mRating);
            }
            reader.ReadLine(mCupSystemType);
            UChar cupOptions = reader.ReadLine<UChar>();
            if (cupOptions & 1)
                mCupHomeAdvantageForTeamsInLowerLeagues = true;
            if (cupOptions & 2)
                mCupCanPlayersBeCupTied = true;
            reader.ReadLine(mCupFinalStadium);
            reader.ReadLine(mSuperCupType);
            UInt leagueCupType = reader.ReadLine<UInt>();
            mLeagueCupSystemType[0].SetFromInt(leagueCupType & 0xFF);
            mLeagueCupSystemType[1].SetFromInt((leagueCupType >> 8) & 0xFF);
            mLeagueCupSystemType[2].SetFromInt((leagueCupType >> 16) & 0xFF);
            mLeagueCupSystemType[3].SetFromInt((leagueCupType >> 24) & 0xFF);
            reader.ReadLine(mYearsForNaturalization);
            reader.ReadLine(Unknown._10[0]);
            reader.ReadLine(Unknown._10[1]);
            reader.ReadLine(Unknown._11);
            reader.ReadLine(Unknown._12);
            reader.ReadLine(Unknown._13);
            reader.ReadLine(Unknown._14);
            reader.ReadLine(mFlags);
            reader.ReadLine(Unknown._15);
            reader.ReadLine(mAmateurRule);
            reader.ReadLine(mClimate);
            for (UInt i = 0; i < 6; i++)
                reader.ReadLine(mAssessmentInfo[i]);
            reader.ReadLine(mProbabilityForOverseasTransfers);
            reader.ReadLine(Unknown._16);
            reader.ReadLine(Unknown._17);
            if (reader.GetGameId() < 11)
                mManager.Read(reader);
            reader.ReadLine(mAveragePlayerRating);
            reader.ReadLine(Unknown._18);
            reader.ReadLine(mAppearanceHairColor);
            reader.ReadLine(mAppearanceHairStyle[0]);
            reader.ReadLine(mAppearanceHairStyle[1]);
            reader.ReadLine(mAppearanceBeard);
            for (UInt i = 0; i < 4; i++)
                reader.ReadLine(mAppearanceFaceType[i]);
            reader.ReadLine(mAverageHeight);
            reader.ReadLine(mAverageWeight);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x01)) {
                UInt numAppearanceDistributions = reader.ReadLine<UInt>();
                mAppearanceData.resize(numAppearanceDistributions);
                for (UInt i = 0; i < numAppearanceDistributions; i++) {
                    reader.ReadLine(mAppearanceData[i].mAppearanceType);
                    reader.ReadLine(mAppearanceData[i].mDistributionProbability);
                }
            }
            if (reader.IsVersionGreaterOrEqual(0x2007, 0xEu)) {
                reader.ReadLine(mPreferredTransfersTerritory);
                reader.ReadLine(mFifaRanking);
            }
            if (reader.GetGameId() < 11)
                reader.ReadLine(Unknown._19);
            reader.ReadLine(mNumContinentalChampions);
            reader.ReadLine(mNumWorldCups);
            reader.ReadLine(mNumContinentalRunnersUp);
            reader.ReadLine(mNumWorldCupRunnersUp);
            reader.ReadLine(mNotes);

            CheckEnum(mYellowCardsLeagueRule);
            CheckEnum(mYellowCardsCupRule);
            CheckEnum(mRedCardRule);
            if (validForAllCompsFlags >= 8)
                Error("validForAllCompsFlags: unknown value (%u)", validForAllCompsFlags);
            if (secondYellowCardLeaguesState >= 32)
                Error("secondYellowCardLeaguesState: unknown value (%u)", secondYellowCardLeaguesState);
            if ((redCardOptions & 4) || (redCardOptions & 8) || (redCardOptions >= 32))
                Error("redCardOptions: unknown value (%u)", redCardOptions);
            for (UInt i = 0; i < numLeagueLevels; i++)
                CheckEnum(mLeagueLevels[i].mEqualPointsSorting);
            CheckEnum(mCupSystemType);
            if (cupOptions >= 4)
                Error("cupOptions: unknown value (%u)", cupOptions);
            CheckEnum(mSuperCupType);
            for (UInt i = 0; i < 4; i++)
                CheckEnum(mLeagueCupSystemType[i]);
            CheckEnum(mAmateurRule);
            CheckEnum(mClimate);
            CheckEnum(mPreferredTransfersTerritory);

            reader.ReadEndIndex(countryMiscSectionName);
        }

        reader.ReadEndIndex(L"COUNTRY");
    }
}

void FifamCountry::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"COUNTRY");
    writer.WriteVersion();
    if (writer.GetGameId() < 11) {
        writer.WriteStartIndex(L"COMPETITIONPART");
        // TODO
        writer.WriteEndIndex(L"COMPETITIONPART");
    }
    writer.WriteStartIndex(L"COMPETITION");
    // TODO
    writer.WriteEndIndex(L"COMPETITION");

    writer.WriteStartIndex(L"CLUBS");
    // TODO
    writer.WriteEndIndex(L"CLUBS");

    writer.WriteStartIndex(L"REFEREES");
    writer.WriteLine(mReferees.size());
    for (auto referee : mReferees)
        referee->Write(writer);
    writer.WriteEndIndex(L"REFEREES");

    if (writer.GetGameId() < 11) {
        writer.WriteStartIndex(L"SPONSORS");
        writer.WriteLine(mSponsors.size());
        for (auto sponsor : mSponsors)
            sponsor->Write(writer);
        writer.WriteEndIndex(L"SPONSORS");
    }

    writer.WriteStartIndex(L"STAFFS");
    // TODO
    writer.WriteEndIndex(L"STAFFS");

    if (writer.GetGameId() < 11) {
        writer.WriteStartIndex(L"STADIUMS");
        writer.WriteLine(mStadiums.size());
        for (auto stadium : mStadiums)
            stadium->Write(writer);
        writer.WriteEndIndex(L"STADIUMS");
    }

    writer.WriteStartIndex(L"ADDMANAGER");
    // TODO
    writer.WriteEndIndex(L"ADDMANAGER");

    writer.WriteStartIndex(L"PLAYERPOOL");
    // TODO
    writer.WriteEndIndex(L"PLAYERPOOL");

    String countryMiscSectionName;
    if (writer.GetGameId() < 11)
        countryMiscSectionName = L"COUNTRY_MIST";
    else
        countryMiscSectionName = L"COUNTRY_MISC";

    writer.WriteStartIndex(countryMiscSectionName);
    if (writer.GetGameId() < 11) {
        writer.WriteLine(Tr(mName));
        writer.WriteLine(Tr(mAbbr));
        writer.WriteLine((mContinent.ToInt() & 7) | (mTerritory.ToInt() << 3));
        writer.WriteLine(mOriginalLeagueSystem);
    }
    writer.WriteLine(mFA_Name);
    writer.WriteLineArray(mLanguages);
    writer.WriteLineArray(mLanguageFrequency);
    writer.WriteLine(mLeagueLevelWithReserveTeams);
    writer.WriteLine(Unknown._1);
    if (writer.GetGameId() < 11) {
        writer.WriteLine(mFA_PresidentFirstName);
        writer.WriteLine(mFA_PresidentLastName);
        writer.WriteLine(mFA_PresidentBirthDate);
    }
    if (!writer.IsVersionGreaterOrEqual(0x2007, 0x17)) {
        writer.WriteLine(Unknown._2);
        writer.WriteLine(Unknown._3);
        writer.WriteLine(Unknown._4);
        writer.WriteLine(Unknown._5);
        writer.WriteLine(Unknown._6);
        writer.WriteLine(Unknown._7);
    }
    UChar yelCardsRule = mYellowCardsLeagueRule.ToInt() & 0x7F;
    if (mYellowCardsLeagueAfterYelRed)
        yelCardsRule |= 0x80;
    writer.WriteLine(yelCardsRule);
    yelCardsRule = mYellowCardsCupRule.ToInt() & 0x7F;
    if (mYellowCardsCupAfterYelRed)
        yelCardsRule |= 0x80;
    writer.WriteLine(yelCardsRule);
    UChar redCardOptions = 0;
    if (Unknown._8)
        redCardOptions |= 1;
    if (Unknown._9)
        redCardOptions |= 2;
    if (mRedCardBanFixedDuration)
        redCardOptions |= 16;
    writer.WriteLine(redCardOptions);
    writer.WriteLine(mRedCardRule);
    UChar validForAllCompsFlags = 0;
    if (writer.GetGameId() < 8) {
        if (mYellowCardsBanForAllComp)
            validForAllCompsFlags |= 1;
        if (mSecondYellowCardBanForAllComp)
            validForAllCompsFlags |= 2;
    }
    if (mRedCardsBanForAllComp)
        validForAllCompsFlags |= 4;
    writer.WriteLine(validForAllCompsFlags);
    UChar secondYellowCardLeaguesState = 0;
    if (mSecondYellowCardLeagueState[0])
        secondYellowCardLeaguesState |= 1;
    if (mSecondYellowCardLeagueState[1])
        secondYellowCardLeaguesState |= 2;
    if (mSecondYellowCardLeagueState[2])
        secondYellowCardLeaguesState |= 4;
    if (mSecondYellowCardLeagueState[3])
        secondYellowCardLeaguesState |= 8;
    if (mSecondYellowCardLeagueState[4])
        secondYellowCardLeaguesState |= 16;
    writer.WriteLine(secondYellowCardLeaguesState);
    UInt numLeagueLevels = writer.IsVersionGreaterOrEqual(0x2007, 0x13) ? 16 : 5;
    for (UInt i = 0; i < numLeagueLevels; i++) {
        writer.WriteLine(mLeagueLevels[i].mEqualPointsSorting);
        writer.WriteLine(mLeagueLevels[i].mNumNonEUPlayersAllowed);
        writer.WriteLine(mLeagueLevels[i].mNumRelegatedTeams);
        writer.WriteLine(mLeagueLevels[i].mRating);
    }
    writer.WriteLine(mCupSystemType);
    UChar cupOptions = 0;
    if (mCupHomeAdvantageForTeamsInLowerLeagues)
        cupOptions |= 1;
    if (mCupCanPlayersBeCupTied)
        cupOptions |= 2;
    writer.WriteLine(cupOptions);
    writer.WriteLine(mCupFinalStadium);
    writer.WriteLine(mSuperCupType);
    writer.WriteLine(mLeagueCupSystemType[0].ToInt() | (mLeagueCupSystemType[1].ToInt() << 8)
        | (mLeagueCupSystemType[2].ToInt() << 16) | (mLeagueCupSystemType[3].ToInt() << 24));
    writer.WriteLine(mYearsForNaturalization);
    writer.WriteLine(Unknown._10[0]);
    writer.WriteLine(Unknown._10[1]);
    writer.WriteLine(Unknown._11);
    writer.WriteLine(Unknown._12);
    writer.WriteLine(Unknown._13);
    writer.WriteLine(Unknown._14);
    writer.WriteLine(mFlags);
    writer.WriteLine(Unknown._15);
    writer.WriteLine(mAmateurRule);
    writer.WriteLine(mClimate);
    for (UInt i = 0; i < 6; i++)
        writer.WriteLine(mAssessmentInfo[i]);
    writer.WriteLine(mProbabilityForOverseasTransfers);
    writer.WriteLine(Unknown._16);
    writer.WriteLine(Unknown._17);
    if (writer.GetGameId() < 11)
        mManager.Write(writer);
    writer.WriteLine(mAveragePlayerRating);
    writer.WriteLine(Unknown._18);
    writer.WriteLine(mAppearanceHairColor);
    writer.WriteLine(mAppearanceHairStyle[0]);
    writer.WriteLine(mAppearanceHairStyle[1]);
    writer.WriteLine(mAppearanceBeard);
    for (UInt i = 0; i < 4; i++)
        writer.WriteLine(mAppearanceFaceType[i]);
    writer.WriteLine(mAverageHeight);
    writer.WriteLine(mAverageWeight);
    if (writer.IsVersionGreaterOrEqual(0x2011, 0x01)) {
        writer.WriteLine(mAppearanceData.size());
        for (auto const &appearance : mAppearanceData) {
            writer.WriteLine(appearance.mAppearanceType);
            writer.WriteLine(appearance.mDistributionProbability);
        }
    }
    if (writer.IsVersionGreaterOrEqual(0x2007, 0xEu)) {
        writer.WriteLine(mPreferredTransfersTerritory);
        writer.WriteLine(mFifaRanking);
    }
    if (writer.GetGameId() < 11)
        writer.WriteLine(Unknown._19);
    writer.WriteLine(mNumContinentalChampions);
    writer.WriteLine(mNumWorldCups);
    writer.WriteLine(mNumContinentalRunnersUp);
    writer.WriteLine(mNumWorldCupRunnersUp);
    writer.WriteLine(mNotes);
    writer.WriteEndIndex(countryMiscSectionName);
    writer.WriteEndIndex(L"COUNTRY");
}

void FifamCountry::ReadFixtures(FifamReader &reader) {

}

void FifamCountry::ReadScript(FifamReader &reader) {

}
