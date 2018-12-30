#pragma once
#include "Color.h"
#include "FifamDbWriteable.h"
#include "FifamPropertied.h"
#include "FifamClubLink.h"
#include "FifamPlayer.h"
#include "FifamStaff.h"
#include "FifamTranslation.h"
#include "FifamStadiumType.h"
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
#include "FifamClubMerchandiseColor.h"
#include "FifamGeoCoords.h"
#include "FifamFormation.h"
#include "FifamClubTeamType.h"
#include "FifamClubLastSeasonInfo.h"
#include "FifamClubPenaltyType.h"
#include "FifamClubTeamColor.h"
#include "FifamCompID.h"

class FifamDatabase;
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
class FifamClub : public FifamDbWriteable, public FifamPropertied {
public:
    // @since FM08
    UInt mUniqueID = 0;
    // @since FM07
    // @maxsize 29
    FifamTrArray<String> mName;
    // @since FM07
    // @maxsize 29
    FifamTrArray<String> mName2;
    // @since FM07
    // @maxsize 10
    FifamTrArray<String> mShortName;
    // @since FM07
    // @maxsize 10
    FifamTrArray<String> mShortName2;
    // @since FM07
    // @maxsize 4
    FifamTrArray<String> mAbbreviation;
    // @since FM07
    // @maxsize 29
    FifamTrArray<String> mCityName;
    // @since FM07
    // @maxsize 19
    // not an array before FM11
    FifamTrArray<String> mPlayerInText;
    // @since FM07
    // @maxsize 19
    // not an array before FM11
    FifamTrArray<String> mFanName1;
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
    // @maxsize 29
    String mNewspaper;
    // @since FM07
    // @maxsize 19
    String mMascotName;
    // @since FM07
    FifamTrArray<UChar> mAbbreviationArticle = {};
    // @since FM07
    FifamTrArray<UChar> mPlayerInTextArticle = {};
    // @since FM07
    FifamTrArray<UChar> mFanName1Article = {};
    // @since FM07
    FifamTrArray<UChar> mClubNameUsageInPhrase = {};
    // @since FM07
    FifamTrArray<UChar> mClubNameUsageInPhrase2 = {};
    // @since FM07
    FifamClubLink mPartnershipClub;
    // @since FM07
    // @size 4
    Vector<FifamClubLink> mRivalClubs;
    // @since FM07
    UShort mYearOfFoundation = 1900;
    // @since FM07
    // can be negative
    Int mInitialCapital = 0;
    // @since FM07
    UInt mTransferBudget = 0;
    // @since FM07
    Bool mJointStockCompany = false;
    // @since FM07
    // 3 (1+2) - penalty points, 16 - ? (Club Sport Herediano), 64 - ? (Universitatea Craiova)
    FifamClubPenaltyType mPenaltyType;
    // @since FM07
    // 0 or negative
    Char mPenaltyPoints = 0;
    // @since FM07
    // @until FM09
    UInt mMaxAttendance = 0;
    // @since FM09
    // @range 0-250'000
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

    /* Staadium */

    // @since FM07
    // if higher than 999, set to 0
    UInt mStadiumVenue = 0;
    // @since FM12
    UInt mStadiumStandingsCapacity = 0;
    // @since FM07
    UInt mStadiumSeatsCapacity = 0;
    // @since FM12
    UInt mStadiumVipCapacity = 0;
    // @since FM07
    // @maxsize 29 in Editor
    FifamTrArray<String> mStadiumName;
    // @since FM07
    FifamStadiumType mStadiumType;
    // @since FM07
    Bool mStadiumBigMainStand = false;
    // @since FM07
    Bool mCanBeDeletedInEditor = false;

    /* Club grounds */

    // @since FM07
    // @range [0;6]
    UChar mClubFacilities = 0; // club centre in 07 (0-5), 0-6
    // @since FM07
    // @range [1;10]
    UChar mYouthCentre = 0; // youth department in 07 (0-5), 1-10 in FM12
    // @since FM07
    // @range [0;10]
    UChar mYouthBoardingSchool = 0; // youth department in 07 (0-5), 0-10 in FM12
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
    FifamClubLastSeasonInfo mFirstTeamLastSeasonInfo;
    // @since FM07
    FifamClubLastSeasonInfo mReserveTeamLastSeasonInfo;
    // @since FM07
    Bool mDoesNotTakePartInITCup = false;
    // @since FM07
    FifamClubBadge mBadge;
    // @since FM07
    ColorPair mClubColour;
    // @since FM07
    // unknown color (usually same as first color)
    ColorPair mClubColour2;
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
    Array<FifamNation, 2> mTransfersCountry;
    // @since FM07
    FifamNation mYouthPlayersCountry;
    // @since FM07
    UInt mFifaID = 0;
    // @since FM07
    FifamKit mKit;
    // @since FM07
    Vector<FifamCompID> mLowestLeagues;

    // @since FM08
    FifamClubMediaPressure mMediaPressure;
    // @since FM08
    Bool mTraditionalClub = false;
    // @since FM08
    Bool mCantPlayInLeague = false;
    // @since FM08
    Bool mRichGuyControlled = false;
    // @since FM08
    Bool mYouthPlayersAreBasques = false;

    // @since FM11
    FifamTrArray<String> mFanName2;
    // @since FM11
    FifamTrArray<String> mTermForTeam1;
    // @since FM11
    FifamTrArray<String> mTermForTeam2;
    // @since FM11
    FifamTrArray<UChar> mFanName2Article = {};
    // @since FM11
    FifamTrArray<UChar> mTermForTeam1Article = {};
    // @since FM11
    FifamTrArray<UChar> mTermForTeam2Article = {};
    // @since FM11
    UInt mSponsorAmount = 0;
    // @since FM11
    // years count
    UChar mSponsorDuration = 0;
    // @since FM11
    Bool mSpecialSponsor = false;
    // @since FM11
    UInt mIndividualTvMoney = 0;

    // @since FM12
    UInt mFanMembers = 0;

    // @since FM13
    Array<FifamFormation, 2> mPreferredFormations;
    // @since FM13
    Array<FifamPlayer *, 3> mCaptains = {};

    struct {
        // @since FM07
        // 0-18, 80 (Stuttgart), 192 (Wigan), unused
        UChar _2 = 0; // 0
        // @since FM07
        // @until FM12
        // always empty in 07, unused
        String _4;

        // @since FM07
        struct {
            // @since FM07
            Bool _2;
            // @since FM07
            Bool _4;
            // @since FM07
            Bool _8;
            // @since FM07
            Bool _16;
            // @since FM07
            Bool _32;
            // @since FM07
            Bool _64;
        } flags;
    } Unknown;

    FifamCountry *mCountry = nullptr;
    FifamDatabase *mDatabase = nullptr;
    Bool mIsNationalTeam = false;
    Vector<FifamPlayer *> mPlayers;
    Vector<FifamStaff *> mStaffs;

    inline static Vector<Pair<UInt, ColorPair>> mTeamColorsTable = {
        { 0,  {{ 20,101,192 } ,{ 0,0,0 }}},
        { 1,  {{ 20,101,192 } ,{ 11,166,3 }}},
        { 2,  {{ 20,101,192 } ,{ 245,30,2 }}},
        { 3,  {{ 20,101,192 } ,{ 255,255,255 }}},
        { 4,  {{ 20,101,192 } ,{ 255,246,0 }}},
        { 5,  {{ 117,12,7 }   ,{ 0,0,0 }}},
        { 6,  {{ 117,12,7 }   ,{ 145,198,250 }}},
        { 7,  {{ 117,12,7 }   ,{ 255,255,255 }}},
        { 8,  {{ 11,166,3 }   ,{ 0,0,0 }}},
        { 9,  {{ 11,166,3 }   ,{ 245,30,2 }}},
        { 10, {{ 11,166,3 }   ,{ 255,255,255 }}},
        { 11, {{ 11,166,3 }   ,{ 255,246,0 }}},
        { 12, {{ 145,198,250 },{ 0,0,0 }}},
        { 13, {{ 145,198,250 },{ 255,255,255 }}},
        { 14, {{ 255,138,0 }  ,{ 0,0,0 }}},
        { 15, {{ 255,138,0 }  ,{ 255,255,255 }}},
        { 16, {{ 172,104,248 },{ 0,0,0 }}},
        { 17, {{ 172,104,248 },{ 255,255,255 }}},
        { 18, {{ 245,30,2 }   ,{ 0,0,0 }}},
        { 19, {{ 245,30,2 }   ,{ 255,255,255 }}},
        { 20, {{ 245,30,2 }   ,{ 255,246,0 }}},
        { 21, {{ 255,246,0 }  ,{ 0,0,0 }}},
        { 22, {{ 255,246,0 }  ,{ 255,255,255 }}},
        { 23, {{ 255,255,255 },{ 0,0,0 }}},
        { 24, {{ 134,52,4 }   ,{ 255,255,255 }}},
        { 25, {{ 6,42,139 }   ,{ 255,255,255 }}},
        { 26, {{ 247,183,211 },{ 34,30,31 }}},
        { 27, {{ 240,169,47 } ,{ 162,29,52 }}},
        // added for better color detection
        { FifamClubTeamColor::Blue_Red, {{ 0, 48, 104 } ,{ 245,30,2 }}},
        { FifamClubTeamColor::Blue_Red, {{ 0, 16, 64 } ,{ 245,30,2 }}},
        { FifamClubTeamColor::Green_Red, {{ 0, 152, 120 } ,{ 200,16,24 }}},
        { FifamClubTeamColor::Green_White, {{ 72, 112, 96 } ,{ 255,255,255 }}},
        { FifamClubTeamColor::Green_Yellow, {{ 0, 120, 96 } ,{ 248,232,40 }}},
        { FifamClubTeamColor::Blue_Yellow, {{ 8, 32, 72 } ,{ 240,248,16 }}},
        { FifamClubTeamColor::Blue_Yellow, {{ 38, 72, 139 } ,{ 230,230,69 }}},
        { FifamClubTeamColor::Blue_Yellow, {{ 0, 32, 96 } ,{ 248,208,0 }}},
        { FifamClubTeamColor::Blue_Yellow, {{ 38, 72, 139 } ,{ 255,226,82 }}},
        { FifamClubTeamColor::Purple_White, {{ 80, 32, 120 } ,{ 248,248,248 }}}

    };
    inline static Vector<Pair<UInt, Color>> mMerchandiseColorsTable = {
        { 0, { 0, 174, 255 }},
        { 1, { 190, 13, 13 }},
        { 2, { 11, 166, 3 }},
        { 3, { 255, 246, 0 }},
        { 4, { 255, 255, 255 }}
    };
    inline static Vector<Pair<UInt, Color>> m07InterfaceColorsTable = {
        { 0,  { 0, 0, 0 }},
        { 1,  { 236, 235, 231 }},
        { 2,  { 63, 63, 62 }},
        { 3,  { 215, 149, 0 }},
        { 4,  { 255, 207, 0 }},
        { 5,  { 223, 0, 24 }},
        { 6,  { 53, 128, 0 }},
        { 7,  { 0, 82, 155 }},
        { 8,  { 46, 48, 146 }},
        { 9,  { 0, 137, 208 }},
        { 10, { 97, 54, 11 }},
        { 11, { 247, 184, 211 }},
        { 12, { 131, 24, 0 }},
        { 13, { 125, 89, 163 }},
        { 14, { 156, 125, 212 }},
        { 15, { 138, 140, 143 }},
        { 16, { 204, 204, 204 }},
        { 17, { 138, 140, 143 }},
        { 18, { 192, 146, 87 }},
        { 19, { 255, 222, 23 }},
        { 20, { 194, 226, 84 }},
        { 21, { 255, 204, 3 }},
        { 22, { 243, 194, 2 }},
        { 23, { 245, 160, 0 }},
        { 24, { 245, 125, 48 }},
        { 25, { 92, 49, 4 }},
        { 26, { 251, 195, 196 }},
        { 27, { 208, 50, 92 }},
        { 28, { 204, 0, 51 }},
        { 29, { 238, 49, 36 }},
        { 30, { 237, 27, 36 }},
        { 31, { 204, 0, 0 }},
        { 32, { 197, 35, 38 }},
        { 33, { 170, 0, 0 }},
        { 34, { 79, 174, 92 }},
        { 35, { 88, 162, 87 }},
        { 36, { 104, 176, 34 }},
        { 37, { 117, 164, 65 }},
        { 38, { 76, 132, 59 }},
        { 39, { 88, 135, 117 }},
        { 40, { 92, 97, 12 }},
        { 41, { 67, 172, 215 }},
        { 42, { 70, 160, 235 }},
        { 43, { 0, 128, 193 }},
        { 44, { 0, 136, 179 }},
        { 45, { 15, 72, 177 }},
        { 46, { 0, 92, 151 }},
        { 47, { 15, 72, 177 }},
        { 48, { 34, 84, 154 }},
        { 49, { 35, 34, 108 }},
        { 50, { 0, 96, 134 }},
        { 51, { 1, 27, 88 }},
        { 52, { 0, 0, 51 }},
        { 53, { 142, 49, 69 }},
        { 54, { 126, 114, 180 }},
        { 55, { 125, 48, 122 }},
        { 56, { 139, 45, 81 }},
        { 57, { 54, 53, 148 }},
        { 58, { 93, 29, 47 }},
        { 59, { 92, 49, 4 }},
        { 60, { 149, 85, 66 }},
        { 61, { 101, 101, 101 }},
        { 62, { 79, 79, 79 }},
        { 63, { 63, 63, 62 }}
    };
    inline static Vector<Pair<UInt, Color>> m08InterfaceColorsTable = {
        { 0,  { 69 , 120, 183 }},
        { 1,  { 37 , 37 , 39 }},
        { 2,  { 101, 101, 103 }},
        { 3,  { 230, 230, 230 }}, // Edited
        { 4,  { 134, 9  , 5 }},
        { 5,  { 198, 32 , 16 }},
        { 6,  { 245, 30 , 2 }},
        { 7,  { 139, 6  , 51 }},
        { 8,  { 97 , 66 , 46 }},
        { 9,  { 152, 119, 88 }},
        { 10, { 134, 52 , 4 }},
        { 11, { 204, 110, 56 }},
        { 12, { 245, 132, 30 }},
        { 13, { 208, 140, 67 }},
        { 14, { 242, 160, 0 }},
        { 15, { 255, 199, 28 }},
        { 16, { 233, 209, 44 }},
        { 17, { 204, 217, 122 }},
        { 18, { 198, 204, 98 }},
        { 19, { 148, 151, 104 }},
        { 20, { 198, 211, 118 }},
        { 21, { 64 , 132, 23 }},
        { 22, { 133, 192, 84 }},
        { 23, { 97 , 121, 73 }},
        { 24, { 148, 202, 131 }},
        { 25, { 184, 201, 172 }},
        { 26, { 80 , 196, 175 }},
        { 27, { 110, 156, 145 }},
        { 28, { 153, 180, 208 }},
        { 29, { 145, 198, 250 }},
        { 30, { 104, 130, 157 }},
        { 31, { 86 , 142, 199 }},
        { 32, { 20 , 101, 192 }},
        { 33, { 6  , 42 , 139 }},
        { 34, { 8  , 75 , 166 }},
        { 35, { 93 , 74 , 163 }},
        { 36, { 142, 127, 230 }},
        { 37, { 116, 112, 139 }},
        { 38, { 147, 77 , 165 }},
        { 39, { 140, 113, 148 }},
        { 40, { 246, 159, 167 }},
        { 41, { 181, 128, 146 }},
        { 42, { 202, 140, 237 }},
        { 43, { 212, 102, 141 }},
        { FifamClubInterfaceColor08::Aqua, { 0, 152, 120 }},
        { FifamClubInterfaceColor08::Seaweed, { 72, 112, 96 }},
        { FifamClubInterfaceColor08::Aqua, { 0, 120, 96 }},
        { FifamClubInterfaceColor08::Midnight, { 8, 32, 72 }},
        { FifamClubInterfaceColor08::Midnight, { 0, 32, 96 }},
        { FifamClubInterfaceColor08::Midnight, { 32, 32, 72 }},
        { FifamClubInterfaceColor08::Midnight, { 16, 16, 80 }},
        { FifamClubInterfaceColor08::Forest, { 82, 181, 97 }},
        { FifamClubInterfaceColor08::Forest, { 60, 181, 113 }},
        { FifamClubInterfaceColor08::Forest, { 60, 140, 87 }},
        { FifamClubInterfaceColor08::Forest, { 0, 103, 55 }},
        { FifamClubInterfaceColor08::Forest, { 0, 157, 58 }},
        { FifamClubInterfaceColor08::Forest, { 8, 136, 88 }},
        { FifamClubInterfaceColor08::Forest, { 8, 104, 64 }},
        { FifamClubInterfaceColor08::Indigo, { 80, 0, 128 }}
    };

    void WriteClubMembers(FifamWriter &writer);
    void ReadClubMembers(FifamReader &reader);
    void Read(FifamReader &reader, UInt id);
    void Write(FifamWriter &writer, UInt id);
};
