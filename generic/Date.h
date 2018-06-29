#pragma once
#include <string>

class Date {
public:
    unsigned char day;
    unsigned char month;
    unsigned short year;

    Date();
    Date(unsigned int Day, unsigned int Month, unsigned int Year);
    Date(Date const &rhs);
    void Set(unsigned int Day, unsigned int Month, unsigned int Year);
    bool IsEmpty();
    void MakeEmpty();
    std::wstring ToString();

    bool operator==(Date const &rhs);
    void operator=(Date const &rhs);
    bool operator!=(Date const &rhs);
    bool operator>(Date const &rhs);
    bool operator>=(Date const &rhs);
    bool operator<(Date const &rhs);
    bool operator<=(Date const &rhs);
};
