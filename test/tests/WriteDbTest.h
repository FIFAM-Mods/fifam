#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        db->Write(Game::id(), 0x2007, 0xC, L"fm_test\\test_db");
    }
};
