#pragma once
#include "FifamDbEnvironment.h"
#include "Color.h"

class ColorsTest {
public:
    ColorsTest() {
        Color red = { 255, 192, 203 }; // pink

        FifamClubInterfaceColor07 color07;
        color07.SetFromInt(red.FindIndexInTable(FifamClub::m07InterfaceColorsTable));

        FifamClubInterfaceColor08 color08;
        color08.SetFromInt(red.FindIndexInTable(FifamClub::m08InterfaceColorsTable));

        std::wcout << L"07 color: " << color07.ToStr() << std::endl;
        std::wcout << L"08 color: " << color08.ToStr() << std::endl;

        std::wcout << std::endl;

        ColorPair clrPair1 = {{ 0, 0, 0 }, { 255, 255, 255 }}; // black-white
        
        FifamClubTeamColor teamColor1;
        teamColor1.SetFromInt(clrPair1.FindIndexInTable(FifamClub::mTeamColorsTable));

        std::wcout << L"teamColor1: " << teamColor1.ToStr() << std::endl;

        ColorPair clrPair2 = {{ 0, 0, 255 }, { 0, 255, 0 }}; // blue-green

        FifamClubTeamColor teamColor2;
        teamColor2.SetFromInt(clrPair2.FindIndexInTable(FifamClub::mTeamColorsTable));

        std::wcout << L"teamColor2: " << teamColor2.ToStr() << std::endl;

        std::wcout << L"Distance between red and pink: " << Color::Distance(Color(255, 0, 0), Color(255, 192, 203)) << std::endl;
    }
};
