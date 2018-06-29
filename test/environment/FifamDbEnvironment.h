#pragma once
#include "TestsEnvironment.h"
#include "FifamDatabase.h"
#include "Utils.h"

template<size_t Version, bool Original>
class FifamDbEnvironment {
    FifamDatabase *mDatabase = nullptr;

public:
    FifamDbEnvironment() {
        if (Original)
            mDatabase = new FifamDatabase(Version, Utils::Format(L"D:\\Games\\dbs\\fm%02d\\database", Version).c_str());
        else
            mDatabase = new FifamDatabase(Version, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", Version).c_str());
    }

    ~FifamDbEnvironment() {
        delete mDatabase;
    }

    FifamDatabase *GetDatabase() { return mDatabase; }
};
