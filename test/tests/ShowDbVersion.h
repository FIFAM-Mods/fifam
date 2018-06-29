#pragma once

#include "FifaDbEnvironment.h"

class ShowDbVersion {
public:
    ShowDbVersion() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        std::wcout << db->GetVersionString() << L" " << db->GetVersionDateString();
    }
};
