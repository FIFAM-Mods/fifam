#pragma once
#include "FifamDbEnvironment.h"
#include "FifamCompID.h"

class CompIDTest {
public:
    CompIDTest() {
        FifamCompID compID1 = { FifamCompRegion::Ukraine, FifamCompType::League, 1 };

        std::wcout << std::endl << L"Test1:" << std::endl;
        std::wcout << L"    " << compID1.ToStr() << L" " << compID1.ToHexStr() << L" " << compID1.ToInt();

        FifamCompID compID2 = { FifamCompRegion::International, L"WORLD_CUP", 0 };

        std::wcout << std::endl << L"Test2:" << std::endl;
        std::wcout << L"    " << compID2.ToStr() << L" " << compID2.ToHexStr() << L" " << compID2.ToInt();

        FifamCompID compID3 = 0x2D070000;

        std::wcout << std::endl << L"Test3:" << std::endl;
        std::wcout << L"    " << compID3.ToStr() << L" (" << compID3.mRegion.ToStr() << L") " << compID3.ToHexStr() << L" " << compID3.ToInt();

        std::wcout << std::endl;
    }
};
