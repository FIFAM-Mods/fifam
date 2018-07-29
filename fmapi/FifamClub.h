#pragma once

#include "FifamPlayer.h"
#include "FifamStaff.h"
#include "FifamTranslation.h"
#include "FifamStadiumType.h"
#include "FifamCompID.h"
#include "FifamKit.h"
#include "FifamClubHistory.h"
#include "FifamNation.h"
#include "FifamClubAiStrategy.h"
#include "FifamClubLandscape.h"
#include "FifamClubSettlement.h"
#include "FifamClubMediaPressure.h"
#include "FifamClubBadge.h"
#include "FifamClubInterfaceColor.h"
#include "FifamClubTeamColor.h"
#include "FifamGeoCoords.h"
#include "Color.h"

class FifamCountry;

/* Articles
# | English   | French  | German     | Italian  | Spanish   | Polish
--|-----------|---------|------------|----------|-----------|--------------
0 | -         | -       | -          | -        | -         |
1 | the       | le      | der        | il       | el        |
2 |           | la      | die        | lo       | la        |
3 |           | l'      | das        | l'       | los       |
4 |           | les     |            | la       | las       |
5 |           |         |            | i        |           |
6 |           |         |            | gli      |           |
7 |           |         |            | le       |           |
*/

// @since FM07
class FifamClub {
public:
    // @since FM08
    UInt mUniqueID = 0;
    // @since FM07
    // @maxsize 29
    TrArray<String> mName;
    // @since FM07
    // @maxsize 29
    TrArray<String> mName2;
    // @since FM07
    // @maxsize 10
    TrArray<String> mShortName;
    // @since FM07
    // @maxsize 10
    TrArray<String> mShortName2;
    // @since FM07
    // @maxsize 4
    TrArray<String> mAbbreviation;
    // @since FM07
    // @maxsize 29
    TrArray<String> mCityName;
    // @since FM07
    // @maxsize 19
    // not an array before FM11
    TrArray<String> mPlayerInText;
    // @since FM07
    // @maxsize 19
    // not an array before FM11
    TrArray<String> mFanName1;
    // @since FM07
    // @maxsize 119
    String mAddress;
    // @since FM07
    // @maxsize 19
    String mTelephone;
    // @since FM07
    // @maxsize 119
    String mWebsiteAndMail;
    // @since FM07
    // @maxsize 255
    String mFansites;
    // @since FM07
    // @maxsize 19
    String mMascotName;
    // @since FM07
    TrArray<UChar> mAbbreviationArticle = {};
    // @since FM07
    TrArray<UChar> mPlayerInTextArticle = {};
    // @since FM07
    TrArray<UChar> mFanName1Article = {};
    // @since FM07
    TrArray<UChar> mClubNameUsageInPhrase = {};
    // @since FM07
    TrArray<UChar> mClubNameUsageInPhrase2 = {};
    // @since FM07
    FifamClub *mPartnershipClub = nullptr;
    // @since FM07
    Array<FifamClub *, 4> mRivalClubs = {};
    // @since FM07
    UShort mYearOfFoundation = 1900;
    // @since FM07
    UInt mInitialCapital = 0;
    // @since FM07
    UInt mTransferBudget = 0;
    // @since FM07
    Bool mJointStockCompany = false;
    // @since FM07
    // 3 (1+2) - penalty points
    UChar mPenaltyFlags = 0;
    // @since FM07
    Char mPenaltyPoints = 0;
    // @since FM07
    UInt mPotentialFansCount = 0;
    // @since FM07
    UInt mAverageAttendanceLastSeason = 0;
    // @since FM07
    UInt mCountOfSoldSeasonTickets = 0;
    // @since FM07
    Array<UInt, 3> mLeagueTotalPoints = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalWins = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalDraws = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalLoses = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalGoals = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalGoalsAgainst = {};
    // @since FM07
    Array<UInt, 3> mLeagueTotalLeadershipsInTable = {};
    // @since FM07
    // if higher than 999, set to 0
    UInt mStadiumVenue = 0;
    // @since FM07
    // if higher than 150000, set to 150000
    UInt mStadiumCapacity = 0;
    // @since FM07
    // @maxsize 29 in Editor
    TrArray<String> mStadiumName;
    // @since FM07
    FifamStadiumType mStadiumType;
    // @since FM07
    // 1 - stadium big main stand, 128 - can't be deleted in the editor
    UChar mFlags = 0; 

    /* Club grounds */

    // @since FM07
    // @range [0;6]
    UChar mClubFacilities = 0; // club centre in 07 (0-5), 0-6
    // @since FM07
    // @range [0;10]
    UChar mYouthCentre = 0; // youth department in 07 (0-5), 1(0)-10 in FM12
    // @since FM07
    // @range [0;10]
    UChar mYouthBoardingSchool = 0; // youth department in 07 (0-5), 1(0)-10 in FM12
    // @since FM07
    // @until FM12
    // @range [0;5]
    UChar mMedicalDepartment = 0; // 0-5
    // @since FM07
    // @until FM12
    // @range [0;5]
    UChar mMerchandising = 0; // 0-5
    // @since FM07
    // @until FM12
    // @range [0;5]
    UChar mTrainingGrounds = 0; // 0-5
    // @since FM07
    // @until FM12
    // @range [0;5]
    UChar mMedia = 0; // 0-5
    // @since FM07
    // @until FM12
    // @range [0;5]
    UChar mResearch = 0; // 0-5
    // @since FM12
    FifamClubAiStrategy mAiStrategy;
    // @since FM12
    FifamClubLandscape mLandscape;
    // @since FM12
    FifamClubSettlement mSettlement;

    // @since FM07
    // 1 - promoted, 2 - reserve team promoted, 4 - relegated, 8 - reserve team relegated, 16 - cup winner, 32 - reserve team cup winner, 64 - cup finalist, 128 - reserve team cup finalist, 256 - does not take part in IT - Cup
    UShort mLastSeasonFlags = 0;
    // @since FM07
    FifamClubBadge mBadge;
    // @since FM07
    ColorPair mClubColour;
    // @since FM07
    Color mMerchandiseColour;
    // @since FM07
    Color mHeaderColour;
    // @since FM07
    Color mBackgroundColour;
    // @since FM07
    FifamClubHistory mHistory;
    // @since FM07
    FifamGeoCoords mGeoCoords;
    // @since FM07
    UChar mNationalPrestige = 0;
    // @since FM07
    UChar mInternationalPrestige = 0;
    // @since FM07
    FifamNation mTransfersCountry1;
    // @since FM07
    FifamNation mTransfersCountry2;
    // @since FM07
    FifamNation mYouthPlayersCountry;
    // @since FM07
    UInt mFifaID = 0;
    // @since FM07
    FifamKit mKit;
    // @since FM07
    Array<FifamCompID, 4> mLowestLeagues;

    // @since FM08
    FifamClubMediaPressure mMediaPressure;
    // @since FM08
    UInt mAdditionalFlags = 0; // 0x400000 - rich guy controlled, 0x1000000 - youth players are basques

    // @since FM11
    TrArray<String> mFanName2;
    // @since FM11
    TrArray<String> mTermForTeam1;
    // @since FM11
    TrArray<String> mTermForTeam2;
    // @since FM11
    TrArray<UChar> mFanName2Article = {};
    // @since FM11
    TrArray<UChar> mTermForTeam1Article = {};
    // @since FM11
    TrArray<UChar> mTermForTeam2Article = {};

    struct {
        // @since FM07
        // @maxsize 29
        String _1; // empty
        // @since FM07
        UChar _2 = 0; // 0
        // @since FM07
        // unknown color
        UChar _3 = 0; // 0
        // @since FM07
        String _4;
    } Unknown;

    FifamCountry *mCountry = nullptr;
    Vector<FifamPlayer *> mPlayers;
    Vector<FifamStaff *> mStaffs;

    inline static Vector<ColorPair> mTeamColorsTable = {
        {{ 20,101,192 } ,{ 0,0,0 }},
        {{ 20,101,192 } ,{ 11,166,3 }},
        {{ 20,101,192 } ,{ 245,30,2 }},
        {{ 20,101,192 } ,{ 255,255,255 }},
        {{ 20,101,192 } ,{ 255,246,0 }},
        {{ 117,12,7 }   ,{ 0,0,0 }},
        {{ 117,12,7 }   ,{ 145,198,250 }},
        {{ 117,12,7 }   ,{ 255,255,255 }},
        {{ 11,166,3 }   ,{ 0,0,0 }},
        {{ 11,166,3 }   ,{ 245,30,2 }},
        {{ 11,166,3 }   ,{ 255,255,255 }},
        {{ 11,166,3 }   ,{ 255,246,0 }},
        {{ 145,198,250 },{ 0,0,0 }},
        {{ 145,198,250 },{ 255,255,255 }},
        {{ 255,138,0 }  ,{ 0,0,0 }},
        {{ 255,138,0 }  ,{ 255,255,255 }},
        {{ 172,104,248 },{ 0,0,0 }},
        {{ 172,104,248 },{ 255,255,255 }},
        {{ 245,30,2 }   ,{ 0,0,0 }},
        {{ 245,30,2 }   ,{ 255,255,255 }},
        {{ 245,30,2 }   ,{ 255,246,0 }},
        {{ 255,246,0 }  ,{ 0,0,0 }},
        {{ 255,246,0 }  ,{ 255,255,255 }},
        {{ 255,255,255 },{ 0,0,0 }},
        {{ 134,52,4 }   ,{ 255,255,255 }},
        {{ 6,42,139 }   ,{ 255,255,255 }},
        {{ 247,183,211 },{ 34,30,31 }},
        {{ 240,169,47 } ,{ 162,29,52 }}
    };
    inline static Vector<Color> mMerchandiseColorsTable = {
        { 0, 174, 255 },
        { 190, 13, 13 },
        { 11, 166, 3 },
        { 255, 246, 0 },
        { 255, 255, 255 }
    };
    inline static Vector<Color> m07InterfaceColorsTable = {
        { 0, 0, 0 },
        { 236, 235, 231 },
        { 63, 63, 62 },
        { 215, 149, 0 },
        { 255, 207, 0 },
        { 223, 0, 24 },
        { 53, 128, 0 },
        { 0, 82, 155 },
        { 46, 48, 146 },
        { 0, 137, 208 },
        { 97, 54, 11 },
        { 247, 184, 211 },
        { 131, 24, 0 },
        { 125, 89, 163 },
        { 156, 125, 212 },
        { 138, 140, 143 },
        { 204, 204, 204 },
        { 138, 140, 143 },
        { 192, 146, 87 },
        { 255, 222, 23 },
        { 194, 226, 84 },
        { 255, 204, 3 },
        { 243, 194, 2 },
        { 245, 160, 0 },
        { 245, 125, 48 },
        { 92, 49, 4 },
        { 251, 195, 196 },
        { 208, 50, 92 },
        { 204, 0, 51 },
        { 238, 49, 36 },
        { 237, 27, 36 },
        { 204, 0, 0 },
        { 197, 35, 38 },
        { 170, 0, 0 },
        { 79, 174, 92 },
        { 88, 162, 87 },
        { 104, 176, 34 },
        { 117, 164, 65 },
        { 76, 132, 59 },
        { 88, 135, 117 },
        { 92, 97, 12 },
        { 67, 172, 215 },
        { 70, 160, 235 },
        { 0, 128, 193 },
        { 0, 136, 179 },
        { 15, 72, 177 },
        { 0, 92, 151 },
        { 15, 72, 177 },
        { 34, 84, 154 },
        { 35, 34, 108 },
        { 0, 96, 134 },
        { 1, 27, 88 },
        { 0, 0, 51 },
        { 142, 49, 69 },
        { 126, 114, 180 },
        { 125, 48, 122 },
        { 139, 45, 81 },
        { 54, 53, 148 },
        { 93, 29, 47 },
        { 92, 49, 4 },
        { 149, 85, 66 },
        { 101, 101, 101 },
        { 79, 79, 79 },
        { 63, 63, 62 }
    };
    inline static Vector<Color> m08InterfaceColorsTable = {
        { 69 , 120, 183 },
        { 37 , 37 , 39 },
        { 101, 101, 103 },
        { 165, 165, 167 },
        { 134, 9  , 5 },
        { 198, 32 , 16 },
        { 245, 30 , 2 },
        { 139, 6  , 51 },
        { 97 , 66 , 46 },
        { 152, 119, 88 },
        { 134, 52 , 4 },
        { 204, 110, 56 },
        { 245, 132, 30 },
        { 208, 140, 67 },
        { 242, 160, 0 },
        { 255, 199, 28 },
        { 233, 209, 44 },
        { 204, 217, 122 },
        { 198, 204, 98 },
        { 148, 151, 104 },
        { 198, 211, 118 },
        { 64 , 132, 23 },
        { 133, 192, 84 },
        { 97 , 121, 73 },
        { 148, 202, 131 },
        { 184, 201, 172 },
        { 80 , 196, 175 },
        { 110, 156, 145 },
        { 153, 180, 208 },
        { 145, 198, 250 },
        { 104, 130, 157 },
        { 86 , 142, 199 },
        { 20 , 101, 192 },
        { 6  , 42 , 139 },
        { 8  , 75 , 166 },
        { 93 , 74 , 163 },
        { 142, 127, 230 },
        { 116, 112, 139 },
        { 147, 77 , 165 },
        { 140, 113, 148 },
        { 246, 159, 167 },
        { 181, 128, 146 },
        { 202, 140, 237 },
        { 212, 102, 141 }
    };

    void Read(FifamReader &reader, UInt id);
    void Write(FifamWriter &writer, UInt id);
};
