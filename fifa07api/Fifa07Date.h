#pragma once
#include <string>
#include "Date.h"

class Fifa07Date : public Date {
public:
    Fifa07Date();
    Fifa07Date(unsigned int Days);
    unsigned int GetDays();
    void Set(unsigned int Day);
};
