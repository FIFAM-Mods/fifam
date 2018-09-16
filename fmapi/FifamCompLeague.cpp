#include "FifamCompLeague.h"
#include "FifamUtils.h"

FifamCompDbType FifamCompLeague::GetDbType() const {
    return FifamCompDbType::League;
}

void FifamCompLeague::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
    if (!reader.IsVersionGreaterOrEqual(0x2011, 0x07))
        reader.ReadLineTranslationArray(mName);
    reader.ReadLine(mNumTeams);
    reader.ReadLine(mLeagueLevel);
    auto relFlags = reader.ReadLineArray<String>(L',', true);
    for (auto const &flag : relFlags) {
        if (flag == L"REL_RULE_0")
            mNumRelegatedTeams = 0;
        else if (flag == L"REL_RULE_1")
            mNumRelegatedTeams = 1;
        else if (flag == L"REL_RULE_2")
            mNumRelegatedTeams = 2;
        else if (flag == L"REL_RULE_3")
            mNumRelegatedTeams = 3;
        else if (flag == L"REL_RULE_4")
            mNumRelegatedTeams = 4;
        else if (flag == L"REL_RULE_5")
            mNumRelegatedTeams = 5;
        else if (flag == L"REL_RULE_INDOOR")
            mIndoorRelegation = true;
        else if (flag == L"REL_RULE_RESERVE")
            mReserveRelegation = true;
        else if (flag == L"REL_RULE_TAKE_POINTS")
            mTakePoints = true;
    }
    reader.ReadLine(mEqualPointsSorting);
    reader.ReadLine(mNumRounds);
    mRoundType.SetFromStr(reader.ReadFullLine());
    reader.ReadLine(mAttendanceMp);
    reader.ReadLine(mTransferMarketMp);
    if (reader.ReadStartIndex(L"TEAMS")) {
        if (!reader.CheckLine(L"%INDEXEND%TEAMS")) {
            auto teamIDs = reader.ReadLineArray<String>();
            for (auto const &teamIDstr : teamIDs) {
                FifamClubLink &newLink = mTeams.emplace_back();
                FifamUtils::SaveClubIDToClubLink(newLink, Utils::SafeConvertInt<UInt>(teamIDstr, true));
            }
        }
        reader.ReadEndIndex(L"TEAMS");
    }
    if (reader.ReadStartIndex(L"MATCHDAYS")) {
        if (!reader.CheckLine(L"%INDEXEND%MATCHDAYS"))
            mFirstSeasonMatchdays = reader.ReadLineArray<UShort>();
        reader.ReadEndIndex(L"MATCHDAYS");
    }
    bool readMatchdays2 = false;
    if (reader.ReadStartIndex(L"MATCHDAYS2")) {
        if (!reader.CheckLine(L"%INDEXEND%MATCHDAYS2")) {
            mSecondSeasonMatchdays = reader.ReadLineArray<UShort>();
            readMatchdays2 = true;
        }
        reader.ReadEndIndex(L"MATCHDAYS2");
    }
    if (!readMatchdays2)
        mSecondSeasonMatchdays = mFirstSeasonMatchdays;
    if (reader.ReadStartIndex(L"FIXTURE")) {
        while (!reader.CheckLine(L"%INDEXEND%FIXTURE")) {
            auto &fixtures = mFixtures.emplace_back();
            auto fixtureTeams = reader.ReadLineArray<UChar>();
            UInt numMatches = fixtureTeams.size() / 2;
            for (UInt i = 0; i < numMatches; i++)
                fixtures.emplace_back(fixtureTeams[i * 2], fixtureTeams[i * 2 + 1]);
        }
        reader.ReadEndIndex(L"FIXTURE");
    }
    for (UInt i = 0; i < mBonuses.size(); i++)
        reader.ReadLine(mBonuses[i]);
    UChar transferFlags = reader.ReadLine<UChar>();
    mFreeAgentsCanBeTranferredAtAnyTime = Utils::CheckFlag(transferFlags, 0x1);
    mForeignFreeAgentsCanBeTransferredAtAnyTime = Utils::CheckFlag(transferFlags, 0x2);
    mPlayerLoansCanBeCancelledAtAnyTime = Utils::CheckFlag(transferFlags, 0x4);
    if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
        reader.ReadLine(mMaxNumberOfNonEUSigns);
        reader.ReadLine(mAgeLimitForNonEUSigns);
    }
    UInt loanFlags = reader.ReadLine<UInt>();
    mShortTermLoansTotalLimit = (loanFlags) & 0xF;
    mShortTermLoansSimultaneosLimit = (loanFlags >> 4) & 0xF;
    mLongTermLoansTotalLimit = (loanFlags >> 8) & 0xF;
    mLongTermLoansSimultaneosLimit = (loanFlags >> 12) & 0xF;
    mOverallTotalLoansLimit = (loanFlags >> 16) & 0xF;
    mOverallTotalSimultaneosLoansLimit = (loanFlags >> 20) & 0xF;
    mLoanedPlayersPerSeason = (loanFlags >> 24) & 0xF;
    mLoanPlayerPerSquad = (loanFlags >> 28) & 0xF;
    UInt otherFlags = reader.ReadLine<UInt>();
    mSimLoanOtherLeague = (otherFlags) & 0xF;
    mSimLoanSameLeague = (otherFlags >> 4) & 0xF;
    mLoanOtherLeagueCount = (otherFlags >> 8) & 0xF;
    mLoanSameLeagueCount = (otherFlags >> 12) & 0xF;
    mMinDomesticPlayerCount = (otherFlags >> 16) & 0x1F;
    mMinU21PlayerCount = (otherFlags >> 21) & 0x1F;
    mMinU24PlayerCount = (otherFlags >> 26) & 0x1F;
    FifamCompetition::Read(reader, database, nationId);
}

void FifamCompLeague::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
    if (!writer.IsVersionGreaterOrEqual(0x2011, 0x07)) {
        if (!writer.IsVersionGreaterOrEqual(0, 0x01))
            writer.WriteLine(FifamTr(mName));
        else
            writer.WriteLineTranslationArray(mName);
    }
    writer.WriteLine(mNumTeams);
    writer.WriteLine(mLeagueLevel);
    Vector<String> relFlags;
    relFlags.push_back(Utils::Format(L"REL_RULE_%d", mNumRelegatedTeams));
    if (mIndoorRelegation)
        relFlags.push_back(L"REL_RULE_INDOOR");
    if (mReserveRelegation)
        relFlags.push_back(L"REL_RULE_RESERVE");
    if (mTakePoints)
        relFlags.push_back(L"REL_RULE_TAKE_POINTS");
    writer.WriteLineArray(relFlags);
    writer.WriteLine(mEqualPointsSorting);
    writer.WriteLine(mNumRounds);
    writer.WriteLine(mRoundType.ToStr());
    writer.WriteLine(mAttendanceMp);
    writer.WriteLine(mTransferMarketMp);
    writer.WriteLine(L";");
    writer.WriteStartIndex(L"TEAMS");
    Vector<UInt> teamIDs;
    if (mID.mType == FifamCompType::League) {
        auto teams = FifamUtils::MakeWriteableIDsList(mTeams);
        UInt numTeams = Utils::Min(mNumTeams, teams.size());
        for (UInt i = 0; i < numTeams; i++) {
            if (i != 0)
                writer.Write(L",");
            writer.Write(Utils::Format(L"%x", teams[i]));
        }
        writer.WriteNewLine();
    }
    else {
        for (UInt i = 0; i < mNumTeams; i++)
            teamIDs.push_back(i + 1);
        writer.WriteLineArray(teamIDs);
    }
    writer.WriteEndIndex(L"TEAMS");
    writer.WriteLine(L";");
    writer.WriteStartIndex(L"MATCHDAYS");
    writer.WriteLineArray(mFirstSeasonMatchdays);
    writer.WriteEndIndex(L"MATCHDAYS");
    writer.WriteStartIndex(L"MATCHDAYS2");
    writer.WriteLineArray(mSecondSeasonMatchdays);
    writer.WriteEndIndex(L"MATCHDAYS2");
    writer.WriteLine(L";");
    writer.WriteStartIndex(L"FIXTURE");
    for (UInt i = 0; i < mFixtures.size(); i++) {
        for (UInt f = 0; f < mFixtures[i].size(); f++) {
            if (f != 0)
                writer.Write(L",");
            writer.Write(mFixtures[i][f].first, mFixtures[i][f].second);
        }
        writer.WriteNewLine();
    }
    writer.WriteEndIndex(L"FIXTURE");
    for (UInt i = 0; i < mBonuses.size(); i++)
        writer.WriteLine(mBonuses[i]);
    UChar transferFlags = 0;
    Utils::SetFlag(transferFlags, 0x1, mFreeAgentsCanBeTranferredAtAnyTime);
    Utils::SetFlag(transferFlags, 0x2, mForeignFreeAgentsCanBeTransferredAtAnyTime);
    Utils::SetFlag(transferFlags, 0x4, mPlayerLoansCanBeCancelledAtAnyTime);
    writer.WriteLine(transferFlags);
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
        writer.WriteLine(mMaxNumberOfNonEUSigns);
        writer.WriteLine(mAgeLimitForNonEUSigns);
    }
    UInt loanFlags =
        (Utils::Clamp(mShortTermLoansTotalLimit, 0, 0xF)) |
        (Utils::Clamp(mShortTermLoansSimultaneosLimit, 0, 0xF) << 4) |
        (Utils::Clamp(mLongTermLoansTotalLimit, 0, 0xF) << 8) |
        (Utils::Clamp(mLongTermLoansSimultaneosLimit, 0, 0xF) << 12) |
        (Utils::Clamp(mOverallTotalLoansLimit, 0, 0xF) << 16) |
        (Utils::Clamp(mOverallTotalSimultaneosLoansLimit, 0, 0xF) << 20) |
        (Utils::Clamp(mLoanedPlayersPerSeason, 0, 0xF) << 24) |
        (Utils::Clamp(mLoanPlayerPerSquad, 0, 0xF) << 28);
    writer.WriteLine(loanFlags);
    UInt otherFlags =
        (Utils::Clamp(mSimLoanOtherLeague, 0, 0xF)) |
        (Utils::Clamp(mSimLoanSameLeague, 0, 0xF) << 4) |
        (Utils::Clamp(mLoanOtherLeagueCount, 0, 0xF) << 8) |
        (Utils::Clamp(mLoanSameLeagueCount, 0, 0xF) << 12) |
        (Utils::Clamp(mMinDomesticPlayerCount, 0, 0x1F) << 16) |
        (Utils::Clamp(mMinU21PlayerCount, 0, 0x1F) << 21) |
        (Utils::Clamp(mMinU24PlayerCount, 0, 0x1F) << 26);
    writer.WriteLine(otherFlags);
    FifamCompetition::Write(writer, database, nationId);
}
