#pragma once
#include "FifamDbEnvironment.h"

class MergeFaceIDs {
public:
    Map<UInt, String> ReadFaceIDs(Path const &fileName) {
        Map<UInt, String> faceIDs;
        FifamReader r(fileName, 14, false, false);
        UInt count = r.ReadLine<UInt>();
        for (UInt i = 0; i < count; i++) {
            String name = r.ReadFullLine();
            UInt faceId = r.ReadLine<UInt>();
            faceIDs[faceId] = name;
        }
        return faceIDs;
    }

    MergeFaceIDs() {
        auto faceIDs = ReadFaceIDs(L"FaceIDs.txt");
        auto faceIDsNew = ReadFaceIDs(L"FaceIDs new.txt");
        for (auto const &[faceId, name] : faceIDsNew)
            faceIDs[faceId] = name;
        FifamWriter w(L"FaceIDs merged.txt");
        w.WriteLine(faceIDs.size());
        for (auto const &[faceId, name] : faceIDs) {
            w.WriteLine(name);
            w.WriteLine(faceId);
        }
    }
};
