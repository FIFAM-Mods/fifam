#include "KitConverter.h"
#include <filesystem>
#include "Utils.h"
#include "Error.h"
#include "FifamReadWrite.h"
#include "FifaDataFile.h"

using namespace std;
using namespace std::filesystem;
using namespace Magick;

KitConverter::GlobalOptions KitConverter::options;

void KitConverter::resizeImage_noAspect(Image & image, int w, int h) {
    Geometry geom(w, h);
    geom.aspect(true);
    image.filterType(Magick::LanczosFilter);
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
    img.filterType(FilterType::LanczosFilter);
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
    if (mode > 1 && options.Allow2xSize) {
        SizeMode = mode;
    }
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

bool KitConverter::ConvertKit(string const &inputShirt, string const &inputShorts, string const &inputCrest, string const &outputFile, array<int, 8> logoPos, bool hasPositions) {
//#define LEGACY_CONVERTER
    SetSizeMode(1);
    if (options.OutputGameId >= 9) {
        
        Image inputShortsImg(inputShorts);
        if (inputShortsImg.columns() > 1024) {
            SetSizeMode(2);
            if (GetSizeMode() == 1) {
                inputShortsImg.filterType(FilterType::LanczosFilter);
                inputShortsImg.resize(ScaledGeometry(1024, 512));
            }
        }

        Image outputFileImg(ScaledGeometry(512, 1024), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(255, 0, 0, 255));

        Image leftSockImg(inputShortsImg, ScaledGeometry(356, 174, 142, 5));
        Image leftSockImg2(ScaledGeometry(356, 174 * 2), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
        ScaledComposite(leftSockImg2, leftSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174, OverCompositeOp);
        ScaledResize(leftSockImg2, 101, 284);
        leftSockImg2.crop(ScaledGeometry(101, 181));
        ScaledComposite(outputFileImg, leftSockImg2, -12, 593, OverCompositeOp);

        Image rightSockImg(inputShortsImg, ScaledGeometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(ScaledGeometry(356, 174 * 2), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
        ScaledComposite(rightSockImg2, rightSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(rightSockImg2, rightSockImg, 0, 174, OverCompositeOp);
        ScaledResize(rightSockImg2, 101, 284);
        rightSockImg2.crop(ScaledGeometry(101, 181));
        ScaledComposite(outputFileImg, rightSockImg2, 423 + 4, 593 + 2, OverCompositeOp);

        Image inputShirtImg(inputShirt);
        if (inputShirtImg.columns() > 1024 && GetSizeMode() == 1) {
            inputShirtImg.filterType(FilterType::LanczosFilter);
            inputShirtImg.resize(ScaledGeometry(1024, 1024));
        }

        bool addLogo = true;
    #ifndef LEGACY_CONVERTER
        if (!inputCrest.empty() && (logoPos[0] != 0 || logoPos[1] != 0 || logoPos[2] != 0 || logoPos[3] != 0)) {
            int logoLeft = logoPos[0];
            int logoTop = logoPos[1];
            int logoRight = logoPos[2];
            int logoBottom = logoPos[3];
            Image inputCrestImg(inputCrest);
            inputCrestImg.filterType(FilterType::LanczosFilter);
            inputCrestImg.resize(Geometry(logoRight - logoLeft, logoBottom - logoTop));
            ScaledComposite(inputShirtImg, inputCrestImg, logoLeft, logoTop, OverCompositeOp);
            addLogo = false;
        }
    #endif

        // left short sleeve
        Image slMask(options.KitsPath + (options.Overlay ? "kit_sleeve_mask_empty.png" : "kit_sleeve_mask.png"));
        {
            Image tmpSL1(inputShirtImg, ScaledGeometry(182, 318, 188, 20));
            ScaledComposite(tmpSL1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, ScaledGeometry(210, 318, 188, 351));
            ScaledComposite(tmpSL2, tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(ScaledGeometry(210, 425), options.Overlay? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
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

            Image shortSleeveRight(ScaledGeometry(210, 425), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
            ScaledComposite(shortSleeveRight, tmpSR2, 0, 114, OverCompositeOp);
            ScaledComposite(shortSleeveRight, tmpSR2, 0, -204, OverCompositeOp);
            shortSleeveRight.rotate(90);
            ScaledResize(shortSleeveRight, 317, 129);

            ScaledComposite(outputFileImg, shortSleeveRight, 512 + 45 + 4 - 317, -3, OverCompositeOp);
        }

        Image sleeveLeftSource(inputShirtImg, ScaledGeometry(415, 322, 0, 352));
        Image sleeveLeft(ScaledGeometry(415, 582), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
        ScaledComposite(sleeveLeft, sleeveLeftSource, 0, 0, OverCompositeOp);
        ScaledComposite(sleeveLeft, sleeveLeftSource, 0, 260, OverCompositeOp);
        sleeveLeft.rotate(-90);
        ScaledResize(sleeveLeft, 180, 281);
        ScaledComposite(outputFileImg, sleeveLeft, -63, 330, OverCompositeOp);

        Image sleeveRightSource(inputShirtImg, ScaledGeometry(415, 322, 1024 - 415, 352));
        Image sleeveRight(ScaledGeometry(415, 582), options.Overlay ? Magick::Color(0, 0, 0, 0) : Magick::Color(0, 0, 0, 255));
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

        outputFileImg.fillColor(Magick::Color(inputShirtImg.pixelColor(150, 700)));
        outputFileImg.draw(ScaledDrawableRectangle(5, 465, 12, 598));
        outputFileImg.draw(ScaledDrawableRectangle(502, 463, 509, 599));

        if (!inputCrest.empty() && addLogo) {
            Image inputCrestImg(inputCrest);
            ScaledResize(inputCrestImg, 44, 44);
            ScaledComposite(outputFileImg, inputCrestImg, 302, 728, OverCompositeOp);
        }

        if (!options.Overlay && options.AddKitOverlay) {
            Image kitOverlayImg(options.KitsPath + "kit_overlay.png");
            ScaledComposite(outputFileImg, kitOverlayImg, 0, 0, OverCompositeOp);
        }

        outputFileImg.fillColor(Magick::Color(255, 255, 255, 255));

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
            if (GetSizeMode() == 1) {
                inputShirtImg.filterType(FilterType::LanczosFilter);
                inputShirtImg.resize(ScaledGeometry(1024, 1024));
            }
        }

        bool addLogo = true;
#ifndef LEGACY_CONVERTER
        if (!inputCrest.empty() && (logoPos[0] != 0 || logoPos[1] != 0 || logoPos[2] != 0 || logoPos[3] != 0)) {
            int logoLeft = logoPos[0];
            int logoTop = logoPos[1];
            int logoRight = logoPos[2];
            int logoBottom = logoPos[3];
            Image inputCrestImg(inputCrest);
            inputCrestImg.filterType(FilterType::LanczosFilter);
            inputCrestImg.resize(Geometry(logoRight - logoLeft, logoBottom - logoTop));
            ScaledComposite(inputShirtImg, inputCrestImg, logoLeft, logoTop, OverCompositeOp);
            addLogo = false;
        }
#endif

        Image outputFileImg(ScaledGeometry(512, 512), Magick::Color(255, 255, 255, 255));

        // left short sleeve
        Image slMask = ScaledImage(options.KitsPath + "kit_sleeve_mask.png");
        {
            Image tmpSL1(inputShirtImg, ScaledGeometry(182, 318, 188, 20));
            ScaledComposite(tmpSL1, slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, ScaledGeometry(210, 318, 188, 351));
            ScaledComposite(tmpSL2, tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(ScaledGeometry(210, 390 + 95), Magick::Color(0, 0, 0, 255));
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, -204, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 95, OverCompositeOp);
            ScaledComposite(shortSleeveLeft, tmpSL2, 0, 299 + 95, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            ScaledResize(shortSleeveLeft, 94, 68);

            ScaledComposite(outputFileImg, shortSleeveLeft, -13, 4, OverCompositeOp);

            //static int c = 1;
            //shortSleeveLeft.write(options.KitsPath + "test" + to_string(c++) + ".png");

            /*
            Image shortSleeveLeft(ScaledGeometry(210, 390), Magick::Color(0, 0, 0, 255));
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

            Image shortSleeveRight(ScaledGeometry(210, 390 + 95), Magick::Color(0, 0, 0, 255));
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
        outputFileImg.fillColor(Magick::Color(inputShirtImg.pixelColor(150, 700)));
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
        if (inputShortsImg.columns() > 1024 && GetSizeMode() == 1) {
            inputShortsImg.filterType(FilterType::LanczosFilter);
            inputShortsImg.resize(ScaledGeometry(1024, 512));
        }

        Image leftSockImg(inputShortsImg, ScaledGeometry(356, 174, 142, 5));
        Image leftSockImg2(ScaledGeometry(356, 174 * 3), Magick::Color(0, 0, 0, 255));
        ScaledComposite(leftSockImg2, leftSockImg, 0, 0, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174, OverCompositeOp);
        ScaledComposite(leftSockImg2, leftSockImg, 0, 174 * 2, OverCompositeOp);
        leftSockImg2.rotate(-90);
        ScaledResize(leftSockImg2, 204, 82);
        leftSockImg2.flop();
        leftSockImg2.crop(ScaledGeometry(81, 69, 64, 0));
        ScaledComposite(outputFileImg, leftSockImg2, 81, 2, OverCompositeOp);

        Image rightSockImg(inputShortsImg, ScaledGeometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(ScaledGeometry(356, 174 * 3), Magick::Color(0, 0, 0, 255));
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

        if (!inputCrest.empty() && addLogo) {
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
    bool isCustom = false;
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
            isCustom = true;
            if (!exists(shirtFileName) || !exists(shortsFileName)) {
                shirtFileName.clear();
                shortsFileName.clear();
                crestFileName.clear();
                isCustom = false;
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
        crestFileName = options.KitsPath + "crest\\" + kitFileName;
        if (!exists(shirtFileName) || !exists(shortsFileName)) {
            shirtFileName.clear();
            shortsFileName.clear();
            crestFileName.clear();
        }
        else if (!exists(crestFileName))
            crestFileName.clear();
    }
    array<int, 8> logoPos = {};
    bool hasPositions = false;
    auto ValidatePositions = [](int &min, int &max) {
        if (min > max)
            std::swap(min, max);
    };
    if (isCustom) {
        FifamReader posReader(Utils::AtoW(options.CustomKitsPath + "kit_" + clubIdStr + "_" + to_string(set) + "_0.txt"), 14, false, false);
        if (posReader.Available()) {
            logoPos[0] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[1] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[2] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[3] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            posReader.SkipLines(4 * 5);
            logoPos[4] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[5] = int(roundf(posReader.ReadLine<float>() * 512.0f));
            logoPos[6] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[7] = int(roundf(posReader.ReadLine<float>() * 512.0f));
            ValidatePositions(logoPos[0], logoPos[2]);
            ValidatePositions(logoPos[1], logoPos[3]);
            ValidatePositions(logoPos[4], logoPos[6]);
            ValidatePositions(logoPos[5], logoPos[7]);
            hasPositions = true;
        }
    }
    else if (fifaId != 0) {
        FifamReader posReader(Utils::AtoW(options.KitsPath + "positions\\kit_" + to_string(fifaId) + "_" + to_string(set) + "_0.txt"), 14, false, false);
        if (posReader.Available()) {
            logoPos[0] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[1] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[2] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[3] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            posReader.SkipLines(4 * 5);
            logoPos[4] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[5] = int(roundf(posReader.ReadLine<float>() * 512.0f));
            logoPos[6] = int(roundf(posReader.ReadLine<float>() * 1024.0f));
            logoPos[7] = int(roundf(posReader.ReadLine<float>() * 512.0f));
            ValidatePositions(logoPos[0], logoPos[2]);
            ValidatePositions(logoPos[1], logoPos[3]);
            ValidatePositions(logoPos[4], logoPos[6]);
            ValidatePositions(logoPos[5], logoPos[7]);
            hasPositions = true;
        }
    }
    if (!shirtFileName.empty() && !shortsFileName.empty()) {
        printf("Converting kit %s (%d)", clubIdStr.c_str(), set);
        bool result = false;
        if (options.V2)
            result = ConvertKitV2(shirtFileName, shortsFileName, crestFileName, outputFile, logoPos, hasPositions);
        else
            result = ConvertKit(shirtFileName, shortsFileName, crestFileName, outputFile, logoPos, hasPositions);
        if (result) {
            printf(" - done\n");
            return true;
        }
        printf(" - failed\n");
    }
    return false;
}

bool KitConverter::ConvertFifaClubMiniKit(int fifaId, string const &clubIdStr, int set, int variation, string const &outputFile) {
    Path miniKitPath;
    if ((options.AllowCustomKits || options.OnlyCustomKits) && !clubIdStr.empty()) {
        string kitType;
        if (set == 0)
            kitType = "_h";
        else if (set == 1)
            kitType = "_a";
        else if (set == 3)
            kitType = "_t";
        if (!kitType.empty())
            miniKitPath = options.CustomMiniKitsPath + clubIdStr + kitType + ".dds";
    }
    if (fifaId != 0 && !options.OnlyCustomKits && (miniKitPath.empty() || !exists(miniKitPath)))
        miniKitPath = options.MiniKitsPath + "j" + to_string(set) + "_" + to_string(fifaId) + "_" + to_string(variation) + ".dds";
    if (!exists(miniKitPath))
        miniKitPath.replace_extension(".png");
    if (exists(miniKitPath)) {
        printf("Converting minikit %s (%d)", clubIdStr.c_str(), set);
        Image mini(miniKitPath.string());
        if (mini.isValid()) {
            mini.defineValue("png", "color-type", "6");
            mini.write(outputFile + ".png");
            printf(" - done\n");
            return true;
        }
        printf(" - failed\n");
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
    if (!options.ConvertOnlyMinikits) {
        outputFile = "output\\graphics\\kits\\";
        create_directories(outputFile);
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
    if ((fifaManagerId & 0xFFFF) != 0xFFFF && options.OutputGameId >= 13 && (options.ConvertMinikits || options.ConvertOnlyMinikits)) {
        outputFile = "output\\graphics\\minikits\\";
        create_directories(outputFile);
        outputFile += clubIdStr;
        if (options.ConvertHomeKit)
            ConvertFifaClubMiniKit(fifaId, clubIdStr, 0, 0, outputFile + "_h");
        if (options.ConverAwayKit)
            ConvertFifaClubMiniKit(fifaId, clubIdStr, 1, 0, outputFile + "_a");
    }
}

void KitConverter::ConvertRefereeKit(int fifaId) {
    StringA refereeKitsPath = "D:\\Projects\\FIFA\\kit\\referee_kits\\";
    create_directories(refereeKitsPath);
    ConvertFifaClubKit(fifaId, std::to_string(fifaId), 5, 0, refereeKitsPath + "tp00@t75__5_0_" + std::to_string(fifaId));
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
        string kitFileName = "shirts\\kit_" + to_string(fifaId) + "_" + to_string(set) + "_" + to_string(variation) + ".png";
        shirtFileName = options.KitsPath + kitFileName;
        if (!exists(shirtFileName))
            shirtFileName.clear();
    }
    if (!shirtFileName.empty()) {
        printf("Converting armband %s (%d)\n", clubIdStr.c_str(), set);
        Image img(shirtFileName);
        img.backgroundColor(Magick::Color(0, 0, 0));
        img.alpha(false);
        if (img.columns() > 1024 && GetSizeMode() == 1) {
            img.filterType(FilterType::LanczosFilter);
            img.resize(ScaledGeometry(1024, 1024));
        }
        img.crop(Geometry(249, 43, 387, 977));
        ScaledResize(img, 256, 64);
        img.write(outputFile + "." + options.OutputFormat);
    }
    return false;
}

void KitConverter::ConvertClubArmbands(string const &clubIdName, int fifaId, int fifaManagerId) {
    if (options.OutputGameId < 9)
        return;
    SetSizeMode(1);
    static char clubIdStr[256];
    sprintf_s(clubIdStr, "%08X", fifaManagerId);
    static char gameIdStr[10];
    sprintf_s(gameIdStr, "%02d", options.OutputGameId);
    string outputFile = "output\\graphics\\kitarmband\\";
    create_directories(outputFile);
    outputFile += clubIdStr;
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
    Image finalImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
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

bool KitConverter::ConvertClubKitNumbersSet(int kitnumersId, bool jersey, std::string const &outputFile, ::Color const &clr1, ::Color const &clr2, ::Color const &clr3) {
    string fileNameBase = options.KitNumbersPath + "numbers_" + to_string(kitnumersId) + "_";
    string fileName = fileNameBase + "1_color.png";
    if (!exists(fileName)) {
        //::Error("File not exists: " + fileName);
        return false;
    }
    //else
    //    ::Error("File exists: " + fileName);
    static unsigned int j_numbers[] = { 2, 6, 4, 0, 8, 3, 7, 5, 1, 9 };
    static unsigned int s_numbers[] = { 1, 2, 8, 3, 9, 4, 7, 6, 5, 0 };
    const unsigned int WIDTH = 256;
    const unsigned int HEIGHT = 512;
    const unsigned int TOTAL_WIDTH = WIDTH * 10;
    Image channelsImg(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(0, 0, 0, 0));
    for (unsigned int i = 0; i < 10; i++) {
        fileName = fileNameBase + to_string((jersey ? j_numbers[i] : s_numbers[i]) + 0) + "_color.png";
        if (!exists(fileName))
            return false;
        Image img(fileName);
        if (!img.isValid())
            return false;
        ScaledResize(img, WIDTH, HEIGHT);
        ScaledComposite(channelsImg, img, WIDTH * i, 0, OverCompositeOp);
    }
    auto imgR = channelsImg.separate(MagickCore::ChannelType::RedChannel);
    auto imgG = channelsImg.separate(MagickCore::ChannelType::GreenChannel);
    auto imgB = channelsImg.separate(MagickCore::ChannelType::BlueChannel);
    auto imgA = channelsImg.separate(MagickCore::ChannelType::AlphaChannel);
    Image channelR(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(clr1.r, clr1.g, clr1.b));
    Image channelG(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(clr2.r, clr2.g, clr2.b));
    Image channelB(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(clr3.r, clr3.g, clr3.b));
    ScaledComposite(channelR, imgR, 0, 0, MultiplyCompositeOp);
    ScaledComposite(channelG, imgG, 0, 0, MultiplyCompositeOp);
    ScaledComposite(channelB, imgB, 0, 0, MultiplyCompositeOp);
    Image finalImg(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(0, 0, 0, 0));
    ScaledComposite(finalImg, channelR, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, channelG, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, channelB, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, imgA, 0, 0, CopyAlphaCompositeOp);
    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    //::Error("Writing: " + outputFile + "." + options.OutputFormat);
    finalImg.write(outputFile + "." + options.OutputFormat);
    //::Error("Wrote: " + outputFile + "." + options.OutputFormat);
    return true;
}

void KitConverter::ConvertClubKitNumbers(int kitnumberId, int fifaManagerId, ::Color const &clr1, ::Color const &clr2, ::Color const &clr3) {
    if (kitnumberId > 0) {
        SetSizeMode(1);
        static char clubIdStr[256];
        sprintf_s(clubIdStr, "%08X", fifaManagerId);
        static char gameIdStr[10];
        sprintf_s(gameIdStr, "%02d", options.OutputGameId);
        string outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\jersey\\" + clubIdStr;
        ConvertClubKitNumbersSet(kitnumberId, true, outputFile + "_h", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, true, outputFile + "_a", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, true, outputFile + "_t", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, true, outputFile + "_g", clr1, clr2, clr3);
        string outputFileShorts = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\shorts\\" + clubIdStr;
        ConvertClubKitNumbersSet(kitnumberId, false, outputFileShorts + "_h", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, false, outputFileShorts + "_a", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, false, outputFileShorts + "_t", clr1, clr2, clr3);
        ConvertClubKitNumbersSet(kitnumberId, false, outputFileShorts + "_g", clr1, clr2, clr3);
    }
}

void KitConverter::ConvertClubKitNumbers(int fifaId, int fifaManagerId) {
    if (fifaId > 0) {
        SetSizeMode(1);
        static char clubIdStr[256];
        sprintf_s(clubIdStr, "%08X", fifaManagerId);
        static char gameIdStr[10];
        sprintf_s(gameIdStr, "%02d", options.OutputGameId);
        string outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\jersey\\" + clubIdStr;
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 0, true, outputFile + "_h");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 1, true, outputFile + "_a");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 3, true, outputFile + "_t");
        ConvertClubKitNumbersSet(fifaId, clubIdStr, 0, 2, true, outputFile + "_g");
        string outputFileShorts = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitnumbers\\shorts\\" + clubIdStr;
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
    const unsigned int WIDTH = 256;
    const unsigned int HEIGHT = 512;
    const unsigned int TOTAL_WIDTH = WIDTH * 10;
    Image finalImg(ScaledGeometry(TOTAL_WIDTH, HEIGHT), Magick::Color(0, 0, 0, 0));
    for (unsigned int i = 0; i < 10; i++) {
        fileName = dir + to_string(jersey ? j_numbers[i] : s_numbers[i]) + ".png";
        if (!exists(fileName))
            return false;
        Image img(fileName);
        if (!img.isValid())
            return false;
        ScaledResize(img, WIDTH, HEIGHT);
        ScaledComposite(finalImg, img, WIDTH * i, 0, OverCompositeOp);
    }
    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    string outputDir = string("output\\graphics\\kitnumbers_custom\\") + (jersey ? "jersey\\" : "shorts\\");
    create_directories(outputDir);
    finalImg.write(outputDir + dirName + "." + options.OutputFormat);
    return true;
}

void KitConverter::ConvertClubKitNumbersCustom() {
    path customDir = "D:\\Projects\\FIFA\\kit\\custom\\kitnumbers\\";
    for (auto const &p : directory_iterator(customDir)) {
        string folderPath = p.path().string();
        string folderName = p.path().filename().string();
        ConvertClubKitNumbersSetCustom(folderPath, folderName, true);
        ConvertClubKitNumbersSetCustom(folderPath, folderName, false);
    }
}

bool KitConverter::ConvertKitV2(string const &inputShirt, string const &inputShorts, string const &inputCrest, string const &outputFile, array<int, 8> logoPos, bool hasPositions) {
    SetSizeMode(options.Force2x ? 2 : 1);
    if (options.OutputGameId >= 9) {

        int texW = 512 * GetSizeMode();
        int texH = 1024 * GetSizeMode();

        Image fifaKitAllInOneImg(ScaledGeometry(1024, 1024 + 512), Magick::Color(255, 0, 0, 255));

        Image fifaKitShirt(inputShirt);
        fifaKitShirt.alpha(false);
        fifaKitShirt.filterType(FilterType::LanczosFilter);
        fifaKitShirt.resize(ScaledGeometry(1024, 1024));

        if (hasPositions && !inputCrest.empty()) {
            if ((logoPos[0] != 0 || logoPos[1] != 0 || logoPos[2] != 0 || logoPos[3] != 0) && logoPos[0] != logoPos[2]) {
                int logoLeft = logoPos[0];
                int logoTop = logoPos[1];
                int logoRight = logoPos[2];
                int logoBottom = logoPos[3];
                if ((logoRight - logoLeft) != 0 && (logoBottom - logoTop) != 0) {
                    Image inputCrestImg(inputCrest);
                    ScaledResize(inputCrestImg, logoRight - logoLeft, logoBottom - logoTop);
                    ScaledComposite(fifaKitShirt, inputCrestImg, logoLeft, logoTop, OverCompositeOp);
                }
            }
        }

        ScaledComposite(fifaKitAllInOneImg, fifaKitShirt, 0, 0, OverCompositeOp);

        Image fifaKitShorts(inputShorts);
        fifaKitShorts.alpha(false);
        fifaKitShorts.filterType(FilterType::LanczosFilter);
        fifaKitShorts.resize(ScaledGeometry(1024, 512));

        if (hasPositions && !inputCrest.empty()) {
            if ((logoPos[4] != 0 || logoPos[5] != 0 || logoPos[6] != 0 || logoPos[7] != 0) && logoPos[4] != logoPos[6]) {
                int logoLeft = logoPos[4];
                int logoTop = logoPos[5];
                int logoRight = logoPos[6];
                int logoBottom = logoPos[7];
                if ((logoRight - logoLeft) != 0 && (logoBottom - logoTop) != 0) {
                    Image inputCrestImg(inputCrest);
                    ScaledResize(inputCrestImg, logoRight - logoLeft, logoBottom - logoTop);
                    ScaledComposite(fifaKitShorts, inputCrestImg, logoLeft, logoTop, OverCompositeOp);
                }
            }
        }

        ScaledComposite(fifaKitAllInOneImg, fifaKitShorts, 0, 1024, OverCompositeOp);

        fifaKitAllInOneImg.write(options.KitsPath + "tmpFifaKit.bmp");

        IDirect3DTexture9 *tex = mRenderer->CreateTexture(Utils::AtoW(options.KitsPath + "tmpFifaKit.bmp").c_str());

        if (!tex)
            ::Error(L"Failed to create tex");

        mRenderer->Begin();
        mRenderer->SetSourceTexture(tex);
        mRenderer->Interface()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        mRenderer->Interface()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        mRenderer->Interface()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        mRenderer->Interface()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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
            L"shorts_back_l_part_56", L"shorts_back_r_part_57", L"shorts_l_part_59", L"shorts_r_part_60", //L"socks_low_part_24",
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
        path savePath = (outputFile + ".png");
        mRenderer->SaveRT(savePath.c_str());
        mRenderer->DestroyTexture(tex);
        
        //if (!hasPositions && !inputCrest.empty()) {
        //    Image resultImg(outputFile + ".tga");
        //    Image inputCrestImg(inputCrest);
        //    ScaledResize(inputCrestImg, 56, 56);
        //    ScaledComposite(resultImg, inputCrestImg, 298, 719, OverCompositeOp);
        //    resultImg.flip();
        //    resultImg.write(outputFile + ".tga");
        //}

        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        path inpaintPath = "tools\\inpaint.exe";
        wstring commandLine = inpaintPath.wstring() + L" -i \"" + savePath.c_str() + L"\" -radius 0 -ns";
        WCHAR wargs[2048];
        wcscpy_s(wargs, commandLine.c_str());
        if (CreateProcessW(inpaintPath.c_str(), wargs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        
        Image resultImg(savePath.string());
        resizeImage_noAspect(resultImg, 512, 1024);
        resultImg.write(outputFile + ".tga");
        remove(savePath);
    }

    return true;
}

Image ConvertNewGenBannerToOldGen(Image const &inImg) {
    Image layout(Geometry(1024, 512), Magick::Color(0, 0, 0, 0));
    auto LayoutComposite = [&](Int dstX, Int dstY, Int dstW, Int dstH, Int srcX, Int srcY, Int srcW, Int srcH) {
        Image tmpImg(inImg, Geometry(srcW, srcH, srcX, srcY));
        if (dstW != srcW || dstH != srcH) {
            Geometry geom(dstW, dstH);
            geom.aspect(true);
            tmpImg.filterType(Magick::LanczosFilter);
            tmpImg.resize(geom);
        }
        layout.composite(tmpImg, dstX, dstY, CopyCompositeOp);
    };
    LayoutComposite(256 * 0, 128 * 0, 256, 128, 0, 0, 512, 256);
    LayoutComposite(256 * 1, 128 * 0, 256, 128, 512, 256, 256, 128);
    LayoutComposite(256 * 2, 128 * 0, 256, 128, 768, 256, 256, 128);
    LayoutComposite(256 * 3, 128 * 0, 256, 128, 512, 384, 256, 128);
    LayoutComposite(256 * 0, 128 * 1, 256, 128, 768, 384, 256, 128);
    LayoutComposite(256 * 1, 128 * 1, 256, 128, 768, 512, 256, 128);
    LayoutComposite(256 * 2, 128 * 1, 256, 128, 768, 640, 256, 128);
    LayoutComposite(256 * 3, 128 * 1, 256, 128, 0, 0, 512, 256);
    LayoutComposite(256 * 0, 128 * 2, 256, 128, 512, 256, 256, 128);
    LayoutComposite(256 * 1, 128 * 2, 256, 128, 768, 256, 256, 128);
    LayoutComposite(256 * 2, 128 * 2, 256, 128, 512, 384, 256, 128);
    LayoutComposite(256 * 3, 128 * 2, 256, 128, 768, 384, 256, 128);
    LayoutComposite(512 * 0, 128 * 3, 512, 128, 0, 384, 512, 128);
    LayoutComposite(512 * 1, 128 * 3, 512, 128, 0, 256, 512, 128);
    return layout;
}

void KitConverter::ConvertBanners(int fifaId, int fifaManagerId, Magick::Color const &primCol, Magick::Color const &secCol) {
    const path assetsDir = "D:\\FIFA_ASSETS";
    const path gameDir = "D:\\Games\\FIFA Manager 25";
    auto bannersFilename = assetsDir / "banners" / ("banner_" + std::to_string(fifaId) + "_color.png");
    auto crestFilename = assetsDir / "crest" / ("l" + std::to_string(fifaId) + ".png");
    auto flag1Filename = assetsDir / "flags" / ("flag_" + std::to_string(fifaId) + "_0_color.png");
    auto flag2Filename = assetsDir / "flags" / ("flag_" + std::to_string(fifaId) + "_1_color.png");
    auto flag3Filename = assetsDir / "flags" / ("flag_" + std::to_string(fifaId) + "_2_color.png");
    auto flag4Filename = assetsDir / "flags" / ("flag_" + std::to_string(fifaId) + "_3_color.png");
    if (exists(bannersFilename) && exists(crestFilename) && exists(flag1Filename) && exists(flag2Filename) && exists(flag3Filename) && exists(flag4Filename)) {
        path outputDir = gameDir / "data" / "banners" / Utils::Format("%08X", fifaManagerId);
        create_directories(outputDir);
        Image bannersImg(bannersFilename.string());
        if (bannersImg.columns() == 1024 && bannersImg.rows() == 1024)
            bannersImg = ConvertNewGenBannerToOldGen(bannersImg);
        bannersImg.type(ImageType::TrueColorAlphaType);
        bannersImg.write((outputDir / "banners.tga").string());
        auto saveFlagImage = [&](Path const &flagPath, string const &fileName) {
            Image flagImg(flagPath.string());
            resizeImage_noAspect(flagImg, 128, 128);
            auto flagOverlayFilename = assetsDir / "flags" / "flag_overlay.png";
            if (exists(flagOverlayFilename)) {
                Image flagOverlayImg(flagOverlayFilename.string());
                flagImg.composite(flagOverlayImg, 0, 0, MultiplyCompositeOp);
            }
            flagImg.type(ImageType::TrueColorAlphaType);
            flagImg.write((outputDir / fileName).string());
        };
        saveFlagImage(flag1Filename, "flag1.tga");
        saveFlagImage(flag2Filename, "flag2.tga");
        saveFlagImage(flag3Filename, "flag3.tga");
        saveFlagImage(flag4Filename, "flag4.tga");
        Image crestImg(crestFilename.string());
        crestImg.type(ImageType::TrueColorAlphaType);
        crestImg.write((outputDir / "crest.tga").string());
        Image primImg(Geometry(4, 4), primCol);
        primImg.type(ImageType::TrueColorAlphaType);
        primImg.write((outputDir / "primary.tga").string());
        Image secImg(Geometry(4, 4), secCol);
        secImg.type(ImageType::TrueColorAlphaType);
        secImg.write((outputDir / "secondary.tga").string());
        Image cornerFlagImg(Geometry(128, 128), primCol);
        crestImg.filterType(Magick::LanczosFilter);
        crestImg.resize(Geometry(80, 80));
        cornerFlagImg.composite(crestImg, (cornerFlagImg.columns() - crestImg.columns()) / 2, (cornerFlagImg.rows() - crestImg.rows()) / 2, OverCompositeOp);
        auto flagOverlayFilename = assetsDir / "flags" / "corner_flag_overlay.png";
        if (exists(flagOverlayFilename)) {
            Image cornerFlagOverlayImg(flagOverlayFilename.string());
            cornerFlagImg.composite(cornerFlagOverlayImg, 0, 0, MultiplyCompositeOp);
        }
        cornerFlagImg.type(ImageType::TrueColorAlphaType);
        cornerFlagImg.write((outputDir / "cornerflag.tga").string());
    }
}

void KitConverter::ConvertBannersFIFA(int fifaId, bool fifa14stadiums, Magick::Color const &primCol, Magick::Color const &secCol) {
    const path assetsDir = "I:\\FIFA_ASSETS";
    auto bannersFilename = assetsDir / "banners" / "final" / ("banner_" + std::to_string(fifaId) + "_texture.dds");
    if (exists(bannersFilename)) {
        Path gameBannersDir = "output\\fifa07";
        if (fifa14stadiums)
            gameBannersDir /= "banners_fifa14stadiums";
        else
            gameBannersDir /= "banners";
        Image bannersImg(bannersFilename.string());
        if (bannersImg.columns() != 1024 || bannersImg.rows() != 512)
            resizeImage_noAspect(bannersImg, 1024, 512);
        Path outputDir = gameBannersDir / ("t13__" + to_string(fifaId) + "_0_0");
        create_directories(outputDir);
        auto WriteBanner = [&](Path const &file, Image const &image, UInt x, UInt y) {
            Image copy(image, Geometry(256, 128, x, y));
            copy.write(file.string());
        };
        if (fifa14stadiums) {
            bannersImg.write((outputDir / "_bna.tga").string());
            Image primImg(Geometry(4, 4), primCol);
            primImg.write((outputDir / "_bnb.tga").string());
            Image secImg(Geometry(4, 4), secCol);
            secImg.write((outputDir / "_bnc.tga").string());
        }
        else {
            WriteBanner(outputDir / "_bna.tga", bannersImg, 256 * 0, 128 * 0);
            WriteBanner(outputDir / "_bnb.tga", bannersImg, 256 * 1, 128 * 0);
            WriteBanner(outputDir / "_bnc.tga", bannersImg, 256 * 2, 128 * 0);
        }
        WriteBanner(outputDir / "_fla.tga", bannersImg, 256 * 3, 128 * 0);
        WriteBanner(outputDir / "_flb.tga", bannersImg, 256 * 0, 128 * 1);
        WriteBanner(outputDir / "_flc.tga", bannersImg, 256 * 1, 128 * 1);
    }
}

void KitConverter::ConvertBannersFIFA_NewFormat(int fifaId, bool fifa14stadiums, Magick::Color const &primCol, Magick::Color const &secCol) {
    const path assetsDir = "I:\\FIFA_ASSETS";
    auto bannersFilename = assetsDir / "banners" / "24" / ("banner_" + std::to_string(fifaId) + "_texture.dds");
    if (exists(bannersFilename)) {
        Path gameBannersDir = "output\\fifa07";
        if (fifa14stadiums)
            gameBannersDir /= "banners_fifa14stadiums";
        else
            gameBannersDir /= "banners";
        Path outputDir = gameBannersDir / ("t13__" + to_string(fifaId) + "_0_0");
        create_directories(outputDir);
        auto WriteBanner = [&](Path const &file, Image const &image, UInt x, UInt y, UInt w, UInt h) {
            Image copy(image, Geometry(w, h, x, y));
            if (w != 256 || h != 128)
                resizeImage_noAspect(copy, 256, 128);
            copy.write(file.string());
        };
        Image bannersImg(bannersFilename.string());
        if (fifa14stadiums) {
            bannersImg = ConvertNewGenBannerToOldGen(bannersImg);
            bannersImg.write((outputDir / "_bna.tga").string());
            Image primImg(Geometry(4, 4), primCol);
            primImg.write((outputDir / "_bnb.tga").string());
            Image secImg(Geometry(4, 4), secCol);
            secImg.write((outputDir / "_bnc.tga").string());
        }
        else {
            WriteBanner(outputDir / "_bna.tga", bannersImg, 0,   0,   512, 256);
            WriteBanner(outputDir / "_bnb.tga", bannersImg, 512, 256, 256, 128);
            WriteBanner(outputDir / "_bnc.tga", bannersImg, 768, 256, 256, 128);
        }
        WriteBanner(outputDir / "_fla.tga", bannersImg, 512, 512, 256, 128);
        WriteBanner(outputDir / "_flb.tga", bannersImg, 768, 512, 256, 128);
        WriteBanner(outputDir / "_flc.tga", bannersImg, 768, 640, 256, 128);
    }
}

void KitConverter::ConvertAdboards(Map<UInt, UInt> const &fifaClubToFifam, Map<UInt, Vector<UInt>> const &compsMap) {
    const Path inPath = "D:\\FIFA_ASSETS\\fc25\\Worlds\\adboard\\textures";
    const Path outPath = "D:\\Projects\\fifam\\content\\fm13\\art_01\\sponsors\\512x1024";
    create_directories(outPath);
    Map<UInt, Path> adboards;
    Set<UInt> writtenAdboards;
    for (auto const &i : recursive_directory_iterator(inPath)) {
        auto fileName = i.path().stem().string();
        if (is_regular_file(i.path()) && Utils::StartsWith(fileName, "adboard_") && Utils::EndsWith(fileName, "_color"))
            adboards[Utils::SafeConvertInt<UInt>(fileName.substr(8, fileName.size() - 14))] = i.path();
    }
    if (Utils::Contains(adboards, 0u)) {
        Image img(adboards[0].string());
        img.type(TrueColorAlphaType);
        resizeImage_noAspect(img, 512, 1024);
        img.write((outPath / L"00000000.tga").string());
        writtenAdboards.insert(0);
    }
    for (auto const &[fifaId, fifamIds] : compsMap) {
        if (Utils::Contains(adboards, fifaId) && !fifamIds.empty()) {
            Image img(adboards[fifaId].string());
            img.type(TrueColorAlphaType);
            resizeImage_noAspect(img, 512, 1024);
            String fileName = Utils::Format((fifamIds[0] < 0xFFFF) ? L"%04X.tga" : L"%08X.tga", fifamIds[0]);
            Path filePath = outPath / fileName;
            img.write(filePath.string());
            for (UInt i = 1; i < fifamIds.size(); i++)
                copy(filePath, outPath / Utils::Format((fifamIds[i] < 0xFFFF) ? L"%04X.tga" : L"%08X.tga", fifamIds[i]), copy_options::overwrite_existing);
            writtenAdboards.insert(fifaId);
        }
    }
    Image bundesliga, bundesliga2;
    Bool hasBundesliga = false;
    if (Utils::Contains(adboards, 19u) && Utils::Contains(adboards, 20u)) {
        bundesliga = Image(adboards[19].string(), Geometry(1024, 128));
        resizeImage_noAspect(bundesliga, bundesliga.columns() / 2, bundesliga.rows() / 2);
        bundesliga2 = Image(adboards[20].string(), Geometry(1024, 128));
        resizeImage_noAspect(bundesliga2, bundesliga2.columns() / 2, bundesliga2.rows() / 2);
        hasBundesliga = true;
    }
    for (auto const &[fifaId, fifamId] : fifaClubToFifam) {
        if (Utils::Contains(adboards, 1000000u + fifaId)) {
            Image img(adboards[1000000 + fifaId].string());
            img.type(TrueColorAlphaType);
            resizeImage_noAspect(img, 512, 1024);
            UInt countryId = (fifamId >> 16) & 0xFF;
            if (countryId == FifamNation::England || countryId == FifamNation::Italy || countryId == FifamNation::Germany) {
                UInt leagueId = 0x00010000 | (countryId << 24);
                String fileNameLeague = Utils::Format(L"%08X_%08X.tga", fifamId, leagueId);
                Path filePathLeague = outPath / fileNameLeague;
                if (hasBundesliga && countryId == FifamNation::Germany)
                    img.composite(bundesliga, 0, 384, OverCompositeOp);
                img.write(filePathLeague.string());
                if (hasBundesliga && countryId == FifamNation::Germany) {
                    img.composite(bundesliga2, 0, 384, OverCompositeOp);
                    String fileNameLeague2 = Utils::Format(L"%08X_%08X.tga", fifamId, leagueId | 1);
                    Path filePathLeague2 = outPath / fileNameLeague2;
                    img.write(filePathLeague2.string());
                }
                if (countryId == FifamNation::Germany) {
                    Image part(img, Geometry(512, 128));
                    img.composite(part, 0, 384, OverCompositeOp);
                }
                else if (countryId == FifamNation::England) {
                    Image part(img, Geometry(512, 64, 0, 384));
                    img.composite(part, 0, 0, OverCompositeOp);
                }
                else if (countryId == FifamNation::Italy) {
                    Image part1(img, Geometry(512, 128, 0, 320));
                    Image part2(img, Geometry(512, 192, 0, 448));
                    Image part3(img, Geometry(512, 128, 0, 704));
                    img.composite(part1, 0, 896, OverCompositeOp);
                    img.composite(part2, 0, 0, OverCompositeOp);
                    img.composite(part3, 0, 192, OverCompositeOp);
                }
            }
            String fileNameClub = Utils::Format(L"%08X.tga", fifamId);
            Path filePathClub = outPath / fileNameClub;
            img.write(filePathClub.string());
            writtenAdboards.insert(1000000 + fifaId);
        }
    }
    Path notConvertedPath = outPath / "_notconverted";
    if (exists(notConvertedPath))
        remove(notConvertedPath);
    for (auto const &[fifaId, filePath] : adboards) {
        if (!Utils::Contains(writtenAdboards, fifaId)) {
            create_directories(notConvertedPath);
            copy(filePath, notConvertedPath / filePath.filename(), copy_options::overwrite_existing);
        }
    }
}

void KitConverter::ConvertSponsors512x64(Map<UInt, UInt>& fifaClubToFifam, Map<UInt, Vector<UInt>>& compsMap) {
    const Path inPath = "D:\\Games\\fc26_files\\other\\adSponsors512x64";
    const Path outPath = "D:\\Projects\\fifam\\content\\fm13\\art_01_sponsors\\sponsors\\512x64";
    create_directories(outPath);
    struct DynamicImage {
        UInt color;
        String name;
    };
    Map<Int, DynamicImage> dynamicImages;
    FifaDataFile file;
    Path fifaDbFolderPath = "D:\\Projects\\fifam\\db\\fifa\\26\\2025.10.07";
    if (file.Open(fifaDbFolderPath / L"dynamicimages.txt")) {
        String basecolour, name;
        Int dynamicimageid;
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            line >> basecolour >> name >> dynamicimageid;
            if (!name.empty()) {
                //::Message(name);
                dynamicImages[dynamicimageid].color = Utils::ToNumber(basecolour);
                dynamicImages[dynamicimageid].name = name;
            }
        }
        file.Close();
    }
    auto RecolorImage = [](Path imgPath, UInt color) {
        Image img(imgPath.string().c_str());
        Image newImg(Magick::Geometry(img.columns(), img.rows()), Magick::Color((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF));
        newImg.type(ImageType::TrueColorAlphaType);
        newImg.composite(img, 0, 0, CompositeOperator::OverCompositeOp);
        return newImg;
    };
    if (file.Open(fifaDbFolderPath / L"teamsponsorlinks.txt")) {
        Map<Int, UInt> count;
        Int artificialkey, adsponserid, teamid, isapproved, dynamicimageid;
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            line >> artificialkey >> adsponserid >> teamid >> isapproved >> dynamicimageid;
            if (teamid != 0 && Utils::Contains(dynamicImages, dynamicimageid) && Utils::Contains(fifaClubToFifam, (UInt)teamid)) {
                auto const& dynamicImage = dynamicImages[dynamicimageid];
                Path imagePath = inPath / (L"adSponsors512x64" + dynamicImage.name + L".png");
                //::Message(imagePath.string());
                if (exists(imagePath.string())) {
                    Image img = RecolorImage(imagePath, dynamicImage.color);
                    if (Utils::Contains(count, teamid))
                        count[teamid] += 1;
                    else
                        count[teamid] = 1;
                    Path newImagePath = outPath / Utils::Format(L"%08X_%u.tga", fifaClubToFifam[teamid], count[teamid]);
                    img.write(newImagePath.string().c_str());
                }
            }
        }
        file.Close();
    }
    if (file.Open(fifaDbFolderPath / L"competitionsponsorlinks.txt")) {
        Map<UInt, UInt> count;
        Int competitionid, artificialkey, adsponserid, isapproved, dynamicimageid;
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            line >> competitionid >> artificialkey >> adsponserid >> isapproved >> dynamicimageid;
            if (competitionid != 0 && Utils::Contains(dynamicImages, dynamicimageid) && Utils::Contains(compsMap, (UInt)competitionid)) {
                auto const& dynamicImage = dynamicImages[dynamicimageid];
                Path imagePath = inPath / (L"adSponsors512x64" + dynamicImage.name + L".png");
                if (exists(imagePath.string())) {
                    Image img = RecolorImage(imagePath, dynamicImage.color);
                    for (auto fifamCompId : compsMap[competitionid]) {
                        if (Utils::Contains(count, fifamCompId))
                            count[fifamCompId] += 1;
                        else
                            count[fifamCompId] = 1;
                        String compFormat = fifamCompId <= 0xFFFF ? L"%04X_%u.tga" : L"%08X_%u.tga";
                        Path newImagePath = outPath / Utils::Format(compFormat, fifamCompId, count[fifamCompId]);
                        img.write(newImagePath.string().c_str());
                    }
                }
            }
        }
        file.Close();
    }
}

void KitConverter::GenerateGenericBanners() {
    const path assetsDir = "D:\\FIFA_ASSETS";
    const Path bannersDir = "D:\\FIFA_ASSETS\\banners";
    const Path gameBannersDir = "E:\\Games\\FIFA Manager 13\\data\\banners";
    for (UInt i = 0; i < 28; i++) {
        Path outputDir = gameBannersDir / ("generic_" + std::to_string(i));
        create_directories(outputDir);
        auto colors = FifamClub::mTeamColorsTable[i].second;
        if (colors.first.r == 0 && colors.first.g == 0 && colors.first.b == 0) {
            colors.first.r = 20;
            colors.first.g = 20;
            colors.first.b = 20;
        }
        if (colors.second.r == 0 && colors.second.g == 0 && colors.second.b == 0) {
            colors.second.r = 20;
            colors.second.g = 20;
            colors.second.b = 20;
        }
        colors.first.r = Utils::Clamp(colors.first.r + 10, 0, 255);
        colors.first.g = Utils::Clamp(colors.first.g + 10, 0, 255);
        colors.first.b = Utils::Clamp(colors.first.b + 10, 0, 255);
        colors.second.r = Utils::Clamp(colors.second.r + 10, 0, 255);
        colors.second.g = Utils::Clamp(colors.second.g + 10, 0, 255);
        colors.second.b = Utils::Clamp(colors.second.b + 10, 0, 255);

        Image bannersImg(Geometry(1024, 512), Magick::Color(colors.second.r, colors.second.g, colors.second.b));
        Image bannersPrimColorImg(Geometry(1024, 512), Magick::Color(colors.first.r, colors.first.g, colors.first.b));
        Image bannersMaskImg((bannersDir / "banners_mask.tga").string());
        bannersPrimColorImg.composite(bannersMaskImg, 0, 0, CopyAlphaCompositeOp);
        bannersImg.composite(bannersPrimColorImg, 0, 0, OverCompositeOp);
        Image bannersWhiteImg((bannersDir / "banners_white.dds").string());
        bannersImg.composite(bannersWhiteImg, 0, 0, MultiplyCompositeOp);
        bannersImg.type(ImageType::TrueColorAlphaType);
        bannersImg.write((outputDir / "banners.tga").string());
        Image flag1Img(bannersImg, Geometry(256, 128, 0, 0));
        resizeImage_noAspect(flag1Img, 128, 128);
        flag1Img.type(ImageType::TrueColorAlphaType);
        flag1Img.write((outputDir / "flag1.tga").string());
        Image flag2Img(bannersImg, Geometry(256, 128, 256, 0));
        resizeImage_noAspect(flag2Img, 128, 128);
        flag2Img.type(ImageType::TrueColorAlphaType);
        flag2Img.write((outputDir / "flag2.tga").string());
        Image flag3Img(bannersImg, Geometry(256, 128, 256 * 2, 0));
        resizeImage_noAspect(flag3Img, 128, 128);
        flag3Img.type(ImageType::TrueColorAlphaType);
        flag3Img.write((outputDir / "flag3.tga").string());
        Image flag4Img(bannersImg, Geometry(256, 128, 256 * 3, 0));
        resizeImage_noAspect(flag4Img, 128, 128);
        flag4Img.type(ImageType::TrueColorAlphaType);
        flag4Img.write((outputDir / "flag4.tga").string());
        Image crestImg(Geometry(256, 256), Magick::Color(0, 0, 0, 0));
        crestImg.type(ImageType::TrueColorAlphaType);
        crestImg.write((outputDir / "crest.tga").string());
        Image primImg(Geometry(4, 4), Magick::Color(colors.first.r, colors.first.g, colors.first.b));
        primImg.type(ImageType::TrueColorAlphaType);
        primImg.write((outputDir / "primary.tga").string());
        Image secImg(Geometry(4, 4), Magick::Color(colors.second.r, colors.second.g, colors.second.b));
        secImg.type(ImageType::TrueColorAlphaType);
        secImg.write((outputDir / "secondary.tga").string());
        Image cornerFlagImg(Geometry(128, 128), Magick::Color(colors.first.r, colors.first.g, colors.first.b));
        auto flagOverlayFilename = assetsDir / "flags" / "flag_overlay.tga";
        if (exists(flagOverlayFilename)) {
            Image cornerFlagOverlayImg(flagOverlayFilename.string());
            cornerFlagImg.composite(cornerFlagOverlayImg, 0, 0, MultiplyCompositeOp);
        }
        cornerFlagImg.type(ImageType::TrueColorAlphaType);
        cornerFlagImg.write((outputDir / "cornerflag.tga").string());
    }
}

void KitConverter::GenerateGenericBannersFIFA(bool fifa14stadiums) {
    const Path bannersDir = "I:\\FIFA_ASSETS\\banners";
    Path gameBannersDir = "output\\fifa07";
    if (fifa14stadiums)
        gameBannersDir /= "banners_fifa14stadiums";
    else
        gameBannersDir /= "banners";
    Vector<Pair<std::string, ColorPair>> colorsTable = {
        { "0_0",  {{ 180,180,180 }, { 255,255,255 }} },
        { "2_2",  {{ 54,54,54 }, { 32,32,32 }} },
        { "0_2",  {{ 255,255,255 }, { 32,32,32 }} },
        { "0_5",  {{ 233,208,76  }, { 255,255,255 }} },
        { "0_10", {{ 236,176,8 }, { 255,255,255 }} },
        { "0_14", {{ 241,109,43 }, { 255,255,255 }} },
        { "0_16", {{ 228,75,57 }, { 255,255,255 }} },
        { "0_18", {{ 183,26,27 }, { 255,255,255 }} },
        { "0_19", {{ 216,41,41 }, { 255,255,255 }} },
        { "0_23", {{ 219,42,41 }, { 255,255,255 }} },
        { "0_32", {{ 148,78,173 }, { 255,255,255 }} },
        { "0_34", {{ 225,204,175 }, { 255,255,255 }} },
        { "0_39", {{ 123,60,57 }, { 255,255,255 }} },
        { "0_41", {{ 170,233,200 }, { 255,255,255 }} },
        { "0_48", {{ 57,121,57 }, { 255,255,255 }} },
        { "0_51", {{ 170,180,192 }, { 255,255,255 }} },
        { "0_56", {{ 53,58,74 }, { 255,255,255 }} },
        { "0_57", {{ 162,196,228 }, { 255,255,255 }} },
        { "0_60", {{ 61,92,185 }, { 255,255,255 }} },
        { "0_61", {{ 41,60,132 }, { 255,255,255 }} },
        { "0_62", {{ 35,49,93 }, { 255,255,255 }} },
        { "6_2",  {{ 235,222,82  }, { 32,32,32 }} },
        { "10_2", {{ 236,176,8 }, { 32,32,32 }} },
        { "14_2", {{ 241,109,43 }, { 32,32,32 }} },
        { "19_2", {{ 216,41,41 }, { 32,32,32 }} },
        { "0_59", {{ 49,130,206 }, { 255,255,255 }} }
    };
    for (UInt i = 0; i < colorsTable.size(); i++) {
        Path outputDir = gameBannersDir / ("t13__0_" + colorsTable[i].first);
        create_directories(outputDir);
        auto colors = colorsTable[i].second;
        Image bannersImg(Geometry(1024, 512), Magick::Color(colors.second.r, colors.second.g, colors.second.b));
        Image bannersPrimColorImg(Geometry(1024, 512), Magick::Color(colors.first.r, colors.first.g, colors.first.b));
        Image bannersMaskImg((bannersDir / "banners_mask.tga").string());
        bannersMaskImg.autoOrient();
        bannersPrimColorImg.composite(bannersMaskImg, 0, 0, CopyAlphaCompositeOp);
        bannersImg.composite(bannersPrimColorImg, 0, 0, OverCompositeOp);
        Image bannersWhiteImg((bannersDir / "banners_white.dds").string());
        bannersWhiteImg.autoOrient();
        bannersImg.composite(bannersWhiteImg, 0, 0, MultiplyCompositeOp);
        auto WriteBanner = [&](Path const &file, Image const &image, UInt x, UInt y) {
            Image copy(image, Geometry(256, 128, x, y));
            copy.write(file.string());
        };
        if (fifa14stadiums) {
            bannersImg.write((outputDir / "_bna.tga").string());
            Image primImg(Geometry(4, 4), Magick::Color(colors.first.r, colors.first.g, colors.first.b));
            primImg.write((outputDir / "_bnb.tga").string());
            Image secImg(Geometry(4, 4), Magick::Color(colors.second.r, colors.second.g, colors.second.b));
            secImg.write((outputDir / "_bnc.tga").string());
        }
        else {
            WriteBanner(outputDir / "_bna.tga", bannersImg, 256 * 3, 128 * 1);
            WriteBanner(outputDir / "_bnb.tga", bannersImg, 256 * 3, 128 * 2);
            WriteBanner(outputDir / "_bnc.tga", bannersImg, 256 * 0, 128 * 1);
        }
        WriteBanner(outputDir / "_fla.tga", bannersImg, 256 * 2, 128 * 0);
        WriteBanner(outputDir / "_flb.tga", bannersImg, 256 * 3, 128 * 0);
        WriteBanner(outputDir / "_flc.tga", bannersImg, 256 * 2, 128 * 2);
    }
}

void KitConverter::GenerateBallsDat(Map<UInt, Vector<UInt>>& compsMap) {
    Map<UInt, String> compNames = {
        { 0x0, L"Default" },
        { 0xFF11, L"World Cup" },
        { 0xFF21, L"Copa America" },
        { 0xF909, L"Champions League" },
        { 0xF90A, L"Europa League" },
        { 0xF933, L"Europa Conference League" },
        { 0xF90C, L"UEFA SuperCup" },
        { 0xFA09, L"Copa Libertadores" },
        { 0xFA0A, L"Copa Sudamericana" },
        { 0xFA0C, L"Supercopa Sudamericana" },
        { 0x07010000, L"Belgium Pro League" },
        { 0x0D010000, L"Denmark Superleague" },
        { 0x0D080000, L"Denmark Superleague Split 1" },
        { 0x0D080001, L"Denmark Superleague Split 2" },
        { 0x0D030000, L"Denmark Cup" },
        { 0x22010000, L"Eredivisie" },
        { 0x0E010000, L"EPL" },
        { 0x0E010001, L"Championship" },
        { 0x0E010002, L"League One" },
        { 0x0E010003, L"League Two" },
        { 0x0E080000, L"Championship Play-Off 1" },
        { 0x0E080001, L"Championship Play-Off 2" },
        { 0x0E080002, L"League One Play-Off 1" },
        { 0x0E080003, L"League One Play-Off 2" },
        { 0x0E080004, L"League Two Play-Off 1" },
        { 0x0E080005, L"League Two Play-Off 2" },
        { 0x0E030000, L"FA Cup" },
        { 0x0E040000, L"Carabao Cup" },
        { 0x0E040001, L"Bristol Street Motors Trophy" },
        { 0x0E070000, L"England Supercup" },
        { 0x12010000, L"Ligue 1" },
        { 0x12010001, L"Ligue 2" },
        { 0x12070000, L"Trophee des Champions" },
        { 0x15010000, L"Bundesliga" },
        { 0x15080000, L"Bundesliga Relegation 1<>2" },
        { 0x15010001, L"Bundesliga 2" },
        { 0x15080001, L"Bundesliga Relegation 2<>3" },
        { 0x15010002, L"3. Liga" },
        { 0x15030000, L"DFB Pokal" },
        { 0x15070000, L"DFL-Supercup" },
        { 0x1B010000, L"Serie A" },
        { 0x1B010001, L"Serie B" },
        { 0x1B030000, L"Italy Cup" },
        { 0x1B070000, L"Italy Supercup" },
        { 0x5F010000, L"MLS" },
        { 0x5F080000, L"MLS Play-Off East" },
        { 0x5F080001, L"MLS Play-Off West" },
        { 0x5F040000, L"MLS Cup" },
        { 0x24010000, L"Norway" },
        { 0x24030000, L"Norway Cup" },
        { 0x2A010000, L"Scottish PFL" },
        { 0x2A080000, L"Scottish PFL Split 1" },
        { 0x2A080001, L"Scottish PFL Split 2" },
        { 0x2D010000, L"LaLiga" },
        { 0x2D010001, L"Segunda" },
        { 0x2E010000, L"Sweden" },
        { 0x19010000, L"Ireland league" },
        { 0x25010000, L"Ekstraklasa" },
        { 0x30010000, L"Turkey league" },
        { 0x04010000, L"Austrian Bundesliga" },
        { 0x04080000, L"Austrian Bundesliga Split 1" },
        { 0x04080001, L"Austrian Bundesliga Split 2" },
        { 0xA7010000, L"Korea league" },
        { 0xA7080000, L"Korea league Split 1" },
        { 0xA7080001, L"Korea league Split 2" },
        { 0x2F010000, L"Switzerland" },
        { 0x26010000, L"Portugal league" },
        { 0x27010000, L"Romania league" },
        { 0x27080000, L"Romania league Split 1" },
        { 0x27080001, L"Romania league Split 2" },
        { 0x27030000, L"Romania cup" },
        { 0xB7010000, L"Saudi Arabia League" },
        { 0x9B010000, L"China league" },
        { 0x53010000, L"Mexican Liga" },
        { 0x34010000, L"Argentina Liga" },
        { 0xC3010000, L"Australia League" },
        { 0xC3040000, L"Australia League Finals" },
        { 0x9F010000, L"IHero Indian SuperLeague" }
    };
    struct BallEntry {
        Int ballId = -1;
        Int winterBallId = -1;
    };
    Map<UInt, BallEntry> entries;
    FifaDataFile file;
    Path fifaDbFolderPath = "D:\\Projects\\fifam\\db\\fifa\\26\\2025.10.07";
    if (file.Open(fifaDbFolderPath / L"competitionballs.txt")) {
        Int competitionid, stage, weather, ballid;
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            line >> competitionid >> stage >> weather >> ballid;
            if (stage == 0 && ballid >= 22 && Utils::Contains(compsMap, (UInt)competitionid)) {
                for (auto const& i : compsMap[(UInt)competitionid]) {
                    if (weather == 0)
                        entries[i].ballId = ballid;
                    else if (weather == 2)
                        entries[i].winterBallId = ballid;
                }
            }
        }
        file.Close();
    }
    FifamWriter w(L"D:\\Games\\FIFA Manager 13\\plugins\\ucp\\balls_new.dat", 13, FifamVersion(), false);
    w.WriteLine(L";CompetitionId, BallId, BallIdWinter");
    auto PadString = [](String& s, UInt size, Bool right) {
        if (s.size() < size) {
            String p(size - s.size(), L' ');
            if (right)
                s.append(p);
            else
                s = p + s;
        }
        };
    for (auto const& [id, e] : entries) {
        String compId;
        if (id == 0)
            compId = L"0";
        else if (id <= 0xFFFF)
            compId = Utils::Format(L"%04X,", id);
        else
            compId = Utils::Format(L"%08X,", id);
        Int ballId = e.ballId;
        Int winterBallId = e.winterBallId;
        if (ballId == -1)
            ballId = winterBallId;
        else if (winterBallId == -1)
            winterBallId = ballId;
        PadString(compId, 15, true);
        String strBallId = Utils::Format(L"%u,", ballId);
        PadString(strBallId, 4, false);
        String strWinterBallId = Utils::Format(L"%u", winterBallId);
        PadString(strWinterBallId, 7, false);
        String comment;
        if (Utils::Contains(compNames, id))
            comment = compNames[id];
        w.WriteLine(compId + strBallId + strWinterBallId + L"          ; " + comment);
    }
}
