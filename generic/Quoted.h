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

class QuotedA {
    std::string *pLinkedString = nullptr;
    std::string ownedValue;
public:
    QuotedA();
    QuotedA(std::string &link);
    QuotedA(std::string *link);
    QuotedA(QuotedA const &rhs);
    explicit QuotedA(std::string &&rvalue);
    void operator=(std::string const &value);
    std::string operator()() const;
    void reset(std::string &link);
    void reset(std::string *link);
    std::string *getLink();
};
