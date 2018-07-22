#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"
#include "Color.h"

ENUM_BEGIN(FifamKitColor, UChar)
    ENUM_MEMBER( 0, Red1,    L"Red1")
    ENUM_MEMBER( 1, Green1,  L"Green1")
    ENUM_MEMBER( 2, Blue1,   L"Blue1")
    ENUM_MEMBER( 3, Yellow1, L"Yellow1")
    ENUM_MEMBER( 4, Brown1,  L"Brown1")
    ENUM_MEMBER( 5, Sky1,    L"Sky1")
    ENUM_MEMBER( 6, Violet1, L"Violet1")
    ENUM_MEMBER( 7, White,   L"White")

    ENUM_MEMBER( 8, Red2,    L"Red2")
    ENUM_MEMBER( 9, Green2,  L"Green2")
    ENUM_MEMBER(10, Blue2,   L"Blue2")
    ENUM_MEMBER(11, Yellow2, L"Yellow2")
    ENUM_MEMBER(12, Brown2,  L"Brown2")
    ENUM_MEMBER(13, Sky2,    L"Sky2")
    ENUM_MEMBER(14, Violet2, L"Violet2")
    ENUM_MEMBER(15, Grey1,   L"Grey1")

    ENUM_MEMBER(16, Red3,    L"Red3")
    ENUM_MEMBER(17, Green3,  L"Green3")
    ENUM_MEMBER(18, Blue3,   L"Blue3")
    ENUM_MEMBER(19, Yellow3, L"Yellow3")
    ENUM_MEMBER(20, Brown3,  L"Brown3")
    ENUM_MEMBER(21, Sky3,    L"Sky3")
    ENUM_MEMBER(22, Violet3, L"Violet3")
    ENUM_MEMBER(23, Grey2,   L"Grey2")

    ENUM_MEMBER(24, Red4,    L"Red4")
    ENUM_MEMBER(25, Green4,  L"Green4")
    ENUM_MEMBER(26, Blue4,   L"Blue4")
    ENUM_MEMBER(27, Yellow4, L"Yellow4")
    ENUM_MEMBER(28, Brown4,  L"Brown4")
    ENUM_MEMBER(29, Field1,  L"Field1")
    ENUM_MEMBER(30, Violet4, L"Violet4")
    ENUM_MEMBER(31, Grey3,   L"Grey3")

    ENUM_MEMBER(32, Red5,    L"Red5")
    ENUM_MEMBER(33, Green5,  L"Green5")
    ENUM_MEMBER(34, Blue5,   L"Blue5")
    ENUM_MEMBER(35, Yellow5, L"Yellow5")
    ENUM_MEMBER(36, Brown5,  L"Brown5")
    ENUM_MEMBER(37, Field2,  L"Field2")
    ENUM_MEMBER(38, Violet5, L"Violet5")
    ENUM_MEMBER(39, Grey4,   L"Grey4")

    ENUM_MEMBER(40, Red6,    L"Red6")
    ENUM_MEMBER(41, Green6,  L"Green6")
    ENUM_MEMBER(42, Blue6,   L"Blue6")
    ENUM_MEMBER(43, Yellow6, L"Yellow6")
    ENUM_MEMBER(44, Brown6,  L"Brown6")
    ENUM_MEMBER(45, Field3,  L"Field3")
    ENUM_MEMBER(46, Violet6, L"Violet6")
    ENUM_MEMBER(47, Grey5,   L"Grey5")

    ENUM_MEMBER(48, Red7,    L"Red7")
    ENUM_MEMBER(49, Green7,  L"Green7")
    ENUM_MEMBER(50, Blue7,   L"Blue7")
    ENUM_MEMBER(51, Yellow7, L"Yellow7")
    ENUM_MEMBER(52, Brown7,  L"Brown7")
    ENUM_MEMBER(53, Gold1,   L"Gold1")
    ENUM_MEMBER(54, Violet7, L"Violet7")
    ENUM_MEMBER(55, Grey6,   L"Grey6")

    ENUM_MEMBER(56, Red8,    L"Red8")
    ENUM_MEMBER(57, Green8,  L"Green8")
    ENUM_MEMBER(58, Blue8,   L"Blue8")
    ENUM_MEMBER(59, Yellow8, L"Yellow8")
    ENUM_MEMBER(60, Brown8,  L"Brown8")
    ENUM_MEMBER(61, Gold2,    L"Gold2")
    ENUM_MEMBER(62, Violet8, L"Violet8")
    ENUM_MEMBER(63, Black,   L"Black")

    ENUM_DEFAULT_VALUE(White)
    ENUM_DEFAULT_READ_WRITE

    ColorRGB GetRGB() {
        static ColorRGB table[] = {
            { 255, 85, 85 },
            { 14, 177, 2 },
            { 1, 186, 220 },
            { 255, 240, 0 },
            { 172, 121, 40 },
            { 152, 206, 216 },
            { 220, 1, 145 },
            { 255, 255, 255 },
            { 255, 0, 0 },
            { 11, 146, 1 },
            { 1, 157, 185 },
            { 255, 210, 0 },
            { 137, 96, 32 },
            { 119, 157, 164 },
            { 220, 1, 181 },
            { 230, 227, 227 },
            { 227, 4, 4 },
            { 8, 116, 0 },
            { 0, 117, 139 },
            { 255, 192, 0 },
            { 98, 66, 15 },
            { 93, 123, 128 },
            { 220, 1, 212 },
            { 201, 201, 201 },
            { 201, 0, 0 },
            { 2, 177, 101 },
            { 0, 156, 255 },
            { 255, 162, 0 },
            { 185, 109, 53 },
            { 144, 202, 136 },
            { 202, 1, 220 },
            { 170, 167, 167 },
            { 176, 2, 2 },
            { 0, 96, 43 },
            { 0, 96, 255 },
            { 255, 138, 0 },
            { 153, 91, 46 },
            { 103, 142, 98 },
            { 176, 1, 220 },
            { 128, 128, 128 },
            { 149, 0, 0 },
            { 121, 148, 13 },
            { 2, 35, 204 },
            { 255, 114, 0 },
            { 119, 68, 30 },
            { 72, 100, 68 },
            { 150, 1, 220 },
            { 95, 95, 95 },
            { 121, 0, 0 },
            { 81, 101, 0 },
            { 66, 0, 255 },
            { 255, 84, 0 },
            { 172, 139, 40 },
            { 255, 236, 148 },
            { 130, 1, 220 },
            { 56, 56, 56 },
            { 92, 0, 0 },
            { 46, 61, 0 },
            { 50, 5, 180 },
            { 255, 36, 0 },
            { 123, 99, 27 },
            { 201, 186, 115 },
            { 92, 0, 156 },
            { 0, 0, 0 }
        };
        return table[ToInt()];
    }
ENUM_END(FifamKitColor)
