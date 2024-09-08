#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

class PlayerNames {
public:
    PlayerNames() {
        Map<String, UInt> commentaries;
        FifamReader r(L"playernames.txt");
        if (r.Available()) {
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    String name;
                    UInt nameid = 0, commentaryid = 0;
                    r.ReadLineWithSeparator(L'\t', name, nameid, commentaryid);
                    if (commentaryid != 0 && commentaryid != 900000)
                        commentaries[name] = commentaryid;
                }
                else
                    r.SkipLine();
            }
        }
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        FifamReader reader(L"player_names.txt", 0);
        FifamWriter writer(L"player_names_out.txt", 14, FifamVersion());
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    UInt fifaId = reader.ReadLine<UInt>();
                    if (fifaId != 0) {
                        FifaPlayer *player = fifadb->GetPlayer(fifaId);
                        if (player) {
                            String name = player->m_commonName.empty() ? player->m_lastName : player->m_commonName;
                            UInt commentaryId = 0;
                            if (Utils::Contains(commentaries, name))
                                commentaryId = commentaries[name];
                            writer.WriteLineWithSeparator(L'\t', fifaId, name, commentaryId);
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
};
