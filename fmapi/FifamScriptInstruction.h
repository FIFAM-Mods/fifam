#pragma once
#include "FifamTypes.h"
#include "FifamCompID.h"
#include "FifamReadWrite.h"

ENUM_BEGIN(FifamScriptInstructionID, UChar)
    ENUM_MEMBER( 1, ID_BUILD_COUNTER,                      L"BUILD_COUNTER")
    ENUM_MEMBER( 2, ID_RESERVE_ASSESSMENT_TEAMS,           L"RESERVE_ASSESSMENT_TEAMS")
    ENUM_MEMBER( 3, ID_GET_CHAMP,                          L"GET_CHAMP")
    ENUM_MEMBER( 4, ID_GET_EUROPEAN_ASSESSMENT_TEAMS,      L"GET_EUROPEAN_ASSESSMENT_TEAMS")
    ENUM_MEMBER( 5, ID_FILL_ASSESSMENT_RESERVES,           L"FILL_ASSESSMENT_RESERVES")
    ENUM_MEMBER( 6, ID_GET_CHAMP_OR_RUNNER_UP,             L"GET_CHAMP_OR_RUNNER_UP")
    ENUM_MEMBER( 7, ID_GET_TAB_X_TO_Y,                     L"GET_TAB_X_TO_Y")
    ENUM_MEMBER( 8, ID_GET_TAB_SURE_X_TO_Y_Z,              L"GET_TAB_SURE_X_TO_Y_Z")
    ENUM_MEMBER( 9, ID_GET_TAB_LEVEL_X_TO_Y,               L"GET_TAB_LEVEL_X_TO_Y")
    ENUM_MEMBER(10, ID_GET_TAB_SPARE,                      L"GET_TAB_SPARE")
    ENUM_MEMBER(11, ID_GET_TAB_LEVEL_START_X_TO_Y,         L"GET_TAB_LEVEL_START_X_TO_Y")
    ENUM_MEMBER(12, ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER,  L"GET_EUROPEAN_ASSESSMENT_CUPWINNER")
    ENUM_MEMBER(13, ID_GET_WINNER,                         L"GET_WINNER")
    ENUM_MEMBER(14, ID_GET_LOSER,                          L"GET_LOSER")
    ENUM_MEMBER(15, ID_GET_POOL,                           L"GET_POOL")
    ENUM_MEMBER(16, ID_GET_NAT_UEFA5_WITH_HOST,            L"GET_NAT_UEFA5_WITH_HOST")
    ENUM_MEMBER(17, ID_GET_NAT_UEFA5_WITHOUT_HOST,         L"GET_NAT_UEFA5_WITHOUT_HOST")
    ENUM_MEMBER(18, ID_GET_NAT_SOUTH_AMERICA,              L"GET_NAT_SOUTH_AMERICA")
    ENUM_MEMBER(19, ID_GET_NAT_AMERICA,                    L"GET_NAT_AMERICA")
    ENUM_MEMBER(20, ID_GET_NAT_AFRICA,                     L"GET_NAT_AFRICA")
    ENUM_MEMBER(21, ID_GET_NAT_ASIA,                       L"GET_NAT_ASIA")
    ENUM_MEMBER(22, ID_GET_NAT_OCEANIA,                    L"GET_NAT_OCEANIA")
    ENUM_MEMBER(23, ID_GET_HOST,                           L"GET_HOST")
    ENUM_MEMBER(24, ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y, L"GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y")
    ENUM_MEMBER(25, ID_GET_INTERNATIONAL_SPARE,            L"GET_INTERNATIONAL_SPARE")
    ENUM_MEMBER(26, ID_GET_RUNNER_UP,                      L"GET_RUNNER_UP")
    ENUM_MEMBER(27, ID_GET_RELEGATED_TEAMS,                L"GET_RELEGATED_TEAMS")
    ENUM_MEMBER(28, ID_GET_INTERNATIONAL_TEAMS,            L"GET_INTERNATIONAL_TEAMS")
    ENUM_MEMBER(29, ID_GET_CC_FA_WINNER,                   L"GET_CC_FA_WINNER")
    ENUM_MEMBER(30, ID_GET_CC_SPARE,                       L"GET_CC_SPARE")
    ENUM_MEMBER(31, ID_GET_CHAMP_COUNTRY_TEAM,             L"GET_CHAMP_COUNTRY_TEAM")
    ENUM_MEMBER(32, ID_GET_RANDOM_NATIONAL_TEAM,           L"GET_RANDOM_NATIONAL_TEAM")
    ENUM_MEMBER(33, ID_CHANGE_TEAM_TYPES,                  L"CHANGE_TEAM_TYPES")
    ENUM_MEMBER(34, ID_GET_FAIRNESS_TEAM,                  L"GET_FAIRNESS_TEAM")
    ENUM_MEMBER(35, ID_COPY_LEAGUE_DATA,                   L"COPY_LEAGUE_DATA")
    ENUM_MEMBER(36, ID_GET_NATIONAL_TEAM,                  L"GET_NATIONAL_TEAM")
    ENUM_MEMBER(37, ID_GET_NATIONAL_TEAM_WITHOUT_HOST,     L"GET_NATIONAL_TEAM_WITHOUT_HOST")
    ENUM_MEMBER(38, ID_SHUFFLE_TEAMS,                      L"SHUFFLE_TEAMS")
    ENUM_MEMBER(39, ID_GET_TAB_LEVEL_INDOOR,               L"GET_TAB_LEVEL_INDOOR")
    ENUM_MEMBER(40, ID_BUILD_COUNTER,                      L"BUILD_COUNTER")
    ENUM_MEMBER(41, ID_BUILD_UEFA5,                        L"BUILD_UEFA5")
    ENUM_MEMBER(42, ID_GET_UEFA5_TAB,                      L"GET_UEFA5_TAB")
    ENUM_MEMBER(43, ID_GET_UEFA5_SURE_TAB,                 L"GET_UEFA5_SURE_TAB")
    ENUM_MEMBER(44, ID_GET_UEFA5_SURE_UIC,                 L"GET_UEFA5_SURE_UIC")
    ENUM_MEMBER(45, ID_GET_UEFA5_CHAMP_OR_FINALIST,        L"GET_UEFA5_CHAMP_OR_FINALIST")
ENUM_END(FifamScriptInstructionID)

bool ExtractCompetitionID(FifamCompID &outID, String &line, size_t &linePos) {
    size_t brace1Posn = line.find(linePos, L'{');
    if (brace1Posn != String::npos) {
        size_t brace2Posn = line.find(brace1Posn + 1, L'}');
        if (brace2Posn != String::npos) {
            outID.SetFromStr(line.substr(brace1Posn, brace2Posn - brace1Posn + 1));
            linePos = brace2Posn + 1;
            return true;
        }
    }
    return false;
}

bool ExtractCompetitionID(FifamCompID &outID, String &line) {
    size_t linePos = 0;
    return ExtractCompetitionID(outID, line, linePos);
}

class FifamScriptInstruction {
public:
    class GET_CHAMP {
    public:
        inline static Vector<UChar> supported_games = { 7, 8, 9, 10, 11, 12, 13, 14 };
        struct Parameters {
            FifamCompID compID;
        } params;
        FifamScriptInstructionID GetId() { return FifamScriptInstructionID::ID_GET_CHAMP; }
        GET_CHAMP(FifamCompID const &compId) {
            params.compID = compId;
        }
        GET_CHAMP(FifamReader &reader) {
            auto line = reader.ReadFullLine();
            ExtractCompetitionID(params.compID, line);
        }
        void Write(FifamWriter &writer) {
            writer.WriteLine(GetId().ToStr() + L" " + params.compID.ToStr());
        }
    };

    class GET_CHAMP_OR_RUNNER_UP {
    public:
        inline static Vector<UChar> supported_games = { 13, 14 };
        struct Parameters {
            FifamCompID compID;
        } params;
        FifamScriptInstructionID GetId() { return FifamScriptInstructionID::ID_GET_CHAMP_OR_RUNNER_UP; }
        GET_CHAMP_OR_RUNNER_UP(FifamCompID const &compId) {
            params.compID = compId;
        }
        GET_CHAMP_OR_RUNNER_UP(FifamReader &reader) {
            auto line = reader.ReadFullLine();
            ExtractCompetitionID(params.compID, line);
        }
        void Write(FifamWriter &writer) {
            String pr = writer.GetGameId() >= 13 ? L"" : L"; ";
            writer.Write(pr + GetId().ToStr() + L" " + params.compID.ToStr());
        }
    };


};

