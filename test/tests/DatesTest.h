#pragma once
#include "FifamDbEnvironment.h"
#include "FifamDate.h"

class DatesTest {
public:
    DatesTest() {
        FifamDate d1(1, 7, 2020);
        FifamDate d2;
        d2.SetDays(d1.GetDays() - 1);
        std::wcout << L"d1: " << d1.ToString() << L" d2: " << d2.ToString();
    }
};
