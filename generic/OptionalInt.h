#pragma once

class OptionalInt {
    int *pLinkedInt = nullptr;
    int ownedValue = 0;
public:
    OptionalInt();
    OptionalInt(int &link);
    OptionalInt(int *link);
    OptionalInt(OptionalInt const &rhs);
    explicit OptionalInt(int &&rvalue);
    void operator=(int value);
    int operator()() const;
    void reset(int &link);
    void reset(int *link);
    int *getLink();
};
