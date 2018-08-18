#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();
        db->Write(Game::id(), Game::year(), Game::vernum(), L"fm_test\\test_db");
    }
};
