#pragma once
#include <string>

class Quoted {
    std::wstring *pLinkedString = nullptr;
    std::wstring ownedValue;
public:
    Quoted();
    Quoted(std::wstring &link);
    Quoted(std::wstring *link);
    Quoted(Quoted const &rhs);
    explicit Quoted(std::wstring &&rvalue);
    void operator=(std::wstring const &value);
    std::wstring operator()() const;
    void reset(std::wstring &link);
    void reset(std::wstring *link);
    std::wstring *getLink();
};
