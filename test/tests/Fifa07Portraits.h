#pragma once
#include "FifamDbEnvironment.h"
#include "FifamClubBadge.h"
#include "Error.h"
#include "FifamNames.h"
#include <FifaDataFile.h>

class Fifa07Portraits {
public:
    static void FixName(String &name) {
        static Map<WideChar, WideChar> chars = {
        { L'А', 0xC0 },
        { L'Б', 0xC1 },
        { L'В', 0xC2 },
        { L'Г', 0xC3 },
        { L'Д', 0xC4 },
        { L'Е', 0xC5 },
        { L'Ж', 0xC6 },
        { L'З', 0xC7 },
        { L'И', 0xC8 },
        { L'Й', 0xC9 },
        { L'К', 0xCA },
        { L'Л', 0xCB },
        { L'М', 0xCC },
        { L'Н', 0xCD },
        { L'О', 0xCE },
        { L'П', 0xCF },
        { L'Р', 0xD0 },
        { L'С', 0xD1 },
        { L'Т', 0xD2 },
        { L'У', 0xD3 },
        { L'Ф', 0xD4 },
        { L'Х', 0xD5 },
        { L'Ц', 0xD6 },
        { L'Ч', 0xD7 },
        { L'Ш', 0xD8 },
        { L'Щ', 0xD9 },
        { L'Ъ', 0xDA },
        { L'Ы', 0xDB },
        { L'Ь', 0xDC },
        { L'Э', 0xDD },
        { L'Ю', 0xDE },
        { L'Я', 0xDF },
        { L'а', 0xE0 },
        { L'б', 0xE1 },
        { L'в', 0xE2 },
        { L'г', 0xE3 },
        { L'д', 0xE4 },
        { L'е', 0xE5 },
        { L'ж', 0xE6 },
        { L'з', 0xE7 },
        { L'и', 0xE8 },
        { L'й', 0xE9 },
        { L'к', 0xEA },
        { L'л', 0xEB },
        { L'м', 0xEC },
        { L'н', 0xED },
        { L'о', 0xEE },
        { L'п', 0xEF },
        { L'р', 0xF0 },
        { L'с', 0xF1 },
        { L'т', 0xF2 },
        { L'у', 0xF3 },
        { L'ф', 0xF4 },
        { L'х', 0xF5 },
        { L'ц', 0xF6 },
        { L'ч', 0xF7 },
        { L'ш', 0xF8 },
        { L'щ', 0xF9 },
        { L'ъ', 0xFA },
        { L'ы', 0xFB },
        { L'ь', 0xFC },
        { L'э', 0xFD },
        { L'ю', 0xFE },
        { L'я', 0xFF },
        };
        for (WideChar &c : name) {
            if (chars.contains(c))
                c = chars[c];
        }
    }

    static void Rename2() {
        Path baseFolder = L"I:\\fifa07_portraits";
        for (UInt i = 0; i < 2; i++) {
            FifamDatabase *db = nullptr;
            if (i == 0)
                db = GetEnvironment<FifamDbEnvironment<FM07, Default>>().GetDatabase();
            else
                db = GetEnvironment<FifamDbEnvironment<FM08, Default>>().GetDatabase();
            Path folder = i == 0 ? (baseFolder / "fm07") : (baseFolder / "fm08");
            for (FifamPlayer *p : db->mPlayers) {
                if (i == 0) {
                    FixName(p->mFirstName);
                    FixName(p->mLastName);
                    FixName(p->mPseudonym);
                }
                UInt targetId = p->mSpecialFace;
                if (targetId == 0)
                    targetId = p->mID;
                bool found = false;
                if (p->mEmpicsId != 0) {
                    Path filename = folder / (FifamNames::GetPersonStringId(7 + i, p->mFirstName, p->mLastName, p->mPseudonym, p->mBirthday, p->mEmpicsId) + L".png");
                    if (exists(filename)) {
                        rename(filename, folder / (Utils::Format(L"%u", targetId) + L".png"));
                        found = true;
                    }
                }
                if (!found) {
                    Path filename = folder / (FifamNames::GetPersonStringId(7 + i, p->mFirstName, p->mLastName, p->mPseudonym, p->mBirthday, 0) + L".png");
                    if (exists(filename))
                        rename(filename, folder / (Utils::Format(L"%u", targetId) + L".png"));
                }
            }
        }
    }

    static void Build() {
        bool hd = true;
        Int mp = hd ? 2 : 1;
        Path gameDir = L"D:\\Games\\FIFA 07";
        Path workingDir = L"I:\\fifa07_portraits";
        Path outputDir = workingDir / (hd? "output_hd" : "output"); // gameDir / L"data\\gui\\assets\\heads";
        create_directories(outputDir);
        Path portraitsDir = "I:\\FIFA_ASSETS\\PC\\miniface\\FIFA 23 PC MINIFACES PNG1"; // workingDir / L"fm07";
        Path tempDir = workingDir / L"temp";
        Path bigDir = workingDir / L"big";
        create_directories(tempDir);
        Path otoolsPath = L"D:\\Projects\\otools\\output\\otools-release-0175\\otools.exe";
        Path packbigPath = L"D:\\Projects\\fifam\\content\\big_from_dir.exe";
        Map<Int, Path> mapPaths;
        for (auto const &i : directory_iterator(portraitsDir)) {
            auto const &p = i.path();
            if (is_regular_file(p) && p.extension() == L".png") {
                String filename = p.stem().c_str();
                if (!filename.empty() && !filename.starts_with(L"notfound")) {
                    if (filename[0] == L'p')
                        filename = filename.substr(1);
                    Int playerid = Utils::SafeConvertInt<Int>(filename);
                    //if (playerid >= 5533)
                    mapPaths[playerid] = p;
                }
            }
        }
        for (auto const &[id, p] : mapPaths) {
            try {
                String sFilename = L"s_" + to_wstring(id);
                Path outputBig = outputDir / (sFilename + L".big");
                if (!exists(outputBig)) {
                    Path buildDir = tempDir / sFilename;
                    create_directories(buildDir);
                    Magick::Image img(p.string());
                    img.interpolate(MagickCore::BilinearInterpolatePixel);
                    img.filterType(FilterType::HermiteFilter);
                    img.resize(Magick::Geometry(85 * mp, 85 * mp));
                    Magick::Image bottom(img, Magick::Geometry(85 * mp, 1, 0, 85 * mp - 1));
                    Magick::Geometry bottomGeom(85 * mp, 43 * mp);
                    bottomGeom.aspect(true);
                    bottom.interpolate(MagickCore::NearestInterpolatePixel);
                    bottom.filterType(MagickCore::PointFilter);
                    bottom.resize(bottomGeom);
                    img.extent(Magick::Geometry(100 * mp, 128 * mp), Magick::Color(0, 0, 0, 0), MagickCore::NorthGravity);
                    bottom.extent(Magick::Geometry(100 * mp, bottom.rows()), Magick::Color(0, 0, 0, 0), MagickCore::NorthGravity);
                    img.composite(bottom, Magick::Geometry(bottom.columns(), bottom.rows(), 0, 85 * mp), MagickCore::OverCompositeOp);
                    img.extent(Magick::Geometry(128 * mp, 128 * mp), Magick::Color(0, 0, 0, 0), MagickCore::WestGravity);
                    Path imgFilename = buildDir / "1   .png";
                    img.write(imgFilename.string());
                    copy(bigDir / L"s_4098.apt", buildDir / (sFilename + L".apt"), copy_options::overwrite_existing);
                    copy(bigDir / L"s_4098.const", buildDir / (sFilename + L".const"), copy_options::overwrite_existing);
                    if (hd)
                        copy(bigDir / L"s_4098_hd.o", buildDir / (sFilename + L".o"), copy_options::overwrite_existing);
                    else
                        copy(bigDir / L"s_4098.o", buildDir / (sFilename + L".o"), copy_options::overwrite_existing);
                    STARTUPINFOW info;
                    PROCESS_INFORMATION processInfo;
                    RtlZeroMemory(&info, sizeof(info));
                    RtlZeroMemory(&processInfo, sizeof(processInfo));
                    info.cb = sizeof(info);
                    WideChar commandStr[2048];
                    wcscpy(commandStr, L"otools packfsh -fshFormat dxt5 -fshLevels 1 -i \"");
                    wcscat(commandStr, buildDir.c_str());
                    wcscat(commandStr, L"\"");
                    if (CreateProcessW(otoolsPath.c_str(), commandStr, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
                        WaitForSingleObject(processInfo.hProcess, INFINITE);
                        CloseHandle(processInfo.hProcess);
                        CloseHandle(processInfo.hThread);
                        remove(imgFilename);
                        STARTUPINFOW info2;
                        PROCESS_INFORMATION processInfo2;
                        RtlZeroMemory(&info2, sizeof(info2));
                        RtlZeroMemory(&processInfo2, sizeof(processInfo2));
                        info2.cb = sizeof(info2);
                        wcscpy(commandStr, L"big_from_dir \"");
                        wcscat(commandStr, outputBig.c_str());
                        wcscat(commandStr, L"\" \"");
                        wcscat(commandStr, buildDir.c_str());
                        wcscat(commandStr, L"\"");
                        if (CreateProcessW(packbigPath.c_str(), commandStr, NULL, NULL, TRUE, 0, NULL, NULL, &info2, &processInfo2)) {
                            WaitForSingleObject(processInfo2.hProcess, INFINITE);
                            CloseHandle(processInfo2.hProcess);
                            CloseHandle(processInfo2.hThread);
                        }
                    }
                    remove_all(buildDir);
                }
            }
            catch (exception e) {
                ::Error(p.string() + "\n" + e.what());
            }
        }
        remove_all(tempDir);
    }

    static void Rename() {
        Path baseFolder = L"I:\\fifa07_portraits";
        for (UInt i = 0; i < 2; i++) {
            Path folder = i == 0 ? (baseFolder / "fm07") : (baseFolder / "fm08");
            //Path playersFilename = L"D:\\Projects\\fifa07tool\\output\\remaster\\fifa\\players.txt";
            Path playersFilename = L"D:\\Projects\\fifam\\db\\fifa\\08\\players.txt";
            FifaDataFile file;
            String firstname, surname;
            Int playerid, birthdate;
            if (file.Open(playersFilename)) {
                for (FifaDataFile::Line line; file.NextLine(line); ) {
                    line >> playerid >> firstname >> surname >> birthdate;
                    if (playerid != 0) {
                        String pseudonym;
                        if (firstname.empty())
                            pseudonym = surname;
                        FifaDate fifaDate;
                        fifaDate.Set(birthdate);
                        Path filename = folder / (FifamNames::GetPersonStringId(7 + i, firstname, surname, pseudonym, FifamDate(fifaDate.day, fifaDate.month, fifaDate.year), 0) + L".png");
                        if (exists(filename))
                            rename(filename, folder / (Utils::Format(L"%u", playerid) + L".png"));
                    }
                }
                file.Close();
            }
        }
    }

    static void WithoutPortrait() {
        Path playersFilename = L"D:\\Projects\\fifa07tool\\output\\remaster\\fifa\\players.txt";
        Path baseFolder = L"I:\\fifa07_portraits\\fm07_done";
        FifaDataFile file;
        String firstname, surname;
        Int playerid, birthdate;
        Int originalname, height, weight, preferredposition1, preferredfoot, facetypeid, haircolorid, hairtypeid, facialhairstyleid, legtypeid, sleevelength, accessoryid1, accessorycolor1, accessoryid2, accessorycolor2, accessoryid3, accessorycolor3, accessoryid4, accessorycolor4, accessoryid5, accessorycolor5, shoecolor, injuryprone, acceleration, aggression, sprintspeed, stamina, strength, marking, tackling, ballcontrol, dribbling, crossing, heading, shortpassing, longpassing, longshotaccuracy, finishing, shotpower, reactions, reflexes, handling, gkdiving, gkpositioning, playercomposure, freekicks, flexibility, adaptability, ambition, fansfavourite, highclubidentification, lowclubidentification, mediadarling, scandalprone, hightrainingworkrate, lowtrainingworkrate, inflexible, assetid, potential;
        if (file.Open(playersFilename)) {
            Vector<Tuple<UInt, String, UInt, FifaDate>> vec;
            for (FifaDataFile::Line line; file.NextLine(line); ) {
                line >> playerid >> firstname >> surname >> birthdate >> originalname >> height >> weight >> preferredposition1 >> preferredfoot >> facetypeid >> haircolorid >> hairtypeid >> facialhairstyleid >> legtypeid >> sleevelength >> accessoryid1 >> accessorycolor1 >> accessoryid2 >> accessorycolor2 >> accessoryid3 >> accessorycolor3 >> accessoryid4 >> accessorycolor4 >> accessoryid5 >> accessorycolor5 >> shoecolor >> injuryprone >> acceleration >> aggression >> sprintspeed >> stamina >> strength >> marking >> tackling >> ballcontrol >> dribbling >> crossing >> heading >> shortpassing >> longpassing >> longshotaccuracy >> finishing >> shotpower >> reactions >> reflexes >> handling >> gkdiving >> gkpositioning >> playercomposure >> freekicks >> flexibility >> adaptability >> ambition >> fansfavourite >> highclubidentification >> lowclubidentification >> mediadarling >> scandalprone >> hightrainingworkrate >> lowtrainingworkrate >> inflexible >> assetid >> potential;
                if (playerid > 0 && playerid < 400'000) {
                    if (!exists(baseFolder / (Utils::Format(L"%u", playerid) + L".png"))) {
                        String pname = surname;
                        if (!firstname.empty())
                            pname = firstname + L" " + surname;
                        FifaDate date;
                        date.Set(birthdate);
                        vec.emplace_back(playerid, pname, potential, date);
                    }
                }
            }
            file.Close();
            FifamWriter w("players_without_portrait_07.csv");
            std::sort(vec.begin(), vec.end(), [](Tuple<UInt, String, UInt, FifaDate> const &a, Tuple<UInt, String, UInt, FifaDate> const &b) {
                return get<2>(a) >= get<2>(b);
            });
            for (auto const &i : vec) {
                w.WriteLine(get<0>(i), get<1>(i), get<2>(i), get<3>(i).ToString(), 2006 - get<3>(i).year);
            }
        }
    }

    Fifa07Portraits() {
        Build();
    }
};
