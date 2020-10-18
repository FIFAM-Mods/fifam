#pragma once

#include <vector>
#include <string>
#include "FifaDate.h"
#include "FifaDataFile.h"

class FifaVersion {
public:
    struct Internal {
        int minor = 0;
        int major = 0;
        int isonline = 0;
        // @since FIFA12
        int exportdate = 0;
        // @since FIFA13
        std::wstring schema;
        int artificialkey = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring GetVersionString();
    FifaDate GetDate();
    std::wstring GetDateString();

    void FromLine(FifaDataFile::Line &line);
};
