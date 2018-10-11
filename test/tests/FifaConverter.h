#pragma once
#include "FifaDbEnvironment.h"
#include "FifamDbEnvironment.h"
#include <iostream>

class FifaConverter {
    FifaConverter() {
        // read FIFA database
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        // read FIFAM database (skip players and staff)
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM14, External>>().GetDatabase();
        // show FIFA version
        std::wcout << L"FIFA" << fifadb->m_lastSupportedGameVersion << L" DB Version " << fifadb->GetVersionDateString() << std::endl;
        
        
    }
};
