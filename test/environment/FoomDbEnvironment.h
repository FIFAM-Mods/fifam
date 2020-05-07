#pragma once
#include "TestsEnvironment.h"
#include "foom_db.h"

class FoomDbEnvironment {
    foom::db *mDatabase = nullptr;

public:
    FoomDbEnvironment() {
        mDatabase = new foom::db(L"E:\\Projects\\fifam\\db\\foom");
    }

    ~FoomDbEnvironment() {
        delete mDatabase;
    }

    foom::db *GetDatabase() { return mDatabase; }
};
