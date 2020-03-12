#include "Quoted.h"

Quoted::Quoted() {
    pLinkedString = &ownedValue;
}

Quoted::Quoted(std::wstring &link) {
    pLinkedString = &link;
}

Quoted::Quoted(std::wstring *link) {
    pLinkedString = link;
}

Quoted::Quoted(Quoted const &rhs) {
    pLinkedString = rhs.pLinkedString;
}

Quoted::Quoted(std::wstring &&rvalue) {
    ownedValue = rvalue;
    pLinkedString = &ownedValue;
}

void Quoted::operator=(std::wstring const &value) {
    if (pLinkedString)
        *pLinkedString = value;
}

std::wstring Quoted::operator()() const {
    return pLinkedString ? *pLinkedString : std::wstring();
}

void Quoted::reset(std::wstring &link) {
    pLinkedString = &link;
}

void Quoted::reset(std::wstring *link) {
    pLinkedString = link;
}

std::wstring *Quoted::getLink() {
    return pLinkedString;
}

QuotedA::QuotedA() {
    pLinkedString = &ownedValue;
}

QuotedA::QuotedA(std::string &link) {
    pLinkedString = &link;
}

QuotedA::QuotedA(std::string *link) {
    pLinkedString = link;
}

QuotedA::QuotedA(QuotedA const &rhs) {
    pLinkedString = rhs.pLinkedString;
}

QuotedA::QuotedA(std::string &&rvalue) {
    ownedValue = rvalue;
    pLinkedString = &ownedValue;
}

void QuotedA::operator=(std::string const &value) {
    if (pLinkedString)
        *pLinkedString = value;
}

std::string QuotedA::operator()() const {
    return pLinkedString ? *pLinkedString : std::string();
}

void QuotedA::reset(std::string &link) {
    pLinkedString = &link;
}

void QuotedA::reset(std::string *link) {
    pLinkedString = link;
}

std::string *QuotedA::getLink() {
    return pLinkedString;
}
