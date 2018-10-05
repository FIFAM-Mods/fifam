#pragma once
#include "FoomDbEnvironment.h"
#include <iostream>

class FoomBasicTest {
public:
    FoomBasicTest() {
        auto foomdb = GetEnvironment<FoomDbEnvironment>().GetDatabase();
        int counter = 1;
        for (auto &entry : foomdb->mNations) {
            auto &nation = entry.second;
            std::wcout << counter++ << L". " << nation.mName << std::endl;
        }
        for (auto &entry : foomdb->mComps) {
            auto &comp = entry.second;
            std::wcout << counter++ << L". " << comp.mName << std::endl;
            int teamCounter = 1;
            for (auto team : comp.mVecTeams)
                std::wcout << L"  " << teamCounter++ << L". " << team->mName << std::endl;
        }
    }
};
