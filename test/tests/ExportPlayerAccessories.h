#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

Bool SkipEmpty = true;

class ExportPlayerAccessories {
public:
    static void WritePlayerAccessories(FifaPlayer *player, FifamWriter &writer) {
        UChar jerseytype = (player->internal.jerseystylecode == 1) ? 0 : 1;
        UChar sleeves = (player->internal.jerseysleevelengthcode == 0) ? 0 : 1;
        UChar socks = 0;
        if (player->internal.socklengthcode == 1 /* || player->internal.socklengthcode == 2*/)
            socks = player->internal.socklengthcode;

        Array<Pair<UChar, UChar>, 4> accessories = {};

        auto GetAccessory = [&](Int type) -> Pair<UChar, UChar> * {
            for (UInt i = 0; i < 4; i++) {
                if (accessories[i].first == type)
                    return &accessories[i];
            }
            return nullptr;
        };

        auto ConvertColor = [](Int color) {
            switch (color) {
            case 0:
                return 1;
            case 1:
            case 5:
                return 2;
            case 2:
            case 11:
            case 12:
                return 3;
            case 3:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                return 4;
            case 4:
                return 5;
            }
            return 2;
        };

        auto ConvertType = [](Int type) {
            switch (type) {
            case 6:
            case 8:
            case 24:
                return 7;
            case 7:
            case 9:
            case 25:
                return 8;
                //case 16:
                //    return 4;
            case 22:
                return 9;
            case 23:
                return 10;
            case 26:
                return 13;
            case 27:
                return 14;
            }
            return 0;
        };

        auto AddAccessory = [&](Int type, Int color) {
            UChar fifamType = ConvertType(type);
            if (fifamType != 0 && !GetAccessory(fifamType)) {
                auto newSlot = GetAccessory(0);
                if (newSlot) {
                    newSlot->first = fifamType;
                    newSlot->second = ConvertColor(color);
                }
            }
        };

        AddAccessory(player->internal.accessorycode1, player->internal.accessorycolourcode1);
        AddAccessory(player->internal.accessorycode2, player->internal.accessorycolourcode2);
        AddAccessory(player->internal.accessorycode3, player->internal.accessorycolourcode3);
        AddAccessory(player->internal.accessorycode4, player->internal.accessorycolourcode4);

        if (!SkipEmpty || jerseytype != 0 || sleeves != 0 || socks != 0 || accessories[0].first != 0) {
            writer.WriteLine(player->GetId(), Quoted(player->m_quickName), jerseytype, sleeves, socks,
                accessories[0].first, accessories[0].second, accessories[1].first, accessories[1].second,
                accessories[2].first, accessories[2].second, accessories[3].first, accessories[3].second
            );
        }
    }

    ExportPlayerAccessories() {
        std::wcout << L"Reading FIFA database..." << std::endl;
        FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 13\\database");
        Map<UInt, Vector<FifamPlayer *>> fifaIds;
        for (FifamPlayer *p : db.mPlayers) {
            if (p->mFifaID != 0)
                fifaIds[p->mFifaID].push_back(p);
        }
        FifamWriter writer(L"D:\\Games\\FIFA Manager 13\\plugins\\ucp\\player_accessories.csv", 14, FifamVersion());
        writer.WriteLine(L"playerfifaid,comment,jerseystyle,sleeves,socks,acc1,acccolor1,acc2,acccolor2,acc3,acccolor3,acc4,acccolor4");
        if (writer.Available()) {
            fifadb->ForAllPlayers([&](FifaPlayer &p) {
                if (Utils::Contains(fifaIds, p.GetId()))
                    WritePlayerAccessories(&p, writer);
            });
            writer.Close();
        }
        else
            ::Error(L"Unable to open output file");

        FifamWriter writer2(L"D:\\Games\\FIFA Manager 13\\plugins\\ucp\\player_shoes.csv", 14, FifamVersion());
        writer2.WriteLine(L"playerfifaid,shoeid,comment");
        if (writer2.Available()) {
            fifadb->ForAllPlayers([&](FifaPlayer &p) {
                if (Utils::Contains(fifaIds, p.GetId())) {
                    UChar shoeid = p.internal.shoetypecode;
                    if (shoeid != 0 && exists(Path(L"D:\\Games\\FIFA Manager 13\\data\\assets\\shoes") / Utils::Format(L"t20__%d.fsh", shoeid)))
                        writer2.WriteLine(p.GetId(), shoeid, Quoted(p.m_quickName));
                }
            });
            writer2.Close();
        }
        else
            ::Error(L"Unable to open output file");

        Vector<String> duplicatedPlayers;
        for (auto const &[fifaId, players] : fifaIds) {
            if (players.size() > 1) {
                for (auto const &p : players)
                    duplicatedPlayers.push_back(Utils::Format(L"%u - %s (%s, %u)", fifaId, p->GetName(), p->mWriteableStringID, p->mFootballManagerID));
            }
        }
        if (!duplicatedPlayers.empty()) {
            std::wcout << "Duplicated players:" << std::endl;
            for (auto const &s : duplicatedPlayers)
                std::wcout << s << std::endl;
        }
    }
};
