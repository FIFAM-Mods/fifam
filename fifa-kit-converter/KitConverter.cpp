#include "KitConverter.h"
#include <filesystem>
#include "Utils.h"
#include "Error.h"
#include "FifamReadWrite.h"

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
        Image circlesImg(options.KitsPath + "circles.png");
        Map<Magick::Color, std::vector<std::pair<int, int>>> circlesMap;
        if (circlesImg.isValid() && circlesImg.columns() == 9 && circlesImg.rows() == 9) {
            for (size_t row = 0; row < circlesImg.rows(); ++row) {
                for (size_t column = 0; column < circlesImg.columns(); ++column) {
                    auto pc = circlesImg.pixelColor(column, row);
                    if (pc.quantumAlpha() == 255)
                        circlesMap[pc].emplace_back(column - 4, row - 4);
                }
            }
            circles.emplace_back(circlesMap[Magick::Color(255, 0, 0, 255)]);
            circles.emplace_back(circlesMap[Magick::Color(0, 255, 0, 255)]);
            //circles.emplace_back(circlesMap[Magick::Color(0, 0, 255, 255)]);
            //circles.emplace_back(circlesMap[Magick::Color(0, 0, 0, 255)]);
        }
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
#define LEGACY_CONVERTER
    SetSizeMode(1);
    if (options.OutputGameId >= 9) {
        
        Image inputShortsImg(inputShorts);
        if (inputShortsImg.columns() > 1024) {
            SetSizeMode(2);
            if (GetSizeMode() == 1)
                inputShortsImg.resize(ScaledGeometry(1024, 512));
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
        if (inputShirtImg.columns() > 1024 && GetSizeMode() == 1)
            inputShirtImg.resize(ScaledGeometry(1024, 1024));

        bool addLogo = true;
    #ifndef LEGACY_CONVERTER
        if (!inputCrest.empty() && (logoPos[0] != 0 || logoPos[1] != 0 || logoPos[2] != 0 || logoPos[3] != 0)) {
            int logoLeft = logoPos[0];
            int logoTop = logoPos[1];
            int logoRight = logoPos[2];
            int logoBottom = logoPos[3];
            Image inputCrestImg(inputCrest);
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
            if (GetSizeMode() == 1)
                inputShirtImg.resize(ScaledGeometry(1024, 1024));
        }
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
        if (inputShortsImg.columns() > 1024 && GetSizeMode() == 1)
            inputShortsImg.resize(ScaledGeometry(1024, 512));

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

        if (!inputCrest.empty() && logoPos[0] == 0 && logoPos[1] == 0 && logoPos[2] == 0 && logoPos[3] == 0) {
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
        printf("Converting club %s (%d)", clubIdStr.c_str(), set);
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
            miniKitPath = options.CustomMiniKitsPath + clubIdStr + kitType + ".dds";
    }
    if (fifaId != 0 && !options.OnlyCustomKits && (miniKitPath.empty() || !exists(miniKitPath)))
        miniKitPath = options.MiniKitsPath + "j" + to_string(set) + "_" + to_string(fifaId) + "_" + to_string(variation) + ".dds";
    if (exists(miniKitPath)) {
        printf("Converting club %d (%d)", fifaId, set);
        Image mini(miniKitPath);
        if (mini.isValid()) {
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
    if (options.SaveLocation == Big) {

    }
    else {
        if (!options.ConvertOnlyMinikits) {
            if (options.SaveLocation == Documents && options.OutputGameId >= 9)
                outputFile = string("E:\\Documents\\FIFA MANAGER ") + gameIdStr + "\\Graphics\\3DMatch\\Kits\\";
            else {
                if (options.OutputGameId >= 9)
                    outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kits\\";
                else
                    outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\user\\kits\\";
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
            outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\minikits\\" + clubIdStr;
            if (options.ConvertHomeKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 0, 0, outputFile + "_h");
            if (options.ConverAwayKit)
                ConvertFifaClubMiniKit(fifaId, clubIdStr, 1, 0, outputFile + "_a");
            //if (options.ConvertThirdKit)
            //    ConvertFifaClubMiniKit(fifaId, clubIdStr, 3, 0, outputFile + "_t");
            //if (options.ConvertGkKit)
            //    ConvertFifaClubMiniKit(fifaId, clubIdStr, 2, 0, outputFile + "_g");
        }
    }
}

void KitConverter::ConvertRefereeKit(int fifaId) {
    ConvertFifaClubKit(fifaId, std::to_string(fifaId), 5, 0, "E:\\Projects\\FIFA20\\ref_kits\\" + std::to_string(fifaId));
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
        string kitFileName = "kit_" + to_string(fifaId) + "_" + to_string(set) + "_" + to_string(variation) + "_2.png";
        shirtFileName = options.KitsPath + kitFileName;
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
    string outputFile = string("E:\\Games\\FIFA Manager ") + gameIdStr + "\\data\\kitarmband\\" + clubIdStr;
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
    string fileNameBase = options.KitNumbersPath + "kitnumbers_" + to_string(kitnumersId) + "_";
    string fileName = fileNameBase + "1.png";
    if (!exists(fileName))
        return false;
    static unsigned int j_numbers[] = { 2, 6, 4, 0, 8, 3, 7, 5, 1, 9 };
    static unsigned int s_numbers[] = { 1, 2, 8, 3, 9, 4, 7, 6, 5, 0 };
    const unsigned int size = 256;
    Image channelsImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
    for (unsigned int i = 0; i < 10; i++) {
        fileName = fileNameBase + to_string((jersey ? j_numbers[i] : s_numbers[i]) + 1) + ".png";
        if (!exists(fileName))
            return false;
        Image img(fileName);
        if (!img.isValid())
            return false;
        ScaledComposite(channelsImg, img, size * i, 0, OverCompositeOp);
    }
    auto imgR = channelsImg.separate(MagickCore::ChannelType::RedChannel);
    auto imgG = channelsImg.separate(MagickCore::ChannelType::GreenChannel);
    auto imgB = channelsImg.separate(MagickCore::ChannelType::BlueChannel);
    auto imgA = channelsImg.separate(MagickCore::ChannelType::AlphaChannel);
    Image channelR(ScaledGeometry(size * 10, size), Magick::Color(clr1.r, clr1.g, clr1.b));
    Image channelG(ScaledGeometry(size * 10, size), Magick::Color(clr2.r, clr2.g, clr2.b));
    Image channelB(ScaledGeometry(size * 10, size), Magick::Color(clr3.r, clr3.g, clr3.b));
    ScaledComposite(channelR, imgR, 0, 0, MultiplyCompositeOp);
    ScaledComposite(channelG, imgG, 0, 0, MultiplyCompositeOp);
    ScaledComposite(channelB, imgB, 0, 0, MultiplyCompositeOp);
    Image finalImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
    ScaledComposite(finalImg, channelR, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, channelG, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, channelB, 0, 0, PlusCompositeOp);
    ScaledComposite(finalImg, imgA, 0, 0, CopyAlphaCompositeOp);
    ScaledResize(finalImg, jersey ? 1024 : 256, jersey ? 128 : 32);
    finalImg.write(outputFile + "." + options.OutputFormat);
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
    unsigned int size = 128;
    Image finalImg(ScaledGeometry(size * 10, size), Magick::Color(0, 0, 0, 0));
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
    finalImg.write("E:\\Games\\FIFA Manager 13\\data\\kitnumbers_custom\\" + finalDir + dirName + "." + options.OutputFormat);
    return true;
}

void KitConverter::ConvertClubKitNumbersCustom() {
    path customDir = "E:\\Projects\\FIFA20\\custom_kitnumbers\\";
    for (auto const &p : directory_iterator(customDir)) {
        string folderPath = p.path().string();
        string folderName = p.path().filename().string();
        ConvertClubKitNumbersSetCustom(folderPath, folderName, true);
        ConvertClubKitNumbersSetCustom(folderPath, folderName, false);
    }
}

bool KitConverter::ConvertKitV2(string const &inputShirt, string const &inputShorts, string const &inputCrest, string const &outputFile, array<int, 8> logoPos, bool hasPositions) {
    SetSizeMode(options.Force2x ? 4 : 1);
    if (options.OutputGameId >= 9) {

        int texW = 512 * GetSizeMode();
        int texH = 1024 * GetSizeMode();

        Image fifaKitAllInOneImg(ScaledGeometry(1024, 1024 + 512), Magick::Color(255, 0, 0, 255));

        Image fifaKitShirt(inputShirt);
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

        bool skipPP = true;

        mRenderer->End();
        mRenderer->SaveRT(Utils::AtoW(skipPP ? (outputFile + ".tga") : (options.KitsPath + "tmpFifaKit2.tga")).c_str());
        mRenderer->DestroyTexture(tex);

        if (!skipPP) {
            Image finalImg(options.KitsPath + "tmpFifaKit2.tga");
            Image resultImg(Geometry(texW, texH), Magick::Color(0, 0, 0, 255));
            int offset = 2;

            Image repaired(finalImg);
            //for (UInt i = 0; i < 50; i++)
            //    blurred.blur(1);
            auto SetPixelColor = [](Image &img, int x, int y, Magick::Color const &clr) {
                img.fillColor(clr);
                img.draw(Magick::DrawableRectangle(x, y, x + 1, y + 1));
            };
            for (size_t row = 0; row < finalImg.rows(); ++row) {
                for (size_t column = 0; column < finalImg.columns(); ++column) {
                    auto pc = finalImg.pixelColor(column, row);
                    if (pc.quantumAlpha() == 0) {
                        for (int ci = 0; ci < circles.size(); ci++) {
                            std::vector<Magick::Color> colors;
                            for (int cj = 0; cj < circles[ci].size(); cj++) {
                                auto jp = finalImg.pixelColor(column + circles[ci][cj].first, row + circles[ci][cj].second);
                                if (jp.quantumAlpha() != 0)
                                    colors.push_back(jp);
                            }
                            if (!colors.empty()) {
                                unsigned int avgR = 0, avgG = 0, avgB = 0;
                                for (auto clr : colors) {
                                    avgR += clr.quantumRed();
                                    avgG += clr.quantumGreen();
                                    avgB += clr.quantumBlue();
                                }
                                avgR /= colors.size();
                                avgG /= colors.size();
                                avgB /= colors.size();
                                SetPixelColor(repaired, column, row, Magick::Color(avgR, avgG, avgB, 255));
                                break;
                            }
                        }
                    }
                    else if (pc.quantumAlpha() != 255)
                        SetPixelColor(repaired, column, row, Magick::Color(pc.quantumRed(), pc.quantumGreen(), pc.quantumBlue(), 255));
                }
            }
            ScaledComposite(resultImg, repaired, 0, 0, MagickCore::OverCompositeOp);

            //ScaledComposite(resultImg, finalImg, -offset, -offset, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, offset, offset, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, -offset, offset, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, offset, -offset, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, -offset, 0, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, offset, 0, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, 0, -offset, MagickCore::OverCompositeOp);
            //ScaledComposite(resultImg, finalImg, 0, offset, MagickCore::OverCompositeOp);
            ScaledComposite(resultImg, finalImg, 0, 0, MagickCore::OverCompositeOp);

            if (!inputCrest.empty() && logoPos[0] == 0 && logoPos[1] == 0 && logoPos[2] == 0 && logoPos[3] == 0) {
                Image inputCrestImg(inputCrest);
                ScaledResize(inputCrestImg, 56, 56);
                ScaledComposite(resultImg, inputCrestImg, 298, 719, OverCompositeOp);
            }

            //if (!options.Force2x && !options.Overlay && options.AddKitOverlay) {
            //    Image kitOverlayImg(options.KitsPath + "kit_overlay.png");
            //    ScaledComposite(resultImg, kitOverlayImg, 0, 0, OverCompositeOp);
            //}

            resultImg.fillColor(Magick::Color(255, 255, 255, 255));
            resultImg.strokeColor(Magick::Color(0, 0, 0, 0));
            if (!options.Overlay && options.AddWatermarkText) {
                resultImg.fontPointsize(14 * GetSizeMode());
                resultImg.fontWeight(550);
                resultImg.annotate("FIFAM Universal Converter Project", ScaledGeometry(1024, 1024, 3, 1012), NorthWestGravity, -90);
                resultImg.annotate("converted from EA SPORTS FIFA 20", ScaledGeometry(1024, 1024, 512 - 12, 1012), NorthWestGravity, -90);
            }

            resultImg.write(outputFile + ".tga");
        }
        else {
            if (!hasPositions && !inputCrest.empty()) {
                Image resultImg(outputFile + ".tga");
                Image inputCrestImg(inputCrest);
                ScaledResize(inputCrestImg, 56, 56);
                ScaledComposite(resultImg, inputCrestImg, 298, 719, OverCompositeOp);
                resultImg.flip();
                resultImg.write(outputFile + ".tga");
            }
        }
    }

    return true;
}

void KitConverter::ConvertBanners(int fifaId, int fifaManagerId) {

    const bool for_fsh = false;
    const bool resize = false;

    const string prefix = for_fsh ? "_" : "";
    auto inputPath = options.BannersPath + "banner_" + std::to_string(fifaId) + "_1.png";
    if (exists(inputPath)) {
        Image input(inputPath);
        if (input.isValid()) {
            if (input.columns() != 1024 || input.rows() != 512)
                resizeImage_noAspect(input, 1024, 512);
            path outputPath =
                path(for_fsh ? "E:\\Games\\FIFA Manager 13\\data\\bnrs" : "E:\\Games\\FIFA Manager 13\\data\\banners") /
                (for_fsh ? to_string(fifaId) : Utils::WtoA(Utils::Format(L"%08X", fifaManagerId)));
            create_directories(outputPath);

            auto WriteBanner = [&](path const &file, Image const &image, unsigned int x, unsigned int y) {
                Image copy(image, Geometry(256, 128, x, y));
                if (resize)
                    copy.resize(Geometry(128, 64));
                copy.write(file.string());
            };

            WriteBanner(outputPath / (prefix + "bna.tga"), input, 256 * 0, 128 * 0);
            WriteBanner(outputPath / (prefix + "bnb.tga"), input, 256 * 1, 128 * 0);
            WriteBanner(outputPath / (prefix + "bnc.tga"), input, 256 * 2, 128 * 0);
            WriteBanner(outputPath / (prefix + "fla.tga"), input, 256 * 3, 128 * 0);
            WriteBanner(outputPath / (prefix + "flb.tga"), input, 256 * 0, 128 * 1);
            WriteBanner(outputPath / (prefix + "flc.tga"), input, 256 * 1, 128 * 1);
            WriteBanner(outputPath / (prefix + "lga.tga"), input, 256 * 2, 128 * 1);
            WriteBanner(outputPath / (prefix + "lgb.tga"), input, 256 * 3, 128 * 1);
        }
    }
}
