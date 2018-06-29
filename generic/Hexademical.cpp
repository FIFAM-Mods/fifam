#include "Hexademical.h"

Hexademical::Hexademical() {
    pLinkedInt = &ownedValue;
}

Hexademical::Hexademical(unsigned int &link) {
    pLinkedInt = &link;
}

Hexademical::Hexademical(unsigned int *link) {
    pLinkedInt = link;
}

Hexademical::Hexademical(Hexademical const &rhs) {
    pLinkedInt = rhs.pLinkedInt;
}

Hexademical::Hexademical(unsigned int &&rvalue) {
    ownedValue = rvalue;
    pLinkedInt = &ownedValue;
}

void Hexademical::operator=(unsigned int value) {
    if (pLinkedInt)
        *pLinkedInt = value;
}

unsigned int Hexademical::operator()() const {
    return pLinkedInt ? *pLinkedInt : 0;
}

void Hexademical::reset(unsigned int &link) {
    pLinkedInt = &link;
}

void Hexademical::reset(unsigned int *link) {
    pLinkedInt = link;
}

unsigned int *Hexademical::getLink() {
    return pLinkedInt;
}
