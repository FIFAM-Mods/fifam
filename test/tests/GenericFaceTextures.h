#pragma once
#include "FifaFifamIDsEnvironment.h"
#include "FifaDbEnvironment.h"
#include "Magick++.h"

class GenericFaceTextures {
public:
    static void ResizeImage(Magick::Image &image, int w, int h) {
        Magick::Geometry geom(w, h);
        geom.aspect(true);
        image.filterType(Magick::FilterType::LanczosFilter);
        image.resize(geom);
    }

    GenericFaceTextures() {
        using namespace Magick;
        path folderPath = "D:\\Projects\\FIFA\\generic_faces";
        path inPath = folderPath / "in";
        path interPath = folderPath / "inter";
        path outPath = folderPath / "out";
        path tempPath = folderPath / "temp";
        create_directories(interPath);
        create_directories(outPath);
        create_directories(tempPath);
        unsigned int IMAGE_RES = 512;
        const size_t NUM_BROWS = 1;
        const size_t NUM_SIDEBURNS = 1;
        const size_t NUM_BEARDCOLORS = 5;
        const size_t NUM_BEARDS = 16;
        const size_t NUM_SKINCOLORS = 10;
        unsigned char skintypes[] = { 0, 1, 2, 100, 104, 101, 102, 103 };
        const size_t NUM_SKINTYPES = std::size(skintypes);
        for (size_t skinclr = 1; skinclr <= NUM_SKINCOLORS; skinclr++) {
            for (size_t skintype = 0; skintype < NUM_SKINTYPES; skintype++) {
                for (size_t beard = 0; beard < NUM_BEARDS; beard++) {
                    for (size_t beardclr = 0; beardclr < NUM_BEARDCOLORS; beardclr++) {
                        for (size_t sideburns = 0; sideburns < NUM_SIDEBURNS; sideburns++) {
                            for (size_t brow = 0; brow < NUM_BROWS; brow++) {
                                path skinPath = folderPath / "skin";
                                bool female = skintypes[skintype] >= 100;
                                if (female) {
                                    if (beard != 0)
                                        continue;
                                    skinPath /= "female";
                                }
                                skinPath /= (std::to_string(skinclr) + ".tga");
                                path facePath = inPath / ("skin_" + std::to_string(skinclr) + "_" + std::to_string(skintypes[skintype]) + "_color.dds");
                                path beardPath = inPath / ("beard_" + std::to_string(skinclr) + "_" + std::to_string(beard) + "_" + std::to_string(beardclr) + "_color.dds");
                                path browPath = inPath / ("brow_" + std::to_string(skinclr) + "_" + std::to_string(skintypes[skintype] < 100 ? 0 : 4) + "_" + std::to_string(beardclr) + "_color.dds");
                                if (exists(skinPath) && exists(facePath) && exists(beardPath) && exists(browPath)) {
                                    Image faceImg(facePath.string());
                                    if (faceImg.columns() != IMAGE_RES || faceImg.rows() != IMAGE_RES)
                                        ResizeImage(faceImg, IMAGE_RES, IMAGE_RES);
                                    Image beardImg(beardPath.string());
                                    if (beardImg.columns() != IMAGE_RES || beardImg.rows() != IMAGE_RES)
                                        ResizeImage(beardImg, IMAGE_RES, IMAGE_RES);
                                    Image browImg(browPath.string());
                                    if (browImg.columns() != IMAGE_RES || browImg.rows() != IMAGE_RES)
                                        ResizeImage(browImg, IMAGE_RES, IMAGE_RES);
                                    Image skinImg(skinPath.string());
                                    if (skinImg.rows() != IMAGE_RES)
                                        ResizeImage(skinImg, IMAGE_RES * 2, IMAGE_RES);
                                    skinImg.composite(faceImg, IMAGE_RES, 0);
                                    skinImg.composite(browImg, IMAGE_RES, 0, MagickCore::CompositeOperator::OverCompositeOp);
                                    skinImg.composite(beardImg, IMAGE_RES, 0, MagickCore::CompositeOperator::OverCompositeOp);
                                    path skinDir = interPath / ("t21__" +
                                        std::to_string(brow) + "_" +
                                        std::to_string(beardclr) + "_" +
                                        std::to_string(beard) + "_" +
                                        std::to_string(skintype) + "_" +
                                        std::to_string(skinclr));
                                    create_directory(skinDir);
                                    //skinImg.flip();
                                    skinImg.write((skinDir / "tp01.tga").string());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};
