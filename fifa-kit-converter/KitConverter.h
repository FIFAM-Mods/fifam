#pragma once
#include <Magick++.h>

class KitConverter {
    void resizeImage_noAspect(Magick::Image &image, int w, int h);
    int SizeMode = 1;
public:
    enum {
        Documents, User, Big
    };

    struct GlobalOptions {
        std::string KitsPath = "D:\\Projects\\FIFA19\\FIFA 19 Kits Converted Textures\\";
        std::string MiniKitsPath = "D:\\Projects\\FIFA19\\mini\\";
        std::string CustomKitsPath = "D:\\Projects\\FIFA19\\custom\\kits\\";
        std::string CustomMiniKitsPath = "D:\\Projects\\FIFA19\\custom\\kits\\mini\\";
        int OutputGameId = 14;
        int SaveLocation = User;
        std::string OutputFormat = "tga";
        bool ConvertHomeKit = true;
        bool ConverAwayKit = true;
        bool ConvertThirdKit = true;
        bool ConvertGkKit = true;
        bool AddWatermarkText = true;
        bool AddKitOverlay = true;
        bool ConvertMinikits = false;
        bool ConvertOnlyMinikits = false;
        bool AllowCustomKits = true;
        bool OnlyCustomKits = false;
        bool Allow2xSize = true;
    } options;

    KitConverter();
    bool ConvertKit(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile);
    bool ConvertFifaClubKit(int fifaId, std::string const &clubIdStr, int set, int variation, std::string const &outputFile);
    bool ConvertFifaClubMiniKit(int fifaId, std::string const & clubIdStr, int set, int variation, std::string const & outputFile);
    void ConvertClubKits(std::string const &clubIdName, int fifaId, int fifaManagerId);
    void SetSizeMode(int mode);
    int GetSizeMode();
    int Size(int value);
    Magick::Geometry ScaledGeometry(int w, int h, int x, int y);
    Magick::Geometry ScaledGeometry(int w, int h);
    Magick::DrawableRectangle ScaledDrawableRectangle(int w, int h, int x, int y);
    void ScaledResize(Magick::Image &image, int w, int h);
    void ScaledComposite(Magick::Image &dstImage, Magick::Image &inputImg, int x, int y, MagickCore::CompositeOperator mode);
    Magick::Image ScaledImage(std::string const &path);
};
