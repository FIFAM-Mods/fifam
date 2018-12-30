#include "KitConverter.h"
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace Magick;

void KitConverter::resizeImage_noAspect(Image & image, int w, int h) {
    Geometry geom(w, h);
    geom.aspect(true);
    image.resize(geom);
}

int KitConverter::INPUT_RES(int orig) { return orig * InputResolutionMultiplier; }

int KitConverter::OUTPUT_RES(int orig) { return orig * options.OutputResolutionMultiplier; }

KitConverter::KitConverter() {
    InitializeMagick(NULL);
}

bool KitConverter::ConvertKit(string const & inputShirt, string const & inputShorts, string const & inputCrest, string const & outputFile) {
    if (options.OutputGameId >= 9) {
        Image outputFileImg(Geometry(512, 1024), "red");

        Image inputShortsImg(inputShorts);

        Image leftSockImg(inputShortsImg, Geometry(356, 174, 142, 5));
        Image leftSockImg2(Geometry(356, 174 * 2), "black");
        leftSockImg2.composite(leftSockImg, 0, 0, OverCompositeOp);
        leftSockImg2.composite(leftSockImg, 0, 174, OverCompositeOp);
        resizeImage_noAspect(leftSockImg2, 101, 284);
        leftSockImg2.crop(Geometry(101, 181));
        outputFileImg.composite(leftSockImg2, -12, 593, OverCompositeOp);

        Image rightSockImg(inputShortsImg, Geometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(Geometry(356, 174 * 2), "black");
        rightSockImg2.composite(rightSockImg, 0, 0, OverCompositeOp);
        rightSockImg2.composite(rightSockImg, 0, 174, OverCompositeOp);
        resizeImage_noAspect(rightSockImg2, 101, 284);
        rightSockImg2.crop(Geometry(101, 181));
        outputFileImg.composite(rightSockImg2, 423 + 4, 593 + 2, OverCompositeOp);

        Image inputShirtImg(inputShirt);

        // left short sleeve
        Image slMask(options.KitsPath + "kit_sleeve_mask.png");
        {
            Image tmpSL1(inputShirtImg, Geometry(182, 318, 188, 20));
            tmpSL1.composite(slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, Geometry(210, 318, 188, 351));
            tmpSL2.composite(tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(Geometry(210, 425), "black");
            shortSleeveLeft.composite(tmpSL2, 0, 114, OverCompositeOp);
            shortSleeveLeft.composite(tmpSL2, 0, -204, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            resizeImage_noAspect(shortSleeveLeft, 317, 129);

            outputFileImg.composite(shortSleeveLeft, -51, -3, OverCompositeOp);
        }
        // right short sleeve
        slMask.flop();
        {
            Image tmpSR1(inputShirtImg, Geometry(182, 318, 1024 - 188 - 182, 20));
            tmpSR1.composite(slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSR2(inputShirtImg, Geometry(210, 318, 1024 - 188 - 210, 351));
            tmpSR2.composite(tmpSR1, 210 - 182, 0, OverCompositeOp);

            Image shortSleeveRight(Geometry(210, 425), "black");
            shortSleeveRight.composite(tmpSR2, 0, 114, OverCompositeOp);
            shortSleeveRight.composite(tmpSR2, 0, -204, OverCompositeOp);
            shortSleeveRight.rotate(90);
            resizeImage_noAspect(shortSleeveRight, 317, 129);

            outputFileImg.composite(shortSleeveRight, 512 + 45 + 4 - 317, -3, OverCompositeOp);
        }

        Image sleeveLeftSource(inputShirtImg, Geometry(415, 322, 0, 352));
        Image sleeveLeft(Geometry(415, 582), "black");
        sleeveLeft.composite(sleeveLeftSource, 0, 0, OverCompositeOp);
        sleeveLeft.composite(sleeveLeftSource, 0, 260, OverCompositeOp);
        sleeveLeft.rotate(-90);
        resizeImage_noAspect(sleeveLeft, 180, 281);
        outputFileImg.composite(sleeveLeft, -63, 330, OverCompositeOp);

        Image sleeveRightSource(inputShirtImg, Geometry(415, 322, 1024 - 415, 352));
        Image sleeveRight(Geometry(415, 582), "black");
        sleeveRight.composite(sleeveRightSource, 0, 0, OverCompositeOp);
        sleeveRight.composite(sleeveRightSource, 0, 260, OverCompositeOp);
        sleeveRight.rotate(90);
        resizeImage_noAspect(sleeveRight, 180, 281);
        outputFileImg.composite(sleeveRight, 512 + 65 - 180, 330, OverCompositeOp);

        Image shirtMainImg(inputShirtImg, Geometry(412, 900, 306, 62));
        resizeImage_noAspect(shirtMainImg, 492, 664);
        Image kitShirtMask(options.KitsPath + "kit_shirt_mask.png");
        shirtMainImg.composite(kitShirtMask, 0, 0, CopyAlphaCompositeOp);
        outputFileImg.composite(shirtMainImg, 10, 348, OverCompositeOp);

        Image shortsImg(inputShortsImg, Geometry(1024, 302, 0, 210));
        resizeImage_noAspect(shortsImg, 512, 212);
        outputFileImg.composite(shortsImg, 0, 127, OverCompositeOp);

        Image collarLeft(inputShirtImg, Geometry(6, 28, 474, 502));
        collarLeft.flip();
        resizeImage_noAspect(collarLeft, 41, 108);
        outputFileImg.composite(collarLeft, 6, 129, OverCompositeOp);

        Image collarRight(inputShirtImg, Geometry(6, 28, 474, 502));
        collarRight.flip();
        resizeImage_noAspect(collarRight, 39, 111);
        outputFileImg.composite(collarRight, 512 - 8 - 39, 126, OverCompositeOp);

        outputFileImg.fillColor(Color(inputShirtImg.pixelColor(150, 700)));
        outputFileImg.draw(DrawableRectangle(5, 465, 12, 598));
        outputFileImg.draw(DrawableRectangle(502, 463, 509, 599));

        if (!inputCrest.empty()) {
            Image inputCrestImg(inputCrest);
            resizeImage_noAspect(inputCrestImg, 44, 44);
            outputFileImg.composite(inputCrestImg, 302, 728, OverCompositeOp);
        }

        if (options.AddKitOverlay) {
            Image kitOverlayImg(options.KitsPath + "kit_overlay.png");
            outputFileImg.composite(kitOverlayImg, 0, 0, OverCompositeOp);
        }

        outputFileImg.fillColor("white");

        if (options.AddWatermarkText) {
            outputFileImg.fontPointsize(14);
            outputFileImg.fontWeight(550);
            outputFileImg.annotate("FIFAM Universal Converter Project", Geometry(1024, 1024, 3, 1012), NorthWestGravity, -90);
            outputFileImg.annotate("converted from EA SPORTS FIFA 19", Geometry(1024, 1024, 512 - 12, 1012), NorthWestGravity, -90);
        }

        outputFileImg.write(outputFile + "." + options.OutputFormat);
    }
    else {
        Image outputFileImg(Geometry(512, 512), "white");

        Image inputShirtImg(inputShirt);

        // left short sleeve
        Image slMask(options.KitsPath + "kit_sleeve_mask.png");
        {
            Image tmpSL1(inputShirtImg, Geometry(182, 318, 188, 20));
            tmpSL1.composite(slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSL2(inputShirtImg, Geometry(210, 318, 188, 351));
            tmpSL2.composite(tmpSL1, 0, 0, OverCompositeOp);

            Image shortSleeveLeft(Geometry(210, 390 + 95), "black");
            shortSleeveLeft.composite(tmpSL2, 0, -204, OverCompositeOp);
            shortSleeveLeft.composite(tmpSL2, 0, 95, OverCompositeOp);
            shortSleeveLeft.composite(tmpSL2, 0, 299 + 95, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            resizeImage_noAspect(shortSleeveLeft, 94, 68);

            outputFileImg.composite(shortSleeveLeft, -13, 4, OverCompositeOp);

            //static int c = 1;
            //shortSleeveLeft.write(options.KitsPath + "test" + to_string(c++) + ".png");

            /*
            Image shortSleeveLeft(Geometry(210, 390), "black");
            shortSleeveLeft.composite(tmpSL2, 0, 0, OverCompositeOp);
            shortSleeveLeft.composite(tmpSL2, 0, 299, OverCompositeOp);
            shortSleeveLeft.rotate(-90);
            resizeImage_noAspect(shortSleeveLeft, 86, 70);

            outputFileImg.composite(shortSleeveLeft, 0, 3, OverCompositeOp);*/
        }
        // right short sleeve
        slMask.flop();
        {
            Image tmpSR1(inputShirtImg, Geometry(182, 318, 1024 - 188 - 182, 20));
            tmpSR1.composite(slMask, 0, 0, CopyAlphaCompositeOp);

            Image tmpSR2(inputShirtImg, Geometry(210, 318, 1024 - 188 - 210, 351));
            tmpSR2.composite(tmpSR1, 210 - 182, 0, OverCompositeOp);

            Image shortSleeveRight(Geometry(210, 390 + 95), "black");
            shortSleeveRight.composite(tmpSR2, 0, -204, OverCompositeOp);
            shortSleeveRight.composite(tmpSR2, 0, 95, OverCompositeOp);
            shortSleeveRight.composite(tmpSR2, 0, 299 + 95, OverCompositeOp);
            shortSleeveRight.rotate(90);
            resizeImage_noAspect(shortSleeveRight, 94, 68);

            shortSleeveRight.flop();
            outputFileImg.composite(shortSleeveRight, -13, 4 + 71, OverCompositeOp);

            outputFileImg.composite(shortSleeveRight, -13, 4 + 327, OverCompositeOp);

            Image captainArmbandImg(inputShirtImg, Geometry(272, 46, 376, 976));
            resizeImage_noAspect(captainArmbandImg, 79, 11);
            captainArmbandImg.flop();
            outputFileImg.composite(captainArmbandImg, 0, 372, OverCompositeOp);
        }

        // blank
        outputFileImg.fillColor(Color(inputShirtImg.pixelColor(150, 700)));
        outputFileImg.draw(DrawableRectangle(0, 261, 166, 261 + 70));
        outputFileImg.draw(DrawableRectangle(81, 332, 81 + 86, 332 + 71));

        Image shirtBackImg(inputShirtImg, Geometry(377, 480, 323, 68));
        resizeImage_noAspect(shirtBackImg, 350, 271);
        shirtBackImg.flip();
        outputFileImg.composite(shirtBackImg, 162, 256, OverCompositeOp);

        Image shirtMainImg(inputShirtImg, Geometry(434, 480, 294, 472));
        resizeImage_noAspect(shirtMainImg, 404, 260);
        outputFileImg.composite(shirtMainImg, 135, 0, OverCompositeOp);

        Image collarFront(inputShirtImg, Geometry(200, 64, 56, 696));
        Image collarFrontMask(options.KitsPath + "kit_collar_front_mask08.png");
        collarFront.composite(collarFrontMask, 0, 0, CopyAlphaCompositeOp);
        outputFileImg.composite(collarFront, 240, 6, OverCompositeOp);

        Image collarBack(inputShirtImg, Geometry(200, 64, 56, 696));
        Image collarBackMask(options.KitsPath + "kit_collar_back_mask08.png");
        collarBack.composite(collarBackMask, 0, 0, CopyAlphaCompositeOp);
        outputFileImg.composite(collarBack, 236, 256, OverCompositeOp);

        Image inputShortsImg(inputShorts);

        Image leftSockImg(inputShortsImg, Geometry(356, 174, 142, 5));
        Image leftSockImg2(Geometry(356, 174 * 3), "black");
        leftSockImg2.composite(leftSockImg, 0, 0, OverCompositeOp);
        leftSockImg2.composite(leftSockImg, 0, 174, OverCompositeOp);
        leftSockImg2.composite(leftSockImg, 0, 174 * 2, OverCompositeOp);
        leftSockImg2.rotate(-90);
        resizeImage_noAspect(leftSockImg2, 204, 82);
        leftSockImg2.flop();
        leftSockImg2.crop(Geometry(81, 69, 64, 0));
        outputFileImg.composite(leftSockImg2, 81, 2, OverCompositeOp);

        Image rightSockImg(inputShortsImg, Geometry(356, 174, 1024 - 142 - 356, 5));
        Image rightSockImg2(Geometry(356, 174 * 3), "black");
        rightSockImg2.composite(rightSockImg, 0, 0, OverCompositeOp);
        rightSockImg2.composite(rightSockImg, 0, 174, OverCompositeOp);
        rightSockImg2.composite(rightSockImg, 0, 174 * 2, OverCompositeOp);
        rightSockImg2.rotate(90);
        resizeImage_noAspect(rightSockImg2, 204, 82);
        rightSockImg2.crop(Geometry(81, 69, 64, 0));
        outputFileImg.composite(rightSockImg2, 81, 2 + 72, OverCompositeOp);

        Image shortsLeft(inputShortsImg, Geometry(435, 325, 77, 187));
        resizeImage_noAspect(shortsLeft, 161, 113);
        outputFileImg.composite(shortsLeft, 0, 143, OverCompositeOp);

        Image shortsRight(inputShortsImg, Geometry(435, 325, 512, 187));
        resizeImage_noAspect(shortsRight, 163, 113);
        shortsRight.flop();
        outputFileImg.composite(shortsRight, 0, 399, OverCompositeOp);

        if (!inputCrest.empty()) {
            Image inputCrestImg(inputCrest);
            resizeImage_noAspect(inputCrestImg, 36, 22);
            outputFileImg.composite(inputCrestImg, 381, 54 + 7, OverCompositeOp);
        }

        if (options.AddKitWrinkles) {
            Image kitOverlayImg(options.KitsPath + "kit_wrinkles08.png");
            if (kitOverlayImg.columns() != outputFileImg.columns())
                resizeImage_noAspect(kitOverlayImg, outputFileImg.columns(), outputFileImg.rows());
            outputFileImg.composite(kitOverlayImg, 0, 0, MultiplyCompositeOp);
        }

        if (options.AddKitOverlay) {
            Image kitOverlayImg(options.KitsPath + "kit_overlay08.png");
            if (kitOverlayImg.columns() != outputFileImg.columns())
                resizeImage_noAspect(kitOverlayImg, outputFileImg.columns(), outputFileImg.rows());
            outputFileImg.composite(kitOverlayImg, 0, 0, OverCompositeOp);
        }

        outputFileImg.write(outputFile + "." + options.OutputFormat);
    }

    return true;
}

bool KitConverter::ConvertFifaClubKit(int fifaId, int set, int variation, string const & outputFile) {
    printf("Converting club %d (%d)", fifaId, set);
    string kitFileName = "kit_" + to_string(fifaId) + "_" + to_string(set) + "_" + to_string(variation) + ".png";
    string kitFolder = options.KitsPath;
    string shirtFileName = kitFolder + "shirts\\" + kitFileName;
    string shortsFileName = kitFolder + "shorts\\" + kitFileName;
    string crestFileName = kitFolder + "crest\\" + kitFileName;
    if (exists(shirtFileName) && exists(shortsFileName)) {
        if (!exists(crestFileName))
            crestFileName.clear();
        bool result = ConvertKit(shirtFileName, shortsFileName, crestFileName, outputFile);
        if (result) {
            printf(" - done\n");
            return true;
        }
    }
    printf(" - failed\n");
    return false;
}

void KitConverter::ConvertClubKits(int fifaId, int fifaManagerId) {
    static char clubIdStr[10];
    sprintf_s(clubIdStr, "%08X", fifaManagerId);
    static char gameIdStr[10];
    sprintf_s(gameIdStr, "%02d", options.OutputGameId);
    string outputFile;
    if (options.SaveLocation == Big) {

    }
    else {
        if (options.SaveLocation == Documents && options.OutputGameId >= 9)
            outputFile = string("C:\\Users\\Dmitri\\Documents\\FIFA MANAGER ") + gameIdStr + "\\Graphics\\3DMatch\\Kits\\";
        else
            outputFile = string("D:\\Games\\FIFA Manager ") + gameIdStr + "\\user\\kits\\";
        outputFile += clubIdStr;
        if (options.ConvertHomeKit)
            ConvertFifaClubKit(fifaId, 0, 0, outputFile + "_h");
        if (options.ConverAwayKit)
            ConvertFifaClubKit(fifaId, 1, 0, outputFile + "_a");
        if (options.ConvertThirdKit)
            ConvertFifaClubKit(fifaId, 3, 0, outputFile + "_t");
        if (options.ConvertGkKit)
            ConvertFifaClubKit(fifaId, 2, 0, outputFile + "_g");
    }
}
