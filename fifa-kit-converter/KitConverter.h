#pragma once
#include <Magick++.h>

class KitConverter {
    void resizeImage_noAspect(Magick::Image &image, int w, int h);

    int InputResolutionMultiplier = 1;

    int INPUT_RES(int orig);
    int OUTPUT_RES(int orig);

public:
    enum {
        Documents, User, Big
    };

    struct GlobalOptions {
        std::string KitsPath = "D:\\Projects\\FIFA19\\FIFA 19 Kits Converted Textures\\";
        int OutputGameId = 14;
        int SaveLocation = User;
        std::string OutputFormat = "tga";
        bool ConvertHomeKit = true;
        bool ConverAwayKit = true;
        bool ConvertThirdKit = true;
        bool ConvertGkKit = true;
        bool AddWatermarkText = true;
        bool AddKitOverlay = true;
        bool AddKitWrinkles = true;
        int OutputResolutionMultiplier = 1;
    } options;

    KitConverter();
    bool ConvertKit(std::string const &inputShirt, std::string const &inputShorts, std::string const &inputCrest, std::string const &outputFile);
    bool ConvertFifaClubKit(int fifaId, int set, int variation, std::string const &outputFile);
    void ConvertClubKits(int fifaId, int fifaManagerId);
};
