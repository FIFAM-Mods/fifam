#pragma once

class Hexadecimal {
    unsigned int *pLinkedInt = nullptr;
    unsigned int ownedValue = 0;
public:
    Hexadecimal();
    Hexadecimal(unsigned int &link);
    Hexadecimal(unsigned int *link);
    Hexadecimal(Hexadecimal const &rhs);
    explicit Hexadecimal(unsigned int &&rvalue);
    void operator=(unsigned int value);
    unsigned int operator()() const;
    void reset(unsigned int &link);
    void reset(unsigned int *link);
    unsigned int *getLink();
};
