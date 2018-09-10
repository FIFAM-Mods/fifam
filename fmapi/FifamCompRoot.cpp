#include "FifamCompRoot.h"

FifamCompDbType FifamCompRoot::GetDbType() const {
    return FifamCompDbType::Root;
}

void FifamCompRoot::Read(FifamReader &reader, FifamDatabase *database) {}
void FifamCompRoot::Write(FifamWriter &writer, FifamDatabase *database) {}
