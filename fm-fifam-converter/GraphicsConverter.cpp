#include "GraphicsConverter.h"
#include "FifamNames.h"
#include "Magick++.h"

using namespace Magick;

const wchar_t *countryLeagueNames[208] = { L"Germany",L"Albanien",L"Andorra",L"Armenien",L"Austria",L"Aserbaidschan",L"Belarus",L"Belgium",L"Bosnienherzegowina",L"Bulgarien",L"Kroatien",L"Zypern",L"Tschechien",L"Denmark",L"England",L"Estland",L"Faroeislands",L"Finnland",L"France",L"Mazedonien",L"Georgien",L"Germany",L"Griechenland",L"Ungarn",L"Island",L"Ireland",L"Israel",L"Italy",L"Latvia",L"Liechtenstein",L"Lithuania",L"Luxemburg",L"Malta",L"Moldawien",L"Netherland",L"Nordirland",L"Norway",L"Polen",L"Portugal",L"Rumaenien",L"Russland",L"Sanmarino",L"Scotland",L"Slowakei",L"Slowenien",L"Spain",L"Sweden",L"Switzerland",L"Tuerkei",L"Ukraine",L"Wales",L"Serbia",L"Argentinien",L"Bolivien",L"Brazil",L"Chile",L"Kolumbien",L"Ecuador",L"Paraguay",L"Peru",L"Uruguay",L"Venezuela",L"Anguilla",L"Antigua",L"Aruba",L"Bahamas",L"Barbados",L"Belize",L"Bermuda",L"Britishvirginisles",L"Kanada",L"Caymanisles-Blue",L"Costarica",L"Kuba",L"Dominica",L"Dominikanischerep",L"Elsalvador",L"Grenada",L"Guatemala",L"Guyana",L"Haiti",L"Honduras",L"Jamaika",L"Mexiko",L"Montseratsm",L"Netherlands",L"Nicaragua",L"Panama",L"Puertorico",L"Stkittsnevi",L"SaIntlucia",L"SaIntvincent",L"Surinam",L"Trinidad",L"Turks",L"Usa",L"Us-Virginis",L"Algerien",L"Angola",L"Benin",L"Botswana",L"Burkinafaso",L"Burundi",L"Kamerun",L"Kapverde",L"Centafrepublic",L"Chad",L"Kongobr",L"Cotedivoire",L"Dschibuti",L"Congo",L"Aegypten",L"Equatorialguinea",L"Eritrea",L"Ethiopia",L"Gabun",L"Gambia",L"Ghana",L"Guinea",L"Guinbissau",L"Kenia",L"Lesotho",L"Liberia",L"Libyen",L"Madagaskar",L"Malawi",L"Mali",L"Mauretanien",L"Mauritius",L"Marokko",L"Mosambik",L"Namibia",L"Niger",L"Nigeria",L"Rwanda",L"Saotome",L"Senegal",L"Seychellen",L"Sierraleone",L"Somalia",L"Suedafrika",L"Sudan",L"Swasiland",L"Tansania",L"Togo",L"Tunesien",L"Uganda",L"Sambia",L"Simbabwe",L"Afghanistan",L"Bahrain",L"Bangladesch",L"Bhutan",L"Brunei",L"Cambodia",L"China",L"Taiwan",L"Guam",L"Hongkong",L"Indien",L"Indonesien",L"Iran",L"Irak",L"Japan",L"Jordan",L"Kasachstan",L"Korea",L"Korea",L"Kuwait",L"Kyrgystan",L"Laos",L"Libanon",L"Macau",L"Malaysia",L"Maldives",L"Mongolei",L"Myanmar",L"Nepal",L"Oman",L"Pakistan",L"Palestines",L"Philippinen",L"Qatar",L"Saudiarabien",L"Singapur",L"Srilanka",L"Syrien",L"Tadschikistan",L"Thailand",L"Turkmenistan",L"Vereinigtearabischeemirate",L"Usbekistan",L"Vietnam",L"Yemen",L"Americasamoa",L"Australien",L"Cookislands",L"Fiji",L"Neuseeland",L"Papuanewguinea",L"Samoa",L"Solomon",L"Tahiti",L"Tonga",L"Vanuatu",L"Gibralter",L"Montenegro",L"Greenlands" };
GraphicsConverter::GraphicsConverter() {
    InitializeMagick(NULL);
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
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath? (leagues ? L"Leagues256" : L"Badge256") : L"256x256") / badgeName).string()));
    badgeImg.resize(Geometry(128, 128));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues128" : L"Badge128") : L"128x128") / badgeName).string()));
    badgeImg.resize(Geometry(64, 64));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues64" : L"Badge64") : L"64x64") / badgeName).string()));
    badgeImg.resize(Geometry(32, 32));
    SafeWriteImage(badgeImg, (Path(outputPath / (oldBadgePath ? (leagues ? L"Leagues32" : L"Badge32") : L"32x32") / badgeName).string()));
}

Bool ConvertOneCompBadge(Path const &badgePath, Path const &outputPath, String const &badgeName, UInt gameId) {
    if (exists(badgePath)) {
        Image badgeImg(badgePath.string());
        if (badgeImg.isValid() && badgeImg.baseRows() == 360) {
            if (badgeImg.baseColumns() != badgeImg.baseRows()) {
                UInt newSize = Utils::Max(badgeImg.baseColumns(), badgeImg.baseRows());
                badgeImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
            }
            WriteOneBadge(badgeImg, outputPath, badgeName, gameId, true);
            return true;
        }
    }
    return false;
}

void GraphicsConverter::ConvertClubBadges(foom::db *db, Map<Int, Path> const &availableBadges, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"clubs";
    for (auto e : db->mClubs) {
        auto &club = e.second;
        if (club.mReputation > minRep && club.mConverterData.mFifamClub) {
            FifamClub *fifamClub = (FifamClub *)club.mConverterData.mFifamClub;
            auto it = availableBadges.find(club.mID);
            if (it != availableBadges.end()) {
                Image badgeImg((*it).second.string());
                if (badgeImg.isValid() && badgeImg.baseRows() >= 360) {
                    if (badgeImg.baseColumns() != badgeImg.baseRows()) {
                        UInt newSize = Utils::Max(badgeImg.baseColumns(), badgeImg.baseRows());
                        badgeImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    }
                    WriteOneBadge(badgeImg, outputPath, Utils::Format(L"%08X.tga", fifamClub->mUniqueID), gameId);
                }
            }
        }
    }
}

void GraphicsConverter::ConvertCompBadges(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep) {
    Map<Int, Path> availableBadges;
    Path foomBadgesPath = fmGraphicsPath / L"dvx-logos" / L"competitions" / L"primary" / L"@2x";
    for (auto const &i : recursive_directory_iterator(foomBadgesPath)) {
        Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
        if (id > 0 && i.path().extension() == ".png")
            availableBadges[id] = i.path();
    }
    Map<Int, Path> availableFlags;
    for (auto const &i : recursive_directory_iterator(fmGraphicsPath / L"dvx-logos" / L"flags" / L"primary" / L"@2x")) {
        Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
        if (id > 0 && i.path().extension() == ".png")
            availableFlags[id] = i.path();
    }
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"Leagues";
    for (auto e : db->mCompMap) {
        auto &comp = e.second;
        if (comp->GetDbType() == FifamCompDbType::League || (gameId >= 10 && comp->GetDbType() == FifamCompDbType::Cup)) {
            if (comp->GetProperty<Int>(L"foom::reputation") >= minRep) {
                Bool badgeCreated = false;
                String badgeName;
                if (gameId >= 10)
                    badgeName = Utils::Format(L"%08X.tga", e.first.ToInt());
                else if (e.first.mRegion.ToInt() > 0 && e.first.mRegion.ToInt() <= 207)
                    badgeName = Utils::Format(L"%s%d.tga", countryLeagueNames[e.first.mRegion.ToInt()], comp->mCompetitionLevel + 1);
                else
                    continue;
                if (gameId >= 10 || comp->GetProperty<Int>(L"NumLeaguesOnLevel") == 1) {
                    auto it = availableBadges.find(comp->GetProperty<Int>(L"foom::id"));
                    if (it != availableBadges.end())
                        badgeCreated = ConvertOneCompBadge((*it).second.string(), outputPath, badgeName, gameId);
                }
                if (!badgeCreated) {
                    FifamCountry *country = db->GetCountry(e.first.mRegion.ToInt());
                    if (country) {
                        foom::nation *nation = country->GetProperty<foom::nation *>(L"foom::nation", nullptr);
                        if (nation) {
                            auto nit = availableFlags.find(nation->mID);
                            if (nit != availableFlags.end()) {
                                Image flagImg((*nit).second.string());
                                if (flagImg.isValid()) {
                                    if (comp->mID.mType == FifamCompType::League) {
                                        if (comp->mID.mIndex == 0) {
                                            Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                            outputImg.composite(flagImg, -52, 18, OverCompositeOp);
                                            Image maskImg(Path(contentPath / L"templates" / L"league_mask.png").string());
                                            outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                            Image frameImg(Path(contentPath / L"templates" / L"league_frame.png").string());
                                            outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                            WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                        }
                                        else if (comp->mID.mIndex == 1) {
                                            Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                            outputImg.composite(flagImg, -52, 18, OverCompositeOp);
                                            Image maskImg(Path(contentPath / L"templates" / L"league2_mask.png").string());
                                            outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                            Image frameImg(Path(contentPath / L"templates" / L"league2_frame.png").string());
                                            outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                            WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                        }
                                        else if (comp->mID.mIndex == 2) {
                                            Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                            flagImg.resize(Geometry(306, 187));
                                            outputImg.composite(flagImg, -25, 34, OverCompositeOp);
                                            Image maskImg(Path(contentPath / L"templates" / L"league3_mask.png").string());
                                            outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                            Image frameImg(Path(contentPath / L"templates" / L"league3_frame.png").string());
                                            outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                            WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                        }
                                        else if (comp->mID.mIndex == 3) {
                                            Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                            flagImg.resize(Geometry(306, 187));
                                            outputImg.composite(flagImg, -25, 34, OverCompositeOp);
                                            Image maskImg(Path(contentPath / L"templates" / L"league4_mask.png").string());
                                            outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                            Image frameImg(Path(contentPath / L"templates" / L"league4_frame.png").string());
                                            outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                            WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                        }
                                        else {
                                            Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                            flagImg.resize(Geometry(306, 187));
                                            outputImg.composite(flagImg, -25, 35, OverCompositeOp);
                                            Image maskImg(Path(contentPath / L"templates" / L"league5_mask.png").string());
                                            outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                            Image frameImg(Path(contentPath / L"templates" / L"league5_frame.png").string());
                                            outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                            WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                        }
                                    }
                                    else if (comp->mID.mType == FifamCompType::FaCup) {
                                        Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                        flagImg.resize(Geometry(172, 105));
                                        outputImg.composite(flagImg, 42, 139, OverCompositeOp);
                                        Image maskImg(Path(contentPath / L"templates" / L"cup_mask.png").string());
                                        outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                        Image frameImg(Path(contentPath / L"templates" / L"cup_frame.png").string());
                                        outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                        WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                    }
                                    else if (comp->mID.mType == FifamCompType::LeagueCup) {
                                        Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                        flagImg.resize(Geometry(299, 183));
                                        outputImg.composite(flagImg, -21, 30, OverCompositeOp);
                                        Image maskImg(Path(contentPath / L"templates" / L"le_cup_mask.png").string());
                                        outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                        Image frameImg(Path(contentPath / L"templates" / L"le_cup_frame.png").string());
                                        outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                        WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                    }
                                    else if (comp->mID.mType == FifamCompType::SuperCup) {
                                        Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                        flagImg.resize(Geometry(159, 97));
                                        outputImg.composite(flagImg, 48, 109, OverCompositeOp);
                                        Image maskImg(Path(contentPath / L"templates" / L"supercup_mask.png").string());
                                        outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                        Image frameImg(Path(contentPath / L"templates" / L"supercup_frame.png").string());
                                        outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                        WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (gameId >= 10) { // logos for international competitions are not present in FM07-FM09 (trophy images are used instead)
        ConvertOneCompBadge(foomBadgesPath / L"90_uefa.png", outputPath, L"FF24.tga", gameId); // UEFA Nations League
        ConvertOneCompBadge(foomBadgesPath / L"95_uefa.png", outputPath, L"FF10.tga", gameId); // European Qualifiers
        ConvertOneCompBadge(foomBadgesPath / L"1301385_fifa.png", outputPath, L"2018_FF11.tga", gameId); // FIFA World Cup 2018
        ConvertOneCompBadge(foomBadgesPath / L"121092_fifa.png", outputPath, L"2018_FF0D.tga", gameId); // FIFA World Club Cup 2018
        ConvertOneCompBadge(foomBadgesPath / L"1301389_conmebol.png", outputPath, L"2019_FF21.tga", gameId); // Copa America 2019
        ConvertOneCompBadge(foomBadgesPath / L"1301388_uefa.png", outputPath, L"2020_FF12.tga", gameId); // UEFA Euro 2020
        ConvertOneCompBadge(foomBadgesPath / L"1301394_uefa.png", outputPath, L"F909.tga", gameId); // UEFA Champions League
        ConvertOneCompBadge(foomBadgesPath / L"1301396_uefa.png", outputPath, L"F90A.tga", gameId); // UEFA Europa League
        ConvertOneCompBadge(foomBadgesPath / L"1301397_uefa.png", outputPath, L"F90C.tga", gameId); // UEFA Super Cup
        ConvertOneCompBadge(foomBadgesPath / L"102415_conmebol.png", outputPath, L"FA09.tga", gameId); // South America Champions League
        ConvertOneCompBadge(foomBadgesPath / L"317567_conmebol.png", outputPath, L"FA0A.tga", gameId); // South America Europa League
        ConvertOneCompBadge(foomBadgesPath / L"102418_conmebol.png", outputPath, L"FA0C.tga", gameId); // South America Super Cup
        ConvertOneCompBadge(foomBadgesPath / L"51002641_concacaf.png", outputPath, L"FB09.tga", gameId); // North America Champions League
        ConvertOneCompBadge(foomBadgesPath / L"127299_caf.png", outputPath, L"FC09.tga", gameId); // Africa Champions League
        ConvertOneCompBadge(foomBadgesPath / L"12017574_caf.png", outputPath, L"FC0A.tga", gameId); // Africa Europa League
        ConvertOneCompBadge(foomBadgesPath / L"131273_caf.png", outputPath, L"FC0C.tga", gameId); // Africa Super Cup
        ConvertOneCompBadge(foomBadgesPath / L"127286_afc.png", outputPath, L"FD09.tga", gameId); // Asia Champions League
        ConvertOneCompBadge(foomBadgesPath / L"1001959_afc.png", outputPath, L"FD0A.tga", gameId); // Asia Europa League
        ConvertOneCompBadge(foomBadgesPath / L"127285_ofc.png", outputPath, L"FE09.tga", gameId); // Oceania Champions League
        ConvertOneCompBadge(contentPath / L"icc.png", outputPath, L"F923.tga", gameId); // International Champions Cup
    }
}

String GetTrophyNameForFM09(FifamCompID const &id) {
    switch (id.mType.ToInt()) {
    case FifamCompType::League:
        switch (id.mRegion.ToInt()) {
        case FifamCompRegion::Austria:
            if (id.mIndex == 0)
                return L"austria/austria_tmobile_bundesliga";
            break;
        case FifamCompRegion::Belgium:
            if (id.mIndex == 0)
                return L"belgium/coupe_de_belgique";
            break;
        case FifamCompRegion::Denmark:
            if (id.mIndex == 0)
                return L"denmark/DanishSuperligaen";
            break;
        case FifamCompRegion::England:
            if (id.mIndex == 0)
                return L"england/FA_premier_league";
            if (id.mIndex == 1)
                return L"england/english_championship";
            if (id.mIndex == 2)
                return L"england/english_league_1";
            if (id.mIndex == 3)
                return L"england/english_league_2";
            break;
        case FifamCompRegion::France:
            if (id.mIndex == 0)
                return L"france/ligue_1_orange";
            if (id.mIndex == 1)
                return L"france/ligue_2_orange";
            break;
        case FifamCompRegion::Germany:
            if (id.mIndex == 0)
                return L"germany/german_bundesliga";
            break;
        case FifamCompRegion::Scotland:
            if (id.mIndex == 0)
                return L"scotland/scottish_premier";
            break;
        case FifamCompRegion::Spain:
            if (id.mIndex == 0)
                return L"spain/spain_primera";
            break;
        case FifamCompRegion::Switzerland:
            if (id.mIndex == 0)
                return L"switzerland/swiss_axpo_super_league";
            break;
        case FifamCompRegion::Korea_Republic:
            if (id.mIndex == 0)
                return L"korea/k-league";
            break;
        }
        break;
    case FifamCompType::FaCup:
        switch (id.mRegion.ToInt()) {
        case FifamCompRegion::Austria:
            if (id.mIndex == 0)
                return L"austria/austria_OFB";
            break;
        case FifamCompRegion::Belgium:
            if (id.mIndex == 0)
                return L"belgium/BelgianFaCup";
            break;
        case FifamCompRegion::Denmark:
            if (id.mIndex == 0)
                return L"denmark/DongCupDBUs";
            break;
        case FifamCompRegion::England:
            if (id.mIndex == 0)
                return L"england/english_cup";
            break;
        case FifamCompRegion::Germany:
            if (id.mIndex == 0)
                return L"germany/german_dfbpokal";
            break;
        case FifamCompRegion::Norway:
            if (id.mIndex == 0)
                return L"norway/Norwegen";
            break;
        case FifamCompRegion::Poland:
            if (id.mIndex == 0)
                return L"poland/PucharPolski";
            break;
        case FifamCompRegion::Portugal:
            if (id.mIndex == 0)
                return L"portugal/Taca_de_Portugal";
            break;
        case FifamCompRegion::United_States:
            if (id.mIndex == 0)
                return L"amerika/MLSOpenCup";
            break;
        }
        break;
    case FifamCompType::LeagueCup:
        switch (id.mRegion.ToInt()) {
        case FifamCompRegion::England:
            if (id.mIndex == 0)
                return L"england/english_league_cup";
            else if (id.mIndex == 1)
                return L"england/Johnstones_Paint_Trophy";
            break;
        case FifamCompRegion::France:
            if (id.mIndex == 0)
                return L"france/coupe_de_la_ligue";
            break;
        case FifamCompRegion::Germany:
            if (id.mIndex == 0)
                return L"germany/german_ligapokal";
            break;
        }
        break;
    case FifamCompType::SuperCup:
        switch (id.mRegion.ToInt()) {
        case FifamCompRegion::England:
            if (id.mIndex == 0)
                return L"england/UK_community_shield";
            break;
        case FifamCompRegion::France:
            if (id.mIndex == 0)
                return L"france/Trophee_des_Champions";
            break;
        case FifamCompRegion::Norway:
            if (id.mIndex == 0)
                return L"norway/norway_tripple";
            break;
        }
        break;
    }
    return String();
}

Bool ConvertOneTrophy(Path const &trophyPath, Path const &outputPath, Path const &trophyNamePath, String const &trophyRoomFolder) {
    if (exists(trophyPath)) {
        Image trophyImg(trophyPath.string());
        if (trophyImg.isValid()) {
            bool result = false;
            if (trophyImg.isValid()) {
                auto WriteImage = [](Image &img, Path const &filepath) {
                    if (!exists(filepath.parent_path()))
                        create_directories(filepath.parent_path());
                    SafeWriteImage(img, filepath.string());
                };
                {
                    auto[newX, newY] = ResizeAndFit(trophyImg.baseColumns(), trophyImg.baseRows(), 518, 410, 0.78f);
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
                    auto[newX, newY] = ResizeAndFit(trophyImg.baseColumns(), trophyImg.baseRows(), 244, 244, 0.88f);
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

void GraphicsConverter::ConvertTrophies(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    String trophyRoomFolder = L"534x423";
    Path artArchiveForLeagues = L"art_05";
    if (gameId <= 12) {
        artArchiveForLeagues = L"art_01";
        if (gameId <= 9) {
            artArchiveForLeagues /= L"art";
            trophyRoomFolder = L"trophyroom";
        }
    }
    Path foomTrophies = fmGraphicsPath / L"trophies";
    Path outputPath = contentPath / gameFolder / artArchiveForLeagues / L"trophies";
    for (auto e : db->mCompMap) {
        auto &comp = e.second;
        if (comp->GetDbType() == FifamCompDbType::League || comp->GetDbType() == FifamCompDbType::Cup) {
            if (comp->GetProperty<Int>(L"foom::reputation") >= minRep) {
                Int foomId = comp->GetProperty<Int>(L"foom::id");
                if (gameId >= 10) {
                    String trophyName = Utils::Format(L"%08X", comp->mID.ToInt());
                    ConvertOneTrophy(foomTrophies / Utils::Format(L"%d.png", foomId), outputPath, trophyName + L".tga", trophyRoomFolder);
                }
                else {
                    String trophyName = GetTrophyNameForFM09(comp->mID);
                    if (!trophyName.empty()) {
                        ConvertOneTrophy(foomTrophies / Utils::Format(L"%d.png", foomId), outputPath, trophyName + L".tga", trophyRoomFolder);
                    }
                }
            }
        }
    }
    if (gameId >= 10) {
        ConvertOneTrophy(foomTrophies / L"90.png", outputPath, L"FF24.tga", trophyRoomFolder); // UEFA Nations League
        ConvertOneTrophy(foomTrophies / L"1301385.png", outputPath, L"FF11.tga", trophyRoomFolder); // FIFA World Cup
        ConvertOneTrophy(foomTrophies / L"131306.png", outputPath, L"FF20.tga", trophyRoomFolder); // FIFA Confederations Cup
        ConvertOneTrophy(foomTrophies / L"157097.png", outputPath, L"FF1F.tga", trophyRoomFolder); // FIFA U20 World Cup
        ConvertOneTrophy(foomTrophies / L"121092.png", outputPath, L"F90D.tga", trophyRoomFolder); // FIFA World Club Cup
        ConvertOneTrophy(foomTrophies / L"1301389.png", outputPath, L"FF21.tga", trophyRoomFolder); // Copa America
        ConvertOneTrophy(foomTrophies / L"1301388.png", outputPath, L"FF12.tga", trophyRoomFolder); // UEFA Euro
        ConvertOneTrophy(foomTrophies / L"1301394.png", outputPath, L"F909.tga", trophyRoomFolder); // UEFA Champions League
        ConvertOneTrophy(foomTrophies / L"1301396.png", outputPath, L"F90A.tga", trophyRoomFolder); // UEFA Europa League
        ConvertOneTrophy(foomTrophies / L"1301397.png", outputPath, L"F90C.tga", trophyRoomFolder); // UEFA Super Cup
        ConvertOneTrophy(foomTrophies / L"102415.png", outputPath, L"FA09.tga", trophyRoomFolder); // South America Champions League
        ConvertOneTrophy(foomTrophies / L"317567.png", outputPath, L"FA0A.tga", trophyRoomFolder); // South America Europa League
        ConvertOneTrophy(foomTrophies / L"102418.png", outputPath, L"FA0C.tga", trophyRoomFolder); // South America Super Cup
        ConvertOneTrophy(foomTrophies / L"51002641.png", outputPath, L"FB09.tga", trophyRoomFolder); // North America Champions League
        ConvertOneTrophy(foomTrophies / L"127299.png", outputPath, L"FC09.tga", trophyRoomFolder); // Africa Champions League
        ConvertOneTrophy(foomTrophies / L"12017574.png", outputPath, L"FC0A.tga", trophyRoomFolder); // Africa Europa League
        ConvertOneTrophy(foomTrophies / L"131273.png", outputPath, L"FC0C.tga", trophyRoomFolder); // Africa Super Cup
        ConvertOneTrophy(foomTrophies / L"127286.png", outputPath, L"FD09.tga", trophyRoomFolder); // Asia Champions League
        ConvertOneTrophy(foomTrophies / L"1001959.png", outputPath, L"FD0A.tga", trophyRoomFolder); // Asia Europa League
        ConvertOneTrophy(foomTrophies / L"127285.png", outputPath, L"FE09.tga", trophyRoomFolder); // Oceania Champions League
        ConvertOneTrophy(contentPath / L"icc_trophy.png", outputPath, L"F923.tga", trophyRoomFolder); // International Champions Cup
    }
    else if (gameId >= 8) {
        ConvertOneTrophy(foomTrophies / L"131306.png", outputPath, L"world/ConfedCup.tga", trophyRoomFolder); // FIFA Confederations Cup
        ConvertOneTrophy(foomTrophies / L"131306.png", outputPath, L"world/ConfedCupCopyright.tga", trophyRoomFolder); // FIFA Confederations Cup
        ConvertOneTrophy(foomTrophies / L"157097.png", outputPath, L"world/WCU20.tga", trophyRoomFolder); // FIFA U20 World Cup
        ConvertOneTrophy(foomTrophies / L"157097.png", outputPath, L"world/WCU20Copyright.tga", trophyRoomFolder); // FIFA U20 World Cup
    }
}

void GraphicsConverter::ConvertPortrait(foom::person *person, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId) {
    if (person->mConverterData.mFifamPerson) {
        Path portraitPath = fmGraphicsPath / L"sortitoutsi" / L"faces" / (std::to_wstring(person->mID) + L".png");
        if (exists(portraitPath)) {
            Image portraitImg(portraitPath.string());
            if (portraitImg.isValid() && portraitImg.baseRows() >= 180 && portraitImg.baseColumns() >= 180) {
                if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                    UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                    portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                }
                String gameFolder = Utils::Format(L"fm%02d", gameId);
                String targetFormat = L".png";
                Path portraitsDir = Path(L"portraits") / L"club" / L"160x160";
                if (gameId <= 12)
                    targetFormat = L".tga";
                if (gameId <= 9)
                    portraitsDir = Path(L"art") / L"picture";
                FifamPerson *fifamPerson = (FifamPerson *)person->mConverterData.mFifamPerson;
                Path outputPath = contentPath / gameFolder / L"art_02" / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat);
                portraitImg.resize(Geometry(160, 160));
                SafeWriteImage(portraitImg, outputPath.string());
            }
        }
    }
}

void GraphicsConverter::ConvertRefereePortrait(foom::official *referee, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId) {
    if (referee->mConverterData.mFifamReferee) {
        Path portraitPath = fmGraphicsPath / L"sortitoutsi" / L"faces" / (std::to_wstring(referee->mID) + L".png");
        if (exists(portraitPath)) {
            Image portraitImg(portraitPath.string());
            if (portraitImg.isValid() && portraitImg.baseRows() >= 180 && portraitImg.baseColumns() >= 180) {
                if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                    UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                    portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                }
                String gameFolder = Utils::Format(L"fm%02d", gameId);
                String targetFormat = L".png";
                String artArchive = L"art_05";
                if (gameId <= 12) {
                    targetFormat = L".tga";
                    artArchive = L"art_02";
                }
                FifamReferee *fifamReferee = (FifamReferee *)referee->mConverterData.mFifamReferee;
                Path outputPath = contentPath / gameFolder / artArchive / L"portraits" / L"Referees" / L"160x160" /
                    (FifamNames::GetPersonStringId(gameId, fifamReferee->mFirstName, fifamReferee->mLastName, String(), Date(), 0) + targetFormat);
                portraitImg.resize(Geometry(160, 160));
                SafeWriteImage(portraitImg, outputPath.string());
            }
        }
    }
}

void GraphicsConverter::ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minCA) {
    for (auto e : db->mPlayers) {
        auto &p = e.second;
        if (p.mCurrentAbility > minCA)
            ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
    }
    if (gameId >= 10) {
        for (auto e : db->mNonPlayers) {
            auto &p = e.second;
            if (p.mCurrentAbility > minCA)
                ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
        }
        for (auto e : db->mOfficials) {
            auto &p = e.second;
            if (p.mCurrentAbility > minCA)
                ConvertRefereePortrait(&p, fmGraphicsPath, contentPath, gameId);
        }
    }
}
