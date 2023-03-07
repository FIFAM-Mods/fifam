#pragma once
#include "FifamDbEnvironment.h"

class FixCompetitionBadges {
public:
    FixCompetitionBadges() {
        Path gameDir = "D:\\Games\\FIFA Manager 22";
        Path contentDir = "D:\\Projects\\fifam\\content\\fm13";
        Path badgesDir[] = {
           contentDir / "badges_clean\\badges\\Leagues\\256x256",
           contentDir / "badges_clean\\badges\\Leagues\\128x128",
           contentDir / "badges_clean_small\\badges\\Leagues\\64x64",
           contentDir / "badges_clean_small\\badges\\Leagues\\32x32"
        };
        Path output = "output\\new_league_badges";
        Path outputDir[] = {
            output / "256x256",
            output / "128x128",
            output / "64x64",
            output / "32x32",
        };
        for (auto const &d : outputDir)
            create_directories(d);
        Path playOffImagePath = "D:\\Projects\\fifam\\content\\playoff_clean.png";
        Set<UInt> usedComps;
        Set<UInt> relegationComps;
        Set<UInt> leagueSplitComps;
        Set<UInt> specialCupComps;
        FifamWriter log("log.txt");
        {
            {
                FifamReader r(gameDir / "plugins\\ucp\\special_league_cups.csv");
                r.SkipLine();
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        UChar countryId = 0;
                        UChar cupIndex = 0;
                        r.ReadLine(countryId, cupIndex);
                        if (countryId >= 1 && countryId <= 207)
                            specialCupComps.insert(FifamCompID(countryId, FifamCompType::LeagueCup, cupIndex).ToInt());
                    }
                    else
                        r.SkipLine();
                }
            }
            {
                FifamReader r(gameDir / "plugins\\ucp\\league_split.csv");
                r.SkipLine();
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        UChar countryId = r.ReadLine<UChar>();
                        if (countryId >= 1 && countryId <= 207) {
                            leagueSplitComps.insert(FifamCompID(countryId, FifamCompType::Relegation, 0).ToInt());
                            leagueSplitComps.insert(FifamCompID(countryId, FifamCompType::Relegation, 1).ToInt());
                        }
                    }
                    else
                        r.SkipLine();
                }
            }
            FifamDatabase::mReadingOptions.mReadPersons = false;
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, gameDir / "database");
            for (auto const &[id, c] : db.mCompMap) {
                if (id.mRegion == FifamCompRegion::Europe || id.mRegion == FifamCompRegion::SouthAmerica
                    || id.mRegion == FifamCompRegion::NorthAmerica || id.mRegion == FifamCompRegion::Africa
                    || id.mRegion == FifamCompRegion::Asia || id.mRegion == FifamCompRegion::Oceania
                    || id.mRegion == FifamCompRegion::International)
                {
                    usedComps.insert((id.ToInt() >> 16) & 0xFFFF);
                }
                else
                    usedComps.insert(id.ToInt());
                if (id.mType == FifamCompType::Relegation && !leagueSplitComps.contains(id.ToInt()))
                    relegationComps.insert(id.ToInt());
            }
        }
        // remove unused badges
        /*
        for (auto const &d : badgesDir) {
            for (auto const &i : directory_iterator(d)) {
                Path p = i.path();
                if (p.extension() == ".png") {
                    String idStr = p.stem().c_str();
                    if (idStr != L"00000000") {
                        UInt id = 0;
                        if (idStr.contains(L'_')) {
                            auto idParts = Utils::Split(idStr, L'_');
                            if (idParts.size() > 1)
                                id = Utils::SafeConvertInt<UInt>(idParts[1], true);
                        }
                        else
                            id = Utils::SafeConvertInt<UInt>(idStr, true);
                        if (id != 0) {
                            if (!usedComps.contains(id))
                                std::wcout << L"removed " << p.filename().c_str() << std::endl;
                        }
                        else {
                            ::Error(L"Unable to read ID: " + idStr);
                            log.WriteLine(L"Unable to read ID: " + idStr);
                        }
                    }
                }
            }
        }
        */
        // create relegation play-off badges
        for (UInt id : relegationComps) {
            String filename = Utils::Format(L"%08X.png", id);
            for (unsigned int d = 0; d < 4; d++)
                copy(playOffImagePath, outputDir[d] / filename, copy_options::overwrite_existing);
            std::wcout << L"created play-off badge " << filename << std::endl;
            log.WriteLine(L"created play-off badge " + filename);
        }
        // create league split badges
        for (UInt id : leagueSplitComps) {
            String dstFilename = Utils::Format(L"%08X.png", id);
            String srcFilename = FifamCompID((id >> 24) & 0xFF, FifamCompType::League, 0).ToHexStr() + L".png";
            bool cantFind = false;
            for (unsigned int d = 0; d < 4; d++) {
                if (exists(badgesDir[d] / srcFilename))
                    copy(badgesDir[d] / srcFilename, outputDir[d] / dstFilename, copy_options::overwrite_existing);
                else
                    cantFind = true;
            }
            if (cantFind) {
                std::wcout << L"can't find league badge " << srcFilename << std::endl;
                log.WriteLine(L"can't find league badge " + srcFilename);
            }
            else {
                std::wcout << L"created league split badge " << dstFilename << std::endl;
                log.WriteLine(L"created league split badge " + dstFilename);
            }
        }
        // create special cup badge
        for (UInt id : specialCupComps) {
            String dstFilename = Utils::Format(L"%08X.png", id);
            String srcFilename = FifamCompID((id >> 24) & 0xFF, FifamCompType::League, 0).ToHexStr() + L".png";
            if (exists(badgesDir[0] / dstFilename)) {
                std::wcout << L"special cup badge already exists " << dstFilename << std::endl;
                log.WriteLine(L"special cup badge already exists " + dstFilename);
            }
            else {
                bool cantFind = false;
                for (unsigned int d = 0; d < 4; d++) {
                    if (exists(badgesDir[d] / srcFilename))
                        copy(badgesDir[d] / srcFilename, outputDir[d] / dstFilename, copy_options::overwrite_existing);
                    else
                        cantFind = true;
                }
                if (cantFind) {
                    std::wcout << L"can't find league badge " << srcFilename << std::endl;
                    log.WriteLine(L"can't find league badge " + srcFilename);
                }
                else {
                    std::wcout << L"created special cup badge " << dstFilename << std::endl;
                    log.WriteLine(L"created special cup badge " + dstFilename);
                }
            }
        }
    }
};
