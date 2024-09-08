#pragma once
#include "FifamDbEnvironment.h"

class AssignPlayerFaces {
public:
    AssignPlayerFaces() {
        Set<UInt> faceIDs;
        FifamReader r(R"(D:\Games\FIFA Manager 22\fmdata\eng\FaceIDs.txt)", 14, false, false);
        UInt count = r.ReadLine<UInt>();
        for (UInt i = 0; i < count; i++) {
            r.SkipLine();
            UInt faceId = r.ReadLine<UInt>();
            //::Error("%d", faceId);
            faceIDs.insert(faceId);
        }

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");

        for (auto& p : db.mPlayers) {
            if (p->mFifaID != 0 && Utils::Contains(faceIDs, p->mFifaID))
                p->mSpecialFace = p->mFifaID;
            else
                p->mSpecialFace = 0;
        }

        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
