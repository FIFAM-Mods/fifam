#pragma once
#include <Magick++.h>
#include "Renderer.h"
#include "ModelReader.h"
#include "Color.h"
#include <array>
#include "FifamClub.h"

class KitConverter {
    void resizeImage_noAspect(Magick::Image &image, int w, int h);
    int SizeMode = 1;
    Renderer *mRenderer = nullptr;
    Model mEditedModel;
    Model mSourceModel;
    std::vector<std::vector<std::pair<int, int>>> circles;
public:
    enum {
        Documents, User, Big
    };

    static struct GlobalOptions {
        std::string KitsPath = "D:\\Projects\\FIFA\\kit\\";
        std::string MiniKitsPath = "D:\\Projects\\FIFA\\kits\\";
        std::string CustomKitsPath = "D:\\Projects\\FIFA\\custom\\kits\\";
        std::string CustomMiniKitsPath = "D:\\Projects\\FIFA\\custom\\kits\\mini\\";
        std::string KitNumbersPath = "D:\\Projects\\FIFA\\kit\\numbers\\";
        std::string BannersPath = "D:\\Projects\\FIFA\\banners\\";
        std::string CrestsPath = "D:\\Projects\\FIFA\\crest\\";
        int OutputGameId = 14;
        std::string OutputFormat = "tga";
        bool ConvertHomeKit = true;
        bool ConverAwayKit = true;
        bool ConvertThirdKit = true;
        bool ConvertGkKit = true;
        bool AddWatermarkText = false;
        bool AddKitOverlay = false;
        bool ConvertMinikits = true;
        bool ConvertOnlyMinikits = false;
        bool AllowCustomKits = true;
        bool OnlyCustomKits = false;
        bool Allow2xSize = true;
        bool GenerateKitNumbers = false;
        bool V2 = true;
        bool Force2x = false;
        bool Overlay = false;
    } options;

    KitConverter();
    ~KitConverter();
    bool ConvertKit(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile, std::array<int, 8> logoPos, bool hasPositions);
    bool ConvertKitV2(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile, std::array<int, 8> logoPos, bool hasPositions);
    bool ConvertFifaClubKit(int fifaId, std::string const &clubIdStr, int set, int variation, std::string const &outputFile);
    bool ConvertFifaClubMiniKit(int fifaId, std::string const & clubIdStr, int set, int variation, std::string const & outputFile);
    void ConvertClubKits(std::string const &clubIdName, int fifaId, int fifaManagerId);
    void ConvertRefereeKit(int fifaId);
    void SetSizeMode(int mode);
    int GetSizeMode();
    int Size(int value);
    Magick::Geometry ScaledGeometry(int w, int h, int x, int y);
    Magick::Geometry ScaledGeometry(int w, int h);
    Magick::DrawableRectangle ScaledDrawableRectangle(int w, int h, int x, int y);
    void ScaledResize(Magick::Image &image, int w, int h);
    void ScaledComposite(Magick::Image &dstImage, Magick::Image &inputImg, int x, int y, MagickCore::CompositeOperator mode);
    Magick::Image ScaledImage(std::string const &path);
    bool ConvertClubArmband(int fifaId, std::string const &clubIdStr, int set, int variation, std::string const &outputFile);
    void ConvertClubArmbands(std::string const &clubIdName, int fifaId, int fifaManagerId);
    bool ConvertClubKitNumbersSet(int fifaId, std::string const &clubIdStr, int tournament, int type, bool jersey, std::string const &outputFile);
    bool ConvertClubKitNumbersSet(int kitnumersId, bool jersey, std::string const &outputFile, ::Color const &clr1, ::Color const &clr2, ::Color const &clr3);
    void ConvertClubKitNumbers(int fifaId, int fifaManagerId);
    void ConvertClubKitNumbers(int kitnumberId, int fifaManagerId, ::Color const &clr1, ::Color const &clr2, ::Color const &clr3);
    bool ConvertClubKitNumbersSetCustom(std::string const &dirPath, std::string const &dirName, bool jersey);
    void ConvertClubKitNumbersCustom();
    void ConvertBanners(int fifaId, int fifaManagerId, Magick::Color const &primCol = Magick::Color(), Magick::Color const &secCol = Magick::Color());
    void ConvertBannersFIFA(int fifaId, bool fifa14stadiums, Magick::Color const &primCol = Magick::Color(), Magick::Color const &secCol = Magick::Color());
    void ConvertBannersFIFA_NewFormat(int fifaId, bool fifa14stadiums, Magick::Color const &primCol = Magick::Color(), Magick::Color const &secCol = Magick::Color());
    void ConvertAdboards(Map<UInt, UInt> const &fifaClubToFifam, Map<UInt, Vector<UInt>> const &compsMap);
    void ConvertSponsors512x64(Map<UInt, UInt>& fifaClubToFifam, Map<UInt, Vector<UInt>>& compsMap);
    void GenerateGenericBanners();
    void GenerateGenericBannersFIFA(bool fifa14stadiums);
};
