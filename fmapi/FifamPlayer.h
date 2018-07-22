#pragma once
#include "FifamTypes.h"
#include "FifamDate.h"
#include "FifamLanguage.h"
#include "FifamReadWrite.h"
#include "FifamPlayerAppearance.h"
#include "FifamNation.h"

class FifamClub;

// @since FM07
class FifamPlayer {
public:
    // @since FM07
    UInt mID = 0;
    // @UNKNOWN
    enum class Type { Player, Manager, Chairman, President, Stuff }
        mType = Type::Player;

    // @UNKNOWN
    UInt mCommentaryId = 0;
    // @since FM07
    // @maxsize 19
    String mLastName;
    // @since FM07
    // @maxsize 15
    String mFirstName;
    // @since FM07
    // @maxsize 19 FM07
    // @maxsize 29 FM13
    String mPseudonym;
    // @since FM07
    // @maxsize 19
    String mNickname;
    // @since FM07
    FifamNation mNationality[2];
    // @since FM07
    FifamLanguage mLanguages[3];
    // @since FM07
    FifamDate mBirthday;
    // @since FM07
    // there are two boolean flags, `IsReserve` and `IsYouth`
    // if none of them enabled then player is in the first team
    // both flags can be enabled in FM07, can't be enabled since FM08
    enum class Team { First, Reserve, Youth }
        mTeam = Team::First;
    
    // @since FM07
    // called as `Does not Count as a Foreign Player by Individual Decision of Local FA` in FM07
    Bool mNaturalised = false;
    // @since FM07
    Bool mBasque = false; 
    // @since FM07
    Bool mRealPlayer = false;

    UChar mTalent = 0; // 0-9
    UChar mPotential = 0; // FM07 only
    UChar mLeftFoot = 0;
    UChar mRightFoot = 0;
    UChar mHeroStatus = 0; // 0-10
    UChar mKitNumber[2] = { 0, 0 }; // only one in old FMs
    UChar mNationalExperience = 0; // experience in new FMs
    UChar mInternationalExperience = 0; // experience in new FMs

    // @since FM07
    enum class Position { NA, GK, RB, LB, CB, SW, RWB, LWB, AC, DM, RM, LM, CM, RW, LW, AM, CF, ST }
        mMainPosition = Position::NA;

    // @since UNKNOWN
    UChar mPositionBias[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // @since FM07
    enum class PlayingStyle {
        None,
        AttackingFB, // Attacking full-back
        DefenceFB, // Defensive full-back
        Libero, // Builds up
        SimplePasser,
        BallWinner,
        HardMan, // Fighter
        Holding, // Closes gaps
        BallWinnerMidfield,
        BoxToBox,
        Busy,
        PlayMaker,
        Dribbler,
        Winger,
        TargetMan,
        PenaltyBox,
        RunsChannels,
        PullsWideLeft,
        PullsWideRight,
        DribblerAttack,
        HoldsUp,
        // new FMs
        BusyAttacker,
        TowerStrength,
        DistanceShooter
    } mPlayingStyle = PlayingStyle::None;

    // @since FM07
    struct Skills {
        UChar BallControl = 0; // FM13
        UChar Volleys = 0; // FM13
        UChar Dribbling = 0;
        UChar Finishing = 0;
        UChar ShotPower = 0;
        UChar LongShots = 0;
        UChar Crossing = 0;
        UChar Passing = 0;
        UChar LongPassing = 0;
        UChar Header = 0;
        UChar TackleStanding = 0; // Tackling in FM07
        UChar TackleSliding = 0; // Tackling in FM07
        UChar ManMarking = 0;
        UChar Technique = 0; // FM07
        UChar Creativity = 0; // FM07
        UChar Flair = 0; // FM07
        UChar Touch = 0; // FM07

        // Physical skills

        UChar Balance = 0; // FM13
        UChar Acceleration = 0;
        UChar Pace = 0;
        UChar Agility = 0;
        UChar Jumping = 0;
        UChar Strength = 0;
        UChar Stamina = 0;
        UChar WorkRate = 0;
        UChar ForwardRuns = 0; // FM07

        // Mental skills

        UChar PosOffensive = 0; // FM13
        UChar PosDefensive = 0; // FM13
        UChar Vision = 0; // FM13
        UChar Reactions = 0; // FM13
        UChar TacticAwareness = 0; // FM13*
        UChar Aggression = 0;
        UChar Composure = 0;
        UChar Consistency = 0;
        UChar Leadership = 0; // FM07*
        UChar Anticipation = 0; // FM07
        UChar Concentration = 0; // FM07
        UChar Decision = 0; // FM07
        UChar Determination = 0; // FM07
        UChar TeamWork = 0; // FM07
        UChar Initimidation = 0; // FM07

        // Set pieces

        UChar FreeKicks = 0;
        UChar Corners = 0;
        UChar PenaltyShot = 0;

        // Goalkeeper skills

        UChar Diving = 0; // FM13
        UChar Reflexes = 0; // FM13
        UChar Handling = 0;
        UChar Positioning = 0;
        UChar OneOnOne = 0;
        UChar Kicking = 0;
        UChar Punching = 0; // FM07
        UChar ShotStopping = 0; // FM07
        UChar Throwing = 0; // FM07
    };

    // @since FM07
    Skills mSkills;

    // Character

    struct Character {
        // FM07,08 only
        Bool AttitudeToWin = false; // 0x01
        Bool Complacent = false; // 0x02
        //
        Bool StrongNerves = false; // 0x04
        Bool WeakNerves = false;
        Bool Fairness = false;
        Bool HardMan = false;
        Bool Introvert = false;
        Bool IdentificationHigh = false;
        Bool IdentificationLow = false;
        Bool LanguageGenius = false;
        Bool MediaDarling = false;
        Bool ScandalProne = false;
        Bool Flexibility = false;
        Bool Inflexibility = false;
        Bool Ambition = false;
        Bool HighTrainingWorkRate = false;
        Bool LazyInTraining = false;
        Bool Temperament = false;
        Bool FansFavorite = false;
        Bool Adaptability = false;
        Bool Professionalism = false;
        Bool LongThrows = false;
        Bool DrivenFreeKicks = false;
        // since FM13
        Bool Diva = false; // 0x01
        Bool LifestyleIcon = false; // 0x02
        Bool InjuryProne = false; // 0x800000
        Bool Composure = false;
        Bool Teamplayer = false;
        Bool Egoist = false;
        Bool LacksAmbition = false;
        Bool LittleInjuryProne = false;
        Bool LongThrowOuts = false;
        Bool UninterestedInMedia = false;
        Bool Forgiving = false;
        Bool NotForgiving = false;
        Bool HappyMan = false;
        Bool Critic = false;
        Bool Extrovert = false;
        Bool Independent = false;
        Bool NeedsAttention = false;
        Bool DoesntLikeNewLang = false;
        Bool Unknown = false;
    };

    Character mCharacter;

    // Appearance

    UInt mSpecialFace = 0; // special FIFA face ID
    UChar mGenericFace = 0;
    UChar mSkinColour = 0;
    UChar mFaceVariation = 0;

    enum class EyeColour { Blue, Brown, GrayGreen, Green, GreenBlue, Grey, LightBlue };

    EyeColour mEyeColour = EyeColour::Grey;

    enum class HairColour { Black, Blonde, Brown, MediumBlonde, Red, /*FM13*/ DarkBrown, LightBrown, PlatinumBlonde };

    HairColour mHairColour = HairColour::Black;

    UChar mHairStyle = 0;

    Bool mSideburns = false;

    enum class BeardType { None, Shadow, Tash, Goatee, FullGoatee, FullBeard, /*FM13*/ Unshaven, ChinBeard, KevinBeard };

    BeardType mBeardType = BeardType::None;

    enum class BeardColour { Black, Blonde, Brown, MediumBlonde, Red };

    BeardColour mBeardColour = BeardColour::Black;

    // @since FM07
    UChar mHeight;

    // @since FM07
    UChar mWeight;

    // @since FM07
    enum class ShoeType {
        Black, White, Red, Blue, Yellow, WhiteBlack, WhiteBlue, WhiteLightBlue, YellowBlack,
        /* since FM13 */ Green, Grey, Orange, Magenta, BrightRed, DarkBlue, LightGreen, LightYellow, WhiteRed, BlackRed
    } mShoeType = ShoeType::Black;

    // @only FM07
    FifamClub *mFirstClub = nullptr;

    // @only FM07
    FifamClub *mPreviousClub = nullptr;

    // @since FM07
    // @format yyyy FM07
    // @format dd-mm-yyyy FM13
    FifamDate mJoinedClubDate;

    // @since FM07
    UChar mNationalTeamMatches = 0;

    // @since FM07
    UChar mNationalTeamGoals = 0;

    // @since FM07
    Bool mCurrentlyInNationalTeam = false;

    // @since FM07
    Bool mRetiredFromNationalTeam = false;

    // @since FM07
    struct HistoryRecord {
        // @since FM07
        FifamDate mStartDate;

        // @since FM07
        FifamDate mEndDate;

        // @since FM07
        FifamClub *mClub = nullptr;

        // @since FM07
        UShort mMatches = 0;

        // @since FM07
        UShort mGoals = 0;

        // @since FM08
        UShort mReserveMatches = 0;

        // @since FM08
        UShort mReserveGoals = 0;

        // @since FM07
        UInt mTransferFee = 0;

        // @since FM07
        Bool mLoan = false;

        // @since FM08
        Bool mStillInClub = false;

        // @since UNKNOWN
        Bool mWasACaptain = false;
    };

    // @since FM07
    std::vector<HistoryRecord> mHistoryRecords;

    // Contract

    FifamDate mContractEndDate; // dd-mm-yyyy in FM13, yyyy in FM07
    UInt mBasicSalary = 0;
    UInt mReleaseClause = 0; // 'fix transfer fee' in FM07
    UChar mOptionClub = 0; // max 1 in FM07
    UChar mOptionPlayer = 0; // max 1 in FM07

    // @since FM07
    FifamClub *mFavouriteClub = nullptr;

    // @since FM07
    FifamClub *mWouldnSignFor = nullptr;

    // @since FM07
    UShort mFavouriteCountry = 0;

    // @since FM13
    FifamClub *mTransferRumors[3] = { nullptr, nullptr, nullptr };

    // @since FM07
    // @maxsize 64 FM13
    String mComment;


    // Unknown data
    struct {
        UChar _1;
    } Unknown;

    void Read(FifamReader &reader) {

        Bool bReserveTeam;
        UChar nFlags;

        reader.ReadLine(mID);
        reader.ReadStartIndex(L"PLAYER");
        reader.ReadLine(mFirstName);
        reader.ReadLine(mLastName);
        reader.ReadLine(mNickname);
        reader.ReadLine(mPseudonym);
        reader.ReadLine(Unknown._1);
        reader.ReadLine(bReserveTeam);
        reader.ReadLine(mNationality[0]);
        reader.ReadLine(mNationality[1]);
        reader.ReadLine(nFlags);
        if (nFlags & 1)
            mNaturalised = true;
        if (nFlags & 2)
            mBasque = true;
        if (nFlags & 4)
            mRealPlayer = true;
        reader.ReadLine(mBirthday);
        reader.ReadLine(nFlags);

        if (nFlags & 0x40) // youth team
            mTeam = Team::Youth;
        else if (bReserveTeam)
            mTeam = Team::Reserve;
        else
            mTeam = Team::First;

        mTalent = ((nFlags & 7) + 1) * 2 - 1;



        reader.ReadEndIndex(L"PLAYER");
    }
};
