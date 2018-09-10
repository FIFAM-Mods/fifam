#pragma once
#include "FifamDbEnvironment.h"

template<typename From, typename To, bool WriteToGameFolder = true>
class WriteDbTest {
public:
    WriteDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<From, External>>().GetDatabase();
        db->Write(To::id(), To::year(), To::vernum(),
            Utils::Format(WriteToGameFolder ?
                L"D:\\Games\\FIFA Manager %02d\\database":
                L"fm_test\\database",
                To::id()).c_str());
    }
};
