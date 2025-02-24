#pragma once
#include "FifaFifamIDsEnvironment.h"

using namespace Magick;

class FifaBadgesToFifam {
public:
    FifaBadgesToFifam() {
        auto const &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        Path inputPath = "fifa";
        Path outputPath = "fifam";
        create_directories(outputPath);
        for (auto const &i : directory_iterator(inputPath)) {
            Path p = i.path();
            if (is_regular_file(p)) {
                StringA filename = p.stem().string();
                if (Utils::StartsWith(filename, "l")) {
                    UInt fifaId = Utils::SafeConvertInt<UInt>(filename.substr(1));
                    auto it = ids.mFifaClubs.find(fifaId);
                    if (it != ids.mFifaClubs.end()) {
                        UInt fifamId = (*it).second;
                        copy(p, outputPath / (Utils::Format("%08X", fifamId) + p.extension().string()));
                    }
                }
            }
        }
    }
};
