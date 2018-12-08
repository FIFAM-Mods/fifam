#include "Hexadecimal.h"

Hexadecimal::Hexadecimal() {
    pLinkedInt = &ownedValue;
}

Hexadecimal::Hexadecimal(unsigned int &link) {
    pLinkedInt = &link;
}

Hexadecimal::Hexadecimal(unsigned int *link) {
    pLinkedInt = link;
}

Hexadecimal::Hexadecimal(Hexadecimal const &rhs) {
    pLinkedInt = rhs.pLinkedInt;
}

Hexadecimal::Hexadecimal(unsigned int &&rvalue) {
    ownedValue = rvalue;
    pLinkedInt = &ownedValue;
}

void Hexadecimal::operator=(unsigned int value) {
    if (pLinkedInt)
        *pLinkedInt = value;
}

unsigned int Hexadecimal::operator()() const {
    return pLinkedInt ? *pLinkedInt : 0;
}

void Hexadecimal::reset(unsigned int &link) {
    pLinkedInt = &link;
}

void Hexadecimal::reset(unsigned int *link) {
    pLinkedInt = link;
}

unsigned int *Hexadecimal::getLink() {
    return pLinkedInt;
}
