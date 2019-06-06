#pragma once
#include <Magick++.h>
#include "Renderer.h"
#include "ModelReader.h"

class KitConverter {
    void resizeImage_noAspect(Magick::Image &image, int w, int h);
    int SizeMode = 1;
    Renderer *mRenderer = nullptr;
    Model mEditedModel;
    Model mSourceModel;
public:
    enum {
        Documents, User, Big
    };

    struct GlobalOptions {
        std::string KitsPath = "D:\\Projects\\FIFA19\\FIFA 19 Kits Converted Textures\\";
        std::string MiniKitsPath = "D:\\Projects\\FIFA19\\mini\\";
        std::string CustomKitsPath = "D:\\Projects\\FIFA19\\custom\\kits\\";
        std::string CustomMiniKitsPath = "D:\\Projects\\FIFA19\\custom\\kits\\mini\\";
        std::string KitNumbersPath = "D:\\Projects\\FIFA19\\F19_specific-kitnumbers\\png\\";
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
        bool GenerateKitNumberes = false;
        bool V2 = true;
    } options;

    KitConverter();
    ~KitConverter();
    bool ConvertKit(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile);
    bool ConvertKitV2(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile);
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
    bool ConvertClubArmband(int fifaId, std::string const &clubIdStr, int set, int variation, std::string const &outputFile);
    void ConvertClubArmbands(std::string const &clubIdName, int fifaId, int fifaManagerId);
    bool ConvertClubKitNumbersSet(int fifaId, std::string const &clubIdStr, int tournament, int type, bool jersey, std::string const &outputFile);
    void ConvertClubKitNumbers(std::string const &clubIdName, int fifaId, int fifaManagerId);
    bool ConvertClubKitNumbersSetCustom(std::string const &dirPath, std::string const &dirName, bool jersey);
    void ConvertClubKitNumbersCustom();
};
