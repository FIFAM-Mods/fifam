#pragma once
#include "TestsEnvironment.h"
#include "FifamDatabase.h"
#include "Utils.h"

struct FM07 { static int id() { return 7; } };
struct FM08 { static int id() { return 8; } };
struct FM09 { static int id() { return 9; } };
struct FM10 { static int id() { return 10; } };
struct FM11 { static int id() { return 11; } };
struct FM12 { static int id() { return 12; } };
struct FM13 { static int id() { return 13; } };
struct FM14 { static int id() { return 14; } };

struct Default { static bool isDefault() { return true; } };
struct External { static bool isDefault() { return false; } };

template<typename Game, typename DbType>
class FifamDbEnvironment {
    FifamDatabase *mDatabase = nullptr;

public:
    FifamDbEnvironment() {
        if (!DbType::isDefault())
            mDatabase = new FifamDatabase(Game::id(), Utils::Format(L"D:\\Games\\dbs\\fm%02d\\database", Game::id()).c_str());
        else
            mDatabase = new FifamDatabase(Game::id(), Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", Game::id()).c_str());
    }

    ~FifamDbEnvironment() {
        delete mDatabase;
    }

    FifamDatabase *GetDatabase() { return mDatabase; }
};
