#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"

class UpdateBudgets {
public:
    UpdateBudgets() {
        UInt const gameVersion = 13;
        Path dbPathIn = L"E:\\Games\\FIFA Manager 13\\database";
        Path dbPathOut = L"E:\\Games\\FIFA Manager 13\\database_budgets";
    }
};

REGISTER_TEST(UpdateBudgets)
