#pragma once
#include "FifamTypes.h"
#include "Magick++.h"

using namespace Magick;

class AdboardsFIFA07 {
public:
    AdboardsFIFA07() {
        Path inputPath = R"(C:\Users\Dmitri\Desktop\f24_adb_3)";
        String adbnames[3] = { L"adba.tga", L"adbb.tga", L"adbc.tga" };
        for (auto const &d : directory_iterator(inputPath)) {
            Path p = d.path();
            if (is_directory(p)) {
                String dirName = p.stem();
                if (Utils::StartsWith(dirName, L"t223__")) {
                    Path adboardPath = p / L"adba.tga";
                    Image adboard(adboardPath.string());
                    adboard.autoOrient();
                    for (UInt i = 0; i < std::size(adbnames); i++) {
                        Image img(adboard, Geometry(512, 192, 0, i * 192));
                        Geometry geom(512, 256);
                        geom.aspect(true);
                        img.filterType(FilterType::LanczosFilter);
                        img.resize(geom);
                        adboardPath.replace_filename(adbnames[i]);
                        img.type(ImageType::TrueColorType);
                        img.compressType(CompressionType::NoCompression);
                        img.write(adboardPath.string());
                    }
                }
            }
        }
    }
};
