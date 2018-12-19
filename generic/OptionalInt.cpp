#include "OptionalInt.h"

OptionalInt::OptionalInt() {
    pLinkedInt = &ownedValue;
}

OptionalInt::OptionalInt(int &link) {
    pLinkedInt = &link;
}

OptionalInt::OptionalInt(int *link) {
    pLinkedInt = link;
}

OptionalInt::OptionalInt(OptionalInt const &rhs) {
    pLinkedInt = rhs.pLinkedInt;
}

OptionalInt::OptionalInt(int &&rvalue) {
    ownedValue = rvalue;
    pLinkedInt = &ownedValue;
}

void OptionalInt::operator=(int value) {
    if (pLinkedInt)
        *pLinkedInt = value;
}

int OptionalInt::operator()() const {
    return pLinkedInt ? *pLinkedInt : 0;
}

void OptionalInt::reset(int &link) {
    pLinkedInt = &link;
}

void OptionalInt::reset(int *link) {
    pLinkedInt = link;
}

int *OptionalInt::getLink() {
    return pLinkedInt;
}
