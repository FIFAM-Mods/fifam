#include "Error.h"
#include "Utils.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "Magick++.h"
#include "FifamNation.h"

Int wmain(Int argc, WideChar **argv) {
    Path inputDir = L"input";
    Path outputDir = L"output";
    create_directories(outputDir / L"64x64");
    create_directories(outputDir / L"96x96");
    create_directories(outputDir / L"200x120");
    create_directories(outputDir / L"512x85");
    FifamWriter writer(outputDir / L"Sponsor List.txt", 14, FifamVersion());
    writer.SetReplaceQuotes(false);
    writer.WriteLine(L"// FIFA Manager 2021 - Sponsors List");
    writer.WriteLine(L"// ");
    writer.WriteLine(L"// Authors: Dmitry, Ilyon, KEBAB, madebygrey, fm-zocker.de");
    writer.WriteLine(L"// ");
    writer.WriteNewLine();
    writer.WriteLine(L"BEGIN( COUNTRIES )");
    writer.WriteLine(L"    BEGIN( #COUNTRY )");
    writer.WriteLine(L"        // Country ID. If ID is 0, the following sponsors will be used in all countries that do not have a specific list of sponsors.");
    writer.WriteLine(L"        COUNTRY_ID = 0");
    writer.WriteLine(L"        // The lowest league level these sponsor can be used for. If this is set to 255, sponsors can be used for all league levels.");
    writer.WriteLine(L"        MAX_LEAGUE_LEVEL = 255");
    writer.WriteLine(L"        BEGIN( #SPONSOR )");
    writer.WriteLine(L"            NAME = \"EA SPORTS\"");
    writer.WriteLine(L"            PICTURE_BIG = \"EASPORTS.tga\"");
    writer.WriteLine(L"            PICTURE_SMALL = \"EASPORTS.tga\"");
    writer.WriteLine(L"            PICTURE_ADBOARD = \"EASPORTS.tga\"");
    writer.WriteLine(L"        END");
    writer.WriteLine(L"    END");
    Magick::Image in96("templates\\sponsor96.tga");
    Magick::Image in64("templates\\sponsor64.tga");

    if (!in96.isValid())
        Error("sponsor96.tga is not available");
    if (!in64.isValid())
        Error("sponsor64.tga is not available");

    auto ScanFolder = [&](Path const &dir, Set<String> &out, Bool isDefault = false) {
        for (auto &i : recursive_directory_iterator(dir)) {
            auto const &p = i.path();
            if (p.extension() == L".png" || p.extension() == L".tga" || p.extension() == L".svg") {
                Magick::Image in(p.string());
                if (in.isValid()) {
                    auto filenameParts = Utils::Split(p.stem().c_str(), L'_', false, false);
                    if (filenameParts.empty()) {
                        Error(L"Filename is empty");
                        continue;
                    }
                    Bool adboard = false;
                    Bool shirtLogo = false;
                    Bool colorize = false;
                    Bool white = false;
                    Bool quantize = false;
                    Bool quantizeSecond = false;
                    Bool q_plus = false;
                    Bool q_minus = false;
                    Bool border = false;
                    String filename = filenameParts[0];
                    String filenameOnly = filename;
                    out.insert(filename);
                    Utils::Replace(filename, L" ", L"");
                    String filenameTga = filename + L".tga";
                    for (UInt param = 1; param < filenameParts.size(); param++) {
                        if (filenameParts[param] == L"adb")
                            adboard = true;
                        else if (filenameParts[param] == L"clr")
                            colorize = true;
                        else if (filenameParts[param] == L"s")
                            shirtLogo = true;
                        else if (filenameParts[param] == L"w")
                            white = true;
                        else if (filenameParts[param] == L"b")
                            border = true;
                        else if (filenameParts[param] == L"q")
                            quantize = true;
                        else if (filenameParts[param] == L"q+") {
                            quantize = true;
                            q_plus = true;
                        }
                        else if (filenameParts[param] == L"q-") {
                            quantize = true;
                            q_minus = true;
                        }
                        else if (filenameParts[param] == L"q2") {
                            quantize = true;
                            quantizeSecond = true;
                        }
                        else if (filenameParts[param] == L"q2+") {
                            quantize = true;
                            q_plus = true;
                            quantizeSecond = true;
                        }
                        else if (filenameParts[param] == L"q2-") {
                            quantize = true;
                            q_minus = true;
                            quantizeSecond = true;
                        }
                    }

                    if (adboard && in.columns() == 512 && in.rows() == 85) {
                        in.type(MagickCore::ImageType::TrueColorAlphaType);
                        in.compressType(MagickCore::CompressionType::NoCompression);
                        in.write((outputDir / L"512x85" / filenameTga).string());
                    }
                    else {

                        in.trim();

                        if (!adboard && !shirtLogo) {

                            Bool colorize = filenameParts.size() > 1 && filenameParts[1] == L"clr";

                            // 96x96
                            
                            Magick::Image x96(in);
                            //x96.flip(); // TEMPORARY fix
                            const UInt border_x96 = 4;
                            x96.resize(Magick::Geometry(96 - border_x96 * 2, 96 - border_x96 * 2));
                            x96.extent(Magick::Geometry(96, 96), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            Magick::Image out96(in96);
                            out96.type(MagickCore::ImageType::TrueColorAlphaType);
                            out96.composite(x96, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                            out96.composite(x96, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::CopyAlphaCompositeOp);
                            out96.compressType(MagickCore::CompressionType::NoCompression);
                            out96.write((outputDir / L"96x96" / filenameTga).string());

                            // 64x64

                            Magick::Image x64(in);
                            //x64.flip(); // TEMPORARY fix
                            const UInt border_x64 = 3;
                            x64.resize(Magick::Geometry(64 - border_x64 * 2, 64 - border_x64 * 2));
                            x64.extent(Magick::Geometry(64, 64), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            Magick::Image out64(in64);
                            out64.type(MagickCore::ImageType::TrueColorAlphaType);
                            out64.composite(x64, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                            out64.composite(x64, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::CopyAlphaCompositeOp);
                            out64.compressType(MagickCore::CompressionType::NoCompression);
                            out64.write((outputDir / L"64x64" / filenameTga).string());
                        }

                        if (!adboard) {

                            // 200x120

                            Magick::Image x200(in);
                            x200.resize(Magick::Geometry(188, 105));
                            x200.extent(Magick::Geometry(188, 105), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            Magick::Image out200(Magick::Geometry(200, 120), Magick::Color(0, 0, 0, 0));
                            out200.composite(x200, Magick::Geometry(188, 105, 6, 14), MagickCore::CompositeOperator::OverCompositeOp);
                            out200.type(MagickCore::ImageType::TrueColorAlphaType);
                            out200.compressType(MagickCore::CompressionType::NoCompression);
                            if (!colorize)
                                out200.write((outputDir / L"200x120" / filenameTga).string());
                            else {
                                Magick::Image out200_a(out200);
                                Magick::Color colors[] = {
                                    Magick::Color(255, 255, 255),
                                    Magick::Color(0, 0, 0),
                                    Magick::Color(255, 255, 0),
                                    Magick::Color(255, 0, 0),
                                    Magick::Color(0, 255, 0),
                                    Magick::Color(0, 0, 255),
                                };
                                for (UInt c = 0; c < std::size(colors); c++) {
                                    out200.fillColor(colors[c]);
                                    out200.draw(Magick::DrawableRectangle(0, 0, 200, 120));
                                    out200.composite(out200_a, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::CopyAlphaCompositeOp);
                                    out200.write((outputDir / L"200x120" / (filename + Utils::Format(L"_%d", c + 1) + L".tga")).string());
                                }
                            }
                        }

                        auto pd = p;
                        if (!shirtLogo &&
                            (adboard ||
                            (!exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb_w" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_w_adb" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb_b" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_b_adb" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb_q" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_q_adb" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb_q+" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_adb_q-" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_q+_adb" + p.extension().string())) &&
                                !exists(pd.replace_filename(Utils::WtoA(filenameOnly) + "_q-_adb" + p.extension().string()))
                                )
                                )
                            )
                        {
                            // 512x85
                            Magick::Image x512(in);
                            Magick::Image out512(Magick::Geometry(512, 85), Magick::Color(0, 0, 0, 0));
                            x512.resize(Magick::Geometry(430, 75));
                            UInt adWidth = x512.columns();
                            x512.extent(Magick::Geometry(adWidth, 81), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            const UInt minDistanceBetweenBlocks = 70;
                            const UInt minDistanceBetweenBlocksHalf = minDistanceBetweenBlocks / 2;
                            UInt adHeight = x512.rows();
                            if (minDistanceBetweenBlocks * 3 + adWidth * 3 < 512) {

                                out512.composite(x512, Magick::Geometry(adWidth, adHeight, minDistanceBetweenBlocksHalf + ((171 - minDistanceBetweenBlocks) - adWidth) / 2, 5), MagickCore::CompositeOperator::OverCompositeOp);
                                out512.composite(x512, Magick::Geometry(adWidth, adHeight, 171 + minDistanceBetweenBlocksHalf + ((171 - minDistanceBetweenBlocks) - adWidth) / 2, 5), MagickCore::CompositeOperator::OverCompositeOp);
                                out512.composite(x512, Magick::Geometry(adWidth, adHeight, 341 + minDistanceBetweenBlocksHalf + ((171 - minDistanceBetweenBlocks) - adWidth) / 2, 5), MagickCore::CompositeOperator::OverCompositeOp);

                            }
                            else if (minDistanceBetweenBlocks * 2 + adWidth * 2 < 512) {
                                out512.composite(x512, Magick::Geometry(adWidth, adHeight, minDistanceBetweenBlocksHalf + ((256 - minDistanceBetweenBlocks) - adWidth) / 2, 5), MagickCore::CompositeOperator::OverCompositeOp);
                                out512.composite(x512, Magick::Geometry(adWidth, adHeight, 256 + minDistanceBetweenBlocksHalf + ((256 - minDistanceBetweenBlocks) - adWidth) / 2, 5), MagickCore::CompositeOperator::OverCompositeOp);
                            }
                            else {
                                out512.composite(x512, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                            }
                            out512.resize("512x68!");
                            out512.extent(Magick::Geometry(512, 85), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            const Bool writeAdBoardPng = false;
                            if (writeAdBoardPng) {
                                out512.type(MagickCore::ImageType::TrueColorAlphaType);
                                out512.compressType(MagickCore::CompressionType::NoCompression);
                                out512.write((outputDir / L"512x85" / (filename + L".png")).string());
                            }

                            if (border) {
                                Magick::Color c(0, 0, 0, 0);
                                for (size_t row = 0; row < in.rows(); ++row) {
                                    for (size_t column = 0; column < in.columns(); ++column) {
                                        auto pc = in.pixelColor(row, column);
                                        if (pc.quantumAlpha() == 255) {
                                            c = pc;
                                        }
                                    }
                                }
                                if (c.quantumAlpha() != 0) {
                                    Magick::Image out512_tga(Magick::Geometry(512, 85), c);
                                    out512_tga.composite(out512, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                                    out512_tga.type(MagickCore::ImageType::TrueColorAlphaType);
                                    out512_tga.compressType(MagickCore::CompressionType::NoCompression);
                                    out512_tga.write((outputDir / L"512x85" / filenameTga).string());
                                    //Error(L"%s - %d %d %d", filename.c_str(), c.quantumRed(), c.quantumGreen(), c.quantumBlue());
                                }
                                else
                                    white = true;
                            }
                            else if (quantize) {
                                Map<Magick::Color, UInt> cmap;
                                for (size_t row = 0; row < in.rows(); ++row) {
                                    for (size_t column = 0; column < in.columns(); ++column) {
                                        auto pc = in.pixelColor(row, column);
                                        if (pc.quantumAlpha() == 255)
                                            cmap[pc]++;
                                    }
                                }
                                if (!cmap.empty() && (!quantizeSecond || cmap.size() >= 2)) {
                                    Magick::Color c(0, 0, 0, 0);
                                    if (!quantizeSecond) {
                                        UInt mostUsed = 0;
                                        for (auto const &[mc, count] : cmap) {
                                            if (count > mostUsed) {
                                                c = mc;
                                                mostUsed = count;
                                            }
                                        }
                                    }
                                    else {
                                        Vector<Pair<Magick::Color, UInt>> clrPairs;
                                        for (auto const &[mc, count] : cmap)
                                            clrPairs.emplace_back(mc, count);
                                        std::sort(clrPairs.begin(), clrPairs.end(), [](Pair<Magick::Color, UInt> const &a, Pair<Magick::Color, UInt> const &b) {
                                            return a.second >= b.second;
                                        });
                                        c = clrPairs[1].first;
                                    }
                                    const UInt f = 75;
                                    if (q_plus) {
                                        c = Magick::Color(
                                            Utils::Clamp(c.quantumRed() + f, 0, 255),
                                            Utils::Clamp(c.quantumGreen() + f, 0, 255),
                                            Utils::Clamp(c.quantumBlue() + f, 0, 255));
                                    }
                                    else if (q_minus) {
                                        c = Magick::Color(
                                            Utils::Clamp(c.quantumRed() - f, 0, 255),
                                            Utils::Clamp(c.quantumGreen() - f, 0, 255),
                                            Utils::Clamp(c.quantumBlue() - f, 0, 255));
                                    }
                                    Magick::Image out512_tga(Magick::Geometry(512, 85), c);
                                    out512_tga.composite(out512, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                                    out512_tga.type(MagickCore::ImageType::TrueColorAlphaType);
                                    out512_tga.compressType(MagickCore::CompressionType::NoCompression);
                                    out512_tga.write((outputDir / L"512x85" / filenameTga).string());
                                    //Error(L"%s - %d %d %d", filename.c_str(), c.quantumRed(), c.quantumGreen(), c.quantumBlue());
                                }
                                else
                                    white = true;
                            }
                            if (white) {
                                Magick::Image x1(in);
                                x1.resize("1x1!");
                                Magick::Color avgColor = x1.pixelColor(0, 0);
                                Magick::Image out512_tga(Magick::Geometry(512, 85), Magick::Color(avgColor.quantumRed(), avgColor.quantumGreen(), avgColor.quantumBlue()));
                                Magick::Image out512_white(out512);
                                out512_white.fillColor(Magick::Color(255, 255, 255));
                                out512_white.draw(Magick::DrawableRectangle(0, 0, 512, 85));
                                out512_white.composite(out512, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::CopyAlphaCompositeOp);
                                out512_tga.composite(out512_white, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                                out512_tga.type(MagickCore::ImageType::TrueColorAlphaType);
                                out512_tga.compressType(MagickCore::CompressionType::NoCompression);
                                out512_tga.write((outputDir / L"512x85" / filenameTga).string());
                            }
                            else if (!quantize && !border) {
                                Magick::Image out512_tga(Magick::Geometry(512, 85), Magick::Color(231, 231, 231));
                                out512_tga.composite(out512, MagickCore::GravityType::CenterGravity, MagickCore::CompositeOperator::OverCompositeOp);
                                out512_tga.type(MagickCore::ImageType::TrueColorAlphaType);
                                out512_tga.compressType(MagickCore::CompressionType::NoCompression);
                                out512_tga.write((outputDir / L"512x85" / filenameTga).string());
                            }
                        }
                    }
                }
            }
        }
    };
    
    for (auto &i : recursive_directory_iterator(inputDir)) {
        if (is_directory(i)) {
            auto const &p = i.path();
            String dirname = p.filename().c_str();
            if (dirname == L"default") {
                Set<String> sponsors;
                ScanFolder(p, sponsors, true);
            }
            else {
                UInt countryId = Utils::SafeConvertInt<UInt>(dirname);
                if (countryId >= 1 && countryId <= 207) {
                    try {
                        writer.WriteLine(L"    BEGIN( #COUNTRY )");
                        writer.WriteLine(L"        // " + FifamNation::MakeFromInt(countryId).ToStr());
                        writer.WriteLine(L"        COUNTRY_ID = " + Utils::Format(L"%u", countryId));
                        writer.WriteLine(L"        MAX_LEAGUE_LEVEL = 255");
                        Set<String> sponsors;
                        ScanFolder(p, sponsors);
                        for (auto s : sponsors) {
                            writer.WriteLine(L"        BEGIN( #SPONSOR )");
                            writer.WriteLine(L"            NAME = \"" + s + L"\"");
                            Utils::Replace(s, L" ", L"");
                            writer.WriteLine(L"            PICTURE_BIG = \"" + s + L".tga\"");
                            writer.WriteLine(L"            PICTURE_SMALL = \"" + s + L".tga\"");
                            writer.WriteLine(L"            PICTURE_ADBOARD = \"" + s + L".tga\"");
                            writer.WriteLine(L"        END");
                        }
                        writer.WriteLine(L"    END");
                    }
                    catch (std::exception e) {
                        Error(Utils::WtoA(dirname) + ": " + e.what());
                    }
                }
            }
        }
    }
    writer.WriteLine(L"END");
    writer.WriteNewLine();
    return 0;
}
