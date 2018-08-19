#include "Date.h"
#include "Utils.h"

Date::Date() { MakeEmpty(); }
Date::Date(unsigned int Day, unsigned int Month, unsigned int Year) { Set(Day, Month, Year); }
Date::Date(Date const &rhs) { *this = rhs; }

void Date::Set(unsigned int Day, unsigned int Month, unsigned int Year) {
    day = Day;
    month = Month;
    year = Year;
}

bool Date::IsEmpty() {
    return year == 0;
}

void Date::MakeEmpty() {
    Set(0, 0, 0);
}

std::wstring Date::ToString() {
    if (year == 0)
        return L"00.00.0000";
    return Utils::Format(L"%02d.%02d.%04d", day, month, year);
}

bool Date::operator==(Date const &rhs) {
    return year == rhs.year && month == rhs.month && day == rhs.day;
}

void Date::operator=(Date const &rhs) {
    this->year = rhs.year;
    this->month = rhs.month;
    this->day = rhs.day;
}

bool Date::operator!=(Date const &rhs) {
    return year != rhs.year || month != rhs.month || day != rhs.day;
}

bool Date::operator>(Date const &rhs) {
    if (year > rhs.year)
        return true;
    else if (year == rhs.year) {
        if (month > rhs.month)
            return true;
        else if (month == rhs.month) {
            if (day > rhs.day) {
                return true;
            }
        }
    }
    return false;
}

bool Date::operator>=(Date const &rhs) {
    if (year > rhs.year)
        return true;
    else if (year == rhs.year) {
        if (month > rhs.month)
            return true;
        else if (month == rhs.month) {
            if (day >= rhs.day) {
                return true;
            }
        }
    }
    return false;
}

bool Date::operator<(Date const &rhs) {
    if (year < rhs.year)
        return true;
    else if (year == rhs.year) {
        if (month < rhs.month)
            return true;
        else if (month == rhs.month) {
            if (day < rhs.day) {
                return true;
            }
        }
    }
    return false;
}

bool Date::operator<=(Date const &rhs) {
    if (year < rhs.year)
        return true;
    else if (year == rhs.year) {
        if (month < rhs.month)
            return true;
        else if (month == rhs.month) {
            if (day <= rhs.day) {
                return true;
            }
        }
    }
    return false;
}
