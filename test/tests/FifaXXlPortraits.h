#pragma once
#include "FifamDbEnvironment.h"
#include "FifamClubBadge.h"
#include "Error.h"
#include "FifamNames.h"
#include <FifaDataFile.h>

class FifaXXlPortraits {
public:
    static void FixName(String& name) {
        static Map<WideChar, WideChar> chars = {
        { L'�', 0xC0 },
        { L'�', 0xC1 },
        { L'�', 0xC2 },
        { L'�', 0xC3 },
        { L'�', 0xC4 },
        { L'�', 0xC5 },
        { L'�', 0xC6 },
        { L'�', 0xC7 },
        { L'�', 0xC8 },
        { L'�', 0xC9 },
        { L'�', 0xCA },
        { L'�', 0xCB },
        { L'�', 0xCC },
        { L'�', 0xCD },
        { L'�', 0xCE },
        { L'�', 0xCF },
        { L'�', 0xD0 },
        { L'�', 0xD1 },
        { L'�', 0xD2 },
        { L'�', 0xD3 },
        { L'�', 0xD4 },
        { L'�', 0xD5 },
        { L'�', 0xD6 },
        { L'�', 0xD7 },
        { L'�', 0xD8 },
        { L'�', 0xD9 },
        { L'�', 0xDA },
        { L'�', 0xDB },
        { L'�', 0xDC },
        { L'�', 0xDD },
        { L'�', 0xDE },
        { L'�', 0xDF },
        { L'�', 0xE0 },
        { L'�', 0xE1 },
        { L'�', 0xE2 },
        { L'�', 0xE3 },
        { L'�', 0xE4 },
        { L'�', 0xE5 },
        { L'�', 0xE6 },
        { L'�', 0xE7 },
        { L'�', 0xE8 },
        { L'�', 0xE9 },
        { L'�', 0xEA },
        { L'�', 0xEB },
        { L'�', 0xEC },
        { L'�', 0xED },
        { L'�', 0xEE },
        { L'�', 0xEF },
        { L'�', 0xF0 },
        { L'�', 0xF1 },
        { L'�', 0xF2 },
        { L'�', 0xF3 },
        { L'�', 0xF4 },
        { L'�', 0xF5 },
        { L'�', 0xF6 },
        { L'�', 0xF7 },
        { L'�', 0xF8 },
        { L'�', 0xF9 },
        { L'�', 0xFA },
        { L'�', 0xFB },
        { L'�', 0xFC },
        { L'�', 0xFD },
        { L'�', 0xFE },
        { L'�', 0xFF },
        };
        for (WideChar& c : name) {
            if (Utils::Contains(chars, c))
                c = chars[c];
        }
    }

    FifaXXlPortraits() {
        Path gameDir = L"D:\\FIFA23";
        Path inputDir = gameDir / L"xxl";
        Path outputDir = gameDir / L"portraits\\club\\512x512";
        Path outputDirNotFound = gameDir / L"xxl_notfound";
        create_directories(outputDir);
        create_directories(outputDirNotFound);
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 22\\database");
        db.SetupWriteableStatus(13);
        Map<Int, FifamPlayer*> fifaPlayers;
        for (auto const& p : db.mPlayers) {
            auto idStartPos = p->mComment.find(L"FIFAID:");
            if (idStartPos != String::npos) {
                String playerIdStr = p->mComment.substr(idStartPos + 7);
                //::Error(playerIdStr);
                Int playerid = Utils::SafeConvertInt<Int>(playerIdStr);
                if (playerid != 0)
                    fifaPlayers[playerid] = p;
            }
        }
        for (auto const& i : directory_iterator(inputDir)) {
            auto const& p = i.path();
            if (is_regular_file(p) && p.extension() == L".png") {
                string filename = p.stem().string();
                auto first = filename.find('(');
                //J (195479)
                string playerIdStr = filename.substr(first + 1, filename.size() - first - 2);
                //::Error(playerIdStr);
                Int playerid = Utils::SafeConvertInt<Int>(playerIdStr);
                if (Utils::Contains(fifaPlayers, playerid)) {
                    FifamPlayer* player = fifaPlayers[playerid];
                    copy(p, outputDir / (player->mWriteableStringID + L".png"), copy_options::overwrite_existing);
                }
                else
                    copy(p, outputDirNotFound / p.filename(), copy_options::overwrite_existing);
            }
        }
    }
};
