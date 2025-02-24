#pragma once
#include "FifamTypes.h"
#include "Utils.h"
#include <Magick++.h>

class PopupColorsFromKits {
public:
    static Bool GetImageColor(Image &img, ColorPair &clr) {
        Bool result = false;
        if (img.isValid()) {
            const Double colorRange = 100.0;
            const Float secondColorFactor = 5.0f;
            Map<::Color, UInt> clrsMap;
            for (size_t row = 0; row < img.rows(); ++row) {
                for (size_t column = 0; column < img.columns(); ++column) {
                    auto pc = img.pixelColor(column, row);
                    if (pc.quantumAlpha() == 255)
                        clrsMap[::Color(pc.quantumRed(), pc.quantumGreen(), pc.quantumBlue())]++;
                }
            }
            if (!clrsMap.empty()) {
                Vector<Pair<::Color, UInt>> clrsVecFromMap;
                clrsVecFromMap.reserve(clrsMap.size());
                for (auto const &i : clrsMap)
                    clrsVecFromMap.push_back(i);
                std::sort(clrsVecFromMap.begin(), clrsVecFromMap.end(), [](Pair<::Color, UInt> const &a, Pair<::Color, UInt> const &b) {
                    return a.second >= b.second;
                });
                Vector<Pair<::Color, UInt>> clrsVec;
                clrsVec.reserve(clrsVecFromMap.size());
                for (auto const &i : clrsVecFromMap) {
                    Bool found = false;
                    if (!clrsVec.empty()) {
                        for (auto &p : clrsVec) {
                            if (::Color::Distance(p.first, i.first) < colorRange) {
                                p.second++;
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found)
                        clrsVec.push_back(i);
                }
                if (!clrsVec.empty()) {
                    std::sort(clrsVec.begin(), clrsVec.end(), [](Pair<::Color, UInt> const &a, Pair<::Color, UInt> const &b) {
                        return a.second >= b.second;
                    });
                    clr.first = clrsVec[0].first;
                    clr.second = clr.first;
                    if (clrsVec.size() > 1 && clrsVec[0].second != 0 && clrsVec[1].second != 0) {
                        if (Float(clrsVec[0].second) / Float(clrsVec[1].second) < secondColorFactor)
                            clr.second = clrsVec[1].first;
                    }
                    result = true;
                }
            }
        }
        return result;
    }

    PopupColorsFromKits() {
        using namespace Magick;
        std::string outputPathVert = "D:\\Games\\FIFA Manager 25\\ucp_popups\\colors\\vert\\";
        std::string outputPathHor = "D:\\Games\\FIFA Manager 25\\ucp_popups\\colors\\hor\\";
        std::string outputPathRot = "D:\\Games\\FIFA Manager 25\\ucp_popups\\colors\\rot\\";
        std::string outputPathOne = "D:\\Games\\FIFA Manager 25\\ucp_popups\\colors\\one\\";
        if (!exists(outputPathVert))
            create_directories(outputPathVert);
        if (!exists(outputPathHor))
            create_directories(outputPathHor);
        if (!exists(outputPathRot))
            create_directories(outputPathRot);
        if (!exists(outputPathOne))
            create_directories(outputPathOne);
        Image colorMask;
        if (exists("kit_colors_mask.png")) {
            colorMask = Image("kit_colors_mask.png");
            if (colorMask.isValid())
                colorMask = colorMask.separate(ChannelType::RedChannel);
        }
        for (auto i : directory_iterator("kits")) {
            if (!i.is_regular_file())
                continue;
            auto fileName = i.path().stem().string() + ".png";
            Image image(i.path().string());
            if (colorMask.isValid())
                image.composite(colorMask, 0, 0, CompositeOperator::CopyAlphaCompositeOp);
            image.resize(Geometry(16, 32));
            ColorPair clr;
            if (GetImageColor(image, clr)) {
                Image img(Geometry(8, 64), Magick::Color(clr.first.r, clr.first.g, clr.first.b));
                img.defineValue("png", "color-type", "2");
                Image one(img, Geometry(8, 8, 0, 0));
                one.defineValue("png", "color-type", "2");
                one.write(outputPathOne + fileName);
                if (clr.first != clr.second) {
                    img.fillColor(Magick::Color(clr.second.r, clr.second.g, clr.second.b));
                    img.draw(DrawableRectangle(0, 32, 8, 64));
                }
                img.write(outputPathVert + fileName);
                img.rotate(-90);
                img.write(outputPathHor + fileName);
                auto i = img.interpolate();
                auto f = img.filterType();
                img.interpolate(PixelInterpolateMethod::NearestInterpolatePixel);
                img.filterType(FilterType::PointFilter);
                img.resize(Geometry(512, 96));
                img.interpolate(i);
                img.filterType(f);
                Double distortArgs[] = { 22 };
                img.distort(DistortMethod::ScaleRotateTranslateDistortion, 1, distortArgs);
                img.extent(Geometry(64, 64), GravityType::CenterGravity);
                img.write(outputPathRot + fileName);
            }
        }
    }
};
