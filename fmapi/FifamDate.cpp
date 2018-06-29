#include "FifamDate.h"
#include <sstream>
#include <iomanip>

FifamDate::FifamDate() : Date() {}

FifamDate::FifamDate(unsigned int Days) {
    Set(Days);
}

FifamDate::FifamDate(unsigned int Day, unsigned int Month, unsigned int Year) : Date(Day, Month, Year) {}

void FifamDate::Set(unsigned int Days) {
    unsigned int j = Days - 1721119;
    year = (j * 4 - 1) / 146097;
    j = j * 4 - 146097 * year - 1;
    unsigned int x = j / 4;
    j = (x * 4 + 3) / 1461;
    year = 100 * year + j;
    x = (x * 4) + 3 - 1461 * j;
    x = (x + 4) / 4;
    month = (5 * x - 3) / 153;
    x = 5 * x - 3 - 153 * month;
    day = (x + 5) / 5;
    if (month < 10)
        month += 3;
    else {
        month -= 9;
        year++;
    }
}

unsigned int FifamDate::GetDays() {
    unsigned int d = day, m = month, y = year;
    if (m > 2)
        m -= 3;
    else {
        m += 9;
        y--;
    }
    unsigned int c = y / 100;
    unsigned int ya = y - 100 * c;
    return 1721119 + d + (146097 * c) / 4 + (1461 * ya) / 4 + (153 * m + 2) / 5;
}

void FifamDate::Validate() {

    unsigned char daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 0 || month > 12)
        month = 1;

    if (day == 0 || day > daysInMonth[month])
        day = 1;
    else if (month == 2 && day == 29 && !(year % 4)) {
        day = 1;
        month = 3;
    }

    if (year < 1700 || year > 3000)
        year = 0;
}
