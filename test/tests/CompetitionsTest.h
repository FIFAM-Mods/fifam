#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"
#include "FifamUtils.h"

class CompetitionsTest {
public:
    CompetitionsTest() {
        FifamDatabase db(13, "db");
        FifamWriter writer(L"comps.csv");
        if (writer.Available()) {
            writer.WriteLine(L"DbType", L"ID", L"StrID", L"Name", L"Level", L"NumSubsAllowed");
            for (auto const &compEntry : db.mCompMap) {
                auto const &comp = compEntry.second;
                writer.WriteLine(
                    comp->GetDbType().ToStr(),
                    comp->mID.ToHexStr(),
                    Quoted(comp->mID.ToStr(true)),
                    Quoted(comp->GetName()),
                    comp->mCompetitionLevel,
                    comp->mNumSubsAllowed
                );
            }
        }
    }
};

REGISTER_TEST(CompetitionsTest)
