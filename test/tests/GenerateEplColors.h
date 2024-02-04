#pragma once
#include "FifaFifamIDsEnvironment.h"
#include "FifaDbEnvironment.h"
#include "FifamCompLeague.h"
#include "Color.h"

class GenerateEplColors {
public:
    static GenColor GenColorFromInt(UInt clrValue) {
        return GenColor((clrValue >> 16) & 0xFF, (clrValue >> 8) & 0xFF, clrValue & 0xFF);
    }
    
    static UInt GenColorToInt(GenColor const &clr) {
        return 0xFF000000 | (clr.r << 16) | (clr.g << 8) | clr.b;
    }
    
    static UInt GetTextColor(UInt clr, Bool grey = false) {
        auto genColor = GenColorFromInt(clr);
        auto white = GenColor::Distance(genColor, GenColor(255, 255, 255));
        auto black = GenColor::Distance(genColor, GenColor(0, 0, 0));
        if (white < black)
            return grey ? 0xFF202020 : 0xFF000000;
        return grey ? 0xFFF0F0F0 : 0xFFFFFFFF;
    }
    
    static UInt GetAltColor(UInt clr) {
        auto genClr = GenColorFromInt(clr);
        if (genClr.r < 20 && genClr.g < 20 && genClr.b < 20) {
            genClr.r = (UChar)(roundf((Float)genClr.r * 0.85f)) + 38;
            genClr.g = (UChar)(roundf((Float)genClr.g * 0.85f)) + 38;
            genClr.b = (UChar)(roundf((Float)genClr.b * 0.85f)) + 38;
        }
        else {
            genClr.r = (UChar)(roundf((Float)genClr.r * 0.7f));
            genClr.g = (UChar)(roundf((Float)genClr.g * 0.7f));
            genClr.b = (UChar)(roundf((Float)genClr.b * 0.7f));
        }
        return GenColorToInt(genClr);
    }

    GenerateEplColors() {
        FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        auto const &ids = GetEnvironment<FifaFifamIDsEnvironment>();
        FifamWriter w("D:\\Games\\FIFA Manager 22\\fmdata\\popups\\teamcolors\\0E010000.csv");
        w.WriteLine(L"TeamID,Type,Color1,Color2,Color3,Color4,Color5,Color6,Color7,Color8,Color9,Color10,Comment");
        w.WriteLine(L"0x00000000,All,KIT_BACKGROUND,KIT_FOREGROUND,EPL_COLOR_1,EPL_COLOR_2,EPL_PRESENTATION_COLOR,EPL_PRESENTATION_COLOR_TEXT,0,0,0,0,Default");
        auto WHITE = GenColor(255, 255, 255);
        auto BLACK = GenColor(0, 0, 0);
        for (auto const &[fifaID, fifamID] : ids.mFifaClubs) {
            String teamIDstrW = Utils::Format(L"%08X", fifamID);
            std::string teamIDstr = Utils::WtoA(teamIDstrW);
            UInt countryId = (fifamID >> 16) & 0xFF;
            FifaTeam *fifaTeam = fifadb->GetTeam(fifaID);
            if (fifaTeam && countryId == FifamNation::England) {
                for (FifaKit *kit : fifaTeam->m_kits) {
                    std::string suffix;
                    String kitType;
                    if (kit->internal.teamkittypetechid == 0) {
                        suffix = "_h";
                        kitType = L"Home";
                    }
                    else if (kit->internal.teamkittypetechid == 1) {
                        suffix = "_a";
                        kitType = L"Away";
                    }
                    else if (kit->internal.teamkittypetechid == 3) {
                        suffix = "_t";
                        kitType = L"Third";
                    }
                    if (!suffix.empty()) {
                        if (exists("D:\\Projects\\fifam\\content\\fm13\\art_04\\data\\kits\\" + teamIDstr + suffix + ".tga")) {
                            Pair<UInt, UInt> kitColor;
                            kitColor.first = GenColorToInt(GenColor(kit->internal.teamcolorprimr, kit->internal.teamcolorprimg, kit->internal.teamcolorprimb));
                            if (kit->internal.teamcolorsecpercent >= 15)
                                kitColor.second = GenColorToInt(GenColor(kit->internal.teamcolorsecr, kit->internal.teamcolorsecg, kit->internal.teamcolorsecb));
                            else
                                kitColor.second = kitColor.first;
                            auto kitDiff = GenColor::Distance(GenColorFromInt(kitColor.first), GenColorFromInt(kitColor.second));
                            UInt eplColor = kitColor.second;
                            if (kitDiff < 100) {
                                eplColor = GetAltColor(kitColor.first);
                                if (GenColor::Distance(GenColorFromInt(kitColor.first), GenColorFromInt(eplColor)) < 100)
                                    eplColor = GetTextColor(kitColor.first, true);
                            }
                            UInt epl_1 = eplColor;
                            UInt epl_2 = 0;
                            if (GenColor::Distance(GenColorFromInt(eplColor), WHITE) < 200) {
                                if (GenColor::Distance(GenColorFromInt(kitColor.first), WHITE) < 200) {
                                    epl_1 = 0xFF202020;
                                    epl_2 = 0;
                                }
                                else {
                                    epl_1 = 0;
                                    epl_2 = eplColor;
                                }
                            }
                            UInt eplPresentation = kitColor.first;
                            if (GenColor::Distance(GenColorFromInt(eplPresentation), WHITE) < 200) {
                                eplPresentation = kitColor.second;
                                if (GenColor::Distance(GenColorFromInt(eplPresentation), WHITE) < 200)
                                    eplPresentation = 0xFF202020;
                            }
                            UInt eplPresentationText = GetTextColor(eplPresentation);
                            if (kitDiff < 100)
                                kitColor.second = GetTextColor(kitColor.first);
                            w.WriteLine(Utils::Format(L"0x%08X,%s,0x%X,0x%X,%s,%s,0x%X,0x%X,0,0,0,0,%s",
                                fifamID, kitType.c_str(), kitColor.first, kitColor.second,
                                (epl_1 == 0) ? L"0" : Utils::Format(L"0x%X", epl_1).c_str(),
                                (epl_2 == 0) ? L"0" : Utils::Format(L"0x%X", epl_2).c_str(),
                                eplPresentation, eplPresentationText, fifaTeam->m_name.c_str()));
                        }
                    }
                }
            }
        };
    }
};
