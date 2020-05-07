#pragma once
#include "TestsEnvironment.h"
#include "FifaDatabase.h"

class FifaDbEnvironment {
    FifaDatabase *mDatabase = nullptr;

public:
    FifaDbEnvironment() {
        mDatabase = new FifaDatabase(L"E:\\Projects\\fifam\\db\\fifa");
    }

    ~FifaDbEnvironment() {
        delete mDatabase;
    }

    FifaDatabase *GetDatabase() { return mDatabase; }
};
