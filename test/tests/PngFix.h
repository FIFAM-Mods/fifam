#pragma once
#include "FifamUtils.h"
#include "Magick++.h"

using namespace Magick;

class PngFix {
public:
    PngFix() {
        for (auto const &i : recursive_directory_iterator(current_path())) {
            Path p = i.path();
            if (Utils::ToLower(p.extension().string()) == ".png") {
                FILE *f = nullptr;
                _wfopen_s(&f, p.c_str(), L"rb");
                if (f) {
                    unsigned char buf[26];
                    bool fix = false;
                    if (fread(buf, 1, 26, f) == 26 && (buf[24] != 8 || buf[25] != 6))
                        fix = true;
                    fclose(f);
                    if (fix) {
                        std::wcout << L"Fix " << p.filename().c_str() << std::endl;
                        Image img(p.string());
                        img.defineValue("png", "color-type", "6");
                        img.write(p.string());
                    }
                }
            }
        }
    }
};
