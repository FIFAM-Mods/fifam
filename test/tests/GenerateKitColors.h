#pragma once
#include "FifaFifamIDsEnvironment.h"
#include "FifaDbEnvironment.h"
#include "FifamCompLeague.h"
#include "Magick++.h"

using namespace Magick;

class GenerateKitColors {
public:
    static void DrawRect(Image &img, int left, int top, int width, int height, unsigned char red, unsigned char green, unsigned char blue) {
        img.fillColor(Magick::Color(red, green, blue));
        img.draw(DrawableRectangle(left, top, left + width, top + height));
    }

    GenerateKitColors() {
        FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        auto const &ids = GetEnvironment<FifaFifamIDsEnvironment>();
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

        auto GenerateForClub = [&](UInt fifamID, UInt fifaID) {
            String teamIDstrW = Utils::Format(L"%08X", fifamID);
            std::string teamIDstr = Utils::WtoA(teamIDstrW);
            std::wcout << teamIDstrW << std::endl;
            UInt countryId = (fifamID >> 16) & 0xFF;
            UInt teamIndex = fifamID & 0xFFFF;
            FifaTeam *fifaTeam = fifadb->GetTeam(fifaID);
            if (fifaTeam) {
                for (FifaKit *fifaKit : fifaTeam->m_kits) {
                    std::string suffix;
                    if (fifaKit->internal.teamkittypetechid == 0)
                        suffix = "_h";
                    else if (fifaKit->internal.teamkittypetechid == 1)
                        suffix = "_a";
                    else if (fifaKit->internal.teamkittypetechid == 3)
                        suffix = "_t";
                    if (!suffix.empty()) {
                        if (exists("D:\\Projects\\fifam\\content\\fm13\\art_04\\data\\kits\\" + teamIDstr + suffix + ".tga")) {
                            Image img(Geometry(8, 64), Magick::Color(fifaKit->internal.teamcolorprimr, fifaKit->internal.teamcolorprimg, fifaKit->internal.teamcolorprimb));
                            img.defineValue("png", "color-type", "2");
                            Image one(img, Geometry(8, 8, 0, 0));
                            one.defineValue("png", "color-type", "2");
                            one.write(outputPathOne + teamIDstr + suffix + ".png");
                            if (fifaKit->internal.teamcolorsecpercent >= 15)
                                DrawRect(img, 0, 32, 8, 32, fifaKit->internal.teamcolorsecr, fifaKit->internal.teamcolorsecg, fifaKit->internal.teamcolorsecb);
                            img.write(outputPathVert + teamIDstr + suffix + ".png");
                            img.rotate(-90);
                            img.write(outputPathHor + teamIDstr + suffix + ".png");
                            auto i = img.interpolate();
                            auto f = img.filterType();
                            img.interpolate(PixelInterpolateMethod::NearestInterpolatePixel);
                            img.filterType(FilterType::PointFilter);
                            img.resize(Geometry(512, 96));
                            img.interpolate(i);
                            img.filterType(f);
                            double distortArgs[] = { 22 };
                            img.distort(DistortMethod::ScaleRotateTranslateDistortion, 1, distortArgs);
                            img.extent(Geometry(64, 64), GravityType::CenterGravity);
                            img.write(outputPathRot + teamIDstr + suffix + ".png");
                        }
                    }
                }
            }
        };
        for (auto const &c : ids.mFifaClubs)
            GenerateForClub(c.second, c.first);
    }
};
