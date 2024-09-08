#pragma once
#include "FifamReadWrite.h"
#include "Magick++.h"

class SortShoes {
public:
    Int GetShoeColor(Path const &filepath) {
        static Vector<Pair<UInt, ColorPair>> shoeColors = {
            { 1,  {{ 216, 217, 216 }, { 216, 217, 216 }}}, // white
            { 2,  {{  28,  28,  28 }, {  28,  28,  28 }}}, // black
            { 3,  {{  63, 102, 148 }, {  63, 102, 148 }}}, // blue
            { 4,  {{ 107,  14,  16 }, { 107,  14,  16 }}}, // red
            { 5,  {{ 153, 132,  41 }, { 153, 132,  41 }}}, // yellow
            { 6,  {{ 233, 233, 233 }, {   5,  57,  93 }}}, // white-blue
            { 7,  {{ 227, 228, 227 }, {  24,  25,  24 }}}, // white-black
            { 6,  {{ 233, 233, 233 }, {  72, 121, 171 }}}, // white-lightblue
            { 10, {{  38, 106,  63 }, {  38, 106,  63 }}}, // green
            { 11, {{  16,  16,  16 }, { 203,  33,  33 }}}, // black-red
            { 12, {{ 104, 105, 104 }, { 104, 105, 104 }}}, // grey
            { 13, {{ 225, 225, 225 }, { 203,  33,  33 }}}, // white-red
            { 14, {{ 208, 187,   2 }, { 208, 187,   2 }}}, // lightyellow
            { 15, {{ 214, 128,  10 }, { 214, 128,  10 }}}, // orange
            { 16, {{   5,  57,  93 }, {   5,  57,  93 }}}, // darkblue
            { 17, {{ 145, 187,  63 }, { 145, 187,  63 }}}, // lightgreen
            { 18, {{ 239,  57,  99 }, { 239,  57,  99 }}}, // magenta
            { 19, {{ 210,  26,  32 }, { 210,  26,  32 }}} // brightred
        };
        ColorPair clr;
        Image img(filepath.string());
        if (img.isValid()) {
            const UInt imgSize = 8;
            const Double colorRange = 100.0;
            const Float secondColorFactor = 5.0f;
            Map<::Color, UInt> clrsMap;
            img.resize(Geometry(imgSize, imgSize));
            for (size_t row = 0; row < img.rows(); ++row) {
                for (size_t column = 0; column < img.columns(); ++column) {
                    auto pc = img.pixelColor(column, row);
                    if (pc.quantumAlpha() == 255)
                        clrsMap[::Color(pc.quantumRed(), pc.quantumGreen(), pc.quantumBlue())]++;
                }
            }
            if (!clrsMap.empty()) {
                Vector<Pair<::Color, UInt>> clrsVecFromMap;
                clrsVecFromMap.reserve(imgSize < 100 ? (imgSize * imgSize) : imgSize);
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
                    clr.second = clr.first; // ::Color(255, 255, 255);
                    if (clrsVec.size() > 1 && clrsVec[0].second != 0 && clrsVec[1].second != 0) {
                        if (Float(clrsVec[0].second) / Float(clrsVec[1].second) < secondColorFactor)
                            clr.second = clrsVec[1].first;
                    }
                    return clr.FindIndexInTable(shoeColors);
                }
            }
        }
        return -1;
    }

    void Sort(bool onlyDefinition) {
        Path shoesPath = L"C:\\Users\\Dmitri\\Desktop\\shoe_boots";
        Path sortedPath = shoesPath / "sorted";
        Map<Int, string> shoeFolders = {
            { 1 , "1 - white"            },
            { 2 , "2 - black"            },
            { 3 , "3 - blue"             },
            { 4 , "4 - red"              },
            { 5 , "5 - yellow"           },
            { 6 , "6 - white_blue"       },
            { 7 , "7 - white_black"      },
            { 8 , "8 - white_light_blue" },
            { 10, "10 - green"           },
            { 11, "11 - black_red"       },
            { 12, "12 - grey"            },
            { 13, "13 - white_red"       },
            { 14, "14 - light_yellow"    },
            { 15, "15 - orange"          },
            { 16, "16 - dark_blue"       },
            { 17, "17 - light_green"     },
            { 18, "18 - magenta"         },
            { 19, "19 - bright_red"      }
        };
        Map<Int, string> shoeColorNames = {
            { 1 , "WHITE" },
            { 2 , "BLACK" },
            { 3 , "BLUE" },
            { 4 , "RED" },
            { 5 , "GOLD" },
            { 6 , "WHITE_BLUE" },
            { 7 , "WHITE_BLACK" },
            { 8 , "WHITE_LIGHT_BLUE" },
            { 10, "GREEN" },
            { 11, "BLACK_RED" },
            { 12, "GREY" },
            { 13, "WHITE_RED" },
            { 14, "YELLOW" },
            { 15, "ORANGE" },
            { 16, "DARK_BLUE" },
            { 17, "LIGHT_GREEN" },
            { 18, "MAGENTA" },
            { 19, "BRIGHT_RED" }
        };
        if (!onlyDefinition) {
            for (auto const &[id, folder] : shoeFolders)
                create_directories(sortedPath / folder);
            for (auto const &i : directory_iterator(shoesPath)) {
                auto const &p = i.path();
                if (is_regular_file(p) && Utils::ToLower(p.extension().string()) == ".png") {
                    string fileName = p.stem().string();
                    if (Utils::StartsWith(fileName, "shoe_")) {
                        auto fileNameParts = Utils::Split(fileName, '_', false, false, false);
                        if (fileNameParts.size() >= 2) {
                            Int shoeId = Utils::SafeConvertInt<UInt>(fileNameParts[1]);
                            if (shoeId >= 15 && shoeId < 400) {
                                auto colorId = GetShoeColor(p);
                                if (colorId == -1)
                                    create_directories(sortedPath / "unknown");
                                string folderName = colorId == -1 ? "unknown" : shoeFolders[colorId];
                                copy(p, sortedPath / folderName / (to_string(shoeId) + ".png"), copy_options::overwrite_existing);
                            }
                        }
                    }
                }
            }
        }
        FifamWriter w(sortedPath / "GetPlayerShoesIdFromFifaId.txt");
        w.WriteLine("int GetPlayerShoesIdFromFifaId(int shoeTypeCode) {");
        w.WriteLine("    int ShoeType = 0;");
        w.WriteLine("    switch (shoeTypeCode) {");
        for (auto const &[id, folder] : shoeFolders) {
            Set<Int> shoeIds;
            for (auto const &i : directory_iterator(sortedPath / folder)) {
                auto const &p = i.path();
                if (is_regular_file(p) && Utils::ToLower(p.extension().string()) == ".png") {
                    Int shoeId = Utils::SafeConvertInt<UInt>(p.stem().string());
                    if (shoeId > 0)
                        shoeIds.insert(shoeId);
                }
            }
            if (!shoeIds.empty()) {
                for (auto const &i : shoeIds)
                    w.WriteLine("    case " + to_string(i) + ":");
                w.WriteLine("        ShoeType = SHOE_" + shoeColorNames[id] + ";");
                w.WriteLine("        break;");
            }
        }
        w.WriteLine("    default:");
        w.WriteLine("        ShoeType = Random::Get(1, 19);");
        w.WriteLine("    }");
        w.WriteLine("    return ShoeType;");
        w.WriteLine("}");
    }

    SortShoes() {
        Sort(true);
    }
};
