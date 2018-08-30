#pragma once
#include "FifamDbEnvironment.h"

template<typename From, typename To>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<From, External>>().GetDatabase();
        db->Write(To::id(), To::year(), To::vernum(),
            Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", To::id()).c_str());
    }
};
