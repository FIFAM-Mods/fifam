#pragma once
#include "FifamDbEnvironment.h"

class Fm07DbFix {
public:
    Fm07DbFix() {
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase db(7, "D:\\Games\\FIFA Manager 07\\database");
        auto p = db.PlayerFromID(45661);
        wcout << p->GetName() << std::endl;
        db.Write(7, FifamDatabase::GetGameDbVersion(7), "db_fm_07_fixed");
    }
};
