#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"
#include "Magick++.h"

class Fifa07Kits {
public:
    //static bool ConvertClubKitNumbersSet(int kitnumersId, bool jersey, std::string const &outputFile, ::Color const &clr1, ::Color const &clr2, ::Color const &clr3) {
    //    string fileNameBase = options.KitNumbersPath + "kitnumbers_" + to_string(kitnumersId) + "_";
    //    string fileName = fileNameBase + "1.png";
    //    if (!exists(fileName))
    //        return false;
    //    static unsigned int j_numbers[] = { 2, 6, 4, 0, 8, 3, 7, 5, 1, 9 };
    //    static unsigned int s_numbers[] = { 1, 2, 8, 3, 9, 4, 7, 6, 5, 0 };
    //    const unsigned int size = 256;
    //    Image channelsImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
    //    for (unsigned int i = 0; i < 10; i++) {
    //        fileName = fileNameBase + to_string((jersey ? j_numbers[i] : s_numbers[i]) + 1) + ".png";
    //        if (!exists(fileName))
    //            return false;
    //        Image img(fileName);
    //        if (!img.isValid())
    //            return false;
    //        ScaledComposite(channelsImg, img, size * i, 0, OverCompositeOp);
    //    }
    //    auto imgR = channelsImg.separate(MagickCore::ChannelType::RedChannel);
    //    auto imgG = channelsImg.separate(MagickCore::ChannelType::GreenChannel);
    //    auto imgB = channelsImg.separate(MagickCore::ChannelType::BlueChannel);
    //    auto imgA = channelsImg.separate(MagickCore::ChannelType::AlphaChannel);
    //    Image channelR(ScaledGeometry(size * 10, size), Magick::Color(clr1.r, clr1.g, clr1.b));
    //    Image channelG(ScaledGeometry(size * 10, size), Magick::Color(clr2.r, clr2.g, clr2.b));
    //    Image channelB(ScaledGeometry(size * 10, size), Magick::Color(clr3.r, clr3.g, clr3.b));
    //    ScaledComposite(channelR, imgR, 0, 0, MultiplyCompositeOp);
    //    ScaledComposite(channelG, imgG, 0, 0, MultiplyCompositeOp);
    //    ScaledComposite(channelB, imgB, 0, 0, MultiplyCompositeOp);
    //    Image finalImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
    //    ScaledComposite(finalImg, channelR, 0, 0, PlusCompositeOp);
    //    ScaledComposite(finalImg, channelG, 0, 0, PlusCompositeOp);
    //    ScaledComposite(finalImg, channelB, 0, 0, PlusCompositeOp);
    //    ScaledComposite(finalImg, imgA, 0, 0, CopyAlphaCompositeOp);
    //    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    //    finalImg.write(outputFile + "." + options.OutputFormat);
    //    return true;
    //}

    Fifa07Kits() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();

        Path infoPath = L"E:\\Projects\\fifam\\db";
        Path dataPath = L"E:\\Projects\\fifam\\content\\fm13\\art_04";
        Path outputPath = L"E:\\Projects\\FIFA07\\kits";
        Path kitShadowPath = L"E:\\Projects\\FIFA07\\kitShadow.png";
        Map<UInt, UInt> mFifaClubs;

        create_directories(outputPath);

        std::wcout << L"Reading fifam_countries.txt..." << std::endl;
        {
            FifamReader reader(infoPath / L"fifam_countries.txt", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        UInt fmId = 0, fifaId = 0;
                        reader.ReadLine(dummy, dummy, dummy, dummy, dummy, dummy, dummy, fmId, dummy, fifaId);
                        if (fmId != 0 && fifaId != 0)
                            mFifaClubs[fifaId] = (fmId << 16) | 0xFFFF;
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
        std::wcout << L"Reading fifam-uids.csv..." << std::endl;
        {
            FifamReader reader(infoPath / L"fifam-uids.csv", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        UInt fmId = 0, fifaId = 0;
                        reader.ReadLine(dummy, dummy, dummy, dummy, dummy, dummy, Hexadecimal(fmId), fifaId);
                        if (fmId != 0 && fifaId != 0)
                            mFifaClubs[fifaId] = fmId;
                    }
                    else
                        reader.SkipLine();
                }
            }
        }

        Vector<UInt> leaguesToProcess = { 53, 54 };

        Vector<Pair<UChar, String>> kitTypes = {
            { 0, L"h" },
            { 1, L"a" },
            { 2, L"g" },
            { 3, L"t" }
        };

        Magick::Image kitShadow(kitShadowPath.string());

        if (kitShadow.isValid()) {
            for (UInt leagueId : leaguesToProcess) {
                auto league = db->GetLeague(leagueId);
                if (league) {
                    league->ForAllTeams([&](FifaTeam &team) {
                        UInt fifaId = team.GetId();
                        if (Utils::Contains(mFifaClubs, fifaId)) {
                            UInt fifamId = mFifaClubs[fifaId];
                            String fifamIdHex = Utils::Format(L"%08X", fifamId);
                            for (auto const &k : kitTypes) {
                                auto srcfilename = fifamIdHex + L"_" + k.second + L".tga";
                                auto kitPath = dataPath / L"data" / L"kits" / srcfilename;
                                if (exists(kitPath)) {
                                    auto dstdir = outputPath / Utils::Format(L"t75__%d_0_%d", k.first, fifaId);
                                    create_directory(dstdir);
                                    Magick::Image kit(kitPath.string());
                                    if (kit.isValid()) {
                                        kit.composite(kitShadow, 0, 0, MagickCore::MultiplyCompositeOp);
                                        kit.write((dstdir / L"tp00.tga").string());
                                        auto armbandPath = dataPath / L"data" / L"kitarmband" / srcfilename;
                                        if (exists(armbandPath))
                                            copy_file(armbandPath, dstdir / L"slvc.tga");
                                    }
                                }
                            }
                        }
                    });
                }
            }
        }
    }
};
