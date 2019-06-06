#pragma once
#include "FifamDbEnvironment.h"
#include "FifaDbEnvironment.h"
#include "Magick++.h"

using namespace Magick;

class GenerateKitColors {
public:
    static void DrawRect(Image &img, int left, int top, int width, int height, unsigned char red, unsigned char green, unsigned char blue) {
        img.fillColor(Magick::Color(red, green, blue));
        img.draw(DrawableRectangle(left, top, left + width, top + height));
    }

    GenerateKitColors() {
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();

        std::string outputPath = "D:\\Games\\FIFA Manager 13\\ucp_popups\\colors\\vert\\";

        if (!exists(outputPath))
            create_directories(outputPath);

        auto GenerateForClub = [&](FifamClub *club) {
            bool generated = false;
            String teamIDstrW = Utils::Format(L"%08X", club->mUniqueID);
            std::string teamIDstr = Utils::WtoA(teamIDstrW);
            std::wcout << teamIDstrW << std::endl;
            if (club->mFifaID) {
                FifaTeam *fifaTeam = fifadb->GetTeam(club->mFifaID);
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
                            if (exists("D:\\Games\\FIFA Manager 13\\data\\kits\\" + teamIDstr + suffix + ".tga")) {
                                Image img(Geometry(8, 64), Magick::Color(fifaKit->internal.teamcolorprimr, fifaKit->internal.teamcolorprimg, fifaKit->internal.teamcolorprimb));
                                img.defineValue("png", "color-type", "2");
                                if (fifaKit->internal.teamcolorsecpercent >= 15)
                                    DrawRect(img, 0, 32, 8, 32, fifaKit->internal.teamcolorsecr, fifaKit->internal.teamcolorsecg, fifaKit->internal.teamcolorsecb);
                                img.write(outputPath + teamIDstr + suffix + ".png");
                                generated = true;
                            }
                        }
                    }
                }
            }
            if (!generated) {
                for (UInt i = 0; i < 2; i++) {
                    FifamKit::Set &kit = club->mKit.mSets[i];
                    Image img(Geometry(8, 64), "white");
                    img.defineValue("png", "color-type", "2");
                    
                    auto Draw1 = [&](int shirtColorId) {
                        shirtColorId -= 1;
                        if (kit.mShortsColors[0] == kit.mShirtColors[shirtColorId])
                            DrawRect(img, 0, 0, 8, 64, kit.mShirtColors[shirtColorId].r, kit.mShirtColors[shirtColorId].g, kit.mShirtColors[shirtColorId].b);
                        else {
                            DrawRect(img, 0, 0, 8, 32, kit.mShirtColors[shirtColorId].r, kit.mShirtColors[shirtColorId].g, kit.mShirtColors[shirtColorId].b);
                            DrawRect(img, 0, 32, 8, 32, kit.mShortsColors[0].r, kit.mShortsColors[0].g, kit.mShortsColors[0].b);
                        }
                    };
                    auto Draw2 = [&](int shirtColorId, int shirtColorId2) {
                        shirtColorId -= 1; shirtColorId2 -= 1;
                        DrawRect(img, 0, 0, 8, 32, kit.mShirtColors[shirtColorId].r, kit.mShirtColors[shirtColorId].g, kit.mShirtColors[shirtColorId].b);
                        DrawRect(img, 0, 32, 8, 32, kit.mShirtColors[shirtColorId2].r, kit.mShirtColors[shirtColorId2].g, kit.mShirtColors[shirtColorId2].b);
                    };
                    auto Draw3 = [&](int shirtColorId, int shirtColorId2, int shirtColorId3) {
                        shirtColorId -= 1; shirtColorId2 -= 1; shirtColorId3 -= 1;
                        DrawRect(img, 0, 0, 8, 21, kit.mShirtColors[shirtColorId].r, kit.mShirtColors[shirtColorId].g, kit.mShirtColors[shirtColorId].b);
                        DrawRect(img, 0, 21, 8, 22, kit.mShirtColors[shirtColorId2].r, kit.mShirtColors[shirtColorId2].g, kit.mShirtColors[shirtColorId2].b);
                        DrawRect(img, 0, 21 + 22, 8, 21, kit.mShirtColors[shirtColorId3].r, kit.mShirtColors[shirtColorId3].g, kit.mShirtColors[shirtColorId3].b);
                    };
                    switch (kit.mShirt) {
                    case 2:
                    case 4:
                    case 5:
                    case 8:
                    case 9:
                    case 18:
                    case 26:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 35:
                    case 54:
                    case 56:
                    case 58:
                    case 61:
                    case 62:
                    case 66:
                        Draw1(1);
                        break;
                    case 1:
                    case 6:
                    case 10:
                    case 11:
                    case 13:
                    case 20:
                    case 27:
                    case 33:
                    case 34:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 50:
                    case 52:
                    case 64:
                        if (kit.mShirtColors[0] == kit.mShirtColors[2])
                            Draw1(1);
                        else
                            Draw2(1, 3);
                        break;
                    case 22:
                    case 53:
                    case 57:
                    case 59:
                    case 60:
                        if (kit.mShirtColors[0] == kit.mShirtColors[1])
                            Draw1(1);
                        else
                            Draw2(1, 2);
                        break;
                    case 23:
                        if (kit.mShirtColors[0] == kit.mShirtColors[2])
                            Draw1(1);
                        else
                            Draw2(3, 1);
                        break;
                    case 3:
                    case 7:
                    case 14:
                    case 16:
                    case 17:
                    case 19:
                    case 21:
                    case 24:
                    case 25:
                    case 36:
                    case 41:
                    case 48:
                    case 55:
                    case 65:
                        if (kit.mShirtColors[0] == kit.mShirtColors[2])
                            Draw1(1);
                        else
                            Draw3(1, 3, 1);
                        break;
                    case 15:
                    case 63:
                        if (kit.mShirtColors[0] == kit.mShirtColors[1])
                            Draw1(1);
                        else
                            Draw3(1, 2, 1);
                        break;
                    case 12:
                    case 49:
                    case 51:
                        if (kit.mShirtColors[0] == kit.mShirtColors[1] && kit.mShirtColors[1] == kit.mShirtColors[2])
                            Draw1(1);
                        else if (kit.mShirtColors[0] == kit.mShirtColors[1])
                            Draw2(1, 3);
                        else if (kit.mShirtColors[0] == kit.mShirtColors[2])
                            Draw2(1, 2);
                        else if (kit.mShirtColors[1] == kit.mShirtColors[2])
                            Draw2(1, 2);
                        else
                            Draw3(1, 2, 3);
                        break;
                    }
                    std::string suffix;
                    if (i == FifamKit::Home)
                        suffix = "_h";
                    else
                        suffix = "_a";
                    img.write(outputPath + teamIDstr + suffix + ".png");
                }
            }
        };
        for (auto c : db->mCountries) {
            if (c) {
                GenerateForClub(&c->mNationalTeam);

                auto countryComps = c->GetCompetitions(true);
                for (auto comp : countryComps) {
                    if (comp.second->GetDbType() == FifamCompDbType::League) {
                        FifamCompLeague *league = comp.second->AsLeague();
                        for (auto club : league->mTeams) {
                            if (club.IsValid()) {
                                if (club.IsFirstTeam()) {
                                    auto clubLeague = club.mPtr->GetProperty<FifamCompLeague *>(L"league", nullptr);
                                    if (!clubLeague)
                                        club.mPtr->SetProperty<FifamCompLeague *>(L"league", league);
                                }
                                else if (club.IsReserveTeam()) {
                                    auto clubLeague = club.mPtr->GetProperty<FifamCompLeague *>(L"reserveleague", nullptr);
                                    if (!clubLeague)
                                        club.mPtr->SetProperty<FifamCompLeague *>(L"reserveleague", league);
                                }
                            }
                        }
                    }
                }
            }
        }
        for (auto c : db->mClubs)
            GenerateForClub(c);
    }
};
