#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

#define SKIP_EMPTY

class ExportPlayerAccessories {
public:
    ExportPlayerAccessories() {

        std::wcout << L"Reading FIFA database..." << std::endl;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();

        std::wcout << L"Reading fm-fifa-players..." << std::endl;
        Vector<Pair<UInt, FifaPlayer *>> playersList;
        Path infoPath = L"D:\\Projects\\fifam\\db";
        FifamReader reader(infoPath / L"fm-fifa-players.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    String foomIdstr, fifaIdstr;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, dummy, dummy, dummy, foomIdstr, fifaIdstr);
                    if (!foomIdstr.empty() && !fifaIdstr.empty()) {
                        Int foomid = Utils::SafeConvertInt<Int>(foomIdstr);
                        if (foomid != -1) {
                            UInt fifaid = Utils::SafeConvertInt<Int>(fifaIdstr);
                            if (fifaid != -1) {
                                FifaPlayer *player = fifadb->GetPlayer(fifaid);
                                if (player)
                                    playersList.emplace_back(foomid, player);
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
            reader.Close();
        }
        else
            ::Error(L"Unable to open fm-fifa-players file");

        std::wcout << L"Sorting players list..." << std::endl;

        std::sort(playersList.begin(), playersList.end(), [](Pair<UInt, FifaPlayer *> const &a, Pair<UInt, FifaPlayer *> const &b) {
            return a.first < b.first;
        });

        std::wcout << L"Processing output file..." << std::endl;

        FifamWriter writer(L"D:\\Games\\FIFA Manager 13\\plugins\\ucp\\player_accessories.csv", 14, FifamVersion());
        writer.WriteLine(L"playerempicsid,comment,jerseystyle,sleeves,socks,acc1,acccolor1,acc2,acccolor2,acc3,acccolor3,acc4,acccolor4");
        if (writer.Available()) {
            for (auto const &entry : playersList) {
                FifaPlayer *player = entry.second;
                UChar jerseytype = 0;
                UChar sleeves = (player->internal.jerseysleevelengthcode == 0) ? 0 : 1;
                UChar socks = 0;
                if (player->internal.socklengthcode == 1 || player->internal.socklengthcode == 2)
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
                    //case 6:
                    //case 8:
                    //case 24:
                    //    return 7;
                    //case 7:
                    //case 9:
                    //case 25:
                    //    return 8;
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

            #ifdef SKIP_EMPTY
                if (jerseytype != 0 || sleeves != 0 || socks != 0 || accessories[0].first != 0) {
                #endif
                    writer.WriteLine(entry.first, Quoted(player->m_quickName), jerseytype, sleeves, socks,
                        accessories[0].first, accessories[0].second, accessories[1].first, accessories[1].second,
                        accessories[2].first, accessories[2].second, accessories[3].first, accessories[3].second
                    );
                #ifdef SKIP_EMPTY
                }
            #endif
            }
            writer.Close();
        }
        else
            ::Error(L"Unable to open output file");

        FifamWriter writer2(L"D:\\Games\\FIFA Manager 13\\plugins\\ucp\\player_shoes.csv", 14, FifamVersion());
        writer2.WriteLine(L"playerempicsid,shoeid,comment");
        if (writer2.Available()) {
            for (auto const &entry : playersList) {
                FifaPlayer *player = entry.second;
                UChar shoeid = player->internal.shoetypecode;
                if (exists(Path(L"D:\\Games\\FIFA Manager 13\\data\\zdata") / Utils::Format(L"t20__%d.fsh", shoeid)))
                    writer2.WriteLine(entry.first, shoeid, Quoted(player->m_quickName));
            }
            writer2.Close();
        }
        else
            ::Error(L"Unable to open output file");
    }
};
