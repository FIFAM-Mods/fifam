#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamLanguage, UChar)
    ENUM_MEMBER(  0, None,         L"None")
    ENUM_MEMBER(  1, African,      L"African")
    ENUM_MEMBER(  2, Albanian,     L"Albanian")
    ENUM_MEMBER(  3, Amharic,      L"Amharic")
    ENUM_MEMBER(  4, Arabic,       L"Arabic")
    ENUM_MEMBER(  5, Armenian,     L"Armenian")
    ENUM_MEMBER(  6, Bosnian,      L"Bosnian")
    ENUM_MEMBER(  7, Bulgarian,    L"Bulgarian")
    ENUM_MEMBER(  8, Azerbaijani,  L"Azerbaijani") // since FM13
    ENUM_MEMBER(  9, Chinese,      L"Chinese")
    ENUM_MEMBER( 10, Croatian,     L"Croatian")
    ENUM_MEMBER( 11, Czech,        L"Czech")
    ENUM_MEMBER( 12, Danish,       L"Danish")
    ENUM_MEMBER( 13, Dutch,        L"Dutch")
    ENUM_MEMBER( 14, English,      L"English")
    ENUM_MEMBER( 15, Estonian,     L"Estonian")
    ENUM_MEMBER( 16, Finnish,      L"Finnish")
    ENUM_MEMBER( 17, French,       L"French")
    ENUM_MEMBER( 18, Georgian,     L"Georgian")
    ENUM_MEMBER( 19, German,       L"German")
    ENUM_MEMBER( 20, Greek,        L"Greek")
    ENUM_MEMBER( 21, Hebrew,       L"Hebrew")
    ENUM_MEMBER( 22, Hindi,        L"Hindi")
    ENUM_MEMBER( 23, Hungarian,    L"Hungarian")
    ENUM_MEMBER( 24, Icelandic,    L"Icelandic")
    ENUM_MEMBER( 25, Italian,      L"Italian")
    ENUM_MEMBER( 26, Japanese,     L"Japanese")
    ENUM_MEMBER( 27, Kiswahili,    L"Kiswahili")
    ENUM_MEMBER( 28, Korean,       L"Korean")
    ENUM_MEMBER( 29, Lettish,      L"Lettish")
    ENUM_MEMBER( 30, Lithuanian,   L"Lithuanian")
    ENUM_MEMBER( 31, Macedonian,   L"Macedonian")
    ENUM_MEMBER( 32, Malay,        L"Malay")
    ENUM_MEMBER( 33, Maltese,      L"Maltese")
    ENUM_MEMBER( 34, Norwegian,    L"Norwegian")
    ENUM_MEMBER( 35, Persian,      L"Persian")
    ENUM_MEMBER( 36, Polish,       L"Polish")
    ENUM_MEMBER( 37, Portuguese,   L"Portuguese")
    ENUM_MEMBER( 38, Romanian,     L"Romanian")
    ENUM_MEMBER( 39, Russian,      L"Russian")
    ENUM_MEMBER( 40, Serbian,      L"Serbian")
    ENUM_MEMBER( 41, Slovak,       L"Slovak")
    ENUM_MEMBER( 42, Slovenian,    L"Slovenian")
    ENUM_MEMBER( 43, Spanish,      L"Spanish")
    ENUM_MEMBER( 44, Thai,         L"Thai")
    ENUM_MEMBER( 45, Turkish,      L"Turkish")
    ENUM_MEMBER( 46, Ukrainian,    L"Ukrainian")
    ENUM_MEMBER( 47, Vietnamese,   L"Vietnamese")
    ENUM_MEMBER( 48, Swedish,      L"Swedish")
    ENUM_MEMBER( 49, Afghani,      L"Afghani")
    ENUM_MEMBER( 50, Bahasa,       L"Bahasa")
    ENUM_MEMBER( 51, Burmese,      L"Burmese")
    ENUM_MEMBER( 52, Kazakh,       L"Kazakh")
    ENUM_MEMBER( 53, Khmer,        L"Khmer")
    ENUM_MEMBER( 54, Laotian,      L"Laotian")
    ENUM_MEMBER( 55, Mongolian,    L"Mongolian")
    ENUM_MEMBER( 56, Nepali,       L"Nepali")
    ENUM_MEMBER( 57, Pilipino,     L"Pilipino")
    ENUM_MEMBER( 58, Polynesian,   L"Polynesian")
    ENUM_MEMBER( 59, Punjabi,      L"Punjabi")
    ENUM_MEMBER( 60, Sinhala,      L"Sinhala")
    ENUM_MEMBER( 61, Turkmenian,   L"Turkmenian")
    ENUM_MEMBER( 62, Uzbek,        L"Uzbek")
    ENUM_MEMBER( 63, Byelorussian, L"Byelorussian") // until FM13 (ID 8)
    ENUM_DEFAULT_VALUE(None)
    
    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 12) {
                if (id == 8)
                    id = Byelorussian;
                else if (id == 63) {
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
        if (writer.GetGameId() <= 12) {
            if (ToInt() == Azerbaijani)
                writer.WriteOne(Russian);
            else if(ToInt() == Byelorussian)
                writer.WriteOne(8);
            else
                writer.WriteOne(ToInt());
        }
        else {
            if (ToInt() == Byelorussian)
                writer.WriteOne(Russian);
            else
                writer.WriteOne(ToInt());
        }
    }
ENUM_END(FifamLanguage)
