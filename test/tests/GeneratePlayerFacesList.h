#pragma once
#include "FifamReadWrite.h"
#include <iostream>

class GeneratePlayerFacesList {
public:
    GeneratePlayerFacesList() {
        struct IconInfo {
            String quickName;
            String firstname;
            String lastname;
            String commonname;
        };
        Map<UInt, IconInfo> icons;
        FifamReader r("icons.csv");
        if (r.Available()) {
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    IconInfo info;
                    UInt id = 0;
                    r.ReadLine(id, info.quickName, info.firstname, info.lastname, info.commonname);
                    if (id > 0)
                        icons[id] = info;
                }
                else
                    r.SkipLine();
            }
        }
        FifamWriter w("player_face_ids.txt");
        w.WriteLine("faceid\tfacevariation\tfaceversion\tdbversion\tplayername\tgender\tfirstname\tlastname\tnickname\tvariation\ticon\tismanager\tskintone\teyecolor\toverall\tbirthdate\tage\t14\t18\t19\t20\t21\t22\t23\ttargetid\ttest");
        Path headsFolder = R"(D:\Downloads\fifa23pc_starheads_F14_update_9_10\heads)";
        FifaDatabase db(R"(D:\Projects\fifam\db\fifa)", true);
        for (auto const &i : directory_iterator(headsFolder)) {
            auto const &p = i.path();
            if (is_regular_file(p)) {
                string filename = p.stem().string();
                if (filename.starts_with("head_")) {
                    UInt playerId = 0;
                    UInt typeId = 0;
                    UInt variationId = 0;
                    if (sscanf(filename.c_str(), "head_%d_%d_%d", &playerId, &typeId, &variationId) == 3 || sscanf(filename.c_str(), "head_%d_%d", &playerId, &typeId) == 2) {
                        if (playerId > 0 && typeId == 0) {
                            String targetid;
                            auto player = db.GetPlayer(playerId);
                            if (player) {
                                w.WriteLineWithSeparator(L'\t', playerId, 0, 23, player->m_gameId, Quoted(player->m_quickName), !player->IsMale(),
                                    Quoted(player->m_firstName), Quoted(player->m_lastName), Quoted(player->m_commonName), 0, 0, 0, player->internal.skintonecode,
                                    player->internal.eyecolorcode, player->internal.overallrating, player->m_birthDate.ToString(), 2024 - player->m_birthDate.year,
                                    0, 0, 0, 0, 0, 0, 1, targetid, 0);
                            }
                            else {
                                auto manager = db.GetManager(playerId);
                                if (manager) {
                                    auto birthdate = FifaDate(manager->internal.birthdate);
                                    w.WriteLineWithSeparator(L'\t', playerId, 0, 23, manager->m_gameId, Quoted(manager->m_quickName), !manager->IsMale(),
                                        Quoted(manager->m_firstName), Quoted(manager->m_lastName), Quoted(manager->m_commonName), 0, 0, 1, manager->internal.skintonecode,
                                        manager->internal.eyecolorcode, 0, birthdate.ToString(), 2024 - birthdate.year,
                                        0, 0, 0, 0, 0, 0, 1, targetid, 0);
                                }
                                else {
                                    IconInfo *icon = icons.contains(playerId) ? &icons[playerId] : nullptr;
                                    if (icon) {
                                        w.WriteLineWithSeparator(L'\t', playerId, 0, 23, 0, Quoted(icon->quickName), 0, Quoted(icon->firstname),
                                            Quoted(icon->lastname), Quoted(icon->commonname), 0, 1, 0, -1, -1, 0, L"01.01.1900", 0, 0, 0, 0, 0, 0, 0, 1, targetid, 0);
                                    }
                                    else {
                                        w.WriteLineWithSeparator(L'\t', playerId, 0, 23, 0, String(), 0, String(), String(), String(), 0, 0, 0, -1,
                                            -1, 0, L"01.01.1900", 0, 0, 0, 0, 0, 0, 0, 1, targetid, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};
