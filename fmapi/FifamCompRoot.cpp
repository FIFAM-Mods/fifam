#include "FifamCompRoot.h"

FifamCompDbType FifamCompRoot::GetDbType() {
    return FifamCompDbType::Root;
}

void FifamCompRoot::Read(FifamReader &reader) {}
void FifamCompRoot::Write(FifamWriter &writer) {}
