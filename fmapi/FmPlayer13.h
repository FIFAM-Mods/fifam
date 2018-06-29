#pragma once

class Condition {
public:
    int m_nType;
    union {
        struct _Ban {
            int a;
            int m_nEndDate;
            int c;
            int d;
            int e;
        } Ban;
        struct _LeagueBan {
            int a;
            int b;
            int c;
            int m_nNumMatches;
            int e;
        } LeagueBan;
        struct _Retirement {
            int a;
            int b;
            int c;
            int d;
            int e;
        } Retirement;
        struct _Injury {
            int m_nStart;
            int m_nEnd;
            int c;
            int m_nInjuryType;
            int e;
        } Injury;
        struct _Loan {
            int m_nStart;
            int m_nEnd;
            unsigned int m_nClubId;
            int m_nBuyValue; // -1 = ingame generated
            int e;
        } Loan;
        struct _WillMoveTo {
            int m_nStart;
            int m_nContractEnd;
            unsigned int m_nClubId;
            int m_nTransferFee;
            int e;
        } WillMoveTo;
        struct _WillJoin {
            int m_nDate;
            int b;
            int c;
            int d;
            int e;
        } WillJoin;
        struct _Leaves {
            int m_nDate;
            int b;
            int c;
            int d;
            int e;
        } Leaves;
        struct _WillBeLoaned {
            int m_nStart;
            int m_nEnd;
            unsigned int m_nClubId;
            int m_nBuyValue; // -1 = ingame generated
            int m_nLoanFee;
        } WillBeLoaned;
    };

    void Read(Reader &reader)
    {
        reader.ReadParamsArray(&m_nType, 6);
    }

    void Write(Writer &writer)
    {
        writer.WriteParams(6, m_nType, Retirement.a, Retirement.b, Retirement.c, Retirement.d, Retirement.e);
    }
};

class PlayerHistory
{
public:
    struct ContractRecord {
        Date m_start;
        Date m_end;
        unsigned int m_nClubId;
        int m_bWasACaptain;
        int m_nMatches;
        int m_nReserveMatches;
        int m_nGoals;
        int m_nReserveGoals;
        int m_nTransferFee;
        int m_nState;

        ContractRecord()
        {
            memset(this, 0, sizeof(ContractRecord));
        }

        void Read(Reader &reader)
        {
            reader.ReadDate(m_start);
            reader.ReadDate(m_end);
            reader.ReadParamsArray(&m_nClubId, 8);
        }

        void Write(Writer &writer)
        {
            writer.WriteDate(m_start);
            writer.WriteDate(m_end);
            writer.WriteParams(8, m_nClubId, m_bWasACaptain, m_nMatches, m_nReserveMatches, m_nGoals, m_nReserveGoals,
                m_nTransferFee, m_nState);
        }
    };
    vector<ContractRecord> records;

    void Read(Reader &reader)
    {
        reader.SkipLines(1);
        int numRecords = reader.ReadOneParam();
        records.resize(numRecords);
        for (int i = 0; i < numRecords; i++)
            records[i].Read(reader);
        reader.SkipLines(1);
    }

    void Write(Writer &writer)
    {
        writer.WriteLine(L"%INDEX%HIST");
        writer.WriteParam(records.size());
        for (int i = 0; i < records.size(); i++)
            records[i].Write(writer);
        writer.WriteLine(L"%INDEXEND%HIST");
    }
};

class PlayerContract
{
public:
    int m_dwAppearancesInternationalClubMatchBonus;
    int unknown1;
    int m_dwSalaryImprovementPerYear;
    int m_dwSalaryDecreaseInCaseOfRelegation;
    union {
        int m_nReleaseClauseFlags;
        struct {
            unsigned int m_bEnableReleaseClauseForClubsFromAbroad : 1; // bit 0
            unsigned int m_bEnableReleaseClauseForClubsFromHigherLeagues : 1; // bit 1
            unsigned int m_bEnableReleaseClauseInCaseOfRelegation : 1; // bit 2
            unsigned int m_bEnableReleaseClauseIfClubIsNotPlayingInternational : 1; // bit 3
            unsigned int m_bEnableReleaseClauseInCaseOfMissingPromotion : 1; // bit 4
        };
    };
    int m_nAutomaticExtensionAfterMatches;
    int unknown2;
    int unknown3;
    int unknown4;
    int m_nLeagueMatchesAppearanceBonus;
    int m_nParticipationInCaseOfTransfer;
    union {
        int m_nFlags;
        struct {
            unsigned int : 2;
            unsigned int m_nOptionForClub : 2;
            unsigned int : 1;
            unsigned int m_bContractWillExtendInCaseOfAvoidingRelegation : 1;
            unsigned int m_bJobInClubAfterActiveCareer : 1;
            unsigned int : 6;
            unsigned int m_nOptionForPlayer : 2;
        };
    };
    int unknown5;
    int m_dwBasicSalary;
    int m_dwGeneralMatchBonus;
    int m_dwLeagueMatchBonus;
    int m_dwBonusIcnCaseOfAvoidingRelegation;
    int m_dwBonusIcnCaseOfPromotion;
    int m_dwInternationalTeamDebut;
    int m_dwCleanSheetBonus;
    int m_dwReleaseClause;
    int m_dwMinimumFeeForParticipationInCaseOfTransfer;
    int m_dwInternationalClubMatchBonus;
    Date m_joinedClubDate;
    Date m_contractValidUntil;
    int m_dwReleaseClauseForClubsFromAbroad;
    int m_dwReleaseClauseForClubsFromHigherLeagues;
    int m_dwReleaseClauseInCaseOfRelegation;
    int m_dwReleaseClauseIfClubIsNotPlayingInternational;
    int m_dwReleaseClauseInCaseOfMissingPromotion;
    int m_bBuyBackClauseEnabled;
    int m_dwBuyBackClauseCost;
    unsigned int m_nBuyBackClauseClubId;
    Date m_buyBackClauseValidUntil;

    void Read(Reader &reader)
    {
        reader.SkipLines(1);
        reader.ReadLineWithDate(&m_dwAppearancesInternationalClubMatchBonus, 23, m_joinedClubDate);
        reader.ReadDate(m_contractValidUntil);
        reader.ReadParamsArray(&m_dwReleaseClauseForClubsFromAbroad, 5);
        if (reader.IsVersionGreaterOrEqual(0x2013, 0x9)) // 2013.09 Added buy-back clause line (player)
            reader.ReadLineWithDate(&m_bBuyBackClauseEnabled, 3, m_buyBackClauseValidUntil);
        else {
            m_bBuyBackClauseEnabled = 0;
            m_dwBuyBackClauseCost = 0;
            m_nBuyBackClauseClubId = 0;
            m_buyBackClauseValidUntil.Set(0, 0, 0);
        }
        reader.SkipLines(1);
    }

    void Write(Writer &writer)
    {
        writer.WriteLine(L"%INDEX%CONTRACT");
        writer.WriteParamsWithDate(m_joinedClubDate, 23, m_dwAppearancesInternationalClubMatchBonus,
            unknown1,
            m_dwSalaryImprovementPerYear,
            m_dwSalaryDecreaseInCaseOfRelegation,
            m_nReleaseClauseFlags,
            m_nAutomaticExtensionAfterMatches,
            unknown2,
            unknown3,
            unknown4,
            m_nLeagueMatchesAppearanceBonus,
            m_nParticipationInCaseOfTransfer,
            m_nFlags,
            unknown5,
            m_dwBasicSalary,
            m_dwGeneralMatchBonus,
            m_dwLeagueMatchBonus,
            m_dwBonusIcnCaseOfAvoidingRelegation,
            m_dwBonusIcnCaseOfPromotion,
            m_dwInternationalTeamDebut,
            m_dwCleanSheetBonus,
            m_dwReleaseClause,
            m_dwMinimumFeeForParticipationInCaseOfTransfer,
            m_dwInternationalClubMatchBonus);
        writer.WriteDate(m_contractValidUntil);
        writer.WriteParams(5, m_dwReleaseClauseForClubsFromAbroad, m_dwReleaseClauseForClubsFromHigherLeagues,
            m_dwReleaseClauseInCaseOfRelegation, m_dwReleaseClauseIfClubIsNotPlayingInternational,
            m_dwReleaseClauseInCaseOfMissingPromotion);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x9)) // 2013.09 Added buy-back clause line (player)
            writer.WriteParamsWithDate(m_buyBackClauseValidUntil, 3, m_bBuyBackClauseEnabled,
                m_dwBuyBackClauseCost, m_nBuyBackClauseClubId);
        writer.WriteLine(L"%INDEXEND%CONTRACT");
    }
};

class Player
{
public:
    int m_nId;
    int m_nType;
    wchar_t m_szFirstName[64];
    wchar_t m_szLastName[64];
    wchar_t m_szPseudonym[64];
    wchar_t m_szNickname[64];
    int m_nNationality;
    int m_n2ndNationality;
    int m_nLanguages[3];
    int unknown1; // unused 4th language
    Date m_birthday;
    int m_nTalent; // 0 - 9
    int unknown2; // related to leadership
    int m_nLeadership; // 1 - 10
    int m_nExperience; // 0 - 18
    int m_nPosition;
    int m_nPositionBias[NUM_POSITIONS];
    struct Ability {
        union {
            struct {
                int BallControl;
                int Dribbling;
                int Finishing;
                int ShotPower;
                int LongShots;
                int Volleys;
                int Crossing;
                int Passing;
                int LongPassing;
                int Header;
                int TackleStanding;
                int TackleSliding;
                int ManMarking;
                int Acceleration;
                int Pace;
                int Agility;
                int Jumping;
                int Strength;
                int Stamina;
                int Balance;
                int PosOffensive;
                int PosDefensive;
                int Vision;
                int Aggression;
                int Reactions;
                int Composure;
                int Consistency;
                int TacAwareness;
                int FreeKicks;
                int Corners;
                int PenaltyShot;
                int Diving;
                int Handling;
                int Positioning;
                int OneOnOne;
                int Reflexes;
                int Kicking;
            };
            int AttributeValues[37];
        };

        float CalcOverall(ePosition Position) {
            PosData_FM *fmPosData = GetPosData();
            float overall = 0.0f;
            for (int i = 0; i < 37; i++) {
                overall += (float)AttributeValues[i] * ((float)fmPosData[i].values[Position] / 100.0f);
            }
            return overall;
        }
    } m_ability;
    int m_nTeam;
    union {
        int m_nFlags;
        struct {
            unsigned int m_bNaturalised : 1;
            unsigned int m_bBasque : 1;
            unsigned int m_bRealPlayer : 1;
            unsigned int m_bYouthSquad : 1;
            unsigned int m_bRetiredFromNationalTeam : 1;
            unsigned int m_bCurrentlyInNationalTeam : 1;
            unsigned int m_bNoContractExtension : 1;
            unsigned int m_bCaptain : 1;
        };
    };
    union {
        int m_nLeftRightFootFlags;
        struct {
            unsigned int m_nRightFoot : 4; // bits 0 - 3
            unsigned int m_nLeftFoot : 4; // bits 4 - 7
        };
    };
    int m_nHero; // 0 - 10
    int m_nPlayingStyle;
    union {
        __int64 m_n64CharacterFlags;
        struct Character {
            unsigned __int64 Diva : 1;
            unsigned __int64 LifestyleIcon : 1;
            unsigned __int64 StrongNerves : 1;
            unsigned __int64 WeakNerves : 1;
            unsigned __int64 Fairness : 1;
            unsigned __int64 HardMan : 1;
            unsigned __int64 Introvert : 1;
            unsigned __int64 IdentificationHigh : 1;
            unsigned __int64 IdentificationLow : 1;
            unsigned __int64 LanguageGenius : 1;
            unsigned __int64 MediaDarling : 1;
            unsigned __int64 ScandalProne : 1;
            unsigned __int64 Flexibility : 1;
            unsigned __int64 Inflexibility : 1;
            unsigned __int64 Ambition : 1;
            unsigned __int64 HighTrainingWorkRate : 1;
            unsigned __int64 LazyInTraining : 1;
            unsigned __int64 Temperament : 1;
            unsigned __int64 FansFavorite : 1;
            unsigned __int64 Adaptability : 1;
            unsigned __int64 Professionalism : 1;
            unsigned __int64 LongThrows : 1;
            unsigned __int64 DrivenFreeKicks : 1;
            unsigned __int64 InjuryProne : 1;
            unsigned __int64 Composure : 1;
            unsigned __int64 Teamplayer : 1;
            unsigned __int64 Egoist : 1;
            unsigned __int64 LacksAmbition : 1;
            unsigned __int64 LittleInjuryProne : 1;
            unsigned __int64 LongThrowOuts : 1;
            unsigned __int64 UninterestedInMedia : 1;
            unsigned __int64 Forgiving : 1;
            unsigned __int64 NotForgiving : 1;
            unsigned __int64 HappyMan : 1;
            unsigned __int64 Critic : 1;
            unsigned __int64 Extrovert : 1;
            unsigned __int64 Independent : 1;
            unsigned __int64 NeedsAttention : 1;
            unsigned __int64 DoesntLikeNewLang : 1;
            unsigned __int64 Unknown : 1;
        } m_character;
    };
    int m_nFace;
    int m_nHairStyle;
    int m_nHairColour;
    int m_nBeard;
    int m_nSideburns;
    int m_nBeardColour;
    int m_nSkinColour;
    int m_nVariation;
    int m_nEyeColour;
    int m_nShoeType;
    int unknown3;
    int m_nSpecialFace;
    int m_nCommentId;
    int m_nHeight;
    int m_nWeight;
    int m_nShirtNumber[2];
    int m_nPlayerAgent;
    int m_nMatchesForNationalTeam;
    int m_nGoalsForNationalTeam;
    unsigned int m_nFavouriteClub;
    unsigned int m_nWouldnSignFor;
    unsigned int m_nTransferRumors[3];
    int m_nNumConditions;
    Condition m_conditions[4];
    PlayerHistory m_playerHistory;
    PlayerContract m_playerContract;
    int unknown4[4]; // zeros
    int m_nPreferredFormation; // 0-11
    int m_nChairmanStability;
    wchar_t m_szComment[64];

    unsigned char fifaRating;
    unsigned char fifaPotential;
    unsigned char fifaHistoryRating;

    unsigned int fifaTraits;

    char m_szQuickName[128];
    bool m_bQuickNameGenerated;

    char m_szQuickNameOriginal[128];
    bool m_bQuickNameOriginalGenerated;

    bool isFromFMNewPatch;

    class Player *m_newPlayer;

    ePlayerPosition m_teamPos;

    Player()
    {
        memset(this, 0, sizeof(Player));
        m_teamPos = POS_RES;
    }

    ~Player()
    {

    }

    void Read(Reader &reader, bool readId = true)
    {
        if (readId)
            m_nId = reader.ReadOneParam();
        else
            m_nId = 0;
        reader.SkipLines(1);
        m_nType = reader.ReadOneParam();
        reader.ReadSeparatedStringLine(4);
        CopyStr(m_szFirstName, reader.m_params[0].str_p, 64);
        CopyStr(m_szLastName, reader.m_params[1].str_p, 64);
        CopyStr(m_szPseudonym, reader.m_params[2].str_p, 64);
        CopyStr(m_szNickname, reader.m_params[3].str_p, 64);

        reader.ReadParams(2);
        m_nNationality = reader.m_params[0].int_p;
        m_n2ndNationality = reader.m_params[1].int_p;
        reader.ReadParams(4);
        m_nLanguages[0] = reader.m_params[0].int_p;
        m_nLanguages[1] = reader.m_params[1].int_p;
        m_nLanguages[2] = reader.m_params[2].int_p;
        unknown1 = reader.m_params[3].int_p;
        reader.ReadDate(m_birthday);
        if (reader.IsVersionGreaterOrEqual(0x2013, 0x4)) { // 2013.04 Added "Leadership" as separated value (player)
            reader.ReadParams(5);
            m_nTalent = reader.m_params[0].int_p;
            unknown2 = reader.m_params[1].int_p;
            m_nLeadership = reader.m_params[2].int_p;
            m_nExperience = reader.m_params[3].int_p;
            m_nPosition = reader.m_params[4].int_p;
        }
        else {
            reader.ReadParams(4);
            m_nTalent = reader.m_params[0].int_p;
            unknown2 = reader.m_params[1].int_p;
            m_nExperience = reader.m_params[2].int_p;
        }
        if (!reader.IsVersionGreaterOrEqual(0x2012, 0x5)) // Changed positions (player)
            m_nPosition = fm12to13pos[m_nPosition];
        reader.ReadParamsArray(m_nPositionBias, 14);
        reader.ReadParamsArray(&m_ability.BallControl, 37);
        m_nTeam = reader.ReadOneParam();
        m_nFlags = reader.ReadOneParam();
        m_nLeftRightFootFlags = reader.ReadOneParam();
        m_nHero = reader.ReadOneParam();
        m_nPlayingStyle = reader.ReadOneParam();
        m_n64CharacterFlags = reader.ReadOneI64Param();
        reader.ReadParamsArray(&m_nFace, 11);
        reader.ReadParamsArray(&m_nSpecialFace, 6);
        m_nPlayerAgent = reader.ReadOneParam();
        reader.ReadParamsArray(&m_nMatchesForNationalTeam, 2);
        m_nFavouriteClub = reader.ReadOneParam();
        m_nWouldnSignFor = reader.ReadOneParam();
        if (reader.IsVersionGreaterOrEqual(0x2013, 0x2)) {
            m_nTransferRumors[0] = reader.ReadOneParam();
            m_nTransferRumors[1] = reader.ReadOneParam();
            m_nTransferRumors[2] = reader.ReadOneParam();
        }
        else {
            m_nTransferRumors[0] = 0;
            m_nTransferRumors[1] = 0;
            m_nTransferRumors[2] = 0;
        }
        m_nNumConditions = reader.ReadOneParam();
        for (int i = 0; i < m_nNumConditions; i++)
            m_conditions[i].Read(reader);
        m_playerHistory.Read(reader);
        m_playerContract.Read(reader);
        reader.ReadParamsArray(unknown4, 4);
        m_nPreferredFormation = reader.ReadOneParam();
        m_nChairmanStability = reader.ReadOneParam();
        reader.ReadString(m_szComment, 64);
        reader.SkipLines(1);
    }

    void Write(Writer &writer, bool writeId = true) {
        if (writeId)
            writer.WriteParam(m_nId);
        writer.WriteLine(L"%INDEX%PLAYER");
        writer.WriteParam(m_nType);
        writer.WriteFormattedLine(L"%s|%s|%s|%s|0", m_szFirstName, m_szLastName, m_szPseudonym, m_szNickname);
        writer.WriteParams(2, m_nNationality, m_n2ndNationality);
        writer.WriteParams(4, m_nLanguages[0], m_nLanguages[1], m_nLanguages[2], unknown1);
        writer.WriteDate(m_birthday);
        unsigned char position = m_nPosition;
        if (!writer.IsVersionGreaterOrEqual(0x2012, 0x5)) // Changed positions (player)
            position = fm13to12pos[m_nPosition];
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x4)) // 2013.04 Added "Leadership" as separated value (player)
            writer.WriteParams(5, m_nTalent, unknown2, m_nLeadership, m_nExperience, position);
        else
            writer.WriteParams(4, m_nTalent, unknown2, m_nExperience, m_nPosition);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x1)) {
            writer.WriteParams(14, m_nPositionBias[POSITION_INVALID],
                m_nPositionBias[POSITION_GK],
                m_nPositionBias[POSITION_RB],
                m_nPositionBias[POSITION_LB],
                m_nPositionBias[POSITION_CB],
                m_nPositionBias[POSITION_DM],
                m_nPositionBias[POSITION_RM],
                m_nPositionBias[POSITION_LM],
                m_nPositionBias[POSITION_CM],
                m_nPositionBias[POSITION_RW],
                m_nPositionBias[POSITION_LW],
                m_nPositionBias[POSITION_AM],
                m_nPositionBias[POSITION_CF],
                m_nPositionBias[POSITION_ST]);
        }
        else {
            unsigned char positionBias[18];
            positionBias[FM12_POSITION_INVALID] = 0;
            positionBias[FM12_POSITION_GK] = GetPlayerFIFAOverall(POSITION_GK);
            positionBias[FM12_POSITION_RB] = GetPlayerFIFAOverall(POSITION_GK);
            positionBias[FM12_POSITION_LB] = GetPlayerFIFAOverall(POSITION_RB);
            positionBias[FM12_POSITION_CB] = GetPlayerFIFAOverall(POSITION_LB);
            positionBias[FM12_POSITION_SW] = GetPlayerFIFAOverall(POSITION_CB);
            positionBias[FM12_POSITION_RWB] = GetPlayerFIFAOverall(POSITION_CB);
            positionBias[FM12_POSITION_LWB] = GetPlayerFIFAOverall(POSITION_RB);
            positionBias[FM12_POSITION_AC] = GetPlayerFIFAOverall(POSITION_LB);
            positionBias[FM12_POSITION_DM] = GetPlayerFIFAOverall(POSITION_CB);
            positionBias[FM12_POSITION_RM] = GetPlayerFIFAOverall(POSITION_DM);
            positionBias[FM12_POSITION_LM] = GetPlayerFIFAOverall(POSITION_RM);
            positionBias[FM12_POSITION_CM] = GetPlayerFIFAOverall(POSITION_LM);
            positionBias[FM12_POSITION_RW] = GetPlayerFIFAOverall(POSITION_CM);
            positionBias[FM12_POSITION_LW] = GetPlayerFIFAOverall(POSITION_RW);
            positionBias[FM12_POSITION_AM] = GetPlayerFIFAOverall(POSITION_LW);
            positionBias[FM12_POSITION_CF] = GetPlayerFIFAOverall(POSITION_AM);
            positionBias[FM12_POSITION_ST] = GetPlayerFIFAOverall(POSITION_CF);
            writer.WriteParams(18, positionBias[FM12_POSITION_INVALID],
                positionBias[FM12_POSITION_GK],
                positionBias[FM12_POSITION_RB],
                positionBias[FM12_POSITION_LB],
                positionBias[FM12_POSITION_CB],
                positionBias[FM12_POSITION_SW],
                positionBias[FM12_POSITION_RWB],
                positionBias[FM12_POSITION_LWB],
                positionBias[FM12_POSITION_AC],
                positionBias[FM12_POSITION_DM],
                positionBias[FM12_POSITION_RM],
                positionBias[FM12_POSITION_LM],
                positionBias[FM12_POSITION_CM],
                positionBias[FM12_POSITION_RW],
                positionBias[FM12_POSITION_LW],
                positionBias[FM12_POSITION_AM],
                positionBias[FM12_POSITION_CF],
                positionBias[FM12_POSITION_ST]);
        }
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x1)) {
            writer.WriteParams(37, m_ability.BallControl, m_ability.Dribbling, m_ability.Finishing, m_ability.ShotPower, m_ability.LongShots,
                m_ability.Volleys, m_ability.Crossing, m_ability.Passing, m_ability.LongPassing, m_ability.Header, m_ability.TackleStanding,
                m_ability.TackleSliding, m_ability.ManMarking, m_ability.Acceleration, m_ability.Pace, m_ability.Agility, m_ability.Jumping,
                m_ability.Strength, m_ability.Stamina, m_ability.Balance, m_ability.PosOffensive, m_ability.PosDefensive, m_ability.Vision,
                m_ability.Aggression, m_ability.Reactions, m_ability.Composure, m_ability.Consistency, m_ability.TacAwareness, m_ability.FreeKicks,
                m_ability.Corners, m_ability.PenaltyShot, m_ability.Diving, m_ability.Handling, m_ability.Positioning, m_ability.OneOnOne, m_ability.Reflexes,
                m_ability.Kicking);
        }
        else {
            PlayerSkills12 fm12;
            ConvertSkillsToFM12(fm12);
            writer.WriteParams(43, fm12.ShotPower, fm12.LongShots, fm12.Dribbling, fm12.Technique, fm12.Crossing, fm12.Creativity, fm12.Finishing,
                fm12.FreeKicks, fm12.Corners, fm12.PenaltyShot, fm12.Passing, fm12.LongPassing, fm12.Flair, fm12.Tackling, fm12.ManMarking,
                fm12.Header, fm12.Touch, fm12.ForwardRuns, fm12.Acceleration, fm12.Pace, fm12.WorkRate, fm12.Strength, fm12.Agility, fm12.Jumping,
                fm12.Stamina, fm12.Anticipation, fm12.Aggression, fm12.Composure, fm12.Concentration, fm12.Decision, fm12.Determination,
                fm12.TeamWork, fm12.Consistency, fm12.Intimidation, fm12.Leadership, fm12.Crosses, fm12.Handling, fm12.GoalKicks, fm12.OneOnOne,
                fm12.Positioning, fm12.Punching, fm12.ShotStopping, fm12.Throwing);
        }
        writer.WriteParam(m_nTeam);
        writer.WriteParam(m_nFlags);
        writer.WriteParam(m_nLeftRightFootFlags);
        writer.WriteParam(m_nHero);
        writer.WriteParam(m_nPlayingStyle);
        writer.WriteI64Param(m_n64CharacterFlags);
        writer.WriteParams(11, m_nFace, m_nHairStyle, m_nHairColour, m_nBeard, m_nSideburns, m_nBeardColour, m_nSkinColour,
            m_nVariation, m_nEyeColour, m_nShoeType, unknown3);
        writer.WriteParams(6, m_nSpecialFace, m_nCommentId, m_nHeight, m_nWeight, m_nShirtNumber[0], m_nShirtNumber[1]);
        writer.WriteParam(m_nPlayerAgent);
        writer.WriteParams(2, m_nMatchesForNationalTeam, m_nGoalsForNationalTeam);
        writer.WriteParam(m_nFavouriteClub);
        writer.WriteParam(m_nWouldnSignFor);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x2)) {
            writer.WriteParam(m_nTransferRumors[0]);
            writer.WriteParam(m_nTransferRumors[1]);
            writer.WriteParam(m_nTransferRumors[2]);
        }
        writer.WriteParam(m_nNumConditions);
        for (int i = 0; i < m_nNumConditions; i++)
            m_conditions[i].Write(writer);
        m_playerHistory.Write(writer);
        m_playerContract.Write(writer);
        writer.WriteParams(4, unknown4[0], unknown4[1], unknown4[2], unknown4[3]);
        writer.WriteParam(m_nPreferredFormation);
        writer.WriteParam(m_nChairmanStability);
        writer.WriteLine(m_szComment);
        writer.WriteLine(L"%INDEXEND%PLAYER");
    }

    bool operator > (const Player& str) const
    {
        return (fifaRating > str.fifaRating);
    }

    int TranslatePosition(int position)
    {
        switch (position)
        {
        case POS_GK:
            return POSITION_GK;
        case POS_RWB:
        case POS_RB:
            return POSITION_RB;
        case POS_SW:
        case POS_RCB:
        case POS_CB:
        case POS_LCB:
            return POSITION_CB;
        case POS_LB:
        case POS_LWB:
            return POSITION_LB;
        case POS_RDM:
        case POS_CDM:
        case POS_LDM:
            return POSITION_DM;
        case POS_RM:
            return POSITION_RM;
        case POS_RCM:
        case POS_CM:
        case POS_LCM:
            return POSITION_CM;
        case POS_LM:
            return POSITION_LM;
        case POS_RAM:
        case POS_CAM:
        case POS_LAM:
            return POSITION_AM;
        case POS_RF:
        case POS_CF:
        case POS_LF:
            return POSITION_CF;
        case POS_RW:
            return POSITION_RW;
        case POS_RS:
        case POS_ST:
        case POS_LS:
            return POSITION_ST;
        case POS_LW:
            return POSITION_LW;
        }
        return POSITION_CM;
    }

    float CalcOverall(ePosition Position) {
        return m_ability.CalcOverall(Position);
    }

    float GetPlayerFIFAOverall() {
        int add = 0;
        switch (m_nExperience) {
        case 18:
            add = 3;
            break;
        case 17:
        case 16:
        case 15:
            add = 2;
            break;
        case 14:
        case 13:
        case 12:
            add = 1;
            break;
        }
        return CalcOverall((ePosition)m_nPosition) + add;
    }

    float GetPlayerFIFAOverall(unsigned char position) {
        int add = 0;
        switch (m_nExperience) {
        case 18:
            add = 3;
            break;
        case 17:
        case 16:
        case 15:
            add = 2;
            break;
        case 14:
        case 13:
        case 12:
            add = 1;
            break;
        }
        return CalcOverall((ePosition)position) + add;
    }

    float GetPlayerRatingForPlayingStyle(ePlayingStyle style) {
        Player::Ability ability = m_ability;
        float res;
        switch (style)
        {
        case PS_NONE:
            return 0.0f;
        case PS_ATTACKING_FB:
            res =
                ability.Crossing;
            return res;
        case PS_DEFENCE_FB:
            res = ability.TackleStanding + ability.TackleSliding + ability.ManMarking + ability.Strength + ability.PosDefensive;
            return res / 5.0f;
        case PS_LIBERO:
            res =
                ability.Passing + // 1.5
                ability.LongPassing + // 2.0
                ability.Vision; // 2.0
            return res / 3.0f;
        case PS_SIMPLE_PASSER:
            if (ability.Passing * 0.9f > ability.Vision && ability.Passing * 0.9f > ability.LongPassing) {
                res = ability.Passing;
                return res;
            }
            return 0.0f;
        case PS_BALL_WINNER:
            res =
                ability.TackleStanding +
                ability.TackleSliding +
                ability.ManMarking +
                ability.Reactions;
            return res / 4.0f;
        case PS_HARD_MAN:
            res =
                ability.TackleSliding + // 1.3
                ability.Strength + // 1.3
                ability.Aggression; // 2.5
            return res / 3.0f;
        case PS_HOLDING:
            res =
                ability.PosDefensive + // 2.0
                ability.Stamina; // 1.2
            return res / 2.0f;
        case PS_BALL_WINNER_MIDFIELD:
            res =
                ability.TackleStanding +
                ability.Reactions;
            return res / 2.0f;
        case PS_BOX_TO_BOX:
            switch (m_nPosition) {
            case POSITION_CM:
            case POSITION_DM:
            case POSITION_AM:
                res =
                    ability.Aggression + // 1.3
                    ability.Stamina; // 1.7
                return res / 2.0f;
            }
            return 0.0f;
        case PS_BUSY:
            res =
                ability.ManMarking + // 1.2
                ability.Stamina + // 1.6
                ability.PosDefensive; // 1.4
            return res / 3.0f;
        case PS_PLAY_MAKER:
            res =
                ability.Passing +
                ability.LongPassing +
                ability.Vision;
            return res / 3.0f;
        case PS_DRIBBLER:
            res =
                ability.BallControl +
                ability.Dribbling +
                ability.Balance;
            return res / 3.0f;
        case PS_WINGER:
            res =
                ability.Crossing + // 2
                ability.Acceleration; // 1.4
            return res / 2.0f;
        case PS_TARGET_MAN:
            res =
                ability.Header +
                ability.Jumping +
                ability.Strength;
            return res / 3.0f;
        case PS_PENALTY_BOX:
            res =
                ability.Finishing + // 2.0
                ability.PosOffensive + // 2.0
                ability.Reactions; // 1.8
            return res / 3.0f;
        case PS_RUNS_CHANNELS:
            res =
                ability.Acceleration +
                ability.Pace;
            return res / 2.0f;
        case PS_PULLS_WIDE_LEFT:
            if (m_nPosition != POSITION_RW && m_nPosition != POSITION_LW) {
                res =
                    ability.Crossing +
                    ability.Acceleration +
                    ability.Pace;
                return res / 3.0f;
            }
            return 0.0f;
        case PS_PULLS_WIDE_RIGHT:
            if (m_nPosition != POSITION_RW && m_nPosition != POSITION_LW) {
                res =
                    ability.Crossing +
                    ability.Acceleration +
                    ability.Pace;
                return res / 3.0f;
            }
            return 0.0f;
        case PS_DRIBBLER_ATTACK:
            res =
                ability.BallControl +
                ability.Dribbling;
            return res / 2.0f;
        case PS_HOLDS_UP:
            res = ability.Strength + ability.Balance;
            return res / 2.0f;
        case PS_BUSY_ATTACKER:
            res = ability.Stamina + ability.PosDefensive;
            return res / 2.0f;
        case PS_TOWER_STRENGTH:
            res = ability.Jumping + ability.Strength;
            return res / 2.0f;
        case PS_DISTANCE_SHOOTER:
            res =
                ability.ShotPower +
                ability.LongShots +
                ability.Volleys;
            return res / 3.0f;
        }
        return 0.0f;
    }

    template<int... Values>
    int SelectPlayingStyle()
    {
        int arr[sizeof...(Values)] = { Values... };
        float overall = 0.0f;
        int result = PS_NONE;
        for (int i = 0; i < sizeof...(Values); i++) {
            float tempOverall = GetPlayerRatingForPlayingStyle((ePlayingStyle)arr[i]);
            if (tempOverall > overall) {
                overall = tempOverall;
                result = arr[i];
            }
        }
        return result;
    }

    int GetFirstAvailableLanguageSlot() {
        if (!m_nLanguages[0])
            return 0;
        if (!m_nLanguages[1])
            return 1;
        if (!m_nLanguages[2])
            return 2;
        return -1;
    }

    void FromFifaPlayer(FifaPlayer &fifaPlayer, int fifaClubId, int clubCaptainId, int fifamClubId, int jerseyNumber, int jerseyNumberReserve, bool inReserve)
    {
        m_nId = fifaPlayer.m_playerId;
#ifndef RELEASE_MODE
        wsprintfW(m_szPseudonym, L"%d", m_nId);
#endif
        fifaRating = fifaPlayer.m_stats.overallRating;
        fifaPotential = fifaPlayer.m_stats.potential;
        unsigned char historyTalent = fifaPlayer.GetHistoryRating(&fifaHistoryRating);

        fifaTraits = fifaPlayer.m_trait[0];
        m_nType = PLAYER_PLAYER;
        CopyStr(m_szFirstName, fifaPlayer.m_firstName, 64);
        CopyStr(m_szLastName, fifaPlayer.m_lastName, 64);
        CopyStr(m_szNickname, fifaPlayer.m_commonName, 64);
        sNationInfo *nationInfo = GetFifaNationInfo(fifaPlayer.m_nationality);
        m_nNationality = nationInfo->fifamId;

        std::unordered_set<int> playerLanguages;

        playerLanguages.insert(nationInfo->language1);
        playerLanguages.insert(nationInfo->language2);

        m_birthday.FromFifaDate(fifaPlayer.m_birthDate);
#ifdef TRANSLATE_DATES
        m_birthday.year -= YEARS_DIFFERENCE;
        m_birthday.FixAfterTranslating();
#endif
        m_birthday.ValidateMin(Date::GetCurrentYear() - 57);
        m_birthday.ValidateMax(Date::GetCurrentYear() - 12);

        m_bRealPlayer = 1;
        if (clubCaptainId == fifaPlayer.m_playerId)
            m_bCaptain = 1;

        if (fifaPlayer.inNationalTeam)
            m_bCurrentlyInNationalTeam = 1;

        if (fifaPlayer.m_preferredFoot == 1)
        {
            m_nRightFoot = 4;
            m_nLeftFoot = fifaPlayer.m_weakFootAbilityTypeCode - 1;
        }
        else
        {
            m_nLeftFoot = 4;
            m_nRightFoot = fifaPlayer.m_weakFootAbilityTypeCode - 1;
        }

        // Talent
        if (fifaPlayer.m_playerId == 28130 || fifaPlayer.m_playerId == 138449)
            m_nTalent = 9;
        else
        {
            //m_nTalent = CalcTalent(fifaPlayer.m_stats.potential > fifaPlayer.m_stats.overallRating? fifaPlayer.m_stats.potential - 1 : fifaPlayer.m_stats.overallRating);
            m_nTalent = CalcTalent(fifaPlayer.m_stats.potential);

            if (historyTalent > m_nTalent)
                m_nTalent = historyTalent;
        }

        // Leadership
        if (fifaPlayer.m_trait[0] & TRAIT1_LEADERSHIP)
        {
            if (clubCaptainId == fifaPlayer.m_playerId)
            {
                m_nLeadership = 10;
                unknown2 = 7;
            }
            else
            {
                m_nLeadership = 9;
                unknown2 = 6;
            }
        }
        else
        {
            if (clubCaptainId == fifaPlayer.m_playerId)
            {
                m_nLeadership = 6;
                unknown2 = 4;
            }
            else
            {
                m_nLeadership = 0;
                unknown2 = 2;
            }
        }

        unknown2 = m_nTalent * 0.7f;

        //Experience
        //int age = 2000 - m_birthday.year;
        //if (age < 0)
        //	age = 0;
        //float experience = (float)(fifaPlayer.m_internationalRep - 1) * 0.75f + age;
        int age = 1998 - m_birthday.year;
        if (age < 0)
            age = 0;
        float exp = age;
        exp /= 7.0f;
        if (exp > 2.0f)
            exp = 2.0f;
        switch (fifaPlayer.m_internationalRep)
        {
        case 5:
            m_nExperience = 18;
            break;
        case 4:
            m_nExperience = 15 + exp;
            break;
        case 3:
            m_nExperience = 12 + exp;
            break;
        default:
            m_nExperience = 9 + exp;
        }

        // Hero status
        unsigned int heroStatus = 0;
        if (fifaPlayer.m_stats.overallRating >= 90 || m_nTalent == 9)
            heroStatus += 3;
        if (m_bCaptain)
            heroStatus += 1;
        if (fifaPlayer.m_trait[1] & TRAIT2_TEAM_PLAYER)
            heroStatus += 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_LEADERSHIP)
            heroStatus += 2;

        if (heroStatus > 4)
            heroStatus = 4;

        m_nHero = heroStatus;

        Player *oldPlayer = FindPlayerInOldFM(*this, &fifaPlayer);
        Player *newPlayer = FindPlayerInNewFM(*this, &fifaPlayer);

        if (!oldPlayer)
            oldPlayer = newPlayer;

        if (oldPlayer) {
            if (oldPlayer->m_nHero > m_nHero) {
                m_nHero = oldPlayer->m_nHero;
            }
            if (!m_bCurrentlyInNationalTeam) {
                m_bRetiredFromNationalTeam = oldPlayer->m_bRetiredFromNationalTeam;
            }
            m_nMatchesForNationalTeam = oldPlayer->m_nMatchesForNationalTeam;
            m_nGoalsForNationalTeam = oldPlayer->m_nGoalsForNationalTeam;
            m_n64CharacterFlags = oldPlayer->m_n64CharacterFlags;
            if (oldPlayer->m_nLeadership > m_nLeadership)
                m_nLeadership = oldPlayer->m_nLeadership;
            m_nCommentId = oldPlayer->m_nCommentId;
            m_bBasque = oldPlayer->m_bBasque;
            m_bNaturalised = oldPlayer->m_bNaturalised;

            if (m_nNationality != oldPlayer->m_nNationality)
                m_n2ndNationality = oldPlayer->m_nNationality;
            else
                m_n2ndNationality = oldPlayer->m_n2ndNationality;

            if (oldPlayer->m_nLanguages[0])
                playerLanguages.insert(oldPlayer->m_nLanguages[0]);
            if (oldPlayer->m_nLanguages[1])
                playerLanguages.insert(oldPlayer->m_nLanguages[1]);
            if (oldPlayer->m_nLanguages[2])
                playerLanguages.insert(oldPlayer->m_nLanguages[2]);
        }
        else {
            if (m_nLeadership == 0) {
                m_nLeadership = 4;
            }
        }

        // Fill with new data if available
        if (newPlayer) {
            if (!m_bCurrentlyInNationalTeam) {
                m_bRetiredFromNationalTeam = newPlayer->m_bRetiredFromNationalTeam;
            }
            if (newPlayer->m_nMatchesForNationalTeam > m_nMatchesForNationalTeam)
                m_nMatchesForNationalTeam = newPlayer->m_nMatchesForNationalTeam;
            if (newPlayer->m_nGoalsForNationalTeam > m_nGoalsForNationalTeam)
                m_nGoalsForNationalTeam = newPlayer->m_nGoalsForNationalTeam;
            if (!m_bBasque)
                m_bBasque = newPlayer->m_bBasque;
            if (!m_bNaturalised)
                m_bNaturalised = newPlayer->m_bNaturalised;
            if (!m_n2ndNationality) {
                if (m_nNationality != newPlayer->m_nNationality)
                    m_n2ndNationality = newPlayer->m_nNationality;
                else
                    m_n2ndNationality = newPlayer->m_n2ndNationality;
            }

            if (newPlayer->m_nLanguages[0])
                playerLanguages.insert(newPlayer->m_nLanguages[0]);
            if (newPlayer->m_nLanguages[1])
                playerLanguages.insert(newPlayer->m_nLanguages[1]);
            if (newPlayer->m_nLanguages[2])
                playerLanguages.insert(newPlayer->m_nLanguages[2]);
        }

        // Если не испанец и не француз
        if (m_bBasque) {
            if (m_nNationality != 18 && m_nNationality != 45 && m_n2ndNationality != 18 && m_n2ndNationality != 45)
                m_bBasque = false;
        }

        int traitSecNat = AdditionalTraits::GetPlayerSecondNationality(m_nId);
        if (traitSecNat != -1 && m_nNationality != traitSecNat && m_n2ndNationality != traitSecNat)
            m_n2ndNationality = traitSecNat;

        if (m_nNationality == m_n2ndNationality)
            m_n2ndNationality = 0;

        if (m_n2ndNationality) {
            sNationInfo *nation2Info = GetFifaNationInfoFromFifaMNation(m_n2ndNationality);
            if (nation2Info)
                playerLanguages.insert(nation2Info->language1);
        }

        int iNat = 0;
        for (auto i : playerLanguages) {
            if (i != 0) {
                m_nLanguages[iNat++] = i;
                if (iNat > 3)
                    break;
            }
        }

        m_character.LongThrows = 0;
        m_character.LongThrowOuts = 0;
        m_character.InjuryProne = 0;
        m_character.LittleInjuryProne = 0;
        m_character.Egoist = 0;
        m_character.Teamplayer = 0;

        // traits
        if (fifaPlayer.m_trait[0] & TRAIT1_INFLEXIBLE)
            m_character.Inflexibility = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_LONG_THROW_IN)
            m_character.LongThrows = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_GIANT_THROW_IN)
            m_character.LongThrows = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_GK_LONG_THROW)
            m_character.LongThrowOuts = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_INJURY_FREE)
            m_character.LittleInjuryProne = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_INJURY_PRONE)
            m_character.InjuryProne = 1;
        if (fifaPlayer.m_trait[1] & TRAIT2_TEAM_PLAYER)
            m_character.Teamplayer = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_SELFISH)
            m_character.Egoist = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_FANS_FAVOURITE)
            m_character.FansFavorite = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_ARGUES_WITH_OFFICIALS)
            m_character.Critic = 1;
        if (fifaPlayer.m_trait[0] & TRAIT1_POWER_FREE_KICK)
            m_character.DrivenFreeKicks = 1;
        if (fifaPlayer.m_trait[1] & TRAIT2_ONE_CLUB_PLAYER)
            m_character.IdentificationHigh = 1;

        // Stats
        m_ability.BallControl = FifaStat(fifaPlayer.m_stats.ballControl);
        m_ability.Dribbling = FifaStat(fifaPlayer.m_stats.dribbling);
        m_ability.Finishing = FifaStat(fifaPlayer.m_stats.finishing);
        m_ability.LongShots = FifaStat(fifaPlayer.m_stats.longShots);
        m_ability.Volleys = FifaStat(fifaPlayer.m_stats.volleys);
        m_ability.Crossing = FifaStat(fifaPlayer.m_stats.crossing);

        if (fifaPlayer.m_preferredPosition[0] == POS_GK)
        {
            m_ability.LongPassing = (FifaStat(fifaPlayer.m_stats.GkKicking) + fifaPlayer.m_stats.overallRating) / 2;
            m_ability.Passing = (FifaStat(fifaPlayer.m_stats.GkKicking) + fifaPlayer.m_stats.overallRating) / 2;
            m_ability.ShotPower = FifaStat(fifaPlayer.m_stats.GkKicking);
            if (fifaPlayer.m_stats.GkDiving > fifaPlayer.m_stats.jumping)
                m_ability.Jumping = FifaStat(fifaPlayer.m_stats.GkDiving);
            else
                m_ability.Jumping = FifaStat(fifaPlayer.m_stats.jumping);
            m_ability.TacAwareness = fifaPlayer.m_stats.overallRating;
            m_ability.Composure = fifaPlayer.m_stats.overallRating;
            m_ability.Consistency = fifaPlayer.m_stats.overallRating;
        }
        else
        {
            m_ability.LongPassing = FifaStat(fifaPlayer.m_stats.longPassing);
            m_ability.Passing = FifaStat(fifaPlayer.m_stats.shortPassing);
            m_ability.ShotPower = FifaStat(fifaPlayer.m_stats.shotPower);
            m_ability.Jumping = FifaStat(fifaPlayer.m_stats.jumping);
            m_ability.Composure = FifaStat(fifaPlayer.m_stats.balance);
            m_ability.Consistency = FifaStat(fifaPlayer.m_stats.balance);
            if (fifaPlayer.m_stats.interceptions > fifaPlayer.m_stats.positioning)
                m_ability.TacAwareness = (FifaStat(fifaPlayer.m_stats.vision) + FifaStat(fifaPlayer.m_stats.interceptions)) / 2;
            else
                m_ability.TacAwareness = (FifaStat(fifaPlayer.m_stats.vision) + FifaStat(fifaPlayer.m_stats.positioning)) / 2;
        }
        m_ability.Header = FifaStat(fifaPlayer.m_stats.headingAccuracy);
        m_ability.TackleStanding = FifaStat(fifaPlayer.m_stats.standingTackle);
        m_ability.TackleSliding = FifaStat(fifaPlayer.m_stats.slidingTackle);
        m_ability.ManMarking = FifaStat(fifaPlayer.m_stats.marking);
        m_ability.Acceleration = FifaStat(fifaPlayer.m_stats.acceleration);
        m_ability.Pace = FifaStat(fifaPlayer.m_stats.sprintSpeed);
        m_ability.Agility = FifaStat(fifaPlayer.m_stats.agility);
        m_ability.Strength = FifaStat(fifaPlayer.m_stats.strength);
        m_ability.Stamina = FifaStat(fifaPlayer.m_stats.stamina);
        m_ability.Balance = FifaStat(fifaPlayer.m_stats.balance);
        m_ability.PosOffensive = FifaStat(fifaPlayer.m_stats.positioning);
        m_ability.PosDefensive = FifaStat(fifaPlayer.m_stats.interceptions);
        m_ability.Vision = FifaStat(fifaPlayer.m_stats.vision);
        m_ability.Aggression = FifaStat(fifaPlayer.m_stats.aggression);
        m_ability.Reactions = FifaStat(fifaPlayer.m_stats.reactions);
        m_ability.FreeKicks = FifaStat(fifaPlayer.m_stats.freeKickAccuracy);
        m_ability.Corners = (FifaStat(fifaPlayer.m_stats.freeKickAccuracy) + FifaStat(fifaPlayer.m_stats.crossing)) / 2;
        m_ability.PenaltyShot = FifaStat(fifaPlayer.m_stats.penalties);
        m_ability.Diving = FifaStat(fifaPlayer.m_stats.GkDiving);
        m_ability.Handling = FifaStat(fifaPlayer.m_stats.GkHandling);
        m_ability.Positioning = FifaStat(fifaPlayer.m_stats.GkPositioning);
        m_ability.OneOnOne = (FifaStat(fifaPlayer.m_stats.GkPositioning) + FifaStat(fifaPlayer.m_stats.GkReflexes)) / 2;
        m_ability.Reflexes = FifaStat(fifaPlayer.m_stats.GkReflexes);
        m_ability.Kicking = FifaStat(fifaPlayer.m_stats.GkKicking);

        if (oldPlayer) {
            if (oldPlayer->m_ability.TacAwareness > (m_ability.TacAwareness - 10))
                m_ability.TacAwareness = oldPlayer->m_ability.TacAwareness;
            if (oldPlayer->m_ability.Composure > (m_ability.Composure - 10))
                m_ability.Composure = oldPlayer->m_ability.Composure;
            if (oldPlayer->m_ability.Consistency > (m_ability.Consistency - 10))
                m_ability.Consistency = oldPlayer->m_ability.Consistency;
        }

        if (fifaPlayer.m_fifaVersion >= 17) {
            m_ability.Composure = FifaStat(fifaPlayer.m_stats.composure);
        }

        // Position
        m_nPosition = TranslatePosition(fifaPlayer.m_preferredPosition[0]);

        for (int i = 1; i < NUM_POSITIONS; i++)
            m_nPositionBias[i] = 40;
        m_nPositionBias[m_nPosition] = 100;

        // Playing style
        unsigned int rating, newrating;
        switch (m_nPosition)
        {
        case POSITION_CB:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_BALL_WINNER;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 85;
            m_nPositionBias[POSITION_LB] = 85;
            m_nPositionBias[POSITION_CB] = 100;
            m_nPositionBias[POSITION_DM] = 85;
            m_nPositionBias[POSITION_RM] = 75;
            m_nPositionBias[POSITION_LM] = 75;
            m_nPositionBias[POSITION_CM] = 80;
            m_nPositionBias[POSITION_RW] = 70;
            m_nPositionBias[POSITION_LW] = 70;
            m_nPositionBias[POSITION_AM] = 75;
            m_nPositionBias[POSITION_CF] = 70;
            m_nPositionBias[POSITION_ST] = 70;
            break;
        case POSITION_RB:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_PULLS_WIDE_RIGHT;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 100;
            m_nPositionBias[POSITION_LB] = 90;
            m_nPositionBias[POSITION_CB] = 85;
            m_nPositionBias[POSITION_DM] = 85;
            m_nPositionBias[POSITION_RM] = 90;
            m_nPositionBias[POSITION_LM] = 85;
            m_nPositionBias[POSITION_CM] = 75;
            m_nPositionBias[POSITION_RW] = 85;
            m_nPositionBias[POSITION_LW] = 80;
            m_nPositionBias[POSITION_AM] = 75;
            m_nPositionBias[POSITION_CF] = 70;
            m_nPositionBias[POSITION_ST] = 70;
            break;
        case POSITION_LB:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_PULLS_WIDE_LEFT;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 90;
            m_nPositionBias[POSITION_LB] = 100;
            m_nPositionBias[POSITION_CB] = 85;
            m_nPositionBias[POSITION_DM] = 85;
            m_nPositionBias[POSITION_RM] = 85;
            m_nPositionBias[POSITION_LM] = 90;
            m_nPositionBias[POSITION_CM] = 75;
            m_nPositionBias[POSITION_RW] = 80;
            m_nPositionBias[POSITION_LW] = 85;
            m_nPositionBias[POSITION_AM] = 75;
            m_nPositionBias[POSITION_CF] = 70;
            m_nPositionBias[POSITION_ST] = 70;
            break;
        case POSITION_DM:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_BOX_TO_BOX;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 85;
            m_nPositionBias[POSITION_LB] = 85;
            m_nPositionBias[POSITION_CB] = 85;
            m_nPositionBias[POSITION_DM] = 100;
            m_nPositionBias[POSITION_RM] = 80;
            m_nPositionBias[POSITION_LM] = 80;
            m_nPositionBias[POSITION_CM] = 90;
            m_nPositionBias[POSITION_RW] = 75;
            m_nPositionBias[POSITION_LW] = 75;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 75;
            m_nPositionBias[POSITION_ST] = 75;
            break;
        case POSITION_RM:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_RUNS_CHANNELS;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 90;
            m_nPositionBias[POSITION_LB] = 85;
            m_nPositionBias[POSITION_CB] = 75;
            m_nPositionBias[POSITION_DM] = 80;
            m_nPositionBias[POSITION_RM] = 100;
            m_nPositionBias[POSITION_LM] = 90;
            m_nPositionBias[POSITION_CM] = 85;
            m_nPositionBias[POSITION_RW] = 96;
            m_nPositionBias[POSITION_LW] = 85;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 80;
            m_nPositionBias[POSITION_ST] = 75;
            break;
        case POSITION_LM:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_RUNS_CHANNELS;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 85;
            m_nPositionBias[POSITION_LB] = 90;
            m_nPositionBias[POSITION_CB] = 75;
            m_nPositionBias[POSITION_DM] = 80;
            m_nPositionBias[POSITION_RM] = 90;
            m_nPositionBias[POSITION_LM] = 100;
            m_nPositionBias[POSITION_CM] = 85;
            m_nPositionBias[POSITION_RW] = 85;
            m_nPositionBias[POSITION_LW] = 96;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 80;
            m_nPositionBias[POSITION_ST] = 75;
            break;
        case POSITION_CM:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_BALL_WINNER_MIDFIELD;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 75;
            m_nPositionBias[POSITION_LB] = 75;
            m_nPositionBias[POSITION_CB] = 80;
            m_nPositionBias[POSITION_DM] = 90;
            m_nPositionBias[POSITION_RM] = 85;
            m_nPositionBias[POSITION_LM] = 85;
            m_nPositionBias[POSITION_CM] = 100;
            m_nPositionBias[POSITION_RW] = 80;
            m_nPositionBias[POSITION_LW] = 80;
            m_nPositionBias[POSITION_AM] = 90;
            m_nPositionBias[POSITION_CF] = 85;
            m_nPositionBias[POSITION_ST] = 80;
            break;
        case POSITION_RW:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_WINGER;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 85;
            m_nPositionBias[POSITION_LB] = 80;
            m_nPositionBias[POSITION_CB] = 70;
            m_nPositionBias[POSITION_DM] = 75;
            m_nPositionBias[POSITION_RM] = 96;
            m_nPositionBias[POSITION_LM] = 85;
            m_nPositionBias[POSITION_CM] = 80;
            m_nPositionBias[POSITION_RW] = 100;
            m_nPositionBias[POSITION_LW] = 95;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 90;
            m_nPositionBias[POSITION_ST] = 85;
            break;
        case POSITION_LW:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_WINGER;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 80;
            m_nPositionBias[POSITION_LB] = 85;
            m_nPositionBias[POSITION_CB] = 70;
            m_nPositionBias[POSITION_DM] = 75;
            m_nPositionBias[POSITION_RM] = 85;
            m_nPositionBias[POSITION_LM] = 96;
            m_nPositionBias[POSITION_CM] = 80;
            m_nPositionBias[POSITION_RW] = 95;
            m_nPositionBias[POSITION_LW] = 100;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 90;
            m_nPositionBias[POSITION_ST] = 85;
            break;
        case POSITION_AM:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_PLAY_MAKER;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 75;
            m_nPositionBias[POSITION_LB] = 75;
            m_nPositionBias[POSITION_CB] = 75;
            m_nPositionBias[POSITION_DM] = 85;
            m_nPositionBias[POSITION_RM] = 85;
            m_nPositionBias[POSITION_LM] = 85;
            m_nPositionBias[POSITION_CM] = 90;
            m_nPositionBias[POSITION_RW] = 85;
            m_nPositionBias[POSITION_LW] = 85;
            m_nPositionBias[POSITION_AM] = 100;
            m_nPositionBias[POSITION_CF] = 90;
            m_nPositionBias[POSITION_ST] = 85;
            break;
        case POSITION_CF:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_DRIBBLER_ATTACK;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 70;
            m_nPositionBias[POSITION_LB] = 70;
            m_nPositionBias[POSITION_CB] = 70;
            m_nPositionBias[POSITION_DM] = 75;
            m_nPositionBias[POSITION_RM] = 80;
            m_nPositionBias[POSITION_LM] = 80;
            m_nPositionBias[POSITION_CM] = 85;
            m_nPositionBias[POSITION_RW] = 90;
            m_nPositionBias[POSITION_LW] = 90;
            m_nPositionBias[POSITION_AM] = 90;
            m_nPositionBias[POSITION_CF] = 100;
            m_nPositionBias[POSITION_ST] = 95;
            break;
        case POSITION_ST:
            // Attacking FB, Ball winner, Builds up, Defensive FB, Distance shooter, Fighter, Simple passer, Tower of Strength
            m_nPlayingStyle = PS_PENALTY_BOX;
            m_nPositionBias[POSITION_GK] = 40;
            m_nPositionBias[POSITION_RB] = 70;
            m_nPositionBias[POSITION_LB] = 70;
            m_nPositionBias[POSITION_CB] = 70;
            m_nPositionBias[POSITION_DM] = 75;
            m_nPositionBias[POSITION_RM] = 75;
            m_nPositionBias[POSITION_LM] = 75;
            m_nPositionBias[POSITION_CM] = 80;
            m_nPositionBias[POSITION_RW] = 85;
            m_nPositionBias[POSITION_LW] = 85;
            m_nPositionBias[POSITION_AM] = 85;
            m_nPositionBias[POSITION_CF] = 95;
            m_nPositionBias[POSITION_ST] = 100;
            break;
        }

        switch (m_nPosition)
        {
        case POSITION_CB:
            m_nPlayingStyle = SelectPlayingStyle<PS_ATTACKING_FB, PS_BALL_WINNER, PS_LIBERO, PS_DEFENCE_FB, PS_DISTANCE_SHOOTER, PS_HARD_MAN, PS_TOWER_STRENGTH>();
            break;
        case POSITION_RB:
            m_nPlayingStyle = SelectPlayingStyle<PS_ATTACKING_FB, PS_BALL_WINNER, PS_BOX_TO_BOX, PS_LIBERO, PS_BUSY, PS_DEFENCE_FB, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HARD_MAN, PS_PULLS_WIDE_RIGHT, PS_SIMPLE_PASSER, PS_WINGER>();
            break;
        case POSITION_LB:
            m_nPlayingStyle = SelectPlayingStyle<PS_ATTACKING_FB, PS_BALL_WINNER, PS_BOX_TO_BOX, PS_LIBERO, PS_BUSY, PS_DEFENCE_FB, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HARD_MAN, PS_PULLS_WIDE_LEFT, PS_SIMPLE_PASSER, PS_WINGER>();
            break;
        case POSITION_DM:
            m_nPlayingStyle = SelectPlayingStyle<PS_BALL_WINNER, PS_BOX_TO_BOX, PS_LIBERO, PS_BUSY, PS_HOLDING, PS_DISTANCE_SHOOTER, PS_HARD_MAN, PS_HOLDS_UP, PS_BALL_WINNER_MIDFIELD, PS_PLAY_MAKER, PS_SIMPLE_PASSER, PS_TOWER_STRENGTH>();
            break;
        case POSITION_RM:
            m_nPlayingStyle = SelectPlayingStyle<PS_BOX_TO_BOX, PS_BUSY, PS_HOLDING, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HARD_MAN, PS_HOLDS_UP, PS_BALL_WINNER_MIDFIELD, PS_PLAY_MAKER, PS_PULLS_WIDE_RIGHT, PS_RUNS_CHANNELS, PS_SIMPLE_PASSER, PS_WINGER>();
            break;
        case POSITION_LM:
            m_nPlayingStyle = SelectPlayingStyle<PS_BOX_TO_BOX, PS_BUSY, PS_HOLDING, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HARD_MAN, PS_HOLDS_UP, PS_BALL_WINNER_MIDFIELD, PS_PLAY_MAKER, PS_PULLS_WIDE_LEFT, PS_RUNS_CHANNELS, PS_SIMPLE_PASSER, PS_WINGER>();
            break;
        case POSITION_CM:
            m_nPlayingStyle = SelectPlayingStyle<PS_BOX_TO_BOX, PS_BUSY, PS_HOLDING, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HARD_MAN, PS_HOLDS_UP, PS_BALL_WINNER_MIDFIELD, PS_PLAY_MAKER, PS_SIMPLE_PASSER>();
            break;
        case POSITION_RW:
            m_nPlayingStyle = SelectPlayingStyle<PS_DRIBBLER_ATTACK, PS_BUSY_ATTACKER, PS_DISTANCE_SHOOTER, PS_PULLS_WIDE_RIGHT, PS_RUNS_CHANNELS, PS_WINGER>();
            break;
        case POSITION_LW:
            m_nPlayingStyle = SelectPlayingStyle<PS_DRIBBLER_ATTACK, PS_BUSY_ATTACKER, PS_DISTANCE_SHOOTER, PS_PULLS_WIDE_LEFT, PS_RUNS_CHANNELS, PS_WINGER>();
            break;
        case POSITION_AM:
            m_nPlayingStyle = SelectPlayingStyle<PS_BOX_TO_BOX, PS_BUSY, PS_HOLDING, PS_DISTANCE_SHOOTER, PS_DRIBBLER, PS_HOLDS_UP, PS_PLAY_MAKER, PS_RUNS_CHANNELS>();
            break;
        case POSITION_CF:
            m_nPlayingStyle = SelectPlayingStyle<PS_DRIBBLER_ATTACK, PS_BUSY_ATTACKER, PS_DISTANCE_SHOOTER, PS_HOLDS_UP, PS_PENALTY_BOX, PS_RUNS_CHANNELS>();
            break;
        case POSITION_ST:
            m_nPlayingStyle = SelectPlayingStyle<PS_DRIBBLER_ATTACK, PS_BUSY_ATTACKER, PS_DISTANCE_SHOOTER, PS_HOLDS_UP, PS_PENALTY_BOX, PS_RUNS_CHANNELS, PS_TARGET_MAN>();
            break;
        }

        if (fifaPlayer.m_preferredPosition[1] != -1)
        {
            int posn = TranslatePosition(fifaPlayer.m_preferredPosition[1]);
            if (m_nPositionBias[posn] < 99)
                m_nPositionBias[posn] = 99;
        }
        if (fifaPlayer.m_preferredPosition[2] != -1)
        {
            int posn = TranslatePosition(fifaPlayer.m_preferredPosition[2]);
            if (m_nPositionBias[posn] < 98)
                m_nPositionBias[posn] = 98;
        }
        if (fifaPlayer.m_preferredPosition[3] != -1)
        {
            int posn = TranslatePosition(fifaPlayer.m_preferredPosition[3]);
            if (m_nPositionBias[posn] < 97)
                m_nPositionBias[posn] = 97;
        }

        // Jersey number
        m_nShirtNumber[0] = jerseyNumber;
        m_nShirtNumber[1] = jerseyNumberReserve;

        if (inReserve)
            m_nTeam = TEAM_RESERVE;

        if (fifaClubId) {
            if (fifaPlayer.linkedClub[0] == fifaClubId) {
                m_teamPos = fifaPlayer.teamPos[0];
                if (m_teamPos != POS_RES)
                    m_nTeam = TEAM_1ST_SQUAD;
            }
            else if (fifaPlayer.linkedClub[1] == fifaClubId) {
                m_teamPos = fifaPlayer.teamPos[1];
                if (m_teamPos != POS_RES)
                    m_nTeam = TEAM_1ST_SQUAD;
            }
        }

        // Contract
        m_playerContract.m_joinedClubDate.FromFifaDate(fifaPlayer.m_playerJoinTeamDate);
#ifdef TRANSLATE_DATES
        m_playerContract.m_joinedClubDate.year -= YEARS_DIFFERENCE;
        m_playerContract.m_joinedClubDate.FixAfterTranslating();
#endif
        m_playerContract.m_joinedClubDate.ValidateMin(Date::GetCurrentYear() - 32);
        m_playerContract.m_joinedClubDate.Validate_History_Last();

        m_playerContract.m_contractValidUntil.Set(30, 6, fifaPlayer.m_contractValidUntil);
#ifdef TRANSLATE_DATES
        m_playerContract.m_contractValidUntil.year -= YEARS_DIFFERENCE;
#endif

        if (m_playerContract.m_contractValidUntil.year > (Date::GetCurrentYear() + 10))
            m_playerContract.m_contractValidUntil.SetEndOfThisSeason();
        else
            m_playerContract.m_contractValidUntil.Validate_After_ThisSeason();

        //// Get injury/ban info from new FM
        //if (newPlayer) {
        //    for (int c = 0; c < newPlayer->m_nNumConditions; c++) {
        //        if (newPlayer->m_conditions[c].m_nType == CONDITION_BAN) {
        //            m_conditions[m_nNumConditions].m_nType = CONDITION_BAN;
        //            Date endDate;
        //            endDate.SetFromValue(newPlayer->m_conditions[c].Ban.m_nEndDate);
#ifdef TRANSLATE_DATES
        //            endDate.year -= YEARS_DIFFERENCE;
        //            endDate.FixAfterTranslating();
#endif
        //            m_conditions[m_nNumConditions].Ban.m_nEndDate = endDate.GetValue();
        //            m_nNumConditions++;
        //        }
        //        else if (newPlayer->m_conditions[c].m_nType == CONDITION_INJURY) {
        //            m_conditions[m_nNumConditions].m_nType = CONDITION_INJURY;
        //            m_conditions[m_nNumConditions].Injury.m_nInjuryType = newPlayer->m_conditions[c].Injury.m_nInjuryType;
        //            Date startDate;
        //            startDate.SetFromValue(newPlayer->m_conditions[c].Injury.m_nStart);
#ifdef TRANSLATE_DATES
        //            startDate.year -= YEARS_DIFFERENCE;
        //            startDate.FixAfterTranslating();
#endif
        //            m_conditions[m_nNumConditions].Injury.m_nStart = startDate.GetValue();
        //            Date endDate;
        //            endDate.SetFromValue(newPlayer->m_conditions[c].Injury.m_nEnd);
#ifdef TRANSLATE_DATES
        //            endDate.year -= YEARS_DIFFERENCE;
        //            endDate.FixAfterTranslating();
#endif
        //            m_conditions[m_nNumConditions].Injury.m_nEnd = endDate.GetValue();
        //            m_nNumConditions++;
        //        }
        //    }
        //}

        Player *playerToGetHistory = newPlayer;
        if (!playerToGetHistory)
            playerToGetHistory = oldPlayer;

        // History
        if (playerToGetHistory) {
            if (playerToGetHistory->m_playerHistory.records.size() > 0) {
                m_playerHistory.records.resize(playerToGetHistory->m_playerHistory.records.size());
                for (int i = 0; i < playerToGetHistory->m_playerHistory.records.size(); i++) {
                    m_playerHistory.records[i] = playerToGetHistory->m_playerHistory.records[i];
#ifdef TRANSLATE_DATES
                    m_playerHistory.records[i].m_start.year -= YEARS_DIFFERENCE;
                    m_playerHistory.records[i].m_start.FixAfterTranslating();
                    m_playerHistory.records[i].m_end.year -= YEARS_DIFFERENCE;
                    m_playerHistory.records[i].m_end.FixAfterTranslating();
#endif
                }
            }
            if (fifamClubId)
            {
                if (fifaPlayer.loanedFrom && m_nNumConditions < 4)
                {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_LOAN;
                    m_conditions[m_nNumConditions].Loan.m_nBuyValue = -1;
                    m_conditions[m_nNumConditions].Loan.m_nClubId = FixClubIdForPlayerHistory(fifaPlayer.loanedFrom);
                    m_conditions[m_nNumConditions].Loan.m_nStart = Date::GetValueForDate(Date::GetCurrentYear(), 7, 1);
                    Date loanEnd = fifaPlayer.loanEndDate;
#ifdef TRANSLATE_DATES
                    loanEnd.year -= YEARS_DIFFERENCE;
                    loanEnd.FixAfterTranslating();
#endif
                    loanEnd.Validate_AfterSeason();

                    if (DEBUGFILE) {
                        wchar_t plname[256];
                        GetPlayerNameToDisplay(plname);
                        fwprintf(DEBUGFILE, L"%-40s contract %2d.%2d.%4d", plname, m_playerContract.m_contractValidUntil.day,
                            m_playerContract.m_contractValidUntil.month, m_playerContract.m_contractValidUntil.year);
                        fwprintf(DEBUGFILE, L" loan %2d.%2d.%4d", loanEnd.day, loanEnd.month, loanEnd.year);
                    }

                    Date loanOld = loanEnd;

                    if (loanEnd <= Date(31, 12, Date::GetCurrentYear()))
                        loanEnd.Set(31, 12, Date::GetCurrentYear());
                    else if (loanEnd <= Date(1, 9, Date::GetCurrentYear() + 1))
                        loanEnd.Set(30, 6, Date::GetCurrentYear() + 1);
                    else if (loanEnd <= Date(31, 12, Date::GetCurrentYear() + 1))
                        loanEnd.Set(31, 12, Date::GetCurrentYear() + 1);
                    else
                        loanEnd.Set(30, 6, Date::GetCurrentYear() + 2);

                    m_conditions[m_nNumConditions].Loan.m_nEnd = loanEnd.GetValue();
                    m_nNumConditions++;

                    bool fixedContract = false;

                    if (m_playerContract.m_contractValidUntil <= loanEnd) {
                        m_playerContract.m_contractValidUntil.Set(30, 6, loanEnd.year + 1);
                        fixedContract = true;
                    }

                    if (DEBUGFILE) {
                        fwprintf(DEBUGFILE, L" loanfix %2d.%2d.%4d contrfix %2d.%2d.%4d", loanEnd.day, loanEnd.month, loanEnd.year,
                            m_playerContract.m_contractValidUntil.day, m_playerContract.m_contractValidUntil.month,
                            m_playerContract.m_contractValidUntil.year);
                        if (loanOld != loanEnd) {
                            fputws(L" FIXED_LOAN", DEBUGFILE);
                            if (fixedContract)
                                fputws(L",FIXED_CONTRACT", DEBUGFILE);
                        }
                        else if (fixedContract)
                            fputws(L" FIXED_CONTRACT", DEBUGFILE);
                        fputws(L"\n", DEBUGFILE);
                    }
                }
            }
        }
        else {
            if (fifamClubId)
            {
                if (fifaPlayer.loanedFrom && m_nNumConditions < 4)
                {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_LOAN;
                    m_conditions[m_nNumConditions].Loan.m_nBuyValue = -1;
                    m_conditions[m_nNumConditions].Loan.m_nClubId = FixClubIdForPlayerHistory(fifaPlayer.loanedFrom);
                    m_conditions[m_nNumConditions].Loan.m_nStart = Date::GetValueForDate(Date::GetCurrentYear(), 7, 1);
                    Date loanEnd = fifaPlayer.loanEndDate;
#ifdef TRANSLATE_DATES
                    loanEnd.year -= YEARS_DIFFERENCE;
                    loanEnd.FixAfterTranslating();
#endif
                    loanEnd.Validate_AfterSeason();

                    if (DEBUGFILE) {
                        wchar_t plname[256];
                        GetPlayerNameToDisplay(plname);
                        fwprintf(DEBUGFILE, L"%-40s contract %2d.%2d.%4d", plname, m_playerContract.m_contractValidUntil.day,
                            m_playerContract.m_contractValidUntil.month, m_playerContract.m_contractValidUntil.year);
                        fwprintf(DEBUGFILE, L" loan %2d.%2d.%4d", loanEnd.day, loanEnd.month, loanEnd.year);
                    }

                    Date loanOld = loanEnd;

                    if (loanEnd <= Date(31, 12, Date::GetCurrentYear()))
                        loanEnd.Set(31, 12, Date::GetCurrentYear());
                    else if (loanEnd <= Date(1, 9, Date::GetCurrentYear() + 1))
                        loanEnd.Set(30, 6, Date::GetCurrentYear() + 1);
                    else if (loanEnd <= Date(31, 12, Date::GetCurrentYear() + 1))
                        loanEnd.Set(31, 12, Date::GetCurrentYear() + 1);
                    else
                        loanEnd.Set(30, 6, Date::GetCurrentYear() + 2);

                    m_conditions[m_nNumConditions].Loan.m_nEnd = loanEnd.GetValue();
                    m_nNumConditions++;

                    bool fixedContract = false;

                    if (m_playerContract.m_contractValidUntil <= loanEnd) {
                        m_playerContract.m_contractValidUntil.Set(30, 6, loanEnd.year + 1);
                        fixedContract = true;
                    }

                    if (DEBUGFILE) {
                        fwprintf(DEBUGFILE, L" loanfix %2d.%2d.%4d contrfix %2d.%2d.%4d", loanEnd.day, loanEnd.month, loanEnd.year,
                            m_playerContract.m_contractValidUntil.day, m_playerContract.m_contractValidUntil.month,
                            m_playerContract.m_contractValidUntil.year);
                        if (loanOld != loanEnd) {
                            fputws(L" FIXED_LOAN", DEBUGFILE);
                            if (fixedContract)
                                fputws(L",FIXED_CONTRACT", DEBUGFILE);
                        }
                        else if (fixedContract)
                            fputws(L" FIXED_CONTRACT", DEBUGFILE);
                        fputws(L"\n", DEBUGFILE);
                    }
                }
            }
        }

        unsigned int historyEntriesSize = m_playerHistory.records.size();
        // Если в списке есть записи
        if (historyEntriesSize > 0) {
            // Убираем флаг CURRENT_CLUB для всех записей
            for (unsigned int i = 0; i < historyEntriesSize; i++) {
                m_playerHistory.records[i].m_nState = m_playerHistory.records[i].m_nState & ~HISTORYCONTRACT_CURRENT_CLUB;
                // Также проверяем все записи (кроме последней)
                if (i != (historyEntriesSize - 1)) {
                    m_playerHistory.records[i].m_start.Validate_History_Not_Last();
                    m_playerHistory.records[i].m_end.Validate_History_Not_Last();
                }
            }
            // Если не СА, если СА - больше делать ничего не нужно
            if (fifamClubId) {
                // Если не совпадает последний клуб в истории
                if (m_playerHistory.records[historyEntriesSize - 1].m_nClubId != fifamClubId) {
                    // Проверяем последнюю запись
                    m_playerHistory.records[historyEntriesSize - 1].m_start.Validate_History_Not_Last();
                    m_playerHistory.records[historyEntriesSize - 1].m_end.Validate_History_Not_Last();
                    // Добавляем запись с текущим клубом
                    m_playerHistory.records.resize(historyEntriesSize + 1);
                    m_playerHistory.records[historyEntriesSize].m_nClubId = fifamClubId;
                    m_playerHistory.records[historyEntriesSize].m_nState = HISTORYCONTRACT_CURRENT_CLUB;
                    m_playerHistory.records[historyEntriesSize].m_end.Set(30, 6, Date::GetCurrentYear() + 1);
                    if (IsLoaned()) {
                        m_playerHistory.records[historyEntriesSize].m_nState |= HISTORYCONTRACT_LOAN;
                        m_playerHistory.records[historyEntriesSize].m_start.Set(1, 7, Date::GetCurrentYear());
                    }
                    else {
                        if (m_playerContract.m_joinedClubDate > m_playerHistory.records[historyEntriesSize - 1].m_end) {
                            m_playerHistory.records[historyEntriesSize].m_start = m_playerContract.m_joinedClubDate;
                        }
                        else if (m_playerContract.m_joinedClubDate > m_playerHistory.records[historyEntriesSize - 1].m_start) {
                            m_playerHistory.records[historyEntriesSize].m_start = m_playerContract.m_joinedClubDate;
                            m_playerHistory.records[historyEntriesSize - 1].m_end = m_playerContract.m_joinedClubDate;
                            m_playerHistory.records[historyEntriesSize - 1].m_end -= 1;
                        }
                        else
                            m_playerHistory.records[historyEntriesSize].m_start.Set(1, 7, Date::GetCurrentYear());
                    }
                }
                else { // Иначе ставим последний клуб-запись в истории текущим
                    m_playerHistory.records[historyEntriesSize - 1].m_nState |= HISTORYCONTRACT_CURRENT_CLUB;
                    m_playerHistory.records[historyEntriesSize - 1].m_end.Set(30, 6, Date::GetCurrentYear() + 1);
                    if (IsLoaned()) {
                        m_playerHistory.records[historyEntriesSize - 1].m_nState |= HISTORYCONTRACT_LOAN;
                        m_playerHistory.records[historyEntriesSize - 1].m_start.Set(1, 7, Date::GetCurrentYear());
                    }
                    else {
                        m_playerHistory.records[historyEntriesSize - 1].m_nState = m_playerHistory.records[historyEntriesSize - 1].m_nState & ~HISTORYCONTRACT_LOAN;
                        m_playerHistory.records[historyEntriesSize - 1].m_start.Validate_History_Last();
                    }

                }
            }
            // Проверяем, чтобы не было несостыковок дат в списке
            for (int i = 0; i < m_playerHistory.records.size(); i++) {
                if ((!fifamClubId) || (i != (m_playerHistory.records.size() - 1))) {
                    if (m_playerHistory.records[i].m_start > m_playerHistory.records[i].m_end)
                        m_playerHistory.records[i].m_start = m_playerHistory.records[i].m_end;
                }
                if (i > 0) {
                    if (m_playerHistory.records[i - 1].m_end > m_playerHistory.records[i].m_start) {
                        m_playerHistory.records[i - 1].m_end = m_playerHistory.records[i].m_start;
                    }
                    if (m_playerHistory.records[i - 1].m_start > m_playerHistory.records[i - 1].m_end)
                        m_playerHistory.records[i - 1].m_start = m_playerHistory.records[i - 1].m_end;
                }
            }
        }
        // Если список пустой
        else if (fifamClubId) { // Если не СА
                                // Если в аренде - добавляем две записи - основной клуб и клуб аренды
            if (IsLoaned()) {
                m_playerHistory.records.resize(2);
                m_playerHistory.records[0].m_nClubId = fifaPlayer.loanedFrom;
                m_playerHistory.records[0].m_start = m_playerContract.m_joinedClubDate;
                m_playerHistory.records[0].m_start.Validate_BeforeSeason();
                m_playerHistory.records[0].m_end.Set(30, 6, Date::GetCurrentYear());
                m_playerHistory.records[0].m_nState = HISTORYCONTRACT_NONE;
                if (m_playerHistory.records[0].m_start >= m_playerHistory.records[0].m_end)
                    m_playerHistory.records[0].m_start.MoveSesonBack(true);
                m_playerHistory.records[1].m_nClubId = fifamClubId;
                m_playerHistory.records[1].m_start.Set(1, 7, Date::GetCurrentYear());
                m_playerHistory.records[1].m_nState = HISTORYCONTRACT_LOAN | HISTORYCONTRACT_CURRENT_CLUB;
                m_playerHistory.records[1].m_end.Set(30, 6, Date::GetCurrentYear() + 1);
            }
            // Добавляем только один клуб - основной
            else {
                m_playerHistory.records.resize(1);
                m_playerHistory.records[0].m_nClubId = fifamClubId;
                m_playerHistory.records[0].m_start = m_playerContract.m_joinedClubDate;
                m_playerHistory.records[0].m_nState = HISTORYCONTRACT_CURRENT_CLUB;
                m_playerHistory.records[0].m_end.Set(30, 6, Date::GetCurrentYear() + 1);
            }
        }

        // Appearance
        m_nHeight = fifaPlayer.m_height;
        m_nWeight = fifaPlayer.m_weight;

        switch (fifaPlayer.m_skinToneCode)
        {
        case 1:
        case 2:
        case 3:
            m_nSkinColour = 0;
            break;
        case 4:
            m_nSkinColour = 1;
            break;
        case 5:
            m_nSkinColour = 2;
            break;
        case 6:
        case 7:
            m_nSkinColour = 3;
            break;
        case 8:
            m_nSkinColour = 4;
            break;
        case 9:
            m_nSkinColour = 5;
            break;
        case 10:
            m_nSkinColour = 6;
            break;
        default:
            m_nSkinColour = 0;
        }
        switch (fifaPlayer.m_skinTypeCode)
        {
        case 0:
            m_nVariation = 0;
            break;
        case 1:
            m_nVariation = 1;
            break;
        case 2:
            m_nVariation = 2;
            break;
        default:
            m_nVariation = 0;
        }
        // 0 - light blue 1 - brown 2 - grey-green 3 - green 4 - green-blue 5 -grey 6 - blue
        switch (fifaPlayer.m_eyeColorCode)
        {
        case 1:
            m_nEyeColour = 6;
            break;
        case 2:
            m_nEyeColour = 0;
            break;
        case 3:
        case 4:
        case 9:
            m_nEyeColour = 1;
            break;
        case 5:
            m_nEyeColour = 2;
            break;
        case 6:
        case 7:
            m_nEyeColour = 3;
            break;
        case 8:
        case 10:
            m_nEyeColour = 5;
            break;
        default:
            m_nEyeColour = 5;
        }
        if (fifaPlayer.m_sideburnsCode)
            m_nSideburns = 1;

        // 0 - none
        // 1 - chin beard  _
        // 2 - kevin beard \_/
        // 3 - full goatee |_|
        // 4 - full beard \|_|/
        // 5 - tash --
        // 6 - shadow
        // 7 - goatee \/
        // 8 - unshaven
        switch (fifaPlayer.m_facialHairTypeCode)
        {
        case 0:
            m_nBeard = 0;
            break;
        case 1:
            m_nBeard = 1;
            break;
        case 2:
        case 11:
            m_nBeard = 2;
            break;
        case 3:
        case 9:
        case 12:
            m_nBeard = 3;
            break;
        case 4:
        case 8:
            m_nBeard = 4;
            break;
        case 5:
            m_nBeard = 5;
            break;
        case 6:
        case 13: // or none?
        case 14: // or none?
        case 15:
            m_nBeard = 6;
            break;
        case 7:
            m_nBeard = 7;
            break;
        default:
            m_nBeard = 0;
        }
        // 0 - black, 1 - blonde, 2 - brown, 3 - medium blonde, 4 - red
        switch (fifaPlayer.m_facialHairColorCode)
        {
        case 0:
            m_nBeardColour = 0;
            break;
        case 1:
            m_nBeardColour = 1;
            break;
        case 2:
            m_nBeardColour = 2;
            break;
        case 3:
            m_nBeardColour = 3;
            break;
        case 4:
            m_nBeardColour = 4;
            break;
        default:
            m_nBeardColour = 0;
            break;
        }
        if (fifaPlayer.m_hairTypeCode <= 96)
        {
            switch (fifaPlayer.m_hairTypeCode)
            {
            case 42:
                m_nHairStyle = 78;
                break;
            case 60:
                m_nHairStyle = 43;
                break;
                //case 50:
                //    m_nHairStyle = 78;
                //    break;
            default:
                m_nHairStyle = fifaPlayer.m_hairTypeCode;
            }
        }
        else
        {
            switch (fifaPlayer.m_hairTypeCode)
            {
            case 98:
                m_nHairStyle = 62;
                break;
            case 99:
                m_nHairStyle = 68;
                break;
            case 100:
                m_nHairStyle = 17;
                break;
            case 101:
                m_nHairStyle = 86;
                break;
            case 102:
                m_nHairStyle = 70;
                break;
            case 103:
                m_nHairStyle = 13;
                break;
            case 104:
                m_nHairStyle = 83;
                break;
            case 105:
                m_nHairStyle = 65;
                break;
            case 106:
                m_nHairStyle = 24;
                break;
            case 107:
                m_nHairStyle = 93;
                break;
            case 108:
                m_nHairStyle = 83;
                break;
            case 109:
                m_nHairStyle = 27;
                break;
            case 110:
                m_nHairStyle = 71;
                break;
            case 111:
                m_nHairStyle = 67;
                break;
            case 112:
                m_nHairStyle = 17;
                break;
            case 113:
                m_nHairStyle = 24;
                break;
            case 114:
                m_nHairStyle = 47;
                break;
            case 115:
                m_nHairStyle = 24;
                break;
            case 116:
                m_nHairStyle = 69;
                break;
            case 117:
                m_nHairStyle = 43;
                break;
            case 118:
                m_nHairStyle = 78;
                break;
            case 119:
                m_nHairStyle = 78;
                break;
            case 120:
                m_nHairStyle = 1;
                break;
            case 121:
                m_nHairStyle = 65;
                break;
            case 122:
                m_nHairStyle = 65;
                break;
            case 123:
                m_nHairStyle = 88;
                break;
            case 124:
                m_nHairStyle = 88;
                break;
            case 125:
                m_nHairStyle = 78;
                break;
            case 126:
                m_nHairStyle = 64;
                break;
            case 127:
                m_nHairStyle = 40;
                break;
            case 128:
                m_nHairStyle = 66;
                break;
            case 129:
                m_nHairStyle = 57;
                break;
            case 130:
                m_nHairStyle = 67;
                break;
            case 131:
                m_nHairStyle = 66;
                break;
            case 132:
                m_nHairStyle = 88;
                break;
            case 133:
                m_nHairStyle = 86;
                break;
            case 134:
                m_nHairStyle = 78;
                break;
            case 135:
                m_nHairStyle = 65;
                break;
            case 136:
                m_nHairStyle = 95;
                break;
            case 137:
                m_nHairStyle = 65;
                break;
            case 138:
                m_nHairStyle = 19;
                break;
            case 139:
                m_nHairStyle = 13;
                break;
            case 140:
                m_nHairStyle = 86;
                break;
            case 141:
                m_nHairStyle = 78;
                break;
            case 142:
                m_nHairStyle = 38;
                break;
            case 143:
                m_nHairStyle = 38;
                break;
            case 144:
                m_nHairStyle = 14;
                break;
            case 145:
                m_nHairStyle = 77;
                break;
            case 146:
                m_nHairStyle = 77;
                break;
            case 147:
                m_nHairStyle = 77;
                break;
            case 148:
                m_nHairStyle = 65;
                break;
            case 149:
                m_nHairStyle = 78;
                break;
            case 150:
                m_nHairStyle = 31;
                break;
            case 151:
                m_nHairStyle = 78;
                break;
            default:
                m_nHairStyle = 78;
            }
        }
        // 0 - blonde, 1 - black, 2 - medium blonde, 3 - darkbrown, 4 - platinumblonde, 5 - lightbrown, 6 - brown, 7 - red
        switch (fifaPlayer.m_hairColorCode)
        {
        case 0:
            m_nHairColour = 0;
            break;
        case 1:
            m_nHairColour = 1;
            break;
        case 2:
        case 9:
            m_nHairColour = 2;
            break;
        case 3:
            m_nHairColour = 3;
            break;
        case 4:
        case 8:
            m_nHairColour = 4;
            break;
        case 5:
            m_nHairColour = 5;
            break;
        case 6:
            m_nHairColour = 6;
            break;
        case 7:
        case 12:
            m_nHairColour = 7;
            break;
        default:
            m_nHairColour = 1;
        }

        // 0-25
        // 26-46
        // 47-65
        // 66-94
        // 93-103
        // 105-111
        // 112-117
        // 118-123
        // 124-127
        // 130
        // 131-134

        if (fifaPlayer.m_headTypeCode <= 25)
            m_nFace = fifaPlayer.m_headTypeCode;
        else if (fifaPlayer.m_headTypeCode >= 2000 && fifaPlayer.m_headTypeCode <= 2010)
            m_nFace = fifaPlayer.m_headTypeCode - 2000 + 93;
        else if (fifaPlayer.m_headTypeCode >= 4000 && fifaPlayer.m_headTypeCode <= 4003)
            m_nFace = fifaPlayer.m_headTypeCode - 4000 + 124;
        else if (fifaPlayer.m_headTypeCode >= 3500 && fifaPlayer.m_headTypeCode <= 3505)
            m_nFace = fifaPlayer.m_headTypeCode - 3500 + 118;
        else if (fifaPlayer.m_headTypeCode >= 1500 && fifaPlayer.m_headTypeCode <= 1528)
            m_nFace = fifaPlayer.m_headTypeCode - 1500 + 66;
        else if (fifaPlayer.m_headTypeCode >= 2500 && fifaPlayer.m_headTypeCode <= 2506)
            m_nFace = fifaPlayer.m_headTypeCode - 2500 + 105;
        else if (fifaPlayer.m_headTypeCode >= 1000 && fifaPlayer.m_headTypeCode <= 1018)
            m_nFace = fifaPlayer.m_headTypeCode - 1000 + 47;
        else if (fifaPlayer.m_headTypeCode >= 5000 && fifaPlayer.m_headTypeCode <= 5003)
            m_nFace = fifaPlayer.m_headTypeCode - 5000 + 131;
        else if (fifaPlayer.m_headTypeCode == 4525 || fifaPlayer.m_headTypeCode == 4502)
            m_nFace = 130;
        else if (fifaPlayer.m_headTypeCode >= 500 && fifaPlayer.m_headTypeCode <= 520)
            m_nFace = fifaPlayer.m_headTypeCode - 500 + 26;
        else if (fifaPlayer.m_headTypeCode >= 3000 && fifaPlayer.m_headTypeCode <= 3005)
            m_nFace = fifaPlayer.m_headTypeCode - 3000 + 112;
        else if (fifaPlayer.m_headTypeCode == 1528)
            m_nFace = 104;
        else if (fifaPlayer.m_headTypeCode == 4500)
            m_nFace = 128;
        else if (fifaPlayer.m_headTypeCode == 4501)
            m_nFace = 129;
        else if (fifaPlayer.m_headTypeCode == 2023)
            m_nFace = 22;
        else if (fifaPlayer.m_headTypeCode == 2015)
            m_nFace = 111;
        else if (fifaPlayer.m_headTypeCode == 2016)
            m_nFace = 100;
        else
            m_nFace = 0;

        if (m_nFace == 0) {
            switch (m_nSkinColour) {
            case 1:
                m_nFace = 33;
                break;
            case 2:
            case 3:
                m_nFace = 67;
                break;
            case 4:
                m_nFace = 48;
                break;
            case 5:
                m_nFace = 129;
                break;
            case 6:
                m_nFace = 134;
                break;
            }
        }

        if (m_nId == 158023)
            m_nFace = 22;
        else if (m_nId == 41)
            m_nFace = 29;
        else if (m_nId == 189332)
            m_nFace = 10;
        else if (m_nId == 178603) // Hummels
            m_nBeard = 3; // Full Goatee
        else if (m_nId == 193082) // Cuadrado
            m_nHairStyle = 97;
        //else if(m_nId == 48940) // Cech
        //    m_nHairStyle = 50;

        m_nSpecialFace = fifaPlayer.faceId;

        m_nShoeType = GetPlayerShoesIdFromFifaId(fifaPlayer.m_shoeTypeCode, fifaPlayer.m_shoeColorCode[0], fifaPlayer.m_shoeColorCode[1]);
    }








    void UpdateFromNewFM(FifaPlayer &fifaPlayer, unsigned int countryId = 0) {
        RemoveConditions();
        Player *newPlayer = FindPlayerInNewFM(*this, &fifaPlayer, countryId);
        m_newPlayer = newPlayer;

        if (!newPlayer)
        {
            unsigned int qid;
            swscanf(m_szPseudonym, L"%d", &qid);
            wsprintfW(m_szPseudonym, L"%d $$$$$", qid);
        }
        else
        {
            unsigned int qid;
            swscanf(m_szPseudonym, L"%d", &qid);
            wsprintfW(m_szPseudonym, L"%d", qid);
        }
        if (newPlayer && m_playerHistory.records.size() == 0) {
            if (newPlayer->m_nHero > m_nHero) {
                m_nHero = newPlayer->m_nHero;
            }
            if (!m_bCurrentlyInNationalTeam) {
                m_bRetiredFromNationalTeam = newPlayer->m_bRetiredFromNationalTeam;
            }
            m_nMatchesForNationalTeam = newPlayer->m_nMatchesForNationalTeam;
            m_nGoalsForNationalTeam = newPlayer->m_nGoalsForNationalTeam;
            if (!m_n64CharacterFlags)
                m_n64CharacterFlags = newPlayer->m_n64CharacterFlags;
            if (newPlayer->m_nLeadership > m_nLeadership)
                m_nLeadership = newPlayer->m_nLeadership;
            if (!m_bBasque)
                m_bBasque = newPlayer->m_bBasque;
            if (!m_bNaturalised)
                m_bNaturalised = newPlayer->m_bNaturalised;

            if (m_nNationality != newPlayer->m_nNationality)
                m_n2ndNationality = newPlayer->m_nNationality;
            else
                m_n2ndNationality = newPlayer->m_n2ndNationality;

            m_nLanguages[0] = newPlayer->m_nLanguages[0];
            m_nLanguages[1] = newPlayer->m_nLanguages[1];
            m_nLanguages[2] = newPlayer->m_nLanguages[2];
        }

        // Fill with new data if available
        if (newPlayer) {
            if (!m_bCurrentlyInNationalTeam) {
                m_bRetiredFromNationalTeam = newPlayer->m_bRetiredFromNationalTeam;
            }
            if (newPlayer->m_nMatchesForNationalTeam > m_nMatchesForNationalTeam)
                m_nMatchesForNationalTeam = newPlayer->m_nMatchesForNationalTeam;
            if (newPlayer->m_nGoalsForNationalTeam > m_nGoalsForNationalTeam)
                m_nGoalsForNationalTeam = newPlayer->m_nGoalsForNationalTeam;
            if (!m_bBasque)
                m_bBasque = newPlayer->m_bBasque;
            if (!m_bNaturalised)
                m_bNaturalised = newPlayer->m_bNaturalised;
            if (!m_n2ndNationality) {
                if (m_nNationality != newPlayer->m_nNationality)
                    m_n2ndNationality = newPlayer->m_nNationality;
                else
                    m_n2ndNationality = newPlayer->m_n2ndNationality;
            }
            if (!m_nLanguages[0])
                m_nLanguages[0] = newPlayer->m_nLanguages[0];
            if (!m_nLanguages[1])
                m_nLanguages[1] = newPlayer->m_nLanguages[1];
            if (!m_nLanguages[2])
                m_nLanguages[2] = newPlayer->m_nLanguages[2];
        }

        if (newPlayer && m_playerHistory.records.size() == 0) {
            if (newPlayer->m_ability.TacAwareness > (m_ability.TacAwareness - 10))
                m_ability.TacAwareness = newPlayer->m_ability.TacAwareness;
            if (newPlayer->m_ability.Composure > (m_ability.Composure - 10))
                m_ability.Composure = newPlayer->m_ability.Composure;
            if (newPlayer->m_ability.Consistency > (m_ability.Consistency - 10))
                m_ability.Consistency = newPlayer->m_ability.Consistency;
        }


        // Get injury/ban info from new FM
        if (newPlayer) {
            for (int c = 0; c < newPlayer->m_nNumConditions; c++) {
                if (newPlayer->m_conditions[c].m_nType == CONDITION_BAN) {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_BAN;
                    Date endDate;
                    endDate.SetFromValue(newPlayer->m_conditions[c].Ban.m_nEndDate);
#ifdef TRANSLATE_DATES
                    endDate.year -= YEARS_DIFFERENCE;
                    endDate.FixAfterTranslating();
#endif
                    m_conditions[m_nNumConditions].Ban.m_nEndDate = endDate.GetValue();
                    m_nNumConditions++;
                }
                else if (newPlayer->m_conditions[c].m_nType == CONDITION_INJURY) {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_INJURY;
                    m_conditions[m_nNumConditions].Injury.m_nInjuryType = newPlayer->m_conditions[c].Injury.m_nInjuryType;
                    Date startDate;
                    startDate.SetFromValue(newPlayer->m_conditions[c].Injury.m_nStart);
#ifdef TRANSLATE_DATES
                    startDate.year -= YEARS_DIFFERENCE;
                    startDate.FixAfterTranslating();
#endif
                    m_conditions[m_nNumConditions].Injury.m_nStart = startDate.GetValue();
                    Date endDate;
                    endDate.SetFromValue(newPlayer->m_conditions[c].Injury.m_nEnd);
#ifdef TRANSLATE_DATES
                    endDate.year -= YEARS_DIFFERENCE;
                    endDate.FixAfterTranslating();
#endif
                    m_conditions[m_nNumConditions].Injury.m_nEnd = endDate.GetValue();
                    m_nNumConditions++;
                }
            }
        }

        // History
        if (newPlayer) {
            if (newPlayer->m_playerHistory.records.size() > 0) {
                m_playerHistory.records.clear();
                m_playerHistory.records.resize(newPlayer->m_playerHistory.records.size());
                for (int i = 0; i < newPlayer->m_playerHistory.records.size(); i++) {
                    m_playerHistory.records[i] = newPlayer->m_playerHistory.records[i];
#ifdef TRANSLATE_DATES
                    m_playerHistory.records[i].m_start.year -= YEARS_DIFFERENCE;
                    m_playerHistory.records[i].m_start.FixAfterTranslating();
                    m_playerHistory.records[i].m_end.year -= YEARS_DIFFERENCE;
                    m_playerHistory.records[i].m_end.FixAfterTranslating();
#endif
                }
            }
        }


    }

    void UpdateFromNewFM_MY_ENGLAND() {
        RemoveConditions();
        Player *newPlayer = FindPlayerInNewFM_MY_ENGLAND(*this);
        m_newPlayer = newPlayer;

        if (newPlayer) {
            m_nMatchesForNationalTeam = newPlayer->m_nMatchesForNationalTeam;
            m_nGoalsForNationalTeam = newPlayer->m_nGoalsForNationalTeam;
        }


        // Get injury/ban info from new FM
        if (newPlayer) {
            for (int c = 0; c < newPlayer->m_nNumConditions; c++) {
                if (newPlayer->m_conditions[c].m_nType == CONDITION_BAN) {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_BAN;
                    m_conditions[m_nNumConditions].Ban.m_nEndDate = newPlayer->m_conditions[c].Ban.m_nEndDate;
                    m_nNumConditions++;
                }
                else if (newPlayer->m_conditions[c].m_nType == CONDITION_INJURY) {
                    m_conditions[m_nNumConditions].m_nType = CONDITION_INJURY;
                    m_conditions[m_nNumConditions].Injury.m_nInjuryType = newPlayer->m_conditions[c].Injury.m_nInjuryType;
                    m_conditions[m_nNumConditions].Injury.m_nStart = newPlayer->m_conditions[c].Injury.m_nStart;
                    m_conditions[m_nNumConditions].Injury.m_nEnd = newPlayer->m_conditions[c].Injury.m_nEnd;
                    m_nNumConditions++;
                }
            }
        }

        // History
        if (newPlayer) {
            if (newPlayer->m_playerHistory.records.size() > 0) {
                m_playerHistory.records.clear();
                m_playerHistory.records.resize(newPlayer->m_playerHistory.records.size());
                for (int i = 0; i < newPlayer->m_playerHistory.records.size(); i++) {
                    m_playerHistory.records[i] = newPlayer->m_playerHistory.records[i];
                }
            }
        }
    }

    char *GetQuickNameOriginal(FifaPlayer *fifaPlayer) {
        if (!m_bQuickNameOriginalGenerated) {
            if (fifaPlayer->m_szQuickName[0] != '\0') {
                strcpy(m_szQuickNameOriginal, fifaPlayer->m_szQuickName);
            }
            else {
                if (m_szNickname[0])
                {
                    char nickname[MAX_PATH];
                    RemoveAccented(nickname, m_szNickname);
                    char name1[MAX_PATH];
                    size_t name1len = strlen(nickname);
                    int counter = 0;
                    for (int i = 0; i < name1len; i++)
                    {
                        if (nickname[i] != ' ')
                            name1[counter++] = nickname[i];
                    }
                    name1[counter] = '\0';
                    sprintf(m_szQuickNameOriginal, "%s%02d%02d%04d", name1, fifaPlayer->m_birthDate.day, fifaPlayer->m_birthDate.month, fifaPlayer->m_birthDate.year);
                }
                else
                {
                    char firstName[MAX_PATH];
                    char lastName[MAX_PATH];
                    RemoveAccented(firstName, m_szFirstName);
                    RemoveAccented(lastName, m_szLastName);
                    char name1[MAX_PATH];
                    char name2[3];
                    size_t name1len = strlen(lastName);
                    int counter = 0;
                    for (int i = 0; i < name1len; i++)
                    {
                        if (lastName[i] != ' ')
                            name1[counter++] = lastName[i];
                    }
                    name1[counter] = '\0';
                    for (int i = 0; i < 2; i++)
                    {
                        if (firstName[i] == ' ')
                        {
                            name2[i] = '\0';
                            break;
                        }
                        else
                            name2[i] = firstName[i];
                    }
                    name2[2] = '\0';
                    sprintf(m_szQuickNameOriginal, "%s%s%02d%02d%04d", name1, name2, fifaPlayer->m_birthDate.day, fifaPlayer->m_birthDate.month, fifaPlayer->m_birthDate.year);
                }
            }
            m_bQuickNameOriginalGenerated = true;
        }
        return m_szQuickNameOriginal;
    }

    void GetPlayerNameToDisplay(wchar_t *output) {
        if (m_szNickname[0])
            wcscpy(output, m_szNickname);
        else
            wsprintfW(output, L"%s %s", m_szFirstName, m_szLastName);
    }

    char *GetQuickName() {
        if (!m_bQuickNameGenerated) {
            if (m_szNickname[0]) {
                char nickname[MAX_PATH];
                RemoveAccented(nickname, m_szNickname);
                char name1[MAX_PATH];
                size_t name1len = strlen(nickname);
                int counter = 0;
                for (int i = 0; i < name1len; i++) {
                    if (nickname[i] != ' ')
                        name1[counter++] = nickname[i];
                }
                name1[counter] = '\0';
                sprintf(m_szQuickName, "%s%02d%02d%04d", name1, m_birthday.day, m_birthday.month, m_birthday.year);
            }
            else {
                char firstName[MAX_PATH];
                char lastName[MAX_PATH];
                RemoveAccented(firstName, m_szFirstName);
                RemoveAccented(lastName, m_szLastName);
                char name1[MAX_PATH];
                char name2[3];
                size_t name1len = strlen(lastName);
                int counter = 0;
                for (int i = 0; i < name1len; i++) {
                    if (lastName[i] != ' ')
                        name1[counter++] = lastName[i];
                }
                name1[counter] = '\0';
                for (int i = 0; i < 2; i++) {
                    if (firstName[i] == ' ') {
                        name2[i] = '\0';
                        break;
                    }
                    else
                        name2[i] = firstName[i];
                }
                name2[2] = '\0';
                sprintf(m_szQuickName, "%s%s%02d%02d%04d", name1, name2, m_birthday.day, m_birthday.month, m_birthday.year);
            }
            m_bQuickNameGenerated = true;
        }
        return m_szQuickName;
    }

    void RemoveConditions() {
        bool hasLoan = false;
        for (int c = 0; c < m_nNumConditions; c++) {
            if (m_conditions[c].m_nType == CONDITION_LOAN) {
                m_conditions[0].m_nType = CONDITION_LOAN;
                m_conditions[0].Loan.m_nClubId = m_conditions[c].Loan.m_nClubId;
                m_conditions[0].Loan.m_nStart = m_conditions[c].Loan.m_nStart;
                m_conditions[0].Loan.m_nEnd = m_conditions[c].Loan.m_nEnd;
                m_conditions[0].Loan.m_nBuyValue = m_conditions[c].Loan.m_nBuyValue;
                m_nNumConditions = 1;
                hasLoan = true;
                break;
            }
        }
        if (!hasLoan)
            m_nNumConditions = 0;
    }

    void RemoveConditions_All() {
        m_nNumConditions = 0;
    }

    bool IsLoaned() {
        for (int c = 0; c < m_nNumConditions; c++) {
            if (m_conditions[c].m_nType == CONDITION_LOAN) {
                return true;
            }
        }
        return false;
    }

    Condition *GetLoanCondition() {
        for (int c = 0; c < m_nNumConditions; c++) {
            if (m_conditions[c].m_nType == CONDITION_LOAN) {
                return &m_conditions[c];
            }
        }
        return NULL;
    }

    void FixHistoryEntries(bool freeAgent) {

    }

    void UpdateLoan(FifaPlayer &fifaPlayer)
    {
        m_playerContract.m_contractValidUntil.Validate_After_ThisSeason();
        RemoveConditions_All();
        if (fifaPlayer.loanedFrom)
        {
            m_conditions[m_nNumConditions].m_nType = CONDITION_LOAN;
            m_conditions[m_nNumConditions].Loan.m_nBuyValue = -1;
            m_conditions[m_nNumConditions].Loan.m_nClubId = FixClubIdForPlayerHistory(fifaPlayer.loanedFrom);
            m_conditions[m_nNumConditions].Loan.m_nStart = Date::GetValueForDate(CURRENT_YEAR, 7, 1);
            Date loandEnd = fifaPlayer.loanEndDate;
#ifdef TRANSLATE_DATES
            loandEnd.year -= YEARS_DIFFERENCE;
            if (loandEnd.FixAfterTranslating()) {
                Error(L"Fixed player %s %s (%s) loand end date (%02d.%02d.%04d)", m_szFirstName, m_szLastName, m_szNickname,
                    fifaPlayer.loanEndDate.day, fifaPlayer.loanEndDate.month, fifaPlayer.loanEndDate.year);
            }
#endif
            FILE *ff = fopen("loans.txt", "at");
            fprintf(ff, "%s    loan:%02d.%02d.%04d contract:%02d.%02d.%04d", GetQuickNameOriginal(&fifaPlayer),
                loandEnd.day, loandEnd.month, loandEnd.year,
                m_playerContract.m_contractValidUntil.day, m_playerContract.m_contractValidUntil.month,
                m_playerContract.m_contractValidUntil.year);
            if (loandEnd.year > (Date::GetCurrentYear() + 2))
                loandEnd.Set(30, 6, Date::GetCurrentYear() + 2);
            else
                loandEnd.Validate_AfterSeason();
            m_conditions[m_nNumConditions].Loan.m_nEnd = loandEnd.GetValue();
            if (m_playerContract.m_contractValidUntil.Validate_GreaterOrEqual(loandEnd)) {
                //Error("Loan date is greater than contract date");
                fprintf(ff, "               ERROR  |  %02d.%02d.%04d", m_playerContract.m_contractValidUntil.day, m_playerContract.m_contractValidUntil.month,
                    m_playerContract.m_contractValidUntil.year);
            }
            fputs("\n", ff);
            fclose(ff);
            m_nNumConditions++;
        }
    }

    void ConvertSkillsToFM12(PlayerSkills12 &fm12) {
        fm12.Acceleration = m_ability.Acceleration;
        fm12.Aggression = m_ability.Aggression;
        fm12.Agility = m_ability.Agility;
        fm12.Anticipation = m_ability.PosDefensive;
        fm12.Composure = m_ability.Composure;
        fm12.Concentration = (m_ability.Composure + m_ability.Reactions) / 2;
        fm12.Consistency = m_ability.Consistency;
        fm12.Corners = m_ability.Corners;
        fm12.Creativity = m_ability.Vision;
        fm12.Crosses = m_ability.Crossing;
        fm12.Crossing = m_ability.Crossing;
        fm12.Decision = (m_ability.TackleSliding + m_ability.Reactions) / 2;
        fm12.Determination = (m_ability.TacAwareness + m_ability.Consistency) / 2;
        fm12.Dribbling = m_ability.Dribbling;
        fm12.Finishing = m_ability.Finishing;
        fm12.Flair = m_ability.Vision;
        fm12.ForwardRuns = m_ability.PosOffensive;
        fm12.FreeKicks = m_ability.FreeKicks;
        fm12.GoalKicks = m_ability.Kicking;
        fm12.Handling = m_ability.Handling;
        fm12.Header = m_ability.Header;
        fm12.Intimidation = m_ability.Aggression;
        fm12.Jumping = m_ability.Jumping;
        switch (m_nLeadership) {
        case 0:
        case 1:
            fm12.Leadership = rand() % 10 + 25;
            break;
        case 2:
            fm12.Leadership = rand() % 10 + 35;
            break;
        case 3:
            fm12.Leadership = rand() % 10 + 45;
            break;
        case 4:
            fm12.Leadership = rand() % 10 + 55;
            break;
        case 5:
            fm12.Leadership = rand() % 7 + 65;
            break;
        case 6:
            fm12.Leadership = rand() % 6 + 72;
            break;
        case 7:
            fm12.Leadership = rand() % 5 + 78;
            break;
        case 8:
            fm12.Leadership = rand() % 5 + 83;
            break;
        case 9:
            fm12.Leadership = rand() % 7 + 88;
            break;
        default:
            fm12.Leadership = rand() % 5 + 95;
            break;
        }
        fm12.LongPassing = m_ability.LongPassing;
        fm12.LongShots = m_ability.LongShots;
        fm12.ManMarking = m_ability.ManMarking;
        fm12.OneOnOne = m_ability.OneOnOne;
        fm12.Pace = m_ability.Pace;
        fm12.Passing = m_ability.Passing;
        fm12.PenaltyShot = m_ability.PenaltyShot;
        fm12.Positioning = m_ability.Positioning;
        fm12.Punching = m_ability.Handling;
        fm12.ShotPower = m_ability.ShotPower;
        fm12.ShotStopping = (m_ability.Reflexes + m_ability.Diving) / 2;
        fm12.Stamina = m_ability.Stamina;
        fm12.Strength = m_ability.Strength;
        fm12.Tackling = m_ability.TackleSliding;
        unsigned int teamWork = m_ability.TacAwareness;
        if (teamWork > 70)
            teamWork = 70;
        if (m_character.Teamplayer)
            teamWork += rand() % 10 + 20;
        fm12.TeamWork = teamWork;
        fm12.Technique = m_ability.BallControl;
        fm12.Touch = m_ability.BallControl;
        if (m_character.LongThrowOuts)
            fm12.Throwing = 90;
        else
            fm12.Throwing = rand() % 31 + 40;
        fm12.WorkRate = m_ability.TacAwareness;
    }
};