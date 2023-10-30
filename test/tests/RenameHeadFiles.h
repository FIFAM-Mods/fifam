#pragma once
#include "FifamReadWrite.h"

class RenameHeadFiles {
public:
    RenameHeadFiles() {
        static StringA filesToRename[] = {
            "m46__%d.o",
            "m47__%d.o",
            "m228__%d.o",
            "m728__%d.o",
            "t21__%d_0_0.fsh",
            "t21__%d_0_0_0_0",
            "t22__%d_0.fsh",
            "cm_%d.dds",
            "cmbhair_%d.dds",
            "eff_%d.dds"
        };
        FifamReader r("rename_heads.txt");
        while (!r.IsEof()) {
            if (!r.EmptyLine()) {
                Int from = 0, to = 0;
                StringA s;
                r.ReadFullLine(s);
                if (sscanf_s(s.c_str(), "%u %u", &from, &to) == 2 && from != 0 && to != 0) {
                    for (auto const &f : filesToRename) {
                        auto pathFrom = Utils::Format(f, from);
                        if (exists(pathFrom))
                            rename(pathFrom, Utils::Format(f, to));
                    }
                }
            }
            else
                r.SkipLine();
        }
    }
};
