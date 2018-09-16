#include "FifamCompRoot.h"

FifamCompDbType FifamCompRoot::GetDbType() const {
    return FifamCompDbType::Root;
}

void FifamCompRoot::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {}
void FifamCompRoot::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {}
