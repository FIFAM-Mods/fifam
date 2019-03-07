#include "Error.h"
#include "Utils.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "Magick++.h"

/*

-input           -i
-output          -o
-minx
-miny
-min2x
-min2y
-inputformat     -if
-outputformat    -of
-quality         -q
-onlypics        -op
-log             -l
-nolog           -nl
-recursive       -r

stadpics -i "D:\pics" -o "D:\Games\FIFA Manager 13" -minx 100 -miny 100 -min2x -1300 -min2y 800 -if "jpg,jpeg,png" -of "jpg" -q 100

*/

Int wmain(Int argc, WideChar **argv) {
    Path input, output, log = L"log_stadpics.csv";
    UInt minx = 530, miny = 400, min2x = 1300, min2y = 800;
    Int quality = -1;
    Vector<String> inputformat = { L"jpg", L"jpeg", L"png", L"bmp" };
    String outputformat = L"jpg";
    Bool onlypics = false, nolog = false, recursive = false;

    SetConsoleTitleW(L"StadPics tool by Dmitry v1.0");

    std::wcout << std::endl << L"Universal Converter Project" << std::endl << L"StadPics tool by Dmitry" << std::endl << L"Version 1.0" << std::endl << std::endl;

    for (Int i = 1; i < argc; i++) {
        if (!_wcsicmp(argv[i], L"-onlypics") || !_wcsicmp(argv[i], L"-op"))
            onlypics = true;
        else if (!_wcsicmp(argv[i], L"-nolog") || !_wcsicmp(argv[i], L"-nl"))
            nolog = true;
        else if (!_wcsicmp(argv[i], L"-recursive") || !_wcsicmp(argv[i], L"-r"))
            recursive = true;
        else if (!_wcsicmp(argv[i], L"-input") || !_wcsicmp(argv[i], L"-i")) {
            if ((i + 1) < argc)
                input = argv[i++ + 1];
            else
                return Error(L"input: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-output") || !_wcsicmp(argv[i], L"-o")) {
            if ((i + 1) < argc)
                output = argv[i++ + 1];
            else
                return Error(L"output: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-minx")) {
            if ((i + 1) < argc)
                minx = Utils::SafeConvertInt<Int>(argv[i++ + 1]);
            else
                return Error(L"minx: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-miny")) {
            if ((i + 1) < argc)
                miny = Utils::SafeConvertInt<Int>(argv[i++ + 1]);
            else
                return Error(L"miny: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-min2x")) {
            if ((i + 1) < argc)
                min2x = Utils::SafeConvertInt<Int>(argv[i++ + 1]);
            else
                return Error(L"min2x: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-min2y")) {
            if ((i + 1) < argc)
                min2y = Utils::SafeConvertInt<Int>(argv[i++ + 1]);
            else
                return Error(L"min2y: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-inputformat") || !_wcsicmp(argv[i], L"-if")) {
            if ((i + 1) < argc)
                inputformat = Utils::Split(argv[i++ + 1], L',', true, true);
            else
                return Error(L"inputformat: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-outputformat") || !_wcsicmp(argv[i], L"-of")) {
            if ((i + 1) < argc)
                outputformat = argv[i++ + 1];
            else
                return Error(L"outputformat: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-quality")) {
            if ((i + 1) < argc)
                quality = Utils::SafeConvertInt<Int>(argv[i++ + 1]);
            else
                return Error(L"quality: expected argument");
        }
        else if (!_wcsicmp(argv[i], L"-log") || !_wcsicmp(argv[i], L"-l")) {
            if ((i + 1) < argc)
                log = argv[i++ + 1];
            else
                return Error(L"log: expected argument");
        }
        else
            return Error(String(argv[i]) + L": unknown command");
    }
    std::wcout << L"input: " << input.c_str() << std::endl;
    std::wcout << L"output: " << output.c_str() << std::endl;
    std::wcout << L"inputformat: " << Utils::Join(inputformat, L',') << std::endl;
    std::wcout << L"outputformat: " << outputformat << std::endl;
    std::wcout << L"recursive: " << (recursive ? "yes" : "no") << std::endl;
    std::wcout << L"onlypics: " << (onlypics ? "yes" : "no") << std::endl;
    std::wcout << L"minx: " << minx << std::endl;
    std::wcout << L"miny: " << miny << std::endl;
    if (!onlypics) {
        std::wcout << L"min2x: " << min2x << std::endl;
        std::wcout << L"min2y: " << min2y << std::endl;
    }
    if (quality != -1)
        std::wcout << L"quality: " << quality << std::endl;
    std::wcout << L"log: " << (nolog? L"nolog" : log.c_str()) << std::endl;

    std::wcout << std::endl;

    if (input.empty())
        return Error(L"input is not set");
    if (output.empty())
        return Error(L"output is not set");

    FifamWriter *logwriter = nullptr;

    try {
        if (!nolog) {
            logwriter = new FifamWriter(log, 14, 0, 0, true);
            logwriter->SetReplaceQuotes(false);
            logwriter->WriteLine(L"FileName", L"FilePath", L"DimX", L"DimY", L"Result");
        }

        Path outputDir = output / L"stadiums";
        if (!exists(outputDir))
            create_directories(outputDir);
        if (!exists(outputDir / L"1920x1200"))
            create_directories(outputDir / L"1920x1200");
        if (!exists(outputDir / L"800x600"))
            create_directories(outputDir / L"800x600");
        if (!exists(outputDir / L"200x150"))
            create_directories(outputDir / L"200x150");
        
        enum ImageResult { NotValid, Ok, TooSmall, TooSmall2x };
        
        auto Log = [&](String const &filename, Path const &filepath, UInt dimX, UInt dimY, ImageResult result) {
            String sresult;
            if (result == NotValid)
                sresult = L"Image is not valid";
            else if (result == Ok)
                sresult = L"Ok";
            else if (result == TooSmall)
                sresult = L"Image is too small";
            else if (result == TooSmall2x)
                sresult = L"Image is too small for 2x version";
            std::wcout << filepath.c_str() << L" (" << dimX << L"x" << dimY << L") - " << sresult << std::endl;
            if (logwriter) {
                String qfilename = L"\"" + filename + L"\"";
                String qfilepath = String(L"\"") + filepath.c_str() + L"\"";
                logwriter->WriteLine(qfilename, qfilepath, dimX, dimY, sresult);
            }
        };
        
        auto ProcessFile = [&](Path const &filepath) {
            for (auto inputext : inputformat) {
                if (!_wcsicmp(filepath.extension().c_str(), String(L"." + inputext).c_str())) {
                    String filename = filepath.filename().c_str();
                    String outfilename = String(filepath.stem().c_str()) + L"." + outputformat;
                    Path outputdir = output / L"stadiums";
                    Magick::Image img(filepath.string());
                    if (!img.isValid()) {
                        Log(filename, filepath, 0, 0, NotValid);
                        return;
                    }
                    if (quality != -1)
                        img.quality(quality);
                    ImageResult result = Ok;
                    if (!onlypics) {
                        Magick::Image img2x(img);
                        if (img2x.baseColumns() >= min2x && img2x.baseRows() >= min2y) {
                            img2x.resize(Magick::Geometry("1920x1200^"));
                            img2x.extent(Magick::Geometry("1920x1200"), Magick::GravityType::CenterGravity);
                            img2x.write(Path(outputdir / L"1920x1200" / outfilename).string());
                        }
                        else
                            result = TooSmall2x;
                    }
                    if (img.baseColumns() >= minx && img.baseRows() >= miny) {
                        img.resize(Magick::Geometry("800x600^"));
                        img.extent(Magick::Geometry("800x600"), Magick::GravityType::CenterGravity);
                        img.write(Path(outputdir / L"800x600" / outfilename).string());
                        img.resize(Magick::Geometry("200x150"));
                        img.write(Path(outputdir / L"200x150" / outfilename).string());
                    }
                    else
                        result = TooSmall;
                    Log(filename, filepath, img.baseColumns(), img.baseRows(), result);
                    return;
                }
            }
        };

        if (recursive) {
            for (auto &p : recursive_directory_iterator(input))
                ProcessFile(p.path());
        }
        else {
            for (auto &p : directory_iterator(input))
                ProcessFile(p.path());
        }
    }
    catch (std::exception &e) {
        Error(e.what());
    }

    delete logwriter;

    return 0;
}
