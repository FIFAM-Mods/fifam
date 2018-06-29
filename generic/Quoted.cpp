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
