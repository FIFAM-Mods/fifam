#pragma once
#include "FifamCompetition.h"

class FifamCompRoot : public FifamCompetition {
public:
    FifamCompDbType GetDbType() override;
    void Read(FifamReader &reader) override;
    void Write(FifamWriter &writer) override;
};
