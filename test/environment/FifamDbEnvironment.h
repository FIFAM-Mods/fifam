#pragma once
#include "TestsEnvironment.h"
#include "FifamDatabase.h"
#include "Utils.h"

struct FM07 { static int id() { return 7; } static int year() { return 0x2007; } static int vernum() { return 0x0C; } static bool unicode() { return false; } };
struct FM08 { static int id() { return 8; } static int year() { return 0x2007; } static int vernum() { return 0x1E; } static bool unicode() { return true; } };
struct FM09 { static int id() { return 9; } static int year() { return 0x2009; } static int vernum() { return 0x05; } static bool unicode() { return true; } };
struct FM10 { static int id() { return 10; } static int year() { return 0x2009; } static int vernum() { return 0x0A; } static bool unicode() { return true; } };
struct FM11 { static int id() { return 11; } static int year() { return 0x2011; } static int vernum() { return 0x0A; } static bool unicode() { return true; } };
struct FM12 { static int id() { return 12; } static int year() { return 0x2012; } static int vernum() { return 0x04; } static bool unicode() { return true; } };
struct FM13 { static int id() { return 13; } static int year() { return 0x2013; } static int vernum() { return 0x0A; } static bool unicode() { return true; } };
struct FM14 { static int id() { return 14; } static int year() { return 0x2013; } static int vernum() { return 0x0A; } static bool unicode() { return true; } };

struct Default { static bool isDefault() { return true; } };
struct External { static bool isDefault() { return false; } };
using GameFolder = Default;

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
