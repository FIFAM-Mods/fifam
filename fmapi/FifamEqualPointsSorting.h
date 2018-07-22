#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamEqualPointsSorting, UChar)
    ENUM_MEMBER(0, GoalDiff,      L"Goal Diff")
    ENUM_MEMBER(4, Goals,         L"Goals")
    ENUM_MEMBER(8, DirectMatches, L"Direct Matches Decide")
    ENUM_DEFAULT_VALUE(GoalDiff)
    
    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (!reader.IsVersionGreaterOrEqual(0x2007, 0xF)) {
                if (id == 1)
                    id = 4;
                else if (id != 0) {
                    SetUnknown(id);
                    SetDefaultValue();
                    return;
                }
            }
            SetFromInt(id);
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (!writer.IsVersionGreaterOrEqual(0x2007, 0xF)) {
            if (ToInt() == Goals)
                writer.WriteOne(1);
            else
                writer.WriteOne(0);
        }
        else
            writer.WriteOne(ToInt());
    }
ENUM_END(FifamEqualPointsSorting)
