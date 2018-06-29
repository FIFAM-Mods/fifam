#include "FifaDate.h"
#include <sstream>
#include <iomanip>

FifaDate::FifaDate() : Date() {}

FifaDate::FifaDate(unsigned int DaysCount) { Set(DaysCount); }

unsigned int FifaDate::GetDays() {
    unsigned int a = (14 - month) / 12;
    unsigned int y = year + 4800 - a;
    unsigned int m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045 - 2299160;
}

void FifaDate::Set(unsigned int Days) {
    unsigned int a = Days + 32044 + 2299160;
    unsigned int b = (4 * a + 3) / 146097;
    unsigned int c = a - (b * 146097) / 4;
    unsigned int d = (4 * c + 3) / 1461;
    unsigned int e = c - (1461 * d) / 4;
    unsigned int m = (5 * e + 2) / 153;
    day = e - (153 * m + 2) / 5 + 1;
    month = m + 3 - 12 * (m / 10);
    year = b * 100 + d - 4800 + m / 10;
}
