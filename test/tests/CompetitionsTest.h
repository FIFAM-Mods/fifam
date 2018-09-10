#pragma once
#include "FifamDbEnvironment.h"
#include "FifamUtils.h"

template<typename Game, typename DbType>
class CompetitionsTest {
public:
    CompetitionsTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        FifamWriter writer(Utils::Format(L"comps_%02d.csv", Game::id()), Game::id(), Game::year(), Game::vernum(), Game::unicode());
        if (writer.Available()) {
            writer.WriteLine(L"DbType", L"ID", L"StrID", L"Name", L"Level", L"NumSubsAllowed");
            for (auto const &compEntry : db->mCompMap) {
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
