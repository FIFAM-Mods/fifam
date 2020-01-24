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

Bool GraphicsConverter::ConvertOneCompBadge(Path const &badgePath, Path const &outputPath, String const &badgeName, UInt gameId) {
    if (exists(badgePath)) {
        Image badgeImg(badgePath.string());
        if (badgeImg.isValid() && badgeImg.baseRows() >= 256) {
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
    Path badgesPath = contentPath / gameFolder / L"badges" / L"badges" / L"clubs";
    Path outputPath;
    if (mOnlyUpdates || mOutputToGameFolder)
        outputPath = L"D:\\Games\\FIFA Manager 13\\badges\\clubs";
    else
        outputPath = badgesPath;
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
    for (auto e : db->mClubs) {
        auto &club = e.second;
        if (club.mReputation > minRep && club.mConverterData.mFifamClub) {
            FifamClub *fifamClub = (FifamClub *)club.mConverterData.mFifamClub;
            Int foomIdForGraphics = club.mID;
            if (!club.mConverterData.mParentClub && club.mConverterData.mOriginalStoredParentClub)
                foomIdForGraphics = club.mConverterData.mOriginalStoredParentClub->mID;

            auto CreateBadge = [&](Int id, Bool reserve) {
                auto it = availableBadges.find(foomIdForGraphics);
                if (it != availableBadges.end()) {
                    String clubBadgeName = Utils::Format(reserve ? L"%08X_1.tga" : L"%08X.tga", fifamClub->mUniqueID);
                    if (!mOnlyUpdates || !exists(badgesPath / L"256x256" / clubBadgeName)) {
                        Image badgeImg((*it).second.string());
                        if (badgeImg.isValid() && badgeImg.baseRows() >= 256) {
                            if (badgeImg.baseColumns() != badgeImg.baseRows()) {
                                UInt newSize = Utils::Max(badgeImg.baseColumns(), badgeImg.baseRows());
                                badgeImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                            }
                            WriteOneBadge(badgeImg, outputPath, clubBadgeName, gameId);
                        }
                    }
                }
            };

            CreateBadge(foomIdForGraphics, false);
            //auto reserveFoomId = fifamClub->GetProperty<Int>(L"res_team_foom_id", -1);
            //if (reserveFoomId != -1)
            //    CreateBadge(foomIdForGraphics, true);
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
    Path badgesPath = contentPath / gameFolder / L"badges" / L"badges" / L"Leagues";
    Path outputPath;
    if (mOnlyUpdates || mOutputToGameFolder)
        outputPath = L"D:\\Games\\FIFA Manager 13\\badges\\Leagues";
    else
        outputPath = badgesPath;
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
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

                if (!mOnlyUpdates || !exists(badgesPath / L"256x256" / badgeName)) {
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
                                            if (comp->mCompetitionLevel == 0) {
                                                Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                                outputImg.composite(flagImg, -52, 18, OverCompositeOp);
                                                Image maskImg(Path(contentPath / L"templates" / L"league_mask.png").string());
                                                outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                                Image frameImg(Path(contentPath / L"templates" / L"league_frame.png").string());
                                                outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                                WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                            }
                                            else if (comp->mCompetitionLevel == 1) {
                                                Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                                outputImg.composite(flagImg, -52, 18, OverCompositeOp);
                                                Image maskImg(Path(contentPath / L"templates" / L"league2_mask.png").string());
                                                outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                                Image frameImg(Path(contentPath / L"templates" / L"league2_frame.png").string());
                                                outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                                WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                            }
                                            else if (comp->mCompetitionLevel == 2) {
                                                Image outputImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
                                                flagImg.resize(Geometry(306, 187));
                                                outputImg.composite(flagImg, -25, 34, OverCompositeOp);
                                                Image maskImg(Path(contentPath / L"templates" / L"league3_mask.png").string());
                                                outputImg.composite(maskImg, 0, 0, CopyAlphaCompositeOp);
                                                Image frameImg(Path(contentPath / L"templates" / L"league3_frame.png").string());
                                                outputImg.composite(frameImg, 0, 0, OverCompositeOp);
                                                WriteOneBadge(outputImg, outputPath, badgeName, gameId, true);
                                            }
                                            else if (comp->mCompetitionLevel == 3) {
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
    }
    if (!mOnlyUpdates && gameId >= 10) { // logos for international competitions are not present in FM07-FM09 (trophy images are used instead)
        ConvertOneCompBadge(foomBadgesPath / L"90_uefa.png", outputPath, L"FF24.tga", gameId); // UEFA Nations League
        ConvertOneCompBadge(foomBadgesPath / L"95_uefa.png", outputPath, L"FF10.tga", gameId); // European Qualifiers
        ConvertOneCompBadge(foomBadgesPath / L"1301385_fifa.png", outputPath, L"2018_FF11.tga", gameId); // FIFA World Cup 2018
        ConvertOneCompBadge(foomBadgesPath / L"121092_fifa.png", outputPath, L"2018_FF0D.tga", gameId); // FIFA World Club Cup 2018
        ConvertOneCompBadge(foomBadgesPath / L"1301389_conmebol.png", outputPath, L"2019_FF21.tga", gameId); // Copa America 2019
        ConvertOneCompBadge(foomBadgesPath / L"1301388_uefa.png", outputPath, L"2020_FF12.tga", gameId); // UEFA Euro 2020
        ConvertOneCompBadge(foomBadgesPath / L"157097_fifa.png", outputPath, L"FF1F_2019.tga", gameId); // FIFA U20 World Cup
        ConvertOneCompBadge(foomBadgesPath / L"1301394_uefa.png", outputPath, L"F909.tga", gameId); // UEFA Champions League
        ConvertOneCompBadge(foomBadgesPath / L"1301396_uefa.png", outputPath, L"F90A.tga", gameId); // UEFA Europa League
        ConvertOneCompBadge(foomBadgesPath / L"1301397_uefa.png", outputPath, L"F90C.tga", gameId); // UEFA Super Cup
        ConvertOneCompBadge(foomBadgesPath / L"102415_conmebol.png", outputPath, L"FA09.tga", gameId); // South America Champions League
        ConvertOneCompBadge(foomBadgesPath / L"317567_conmebol.png", outputPath, L"FA0A.tga", gameId); // South America Europa League
        ConvertOneCompBadge(foomBadgesPath / L"102418_conmebol.png", outputPath, L"FA0C.tga", gameId); // South America Super Cup
        ConvertOneCompBadge(foomBadgesPath / L"51002641_concacaf.png", outputPath, L"FB09.tga", gameId); // North America Champions League
        ConvertOneCompBadge(foomBadgesPath / L"222987_concacaf.png", outputPath, L"FB0C.tga", gameId); // North America Super Cup
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

Bool GraphicsConverter::ConvertOneTrophy(Path const &trophyPath, Path const &outputPath, Path const &trophyNamePath, String const &trophyRoomFolder) {
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
    create_directories(outputPath / trophyRoomFolder);
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
    for (auto e : db->mCompMap) {
        auto &comp = e.second;
        if (comp->GetDbType() == FifamCompDbType::League || comp->GetDbType() == FifamCompDbType::Cup) {
            if (comp->GetProperty<Int>(L"foom::reputation") >= minRep) {
                Int foomId = comp->GetProperty<Int>(L"foom::id");
                String trophyName;
                if (gameId >= 10)
                    trophyName = Utils::Format(L"%08X", comp->mID.ToInt());
                else
                    trophyName = GetTrophyNameForFM09(comp->mID);
                if (!mOnlyUpdates || !exists(outputPath / L"256x256" / (trophyName + L".tga")))
                    ConvertOneTrophy(foomTrophies / Utils::Format(L"%d.png", foomId), outputPath, trophyName + L".tga", trophyRoomFolder);
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
            String gameFolder = Utils::Format(L"fm%02d", gameId);
            String targetFormat = L".png";
            Path portraitsDir = Path(L"portraits") / L"club" / L"160x160";
            if (gameId <= 12)
                targetFormat = L".tga";
            if (gameId <= 9)
                portraitsDir = Path(L"art") / L"picture";
            FifamPerson *fifamPerson = (FifamPerson *)person->mConverterData.mFifamPerson;
            String dstFolder = L"art_02";
            if (gameId >= 13 && !fifamPerson->mWriteableStringID.empty()) {
                if (fifamPerson->mWriteableStringID[0] >= L'F' && fifamPerson->mWriteableStringID[0] <= L'L')
                    dstFolder = L"art_03";
                else if (fifamPerson->mWriteableStringID[0] >= L'M' && fifamPerson->mWriteableStringID[0] <= L'R')
                    dstFolder = L"art_06";
                else if (fifamPerson->mWriteableStringID[0] >= L'S' /*&& fifamPerson->mWriteableStringID[0] <= L'Z'*/)
                    dstFolder = L"art_07";
            }
            Path outputPath = contentPath / gameFolder / dstFolder / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat);
            if (!mOnlyUpdates || (!exists(outputPath) &&
                !exists(contentPath / gameFolder / L"art_02" / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat)) &&
                !exists(contentPath / gameFolder / L"art_03" / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat)) &&
                !exists(contentPath / gameFolder / L"art_06" / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat)) &&
                !exists(contentPath / gameFolder / L"art_07" / portraitsDir / (fifamPerson->mWriteableStringID + targetFormat))))
            {
                if (mOnlyUpdates || mOutputToGameFolder)
                    outputPath = Path(L"D:\\Games\\FIFA Manager 13\\portraits\\club\\160x160") / (fifamPerson->mWriteableStringID + targetFormat);
                Image portraitImg(portraitPath.string());
                if (portraitImg.isValid() && portraitImg.baseRows() >= 150 && portraitImg.baseColumns() >= 150) {
                    if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                        UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                        portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    }
                    portraitImg.resize(Geometry(160, 160));
                    SafeWriteImage(portraitImg, outputPath.string());
                }
            }
        }
    }
}

void GraphicsConverter::ConvertRefereePortrait(foom::official *referee, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId) {
    if (referee->mConverterData.mFifamReferee) {
        Path portraitPath = fmGraphicsPath / L"sortitoutsi" / L"faces" / (std::to_wstring(referee->mID) + L".png");
        if (exists(portraitPath)) {
            String targetFormat = L".png";
            FifamReferee *fifamReferee = (FifamReferee *)referee->mConverterData.mFifamReferee;
            Path basePath;
            if (mOnlyUpdates || mOutputToGameFolder)
                basePath = L"D:\\Games\\FIFA Manager 13";
            else {
                String artArchive = L"art_05";
                if (gameId <= 12) {
                    targetFormat = L".tga";
                    artArchive = L"art_02";
                }
                basePath = contentPath / Utils::Format(L"fm%02d", gameId) / artArchive;
            }
            Path outputPath = basePath / L"portraits" / L"Referees" / L"160x160" /
                (FifamNames::GetPersonStringId(gameId, fifamReferee->mFirstName, fifamReferee->mLastName, String(), Date(), 0) + targetFormat);
            if (!mOnlyUpdates || !exists(outputPath)) {
                Image portraitImg(portraitPath.string());
                if (portraitImg.isValid() && portraitImg.baseRows() >= 150 && portraitImg.baseColumns() >= 150) {
                    if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                        UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                        portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    }
                    portraitImg.resize(Geometry(160, 160));
                    SafeWriteImage(portraitImg, outputPath.string());
                }
            }
        }
    }
}

void GraphicsConverter::ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minCA) {
    create_directories(contentPath / L"fm13\\art_02\\portraits\\club\\160x160");
    create_directories(contentPath / L"fm13\\art_03\\portraits\\club\\160x160");
    create_directories(contentPath / L"fm13\\art_06\\portraits\\club\\160x160");
    create_directories(contentPath / L"fm13\\art_07\\portraits\\club\\160x160");
    create_directories(contentPath / L"fm13\\art_05\\portraits\\Referees\\160x160");
    std::wcout << L"Converting player portraits...  0%";
    UInt max = db->mPlayers.size();
    UInt counter = 0;
    for (auto e : db->mPlayers) {
        auto &p = e.second;
        if (p.mCurrentAbility > minCA)
            ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
        std::wcout << Utils::Format(L"\b\b\b\b%3d%%", (Int)((Float)counter / max * 100));
        counter++;
    }
    std::wcout << L"\b\b\b\b100%" << std::endl;
    if (gameId >= 10) {
        max = db->mNonPlayers.size();
        counter = 0;
        std::wcout << L"Converting staff portraits...   0%";
        for (auto e : db->mNonPlayers) {
            auto &p = e.second;
            if (p.mCurrentAbility > minCA)
                ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
            std::wcout << Utils::Format(L"\b\b\b\b%3d%%", (Int)((Float)counter / max * 100));
            counter++;
        }
        std::wcout << L"\b\b\b\b100%" << std::endl;
        if (!mOnlyUpdates) {
            max = db->mOfficials.size();
            counter = 0;
            std::wcout << L"Converting referee portraits...   0%";
            for (auto e : db->mOfficials) {
                auto &p = e.second;
                if (p.mCurrentAbility > minCA)
                    ConvertRefereePortrait(&p, fmGraphicsPath, contentPath, gameId);
                std::wcout << Utils::Format(L"\b\b\b\b%3d%%", (Int)((Float)counter / max * 100));
                counter++;
            }
            std::wcout << L"\b\b\b\b100%" << std::endl;
        }
    }
}

void GraphicsConverter::ConvertOneCity(Int foomClubId, Int fifamClubId, Path const &inputPath, Path const &outputPath, String const &name, Int rep, FifamWriter &writer) {
    Path imagePath = inputPath / L"cities" / Utils::Format(L"%d.jpg", foomClubId);
    if (!exists(imagePath)) {
        imagePath = inputPath / L"cities" / Utils::Format(L"%d.png", foomClubId);
        if (!exists(imagePath)) {
            imagePath = inputPath / L"cities" / Utils::Format(L"%d.jpeg", foomClubId);
            if (!exists(imagePath)) {
                String n = name;
                writer.WriteLine(Quoted(n), foomClubId, rep, false, 0, 0);
                return;
            }
        }
    }
    Image img(imagePath.string());
    if (img.isValid() && img.baseColumns() >= 600 && img.baseRows() >= 450) {
        img.resize(Geometry("615x461^"));
        img.extent(Geometry("615x461"), GravityType::CenterGravity);
        //img.brightnessContrast(0.0, 25.0);
        //img.quality(100);
        img.write(Path(outputPath / Utils::Format(L"%08X.jpg", fifamClubId)).string());
    }
    else {
        String n = name;
        writer.WriteLine(Quoted(n), foomClubId, rep, true, img.baseColumns(), img.baseRows());
    }
}

void GraphicsConverter::ConvertCities(foom::db *db, Path const &inputPath, Path const &contentPath, UInt gameId, Int minRep) {
    Path outputPath = contentPath / L"cities" / L"615x461";
    if (!exists(outputPath))
        create_directories(outputPath);
    FifamWriter writer(outputPath / "_missed.csv", 14, FifamVersion());
    writer.WriteLine(L"Name", L"ID", L"Reputation", L"Exists", L"DimX", L"DimY");
    for (auto[id, club] : db->mClubs) {
        if (club.mReputation >= minRep && club.mConverterData.mFifamClub)
            ConvertOneCity(id, ((FifamClub *)club.mConverterData.mFifamClub)->mUniqueID, inputPath, outputPath, club.mName, club.mReputation, writer);
    }
    for (auto[id, country] : db->mNations) {
        if (country.mReputation >= minRep && country.mConverterData.mFifamCountry) {
            FifamCountry *fifamCountry = (FifamCountry *)country.mConverterData.mFifamCountry;
            Int clubInCapital = -1;
            Int maxRepClubInCapital = -1;
            for (auto[clubId, club] : db->mClubs) {
                if (club.mNation && club.mNation->mID == id) {
                    if (club.mReputation > maxRepClubInCapital) {
                        if (club.mCity && club.mCity->mName == FifamTr(fifamCountry->mNationalTeam.mCityName)) {
                            clubInCapital = clubId;
                            maxRepClubInCapital = club.mReputation;
                        }
                    }
                }
            }
            if (clubInCapital != -1)
                ConvertOneCity(clubInCapital, fifamCountry->mNationalTeam.mUniqueID, inputPath, outputPath, country.mName, country.mReputation, writer);
        }
            
    }
}

void GraphicsConverter::ConvertOneStadium(Int foomId, UInt fifamClubId, Path const &inputPath, Path const &outputPath, String const &name, String const &stadName, Int rep, FifamWriter &writer) {
    Path imagePath = inputPath / Utils::Format(L"%d.png", foomId);
    if (!exists(imagePath)) {
        imagePath = inputPath / Utils::Format(L"%d.jpg", foomId);
        if (!exists(imagePath)) {
            imagePath = inputPath / Utils::Format(L"%d.jpeg", foomId);
            if (!exists(imagePath)) {
                String n = name;
                String sn = stadName;
                writer.WriteLine(Quoted(n), Quoted(sn), foomId, rep, Hexadecimal(fifamClubId), 0, false, 0, 0);
                return;
            }
        }
    }
    String clubIdStr = Utils::Format(L"%08X.jpg", fifamClubId);
    Image img(imagePath.string());
    Image img2x(img);
    if (img2x.isValid() && img2x.baseColumns() >= 1300 && img2x.baseRows() >= 800) {
        img2x.resize(Geometry("1920x1200^"));
        img2x.extent(Geometry("1920x1200"), GravityType::CenterGravity);
        img2x.write(Path(outputPath / L"1920x1200" / clubIdStr).string());
    }
    else {
        String n = name;
        String sn = stadName;
        writer.WriteLine(Quoted(n), Quoted(sn), foomId, rep, Hexadecimal(fifamClubId), 2, true, img.baseColumns(), img.baseRows());
    }
    if (img.isValid() && img.baseColumns() >= 530 && img.baseRows() >= 400) {
        img.resize(Geometry("800x600^"));
        img.extent(Geometry("800x600"), GravityType::CenterGravity);
        img.write(Path(outputPath / L"800x600" / clubIdStr).string());
        img.resize(Geometry("200x150"));
        img.write(Path(outputPath / L"200x150" / clubIdStr).string());
    }
    else {
        String n = name;
        String sn = stadName;
        writer.WriteLine(Quoted(n), Quoted(sn), foomId, rep, Hexadecimal(fifamClubId), 1, true, img.baseColumns(), img.baseRows());
    }
}

void GraphicsConverter::ConvertStadiums(foom::db *db, Path const &inputPath, Path const &contentPath, UInt gameId, Int minRep, bool overview) {
    Path outputPath = contentPath / L"stadiums";
    Path inputImagesFolder = inputPath / (overview ? L"stadiums_overview" : L"stadiums_my");
    if (!exists(outputPath))
        create_directories(outputPath);
    if (!exists(outputPath / L"1920x1200"))
        create_directories(outputPath / L"1920x1200");
    if (!exists(outputPath / L"800x600"))
        create_directories(outputPath / L"800x600");
    if (!exists(outputPath / L"200x150"))
        create_directories(outputPath / L"200x150");
    FifamWriter writer(outputPath / (overview ? L"_missed_overview2.csv" : L"_missed2.csv"), 14, FifamVersion());
    writer.WriteLine(L"Name", L"StadiumName", L"ID", L"Reputation", L"FifamID", L"Type", L"Exists", L"DimX", L"DimY");
    for (auto &[id, club] : db->mClubs) {
        if (club.mReputation >= minRep && club.mConverterData.mFifamClub) {
            auto fifamClub = (FifamClub *)club.mConverterData.mFifamClub;
            Int stadId = overview ? fifamClub->GetProperty<Int>(L"foom::stad_id", -1) : id;
            ConvertOneStadium(stadId, fifamClub->mUniqueID, inputImagesFolder, outputPath, club.mName, FifamTr(fifamClub->mStadiumName), club.mReputation, writer);
        }
    }
    //for (auto &[id, country] : db->mNations) {
    //    auto fifamCountry = (FifamCountry *)country.mConverterData.mFifamCountry;
    //    if (fifamCountry) {
    //        Int stadId = -1;
    //        if (overview)
    //            stadId = fifamCountry->GetProperty<Int>(L"foom::stad_id", -1);
    //        else {
    //            foom::team *owner = fifamCountry->GetProperty<foom::team *>(L"foom::stad_owner", nullptr);
    //            if (owner && !owner->mIsNation)
    //                stadId = owner->mID;
    //        }
    //        ConvertOneStadium(stadId, fifamCountry->mNationalTeam.mUniqueID, inputImagesFolder, outputPath, country.mName, FifamTr(fifamCountry->mNationalTeam.mStadiumName), country.mReputation, writer);
    //    }
    //}
}

void WriteTextModeBadge(Image &img, Image &mask, Path const &outputPath, String const &fileName) {
    Image tmImg = img;
    tmImg.resize(Geometry("168x168"));
    tmImg.extent(Geometry("114x168"), GravityType::CenterGravity);
    tmImg.composite(mask, 0, 0, CopyAlphaCompositeOp);
    tmImg.write(Path(outputPath / fileName).string());
}

void GraphicsConverter::ConvertClubBadgesFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"clubs";
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
    for (auto [clubId, club] : db->mClubsMap) {
        if (club->mFifaID != 0) {
            Path badgePath = fifaAssetsPath / L"crest" / Utils::Format(L"%d.png", club->mFifaID);
            if (!exists(badgePath))
                badgePath = fifaAssetsPath / L"crest" / Utils::Format(L"l%d.dds", club->mFifaID);
            if (exists(badgePath)) {
                Image badgeImg(badgePath.string());
                if (badgeImg.isValid() && badgeImg.baseRows() == 256)
                    WriteOneBadge(badgeImg, outputPath, Utils::Format(L"%08X.tga", club->mUniqueID), gameId);
            }
        }
    }
    if (gameId <= 9) {
        Path logoPath = fifaAssetsPath / L"league" / L"l365.dds";
        if (exists(logoPath)) {
            Image badgeImg(logoPath.string());
            if (badgeImg.isValid() && badgeImg.baseRows() == 256) {
                Path genericPath = contentPath / gameFolder / L"badges" / L"badges" / L"generic";
                badgeImg.resize(Geometry(256, 256));
                SafeWriteImage(badgeImg, (Path(genericPath / L"Badge256" / L"generic256.tga").string()));
                badgeImg.resize(Geometry(128, 128));
                SafeWriteImage(badgeImg, (Path(genericPath / L"Badge128" / L"generic128.tga").string()));
                badgeImg.resize(Geometry(64, 64));
                SafeWriteImage(badgeImg, (Path(genericPath / L"Badge64" / L"generic64.tga").string()));
                badgeImg.resize(Geometry(32, 32));
                SafeWriteImage(badgeImg, (Path(genericPath / L"Badge32" / L"generic32.tga").string()));
            }
        }
    }
}

void GraphicsConverter::DownloadClubBadgesFIFA(FifamDatabase *db, Path const &contentPath, UInt gameId) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"clubs";
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
    for (auto c : db->mCountries) {
        if (c) {
            std::wcout << L"Badges: " << FifamTr(c->mName) << std::endl;
            for (auto [clubId, club] : c->mClubsMap) {
                if (club->mFifaID != 0) {
                    String clubIdFile = Utils::Format(L"%08X.tga", club->mUniqueID);
                    if (!mOnlyUpdates || !exists(outputPath / L"256x256" / clubIdFile)) {
                        Char url[MAX_PATH];
                        sprintf(url, "https://cdn-p2.frzdb.net/fifamobile/images/clubbadges/%d.png", club->mFifaID);
                        if (URLDownloadToFile(NULL, url, "tmpFifaBadge.png", 0, NULL) == S_OK) {
                            Image badgeImg("tmpFifaBadge.png");
                            if (badgeImg.isValid() && badgeImg.baseRows() == 256)
                                WriteOneBadge(badgeImg, outputPath, clubIdFile, gameId);
                        }
                    }
                }
            }
        }
    }
}

Bool DownloadAndSavePlayerPhoto(Int version, Char const *filepath, Char const *url) {
    if (URLDownloadToFile(NULL, url, "tmpFifaPortrait.png", 0, NULL) == S_OK) {
        Image portraitImg("tmpFifaPortrait.png");
        if (portraitImg.isValid() && portraitImg.columns() >= 160) {
            portraitImg.resize(Geometry(160, 160));
            portraitImg.write(filepath);
            return true;
        }
    }
    return false;
}

Bool DownloadAndSavePlayerPhoto(Int playerId, Char const *filepath) {
    char url[MAX_PATH];
    sprintf(url, "https://www.futwiz.com/assets/img/fifa%d/careerfaces/%d.png", 20, playerId);
    if (!DownloadAndSavePlayerPhoto(20, filepath, url) && !exists(filepath)) {
        sprintf(url, "https://www.futwiz.com/assets/img/fifa%d/careerfaces/%d.png", 19, playerId);
        if (!DownloadAndSavePlayerPhoto(19, filepath, url)) {
            sprintf(url, "https://www.futwiz.com/assets/img/fifa%d/careerfaces/%d.png", 18, playerId);
            if (!DownloadAndSavePlayerPhoto(15, filepath, url)) {
                sprintf(url, "http://futhead.cursecdn.com/static/img/%d/players/%d.png", 17, playerId);
                if (!DownloadAndSavePlayerPhoto(17, filepath, url))
                    return false;
            }
        }
    }
    return true;
}

void GraphicsConverter::DownloadPlayerPortraitsFIFA(FifamDatabase *db, Path const &contentPath, UInt gameId) {
    if (gameId >= 9) {
        Path outputPath = contentPath / L"portraits" / L"club" / L"160x160";
        create_directories(outputPath);
        for (auto c : db->mCountries) {
            if (c && c->mId == FifamNation::Romania) {
                std::wcout << L"Portraits: " << FifamTr(c->mName) << std::endl;
                for (auto b : c->mClubs) {
                    for (auto p : b->mPlayers) {
                        try {
                            if (p->mEmpicsId != 0) {
                                Path outputPic = outputPath / (p->mWriteableStringID + L".tga");
                                DownloadAndSavePlayerPhoto(p->mEmpicsId, outputPic.string().c_str());
                            }
                        }
                        catch (std::exception &e) {
                            ::Error(Utils::AtoW(e.what()) + String(L"\n") + p->mWriteableStringID);
                        }
                    }
                }
            }
        }
        std::wcout << L"Portraits: Free agents" << std::endl;
        for (auto p : db->mPlayers) {
            try {
                if (!p->mClub && p->mEmpicsId != 0) {
                    Path outputPic = outputPath / (p->mWriteableStringID + L".tga");
                    DownloadAndSavePlayerPhoto(p->mEmpicsId, outputPic.string().c_str());
                }
            }
            catch (std::exception &e) {
                ::Error(Utils::AtoW(e.what()) + String(L"\n") + p->mWriteableStringID);
            }
        }
    }
}

Bool operator<(::Color const &a, ::Color const &b) {
    return a.ToHexStr() <= b.ToHexStr();;
}

FifamClubTeamColor GraphicsConverter::GetBadgeColor(Path const &filepath) {
    ColorPair clr;
    Image img(filepath.string());
    if (img.isValid()) {
        const UInt badgeSize = 8;
        const Double colorRange = 100.0;
        const Float secondColorFactor = 5.0f;
        Map<::Color, UInt> clrsMap;
        img.resize(Geometry(badgeSize, badgeSize));
        for (size_t row = 0; row < img.rows(); ++row) {
            for (size_t column = 0; column < img.columns(); ++column) {
                auto pc = img.pixelColor(column, row);
                if (pc.quantumAlpha() == 255)
                    clrsMap[::Color(pc.quantumRed(), pc.quantumGreen(), pc.quantumBlue())]++;
            }
        }
        if (!clrsMap.empty()) {
            Vector<Pair<::Color, UInt>> clrsVecFromMap;
            clrsVecFromMap.reserve(badgeSize < 100 ? (badgeSize * badgeSize) : badgeSize);
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
                clr.second = ::Color(255, 255, 255);
                if (clrsVec.size() > 1 && clrsVec[0].second != 0 && clrsVec[1].second != 0) {
                    if (Float(clrsVec[0].second) / Float(clrsVec[1].second) < secondColorFactor)
                        clr.second = clrsVec[1].first;
                }
                return FifamClubTeamColor::MakeFromInt(clr.FindIndexInTable(FifamClub::mTeamColorsTable));
            }
        }
    }
    return FifamClubTeamColor::MakeFromInt(Random::Get(0, 27));
}

void GraphicsConverter::ConvertPlayerPortraitsFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId) {
    Path picPath;
    if (gameId >= 9) {
        Path assetsDir = fifaAssetsPath / L"minifaces";
        Path outputPath = contentPath / Utils::Format(L"fm%02d", gameId) / L"portraits" / L"club" / L"160x160";
        create_directories(outputPath);
        for (auto c : db->mCountries) {
            if (c) {
                std::wcout << FifamTr(c->mName) << std::endl;
                for (auto b : c->mClubs) {
                    for (auto p : b->mPlayers) {
                        try {
                            if (p->mEmpicsId != 0) {
                                picPath = assetsDir / Utils::Format(L"p%d.dds", p->mEmpicsId);
                                if (exists(picPath)) {
                                    Image pic(picPath.string());
                                    pic.resize(Geometry(160, 160));
                                    Path outputPic = outputPath / (p->mWriteableStringID + L".tga");
                                    pic.write(outputPic.string());
                                }
                            }
                        }
                        catch (std::exception &e) {
                            ::Error(e.what() + std::string("\n") + picPath.string());
                        }
                    }
                }
            }
        }
        std::wcout << L"Free agents" << std::endl;
        for (auto p : db->mPlayers) {
            try {
                if (!p->mClub && p->mEmpicsId != 0) {
                    picPath = assetsDir / Utils::Format(L"p%d.dds", p->mEmpicsId);
                    if (exists(picPath)) {
                        Image pic(picPath.string());
                        pic.resize(Geometry(160, 160));
                        Path outputPic = outputPath / (p->mWriteableStringID + L".tga");
                        pic.write(outputPic.string());
                    }
                }
            }
            catch (std::exception &e) {
                ::Error(e.what() + std::string("\n") + picPath.string());
            }
        }
    }
}

void GraphicsConverter::ConvertCompBadgesFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"Leagues";
    create_directories(outputPath / L"256x256");
    create_directories(outputPath / L"128x128");
    create_directories(outputPath / L"64x64");
    create_directories(outputPath / L"32x32");
    Image tmMask(Path(contentPath / L"templates" / L"textmode_mask.png").string());
    for (auto [compId, comp] : db->mCompMap) {
        if (comp->GetDbType() == FifamCompDbType::League) {
            Int logoId = -1;
            switch (compId.mRegion.ToInt()) {
            case FifamCompRegion::Denmark:
                if (compId.mIndex == 0)
                    logoId = 1;
                break;
            case FifamCompRegion::Belgium:
                if (compId.mIndex == 0)
                    logoId = 4;
                break;
            case FifamCompRegion::Brazil:
                if (compId.mIndex == 0)
                    logoId = 7;
                break;
            case FifamCompRegion::Netherlands:
                if (compId.mIndex == 0)
                    logoId = 10;
                break;
            case FifamCompRegion::England:
                if (compId.mIndex == 0)
                    logoId = 13;
                else if (compId.mIndex == 1)
                    logoId = 14;
                else if (compId.mIndex == 3)
                    logoId = 60;
                else if (compId.mIndex == 4)
                    logoId = 61;
                break;
            case FifamCompRegion::France:
                if (compId.mIndex == 0)
                    logoId = 16;
                else if (compId.mIndex == 1)
                    logoId = 17;
                break;
            case FifamCompRegion::Germany:
                if (compId.mIndex == 0)
                    logoId = 19;
                else if (compId.mIndex == 1)
                    logoId = 20;
                else if (compId.mIndex == 2)
                    logoId = 2076;
                break;
            case FifamCompRegion::Italy:
                if (compId.mIndex == 0)
                    logoId = 31;
                else if (compId.mIndex == 1)
                    logoId = 32;
                break;
            case FifamCompRegion::United_States:
                if (compId.mIndex == 0)
                    logoId = 39;
                break;
            case FifamCompRegion::Norway:
                if (compId.mIndex == 0)
                    logoId = 41;
                break;
            case FifamCompRegion::Scotland:
                if (compId.mIndex == 0)
                    logoId = 50;
                break;
            case FifamCompRegion::Spain:
                if (compId.mIndex == 0)
                    logoId = 53;
                else if (compId.mIndex == 1)
                    logoId = 54;
                break;
            case FifamCompRegion::Sweden:
                if (compId.mIndex == 0)
                    logoId = 56;
                break;
            case FifamCompRegion::Greece:
                if (compId.mIndex == 0)
                    logoId = 63;
                break;
            case FifamCompRegion::Ireland:
                if (compId.mIndex == 0)
                    logoId = 65;
                break;
            case FifamCompRegion::Poland:
                if (compId.mIndex == 0)
                    logoId = 66;
                break;
            case FifamCompRegion::Turkey:
                if (compId.mIndex == 0)
                    logoId = 68;
                break;
            case FifamCompRegion::Austria:
                if (compId.mIndex == 0)
                    logoId = 80;
                break;
            case FifamCompRegion::Korea_Republic:
                if (compId.mIndex == 0)
                    logoId = 83;
                break;
            case FifamCompRegion::Switzerland:
                if (compId.mIndex == 0)
                    logoId = 189;
                break;
            case FifamCompRegion::Portugal:
                if (compId.mIndex == 0)
                    logoId = 308;
                break;
            case FifamCompRegion::Croatia:
                if (compId.mIndex == 0)
                    logoId = 317;
                break;
            case FifamCompRegion::Czech_Republic:
                if (compId.mIndex == 0)
                    logoId = 319;
                break;
            case FifamCompRegion::Finland:
                if (compId.mIndex == 0)
                    logoId = 322;
                break;
            case FifamCompRegion::Ukraine:
                if (compId.mIndex == 0)
                    logoId = 332;
                break;
            case FifamCompRegion::Chile:
                if (compId.mIndex == 0)
                    logoId = 335;
                break;
            case FifamCompRegion::Colombia:
                if (compId.mIndex == 0)
                    logoId = 336;
                break;
            case FifamCompRegion::Mexico:
                if (compId.mIndex == 0)
                    logoId = 341;
                break;
            case FifamCompRegion::South_Africa:
                if (compId.mIndex == 0)
                    logoId = 347;
                break;
            case FifamCompRegion::Japan:
                if (compId.mIndex == 0)
                    logoId = 349;
                break;
            case FifamCompRegion::Saudi_Arabia:
                if (compId.mIndex == 0)
                    logoId = 350;
                break;
            case FifamCompRegion::Australia:
                if (compId.mIndex == 0)
                    logoId = 351;
                break;
            case FifamCompRegion::Argentina:
                if (compId.mIndex == 0)
                    logoId = 353;
                break;
            case FifamCompRegion::China_PR:
                if (compId.mIndex == 0)
                    logoId = 2012;
                break;
            case FifamCompRegion::Russia:
                if (compId.mIndex == 0)
                    logoId = 3006;
                break;
            }
            if (logoId != -1) {
                Path logoPath = fifaAssetsPath / L"league" / Utils::Format(L"%d.png", logoId);
                if (!exists(logoPath))
                    logoPath = fifaAssetsPath / L"league" / Utils::Format(L"l%d.dds", logoId);
                if (exists(logoPath)) {
                    String badgeName;
                    if (gameId >= 10)
                        badgeName = Utils::Format(L"%08X.tga", compId.ToInt());
                    else if (compId.mRegion.ToInt() > 0 && compId.mRegion.ToInt() <= 207)
                        badgeName = Utils::Format(L"%s%d.tga", countryLeagueNames[compId.mRegion.ToInt()], comp->mCompetitionLevel + 1);
                    else
                        continue;
                    Image badgeImg(logoPath.string());
                    if (badgeImg.isValid() && badgeImg.baseRows() == 256) {
                        if (gameId <= 8)
                            WriteTextModeBadge(badgeImg, tmMask, outputPath / L"TextMode", badgeName);
                        WriteOneBadge(badgeImg, outputPath, badgeName, gameId, true);
                    }
                }
            }
        }
    }
    if (gameId <= 9) {
        Path logoPath = fifaAssetsPath / L"league" / L"l76.dds";
        if (exists(logoPath)) {
            Image badgeImg(logoPath.string());
            if (badgeImg.isValid() && badgeImg.baseRows() == 256) {
                for (UInt i = 1; i <= 10; i++) {
                    String badgeName = Utils::Format(L"Generics%d.tga", i);
                    Image genImg = badgeImg;
                    WriteOneBadge(genImg, outputPath, badgeName, gameId, true);
                    if (gameId <= 8)
                        WriteTextModeBadge(badgeImg, tmMask, outputPath / L"TextMode", badgeName);
                }
            }
        }
    }
}

void GraphicsConverter::CopyLeagueSplitAndRelegationBadges(FifamDatabase *db, Path const &outputPath, Path const &contentPath, UInt gameId) {
    String gameFolder = Utils::Format(L"fm%02d", gameId);
    Path badgesPath = /*contentPath / gameFolder*/ outputPath / L"badges" / L"Leagues";
    Path badgesOutputPath = outputPath / L"badges" / L"Leagues";
    for (auto const &[compId, comp] : db->mCompMap) {
        if (compId.mRegion.ToInt() > 0 && compId.mRegion.ToInt() <= FifamDatabase::NUM_COUNTRIES
            && compId.mType == FifamCompType::Relegation
            && comp->GetDbType() == FifamCompDbType::League
            && comp->mCompetitionLevel == 0
            && comp->AsLeague()->mTakePoints == true)
        {
            std::error_code ec;
            UInt res[] = { 256, 128, 64, 32 };
            for (UInt i = 0; i < std::size(res); i++) {
                String badgesResFolder = Utils::Format(L"%ux%u", res[i], res[i]);
                Path srcFilePath = badgesPath / badgesResFolder / (FifamCompID(compId.mRegion, FifamCompType::League, 0).ToHexStr() + L".tga");
                if (exists(srcFilePath, ec))
                    copy(srcFilePath, badgesOutputPath / badgesResFolder / (compId.ToHexStr() + L".tga"), copy_options::overwrite_existing, ec);
            }
        }
        else if (compId.mType == FifamCompType::Relegation)
            ConvertOneCompBadge(contentPath / L"playoff.tga", badgesOutputPath, compId.ToHexStr() + L".tga", gameId);
    }
}
