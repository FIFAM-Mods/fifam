#pragma once

#include "FifamClub.h"
#include "Error.h"

class FifamCountry {
public:
    void Read(FifamReader &reader) {
        reader.ReadStartIndex(L"COUNTRY");
        while (reader.FindLine(L"%INDEX%STAFF", false)) {
            //reader.ReadLine<std::wstring>();
            FifamStaff staff;
            staff.Read(reader);
        }
    }

    void ReadFixtures(FifamReader &reader) {

    }

    void ReadScript(FifamReader &reader) {

    }
};
