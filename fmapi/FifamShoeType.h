#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

// @since FM04
ENUM_BEGIN(FifamShoeType, UChar)
    ENUM_MEMBER( 1,  White,          L"White")            // since FM04
    ENUM_MEMBER( 2,  Black,          L"Black")            // since FM04
    ENUM_MEMBER( 3,  Blue,           L"Blue")             // since FM04
    ENUM_MEMBER( 4,  Red,            L"Red")              // since FM04
    ENUM_MEMBER( 5,  Yellow,         L"Yellow")           // since FM04
    ENUM_MEMBER( 6,  WhiteBlue,      L"White/Blue")       // since FM07
    ENUM_MEMBER( 7,  WhiteBlack,     L"White/Black")      // since FM07
    ENUM_MEMBER( 8,  WhiteLightBlue, L"White/Light Blue") // since FM07
    ENUM_MEMBER( 9,  YellowBlack,    L"Yellow/Black")     // since FM07
    ENUM_MEMBER(10,  Green,          L"Green")            // since FM10
    ENUM_MEMBER(11,  BlackRed,       L"Black/Red")        // since FM10
    ENUM_MEMBER(12,  Grey,           L"Grey")             // since FM10
    ENUM_MEMBER(13,  WhiteRed,       L"White/Red")        // since FM10
    ENUM_MEMBER(14,  LightYellow,    L"Light/Yellow")     // since FM10
    ENUM_MEMBER(15,  Orange,         L"Orange")           // since FM10
    ENUM_MEMBER(16,  DarkBlue,       L"Dark/Blue")        // since FM10
    ENUM_MEMBER(17,  LightGreen,     L"Light/Green")      // since FM10
    ENUM_MEMBER(18,  Magenta,        L"Magenta")          // since FM10
    ENUM_MEMBER(19,  BrightRed,      L"Bright/Red")       // since FM10

    ENUM_DEFAULT_VALUE(Black)
    
    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 6 && id >= WhiteBlue) {
                SetUnknown(id);
                SetDefaultValue();
            }
            else if (reader.GetGameId() <= 9 && id >= Green) {
                SetUnknown(id);
                SetDefaultValue();
            }
            else {
                if (id == YellowBlack && reader.IsVersionGreaterOrEqual(0x2012, 0x01) && !reader.IsVersionGreaterOrEqual(0x2013, 0x0B))
                    SetFromInt(Black);
                else
                    SetFromInt(id);
            }
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (writer.GetGameId() <= 6 && ToInt() >= WhiteBlue) {
            if (ToInt() == WhiteBlue)
                writer.WriteOne(White);
            else if (ToInt() == WhiteBlack)
                writer.WriteOne(White);
            else if (ToInt() == WhiteLightBlue)
                writer.WriteOne(White);
            else if (ToInt() == YellowBlack)
                writer.WriteOne(Yellow);
            else if (ToInt() == Green)
                writer.WriteOne(Yellow);
            else if (ToInt() == BlackRed)
                writer.WriteOne(Red);
            else if (ToInt() == Grey)
                writer.WriteOne(White);
            else if (ToInt() == WhiteRed)
                writer.WriteOne(Red);
            else if (ToInt() == LightYellow)
                writer.WriteOne(Yellow);
            else if (ToInt() == Orange)
                writer.WriteOne(Yellow);
            else if (ToInt() == DarkBlue)
                writer.WriteOne(Blue);
            else if (ToInt() == Magenta)
                writer.WriteOne(Red);
            else if (ToInt() == BrightRed)
                writer.WriteOne(Red);
            else
                writer.WriteOne(Black);
        }
        else if (writer.GetGameId() <= 9 && ToInt() >= Green) {
            if (ToInt() == Green)
                writer.WriteOne(Yellow);
            else if (ToInt() == BlackRed)
                writer.WriteOne(Red);
            else if (ToInt() == Grey)
                writer.WriteOne(White);
            else if (ToInt() == WhiteRed)
                writer.WriteOne(Red);
            else if (ToInt() == LightYellow)
                writer.WriteOne(Yellow);
            else if (ToInt() == Orange)
                writer.WriteOne(Yellow);
            else if (ToInt() == DarkBlue)
                writer.WriteOne(Blue);
            else if (ToInt() == Magenta)
                writer.WriteOne(Red);
            else if (ToInt() == BrightRed)
                writer.WriteOne(Red);
            else
                writer.WriteOne(Black);
        }
        else {
            if (ToInt() == YellowBlack && writer.IsVersionGreaterOrEqual(0x2012, 0x01) && !writer.IsVersionGreaterOrEqual(0x2013, 0x0B))
                writer.WriteOne(Black);
            else
                writer.WriteOne(ToInt());
        }
    }
ENUM_END(FifamShoeType)
