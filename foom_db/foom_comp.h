#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;
struct stage_name;
struct club;
struct stadium;

struct comp {
    Int mID = 0;
    String mName;
    String mShortName;
    nation *mNation = 0;
    Int mContinent = -1;
    comp *mParentCompetition = 0;
    Int mReputation = 0;
    Bool mUsesSeatedStadiumsOnly = 0;
    Bool mIsExtinct = 0;
    Int mType = 0;
    Int mCompetitionLevel = 0;

    struct history {
        Int mYear = 0;
        team *mFirstPlaced = 0;
        team *mSecondPlaced = 0;
        team *mThirdPlaced = 0;
        team *mPlayOffWinner = 0;
        stage_name *mStageName = 0;
        Int mYearOrder = 0;
        Bool mNoDataForYear = 0;
        stadium *mHostStadium = 0;
    };

    Vector<history> mVecHistory;

    Vector<team *> mVecTeams;

    struct converter_data {
        Bool mUsesTableData = false;
        void *mDivInfo = nullptr;
        void *mLeague = nullptr;
        void *mCup = nullptr;
    } mConverterData;
};

}
