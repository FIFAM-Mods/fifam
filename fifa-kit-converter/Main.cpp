#include "KitConverter.h"
#include "FifamDatabase.h"
#include "FifamNames.h"
#include <exception>
#include "FifaDatabase.h"
#include "FifaDataFile.h"
#include "commandline.h"

FifamDatabase *db = nullptr;
FifaDatabase *fifadb = nullptr;
Map<UInt, UInt> mFifamClubs;
Map<UInt, UInt> mClubsByFifaId;

Map<UInt, Vector<UInt>> &GetCompsMap() {
    static Map<UInt, Vector<UInt>> compsMap = {
    { 1   , { 0x0D010000 } }, // Denmark Superliga (1)
    { 4   , { 0x07010000 } }, // Belgium Pro League (1)
    { 7   , { 0x36010000 } }, // Brazil Serie A (1)
    { 10  , { 0x22010000 } }, // Holland Eredivisie (1)
    { 13  , { 0x0E010000 } }, // England Premier League (1)
    { 14  , { 0x0E010001 } }, // England Championship (2)
    { 16  , { 0x12010000 } }, // France Ligue 1 (1)
    { 17  , { 0x12010001 } }, // France Ligue 2 (2)
    { 19  , { 0x15010000 } }, // Germany 1. Bundesliga (1)
    { 20  , { 0x15010001 } }, // Germany 2. Bundesliga (2)
    { 21  , { 0x15080000 } }, // Germany relegation
    { 31  , { 0x1B010000 } }, // Italy Serie A (1)
    { 32  , { 0x1B010001 } }, // Italy Serie B (2)
    { 39  , { 0x5F010000, 0x5F080000, 0x5F080001, 0x5F040000 } }, // USA Major League Soccer (1)
    { 41  , { 0x24010000 } }, // Norway Eliteserien (1)
    { 50  , { 0x2A010000 } }, // Scotland Premiership (1)
    { 53  , { 0x2D010000 } }, // Spain Primera División (1)
    { 54  , { 0x2D010001 } }, // Spain Segunda División (2)
    { 56  , { 0x2E010000 } }, // Sweden Allsvenskan (1)
    { 60  , { 0x0E010002 } }, // England League One (3)
    { 61  , { 0x0E010003 } }, // England League Two (4)
    { 65  , { 0x19010000 } }, // Rep. Ireland Premier Division (1)
    { 66  , { 0x25010000 } }, // Poland Ekstraklasa (1)
    { 68  , { 0x30010000 } }, // Turkey Süper Lig (1)
    { 80  , { 0x04010000 } }, // Austria Bundesliga (1)
    { 83  , { 0xA7010000 } }, // Korea K-League 1 (1)
    { 189 , { 0x2F010000 } }, // Switzerland Super League (1)
    { 201 , { 0x0E030000 } }, // England FA Cup
    { 202 , { 0x0E040000 } }, // England League Cup
    { 203 , { 0x0E040001 } }, // England League Cup 2
    { 206 , { 0x15030000 } }, // Germany Cup
    { 207 , { 0x15070000 } }, // Germany SuperCup
    { 210 , { 0x1B030000 } }, // Coppa Italia
    { 211 , { 0x1B070000 } }, // Supercoppa Italia
    { 223 , { 0xF909 } }, // UEFA Champions League
    { 224 , { 0xF90A } }, // UEFA Europa League
    { 226 , { 0xF933 } }, // UEFA Europa Conference League
    { 228 , { 0x0E070000 } }, // England SuperCup
    { 232 , { 0xF90C } }, // UEFA SuperCup
    { 233 , { 0x12070000 } }, // France SuperCup
    { 308 , { 0x26010000 } }, // Portugal Primeira Liga (1)
    { 330 , { 0x27010000 } }, // Romania Liga I (1)
    { 335 , { 0x37010000 } }, // Chile Primera División (1)
    { 336 , { 0x38010000 } }, // Colombia Categoría Primera (1)
    { 341 , { 0x53010000, 0x53080000, 0x53080001, 0x53080002 } }, // Mexico Liga MX (1)
    { 349 , { 0xA3010000 } }, // Japan J1 League (1)
    { 350 , { 0xB7010000 } }, // Saudi Pro League (1)
    { 351 , { 0xC3010000, 0xC3040000 } }, // Australia A-League (1)
    { 360 , { 0x0E080000 } }, // England play-off 1
    { 361 , { 0x0E080001 } }, // England play-off 2
    { 353 , { 0x34010000 } }, // Argentina Primera División (1)
    { 414 , { 0x0E080002 } }, // England play-off 3
    { 451 , { 0x12080000 } }, // France League 1 play-off
    { 1003, { 0xFA09 } }, // CONMEBOL Copa Libertadores
    { 1014, { 0xFA0A } }, // CONMEBOL Copa Sudamericana
    { 1015, { 0xFA0C } }, // CONMEBOL Recopa Sudamericana
    { 2012, { 0x9B010000 } }, // China Super League (1)
    { 2076, { 0x15010002 } }, // Germany 3. Liga (3)
    { 2149, { 0x9F010000 } }, // Indian Super League (1)
    { 10021, { 0x15080000 } }, // Germany Relegation
    { 10201, { 0x0E030000 } }, // England FA Cup
    { 10206, { 0x15030000 } }, // Germany Cup
    { 10207, { 0x15070000 } }, // Germany SuperCup
    { 10223, { 0xF909 } }, // UEFA Champions League
    { 10224, { 0xF90A } }, // UEFA Europa League
    { 10226, { 0xF933 } }, // UEFA Europa Conference League
    { 10232, { 0xF90C } }, // UEFA SuperCup
    { 10233, { 0x12070000 } } // France SuperCup
    };
    return compsMap;
}

void ReadTextDatabase() {
    if (!mFifamClubs.empty())
        return;
    Path infoPath = L"D:\\Projects\\fifam\\db";
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
    for (auto const &c : mFifamClubs) {
        if (c.second != 0)
            mClubsByFifaId[c.second] = c.first;
    }
}

void ReadFifamDatabase(UInt gameId = 13) {
    if (db)
        return;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    db = new FifamDatabase(gameId, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", gameId));
}

void ReadFifaDatabase(Bool onlyLast = false) {
    if (fifadb)
        return;
    if (onlyLast)
        FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
    fifadb = new FifaDatabase(L"D:\\Projects\\fifam\\db\\fifa");
}

UInt GetClubUniqueIdByFifaID(Int fifaId) {
    return mClubsByFifaId[fifaId];
};

std::string GetClubIdName(String const &clubName) {
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

void ConvertRefereeKits(KitConverter &kitConverter) {
    for (int i = 6000; i < 7000; i++)
        kitConverter.ConvertRefereeKit(i);
}

void TestGenerateGenericMinikit() {
    Magick::Image image("D:\\Games\\FIFA Manager 21\\data\\minikits_generic\\11.tga");
    //Magick::Image result(Magick::Geometry(test.columns(), test.rows()), Magick::Color(0, 0, 0));
    image.type(Magick::TrueColorAlphaType);
    image.modifyImage();
    Magick::Pixels view(image);
    Magick::Quantum *pixels = view.get(0, 0, image.columns(), image.rows());
    Magick::Color clr1(255, 255, 255);
    Magick::Color clr2(0, 0, 0);
    Magick::Color clr3(255, 0, 54);
    for (ssize_t row = 0; row < image.rows(); ++row) {
        for (ssize_t column = 0; column < image.columns(); ++column) {
            float q[3] = { (float)pixels[0] / 255.0f, (float)pixels[1] / 255.0f, (float)pixels[2] / 255.0f };
            float sum = q[0] + q[1] + q[2];
            if (sum < 0.0f) {
                pixels[0] = sum * 255.0f;
                pixels[1] = sum * 255.0f;
                pixels[2] = sum * 255.0f;
            }
            else {
                float g = min(q[0], q[1]);
                g = min(g, q[2]);
                q[0] -= g;
                q[1] -= g;
                q[2] -= g;
                if (sum > 1.0f) {
                    q[0] /= sum;
                    q[1] /= sum;
                    q[2] /= sum;
                }
                float p[3];
                p[0] = (float)clr1.quantumRed() * q[0] + (float)clr2.quantumRed() * q[1] + (float)clr3.quantumRed() * q[2];
                p[1] = (float)clr1.quantumGreen() * q[0] + (float)clr2.quantumGreen() * q[1] + (float)clr3.quantumGreen() * q[2];
                p[2] = (float)clr1.quantumBlue() * q[0] + (float)clr2.quantumBlue() * q[1] + (float)clr3.quantumBlue() * q[2];
    
                //auto mx = max(p[0], p[1]);
                //mx = max(mx, p[2]);
                //if (mx > 255.0f) {
                //    p[0] /= mx;
                //    p[1] /= mx;
                //    p[2] /= mx;
                //}
                pixels[0] = p[0] + g * 255.0f;
                pixels[1] = p[1] + g * 255.0f;
                pixels[2] = p[2] + g * 255.0f;
            }
            pixels += 4;
        }
    }
    // Save changes to image.
    view.sync();
    image.write("D:\\Games\\FIFA Manager 21\\data\\minikits_generic\\11_.tga");
}

void Annotate() {
    const bool annotate = false;
    const bool makispla = true;
    if (annotate) {
        Magick::Image imgAnnotate(makispla ? "E:\\Projects\\FIFA20\\kit\\kit_watermark_makispla.tga" : "E:\\Projects\\FIFA20\\kit\\kit_watermark.tga");
        String inputAnnotate = LR"(C:\Users\Dmitri\Desktop\Kits_Bolton_Wanderers\data\kits)"; //L"E:\\Games\\FIFA Manager 13\\data\\kits"; //L"E:\\Projects\\fifam\\content\\fm13\\art_04\\data\\kits";
        for (auto const &p : directory_iterator(inputAnnotate)) {
            if (p.path().extension() == L".tga") {
                Magick::Image finalImg(p.path().string());
                finalImg.composite(imgAnnotate, 0, 0, MagickCore::OverCompositeOp);
                finalImg.write(p.path().string());
            }
        }
    }
}

void ConvertKitsOneClub(KitConverter &kitConverter) {
    kitConverter.ConvertClubKits("", 15040, 0x00043E32);
    kitConverter.ConvertClubArmbands("", 15040, 0x00043E32);
}

void ConvertKitsFewClubs(KitConverter &kitConverter) {
    kitConverter.ConvertClubKits("00150006", 22, 0x00150006);
    kitConverter.ConvertClubKits("00150008", 21, 0x00150008);
    kitConverter.ConvertClubKits("002D0004", 241, 0x002D0004);
    kitConverter.ConvertClubKits("000E0001", 1, 0x000E0001);
    kitConverter.ConvertClubKits("001B0005", 39, 0x001B0005);
    kitConverter.ConvertClubKits("002D0004", 241, 0x002D0004);
    kitConverter.ConvertClubKits("002D000E", 243, 0x002D000E);
}

void GenerateGenericBanners(KitConverter &kitConverter) {
    kitConverter.GenerateGenericBanners();
}

void ConvertClubKitNumbersCustom(KitConverter &kitConverter) {
    kitConverter.ConvertClubKitNumbersCustom();
}

void ConvertAdboards(KitConverter& kitConverter) {
    ReadTextDatabase();
    kitConverter.ConvertAdboards(mClubsByFifaId, GetCompsMap());
}

void ConvertSponsors512x64(KitConverter& kitConverter) {
    ReadTextDatabase();
    kitConverter.ConvertSponsors512x64(mClubsByFifaId, GetCompsMap());
}

void ConvertKitnumbers(KitConverter &kitConverter) {
    ReadTextDatabase();
    ReadFifaDatabase(true);
    std::string outPath = "output\\graphics\\kitnumbers\\jersey\\";
    create_directories(outPath);
    static char const *kitTypeStr[] = { "_h", "_a", "_g", "_t" };
    for (auto const& [fmId, fifaId] : mFifamClubs) {
        if (fifaId != 0) {
            auto fifaClub = fifadb->GetTeam(fifaId);
            if (fifaClub && fifaClub->m_gameId == FifaDatabase::m_lastSupportedGameVersion) {
                for (int k = 0; k <= 3; k++) {
                    auto fifaKit = fifaClub->GetKit(k);
                    if (fifaKit) {
                        std::vector<std::string> numberNames;
                        std::string clubId = Utils::Format("%08X", fmId);
                        if (fifaClub->m_league) {
                            if (fifaClub->m_league->GetId() == 13) // England 1
                                numberNames.push_back("0E010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 14 || fifaClub->m_league->GetId() == 60 || fifaClub->m_league->GetId() == 61) { // England 2
                                numberNames.push_back("0E010001_" + clubId);
                                numberNames.push_back("0E010002_" + clubId);
                                numberNames.push_back("0E010003_" + clubId);
                            }
                            else if (fifaClub->m_league->GetId() == 16) // France 1
                                numberNames.push_back("12010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 17) // France 2
                                numberNames.push_back("12010001_" + clubId);
                            else if (fifaClub->m_league->GetId() == 31) // Italy 1
                                numberNames.push_back("1B010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 39) { // USA 1
                                numberNames.push_back("5F010000_" + clubId);
                                numberNames.push_back("5F080000_" + clubId);
                                numberNames.push_back("5F080001_" + clubId);
                                numberNames.push_back("5F040000_" + clubId);
                            }
                            //else if (fifaClub->m_league->GetId() == 41) // Norway 1
                            //    numberNames.push_back("24010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 50) { // Scotland 1
                                numberNames.push_back("2A010000_" + clubId);
                                //numberNames.push_back("2A080000_" + clubId);
                                //numberNames.push_back("2A080001_" + clubId);
                            }
                            else if (fifaClub->m_league->GetId() == 53 || fifaClub->m_league->GetId() == 54) { // Spain
                                numberNames.push_back("2D010000_" + clubId);
                                numberNames.push_back("2D010001_" + clubId);
                            }
                            else if (fifaClub->m_league->GetId() == 66) { // Poland 1
                                numberNames.push_back("25010000_" + clubId);
                                //numberNames.push_back("25080000_" + clubId);
                                //numberNames.push_back("25080001_" + clubId);
                            }
                            else if (fifaClub->m_league->GetId() == 83) // Korea 1
                                numberNames.push_back("A7010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 308) // Portugal 1
                                numberNames.push_back("26010000_" + clubId);
                            else if (fifaClub->m_league->GetId() == 351) { // Australia 1
                                numberNames.push_back("C3010000_" + clubId);
                                numberNames.push_back("C3040000_" + clubId);
                                numberNames.push_back("C3080000_" + clubId);
                                numberNames.push_back("C3080001_" + clubId);
                            }
                            else if (fifaClub->m_league->GetId() == 2012) // China 1
                                numberNames.push_back("9B010000_" + clubId);
                        }
                        if (numberNames.empty())
                            numberNames.push_back(clubId);
                        for (auto const &numberName : numberNames) {
                            kitConverter.ConvertClubKitNumbersSet(fifaKit->internal.numberfonttype, true,
                                outPath + numberName + kitTypeStr[k],
                                ::Color(fifaKit->internal.jerseynumbercolorprimr, fifaKit->internal.jerseynumbercolorprimg, fifaKit->internal.jerseynumbercolorprimb),
                                ::Color(fifaKit->internal.jerseynumbercolorsecr, fifaKit->internal.jerseynumbercolorsecg, fifaKit->internal.jerseynumbercolorsecb),
                                ::Color(fifaKit->internal.jerseynumbercolorterr, fifaKit->internal.jerseynumbercolorterg, fifaKit->internal.jerseynumbercolorterb));
                        }
                    }
                }
            }
        }
    }
    kitConverter.ConvertClubKitNumbersSet(291, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_h", ::Color(220, 220, 220), ::Color(3, 57, 121), ::Color(3, 57, 121));
    kitConverter.ConvertClubKitNumbersSet(291, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_a", ::Color(54, 74, 65), ::Color(227, 225, 219), ::Color(227, 225, 219));
    kitConverter.ConvertClubKitNumbersSet(291, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_g", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(291, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_t", ::Color(139, 167, 171), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(174, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_h", ::Color(220, 220, 220), ::Color(231, 31, 41), ::Color(253, 183, 19));
    kitConverter.ConvertClubKitNumbersSet(174, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_a", ::Color(12, 12, 12), ::Color(253, 183, 19), ::Color(231, 31, 41));
    kitConverter.ConvertClubKitNumbersSet(174, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_g", ::Color(220, 220, 220), ::Color(231, 31, 41), ::Color(253, 183, 19));
    kitConverter.ConvertClubKitNumbersSet(174, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_t", ::Color(220, 220, 220), ::Color(125, 135, 141), ::Color(125, 135, 141));
    kitConverter.ConvertClubKitNumbersSet(294, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_h", ::Color(33, 54, 84), ::Color(86, 163, 219), ::Color(86, 163, 219));
    kitConverter.ConvertClubKitNumbersSet(294, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_a", ::Color(168, 175, 176), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(294, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_g", ::Color(84, 195, 188), ::Color(238, 65, 155), ::Color(238, 65, 155));
    kitConverter.ConvertClubKitNumbersSet(294, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_t", ::Color(150, 251, 38), ::Color(150, 155, 186), ::Color(150, 155, 186));
    kitConverter.ConvertClubKitNumbersSet(151, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_h", ::Color(28, 30, 77), ::Color(28, 30, 77), ::Color(28, 30, 77));
    kitConverter.ConvertClubKitNumbersSet(151, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_a", ::Color(220, 220, 220), ::Color(28, 30, 77), ::Color(28, 30, 77));
    kitConverter.ConvertClubKitNumbersSet(151, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_g", ::Color(220, 220, 220), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(151, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_t", ::Color(28, 30, 77), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(239, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(66))) + "_h", ::Color(56, 48, 135), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(239, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(66))) + "_a", ::Color(220, 220, 220), ::Color(36, 53, 89), ::Color(36, 53, 89));
    kitConverter.ConvertClubKitNumbersSet(239, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(66))) + "_g", ::Color(220, 220, 220), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(239, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(66))) + "_t", ::Color(89, 20, 20), ::Color(213, 130, 119), ::Color(213, 130, 119));
    kitConverter.ConvertClubKitNumbersSet(289, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_h", ::Color(182, 220, 220), ::Color(206, 30, 60), ::Color(190, 187, 220));
    kitConverter.ConvertClubKitNumbersSet(289, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_a", ::Color(53, 53, 74), ::Color(220, 220, 220), ::Color(65, 65, 90));
    kitConverter.ConvertClubKitNumbersSet(289, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_g", ::Color(220, 220, 220), ::Color(67, 163, 91), ::Color(190, 187, 187));
    kitConverter.ConvertClubKitNumbersSet(289, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_t", ::Color(220, 220, 220), ::Color(206, 30, 60), ::Color(190, 187, 187));
    kitConverter.ConvertClubKitNumbersSet(290, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_h", ::Color(67, 74, 152), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(290, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_a", ::Color(247, 238, 104), ::Color(37, 44, 100), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(290, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_g", ::Color(12, 12, 12), ::Color(231, 215, 93), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(290, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_t", ::Color(220, 220, 220), ::Color(65, 104, 171), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(284, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_h", ::Color(77, 68, 59), ::Color(220, 220, 220), ::Color(107, 96, 90));
    kitConverter.ConvertClubKitNumbersSet(284, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_a", ::Color(136, 136, 136), ::Color(22, 27, 51), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(284, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_g", ::Color(220, 220, 220), ::Color(196, 80, 102), ::Color(193, 190, 190));
    kitConverter.ConvertClubKitNumbersSet(284, true, outPath + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_t", ::Color(220, 220, 220), ::Color(45, 88, 158), ::Color(193, 190, 190));
}

void ConvertKitnumbersFewClubs(KitConverter &kitConverter) {
    kitConverter.ConvertClubKitNumbers(241, 0x002D0004);
    kitConverter.ConvertClubKitNumbers(243, 0x002D000E);
    kitConverter.ConvertClubKitNumbers(44, 0x001A0009);
    kitConverter.ConvertClubKitNumbers(45, 0x001A000A);
}

void ConvertBanners(KitConverter &kitConverter) {
    ReadTextDatabase();
    ReadFifaDatabase();
    for (auto const &[fifamClubId, fifaClubId] : mFifamClubs) {
        if (fifaClubId != 0) {
            auto fifaClub = fifadb->GetTeam(fifaClubId);
            if (fifaClub) {
                kitConverter.ConvertBanners(fifaClubId, fifamClubId,
                    Magick::Color(fifaClub->internal.teamcolor1r, fifaClub->internal.teamcolor1g, fifaClub->internal.teamcolor1b),
                    Magick::Color(fifaClub->internal.teamcolor2r, fifaClub->internal.teamcolor2g, fifaClub->internal.teamcolor2b));
            }
            //else
            //    Error("Unable to find team with FIFA ID %d", fifamClub->mFifaID);
        }
    }
}

void FindBannersWithoutId() {
    ReadTextDatabase();
    Set<UInt> usedFifaIDs;
    for (auto [clubId, fifaId] : mFifamClubs)
        usedFifaIDs.insert(fifaId);
    path withoutFifaIdDir = "H:\\FIFA_ASSETS\\without_fifa_id";
    create_directories(withoutFifaIdDir);
    for (auto const &e : directory_iterator("H:\\FIFA_ASSETS\\crest\\24")) {
        auto const &p = e.path();
        if (p.extension() == ".png") {
            auto filename = p.stem().string();
            if (Utils::StartsWith(filename, "l")) {
                filename = filename.substr(1);
                auto fifaId = Utils::SafeConvertInt<UInt>(filename);
                if (fifaId != 0 && !Utils::Contains(usedFifaIDs, fifaId) && exists("H:\\FIFA_ASSETS\\banners\\24\\banner_" + filename + "_color.png")) {
                    Magick::Image image(p.string());
                    image.write((withoutFifaIdDir / (p.stem().string() + ".png")).string());
                }
            }
        }
    }
}

void ConvertBannersFewClubs(KitConverter &kitConverter) {
    kitConverter.ConvertBanners(101059, 0x0031000C);
    kitConverter.ConvertBanners(101047, 0x00310004);
}

void ConvertKits(KitConverter &kitConverter) {
    ReadTextDatabase();
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
    else {
        for (auto [clubId, fifaId] : mFifamClubs) {
            kitConverter.ConvertClubKits("", fifaId, clubId);
            kitConverter.ConvertClubArmbands("", fifaId, clubId);
        }
    }
}

void ConvertClubKitNumbersSetTest(KitConverter &kitConverter) {
    kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_h", ::Color(247, 192, 53), ::Color(28, 53, 80), ::Color(0, 0, 0));
    kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_a", ::Color(28, 53, 80), ::Color(247, 192, 53), ::Color(0, 0, 0));
    kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_g", ::Color(32, 61, 90), ::Color(247, 192, 53), ::Color(0, 0, 0));
    kitConverter.ConvertClubKitNumbersSet(1000, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", 0x002D0004)) + "_t", ::Color(37, 43, 97), ::Color(247, 192, 53), ::Color(0, 0, 0));
}

void ConvertClubArmbands(KitConverter &kitConverter) {
    ReadFifamDatabase();
    for (auto country : db->mCountries) {
        if (country) {
            kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
            for (auto club : country->mClubs)
                kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
        }
    }
}

void ConvertClubKitNumbersSetFewClubs(KitConverter &kitConverter) {
    ReadTextDatabase();
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_h", ::Color(220, 220, 220), ::Color(0, 59, 123), ::Color(0, 59, 123));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_a", ::Color(0, 59, 123), ::Color(220, 220, 220), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_g", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(171, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(5))) + "_t", ::Color(250, 79, 16), ::Color(12, 12, 12), ::Color(250, 79, 16));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_h", ::Color(220, 220, 220), ::Color(210, 29, 61), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_a", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_g", ::Color(12, 12, 12), ::Color(220, 220, 220), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(174, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(9))) + "_t", ::Color(220, 220, 220), ::Color(12, 12, 12), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_h", ::Color(74, 43, 120), ::Color(102, 158, 214), ::Color(102, 158, 214));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_a", ::Color(251, 212, 3), ::Color(107, 171, 220), ::Color(251, 212, 3));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_g", ::Color(51, 171, 210), ::Color(74, 43, 120), ::Color(51, 171, 210));
    kitConverter.ConvertClubKitNumbersSet(173, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(10))) + "_t", ::Color(12, 12, 12), ::Color(251, 212, 3), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_h", ::Color(15, 21, 55), ::Color(220, 220, 220), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_a", ::Color(220, 220, 220), ::Color(15, 21, 55), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_g", ::Color(15, 21, 55), ::Color(209, 151, 41), ::Color(15, 21, 55));
    kitConverter.ConvertClubKitNumbersSet(151, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(18))) + "_t", ::Color(15, 21, 55), ::Color(45, 161, 203), ::Color(45, 161, 203));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_h", ::Color(220, 220, 220), ::Color(4, 24, 54), ::Color(220, 220, 220));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_a", ::Color(12, 12, 12), ::Color(172, 38, 34), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_g", ::Color(12, 12, 12), ::Color(215, 156, 47), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(142, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(73))) + "_t", ::Color(23, 44, 68), ::Color(220, 220, 220), ::Color(174, 38, 35));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_h", ::Color(220, 220, 220), ::Color(193, 33, 52), ::Color(193, 33, 52));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_a", ::Color(224, 36, 48), ::Color(12, 12, 12), ::Color(12, 12, 12));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_g", ::Color(12, 12, 12), ::Color(100, 113, 136), ::Color(100, 113, 136));
    kitConverter.ConvertClubKitNumbersSet(153, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(240))) + "_t", ::Color(39, 45, 71), ::Color(138, 177, 213), ::Color(138, 177, 213));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_h", ::Color(140, 113, 73), ::Color(220, 220, 220), ::Color(140, 113, 73));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_a", ::Color(189, 155, 51), ::Color(14, 45, 82), ::Color(189, 155, 51));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_g", ::Color(12, 12, 12), ::Color(232, 162, 0), ::Color(211, 185, 58));
    kitConverter.ConvertClubKitNumbersSet(147, true, std::string("E:\\Games\\FIFA Manager 13\\data\\kitnumbers\\jersey2\\") + Utils::WtoA(Utils::Format(L"%08X", GetClubUniqueIdByFifaID(243))) + "_t", ::Color(29, 60, 112), ::Color(105, 177, 177), ::Color(29, 60, 112));
}

void ConvertOverlay() {
    ReadTextDatabase();
    std::string outPath = "output\\graphics\\kitcompbadges\\";
    create_directories(outPath);
    auto WriteOverlay = [&](Path const &inputOverlay, String outputFileName) {
        if (exists(inputOverlay)) {
            Magick::Image overlayImg(inputOverlay.string());
            if (overlayImg.isValid()) {
                overlayImg.resize(Magick::Geometry(64, 64));
                overlayImg.write(outPath + Utils::WtoA(outputFileName) + ".tga");
            }
        }
    };

    Path kitOverlayPath = L"D:\\Projects\\FIFA\\kit\\compbadges";

    Map<UInt, UInt> mFifaClubToFifam;

    for (auto c : mFifamClubs) {
        if (c.second != 0)
            mFifaClubToFifam[c.second] = c.first;
    }

    for (auto const &p : directory_iterator(kitOverlayPath)) {
        if (p.is_regular_file() && p.path().extension().string() == ".png" && Utils::StartsWith(p.path().filename().c_str(), L"badge_")) {
            auto parts = Utils::Split(p.path().filename().c_str(), L'_', true, true, false);
            if (parts.size() >= 6) {
                UInt compId = 0;
                UInt teamId = Utils::SafeConvertInt<UInt>(parts[2]);
                UInt kitType = Utils::SafeConvertInt<UInt>(parts[3]);
                UInt sleeveId = Utils::SafeConvertInt<UInt>(parts[5]);
                Bool isChampion = parts[1].size() == 6 && parts[1].at(0) == L'5';
                if (isChampion)
                    compId = Utils::SafeConvertInt<UInt>(parts[1].substr(1));
                else
                    compId = Utils::SafeConvertInt<UInt>(parts[1]);
                if (GetCompsMap().find(compId) != GetCompsMap().end() && (sleeveId == 0 || sleeveId == 1) && kitType <= 3) {
                    UInt clubId = 0;
                    if (teamId != 0)
                        clubId = mFifaClubToFifam[teamId];
                    if (teamId == 0 || clubId != 0) {
                        static Array<String, 4> kitTypes = { L"_h", L"_a", L"_g", L"_t" };
                        for (UInt fifamCompId : GetCompsMap()[compId]) {
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
    }
    // UEFA Champions League
    //for (UInt i = 1; i <= 2; i++)
    //    WriteOverlay(kitOverlayPath / L"badge_500223_0_0_0_1_color.png", Utils::Format(L"F909_Titles_%d", i));
    for (UInt i = 3; i <= 33; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 600000), Utils::Format(L"F909_Titles_%d", i));
    for (UInt i = 3; i <= 33; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 600100), Utils::Format(L"F909_Winner_Titles_%d", i));
    for (UInt i = 21; i <= 40; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 600200), Utils::Format(L"F909_F90A_Winner_%d", i + 2000));
    // UEFA Europa League
    //for (UInt i = 1; i <= 4; i++)
    //    WriteOverlay(kitOverlayPath / L"badge_500224_0_0_0_1_color.png", Utils::Format(L"F909_Titles_%d", i));
    for (UInt i = 5; i <= 20; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 601000), Utils::Format(L"F90A_Titles_%d", i));
    for (UInt i = 5; i <= 20; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 601100), Utils::Format(L"F90A_Winner_Titles_%d", i));
    for (UInt i = 21; i <= 40; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 601200), Utils::Format(L"F90A_F933_Winner_%d", i + 2000));
    // UEFA Europa Conference League
    //for (UInt i = 1; i <= 4; i++)
    //    WriteOverlay(kitOverlayPath / L"badge_500226_0_0_0_1_color.png", Utils::Format(L"F933_Titles_%d", i));
    for (UInt i = 5; i <= 15; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 605000), Utils::Format(L"F933_Titles_%d", i));
    for (UInt i = 5; i <= 15; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 605100), Utils::Format(L"F933_Winner_Titles_%d", i));
    // FA Cup
    for (UInt i = 1; i <= 28; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_0_color.png", i + 604000), Utils::Format(L"0E040000_Titles_%d", i));
    // DFB Cup
    for (UInt i = 1; i <= 36; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_1_color.png", i + 606000), Utils::Format(L"15040000_Titles_%d", i));
    // CONMEBOL Copa Libertadores
    for (UInt i = 1; i <= 21; i++)
        WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_0_color.png", i + 602000), Utils::Format(L"FA09_Titles_%d", i));
    // ?
    //for (UInt i = 20; i <= 36; i++)
    //    WriteOverlay(kitOverlayPath / Utils::Format(L"badge_%d_0_0_0_0_color.png", i + 603000), Utils::Format(L"FA0A_%d", i + 2000));
}

void ConvertBannersFifa(KitConverter &kitConverter, bool fifa14stadiums) {
    ReadFifaDatabase();
    kitConverter.GenerateGenericBannersFIFA(fifa14stadiums);
    fifadb->ForAllTeams([&](FifaTeam &team) {
        kitConverter.ConvertBannersFIFA(team.GetId(), fifa14stadiums,
            Magick::Color(team.internal.teamcolor1r, team.internal.teamcolor1g, team.internal.teamcolor1b),
            Magick::Color(team.internal.teamcolor2r, team.internal.teamcolor2g, team.internal.teamcolor2b));
    });
}

int main(int argc, char *argv[]) {
    UInt gameId = 13; // 13
    KitConverter::options.OutputGameId = gameId;
    KitConverter::options.ConvertHomeKit = true;
    KitConverter::options.ConverAwayKit = true;
    KitConverter::options.ConvertGkKit = true; // true
    KitConverter::options.ConvertThirdKit = true; // true
    KitConverter::options.ConvertMinikits = true; // true
    KitConverter::options.ConvertOnlyMinikits = false;
    KitConverter::options.OnlyCustomKits = false;
    KitConverter::options.AllowCustomKits = true;
    KitConverter::options.Allow2xSize = true;
    KitConverter::options.Force2x = true; // true
    KitConverter::options.V2 = true; // true
    KitConverter::options.AddKitOverlay = false; // false
    KitConverter kitConverter;
    kitConverter.GenerateBallsDat(GetCompsMap());
    //ConvertRefereeKits(kitConverter);
}
