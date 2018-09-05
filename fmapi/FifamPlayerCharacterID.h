#pragma once

#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerCharacterID, UChar) 
    ENUM_MEMBER( 0, AttitudeToWin,        L"AttitudeToWin") 
    ENUM_MEMBER( 1, Complacent,           L"Complacent") 
    ENUM_MEMBER( 2, StrongNerves,         L"StrongNerves") 
    ENUM_MEMBER( 3, WeakNerves,           L"WeakNerves") 
    ENUM_MEMBER( 4, Fairness,             L"Fairness") 
    ENUM_MEMBER( 5, HardMan,              L"HardMan") 
    ENUM_MEMBER( 6, Introvert,            L"Introvert") 
    ENUM_MEMBER( 7, IdentificationHigh,   L"IdentificationHigh") 
    ENUM_MEMBER( 8, IdentificationLow,    L"IdentificationLow") 
    ENUM_MEMBER( 9, LanguageGenius,       L"LanguageGenius") 
    ENUM_MEMBER(10, MediaDarling,         L"MediaDarling") 
    ENUM_MEMBER(11, ScandalProne,         L"ScandalProne") 
    ENUM_MEMBER(12, Flexibility,          L"Flexibility") 
    ENUM_MEMBER(13, Inflexibility,        L"Inflexibility") 
    ENUM_MEMBER(14, Ambition,             L"Ambition") 
    ENUM_MEMBER(15, HighTrainingWorkRate, L"HighTrainingWorkRate")
    ENUM_MEMBER(16, LazyInTraining,       L"LazyInTraining") 
    ENUM_MEMBER(17, Temperament,          L"Temperament") 
    ENUM_MEMBER(18, FansFavorite,         L"FansFavorite") 
    ENUM_MEMBER(19, Adaptability,         L"Adaptability") 
    ENUM_MEMBER(20, Professionalism,      L"Professionalism") 
    ENUM_MEMBER(21, LongThrows,           L"LongThrows") 
    ENUM_MEMBER(22, DrivenFreeKicks,      L"DrivenFreeKicks") 
    ENUM_MEMBER(23, InjuryProne,          L"InjuryProne") 
    ENUM_MEMBER(24, Composure,            L"Composure") 
    ENUM_MEMBER(25, Teamplayer,           L"Teamplayer") 
    ENUM_MEMBER(26, Egoist,               L"Egoist") 
    ENUM_MEMBER(27, LacksAmbition,        L"LacksAmbition") 
    ENUM_MEMBER(28, LittleInjuryProne,    L"LittleInjuryProne") 
    ENUM_MEMBER(29, LongThrowOuts,        L"LongThrowOuts") 
    ENUM_MEMBER(30, UninterestedInMedia,  L"UninterestedInMedia") 
    ENUM_MEMBER(31, Forgiving,            L"Forgiving") 
    ENUM_MEMBER(32, NotForgiving,         L"NotForgiving") 
    ENUM_MEMBER(33, HappyMan,             L"HappyMan") 
    ENUM_MEMBER(34, Critic,               L"Critic") 
    ENUM_MEMBER(35, Extrovert,            L"Extrovert") 
    ENUM_MEMBER(36, Independent,          L"Independent") 
    ENUM_MEMBER(37, NeedsAttention,       L"NeedsAttention") 
    ENUM_MEMBER(38, DoesntLikeNewLang,    L"DoesntLikeNewLang") 
    ENUM_MEMBER(39, Diva,                 L"Diva")
    ENUM_MEMBER(40, LifestyleIcon,        L"LifestyleIcon")
    ENUM_DEFAULT_VALUE(Ambition)

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x03)) {
                if (id == 0)
                    id = 39;
                else if (id == 1)
                    id = 40;
            }
            this->SetFromInt(id);
        }
    }

    ENUM_WRITE(writer) {
        UChar id = ToInt();
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x03)) {
            if (id == 39)
                id = 0;
            else if (id == 40)
                id = 1;
        }
        else {
            UChar maxId = 22;
            if (writer.GetGameId() > 12)
                maxId = 23;
            if (id >= maxId)
                id = Ambition;
        }
        writer.WriteOne(id);
    }
ENUM_END(FifamPlayerCharacterID)
