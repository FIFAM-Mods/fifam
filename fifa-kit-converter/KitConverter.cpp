#include "KitConverter.h"
#include <filesystem>
#include "Utils.h"
#include "Error.h"

using namespace std;
using namespace std::filesystem;
using namespace Magick;

KitConverter::GlobalOptions KitConverter::options;

void KitConverter::resizeImage_noAspect(Image & image, int w, int h) {
    Geometry geom(w, h);
    geom.aspect(true);
    image.resize(geom);
}

Geometry KitConverter::ScaledGeometry(int w, int h, int x, int y) {
    return Geometry(Size(w), Size(h), Size(x), Size(y));
}

Geometry KitConverter::ScaledGeometry(int w, int h) {
    return Geometry(Size(w), Size(h));
}

DrawableRectangle KitConverter::ScaledDrawableRectangle(int w, int h, int x, int y) {
    return DrawableRectangle(Size(w), Size(h), Size(x), Size(y));
}

void KitConverter::ScaledResize(Image &image, int w, int h) {
    resizeImage_noAspect(image, Size(w), Size(h));
}

void KitConverter::ScaledComposite(Image &dstImage, Image &inputImg, int x, int y, CompositeOperator mode) {
    dstImage.composite(inputImg, Size(x), Size(y), mode);
}

Image KitConverter::ScaledImage(string const &path) {
    Image img (path);
    img.resize(ScaledGeometry(img.columns(), img.rows()));
    return img;
}

KitConverter::KitConverter() {
    InitializeMagick(NULL);
    if (options.V2) {
        mRenderer = new Renderer(options.Force2x ? 1024 : 512, options.Force2x ? 2048 : 1024);
        if (!mRenderer->Available()) {
            ::Error("Failed to initialize V2 Renderer");
        }
        ReadObjModel(mSourceModel, Utils::AtoW(options.KitsPath + "model_source.obj").c_str());
        ReadObjModel(mEditedModel, Utils::AtoW(options.KitsPath + "model_edited.obj").c_str());
    }
    else
        mRenderer = nullptr;
}

KitConverter::~KitConverter() {
    delete mRenderer;
}

void KitConverter::SetSizeMode(int mode) {
    if (mode > 1 && options.Allow2xSize)
        SizeMode = 2;
    else
        SizeMode = 1;
}

int KitConverter::GetSizeMode() {
    return SizeMode;
}

int KitConverter::Size(int value) {
    if (SizeMode != 1)
        return value * SizeMode;
    return value;
}

bool KitConverter::ConvertKit(string const &inputShirt, string const &inputShorts, string const &inputCrest, string const &outputFile) {
    SetSizeMode(1);
    if (options.OutputGameId >= 9) {
        
        Image inputShortsImg(inputShorts);
        if (inputShortsImg.columns() > 1024) {
            SetSizeMode(2);
            if (GetSizeMode() == 1)
                inputShortsImg.resize(ScaledGeometry(1024, 512));
        }

        Image outputFileImg(ScaledGeometry(512, 1024), options.Overlay ? "transparent" : "red");

        Image leftSockImg(inputShortsImg, ScaledGeometry(356, 174, 142, 5));
        Image leftSockImg2(ScaledGeometry(356, 174 * 2), options.Overlay ? "transparent" : "black");
        ScaledComposite(leftSockImg2, leftSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174, OverCompositeOp);
        ScaledResize(leftSockImg2, 101, 284);
        leftSockImg2.crop(ScaledGeometry(101, 181));
        ScaledComposite(outputFileImg, leftSockImg2, -12, 593, OverCompositeOp);

        Image rightSockImg(inputShortsImg, ScaledGeometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(ScaledGeometry(356, 174 * 2), options.Overlay ? "transparent" : "black");
        ScaledComposite(rightSockImg2, rightSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(rightSockImg2, rightSockImg, 0, 174, OverCompositeOp);
        ScaledResize(rightSockImg2, 101, 284);
        rightSockImg2.crop(ScaledGeometry(101, 181));
        ScaledComposite(outputFileImg, rightSockImg2, 423 + 4, 593 + 2, OverCompositeOp);

        Image inputShirtImg(inputShirt);
        if (inputShirtImg.columns() > 1024 && GetSizeMode() == 1)
            inputShirtImg.resize(ScaledGeometry(1024, 1024));

        // left short sleeve
        Image slMask(options.KitsPath + (options.Overlay ? "kit_sleeve_mask_empty.png" : "kit_sleeve_mask.png"));
        {
            Image tmpSL1(inputShirtImg, ScaledGeometry(182, 318, 188, 20));
            ScaledComposite(tmpSL1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, ScaledGeometry(210, 318, 188, 351));
            ScaledComposite(tmpSL2, tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(ScaledGeometry(210, 425), options.Overlay? "transparent" : "black");
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 114, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, -204, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            ScaledResize(shortSleeveLeft, 317, 129);

            ScaledComposite(outputFileImg, shortSleeveLeft, -51, -3, OverCompositeOp);
        }
        // right short sleeve
        slMask.flop();
        {
            Image tmpSR1(inputShirtImg, ScaledGeometry(182, 318, 1024 - 188 - 182, 20));
            ScaledComposite(tmpSR1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSR2(inputShirtImg, ScaledGeometry(210, 318, 1024 - 188 - 210, 351));
            ScaledComposite(tmpSR2, tmpSR1, 210 - 182, 0, OverCompositeOp);

            Image shortSleeveRight(ScaledGeometry(210, 425), options.Overlay ? "transparent" : "black");
            ScaledComposite(shortSleeveRight, tmpSR2, 0, 114, OverCompositeOp);
            ScaledComposite(shortSleeveRight, tmpSR2, 0, -204, OverCompositeOp);
            shortSleeveRight.rotate(90);
            ScaledResize(shortSleeveRight, 317, 129);

            ScaledComposite(outputFileImg, shortSleeveRight, 512 + 45 + 4 - 317, -3, OverCompositeOp);
        }

        Image sleeveLeftSource(inputShirtImg, ScaledGeometry(415, 322, 0, 352));
        Image sleeveLeft(ScaledGeometry(415, 582), options.Overlay ? "transparent" : "black");
        ScaledComposite(sleeveLeft, sleeveLeftSource, 0, 0, OverCompositeOp);
        ScaledComposite(sleeveLeft, sleeveLeftSource, 0, 260, OverCompositeOp);
        sleeveLeft.rotate(-90);
        ScaledResize(sleeveLeft, 180, 281);
        ScaledComposite(outputFileImg, sleeveLeft, -63, 330, OverCompositeOp);

        Image sleeveRightSource(inputShirtImg, ScaledGeometry(415, 322, 1024 - 415, 352));
        Image sleeveRight(ScaledGeometry(415, 582), options.Overlay ? "transparent" : "black");
        ScaledComposite(sleeveRight, sleeveRightSource, 0, 0, OverCompositeOp);
        ScaledComposite(sleeveRight, sleeveRightSource, 0, 260, OverCompositeOp);
        sleeveRight.rotate(90);
        ScaledResize(sleeveRight, 180, 281);
        ScaledComposite(outputFileImg, sleeveRight, 512 + 65 - 180, 330, OverCompositeOp);

        Image shirtMainImg(inputShirtImg, ScaledGeometry(412, 900, 306, 62));
        ScaledResize(shirtMainImg, 492, 664);
        Image kitShirtMask(options.KitsPath + (options.Overlay ? "kit_shirt_mask_empty.png" : "kit_shirt_mask.png"));
        ScaledComposite(shirtMainImg, kitShirtMask, 0, 0, CopyAlphaCompositeOp);
        ScaledComposite(outputFileImg, shirtMainImg, 10, 348, OverCompositeOp);

        Image shortsImg(inputShortsImg, ScaledGeometry(1024, 302, 0, 210));
        ScaledResize(shortsImg, 512, 212);
        ScaledComposite(outputFileImg, shortsImg, 0, 127, OverCompositeOp);

        Image collarLeft(inputShirtImg, ScaledGeometry(6, 28, 474, 502));
        collarLeft.flip();
        ScaledResize(collarLeft, 41, 108);
        ScaledComposite(outputFileImg, collarLeft, 6, 129, OverCompositeOp);

        Image collarRight(inputShirtImg, ScaledGeometry(6, 28, 474, 502));
        collarRight.flip();
        ScaledResize(collarRight, 39, 111);
        ScaledComposite(outputFileImg, collarRight, 512 - 8 - 39, 126, OverCompositeOp);

        outputFileImg.fillColor(Color(inputShirtImg.pixelColor(150, 700)));
        outputFileImg.draw(ScaledDrawableRectangle(5, 465, 12, 598));
        outputFileImg.draw(ScaledDrawableRectangle(502, 463, 509, 599));

        if (!inputCrest.empty()) {
            Image inputCrestImg(inputCrest);
            ScaledResize(inputCrestImg, 44, 44);
            ScaledComposite(outputFileImg, inputCrestImg, 302, 728, OverCompositeOp);
        }

        if (!options.Overlay && options.AddKitOverlay) {
            Image kitOverlayImg(options.KitsPath + "kit_overlay.png");
            ScaledComposite(outputFileImg, kitOverlayImg, 0, 0, OverCompositeOp);
        }

        outputFileImg.fillColor("white");

        if (!options.Overlay && options.AddWatermarkText) {
            outputFileImg.fontPointsize(14);
            outputFileImg.fontWeight(550);
            outputFileImg.annotate("FIFAM Universal Converter Project", ScaledGeometry(1024, 1024, 3, 1012), NorthWestGravity, -90);
            outputFileImg.annotate("converted from EA SPORTS FIFA 19", ScaledGeometry(1024, 1024, 512 - 12, 1012), NorthWestGravity, -90);
        }

        outputFileImg.write(outputFile + "." + options.OutputFormat);
    }
    else {
        Image inputShirtImg(inputShirt);
        if (inputShirtImg.columns() > 1024) {
            SetSizeMode(2);
            if (GetSizeMode() == 1)
                inputShirtImg.resize(ScaledGeometry(1024, 1024));
        }
        Image outputFileImg(ScaledGeometry(512, 512), "white");

        // left short sleeve
        Image slMask = ScaledImage(options.KitsPath + "kit_sleeve_mask.png");
        {
            Image tmpSL1(inputShirtImg, ScaledGeometry(182, 318, 188, 20));
            ScaledComposite(tmpSL1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, ScaledGeometry(210, 318, 188, 351));
            ScaledComposite(tmpSL2, tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(ScaledGeometry(210, 390 + 95), "black");
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, -204, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 95, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 299 + 95, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            ScaledResize(shortSleeveLeft, 94, 68);

            ScaledComposite(outputFileImg, shortSleeveLeft, -13, 4, OverCompositeOp);

            //static int c = 1;
            //shortSleeveLeft.write(options.KitsPath + "test" + to_string(c++) + ".png");

            /*
            Image shortSleeveLeft(ScaledGeometry(210, 390), "black");
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 0, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 299, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            ScaledResize(shortSleeveLeft, 86, 70);

            ScaledComposite(outputFileImg, shortSleeveLeft, 0, 3, OverCompositeOp);*/
        }
        // right short sleeve
        slMask.flop();
        {
            Image tmpSR1(inputShirtImg, ScaledGeometry(182, 318, 1024 - 188 - 182, 20));
            ScaledComposite(tmpSR1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSR2(inputShirtImg, ScaledGeometry(210, 318, 1024 - 188 - 210, 351));
            ScaledComposite(tmpSR2, tmpSR1, 210 - 182, 0, OverCompositeOp);

            Image shortSleeveRight(ScaledGeometry(210, 390 + 95), "black");
            ScaledComposite(shortSleeveRight, tmpSR2, 0, -204, OverCompositeOp);
            ScaledComposite(shortSleeveRight, tmpSR2, 0, 95, OverCompositeOp);
            ScaledComposite(shortSleeveRight, tmpSR2, 0, 299 + 95, OverCompositeOp);
            shortSleeveRight.rotate(90);
            ScaledResize(shortSleeveRight, 94, 68);

            shortSleeveRight.flop();
            ScaledComposite(outputFileImg, shortSleeveRight, -13, 4 + 71, OverCompositeOp);

            ScaledComposite(outputFileImg, shortSleeveRight, -13, 4 + 327, OverCompositeOp);

            Image captainArmbandImg(inputShirtImg, ScaledGeometry(272, 46, 376, 976));
            ScaledResize(captainArmbandImg, 79, 11);
            captainArmbandImg.flop();
            ScaledComposite(outputFileImg, captainArmbandImg, 0, 372, OverCompositeOp);
        }

        // blank
        outputFileImg.fillColor(Color(inputShirtImg.pixelColor(150, 700)));
        outputFileImg.draw(ScaledDrawableRectangle(0, 261, 166, 261 + 70));
        outputFileImg.draw(ScaledDrawableRectangle(81, 332, 81 + 86, 332 + 71));

        Image shirtBackImg(inputShirtImg, ScaledGeometry(377, 480, 323, 68));
        ScaledResize(shirtBackImg, 350, 271);
        shirtBackImg.flip();
        shirtBackImg.flop();
        ScaledComposite(outputFileImg, shirtBackImg, 162, 256, OverCompositeOp);

        Image shirtMainImg(inputShirtImg, ScaledGeometry(434, 480, 294, 472));
        ScaledResize(shirtMainImg, 404, 260);
        ScaledComposite(outputFileImg, shirtMainImg, 135, 0, OverCompositeOp);

        Image collarFront(inputShirtImg, ScaledGeometry(200, 64, 56, 696));
        Image collarFrontMask = ScaledImage(options.KitsPath + "kit_collar_front_mask08.png");
        ScaledComposite(collarFront, collarFrontMask, 0, 0, CopyAlphaCompositeOp);
        ScaledComposite(outputFileImg, collarFront, 240, 6, OverCompositeOp);

        Image collarBack(inputShirtImg, ScaledGeometry(200, 64, 56, 696));
        Image collarBackMask = ScaledImage(options.KitsPath + "kit_collar_back_mask08.png");
        ScaledComposite(collarBack, collarBackMask, 0, 0, CopyAlphaCompositeOp);
        ScaledComposite(outputFileImg, collarBack, 236, 256, OverCompositeOp);

        Image inputShortsImg(inputShorts);
        if (inputShortsImg.columns() > 1024 && GetSizeMode() == 1)
            inputShortsImg.resize(ScaledGeometry(1024, 512));

        Image leftSockImg(inputShortsImg, ScaledGeometry(356, 174, 142, 5));
        Image leftSockImg2(ScaledGeometry(356, 174 * 3), "black");
        ScaledComposite(leftSockImg2, leftSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174 * 2, OverCompositeOp);
        leftSockImg2.rotate(-90);
        ScaledResize(leftSockImg2, 204, 82);
        leftSockImg2.flop();
        leftSockImg2.crop(ScaledGeometry(81, 69, 64, 0));
        ScaledComposite(outputFileImg, leftSockImg2, 81, 2, OverCompositeOp);

        Image rightSockImg(inputShortsImg, ScaledGeometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(ScaledGeometry(356, 174 * 3), "black");
        ScaledComposite(rightSockImg2, rightSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(rightSockImg2, rightSockImg, 0, 174, OverCompositeOp);
        ScaledComposite(rightSockImg2, rightSockImg, 0, 174 * 2, OverCompositeOp);
        rightSockImg2.rotate(90);
        ScaledResize(rightSockImg2, 204, 82);
        rightSockImg2.crop(ScaledGeometry(81, 69, 64, 0));
        ScaledComposite(outputFileImg, rightSockImg2, 81, 2 + 72, OverCompositeOp);

        Image shortsLeft(inputShortsImg, ScaledGeometry(435, 325, 77, 187));
        ScaledResize(shortsLeft, 161, 113);
        ScaledComposite(outputFileImg, shortsLeft, 0, 143, OverCompositeOp);

        Image shortsRight(inputShortsImg, ScaledGeometry(435, 325, 512, 187));
        ScaledResize(shortsRight, 163, 113);
        shortsRight.flop();
        ScaledComposite(outputFileImg, shortsRight, 0, 399, OverCompositeOp);

        if (!inputCrest.empty()) {
            Image inputCrestImg(inputCrest);
            ScaledResize(inputCrestImg, 40, 26);
            ScaledComposite(outputFileImg, inputCrestImg, 379, 54 + 7, OverCompositeOp);
        }

        if (options.AddKitOverlay) {
            Image kitWrinklesImg(options.KitsPath + "kit_wrinkles08.png");
            if (kitWrinklesImg.columns() != outputFileImg.columns())
                ScaledResize(kitWrinklesImg, outputFileImg.columns(), outputFileImg.rows());
            ScaledComposite(outputFileImg, kitWrinklesImg, 0, 0, OverCompositeOp);

            Image kitOverlayImg(options.KitsPath + "kit_overlay08.png");
            if (kitOverlayImg.columns() != outputFileImg.columns())
                ScaledResize(kitOverlayImg, outputFileImg.columns(), outputFileImg.rows());
            ScaledComposite(outputFileImg, kitOverlayImg, 0, 0, OverCompositeOp);
        }

        outputFileImg.write(outputFile + "." + options.OutputFormat);
    }

    return true;
}

bool KitConverter::ConvertFifaClubKit(int fifaId, string const &clubIdStr, int set, int variation, string const &outputFile) {
    string shirtFileName;
    string shortsFileName;
    string crestFileName;
    if ((options.AllowCustomKits || options.OnlyCustomKits) && !clubIdStr.empty()) {
        string kitType;
        if (set == 0)
            kitType = "_h";
        else if (set == 1)
            kitType = "_a";
        else if (set == 2)
            kitType = "_g";
        else if (set == 3)
            kitType = "_t";
        if (!kitType.empty()) {
            shirtFileName = options.CustomKitsPath + clubIdStr + "_j" + kitType + ".png";
            shortsFileName = options.CustomKitsPath + clubIdStr + "_s" + kitType + ".png";
            crestFileName = options.CustomKitsPath + clubIdStr + "_l" + kitType + ".png";
            if (!exists(shirtFileName) || !exists(shortsFileName)) {
                shirtFileName.clear();
                shortsFileName.clear();
                crestFileName.clear();
            }
            else {
                if (!exists(crestFileName)) {
                    crestFileName = options.CustomKitsPath + clubIdStr + "_l" + ".png";
                    if (!exists(crestFileName))
                        crestFileName.clear();
                }
            }
        }
    }
    if (fifaId != 0 && !options.OnlyCustomKits && shirtFileName.empty()) {
        string kitFileName = "kit_" + to_string(fifaId) + "_" + to_string(set) + "_" + to_string(variation) + ".png";
        shirtFileName = options.KitsPath + "shirts\\" + kitFileName;
        shortsFileName = options.KitsPath + "shorts\\" + kitFileName;
        if (set != 5)
            crestFileName = options.KitsPath + "crest\\" + kitFileName;
        if (!exists(shirtFileName) || !exists(shortsFileName)) {
            shirtFileName.clear();
            shortsFileName.clear();
            crestFileName.clear();
        }
        else if (!exists(crestFileName))
            crestFileName.clear();
    }
    if (!shirtFileName.empty() && !shortsFileName.empty()) {
        printf("Converting club %s (%d)", clubIdStr.c_str(), set);
        bool result = false;
        if (options.V2)
            result = ConvertKitV2(shirtFileName, shortsFileName, crestFileName, outputFile);
        else
            result = ConvertKit(shirtFileName, shortsFileName, crestFileName, outputFile);
        if (result) {
            printf(" - done\n");
            return true;
        }
        printf(" - failed\n");
    }
    return false;
}

bool KitConverter::ConvertFifaClubMiniKit(int fifaId, string const &clubIdStr, int set, int variation, string const &outputFile) {
    string miniKitPath;
    if ((options.AllowCustomKits || options.OnlyCustomKits) && !clubIdStr.empty()) {
        string kitType;
        if (set == 0)
            kitType = "_h";
        else if (set == 1)
            kitType = "_a";
        else if (set == 3)
            kitType = "_t";
        if (!kitType.empty())
            miniKitPath = options.CustomMiniKitsPath + clubIdStr + kitType + ".png";
    }
    if (fifaId != 0 && !options.OnlyCustomKits && (miniKitPath.empty() || !exists(miniKitPath)))
        miniKitPath = options.MiniKitsPath + "j" + to_string(set) + "_" + to_string(fifaId) + "_" + to_string(variation) + ".png";
    if (exists(miniKitPath)) {
        printf("Converting club %d (%d)", fifaId, set);
        std::error_code ec;
        if (copy_file(miniKitPath, outputFile + ".png", copy_options::overwrite_existing, ec)) {
            printf(" - done\n");
            return true;
        }
        else {
            printf(" - failed : %s\n", ec.message().c_str());
        }
    }
    return false;
}

void KitConverter::ConvertClubKits(string const &clubIdName, int fifaId, int fifaManagerId) {
    static char clubIdStr[256];
    if (options.OutputGameId >= 8)
        sprintf_s(clubIdStr, "%08X", fifaManagerId);
    else
        strcpy_s(clubIdStr, clubIdName.c_str());
    static char gameIdStr[10];
    sprintf_s(gameIdStr, "%02d", options.OutputGameId);
    string outputFile;
    if (options.SaveLocation == Big) {

    }
    else {
        if (!options.ConvertOnlyMinikits) {
            if (options.SaveLocation == Documents && options.OutputGameId >= 9)
                outputFile = string("C:\\Users\\Dmitri\\Documents\\FIFA MANAGER ") + gameIdStr + "\\Graphics\\3DMatch\\Kits\\";
            else {
                if (options.OutputGameId >= 9)
                    outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kits\\";
                else
                    outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\user\\kits\\";
            }
            outputFile += clubIdStr;
            if (options.ConvertHomeKit)
                ConvertFifaClubKit(fifaId, clubIdStr, 0, 0, outputFile + "_h");
            if (options.ConverAwayKit)
                ConvertFifaClubKit(fifaId, clubIdStr, 1, 0, outputFile + "_a");
            if (options.ConvertThirdKit)
                ConvertFifaClubKit(fifaId, clubIdStr, 3, 0, outputFile + "_t");
            if (options.ConvertGkKit)
                ConvertFifaClubKit(fifaId, clubIdStr, 2, 0, outputFile + "_g");
        }
        if (options.OutputGameId >= 13 && (options.ConvertMinikits || options.ConvertOnlyMinikits)) {
            outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\minikits\\" + clubIdStr;
            if (options.ConvertHomeKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 0, 0, outputFile + "_h");
            if (options.ConverAwayKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 1, 0, outputFile + "_a");
            if (options.ConvertThirdKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 3, 0, outputFile + "_t");
            if (options.ConvertGkKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 2, 0, outputFile + "_g");
        }
    }
}

void KitConverter::ConvertRefereeKit(int fifaId) {
    ConvertFifaClubKit(fifaId, std::to_string(fifaId), 5, 0, "D:\\Projects\\FIFA19\\ref_kits\\t75__5_0_" + std::to_string(fifaId));
}

bool KitConverter::ConvertClubArmband(int fifaId, string const &clubIdStr, int set, int variation, string const &outputFile) {
    string shirtFileName;
    if ((options.AllowCustomKits || options.OnlyCustomKits) && !clubIdStr.empty()) {
        string kitType;
        if (set == 0)
            kitType = "_h";
        else if (set == 1)
            kitType = "_a";
        else if (set == 2)
            kitType = "_g";
        else if (set == 3)
            kitType = "_t";
        if (!kitType.empty()) {
            shirtFileName = options.CustomKitsPath + clubIdStr + "_j" + kitType + ".png";
            if (!exists(shirtFileName)) {
                shirtFileName.clear();
            }
        }
    }
    if (fifaId != 0 && !options.OnlyCustomKits && shirtFileName.empty()) {
        string kitFileName = "kit_" + to_string(fifaId) + "_" + to_string(set) + "_" + to_string(variation) + ".png";
        shirtFileName = options.KitsPath + "shirts\\" + kitFileName;
        if (!exists(shirtFileName))
            shirtFileName.clear();
    }
    if (!shirtFileName.empty()) {
        printf("Converting armband %s (%d)\n", clubIdStr.c_str(), set);
        Image img(shirtFileName);
        if (img.columns() > 1024 && GetSizeMode() == 1)
            img.resize(ScaledGeometry(1024, 1024));
        img.crop(Geometry(249, 43, 387, 977));
        ScaledResize(img, 256, 64);
        img.write(outputFile + "." + options.OutputFormat);
    }
    return false;
}

void KitConverter::ConvertClubArmbands(string const &clubIdName, int fifaId, int fifaManagerId) {
    SetSizeMode(1);
    static char clubIdStr[256];
    sprintf_s(clubIdStr, "%08X", fifaManagerId);
    static char gameIdStr[10];
    sprintf_s(gameIdStr, "%02d", options.OutputGameId);
    string outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitarmband\\" + clubIdStr;
    ConvertClubArmband(fifaId, clubIdStr, 0, 0, outputFile + "_h");
    ConvertClubArmband(fifaId, clubIdStr, 1, 0, outputFile + "_a");
    ConvertClubArmband(fifaId, clubIdStr, 3, 0, outputFile + "_t");
    ConvertClubArmband(fifaId, clubIdStr, 2, 0, outputFile + "_g");
}

bool KitConverter::ConvertClubKitNumbersSet(int fifaId, string const &clubIdStr, int tournament, int type, bool jersey, string const &outputFile) {
    string fileNameBase = options.KitNumbersPath + "specifickitnumbers_" + to_string(fifaId) + "_"
        + to_string(jersey ? 1 : 2) + "_" + to_string(tournament) + "_" + to_string(type) + "_";
    string fileName = fileNameBase + "1.png";
    if (!exists(fileName))
        return false;
    static unsigned int j_numbers[] = { 2, 6, 4, 0, 8, 3, 7, 5, 1, 9 };
    static unsigned int s_numbers[] = { 1, 2, 8, 3, 9, 4, 7, 6, 5, 0 };
    const unsigned int size = 128;
    Image finalImg(ScaledGeometry(size * 10, size), "transparent");
    for (unsigned int i = 0; i < 10; i++) {
        fileName = fileNameBase + to_string((jersey ? j_numbers[i] : s_numbers[i]) + 1) + ".png";
        if (!exists(fileName))
            return false;
        Image img(fileName);
        if (!img.isValid())
            return false;
        ScaledComposite(finalImg, img, size * i, 0, OverCompositeOp);
    }
    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    finalImg.write(outputFile + "." + options.OutputFormat);
    return true;
}

void KitConverter::ConvertClubKitNumbers(string const &clubIdName, int fifaId, int fifaManagerId) {
    if (fifaId > 0) {
        SetSizeMode(1);
        static char clubIdStr[256];
        sprintf_s(clubIdStr, "%08X", fifaManagerId);
        static char gameIdStr[10];
        sprintf_s(gameIdStr, "%02d", options.OutputGameId);
        string outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\jersey\\" + clubIdStr;
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 0, true, outputFile + "_h");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 1, true, outputFile + "_a");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 3, true, outputFile + "_t");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 2, true, outputFile + "_g");
        string outputFileShorts = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\shorts\\" + clubIdStr;
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 0, false, outputFileShorts + "_h");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 1, false, outputFileShorts + "_a");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 3, false, outputFileShorts + "_t");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 2, false, outputFileShorts + "_g");
    }
}

bool KitConverter::ConvertClubKitNumbersSetCustom(string const &dirPath, string const &dirName, bool jersey) {
    if (dirPath.empty())
        return false;
    string dir = dirPath;
    if (dir.back() != '\\' && dir.back() != '/')
        dir += '\\';
    string fileName = dir + "0.png";
    if (!exists(fileName))
        return false;
    static unsigned int j_numbers[] = { 2, 6, 4, 0, 8, 3, 7, 5, 1, 9 };
    static unsigned int s_numbers[] = { 1, 2, 8, 3, 9, 4, 7, 6, 5, 0 };
    unsigned int size = 128;
    Image finalImg(ScaledGeometry(size * 10, size), "transparent");
    for (unsigned int i = 0; i < 10; i++) {
        fileName = dir + to_string(jersey ? j_numbers[i] : s_numbers[i]) + ".png";
        if (!exists(fileName))
            return false;
        Image img(fileName);
        if (!img.isValid())
            return false;
        ScaledResize(img, size, size);
        ScaledComposite(finalImg, img, size * i, 0, OverCompositeOp);
    }
    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    string finalDir = jersey ? "jersey\\" : "shorts\\";
    finalImg.write("D:\\Games\\FIFA Manager 13\\data\\kitnumbers_custom\\" + finalDir + dirName + "." + options.OutputFormat);
    return true;
}

void KitConverter::ConvertClubKitNumbersCustom() {
    path customDir = "D:\\Projects\\FIFA19\\custom_kitnumbers\\";
    for (auto const &p : directory_iterator(customDir)) {
        string folderPath = p.path().string();
        string folderName = p.path().filename().string();
        ConvertClubKitNumbersSetCustom(folderPath, folderName, true);
        ConvertClubKitNumbersSetCustom(folderPath, folderName, false);
    }
}

bool KitConverter::ConvertKitV2(string const &inputShirt, string const &inputShorts, string const &inputCrest, string const &outputFile) {
    SetSizeMode(options.Force2x ? 2 : 1);
    if (options.OutputGameId >= 9) {

        int texW = 512 * GetSizeMode();
        int texH = 1024 * GetSizeMode();

        Image fifaKitAllInOneImg(ScaledGeometry(1024, 1024 + 512), "red");

        Image fifaKitShirt(inputShirt);
        fifaKitShirt.resize(ScaledGeometry(1024, 1024));

        ScaledComposite(fifaKitAllInOneImg, fifaKitShirt, 0, 0, OverCompositeOp);

        Image fifaKitShorts(inputShorts);
        fifaKitShorts.resize(ScaledGeometry(1024, 512));

        ScaledComposite(fifaKitAllInOneImg, fifaKitShorts, 0, 1024, OverCompositeOp);

        fifaKitAllInOneImg.write(options.KitsPath + "tmpFifaKit.bmp");

        IDirect3DTexture9 *tex = mRenderer->CreateTexture(Utils::AtoW(options.KitsPath + "tmpFifaKit.bmp").c_str());

        if (!tex)
            ::Error(L"Failed to create tex");

        mRenderer->Begin();
        mRenderer->SetSourceTexture(tex);
        mRenderer->Interface()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        float positions[2 * 3];
        float uvs[2 * 3];

        positions[0] = 0; // 1 x
        positions[1] = 0; // 1 y

        positions[2] = texW; // 2 x
        positions[3] = 0; // 2 y

        positions[4] = 0; // 3 x
        positions[5] = texH; // 3 y

        uvs[0] = 0; // 1 u
        uvs[1] = 0; // 1 v

        uvs[2] = 1; // 2 u
        uvs[3] = 0; // 2 v

        uvs[4] = 0; // 3 u
        uvs[5] = 1; // 3 v

        //mRenderer->RenderTriangle(positions, uvs);

        // for all triangles in edited model
        //     find this triangle in source model by its xyz position
        //     if found
        //         draw triangle with vertex data:
        //             position x : transformed u from source model ([0,1]>[0,512])
        //             position y : transformed v from source model ([0,1]>[0,1024])
        //             tex u : u from edited model
        //             tex v : v from edited model

        // find tri in model
        auto findTri = [](Model::Triangle & out, Model::Triangle const &in, Model const &model) {
            for (auto &p : model.parts) {
                for (auto &t : p.tris) {
                    if (in.v[0].x == t.v[0].x && in.v[0].y == t.v[0].y && in.v[0].z == t.v[0].z
                        && in.v[1].x == t.v[1].x && in.v[1].y == t.v[1].y && in.v[1].z == t.v[1].z
                        && in.v[2].x == t.v[2].x && in.v[2].y == t.v[2].y && in.v[2].z == t.v[2].z)
                    {
                        out = t;
                        return true;
                    }
                }
            }
            return false;
        };

        std::vector<std::wstring> usedParts = {
            L"jersey_part_52", L"frontnumber_part_58", L"name_bottom_part_53", L"name_part_54", L"backnumber_part_55",
            L"shorts_back_l_part_56", L"shorts_back_r_part_57", L"shorts_l_part_59", L"shorts_r_part_60",
            L"socks_m2_part_20", L"collar_g_part_28", L"sleeve_short_l_part_3", L"sleeve_short_r_part_1", L"sleeve_long_l_part_2", L"sleeve_long_r_part_0"
        };

        unsigned int foundCount = 0;
        for (auto &p : mEditedModel.parts) {

            if (!Utils::Contains(usedParts, p.name))
                continue;

            for (auto &t : p.tris) {

                //::Error(L"%f %f %f - %f %f %f - %f %f %f", t.v[0].x, t.v[0].y, t.v[0].z, t.v[1].x, t.v[1].y, t.v[1].z, t.v[2].x, t.v[2].y, t.v[2].z);

                Model::Triangle tri;
                if (findTri(tri, t, mSourceModel)) {

                    positions[0] = tri.v[0].u * texW; // 1 x
                    positions[1] = float(texH) - tri.v[0].v * texH; // 1 y

                    positions[2] = tri.v[1].u * texW; // 2 x
                    positions[3] = float(texH) - tri.v[1].v * texH; // 2 y

                    positions[4] = tri.v[2].u * texW; // 3 x
                    positions[5] = float(texH) - tri.v[2].v * texH; // 3 y

                    uvs[0] = t.v[0].u; // 1 u
                    uvs[1] = 1.0f - t.v[0].v; // 1 v

                    uvs[2] = t.v[1].u; // 2 u
                    uvs[3] = 1.0f - t.v[1].v; // 2 v

                    uvs[4] = t.v[2].u; // 3 u
                    uvs[5] = 1.0f - t.v[2].v; // 3 v

                    mRenderer->RenderTriangle(positions, uvs);

                    foundCount++;
                }
            }
        }

        //::Error("found: %d", foundCount);

        mRenderer->End();
        mRenderer->SaveRT(Utils::AtoW(options.KitsPath + "tmpFifaKit2.png").c_str());
        mRenderer->DestroyTexture(tex);

        Image finalImg(options.KitsPath + "tmpFifaKit2.png");

        Image resultImg(Geometry(texW, texH), "black");
        int offset = 2;
        ScaledComposite(resultImg, finalImg, -offset, -offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, offset, offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, -offset, offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, offset, -offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, -offset, 0, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, offset, 0, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, 0, -offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, 0, offset, MagickCore::OverCompositeOp);
        ScaledComposite(resultImg, finalImg, 0, 0, MagickCore::OverCompositeOp);

        if (!inputCrest.empty()) {
            Image inputCrestImg(inputCrest);
            ScaledResize(inputCrestImg, 56, 56);
            ScaledComposite(resultImg, inputCrestImg, 298, 739, OverCompositeOp);
        }

        //if (!options.Force2x && !options.Overlay && options.AddKitOverlay) {
        //    Image kitOverlayImg(options.KitsPath + "kit_overlay.png");
        //    ScaledComposite(resultImg, kitOverlayImg, 0, 0, OverCompositeOp);
        //}

        resultImg.fillColor("white");

        if (!options.Overlay && options.AddWatermarkText) {
            resultImg.fontPointsize(14 * GetSizeMode());
            resultImg.fontWeight(550);
            resultImg.annotate("FIFAM Universal Converter Project", ScaledGeometry(1024, 1024, 3, 1012), NorthWestGravity, -90);
            resultImg.annotate("converted from EA SPORTS FIFA 20", ScaledGeometry(1024, 1024, 512 - 12, 1012), NorthWestGravity, -90);
        }

        resultImg.write(outputFile + ".tga");
    }

    return true;
}
