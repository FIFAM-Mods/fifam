#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"
#include "Color.h"

ENUM_BEGIN(FifamClubTeamColor, UChar)
    ENUM_MEMBER( 0, Blue_Black,        L"Blue-black")
    ENUM_MEMBER( 1, Blue_Green,        L"Blue-green")
    ENUM_MEMBER( 2, Blue_Red,          L"Blue-red")
    ENUM_MEMBER( 3, Blue_White,        L"Blue-white")
    ENUM_MEMBER( 4, BlueYellow,        L"Blue-yellow")
    ENUM_MEMBER( 5, DarkRed_Black,     L"Dark red-black")
    ENUM_MEMBER( 6, DarkRed_LightBlue, L"Dark red-light blue")
    ENUM_MEMBER( 7, DarkRed_White,     L"Dark red-white")
    ENUM_MEMBER( 8, GreenBlack,        L"Green-black")
    ENUM_MEMBER( 9, Green_Red,         L"Green-red")
    ENUM_MEMBER(10, Green_White,       L"Green-white")
    ENUM_MEMBER(11, Green_Yellow,      L"Green-yellow")
    ENUM_MEMBER(12, LightBlue_Black,   L"Light blue-black")
    ENUM_MEMBER(13, LightBlue_White,   L"Light blue-white")
    ENUM_MEMBER(14, Orange_Black,      L"Orange-black")
    ENUM_MEMBER(15, Orange_White,      L"Orange-white")
    ENUM_MEMBER(16, Purple_Black,      L"Purple-black")
    ENUM_MEMBER(17, Purple_White,      L"Purple-white")
    ENUM_MEMBER(18, Red_Black,         L"Red-black")
    ENUM_MEMBER(19, Red_White,         L"Red-white")
    ENUM_MEMBER(20, Red_Yellow,        L"Red-yellow")
    ENUM_MEMBER(21, Yellow_Black,      L"Yellow-black")
    ENUM_MEMBER(22, Yellow_White,      L"Yellow-white")
    ENUM_MEMBER(23, White_Black,       L"White-black")
    ENUM_MEMBER(24, Brown_White,       L"Brown-white")
    ENUM_MEMBER(25, DarkBlue_White,    L"Dark blue-white")
    ENUM_MEMBER(26, Pink_Black,        L"Pink-black")
    ENUM_MEMBER(27, Orange_Red,        L"Orange-red")
    ENUM_DEFAULT_VALUE(Blue_Black)
    ENUM_DEFAULT_READ_WRITE

    String GetShortName() {
        static const WideChar *colorNames[] = {
            L"bl_sw",
            L"bl_gr",
            L"bl_ro",
            L"bl_ws",
            L"bl_ge",
            L"dro_sw",
            L"dro_hbl",
            L"dro_ws",
            L"gr_sw",
            L"gr_ro",
            L"gr_ws",
            L"gr_ge",
            L"hbl_sw",
            L"hbl_ws",
            L"or_sw",
            L"or_ws",
            L"li_sw",
            L"li_ws",
            L"ro_sw",
            L"ro_ws",
            L"ro_ge",
            L"ge_sw",
            L"ge_ws",
            L"sw_ws",
            L"br_ws",
            L"dbl_ws",
            L"pi_sw",
            L"or_ro"
        };
        return colorNames[ToInt()];
    }
ENUM_END(FifamClubTeamColor)
