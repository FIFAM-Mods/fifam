#pragma once
#include <string>
#include "Date.h"

class FifaDate : public Date {
public:
    FifaDate();
    FifaDate(unsigned int Days);
    unsigned int GetDays();
    void Set(unsigned int Day);
};
