#include "KitConverter.h"
#include "FifamDatabase.h"
#include "FifamNames.h"
#include <exception>
#include "FifaDatabase.h"

int main() {

    UInt gameId = 13;

    KitConverter::options.OutputGameId = gameId;
    KitConverter::options.ConvertHomeKit = true;
    KitConverter::options.ConverAwayKit = true;
    KitConverter::options.ConvertGkKit = true;
    KitConverter::options.ConvertThirdKit = true;
    KitConverter::options.SaveLocation = KitConverter::User;
    KitConverter::options.ConvertMinikits = true;
    KitConverter::options.ConvertOnlyMinikits = false;
    KitConverter::options.OnlyCustomKits = true;
    KitConverter::options.AllowCustomKits = true;
    KitConverter::options.Allow2xSize = true;
    KitConverter::options.Force2x = true;
    KitConverter::options.V2 = true;

    KitConverter kitConverter;

    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    //FifamDatabase *db = new FifamDatabase(gameId, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database text", gameId));
    Path infoPath = L"D:\\Projects\\fifam\\db";
    Map<UInt, UInt> mFifamClubs;

    //kitConverter.ConvertClubKits("", 15040, 0x00043E32);
    //kitConverter.ConvertClubArmbands("", 15040, 0x00043E32);
    //return 0;

    const bool annotate = false;
    const bool makispla = true;
    if (annotate) {
        Magick::Image imgAnnotate(makispla? "D:\\Projects\\FIFA20\\kit\\kit_watermark_makispla.tga" : "D:\\Projects\\FIFA20\\kit\\kit_watermark.tga");
        String inputAnnotate = LR"(C:\Users\Dmitri\Desktop\Kits_Bolton_Wanderers\data\kits)"; //L"D:\\Games\\FIFA Manager 13\\data\\kits"; //L"D:\\Projects\\fifam\\content\\fm13\\art_04\\data\\kits";
        for (auto const &p : directory_iterator(inputAnnotate)) {
            if (p.path().extension() == L".tga") {
                Magick::Image finalImg(p.path().string());
                finalImg.composite(imgAnnotate, 0, 0, MagickCore::OverCompositeOp);
                finalImg.write(p.path().string());
            }
        }
        return 0;
    }

    //return 0;

    if (!KitConverter::options.OnlyCustomKits) {
        std::wcout << L"Reading fifam_countries.txt..." << std::endl;
        {
            FifamReader reader(infoPath / L"fifam_countries.txt", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        UInt fmId = 0, fifaId = 0;
                        reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, dummy, dummy, dummy, fmId, dummy, fifaId);
                        if (fmId != 0 && fifaId != 0)
                            mFifamClubs[(fmId << 16) | 0xFFFF] = fifaId;
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
                            mFifamClubs[fmId] = fifaId;
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
    }
    //FifaDatabase *fifadb = new FifaDatabase(L"D:\\Projects\\fifam\\db\\fifa");

    Map<UInt, FifamClub *> mClubsByFifaId;

    //for (auto c : db->mClubs) {
    //    if (c->mFifaID != 0)
    //        mClubsByFifaId[c->mFifaID] = c;
    //}

    auto GetClubByFifaID = [&](Int fifaId) -> FifamClub * {
        auto it = mClubsByFifaId.find(fifaId);
        if (it != mClubsByFifaId.end())
            return (*it).second;
        return nullptr;
    };

    auto GetClubUniqueIdByFifaID = [&](Int fifaId) {
        auto club = GetClubByFifaID(fifaId);
        return club ? club->mUniqueID : 0;
    };

    auto GetClubIdName = [](String const &clubName) {
        String clubNameAscii = Utils::GetStringWithoutUnicodeChars(clubName);
        std::string result;
        for (auto c : clubNameAscii) {
            if (
                c != '<' &&
                c != '>' &&
                c != ':' &&
                c != '"' &&
                c != '/' &&
                c != '\\' &&
                c != '|' &&
                c != '?' &&
                c != '*' &&
                c != ' '
                )
            {
                result += static_cast<char>(c);
            }
        }
        return result;
    };

    //for (int i = 6000; i < 7000; i++) {
    //    kitConverter.ConvertRefereeKit(i);
    //}
    
    //return 0;

    //kitConverter.ConvertClubKits("00150006", 22, 0x00150006);
    //kitConverter.ConvertClubKits("00150008", 21, 0x00150008);
    //kitConverter.ConvertClubKits("002D0004", 241, 0x002D0004);

    //kitConverter.ConvertClubKits("000E0001", 1, 0x000E0001);
    //kitConverter.ConvertClubKits("001B0005", 39, 0x001B0005);
    //kitConverter.ConvertClubKits("002D0004", 241, 0x002D0004);

    //for (auto country : db->mCountries) {
    //    if (country) {
    //        if (country->mNationalTeam.mFifaID != 0)
    //            kitConverter.ConvertBanners(country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
    //        for (auto club : country->mClubs) {
    //            if (club->mFifaID != 0)
    //                kitConverter.ConvertBanners(club->mFifaID, club->mUniqueID);
    //        }
    //    }
    //}
    //kitConverter.ConvertBanners(101059, 0x0031000C);
    //kitConverter.ConvertBanners(101047, 0x00310004);

    //auto GetClubIdCountry = [](UInt id) {
    //    return (id >> 16) & 0xFF;
    //};
    //
    //for (auto [clubId, fifaId] : mFifamClubs) {
    //    kitConverter.ConvertClubKits("", fifaId, clubId);
    //}

    if (KitConverter::options.OnlyCustomKits) {
        for (auto const &p : directory_iterator(kitConverter.options.CustomKitsPath)) {
            if (p.path().extension() == L".png") {
                String filename = p.path().stem().c_str();
                if (Utils::EndsWith(filename, L"_j_h")) {
                    UInt clubId = Utils::SafeConvertInt<UInt>(filename.substr(0, 8), true);
                    if (clubId != 0) {
                        //Error(L"%08X", clubId);
                        kitConverter.ConvertClubKits("", 0, clubId);
                        kitConverter.ConvertClubArmbands("", 0, clubId);
                    }
                }
            }
        }
    }

    //kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_h", ::Color(247, 192, 53), ::Color(28, 53, 80), ::Color(0, 0, 0));
    //kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_a", ::Color(28, 53, 80), ::Color(247, 192, 53), ::Color(0, 0, 0));
    //kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_g", ::Color(32, 61, 90), ::Color(247, 192, 53), ::Color(0, 0, 0));
    //kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_t", ::Color(37, 43, 97), ::Color(247, 192, 53), ::Color(0, 0, 0));


    //kitConverter.ConvertClubKitNumbersCustom();
    //
    //for (auto country : db->mCountries) {
    //    if (country) {
    //        kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
    //        for (auto club : country->mClubs)
    //            kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
    //    }
    //}
    /*
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_h", ::Color(220, 220, 220), ::Color(0, 59, 123), ::Color(0, 59, 123));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_a", ::Color(0, 59, 123), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_g", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_t", ::Color(250, 79, 16), ::Color(12, 12, 12), ::Color(250, 79, 16));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_h", ::Color(220, 220, 220), ::Color(210, 29, 61), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_a", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_g", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_t", ::Color(220, 220, 220), ::Color(12, 12, 12), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_h", ::Color(74, 43, 120), ::Color(102, 158, 214), ::Color(102, 158, 214));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_a", ::Color(251, 212, 3), ::Color(107, 171, 220), ::Color(251, 212, 3));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_g", ::Color(51, 171, 210), ::Color(74, 43, 120), ::Color(51, 171, 210));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_t", ::Color(12, 12, 12), ::Color(251, 212, 3), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_h", ::Color(15, 21, 55), ::Color(220, 220, 220), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_a", ::Color(220, 220, 220), ::Color(15, 21, 55), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_g", ::Color(15, 21, 55), ::Color(209, 151, 41), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_t", ::Color(15, 21, 55), ::Color(45, 161, 203), ::Color(45, 161, 203));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_h", ::Color(220, 220, 220), ::Color(4, 24, 54), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_a", ::Color(12, 12, 12), ::Color(172, 38, 34), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_g", ::Color(12, 12, 12), ::Color(215, 156, 47), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_t", ::Color(23, 44, 68), ::Color(220, 220, 220), ::Color(174, 38, 35));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_h", ::Color(220, 220, 220), ::Color(193, 33, 52), ::Color(193, 33, 52));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_a", ::Color(224, 36, 48), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_g", ::Color(12, 12, 12), ::Color(100, 113, 136), ::Color(100, 113, 136));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_t", ::Color(39, 45, 71), ::Color(138, 177, 213), ::Color(138, 177, 213));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_h", ::Color(140, 113, 73), ::Color(220, 220, 220), ::Color(140, 113, 73));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_a", ::Color(189, 155, 51), ::Color(14, 45, 82), ::Color(189, 155, 51));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_g", ::Color(12, 12, 12), ::Color(232, 162, 0), ::Color(211, 185, 58));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("D:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_t", ::Color(29, 60, 112), ::Color(105, 177, 177), ::Color(29, 60, 112));
    */
    auto WriteOverlay = [](Path const &inputOverlay, String outputFileName) {
        Magick::Image overlayImg(inputOverlay.string());
        if (overlayImg.isValid()) {
            overlayImg.resize(Magick::Geometry(64, 64));
            overlayImg.write("D:\\Games\\FIFA Manager 13\\data\\kitcompbadges\\" + Utils::WtoA(outputFileName) + ".tga");
        }
    };

    Path kitOverlayPath = L"D:\\Projects\\FIFA20\\compbadges";
    /*
    for (auto const &p : directory_iterator(kitOverlayPath)) {
        static Map<UInt, UInt> compsMap = {
            { 1   , 0x0D010000 }, // Denmark Superliga (1)
            { 4   , 0x07010000 }, // Belgium Pro League (1)
            { 7   , 0x36010000 }, // Brazil Serie A (1)
            { 10  , 0x22010000 }, // Holland Eredivisie (1)
            { 13  , 0x0E010000 }, // England Premier League (1)
            { 14  , 0x0E010001 }, // England Championship (2)
            { 16  , 0x12010000 }, // France Ligue 1 (1)
            { 17  , 0x12010001 }, // France Ligue 2 (2)
            { 19  , 0x15010000 }, // Germany 1. Bundesliga (1)
            { 20  , 0x15010001 }, // Germany 2. Bundesliga (2)
            { 31  , 0x1B010000 }, // Italy Serie A (1)
            { 32  , 0x1B010001 }, // Italy Serie B (2)
            { 39  , 0x5F010000 }, // USA Major League Soccer (1)
            { 41  , 0x24010000 }, // Norway Eliteserien (1)
            { 50  , 0x2A010000 }, // Scotland Premiership (1)
            { 53  , 0x2D010000 }, // Spain Primera División (1)
            { 54  , 0x2D010001 }, // Spain Segunda División (2)
            { 56  , 0x2E010000 }, // Sweden Allsvenskan (1)
            { 60  , 0x0E010002 }, // England League One (3)
            { 61  , 0x0E010003 }, // England League Two (4)
            { 65  , 0x19010000 }, // Rep. Ireland Premier Division (1)
            { 66  , 0x25010000 }, // Poland Ekstraklasa (1)
            { 68  , 0x30010000 }, // Turkey Süper Lig (1)
            { 80  , 0x04010000 }, // Austria Bundesliga (1)
            { 83  , 0xA7010000 }, // Korea K-League 1 (1)
            { 189 , 0x2F010000 }, // Switzerland Super League (1)
            { 308 , 0x26010000 }, // Portugal Primeira Liga (1)
            { 330 , 0x27010000 }, // Romania Liga I (1)
            { 335 , 0x37010000 }, // Chile Primera División (1)
            { 336 , 0x38010000 }, // Colombia Categoría Primera (1)
            { 341 , 0x53010000 }, // Mexico Liga MX (1)
            { 349 , 0xA3010000 }, // Japan J1 League (1)
            { 350 , 0xB7010000 }, // Saudi Pro League (1)
            { 351 , 0xC3010000 }, // Australia A-League (1)
            { 353 , 0x34010000 }, // Argentina Primera División (1)
            { 2012, 0x9B010000 }, // China Super League (1)
            { 2076, 0x15010002 }, // Germany 3. Liga (3)
            { 201 , 0x0E030000 }, // England FA Cup
            { 202 , 0x0E040000 }, // England League Cup
            { 203 , 0x0E040001 }, // England League Cup 2
            { 223 , 0xF909 }, // UEFA Champions League
            { 224 , 0xF90A }, // UEFA Europa League
            { 228 , 0x0E070000 }, // England SuperCup
            { 232 , 0xF90C }, // UEFA SuperCup
            { 1003, 0xFA09 }, // CONMEBOL Copa Libertadores
        };
        if (p.is_regular_file() && p.path().extension().string() == ".png" && Utils::StartsWith(p.path().filename().c_str(), L"badge_")) {
            auto parts = Utils::Split(p.path().filename().c_str(), L'_', true, true, false);
            if (parts.size() >= 6) {
                UInt compId = 0;
                UInt teamId = Utils::SafeConvertInt<UInt>(parts[2]);
                UInt kitType = Utils::SafeConvertInt<UInt>(parts[3]);
                UInt sleeveId = Utils::SafeConvertInt<UInt>(parts[5]);
                Bool isChampion = parts[1].size() == 4 && parts[1].at(0) == L'5';
                if (isChampion)
                    compId = Utils::SafeConvertInt<UInt>(parts[1].substr(1));
                else
                    compId = Utils::SafeConvertInt<UInt>(parts[1]);
                if (compsMap.find(compId) != compsMap.end() && (sleeveId == 0 || sleeveId == 1) && kitType <= 3) {
                    UInt clubId = 0;
                    if (teamId != 0) {
                        for (auto c : db->mClubs) {
                            if (c->mFifaID == teamId) {
                                clubId = c->mUniqueID;
                                break;
                            }
                        }
                        if (clubId == 0) {
                            for (auto c : db->mCountries) {
                                if (c && c->mNationalTeam.mFifaID == teamId) {
                                    clubId = c->mNationalTeam.mUniqueID;
                                    break;
                                }
                            }
                        }
                    }
                    if (teamId == 0 || clubId != 0) {
                        static Array<String, 4> kitTypes = { L"_h", L"_a", L"_g", L"_t" };
                        UInt fifamCompId = compsMap[compId];
                        String overlayFileName = Utils::Format(fifamCompId < 0xFFFF ? L"%04X" : L"%08X", fifamCompId);
                        if (clubId != 0)
                            overlayFileName += Utils::Format(L"_%08X", clubId) + kitTypes[kitType];
                        overlayFileName += sleeveId == 0 ? L"_l" : L"_r";
                        if (isChampion)
                            overlayFileName += L"_champion";
                        WriteOverlay(p.path(), overlayFileName);
                    }
                }
            }
        }
    }
    

    for (UInt i = 0; i <= 28; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_0_unpacked_1.png", i + 6000), Utils::Format(L"ChampionsLeague_Titles_%d", i + 4));
    WriteOverlay(kitOverlayPath / L"badge_6099_0_0_0_0_unpacked_1.png", L"ChampionsLeague_Titles_3");
    for (UInt i = 0; i <= 17; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_unpacked_1.png", i + 6100), Utils::Format(L"ChampionsLeague_Winner_%d", i + 2018));
    for (UInt i = 0; i <= 13; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_0_unpacked_1.png", i + 6200), Utils::Format(L"EuropaLeague_Titles_%d", i + 5));
    for (UInt i = 0; i <= 16; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_unpacked_1.png", i + 6300), Utils::Format(L"EuropaLeague_Winner_%d", i + 2019));
    for (UInt i = 0; i <= 16; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_unpacked_1.png", i + 6400), Utils::Format(L"ChampionsLeague_EuropaLeagueWinner_%d", i + 2019));
    */
    
    //for (auto country : db->mCountries) {
    //    if (country) {
    //        
    //        if (country->mNationalTeam.mFifaID != 0) {
    //            GenerateForClub(&country->mNationalTeam);
    //        }
    //        for (auto club : country->mClubs) {
    //            if (club->mFifaID != 0) {
    //                GenerateForClub(club);
    //            }
    //        }
    //    }
    //}

    //GenerateForClub(club);
    

    //kitConverter.ConvertClubKitNumbersCustom();
}
