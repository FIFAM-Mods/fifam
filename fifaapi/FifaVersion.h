#pragma once

#include <vector>
#include <string>
#include "FifaDate.h"
#include "FifaDataFile.h"

class FifaVersion {
public:
    struct Internal {
        int minor;
        int major;
        std::wstring schema;
        int isonline;
        int exportdate;
        int artificialkey;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring GetVersionString();
    FifaDate GetDate();
    std::wstring GetDateString();

    void FromLine(FifaDataFile::Line &line);
};