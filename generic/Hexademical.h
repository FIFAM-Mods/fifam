#pragma once

class Hexademical {
    unsigned int *pLinkedInt = nullptr;
    unsigned int ownedValue = 0;
public:
    Hexademical();
    Hexademical(unsigned int &link);
    Hexademical(unsigned int *link);
    Hexademical(Hexademical const &rhs);
    explicit Hexademical(unsigned int &&rvalue);
    void operator=(unsigned int value);
    unsigned int operator()() const;
    void reset(unsigned int &link);
    void reset(unsigned int *link);
    unsigned int *getLink();
};
