#pragma once
#include "FifamDbEnvironment.h"

class CopyTeamKits {
public:
    CopyTeamKits() {
        Set<UInt> clubIds = { 0x1B0004, 0x1B0005, 0x1B000F, 0x1B002D, 0x2D0007, 0x050006,
            0x360001, 0x360003, 0x360004, 0x360005, 0x360006, 0x360008, 0x36000B, 0x36000E, 0x36000F, 0x360011, 0x360012, 0x360018, 0x360026,
            0x36002F, 0x360030, 0x360032, 0x360042, 0x360045, 0x361001, 0x361008, 0x360002, 0x360007, 0x360009, 0x36000C, 0x360013, 0x360028,
            0x360029, 0x36002E, 0x360036, 0x36003C, 0x360044, 0x360047, 0x36004E, 0x361026, 0x361028, 0x361034, 0x361082, 0x36305C, 0x3630FC,
            0x363495
        };
        for (UInt i = 1; i <= 207; i++)
            clubIds.insert((i << 16) | 0xFFFF);
        Map<UInt, FifamKit> kits;
        {
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "db_kits");
            for (auto id : clubIds) {
                auto club = db.GetClubFromUID(id);
                if (club)
                    kits[id] = club->mKit;
            }
        }
        {
            FifamWriter log("log_teamkits.txt");
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "db");
            for (auto &[id, kit] : kits) {
                auto club = db.GetClubFromUID(id);
                if (club) {
                    club->mKit = kit;
                    log.WriteLine(L"Updated kit for " + FifamTr(club->mName));
                }
            }
            db.mWritingOptions.mWriteFixtures = false;
            db.mWritingOptions.mWriteExternalScripts = false;
            db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
        }
    }
};
