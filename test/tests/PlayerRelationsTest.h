#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class PlayerRelationsTest {
public:
    PlayerRelationsTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();
        // TODO: remake this
        //auto playerIt = db->mPersonsMap.find(60832);
        //if (playerIt != db->mPersonsMap.end()) {
        //    FifamPlayer *player = (*playerIt).second->AsPlayer();
        //    std::wcout << player->GetName() << " brothers: ";
        //    if (!player->mBrothers.empty()) {
        //        for (auto p : player->mBrothers)
        //            std::wcout << p->GetName() << " ";
        //    }
        //    else
        //        std::wcout << "- ";
        //    std::wcout << ", cousins: ";
        //    if (!player->mCousins.empty()) {
        //        for (auto p : player->mCousins)
        //            std::wcout << p->GetName() << " ";
        //    }
        //    else
        //        std::wcout << "- ";
        //
        //}
    }
};
