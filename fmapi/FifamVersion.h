#pragma once
#include "FifamTypes.h"

class FifamVersion {
    union {
        unsigned int mIntValue;
        struct {
            unsigned int mNumber : 16;
            unsigned int mYear : 16;
        };
    };
public:
    FifamVersion();
    void Set(unsigned short year, unsigned short number);
    void Set(unsigned int intValue);
    unsigned short GetYear();
    unsigned short GetNumber();
    unsigned int GetIntValue();
    bool IsGreaterOrEqual(unsigned short year, unsigned short number);
    String GetString();
};
