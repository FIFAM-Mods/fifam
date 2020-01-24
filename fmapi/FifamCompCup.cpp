#include "FifamCompCup.h"
#include "FifamDatabase.h"

FifamCompDbType FifamCompCup::GetDbType() const {
    return FifamCompDbType::Cup;
}

void FifamCompCup::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
    reader.ReadLineTranslationArray(mName);
    if (reader.IsVersionGreaterOrEqual(0, 0x1)) {
        reader.ReadLine(mCupTemplate);
        reader.ReadLine(mDrawPeriodInWeeks);
        reader.SkipLine();
    }
    if (reader.GetGameId() >= 11) {
        UInt numMatchdays = 0;
        UInt numRounds = 0;
        reader.ReadLine(mNumTeams, numRounds, numMatchdays);
        mFirstSeasonMatchdays.resize(numMatchdays);
        mSecondSeasonMatchdays.resize(numMatchdays);
        if (reader.ReadStartIndex(L"MATCHDAYS")) {
            reader.ReadLineArray(mFirstSeasonMatchdays);
            reader.ReadLineArray(mSecondSeasonMatchdays);
            reader.ReadEndIndex(L"MATCHDAYS");
        }
        if (reader.ReadStartIndex(L"ROUNDINFO")) {
            for (UInt i = 0; i < numRounds; i++) {
                Round &round = mRounds.emplace_back();
                UInt dummy;
                String flags;
                reader.ReadLine(dummy, round.mRoundID, round.mTeamsRound, round.mNewTeamsRound, round.mStartBeg, round.mEndBeg, flags,
                    round.mBonuses[0], round.mBonuses[1], round.mBonuses[2], round.mBonuses[3]);
                round.mFlags.SetFromStr(flags, L'|');
            }
            reader.ReadEndIndex(L"ROUNDINFO");
        }
    }
    else {
        if (mCupTemplate == FifamCupSystemType::None || !database)
            return;
        auto cupTemplate = database->GetCupTemplate(mCupTemplate);
        if (!cupTemplate)
            return;
        UInt numRounds = cupTemplate->mRounds.size();
        mFirstSeasonMatchdays.resize(numRounds * 2);
        mSecondSeasonMatchdays.resize(numRounds * 2);
        if (reader.ReadStartIndex(L"MATCHDAYS")) {
            reader.ReadLineArray(mFirstSeasonMatchdays);
            Utils::Remove(mFirstSeasonMatchdays, 0);
            reader.ReadEndIndex(L"MATCHDAYS");
        }
        if (reader.ReadStartIndex(L"MATCHDAYS2")) {
            reader.ReadLineArray(mSecondSeasonMatchdays);
            Utils::Remove(mSecondSeasonMatchdays, 0);
            reader.ReadEndIndex(L"MATCHDAYS2");
        }
        else
            mSecondSeasonMatchdays = mFirstSeasonMatchdays;
        mRounds.resize(numRounds);
        for (UInt i = 0; i < numRounds; i++) {
            reader.ReadLine(mRounds[i].mBonuses[3]);
            reader.ReadLine(mRounds[i].mBonuses[1]);
            reader.ReadLine(mRounds[i].mBonuses[2]);
            reader.ReadLine(mRounds[i].mBonuses[0]);
            mRounds[i].mTeamsRound = cupTemplate->mRounds[i]->mTeamsRound;
            mRounds[i].mNewTeamsRound = cupTemplate->mRounds[i]->mNewTeamsRound;
            mRounds[i].mStartBeg = cupTemplate->mRounds[i]->mStartBeg;
            mRounds[i].mEndBeg = cupTemplate->mRounds[i]->mEndBeg;
            mRounds[i].mFlags = cupTemplate->mRounds[i]->mFlags;
            mRounds[i].mRoundID = cupTemplate->mRounds[i]->mRoundID;
        }
        mInstructions = cupTemplate->mInstructions;
        mInstructions.ForAllCompetitionLinks([=](FifamCompID & competition, UInt, FifamAbstractInstruction *instruction) {
            if (competition.mRegion == FifamCompRegion::None)
                competition.mRegion.SetFromInt(nationId.ToInt());
        });
    }
    FifamCompetition::Read(reader, database, nationId);
}

void FifamCompCup::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
    if (!writer.IsVersionGreaterOrEqual(0, 0x01))
        writer.WriteLine(FifamTr(mName));
    else
        writer.WriteLineTranslationArray(mName);
    if (writer.IsVersionGreaterOrEqual(0, 0x1)) {
        if (writer.GetGameId() >= 11)
            writer.WriteLine(0);
        else {
            if (mCupTemplate == FifamCupSystemType::None)
                writer.WriteLine(FifamCompCup::DetectCupSystemType(this));
            else
                writer.WriteLine(mCupTemplate);
        }
        writer.WriteLine(mDrawPeriodInWeeks);
        if (writer.GetGameId() >= 11)
            writer.WriteLine(mRounds.size());
        else
            writer.WriteLine(0);
    }
    if (writer.GetGameId() >= 11) {
        writer.WriteLine(mNumTeams, mRounds.size(), mFirstSeasonMatchdays.size());
        writer.WriteStartIndex(L"MATCHDAYS");
        writer.WriteLineArray(mFirstSeasonMatchdays);
        writer.WriteLineArray(mSecondSeasonMatchdays);
        writer.WriteEndIndex(L"MATCHDAYS");
        writer.WriteStartIndex(L"ROUNDINFO");
        for (UInt i = 0; i < mRounds.size(); i++) {
            Round const &round = mRounds[i];
            writer.WriteLine(i, round.mRoundID, round.mTeamsRound, round.mNewTeamsRound, round.mStartBeg, round.mEndBeg,
                round.mFlags.ToStr(L"| "), round.mBonuses[0], round.mBonuses[1], round.mBonuses[2], round.mBonuses[3]);
        }
        writer.WriteEndIndex(L"ROUNDINFO");
    }
    else {
        writer.WriteStartIndex(L"MATCHDAYS");
        writer.WriteLineArray(mFirstSeasonMatchdays);
        writer.WriteEndIndex(L"MATCHDAYS");
        writer.WriteStartIndex(L"MATCHDAYS2");
        writer.WriteLineArray(mSecondSeasonMatchdays);
        writer.WriteEndIndex(L"MATCHDAYS2");
        for (UInt i = 0; i < mRounds.size(); i++) {
            writer.WriteLine(mRounds[i].mBonuses[3]);
            writer.WriteLine(mRounds[i].mBonuses[1]);
            writer.WriteLine(mRounds[i].mBonuses[2]);
            writer.WriteLine(mRounds[i].mBonuses[0]);
        }
    }
    FifamCompetition::Write(writer, database, nationId);
}

FifamCupSystemType FifamCompCup::DetectCupSystemType(FifamCompCup *cup) {
    UInt numMatches = 0;
    for (auto const &r : cup->mRounds)
        numMatches += ((r.mFlags.Check(FifamBeg::_2ndLeg) || r.mFlags.Check(FifamBeg::WithReplay)) ? 2 : 1);
    auto countryId = cup->mID.mRegion;
    auto compType = cup->mID.mType;
    auto compIndex = cup->mID.mIndex;
    switch (numMatches) {
    case 1:
        return FifamCupSystemType::Supercup1Leg;
    case 2:
        return FifamCupSystemType::Supercup2Leg;
    case 3:
        if (countryId == FifamCompRegion::Germany && compType == FifamCompType::LeagueCup)
            return FifamCupSystemType::LeagueCup_Germany;
        return FifamCupSystemType::FA_Cup_Small;
    case 5:
        return FifamCupSystemType::LeagueCup_Scotland;
    case 6:
        if (countryId == FifamCompRegion::France && compType == FifamCompType::FaCup)
            return FifamCupSystemType::FA_France;
        if (countryId == FifamCompRegion::France && compType == FifamCompType::LeagueCup)
            return FifamCupSystemType::LeagueCup_France;
        if (countryId == FifamCompRegion::England && compType == FifamCompType::LeagueCup && compIndex == 1)
            return FifamCupSystemType::LeaguecupDiv2And3;
        return FifamCupSystemType::FA_Germany;
    case 8:
        return FifamCupSystemType::LeagueCup_England;
    case 11:
        return FifamCupSystemType::FA_Italy;
    case 12:
        return FifamCupSystemType::FA_Spain;
    case 13:
        return FifamCupSystemType::FA_Scotland;
    case 14:
        return FifamCupSystemType::FA_England;
    }
    return FifamCupSystemType::None;
}
