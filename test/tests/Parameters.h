#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

Pair<UInt, WideChar const *> paramFiles[] = {
{ 0x66868C, L"%s\\fmdata\\Parameter File - Staff Generation.enc" },
{ 0x66AF0E, L"fmdata\\Parameter File - Player Level.enc" },
{ 0x66AF7E, L"fmdata\\Parameter File - Player Styles.enc" },
{ 0x67E948, L"Parameter File - Club Jobs.enc" },
{ 0x67E988, L"Parameter File - Club Positions.enc" },
{ 0x683508, L"Parameter File - Training Camp New.txt" }
};

class ParametersTest {
public:
    ParametersTest() {
        FifamWriter writer(L"parameters.txt", 14, FifamVersion());
        writer.SetReplaceQuotes(false);
        for (auto const &p : paramFiles) {
            String newPath = p.second;
            auto pos = newPath.find(L"Parameter File - ");
            if (pos == String::npos) {
                ::Error(L"Wrong parameter file path: %s", p.second);
                continue;
            }
            Utils::Replace(newPath, L"Parameter File - ", L"ParameterFiles\\");
            Utils::Replace(newPath, L".enc", L".txt");
            Utils::Replace(newPath, L"\\", L"\\\\");
            writer.WriteLine(Utils::Format(L"CopyStrMemEditor(0x%X, L\"%s\");", p.first - pos * 2, newPath.c_str()));
        }
    }
};
