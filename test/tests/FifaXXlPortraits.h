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
        { L'À', 0xC0 },
        { L'Á', 0xC1 },
        { L'Â', 0xC2 },
        { L'Ã', 0xC3 },
        { L'Ä', 0xC4 },
        { L'Å', 0xC5 },
        { L'Æ', 0xC6 },
        { L'Ç', 0xC7 },
        { L'È', 0xC8 },
        { L'É', 0xC9 },
        { L'Ê', 0xCA },
        { L'Ë', 0xCB },
        { L'Ì', 0xCC },
        { L'Í', 0xCD },
        { L'Î', 0xCE },
        { L'Ï', 0xCF },
        { L'Ð', 0xD0 },
        { L'Ñ', 0xD1 },
        { L'Ò', 0xD2 },
        { L'Ó', 0xD3 },
        { L'Ô', 0xD4 },
        { L'Õ', 0xD5 },
        { L'Ö', 0xD6 },
        { L'×', 0xD7 },
        { L'Ø', 0xD8 },
        { L'Ù', 0xD9 },
        { L'Ú', 0xDA },
        { L'Û', 0xDB },
        { L'Ü', 0xDC },
        { L'Ý', 0xDD },
        { L'Þ', 0xDE },
        { L'ß', 0xDF },
        { L'à', 0xE0 },
        { L'á', 0xE1 },
        { L'â', 0xE2 },
        { L'ã', 0xE3 },
        { L'ä', 0xE4 },
        { L'å', 0xE5 },
        { L'æ', 0xE6 },
        { L'ç', 0xE7 },
        { L'è', 0xE8 },
        { L'é', 0xE9 },
        { L'ê', 0xEA },
        { L'ë', 0xEB },
        { L'ì', 0xEC },
        { L'í', 0xED },
        { L'î', 0xEE },
        { L'ï', 0xEF },
        { L'ð', 0xF0 },
        { L'ñ', 0xF1 },
        { L'ò', 0xF2 },
        { L'ó', 0xF3 },
        { L'ô', 0xF4 },
        { L'õ', 0xF5 },
        { L'ö', 0xF6 },
        { L'÷', 0xF7 },
        { L'ø', 0xF8 },
        { L'ù', 0xF9 },
        { L'ú', 0xFA },
        { L'û', 0xFB },
        { L'ü', 0xFC },
        { L'ý', 0xFD },
        { L'þ', 0xFE },
        { L'ÿ', 0xFF },
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
