#pragma once
#include "Date.h"
#include "FifamTypes.h"

class FifamDate : public Date {
public:
    FifamDate();
    FifamDate(UInt Days);
    FifamDate::FifamDate(UInt Day, UInt Month, UInt Year);
    void SetDays(UInt Days);
    UInt GetDays() const;
    void Validate();

    Bool operator==(FifamDate const &rhs) const;
    void operator=(FifamDate const &rhs);
    Bool operator!=(FifamDate const &rhs) const;
    Bool operator>(FifamDate const &rhs) const;
    Bool operator>=(FifamDate const &rhs) const;
    Bool operator<(FifamDate const &rhs) const;
    Bool operator<=(FifamDate const &rhs) const;
};

Int operator-(FifamDate const &lhs, FifamDate const &rhs);
