#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubStaffPosition, UChar)
    ENUM_MEMBER( 1, Manager,             L"Manager")
    ENUM_MEMBER( 2, ChiefExec,           L"Chief Executive")
    ENUM_MEMBER( 3, President,           L"President")
    ENUM_MEMBER( 6, AssistantCoach,      L"Assistant Coach")
    ENUM_MEMBER( 7, AmateurCoach,        L"Amateur Coach")
    ENUM_MEMBER( 8, YouthCoach,          L"Youth Coach")
    ENUM_MEMBER( 9, FitnessCoach,        L"Fitness Coach")
    ENUM_MEMBER(10, GoalkeeperCoach,     L"Goalkeeper Coach")
    ENUM_MEMBER(11, TeamDoctor,          L"Team Doctor")
    ENUM_MEMBER(12, SpecialistBone,      L"Specialist Bone")
    ENUM_MEMBER(13, SpecialistKnee,      L"Specialist Knee")
    ENUM_MEMBER(14, SpecialistMuscle,    L"Specialist Muscle")
    ENUM_MEMBER(15, Masseur,             L"Masseur")
    ENUM_MEMBER(16, Psychologist,        L"Psychologist")
    ENUM_MEMBER(17, GeneralManager,      L"General Manager")
    ENUM_MEMBER(18, MarketingManager,    L"Marketing Manager")
    ENUM_MEMBER(19, ConstructionManager, L"Construction Manager")
    ENUM_MEMBER(20, SportsDirector,      L"Sports Director")
    ENUM_MEMBER(21, FanRepresentative,   L"Fan Representative")
    ENUM_MEMBER(22, Spokesperson,        L"Spokesperson")
    ENUM_MEMBER(23, Lawyer,              L"Lawyer")
    ENUM_MEMBER(24, GeneralScout,        L"General Scout")

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 8) {
                static Vector<Pair<UChar, UChar>> table = {
                    { 1,  Manager },
                    { 2,  ChiefExec },
                    { 3,  President },
                    { 4,  AssistantCoach },
                    { 5,  GoalkeeperCoach },
                    { 6,  AmateurCoach },
                    { 7,  YouthCoach },
                    { 8,  GeneralScout },
                    { 9,  TeamDoctor },
                    { 10, SpecialistKnee },
                    { 11, SpecialistBone },
                    { 12, SpecialistMuscle },
                    { 13, Masseur },
                    { 14, Psychologist },
                    { 15, GeneralManager },
                    { 16, MarketingManager },
                    { 17, ConstructionManager },
                    { 18, FanRepresentative }
                };
                TranslateFrom(id, table);
            }
            else
                SetFromInt(id);
        }
    }

    ENUM_WRITE(writer) {
        if (writer.GetGameId() <= 8) {
            static Vector<Pair<UChar, UChar>> table = {
                {  1, Manager             }, 
                {  2, ChiefExec           }, 
                {  3, President           }, 
                {  4, AssistantCoach      }, 
                {  6, AmateurCoach        }, 
                {  7, YouthCoach          }, 
                { 13, FitnessCoach        }, 
                {  5, GoalkeeperCoach     }, 
                {  9, TeamDoctor          }, 
                { 11, SpecialistBone      }, 
                { 10, SpecialistKnee      }, 
                { 12, SpecialistMuscle    }, 
                { 13, Masseur             }, 
                { 14, Psychologist        }, 
                { 15, GeneralManager      }, 
                { 16, MarketingManager    }, 
                { 17, ConstructionManager }, 
                { 15, SportsDirector      }, 
                { 18, FanRepresentative   }, 
                { 18, Spokesperson        }, 
                { 15, Lawyer              }, 
                {  8, GeneralScout        }
            };
            writer.WriteOne(TranslateTo<UChar>(table, 11));
        }
        else
            writer.WriteOne(ToInt());
    }
ENUM_END(FifamClubStaffPosition)
