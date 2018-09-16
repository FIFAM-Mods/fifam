#pragma once
#include "FifamCompetition.h"

class FifamCompRoot : public FifamCompetition {
public:
    FifamCompDbType GetDbType() const override;
    void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) override;
    void Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) override;
};
