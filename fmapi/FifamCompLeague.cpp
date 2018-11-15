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
            if (fixtureTeams.size() > 1) {
                UInt numMatches = fixtureTeams.size() / 2;
                for (UInt i = 0; i < numMatches; i++)
                    fixtures.emplace_back(fixtureTeams[i * 2], fixtureTeams[i * 2 + 1]);
            }
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
    auto loanFlags = reader.ReadLine<Int>();
    mShortTermLoansTotalLimit = (loanFlags) & 0xF;
    mShortTermLoansSimultaneosLimit = (loanFlags >> 4) & 0xF;
    mLongTermLoansTotalLimit = (loanFlags >> 8) & 0xF;
    mLongTermLoansSimultaneosLimit = (loanFlags >> 12) & 0xF;
    mOverallTotalLoansLimit = (loanFlags >> 16) & 0xF;
    mOverallTotalSimultaneosLoansLimit = (loanFlags >> 20) & 0xF;
    mLoanedPlayersPerSeason = (loanFlags >> 24) & 0xF;
    mLoanPlayerPerSquad = (loanFlags >> 28) & 0xF;
    auto otherFlags = reader.ReadLine<Int>();
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

const UChar fixture2[] = { 1,2 };
const UChar fixture3[] = { 1,2, 2,3, 3,1 };
const UChar fixture4[] = { 1,2,3,4, 2,3,4,1, 1,3,2,4 };
const UChar fixture5[] = { 1,2,3,4, 2,3,4,5, 3,4,5,1, 4,5,1,2, 5,1,2,3 };
const UChar fixture6[] = { 1,2,3,4,5,6, 3,1,6,4,5,2, 1,4,2,6,3,5, 5,1,4,2,6,3, 1,6,2,3,4,5 };
const UChar fixture7[] = { 2,1,4,3,6,5, 1,3,5,2,7,4, 5,1,3,7,4,6, 1,7,6,3,4,2, 7,6,5,4,3,2, 1,6,2,7,3,5, 4,1,6,2,7,5 };
const UChar fixture8[] = { 2,1,4,3,6,5,8,7, 1,3,5,2,7,4,8,6, 5,1,3,7,2,8,4,6, 1,7,8,5,6,3,4,2, 8,1,7,6,5,4,3,2, 1,6,4,8,2,7,3,5, 4,1,6,2,8,3,7,5 };
const UChar fixture9[] = { 2,1,4,3,6,5,8,7, 1,3,5,2,7,4,9,6, 5,1,3,7,2,9,6,8, 1,7,9,5,6,4,8,2, 9,1,2,4,5,8,3,6, 2,3,8,9,6,7,4,5, 8,1,5,3,9,4,7,2, 1,6,4,8,5,7,3,9, 4,1,6,2,8,3,9,7 };
const UChar fixture10[] = { 7,9,3,2,10,4,6,8,5,1, 8,10,4,3,9,5,1,6,2,7, 8,1,7,4,10,3,6,9,5,2, 4,5,9,8,3,7,1,10,2,6, 8,2,1,9,10,7,6,4,5,3, 4,8,7,5,9,10,3,6,2,1, 8,3,9,2,1,4,10,5,6,7, 4,9,7,8,3,1,2,10,5,6, 8,5,9,3,1,7,2,4,6,10 };
const UChar fixture11[] = { 9,10,11,2,3,5,6,7,1,4, 10,1,4,11,7,8,5,6,2,3, 9,7,3,11,6,2,8,5,4,10, 11,10,5,9,2,8,3,6,7,1, 8,3,9,2,1,5,4,7,6,11, 3,9,6,8,7,10,5,4,2,1, 1,3,4,2,10,5,8,11,9,6, 8,9,11,7,6,1,2,10,3,4, 9,11,1,8,4,6,10,3,7,5, 9,1,2,7,11,5,6,10,8,4, 1,11,10,8,7,3,5,2,4,9 };
const UChar fixture12[] = { 9,10,11,2,3,5,6,7,8,12,1,4, 10,1,4,11,12,9,7,8,5,6,2,3, 9,7,3,11,6,2,8,5,1,12,4,10, 11,10,12,4,5,9,2,8,3,6,7,1, 8,3,9,2,10,12,1,5,4,7,6,11, 11,12,3,9,6,8,7,10,5,4,2,1, 1,3,4,2,10,5,12,7,8,11,9,6, 8,9,11,7,6,1,5,12,2,10,3,4, 9,11,1,8,4,6,10,3,12,2,7,5, 9,1,3,12,2,7,11,5,6,10,8,4, 1,11,10,8,12,6,7,3,5,2,4,9 };
const UChar fixture13[] = { 1,2,3,4,5,6,7,8,9,10,11,12, 12,1,8,3,6,13,2,5,9,11,10,4, 1,9,7,6,13,2,11,10,5,12,4,8, 11,1,6,3,10,8,12,13,2,7,9,5, 3,2,1,10,7,12,4,6,13,9,5,11, 1,5,12,3,11,13,2,4,9,7,6,8, 13,1,3,9,8,2,5,10,7,11,4,12, 11,3,12,8,1,7,9,4,10,6,5,13, 8,9,4,11,7,5,6,2,13,10,3,1, 1,4,8,11,5,3,13,7,12,6,10,2, 8,1,3,13,6,9,2,12,4,5,7,10, 11,6,13,4,7,3,9,2,10,12,5,8, 6,1,8,13,3,10,12,9,4,7,2,11 };
const UChar fixture14[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14, 12,1,8,3,6,13,2,5,14,7,9,11,10,4, 1,9,7,6,3,14,13,2,11,10,5,12,4,8, 11,1,6,3,10,8,12,13,2,7,9,5,14,4, 3,2,1,10,8,14,7,12,4,6,13,9,5,11, 1,5,12,3,11,13,2,4,9,7,6,8,10,14, 13,1,3,9,14,6,8,2,5,10,7,11,4,12, 2,14,11,3,12,8,1,7,9,4,10,6,5,13, 8,9,14,12,4,11,7,5,6,2,13,10,3,1, 1,4,8,11,5,3,13,7,9,14,12,6,10,2, 8,1,3,13,6,9,2,12,4,5,14,11,7,10, 11,6,13,4,7,3,9,2,10,12,5,8,1,14, 6,1,8,13,3,10,14,5,12,9,4,7,2,11 };
const UChar fixture15[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14, 15,3,4,5,12,13,10,1,8,9,6,7,2,11, 5,15,7,4,9,6,1,8,11,10,13,2,14,12, 10,13,8,11,6,1,4,9,15,7,3,5,2,14, 14,10,1,4,7,3,9,15,11,6,12,2,13,8, 5,7,10,12,8,14,6,13,4,11,3,9,15,1, 14,6,1,3,9,5,11,15,12,8,2,10,13,4, 15,13,5,1,8,2,6,12,4,14,3,11,7,9, 14,15,1,7,11,5,12,4,2,6,10,8,13,3, 5,13,6,10,4,2,15,12,3,14,7,11,9,1, 2,15,11,9,12,3,10,4,8,6,14,5,13,7, 15,10,4,8,3,2,5,12,7,14,9,13,1,11, 13,1,14,9,12,7,10,3,8,15,6,4,2,5, 5,10,3,8,7,2,9,12,11,13,1,14,15,6, 12,1,2,9,10,7,8,5,6,3,4,15,14,11 };
const UChar fixture16[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16, 15,3,4,5,12,13,10,1,8,9,6,7,2,11,16,14, 3,16,5,15,7,4,9,6,1,8,11,10,13,2,14,12, 10,13,8,11,6,1,4,9,15,7,3,5,2,14,16,12, 14,10,1,4,7,3,9,15,11,6,12,2,13,8,5,16, 5,7,10,12,8,14,6,13,4,11,3,9,15,1,16,2, 14,6,1,3,7,16,9,5,11,15,12,8,2,10,13,4, 15,13,5,1,8,2,6,12,4,14,3,11,7,9,16,10, 14,15,9,16,1,7,11,5,12,4,2,6,10,8,13,3, 16,8,5,13,6,10,4,2,15,12,3,14,7,11,9,1, 2,15,11,9,12,3,10,4,8,6,14,5,1,16,13,7, 15,10,16,6,4,8,3,2,5,12,7,14,9,13,1,11, 11,16,13,1,14,9,12,7,10,3,8,15,6,4,2,5, 5,10,16,4,3,8,7,2,9,12,11,13,1,14,15,6, 12,1,2,9,10,7,8,5,6,3,4,15,14,11,13,16 };
const UChar fixture17[] = { 6,9,5,4,3,14,10,16,11,13,1,7,8,2,15,17, 17,8,7,11,13,10,16,3,14,5,4,6,12,15,2,1, 6,14,5,16,3,13,10,7,11,2,1,17,12,9,15,8, 17,11,7,3,13,5,16,6,4,12,9,15,8,1,2,10, 6,13,5,7,3,2,10,17,11,8,9,4,12,14,15,1, 17,3,7,6,16,12,14,9,4,15,1,11,8,10,2,5, 6,2,5,17,3,8,10,1,4,14,9,16,12,13,15,11, 17,6,7,12,13,9,16,4,14,15,11,10,1,3,8,5, 6,8,5,1,3,11,14,16,4,13,9,7,12,2,15,10, 17,12,7,4,13,14,16,15,10,3,11,5,1,6,2,9, 6,11,5,10,16,13,14,7,4,2,9,17,12,8,15,3, 17,4,7,16,13,15,3,5,10,6,1,12,8,9,2,14, 6,3,13,7,16,2,14,17,4,8,9,1,12,11,15,5, 17,16,7,15,5,6,10,12,11,9,1,4,8,14,2,13, 6,15,13,17,16,8,14,1,4,11,9,10,12,3,2,7, 17,7,5,12,3,9,10,4,11,14,1,16,8,13,15,2, 7,8,13,1,16,11,14,10,4,3,9,5,12,6,2,17 };
const UChar fixture18[] = { 18,12,6,9,5,4,3,14,10,16,11,13,1,7,8,2,15,17, 17,8,7,11,13,10,16,3,14,5,4,6,9,18,12,15,2,1, 18,4,6,14,5,16,3,13,10,7,11,2,1,17,12,9,15,8, 17,11,7,3,13,5,16,6,14,18,4,12,9,15,8,1,2,10, 18,16,6,13,5,7,3,2,10,17,11,8,9,4,12,14,15,1, 17,3,7,6,13,18,16,12,14,9,4,15,1,11,8,10,2,5, 18,7,6,2,5,17,3,8,10,1,4,14,9,16,12,13,15,11, 17,6,7,12,13,9,16,4,14,15,11,10,1,3,8,5,2,18, 18,17,6,8,5,1,3,11,14,16,4,13,9,7,12,2,15,10, 17,12,7,4,13,14,16,15,10,3,11,5,1,6,8,18,2,9, 18,1,6,11,5,10,16,13,14,7,4,2,9,17,12,8,15,3, 17,4,7,16,13,15,3,5,10,6,11,18,1,12,8,9,2,14, 18,10,6,3,13,7,16,2,14,17,4,8,9,1,12,11,15,5, 17,16,7,15,5,6,3,18,10,12,11,9,1,4,8,14,2,13, 18,5,6,15,13,17,16,8,14,1,4,11,9,10,12,3,2,7, 17,7,6,18,5,12,3,9,10,4,11,14,1,16,8,13,15,2, 18,15,7,8,13,1,16,11,14,10,4,3,9,5,12,6,2,17 };
const UChar fixture19[] = { 6,17,10,19,3,18,16,1,9,14,13,15,12,2,4,7,8,11, 6,3,7,8,2,4,15,5,14,12,1,9,18,13,19,16,17,10, 10,6,3,17,16,18,9,19,13,14,12,1,4,15,8,2,5,11, 6,9,10,12,7,11,15,8,14,4,1,13,18,5,19,3,17,16, 11,2,3,10,16,6,9,17,13,19,12,18,4,1,8,14,5,7, 6,4,10,16,3,5,2,7,15,11,1,8,18,9,19,12,17,13, 11,14,7,15,16,3,9,10,13,6,12,17,4,19,8,18,5,2, 6,12,10,13,3,9,16,8,15,2,14,7,1,11,19,5,17,4, 11,18,7,1,2,14,9,16,13,3,12,15,4,10,8,19,5,17, 6,8,10,7,3,12,16,13,9,5,14,15,1,2,18,4,19,11, 11,6,7,18,2,10,15,1,13,9,12,16,4,3,8,17,5,14, 6,7,3,8,16,4,9,12,13,5,1,14,18,15,19,2,17,11, 11,16,7,19,2,17,15,6,14,18,12,13,4,9,8,10,5,1, 6,2,10,5,3,11,9,7,13,4,12,8,18,1,19,14,17,15, 11,10,7,3,2,18,15,19,14,17,1,6,4,12,8,13,5,16, 6,14,10,15,3,2,16,7,9,8,12,5,4,11,19,18,17,1, 11,9,7,13,2,16,15,3,14,10,1,19,18,17,8,4,5,6, 6,18,10,1,3,14,16,15,9,2,13,11,12,7,8,5,17,19, 11,12,7,17,2,13,15,9,14,16,1,3,18,10,19,6,5,4 };
const UChar fixture20[] = { 6,17,10,19,3,18,16,1,9,14,13,15,12,2,4,7,8,11,5,20, 6,3,11,20,7,8,2,4,15,5,14,12,1,9,18,13,19,16,17,10, 20,7,10,6,3,17,16,18,9,19,13,14,12,1,4,15,8,2,5,11, 6,9,10,12,7,11,2,20,15,8,14,4,1,13,18,5,19,3,17,16, 20,15,11,2,3,10,16,6,9,17,13,19,12,18,4,1,8,14,5,7, 6,4,10,16,3,5,2,7,15,11,14,20,1,8,18,9,19,12,17,13, 20,1,11,14,7,15,16,3,9,10,13,6,12,17,4,19,8,18,5,2, 6,12,10,13,3,9,16,8,15,2,14,7,1,11,18,20,19,5,17,4, 20,6,11,18,7,1,2,14,9,16,13,3,12,15,4,10,8,19,5,17, 6,8,10,7,3,12,16,13,9,5,14,15,1,2,18,4,19,11,17,20, 20,19,11,6,7,18,2,10,15,1,13,9,12,16,4,3,8,17,5,14, 6,7,10,20,3,8,16,4,9,12,13,5,1,14,18,15,19,2,17,11, 20,3,11,16,7,19,2,17,15,6,14,18,12,13,4,9,8,10,5,1, 6,2,10,5,3,11,16,20,9,7,13,4,12,8,18,1,19,14,17,15, 20,9,11,10,7,3,2,18,15,19,14,17,1,6,4,12,8,13,5,16, 6,14,10,15,3,2,16,7,9,8,13,20,12,5,4,11,19,18,17,1, 20,12,11,9,7,13,2,16,15,3,14,10,1,19,18,17,8,4,5,6, 6,18,10,1,3,14,16,15,9,2,13,11,12,7,4,20,8,5,17,19, 20,8,11,12,7,17,2,13,15,9,14,16,1,3,18,10,19,6,5,4 };
const UChar fixture21[] = { 13,4,1,5,14,15,10,20,6,8,19,17,2,12,7,16,11,3,9,21, 4,9,18,13,15,1,20,14,8,10,17,6,12,19,16,2,3,7,21,11, 4,18,13,5,1,20,14,8,10,17,6,12,19,16,2,3,7,21,9,11, 18,9,5,4,15,13,8,1,17,14,12,10,16,6,3,19,21,2,11,7, 18,5,4,15,13,20,1,17,14,12,10,16,6,3,19,21,2,11,9,7, 5,9,15,18,20,4,8,13,12,1,16,14,3,10,21,6,11,19,7,2, 5,15,18,20,4,8,13,17,1,16,14,3,10,21,6,11,19,7,9,2, 15,9,20,5,8,18,17,4,12,13,3,1,21,14,11,10,7,6,2,19, 15,20,5,8,18,17,4,12,13,16,1,21,14,11,10,7,6,2,9,19, 20,9,8,15,17,5,12,18,16,4,3,13,11,1,7,14,2,10,19,6, 20,8,15,17,5,12,18,16,4,3,13,21,1,7,14,2,10,19,9,6, 8,9,17,20,12,15,16,5,3,18,21,4,11,13,2,1,19,14,6,10, 8,17,20,12,15,16,5,3,18,21,4,11,13,7,1,19,14,6,9,10, 17,9,12,8,16,20,3,15,21,5,11,18,7,4,2,13,6,1,10,14, 17,12,8,16,20,3,15,21,5,11,18,7,4,2,13,19,1,10,9,14, 12,9,16,17,3,8,21,20,11,15,7,5,2,18,19,4,6,13,14,1, 12,16,17,3,8,21,20,11,15,7,5,2,18,19,4,6,13,10,9,1, 16,9,3,12,21,17,11,8,7,20,2,15,19,5,6,18,10,4,14,13, 16,3,12,21,17,11,8,7,20,2,15,19,5,6,18,10,4,14,13,1, 9,3,21,16,11,12,7,17,2,8,19,20,6,15,10,5,14,18,1,4, 3,21,16,11,12,7,17,2,8,19,20,6,15,10,5,14,18,1,13,9 };
const UChar fixture22[] = { 13,4,22,18,1,5,14,15,10,20,6,8,19,17,2,12,7,16,11,3,9,21, 4,9,18,13,5,22,15,1,20,14,8,10,17,6,12,19,16,2,3,7,21,11, 4,18,13,5,22,15,1,20,14,8,10,17,6,12,19,16,2,3,7,21,9,11, 18,9,5,4,15,13,20,22,8,1,17,14,12,10,16,6,3,19,21,2,11,7, 18,5,4,15,13,20,22,8,1,17,14,12,10,16,6,3,19,21,2,11,9,7, 5,9,15,18,20,4,8,13,17,22,12,1,16,14,3,10,21,6,11,19,7,2, 5,15,18,20,4,8,13,17,22,12,1,16,14,3,10,21,6,11,19,7,9,2, 15,9,20,5,8,18,17,4,12,13,16,22,3,1,21,14,11,10,7,6,2,19, 15,20,5,8,18,17,4,12,13,16,22,3,1,21,14,11,10,7,6,2,9,19, 20,9,8,15,17,5,12,18,16,4,3,13,21,22,11,1,7,14,2,10,19,6, 20,8,15,17,5,12,18,16,4,3,13,21,22,11,1,7,14,2,10,19,9,6, 8,9,17,20,12,15,16,5,3,18,21,4,11,13,7,22,2,1,19,14,6,10, 8,17,20,12,15,16,5,3,18,21,4,11,13,7,22,2,1,19,14,6,9,10, 17,9,12,8,16,20,3,15,21,5,11,18,7,4,2,13,19,22,6,1,10,14, 17,12,8,16,20,3,15,21,5,11,18,7,4,2,13,19,22,6,1,10,9,14, 12,9,16,17,3,8,21,20,11,15,7,5,2,18,19,4,6,13,10,22,14,1, 12,16,17,3,8,21,20,11,15,7,5,2,18,19,4,6,13,10,22,14,9,1, 16,9,3,12,21,17,11,8,7,20,2,15,19,5,6,18,10,4,14,13,1,22, 16,3,12,21,17,11,8,7,20,2,15,19,5,6,18,10,4,14,13,1,9,22, 9,3,21,16,11,12,7,17,2,8,19,20,6,15,10,5,14,18,1,4,22,13, 3,21,16,11,12,7,17,2,8,19,20,6,15,10,5,14,18,1,4,22,13,9 };
const UChar fixture23[] = { 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, 13,10,11,4,5,2,1,6,17,22,23,14,15,12,3,20,21,8,7,18,19,16, 2,11,4,13,18,1,6,9,8,3,20,5,10,15,12,23,14,17,22,16,7,19, 17,12,15,4,13,2,11,20,16,14,23,10,5,8,21,6,9,18,1,7,19,22, 2,15,18,21,20,13,4,23,10,17,12,16,14,22,7,9,19,1,6,3,8,11, 22,12,17,4,13,8,3,18,14,19,23,2,5,6,9,1,16,10,15,20,21,7, 6,11,10,22,18,5,8,15,20,23,12,14,7,3,19,9,2,17,4,16,1,21, 13,6,14,10,22,4,17,20,23,8,11,18,5,7,3,1,21,9,12,19,16,2, 18,13,6,15,8,17,20,16,10,12,9,3,7,11,19,21,2,22,4,14,1,5, 10,19,15,18,12,4,14,2,22,20,23,6,11,1,5,9,16,8,13,7,3,21, 20,14,2,12,4,10,9,11,18,23,6,17,8,22,21,5,1,13,7,15,19,3, 10,2,12,20,4,19,14,8,16,6,13,9,11,21,5,3,17,18,23,7,15,1, 6,22,2,4,19,5,18,16,8,12,20,10,3,11,21,13,9,15,1,23,7,17, 2,19,14,6,4,20,10,8,22,18,13,3,11,5,16,7,23,9,15,21,17,1, 8,4,3,15,18,14,5,13,9,17,1,16,7,22,19,11,20,2,6,12,21,23, 17,21,13,11,12,18,20,19,2,8,14,7,22,1,23,3,15,5,10,6,16,9, 3,17,9,22,18,10,6,4,8,20,1,14,7,12,19,13,11,15,5,23,21,16, 10,7,12,1,23,11,2,6,4,18,14,9,22,21,16,3,15,13,8,19,17,5, 13,23,3,22,6,20,5,16,21,14,9,12,7,4,19,15,18,2,11,17,1,10, 4,1,23,15,8,6,20,18,2,7,10,9,12,21,22,5,16,11,17,13,14,3, 13,16,3,12,18,8,15,17,11,22,21,10,1,2,7,20,5,14,9,4,19,23, 8,7,2,9,10,3,20,1,4,21,14,11,22,13,16,15,17,23,6,19,12,5, 13,14,3,4,5,10,21,2,9,20,1,8,19,17,18,6,23,16,15,22,11,12 };
const UChar fixture24[] = { 24,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, 13,10,11,4,5,2,1,6,17,22,23,14,15,12,3,20,21,8,9,24,7,18,19,16, 2,11,4,13,18,1,6,9,8,3,20,5,10,15,12,23,14,17,24,21,22,16,7,19, 17,12,15,4,13,2,11,20,16,14,23,10,5,8,3,24,21,6,9,18,1,7,19,22, 2,15,18,21,20,13,4,23,10,17,12,16,14,22,7,9,19,1,6,3,24,5,8,11, 22,12,17,4,13,8,3,18,14,19,23,2,11,24,5,6,9,1,16,10,15,20,21,7, 6,11,24,13,10,22,18,5,8,15,20,23,12,14,7,3,19,9,2,17,4,16,1,21, 13,6,14,10,22,4,17,20,23,8,11,18,5,7,3,1,21,9,12,19,15,24,16,2, 18,13,6,15,24,23,8,17,20,16,10,12,9,3,7,11,19,21,2,22,4,14,1,5, 10,19,15,18,12,4,14,2,22,20,17,24,23,6,11,1,5,9,16,8,13,7,3,21, 20,14,2,12,4,10,9,11,18,23,6,17,24,16,8,22,21,5,1,13,7,15,19,3, 10,2,12,20,4,19,14,8,22,24,16,6,13,9,11,21,5,3,17,18,23,7,15,1, 6,22,24,14,2,4,19,5,18,16,8,12,20,10,3,11,21,13,9,15,1,23,7,17, 2,19,14,6,4,20,10,8,12,24,22,18,13,3,11,5,16,7,23,9,15,21,17,1, 24,10,8,4,3,15,18,14,5,13,9,17,1,16,7,22,19,11,20,2,6,12,21,23, 17,21,13,11,12,18,20,19,2,8,4,24,14,7,22,1,23,3,15,5,10,6,16,9, 24,2,3,17,9,22,18,10,6,4,8,20,1,14,7,12,19,13,11,15,5,23,21,16, 10,7,12,1,23,11,20,24,2,6,4,18,14,9,22,21,16,3,15,13,8,19,17,5, 24,8,13,23,3,22,6,20,5,16,21,14,9,12,7,4,19,15,18,2,11,17,1,10, 24,19,4,1,23,15,8,6,20,18,2,7,10,9,12,21,22,5,16,11,17,13,14,3, 13,16,3,12,18,8,15,17,11,22,5,14,21,10,1,2,7,20,6,24,9,4,19,23, 8,7,2,9,10,3,20,1,4,21,14,11,22,13,16,15,17,23,6,19,24,18,12,5, 13,14,3,4,5,10,21,2,9,20,1,8,7,24,19,17,18,6,23,16,15,22,11,12 };

const UChar * const fixtureTables[] = {
    fixture2,
    fixture3,
    fixture4,
    fixture5,
    fixture6,
    fixture7,
    fixture8,
    fixture9,
    fixture10,
    fixture11,
    fixture12,
    fixture13,
    fixture14,
    fixture15,
    fixture16,
    fixture17,
    fixture18,
    fixture19,
    fixture20,
    fixture21,
    fixture22,
    fixture23,
    fixture24
};

void FifamCompLeague::GenerateFixtures() {
    mFixtures.clear();
    UInt numTeams = mTeams.size();
    UInt numRounds = mNumRounds;
    if (numRounds < 1 || numTeams < 2 || numTeams > 24)
        return;
    UInt numMatchdaysInRound = numTeams - 1 + (numTeams % 2);
    UInt numMatchesInMatchday = numTeams / 2;
    UInt numMatchesInRound = numMatchdaysInRound * numMatchesInMatchday;
    UChar const *data = fixtureTables[numTeams - 2];
    mFixtures.resize(numRounds * numMatchdaysInRound);
    for (UInt r = 0; r < numRounds; r++) {
        bool secondRound = r % 2;
        UInt currFixtureData = 0;
        for (UInt md = 0; md < numMatchdaysInRound; md++) {
            auto &fixtures = mFixtures[numMatchdaysInRound * r + md];
            fixtures.resize(numMatchesInMatchday);
            for (UInt m = 0; m < numMatchesInMatchday; m++) {
                if (secondRound)
                    fixtures[m] = { data[currFixtureData + 1], data[currFixtureData] };
                else
                    fixtures[m] = { data[currFixtureData], data[currFixtureData + 1] };
                currFixtureData += 2;
            }
        }
    }
}

void FifamCompLeague::GenerateCalendar(UInt startDay, UInt endDay, UInt winterBreakStartDay, UInt winterBreakEndDay) {
    UInt numRounds = mNumRounds;
    if (mNumRounds < 1 || mTeams.size() < 2 || mTeams.size() > 24)
        return;
    UInt numMatchdaysInRound = mTeams.size() - 1 + (mTeams.size() % 2);
    UInt numMatchdays = numMatchdaysInRound * mNumRounds;
    mFirstSeasonMatchdays.resize(numMatchdays);
    UInt matchday = startDay;
    for (UInt i = 0; i < numMatchdays; i++) {
        mFirstSeasonMatchdays[i] = matchday;
        matchday += 7;
    }
    mSecondSeasonMatchdays = mFirstSeasonMatchdays;
}
