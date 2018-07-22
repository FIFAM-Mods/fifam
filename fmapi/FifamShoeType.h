#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

// @since FM07
ENUM_BEGIN(FifamShoeType, UChar)
    ENUM_MEMBER( 1,  White,          L"White")
    ENUM_MEMBER( 2,  Black,          L"Black")
    ENUM_MEMBER( 3,  Blue,           L"Blue")
    ENUM_MEMBER( 4,  Red,            L"Red")
    ENUM_MEMBER( 5,  Yellow,         L"Yellow")
    ENUM_MEMBER( 6,  WhiteBlue,      L"White/Blue")
    ENUM_MEMBER( 7,  WhiteBlack,     L"White/Black")
    ENUM_MEMBER( 8,  WhiteLightBlue, L"White/Light Blue")
    ENUM_MEMBER( 9,  YellowBlack,    L"Yellow/Black")
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
            if (reader.GetGameId() <= 9 && id >= Green) {
                SetUnknown(id);
                SetDefaultValue();
            }
            else
                SetFromInt(id);
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (writer.GetGameId() <= 9 && ToInt() >= Green) {
            if (ToInt() == BlackRed)
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
        else
            writer.WriteOne(ToInt());
    }
ENUM_END(FifamShoeType)
