#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

// @since FM07
ENUM_BEGIN(FifamRefereeType, UChar)
    ENUM_MEMBER(0, None,                           L"None")
    ENUM_MEMBER(1, WorldClassReferee,              L"World class referee")
    ENUM_MEMBER(2, SeverePerson,                   L"Severe person")
    ENUM_MEMBER(3, DoesNotInterfereMuchInTheGame,  L"Does not interfere much in the game")
    ENUM_MEMBER(4, BraveDecisionMaker,             L"Brave decision maker")
    ENUM_MEMBER(5, Coward,                         L"Coward")
    ENUM_MEMBER(6, DoesNotLikeManagersInterfering, L"Does not like managers interfering") // until FM09
    ENUM_DEFAULT_VALUE(None);

    inline static Vector<Pair<UChar, UChar>> m07ConvertTable = {
        { 0, WorldClassReferee              },
        { 1, SeverePerson                   },
        { 2, DoesNotInterfereMuchInTheGame  },
        { 3, DoesNotLikeManagersInterfering },
        { 4, BraveDecisionMaker             },
        { 5, Coward                         },
        { 6, None                           }
    };

    inline static Vector<Pair<UChar, UChar>> m09ConvertTable = {
        { 0, None                          },
        { 1, WorldClassReferee             },
        { 2, SeverePerson                  },
        { 3, DoesNotInterfereMuchInTheGame },
        { 4, BraveDecisionMaker            },
        { 5, Coward                        }
    };

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 8)
                TranslateFrom(id, m07ConvertTable);
            else
                TranslateFrom(id, m09ConvertTable);
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (writer.GetGameId() <= 8)
            writer.WriteOne(TranslateTo(m07ConvertTable));
        else
            writer.WriteOne(TranslateTo(m09ConvertTable));
    }
ENUM_END(FifamRefereeType)
