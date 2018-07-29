#pragma once
#include "FifamDbEnvironment.h"
#include "FifamClubBadge.h"

class BadgeTest {
public:
    void PrintBadgeInfo(String const &badgePath) {
        FifamClubBadge badge;
        badge.SetBadgePath(badgePath);
        std::wcout << badge.ToStr() << std::endl;
        std::wcout << L"    07:" << badge.GetBadgePath(7) << std::endl;
        std::wcout << L"    08:" << badge.GetBadgePath(8) << std::endl;
        std::wcout << L"    10:" << badge.GetBadgePath(10) << std::endl;
        std::wcout << L"    12:" << badge.GetBadgePath(12) << std::endl;
        std::wcout << std::endl;
    }

    BadgeTest() {
        PrintBadgeInfo(L"");
        PrintBadgeInfo(L"generic\\Badge%d\\generic256.tga");
        PrintBadgeInfo(L"Color\\Badge%d\\dro_hbl_18a.tga");
        PrintBadgeInfo(L"generic\\256x256\\Crest_14_04_or_sw.tga");
        PrintBadgeInfo(L"Color\\Badge%d\\ro_ws_02.tga");
    }
};
