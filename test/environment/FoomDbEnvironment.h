#pragma once
#include "TestsEnvironment.h"
#include "foom_db.h"

class FoomDbEnvironment {
    foom::db *mDatabase = nullptr;

public:
    FoomDbEnvironment() {
        mDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom", foom::db::db_gender::Men);
    }

    ~FoomDbEnvironment() {
        delete mDatabase;
    }

    foom::db *GetDatabase() { return mDatabase; }
};
