#pragma once
#include "FifamCompetition.h"
#include "FifamEqualPointsSorting.h"
#include "FifamRoundID.h"
#include "FifamClubLink.h"
#include "FifamUtils.h"

// @since FM07
class FifamCompLeague : public FifamCompetition {
public:
    // @since FM07
    // league name
    FifamTrArray<String> mName;
    // @since FM07
    // num of teams in round
    UInt mNumTeams = 0;
    // @since FM07
    // league level
    UChar mLeagueLevel = 0;
    // @since FM07
    // number of relegated teams
    UChar mNumRelegatedTeams = 0;
    // @since FM07
    // indoor relegation flag
    Bool mIndoorRelegation = false;
    // @since FM07
    // take points from this league in next round
    Bool mTakePoints = false;
    // @since FM07
    // equal points sorting rule
    FifamEqualPointsSorting mEqualPointsSorting;
    // @since FM07
    // number of rounds
    UChar mNumRounds = 0;
    // @since FM07
    // round type - usually Group1 for leagues
    FifamRoundID mRoundType = FifamRoundID::Group1;
    // @since FM07
    // attendance multiplier
    UChar mAttendanceMp = 0;
    // @since FM07
    // transfers multiplier
    UChar mTransferMarketMp = 0;
    // @since FM07
    // teams
    Vector<FifamClubLink> mTeams;
    // @since FM07
    // first season calendar
    Vector<UShort> mFirstSeasonMatchdays;
    // @since FM07
    // second season calendar
    Vector<UShort> mSecondSeasonMatchdays;
    // @since FM07
    // fixtures
    Vector<Vector<Pair<UChar, UChar>>> mFixtures;
    // @since FM07
    // bonuses
    Array<UInt, 4> mBonuses = {};
    // @since FM07
    Bool mFreeAgentsCanBeTranferredAtAnyTime = false;
    // @since FM07
    Bool mForeignFreeAgentsCanBeTransferredAtAnyTime = false;
    // @since FM07
    Bool mPlayerLoansCanBeCancelledAtAnyTime = false;


    void Read(FifamReader &reader, FifamDatabase *database) {
        if (!reader.IsVersionGreaterOrEqual(0x2011, 0x07))
            reader.ReadLineTranslationArray(mName);
        reader.ReadLine(mNumTeams);
        reader.ReadLine(mLeagueLevel);
        auto relFlags = Utils::Split(reader.ReadFullLine(), L',');
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
        if (reader.ReadStartIndex(L"MATCHDAYS2")) {
            if (!reader.CheckLine(L"%INDEXEND%MATCHDAYS2"))
                mSecondSeasonMatchdays = reader.ReadLineArray<UShort>();
            reader.ReadEndIndex(L"MATCHDAYS2");
        }
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

    }

    void Write(FifamWriter &writer, FifamDatabase *database) {

    }
};
