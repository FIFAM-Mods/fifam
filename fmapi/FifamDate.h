#pragma once
#include <string>
#include "Date.h"

class FifamDate : public Date {
public:
    FifamDate();
    FifamDate(unsigned int Days);
    FifamDate::FifamDate(unsigned int Day, unsigned int Month, unsigned int Year);
    void Set(unsigned int Days);
    unsigned int GetDays();
    void Validate();
};
