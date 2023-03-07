#pragma once
#include "FifaFifamIDsEnvironment.h"
#include "Magick++.h"

class WriteBadgesLaLiga {
public:
    WriteBadgesLaLiga() {
        auto &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        auto ProcessDir = [&](Path const &input, Path const &output) {
            create_directories(output);
            for (auto const &i : directory_iterator(input)) {
                Path p = i.path();
                String filename = p.stem().c_str();
                if (p.extension().string() == ".dds" && filename.starts_with(L'l')) {
                    filename = filename.substr(1);
                    UInt id = Utils::SafeConvertInt<UInt>(filename);
                    if (ids.mFifaClubs.contains(id)) {
                        Magick::Image image(p.string());
                        image.defineValue("png", "color-type", "6");
                        image.write((output / Utils::Format("%08X.png", ids.mFifaClubs[id])).string());
                    }
                }
            }
        };
        ProcessDir(R"(C:\Users\user\Desktop\dark)", R"(D:\Projects\fifam\content\art_05\ucp_popups\badges\2D010000)");
        ProcessDir(R"(C:\Users\user\Desktop\light)", R"(D:\Projects\fifam\content\art_05\ucp_popups\badges\2D010001)");
    }
};
