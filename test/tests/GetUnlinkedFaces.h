#pragma once
#include "FifamDbEnvironment.h"
#include "FifaDbEnvironment.h"
#include "FifamPlayerLevel.h"

class GetUnlinkedFaces {
public:
    GetUnlinkedFaces() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        Set<UInt> usedFaces;
        Map<UInt, Vector<String>> duplicatedFaces;
        for (auto player : db->mPlayers) {
            if (player->mSpecialFace != 0) {
                if (usedFaces.contains(player->mSpecialFace))
                    duplicatedFaces[player->mSpecialFace].push_back(player->GetName());
                else
                    usedFaces.insert(player->mSpecialFace);
            }
        }
        FifamWriter unlinkedFacesWriter(Utils::Format(L"unlinked_3d_faces.csv", FM13::id()), FM13::id(), FifamVersion(FM13::year(), FM13::vernum()), FM13::unicode());
        if (unlinkedFacesWriter.Available()) {
            Path assetsPath = "D:\\Games\\FIFA Manager 13\\data\\assets";
            if (exists(assetsPath)) {
                for (auto const &i : directory_iterator(assetsPath)) {
                    if (i.is_regular_file()) {
                        auto const &p = i.path();
                        if (p.extension() == ".o") {
                            auto filename = p.stem().string();
                            if (filename.starts_with("m228__")) {
                                auto playerId = Utils::SafeConvertInt<Int>(filename.substr(6));
                                if (playerId > 0 && !usedFaces.contains(playerId)) {
                                    FifaPlayer *palyer = fifadb->GetPlayer(playerId);
                                    if (palyer)
                                        unlinkedFacesWriter.WriteLine(playerId, palyer->m_quickName);
                                    else
                                        unlinkedFacesWriter.WriteLine(playerId, L"");
                                }
                            }
                        }
                    }
                }
            }
        }
        FifamWriter duplicatedFacesWriter(Utils::Format(L"duplicated_3d_faces.csv", FM13::id()), FM13::id(), FifamVersion(FM13::year(), FM13::vernum()), FM13::unicode());
        if (duplicatedFacesWriter.Available()) {
            for (auto const &i : duplicatedFaces) {
                for (auto const &j : i.second)
                    duplicatedFacesWriter.WriteLine(i.first, j);
            }
        }
    }
};
