#pragma once
#include "FifamDbEnvironment.h"

template<size_t Version, bool Original>
class FifamDbTest {
public:
    FifamDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Version, Original>>().GetDatabase();
        
        //db->Write(7, 0x2007, 0xC, "fm_test\\test_db");

        std::wofstream out("countryNames.txt");
        out << L"None, ";
        for (size_t i = 0; i < 207; i++) {
            size_t countryId = i + 1;
            out << Tr(db->mCountries[i]->mName);
            if (!((countryId + 1) % 10))
                out << std::endl;
            else
                out << L", ";
        }
    }
};
