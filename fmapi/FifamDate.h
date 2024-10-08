#pragma once
#include "Date.h"
#include "FifamTypes.h"

class FifamDate : public Date {
public:
    FifamDate();
    FifamDate(UInt Days);
    FifamDate(UInt Day, UInt Month, UInt Year);
    FifamDate(Date const &rhs);
    void SetDays(UInt Days);
    UInt GetDays() const;
    void Validate();
    FifamDate IncreasedByYears(UInt numYears);
    FifamDate DecreasedByYears(UInt numYears);
    FifamDate IncreasedByOneYear();
    FifamDate DecreasedByOneYear();
    Bool operator==(FifamDate const &rhs) const;
    void operator=(FifamDate const &rhs);
    void operator=(Date const &rhs);
    Bool operator!=(FifamDate const &rhs) const;
    Bool operator>(FifamDate const &rhs) const;
    Bool operator>=(FifamDate const &rhs) const;
    Bool operator<(FifamDate const &rhs) const;
    Bool operator<=(FifamDate const &rhs) const;
};

Int operator-(FifamDate const &lhs, FifamDate const &rhs);
