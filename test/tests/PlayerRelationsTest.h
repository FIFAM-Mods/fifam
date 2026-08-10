#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"

class PlayerRelationsTest {
public:
    PlayerRelationsTest() {
        FifamDatabase db(13, "db");
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

REGISTER_TEST(PlayerRelationsTest)
