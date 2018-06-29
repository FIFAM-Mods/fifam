#pragma once
#include "TestsEnvironment.h"
#include "FifaDatabase.h"

class FifaDbEnvironment {
    FifaDatabase *mDatabase = nullptr;

public:
    FifaDbEnvironment() {
        mDatabase = new FifaDatabase(L"D:\\Projects\\fifam\\db\\fifa\\18\\25.09.2017");
    }

    ~FifaDbEnvironment() {
        delete mDatabase;
    }

    FifaDatabase *GetDatabase() { return mDatabase; }
};
