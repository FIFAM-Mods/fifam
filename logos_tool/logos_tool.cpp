#include "Error.h"
#include "Utils.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "Magick++.h"

using namespace Magick;

void SafeWriteImage(Image &image, std::string const &filename) {
    try {
        image.write(filename);
    }
    catch (...) {

    }
}

void WriteOneBadge(Image &badgeImg, Path const &outputPath, String const &badgeName, UInt gameId, Bool leagues = false) {
    Bool oldBadgePath = gameId <= 9;
    badgeImg.resize(Geometry(256, 256));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues256" : L"Badge256") : L"256x256") / badgeName).string()));
    badgeImg.resize(Geometry(128, 128));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues128" : L"Badge128") : L"128x128") / badgeName).string()));
    badgeImg.resize(Geometry(64, 64));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues64" : L"Badge64") : L"64x64") / badgeName).string()));
    badgeImg.resize(Geometry(32, 32));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues32" : L"Badge32") : L"32x32") / badgeName).string()));
}

Pair<Int, Int> ResizeWithAspectRatio(Int x, Int y, Int dest_x, Int dest_y, Float minRatioPercentage = 0.75f) {
    Float dest_ratio = (Float)dest_x / dest_y;
    Float ratio = (Float)x / y;
    Bool is_main_x = false;
    Int main = y;
    Int other = x;
    Int main_dest = dest_y;
    if (ratio < dest_ratio) {
        is_main_x = true;
        main = x;
        other = y;
        main_dest = dest_x;
    }
    if (main != main_dest) {
        if (main < main_dest && (Float)main / main_dest < minRatioPercentage)
            return { 0, 0 };
        other = (Int)((Float)other * ((Float)main_dest / main));
        main = main_dest;
    }
    if (is_main_x)
        return { main, other };
    return { other, main };
}

Pair<Int, Int> ResizeAndFit(Int x, Int y, Int dest_x, Int dest_y, Float minRatioPercentage = 0.75f) {
    Float dest_ratio = (Float)dest_x / dest_y;
    Float ratio = (Float)x / y;
    Bool is_main_x = false;
    Int main = y;
    Int other = x;
    Int main_dest = dest_y;
    Int other_dest = dest_x;
    if (ratio > dest_ratio) {
        is_main_x = true;
        main = x;
        other = y;
        main_dest = dest_x;
        other_dest = dest_y;
    }
    if (main != main_dest) {
        if (main < main_dest && (Float)main / main_dest < minRatioPercentage && other < other_dest && (Float)other / other_dest < minRatioPercentage)
            return { 0, 0 };
        if (main > main_dest) {
            other = (Int)((Float)other * ((Float)main_dest / main));
            main = main_dest;
        }
    }
    if (is_main_x)
        return { main, other };
    return { other, main };
}

Bool ConvertOneTrophy(Path const &trophyPath, Path const &outputPath, Path const &trophyNamePath, String const &trophyRoomFolder) {
    if (exists(trophyPath)) {
        Image trophyImg(trophyPath.string());
        if (trophyImg.isValid()) {
            bool result = false;
            if (trophyImg.isValid()) {
                auto WriteImage = [](Image & img, Path const &filepath) {
                    if (!exists(filepath.parent_path()))
                        create_directories(filepath.parent_path());
                    SafeWriteImage(img, filepath.string());
                };
                {
                    auto [newX, newY] = ResizeAndFit(trophyImg.baseColumns(), trophyImg.baseRows(), 518, 410, 0.78f);
                    if (newX && newY) {
                        Image trophyRoomImg = trophyImg;
                        if (newX != trophyRoomImg.baseColumns() || newY != trophyRoomImg.baseRows())
                            trophyRoomImg.resize(Geometry(newX, newY));
                        trophyRoomImg.extent(Geometry(534, 423), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                        WriteImage(trophyRoomImg, outputPath / trophyRoomFolder / trophyNamePath);
                        result = true;
                    }
                }
                {
                    auto [newX, newY] = ResizeAndFit(trophyImg.baseColumns(), trophyImg.baseRows(), 244, 244, 0.88f);
                    if (newX && newY) {
                        if (newX != trophyImg.baseColumns() || newY != trophyImg.baseRows())
                            trophyImg.resize(Geometry(newX, newY));
                        trophyImg.extent(Geometry(256, 256), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                        WriteImage(trophyImg, outputPath / L"256x256" / trophyNamePath);
                        trophyImg.resize(Geometry(128, 128));
                        WriteImage(trophyImg, outputPath / L"128x128" / trophyNamePath);
                        trophyImg.resize(Geometry(64, 64));
                        WriteImage(trophyImg, outputPath / L"64x64" / trophyNamePath);
                        trophyImg.resize(Geometry(32, 32));
                        WriteImage(trophyImg, outputPath / L"32x32" / trophyNamePath);
                        result = true;
                    }
                }
                return result;
            }
        }
    }
    return false;
}

Int wmain(Int argc, WideChar **argv) {

    auto Copy = [](Path const &dst, Path const &src) {
        if (exists(src))
            copy(src, dst, copy_options::overwrite_existing);
    };

    Path kits = L".output";
    Path mini = L".output\\mini";
    create_directories(L".output\\mini");
    for (auto &p : directory_iterator(current_path())) {
        if (p.is_directory() && p.path().c_str()[0] != '.') {
            String dir = p.path().filename().c_str();
            UInt id = Utils::SafeConvertInt<UInt>(dir, true);
            if (id != 0) {
                std::wcout << Utils::Format(L"Processing %08X", id) << std::endl;

                Copy(mini / (dir + L"_h.png"), p.path() / L"mini0.png");
                Copy(mini / (dir + L"_a.png"), p.path() / L"mini1.png");

                Copy(kits / (dir + L"_j_h.png"), p.path() / L"H1.png");
                Copy(kits / (dir + L"_s_h.png"), p.path() / L"H2.png");

                Copy(kits / (dir + L"_j_a.png"), p.path() / L"A1.png");
                Copy(kits / (dir + L"_s_a.png"), p.path() / L"A2.png");

                Copy(kits / (dir + L"_j_t.png"), p.path() / L"T1.png");
                Copy(kits / (dir + L"_s_t.png"), p.path() / L"T2.png");

                Copy(kits / (dir + L"_j_g.png"), p.path() / L"G1.png");
                Copy(kits / (dir + L"_s_g.png"), p.path() / L"G2.png");

                Copy(kits / (dir + L"_l.png"), p.path() / L"L.png");
                Copy(kits / (dir + L"_l_h.png"), p.path() / L"L1.png");
                Copy(kits / (dir + L"_l_a.png"), p.path() / L"L2.png");
                Copy(kits / (dir + L"_l_t.png"), p.path() / L"L3.png");
                Copy(kits / (dir + L"_l_g.png"), p.path() / L"L4.png");
            }
        }
    }

    return 0;
    
    SetConsoleTitleW(L"Logos Tool by Dmitry v1.0");

    std::wcout << std::endl << L"Universal Converter Project" << std::endl << L"Logos Tool by Dmitry" << std::endl << L"Version 1.0" << std::endl << std::endl;

    try {
        Path outputDir = L"output";
        if (!exists(outputDir))
            create_directories(outputDir);

        // convert trophies

        for (auto &p : directory_iterator(L"trophies")) {
            if (p.path().extension() == L".png" || p.path().extension() == L".tga")
                ConvertOneTrophy(p.path(), outputDir / L"trophies", String(p.path().stem().c_str()) + L".tga", L"534x423");
        }

        // convert badges

        Path outputClubs = outputDir / L"badges" / L"clubs";
        Path outputLeagues = outputDir / L"badges" / L"Leagues";

        if (!exists(outputClubs / L"256x256"))
            create_directories(outputClubs / L"256x256");
        if (!exists(outputClubs / L"128x128"))
            create_directories(outputClubs / L"128x128");
        if (!exists(outputClubs / L"64x64"))
            create_directories(outputClubs / L"64x64");
        if (!exists(outputClubs / L"32x32"))
            create_directories(outputClubs / L"32x32");
        if (!exists(outputLeagues / L"256x256"))
            create_directories(outputLeagues / L"256x256");
        if (!exists(outputLeagues / L"128x128"))
            create_directories(outputLeagues / L"128x128");
        if (!exists(outputLeagues / L"64x64"))
            create_directories(outputLeagues / L"64x64");
        if (!exists(outputLeagues / L"32x32"))
            create_directories(outputLeagues / L"32x32");

        for (auto &p : directory_iterator(L"badges")) {
            if (p.path().extension() == L".png" || p.path().extension() == L".tga") {
                String badgeName = p.path().stem().c_str();
                Bool isCompBadge = Utils::SafeConvertInt<UInt>(badgeName) > 0xFFFFFF;
                Image badgeImg(p.path().string());
                if (badgeImg.isValid()) {
                    badgeImg.resize(Geometry(248, 248));
                    //badgeImg.stroke
                    badgeImg.extent(Geometry(256, 256), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    WriteOneBadge(badgeImg, isCompBadge ? outputLeagues : outputClubs, badgeName + L".tga", 14);
                }
            }
        }
    }
    catch (std::exception &e) {
        ::Error(e.what());
    }

    return 0;
}
