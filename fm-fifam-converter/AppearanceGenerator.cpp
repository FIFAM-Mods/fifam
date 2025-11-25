#include "AppearanceGenerator.h"
#include "FifamPlayer.h"
#include "FifaPlayer.h"
#include "FifamPlayerAppearance.h"
#include "FifaDatabase.h"
#include "Random.h"

void AppearanceGenerator::Read(Path const &filename) {
    static Set<Int> ShortHairIDs = {
        0,1,2,16,17,18,19,21,22,24,25,26,28,29,30,31,36,37,38,39,40,41,42,43,45,46,47,52,53,56,57,59,60,61,62,63,64,65,66,67,69,74,76,77,81,82,84,85,87,88,89,91,92,93,94,96,97,99,100,101,102,103,104,105,106,110,112,113,114,115,116,117,118,119,120,121,122,123,125,126,127,128,129,130,131,132,133,134,135,136,137,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,219,221,223,224,225,226,228,229,230,231,233,234,235,236,237,238,239,241,250,252,253,254,255,258,265,266,267,271,272,273,276,277,278,279,280,281,282,284,285,286,288,294,297,298,300,311,312,316,318,327,338,359,366,367,368,369,370,371,372,375,378,379,382,396,397,398,399,400,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,485,486,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,508,509,510,511,512,513,514,515,516,518,519,520,521,539,556,558,566,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,614,615,616,617,619,620,621,622,623,624,625,629,635,637,638,639,640,641,808
    };
    static Set<Int> MediumHairIDs = {
        3,8,10,13,14,20,23,35,44,50,51,58,68,70,72,73,80,83,86,90,95,98,107,108,109,111,124,138,167,218,222,227,232,240,242,246,247,248,249,251,256,260,263,264,268,269,270,274,275,283,287,289,290,296,299,302,313,315,320,321,322,326,328,329,333,334,335,337,339,340,341,344,345,346,350,351,353,354,356,360,361,362,363,365,373,374,376,377,380,383,385,386,387,401,404,405,406,407,426,427,432,484,487,517,540,554,557,565,584,585,587,588,589,590,591,592,593,594,613,618,626,627,628,630,643 
    };
    static Set<Int> LongHairIDs = {
        4,5,6,7,9,11,12,15,32,33,34,48,49,54,55,71,75,78,79,220,244,245,257,259,261,262,295,301,303,304,305,306,307,308,309,310,314,317,323,324,325,330,331,332,336,342,343,347,348,349,352,355,357,358,364,381,384,388,389,390,391,392,393,394,395,402,403,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,428,429,430,431,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,451,452,453,507,522,523,524,525,526,527,528,529,530,531,532,534,535,536,537,538,541,542,543,544,545,546,547,548,549,550,551,555,560,562,563,564,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,586,595,857
    };
    FifamAppearanceDefs defs;
    defs.Read(filename);
    for (auto const &[id, def] : defs.mAppearanceDefs) {
        mAppearanceDefs[id] = def;
        auto hairsDef = mAppearanceDefsHairstyles[id];
        auto const &hairIDs = def.mParameters[FifamAppearanceDefs::ParamHair];
        for (auto const &i : hairIDs) {
            if (Utils::Contains(ShortHairIDs, i.first)) {
                hairsDef.mHairstylesByLength[HairShort].push_back(i);
                hairsDef.mHairstylesSum[HairShort] += i.second;
            }
            else if (Utils::Contains(MediumHairIDs, i.first)) {
                hairsDef.mHairstylesByLength[HairMedium].push_back(i);
                hairsDef.mHairstylesSum[HairMedium] += i.second;
            }
            else if (Utils::Contains(LongHairIDs, i.first)) {
                hairsDef.mHairstylesByLength[HairLong].push_back(i);
                hairsDef.mHairstylesSum[HairLong] += i.second;
            }
        }
    }
}

Int AppearanceGenerator::GetRandomAppearanceParam(FifamAppearanceDefs::Type type, FifamAppearanceDefs::Param param, Int defaultValue) {
    auto &def = mAppearanceDefs[type];
    if (def.mParametersSum[param] > 0) {
        UInt rnd = Random::Get(0, def.mParametersSum[param] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < def.mParameters[param].size(); i++) {
            acc += def.mParameters[param][i].second;
            if (rnd < acc)
                return def.mParameters[param][i].first;
        }
    }
    return defaultValue;
}

Int AppearanceGenerator::GetRandomAppearanceHairstyle(FifamAppearanceDefs::Type type, AppearanceGenerator::HairLength hairLength, Int defaultValue) {
    auto &hairsDef = mAppearanceDefsHairstyles[type];
    if (hairsDef.mHairstylesSum[hairLength] > 0) {
        UInt rnd = Random::Get(0, hairsDef.mHairstylesSum[hairLength] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < hairsDef.mHairstylesByLength[hairLength].size(); i++) {
            acc += hairsDef.mHairstylesByLength[hairLength][i].second;
            if (rnd < acc)
                return hairsDef.mHairstylesByLength[hairLength][i].first;
        }
    }
    return defaultValue;
}

void AppearanceGenerator::Generate(FifamPlayer *player, FifamAppearanceDefs::Type type) {
    player->mAppearance.mSkinColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamSkinColor, player->mAppearance.mSkinColor);
    player->mAppearance.mGenericFace = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamFace, player->mAppearance.mGenericFace);
    player->mAppearance.mHairStyle = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamHair, player->mAppearance.mHairStyle);
    player->mAppearance.mFaceVariation = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamVariation, player->mAppearance.mFaceVariation);
    player->mAppearance.mBeardColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamBeardColor, player->mAppearance.mBeardColor);
    player->mAppearance.mEyeColour = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamEyeColor, player->mAppearance.mEyeColour);
    player->mAppearance.mBeardType = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamBeard, player->mAppearance.mBeardType);
    player->mAppearance.mSideburns = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamSideburns, player->mAppearance.mSideburns);
    player->mAppearance.mHairColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamHairColor, player->mAppearance.mHairColor);
    player->mAppearance.SetBeardColorFromHairColor();
}

enum ShoeType {
    SHOE_WHITE = 1,
    SHOE_BLACK = 2,
    SHOE_BLUE = 3,
    SHOE_RED = 4,
    SHOE_GOLD = 5,
    SHOE_WHITE_BLUE = 6,
    SHOE_WHITE_BLACK = 7,
    SHOE_WHITE_LIGHT_BLUE = 8,
    SHOE_GREEN = 10,
    SHOE_BLACK_RED = 11,
    SHOE_GREY = 12,
    SHOE_WHITE_RED = 13,
    SHOE_YELLOW = 14,
    SHOE_ORANGE = 15,
    SHOE_DARK_BLUE = 16,
    SHOE_LIGHT_GREEN = 17,
    SHOE_MAGENTA = 18,
    SHOE_BRIGHT_RED = 19
};

int GetPlayerShoesIdFromFifaId(int shoeTypeCode) {
    int ShoeType = 0;
    switch (shoeTypeCode) {
    case 17:
    case 20:
    case 21:
    case 31:
    case 136:
    case 148:
    case 150:
    case 162:
    case 205:
    case 210:
    case 211:
    case 213:
    case 214:
    case 238:
    case 319:
    case 321:
    case 334:
    case 362:
    case 380:
    case 385:
        ShoeType = SHOE_WHITE;
        break;
    case 26:
    case 37:
    case 42:
    case 54:
    case 55:
    case 56:
    case 180:
    case 185:
    case 194:
    case 197:
    case 202:
    case 225:
    case 229:
    case 234:
    case 250:
    case 300:
    case 302:
    case 306:
    case 307:
    case 308:
    case 314:
    case 315:
    case 339:
    case 355:
    case 357:
    case 359:
        ShoeType = SHOE_BLACK;
        break;
    case 29:
    case 32:
    case 53:
    case 62:
    case 75:
    case 84:
    case 91:
    case 131:
    case 132:
    case 133:
    case 146:
    case 157:
    case 175:
    case 193:
    case 195:
    case 203:
    case 212:
    case 230:
    case 301:
    case 317:
    case 323:
    case 325:
    case 369:
    case 371:
        ShoeType = SHOE_BLUE;
        break;
    case 52:
    case 97:
    case 117:
    case 167:
    case 168:
    case 172:
    case 173:
    case 186:
    case 188:
    case 189:
    case 249:
    case 340:
        ShoeType = SHOE_RED;
        break;
    case 28:
    case 35:
    case 66:
    case 67:
    case 68:
    case 71:
    case 98:
    case 253:
        ShoeType = SHOE_GOLD;
        break;
    case 18:
    case 108:
    case 247:
    case 311:
    case 316:
    case 363:
    case 378:
    case 379:
        ShoeType = SHOE_WHITE_BLUE;
        break;
    case 60:
    case 70:
    case 100:
    case 101:
    case 104:
    case 128:
    case 134:
    case 153:
    case 154:
    case 170:
    case 171:
    case 303:
    case 304:
    case 338:
    case 360:
    case 389:
    case 441:
    case 442:
        ShoeType = SHOE_WHITE_BLACK;
        break;
    case 72:
    case 74:
    case 92:
    case 94:
    case 159:
    case 222:
    case 305:
    case 318:
    case 368:
        ShoeType = SHOE_WHITE_LIGHT_BLUE;
        break;
    case 83:
    case 88:
    case 109:
    case 143:
    case 152:
    case 165:
    case 166:
    case 176:
    case 177:
    case 219:
    case 223:
    case 224:
    case 226:
    case 243:
    case 244:
    case 246:
    case 332:
    case 431:
    case 432:
        ShoeType = SHOE_GREEN;
        break;
    case 41:
    case 57:
    case 58:
    case 59:
    case 77:
    case 95:
    case 115:
    case 130:
    case 137:
    case 142:
    case 155:
    case 169:
    case 179:
    case 184:
    case 227:
    case 228:
    case 233:
    case 326:
    case 384:
        ShoeType = SHOE_BLACK_RED;
        break;
    case 22:
    case 27:
    case 30:
    case 33:
    case 39:
    case 47:
    case 61:
    case 80:
    case 82:
    case 107:
    case 149:
    case 160:
    case 191:
    case 192:
    case 196:
    case 198:
    case 204:
    case 207:
    case 236:
    case 237:
    case 312:
    case 320:
    case 324:
    case 330:
    case 333:
    case 354:
    case 356:
    case 358:
        ShoeType = SHOE_GREY;
        break;
    case 43:
    case 45:
    case 46:
    case 50:
    case 69:
    case 85:
    case 96:
    case 102:
    case 112:
    case 140:
    case 141:
    case 145:
    case 147:
    case 156:
    case 220:
    case 241:
    case 242:
    case 245:
    case 309:
    case 313:
    case 322:
    case 383:
        ShoeType = SHOE_WHITE_RED;
        break;
    case 81:
    case 86:
    case 99:
    case 110:
    case 113:
    case 121:
    case 122:
    case 164:
    case 182:
        ShoeType = SHOE_YELLOW;
        break;
    case 19:
    case 36:
    case 51:
    case 78:
    case 79:
    case 116:
    case 129:
    case 139:
    case 144:
    case 174:
    case 181:
    case 183:
    case 187:
    case 199:
    case 200:
    case 201:
    case 252:
    case 254:
    case 310:
    case 327:
    case 328:
    case 331:
    case 381:
    case 382:
        ShoeType = SHOE_ORANGE;
        break;
    case 38:
    case 40:
    case 64:
    case 73:
    case 93:
    case 135:
    case 161:
    case 190:
    case 206:
    case 221:
    case 231:
    case 235:
        ShoeType = SHOE_DARK_BLUE;
        break;
    case 16:
    case 23:
    case 25:
    case 34:
    case 63:
    case 76:
    case 87:
    case 90:
    case 105:
    case 106:
    case 111:
    case 114:
    case 125:
    case 126:
    case 138:
    case 158:
    case 163:
    case 215:
    case 216:
    case 217:
    case 218:
    case 232:
    case 335:
        ShoeType = SHOE_LIGHT_GREEN;
        break;
    case 48:
    case 49:
    case 65:
    case 89:
    case 118:
    case 119:
    case 120:
    case 123:
    case 124:
    case 127:
    case 178:
    case 239:
    case 240:
    case 248:
    case 251:
    case 329:
    case 336:
    case 337:
    case 364:
    case 365:
    case 366:
    case 367:
    case 370:
        ShoeType = SHOE_MAGENTA;
        break;
    case 24:
    case 44:
    case 103:
    case 151:
    case 208:
    case 209:
    case 361:
        ShoeType = SHOE_BRIGHT_RED;
        break;
    default:
        ShoeType = Random::Get(1, 19);
    }
    return ShoeType;
}

void AppearanceGenerator::SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer) {
    bool isLastVersion = fifaPlayer->m_gameId == FifaDatabase::m_lastSupportedGameVersion;
    switch (fifaPlayer->internal.skintonecode) {
    case 1:
        player->mAppearance.mSkinColor = 0;
        break;
    case 2:
        player->mAppearance.mSkinColor = 1;
        break;
    case 3:
        player->mAppearance.mSkinColor = 2;
        break;
    case 4:
        player->mAppearance.mSkinColor = 3;
        break;
    case 5:
        player->mAppearance.mSkinColor = 4;
        break;
    case 6:
        player->mAppearance.mSkinColor = 5;
        break;
    case 7:
        player->mAppearance.mSkinColor = 6;
        break;
    case 8:
        player->mAppearance.mSkinColor = 7;
        break;
    case 9:
        player->mAppearance.mSkinColor = 8;
        break;
    case 10:
        player->mAppearance.mSkinColor = 9;
        break;
    }
    // FM 26 mFaceVariation: 0 - normal, 1 - freckles, 2 - wrinkles, 3 - female normal, 4 - female freckles, 5 - female wrinkles,
    //                       6 - female makeup 1, 7 - female makeup 2
    if (fifaPlayer->m_gameId >= 25) {
        // FC 25 skintypecode: 0 - normal, 1-5 - freckles, 6 - five o'clock shadow, 7 - aging spots
        // FC 25 skinsurfacepack
        // FC 25 skinmakeup: = 0 - none, 1-3 - makeup types
        if (fifaPlayer->IsMale()) {
            player->mAppearance.mFaceVariation = 0; // normal
            if (fifaPlayer->internal.skintypecode >= 1 && fifaPlayer->internal.skintypecode <= 5)
                player->mAppearance.mFaceVariation = 1; // freckles
            else {
                switch (fifaPlayer->internal.skinsurfacepack) {
                case 232201: // Male 30s Caucasian Heavy 01
                case 240201: // Male 40s African Heavy 01
                case 240202: // Male 40s African Heavy 02
                case 242201: // Male 40s Caucasian Heavy 01
                case 250201: // Male 50s African Heavy 01
                case 250202: // Male 50s African Heavy 02
                case 252001: // Male 50s Caucasian Thin 01
                    player->mAppearance.mFaceVariation = 2; // wrinkles
                    break;
                }
            }
        }
        else {
            player->mAppearance.mFaceVariation = 3; // female normal
            if (fifaPlayer->internal.skintypecode >= 1 && fifaPlayer->internal.skintypecode <= 5)
                player->mAppearance.mFaceVariation = 4; // female freckles
            else {
                if (fifaPlayer->internal.skinmakeup != 0) {
                    if (fifaPlayer->internal.skinmakeup == 1)
                        player->mAppearance.mFaceVariation = 6; // female makeup 1
                    else
                        player->mAppearance.mFaceVariation = 7; // female makeup 2
                }
                else {
                    switch (fifaPlayer->internal.skinsurfacepack) {
                    case 122201: // Female 20s Caucasian Heavy 01
                    case 131201: // Female 30s Asian Heavy 01
                    case 132201: // Female 30s Caucasian Heavy 01
                    case 132202: // Female 30s Caucasian Heavy 02
                    case 141001: // Female 40s Asian Thin 01
                        player->mAppearance.mFaceVariation = 5; // female wrinkles
                        break;
                    }
                }
            }
        }
    }
    else {
        // skintypecode: 0 - normal, 1 - freckles, 2 - wrinkles, 5-7 - ?, 50 - young, 51 - old,
        //              100 - female normal, 101 - female wrinkles, 102 - female makeup 1, 103 - female makeup 2,
        //              104 - female freckles, 150 - female young, 151 - female old
        if (fifaPlayer->IsMale()) {
            player->mAppearance.mFaceVariation = 0; // normal
            switch (fifaPlayer->internal.skintypecode) {
            case 1:
            case 104:
                player->mAppearance.mFaceVariation = 1; // freckles
                break;
            case 2:
            case 51:
            case 101:
            case 151:
                player->mAppearance.mFaceVariation = 2; // wrinkles
                break;
            }
        }
        else {
            player->mAppearance.mFaceVariation = 3; // female normal
            switch (fifaPlayer->internal.skintypecode) {
            case 1:
            case 104:
                player->mAppearance.mFaceVariation = 4; // female freckles
                break;
            case 2:
            case 51:
            case 101:
            case 151:
                player->mAppearance.mFaceVariation = 5; // female wrinkles
                break;
            case 102:
                player->mAppearance.mFaceVariation = 6; // female makeup 1
                break;
            case 103:
                player->mAppearance.mFaceVariation = 7; // female makeup 2
                break;
            }
        }
    }
    // 0 - light blue 1 - brown 2 - grey-green 3 - green 4 - green-blue 5 -grey 6 - blue
    switch (fifaPlayer->internal.eyecolorcode) {
    case 1:
        player->mAppearance.mEyeColour = 6;
        break;
    case 2:
        player->mAppearance.mEyeColour = 0;
        break;
    case 3:
        player->mAppearance.mEyeColour = 1;
        break;
    case 4:
        player->mAppearance.mEyeColour = 7;
        break;
    case 5:
        player->mAppearance.mEyeColour = 4;
        break;
    case 6:
        player->mAppearance.mEyeColour = 3;
        break;
    case 7:
        player->mAppearance.mEyeColour = 9;
        break;
    case 8:
        player->mAppearance.mEyeColour = 5;
        break;
    case 9:
        player->mAppearance.mEyeColour = 8;
        break;
    case 10:
        player->mAppearance.mEyeColour = 2;
        break;
    }
    if (fifaPlayer->internal.sideburnscode)
        player->mAppearance.mSideburns = 1;
    else
        player->mAppearance.mSideburns = 0;
    if (fifaPlayer->IsMale()) {
        switch (fifaPlayer->internal.facialhairtypecode) {
        case 0:
        case 246:
            player->mAppearance.mBeardType = 0; // Clean Shaved
            break;
        case 1:
        case 254:
        case 259:
        case 288:
            player->mAppearance.mBeardType = 1; // Chin Stubble Light  _
            break;
        case 2:
        case 285:
            player->mAppearance.mBeardType = 2; // Chin Strap \_/
            break;
        case 3:
        case 251:
            player->mAppearance.mBeardType = 3; // Goatee |_|
            break;
        case 4:
        case 28:
        case 38:
        case 45:
        case 50:
        case 53:
        case 64:
        case 243:
        case 255:
            player->mAppearance.mBeardType = 4; // Casual Beard \|_|/
            break;
        case 5:
        case 27:
        case 31:
        case 32:
            player->mAppearance.mBeardType = 5; // PartialGoatee |_|
            break;
        case 6:
        case 57:
            player->mAppearance.mBeardType = 6; // Stubble \|_|/
            break;
        case 7:
            player->mAppearance.mBeardType = 7; // Tuft \/
            break;
        case 8:
        case 18:
        case 20:
        case 21:
        case 25:
        case 30:
        case 34:
        case 35:
        case 39:
        case 41:
        case 54:
        case 61:
        case 62:
        case 63:
        case 65:
        case 66:
        case 67:
        case 238:
        case 245:
        case 249:
        case 252:
        case 266:
        case 270:
        case 289:
            player->mAppearance.mBeardType = 8; // Full Beard \|_|/
            break;
        case 9:
        case 16:
        case 17:
        case 29:
        case 46:
        case 47:
        case 48:
        case 49:
        case 257:
        case 286:
            player->mAppearance.mBeardType = 9; // Light Goatee |_|
            break;
        case 10:
        case 33:
        case 36:
        case 42:
        case 43:
            player->mAppearance.mBeardType = 10; // Moustache --
            break;
        case 11:
        case 236:
            player->mAppearance.mBeardType = 11; // Light Chin Curtain \\//
            break;
        case 12:
        case 19:
        case 60:
        case 248:
        case 253:
        case 258:
            player->mAppearance.mBeardType = 12; // FullGoatee |_|
            break;
        case 13:
        case 22:
        case 237:
        case 247:
        case 260:
        case 272:
            player->mAppearance.mBeardType = 13; // Chin Curtain \\//
            break;
        case 14:
        case 23:
        case 26:
        case 44:
        case 51:
        case 55:
        case 56:
        case 58:
        case 244:
            player->mAppearance.mBeardType = 14; // Beard \|_|/
            break;
        case 15:
        case 24:
        case 37:
        case 40:
        case 52:
        case 59:
        case 287:
            player->mAppearance.mBeardType = 15; // Patchy Beard \|_|/
            break;
        }
    }
    else
        player->mAppearance.mBeardType = 0; // Clean Shaved
    // 0 - black, 1 - blonde, 2 - brown, 3 - medium blonde, 4 - red
    if (fifaPlayer->m_gameId >= 23) {
        switch (fifaPlayer->internal.facialhaircolorcode) {
        case 0:
        case 8:
        case 9:
        case 10:
        case 11:
        case 21:
        case 22:
        case 23:
        case 24:
        case 32:
        case 33:
        case 34:
            player->mAppearance.mBeardColor = 0; // black
            break;
        case 4:
        case 15:
        case 16:
        case 17:
        case 18:
            player->mAppearance.mBeardColor = 1; // blonde
            break;
        case 3:
        case 6:
        case 12:
        case 13:
            player->mAppearance.mBeardColor = 2; // brown
            break;
        case 1:
        case 2:
        case 5:
        case 19:
        case 20:
            player->mAppearance.mBeardColor = 3; // medium blonde (light brown)
            break;
        case 7:
        case 14:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            player->mAppearance.mBeardColor = 4; // red
            break;
        }
    }
    else {
        switch (fifaPlayer->internal.facialhaircolorcode) {
        case 0:
            player->mAppearance.mBeardColor = 0; // black
            break;
        case 1:
            player->mAppearance.mBeardColor = 1; // blonde
            break;
        case 2:
            player->mAppearance.mBeardColor = 2; // brown
            break;
        case 3:
            player->mAppearance.mBeardColor = 3; // medium blonde (light brown)
            break;
        case 4:
            player->mAppearance.mBeardColor = 4; // red
            break;
        }
    }

    static Map<UInt, UInt> fifaHairMapping = {
        // direct mapping
        {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{26,26},{27,27},{28,28},{29,29},{30,30},{31,31},{32,32},{33,33},{34,34},{35,35},{36,36},{37,37},{38,38},{39,39},{40,40},{41,41},{42,42},{43,43},{44,44},{45,45},{46,46},{47,47},{48,48},{49,49},{51,50},{52,51},{53,52},{54,53},{55,54},{56,55},{57,56},{58,57},{59,58},{60,59},{61,60},{62,61},{63,62},{64,63},{65,64},{66,65},{67,66},{68,67},{69,68},{70,69},{71,70},{72,71},{73,72},{74,73},{75,74},{76,75},{77,76},{78,77},{79,78},{80,79},{81,80},{82,81},{83,82},{84,83},{85,84},{86,85},{87,86},{88,87},{89,88},{90,89},{91,90},{92,91},{93,92},{94,93},{95,94},{96,95},{97,96},{98,97},{99,98},{100,99},{101,100},{102,101},{103,102},{104,103},{105,104},{106,105},{107,106},{108,107},{109,108},{110,109},{111,110},{112,111},{113,112},{114,113},{115,114},{116,115},{117,116},{118,117},{119,118},{120,119},{121,120},{122,121},{123,122},{124,123},{125,124},{126,125},{127,126},{128,127},{129,128},{130,129},{131,130},{132,131},{133,132},{134,133},{135,134},{136,135},{137,136},{138,137},{139,138},{140,139},{141,140},{142,141},{143,142},{144,143},{145,144},{146,145},{147,146},{148,147},{149,148},{150,149},{151,150},{152,151},{153,152},{154,153},{155,154},{156,155},{157,156},{158,157},{159,158},{160,159},{161,160},{162,161},{163,162},{164,163},{165,164},{166,165},{167,166},{168,167},{169,168},{170,169},{171,170},{172,171},{173,172},{174,173},{175,174},{176,175},{177,176},{178,177},{179,178},{180,179},{181,180},{182,181},{183,182},{184,183},{185,184},{186,185},{187,186},{188,187},{189,188},{190,189},{191,190},{192,191},{193,192},{194,193},{195,194},{196,195},{197,196},{198,197},{199,198},{200,199},{201,200},{202,201},{203,202},{204,203},{205,204},{206,205},{207,206},{208,207},{209,208},{210,209},{211,210},{212,211},{218,212},{219,213},{220,214},{221,215},{222,216},{223,217},{224,218},{225,219},{226,220},{227,221},{228,222},{229,223},{230,224},{231,225},{232,226},{233,227},{234,228},{235,229},{236,230},{237,231},{238,232},{239,233},{240,234},{241,235},{242,236},{243,237},{244,238},{245,239},{246,240},{250,241},{251,242},{253,243},{255,244},{256,245},{257,246},{258,247},{260,248},{261,249},{262,250},{263,251},{264,252},{265,253},{266,254},{267,255},{268,256},{269,257},{270,258},{271,259},{272,260},{273,261},{274,262},{275,263},{276,264},{277,265},{278,266},{279,267},{280,268},{281,269},{282,270},{283,271},{284,272},{285,273},{286,274},{288,275},{289,276},{290,277},{291,278},{292,279},{293,280},{294,281},{295,282},{296,283},{297,284},{298,285},{299,286},{374,287},{375,288},{376,289},{381,290},{385,291},{386,292},{389,293},{390,294},{392,295},{394,296},{398,297},{399,298},{418,299},{419,300},{421,301},{431,302},{432,303},{433,304},{434,305},{435,306},{436,307},{437,308},{438,309},{444,310},{445,311},{446,312},{447,313},{448,314},{450,315},{451,316},{452,317},{460,318},{461,319},{462,320},{463,321},{492,322},{494,323},{500,324},{501,325},{502,326},{503,327},{504,328},{505,329},{506,330},{507,331},{508,332},{509,333},{510,334},{511,335},{512,336},{513,337},{514,338},{515,339},{516,340},{517,341},{518,342},{519,343},{520,344},{521,345},{522,346},{523,347},{524,348},{525,349},{526,350},{527,351},{528,352},{529,353},{530,354},{531,355},{532,356},{533,357},{534,358},{535,359},{536,360},{537,361},{538,362},{539,363},{540,364},{541,365},{542,366},{543,367},{544,368},{545,369},{546,370},{547,371},{548,372},{549,373},{550,374},{551,375},{552,376},{553,377},{554,378},{555,379},{556,380},{557,381},{558,382},{559,383},{560,384},{561,385},{562,386},{563,387},{564,388},{565,389},{566,390},{567,391},{568,392},{569,393},{570,394},{571,395},{572,396},{573,397},{574,398},{575,399},{576,400},{577,401},{578,402},{579,403},{580,404},{581,405},{582,406},{583,407},{584,408},{585,409},{586,410},{587,411},{588,412},{589,413},{590,414},{591,415},{592,416},{593,417},{594,418},{595,419},{596,420},{597,421},{598,422},{599,423},{600,424},{601,425},{602,426},{603,427},{604,428},{605,429},{606,430},{607,431},{608,432},{609,433},{610,434},{611,435},{612,436},{613,437},{614,438},{615,439},{616,440},{617,441},{618,442},{619,443},{620,444},{621,445},{622,446},{623,447},{624,448},{625,449},{626,450},{627,451},{628,452},{629,453},{630,454},{631,455},{632,456},{633,457},{634,458},{635,459},{636,460},{637,461},{638,462},{639,463},{640,464},{641,465},{642,466},{643,467},{644,468},{645,469},{646,470},{647,471},{648,472},{649,473},{650,474},{651,475},{652,476},{653,477},{654,478},{655,479},{656,480},{657,481},{659,482},{660,483},{661,484},{662,485},{663,486},{664,487},{665,488},{666,489},{667,490},{668,491},{669,492},{670,493},{671,494},{672,495},{673,496},{674,497},{675,498},{676,499},{677,500},{678,501},{679,502},{680,503},{681,504},{682,505},{683,506},{684,507},{685,508},{686,509},{687,510},{688,511},{689,512},{690,513},{691,514},{692,515},{693,516},{694,517},{695,518},{696,519},{697,520},{698,521},{720,522},{721,523},{722,524},{723,525},{724,526},{725,527},{726,528},{727,529},{728,530},{729,531},{730,532},{731,533},{732,534},{733,535},{734,536},{735,537},{736,538},{737,539},{738,540},{739,541},{740,542},{741,543},{742,544},{743,545},{744,546},{745,547},{746,548},{747,549},{748,550},{749,551},{810,552},{811,553},{815,554},{816,555},{828,556},{831,557},{832,558},{845,559},{881,560},{890,561},{891,562},{892,563},{900,564},{901,565},{902,566},{903,567},{904,568},{905,569},{906,570},{907,571},{908,572},{909,573},{910,574},{911,575},{912,576},{913,577},{914,578},{915,579},{916,580},{917,581},{918,582},{919,583},{920,584},{921,585},{922,586},{923,587},{924,588},{925,589},{926,590},{927,591},{928,592},{930,593},{931,594},{932,595},{1059,596},{1060,597},{1061,598},{1062,599},{1063,600},{1064,601},{1065,602},{1066,603},{1067,604},{1068,605},{1069,606},{1070,607},{1071,608},{1072,609},{1073,610},{1074,611},{1075,612},{1076,613},{1077,614},{1078,615},{1079,616},{1080,617},{1081,618},{1082,619},{1083,620},{1084,621},{1085,622},{1086,623},{1087,624},{1088,625},{1089,626},{1090,627},{1091,628},{1092,629},{1093,630},{1094,631},{1095,632},{1096,633},{1099,634},{1106,635},{1157,636},{1281,637},{1282,638},{1283,639},{1284,640},{1285,641},{1316,642},{1317,643},{1337,644},{1338,645},{1341,646},{1368,647},{1370,648},{1373,649},{1374,650},{1377,651},{1378,652},{1384,653},{1385,654},{1386,655},{2000,656},{2001,657},{2002,658},{2003,659},{2004,660},{2005,661},{2006,662},{2007,663},{2008,664},{2009,665},{2010,666},{2011,667},{2012,668},{2013,669},{2014,670},{2015,671},{2016,672},{2017,673},{2018,674},{2019,675},{2020,676},{2021,677},{2022,678},{2023,679},{2024,680},{2025,681},{2026,682},{2027,683},{2028,684},{2029,685},{2030,686},{2031,687},{2032,688},{2033,689},{2034,690},{2035,691},{2036,692},{2037,693},{2038,694},{2039,695},{2040,696},{2041,697},{2042,698},{2043,699},{2044,700},{2045,701},{2046,702},{2047,703},{2048,704},{2049,705},{2050,706},{2051,707},{2052,708},{2053,709},{2054,710},{2055,711},{2056,712},{2057,713},{2058,714},{2059,715},{2060,716},{2061,717},{2062,718},{2063,719},{2064,720},{2065,721},{2066,722},{2067,723},{2068,724},{3000,725},{3001,726},{3002,727},{3003,728},{3004,729},{3005,730},{3006,731},{3007,732},{3008,733},{3009,734},{3010,735},{3011,736},{3012,737},{3013,738},{3014,739},{3015,740},{3016,741},{3017,742},{3018,743},{3019,744},{3020,745},{3021,746},{3022,747},{3025,748},{3026,749},{3027,750},{3028,751},{3029,752},{3030,753},{3031,754},{3032,755},{3033,756},{3034,757},{3035,758},{3036,759},{3037,760},{3038,761},{3039,762},{3040,763},{3041,764},{3042,765},{3043,766},{3044,767},{3045,768},{3046,769},{3047,770},{3048,771},{3049,772},{3050,773},{3051,774},{3052,775},{3053,776},{3055,777},{3056,778},{3057,779},{3058,780},{3059,781},{3060,782},{3061,783},{3062,784},{3063,785},{3064,786},{3065,787},{3066,788},{3067,789},{3068,790},{3069,791},{3070,792},{3071,793},{3072,794},{3073,795},{3074,796},{3075,797},{3076,798},{3077,799},{3078,800},{3079,801},{3080,802},{3081,803},{3082,804},{3083,805},{3084,806},{3085,807},{3086,808},{3087,809},{3088,810},{3089,811},{3090,812},{3091,813},{3092,814},{3093,815},{3094,816},{3095,817},{3096,818},{3097,819},{3098,820},{3100,821},{3101,822},{3102,823},{3103,824},{3104,825},{3105,826},{3106,827},{3107,828},{3108,829},{3109,830},{3110,831},{3111,832},{3112,833},{3113,834},{3114,835},{3115,836},{3116,837},{3117,838},{3118,839},{3119,840},{3120,841},{3121,842},{3122,843},{3123,844},{3124,845},{3125,846},{3126,847},{3127,848},{3128,849},{3129,850},{3130,851},{3131,852},{3132,853},{3133,854},{3134,855},{3135,856},{3136,857},{3137,858},{3138,859},{3139,860},{3140,861},{3141,862},{3142,863},{3143,864},{3144,865},{3145,866},{3146,867},{3147,868},{3148,869},{3149,870},{3150,871},{3151,872},{3152,873},{3153,874},{3154,875},{3155,876},{3156,877},{3157,878},{3158,879},{3159,880},{3160,881},{3161,882},{3162,883},{3163,884},{3164,885},{3165,886},{3166,887},{3167,888},{3168,889},{3169,890},{3170,891},{3171,892},{3172,893},{3173,894},{3174,895},{3175,896},{3176,897},{3177,898},{3178,899},{3179,900},{3180,901},{3181,902},{3182,903},{3183,904},{3184,905},{3185,906},{3186,907},{3187,908},{3188,909},{3189,910},{3190,911},{3191,912},{3192,913},{3193,914},{3194,915},{3195,916},{3196,917},{3197,918},{3198,919},{3199,920},{3200,921},{3201,922},{3202,923},{3203,924},{3204,925},{3205,926},{3206,927},{3207,928},{3208,929},{3209,930},{3210,931},{3211,932},{3212,933},{3213,934},{3214,935},{3215,936},{3216,937},{3217,938},{3218,939},{3219,940},{3220,941},{3221,942},{3232,943},{3233,944},{3234,945},{3237,946},{3238,947},{3239,948},{3240,949},{3241,950},{3242,951},{3244,952},{3245,953},{3246,954},{3247,955},{3248,956},{3249,957},{3250,958},{3251,959},{3252,960},{3253,961},{3254,962},{3255,963},{3256,964},{3257,965},{3258,966},{3259,967},{3260,968},{3261,969},{3262,970},{3263,971},{3264,972},{3265,973},{3266,974},{3267,975},{3268,976},{3269,977},{3270,978},{3271,979},{3272,980},{3273,981},{3274,982},{3275,983},{3276,984},{3277,985},{3278,986},{3279,987},{3280,988},{3281,989},{3282,990},{3283,991},{3284,992},{3285,993},{3298,994},{3300,995},{4000,996},{4001,997},{4002,998},{4003,999},{4004,1000},{4005,1001},{4006,1002},{4007,1003},{4008,1004},{4009,1005},{4010,1006},{4011,1007},{4012,1008},{4013,1009},{4014,1010},{4015,1011},{4016,1012},{4017,1013},{4018,1014},{4019,1015},{4020,1016},{4021,1017},{4022,1018},{4023,1019},{4024,1020},{4025,1021},{4026,1022},{4027,1023},{4028,1024},{4029,1025},{4030,1026},{5000,1027},{5001,1028},{5002,1029},{5003,1030},{5004,1031},{5005,1032},{5006,1033},{5007,1034},{5008,1035},{5009,1036},{5010,1037},{5011,1038},{5012,1039},{5013,1040},{5014,1041},{5015,1042},{5016,1043},{5017,1044},{6000,1045},{6001,1046},{6002,1047},{6003,1048},{6004,1049},{6005,1050},{6006,1051},{6007,1052},{6008,1053},{6009,1054},{6010,1055},{6011,1056},{6012,1057},{6013,1058},{6014,1059},{6015,1060},{6016,1061},{6017,1062},{6018,1063},{6019,1064},{6020,1065},{6021,1066},{6022,1067},{6023,1068},{6024,1069},{6025,1070},{6026,1071},{6033,1072},{6035,1073},{6036,1074},{6037,1075},{6038,1076},{6039,1077},{6040,1078},{6041,1079},{6042,1080},{7007,1081},{7010,1082},{7013,1083},{7014,1084},{7018,1085},{7021,1086},{7025,1087},{7027,1088},{7034,1089},{7036,1090},{7037,1091},{7038,1092},{7045,1093},{7046,1094},{7047,1095},{7050,1096},{7051,1097},{7052,1098},{7054,1099},{7056,1100},{7057,1101},{7060,1102},{7070,1103},{7072,1104},{7080,1105},{7082,1106},{7083,1107},{7087,1108},{7095,1109},{7103,1110},{7117,1111},{7129,1112},{7136,1113},{7140,1114},{7145,1115},{7147,1116},{7155,1117},{7161,1118},{7163,1119},{7167,1120},{7181,1121},{7182,1122},{7183,1123},{7192,1124},{7195,1125},{7198,1126},{7251,1127},{7254,1128},{7257,1129},{7260,1130},{7262,1131},{7263,1132},{7264,1133},{7268,1134},{7270,1135},{7271,1136},{7273,1137},{7275,1138},{7277,1139},{7284,1140},{7286,1141},{7287,1142},{7288,1143},{7289,1144},{7292,1145},{7293,1146},{7294,1147},{7295,1148},{7296,1149},{7297,1150},{7298,1151},{7299,1152},{7300,1153},{7301,1154},{7302,1155},{7304,1156},{7306,1157},{7307,1158},{7310,1159},{7320,1160},{7322,1161},{7330,1162},{7332,1163},{7333,1164},{7337,1165},{7345,1166},{7353,1167},{7367,1168},{7379,1169},{7383,1170},{7386,1171},{7390,1172},{7395,1173},{7397,1174},{7405,1175},{7411,1176},{7413,1177},{7417,1178},{7431,1179},{7432,1180},{7433,1181},{7442,1182},{7445,1183},{7448,1184},{7538,1185},{7541,1186},{7544,1187},{7545,1188},{7549,1189},{7552,1190},{7556,1191},{7558,1192},{7565,1193},{7567,1194},{7568,1195},{7569,1196},{7576,1197},{7577,1198},{7578,1199},{7581,1200},{7582,1201},{7583,1202},{7585,1203},{7587,1204},{7588,1205},{7591,1206},{7601,1207},{7603,1208},{7611,1209},{7613,1210},{7614,1211},{7618,1212},{7626,1213},{7634,1214},{7648,1215},{7660,1216},{7664,1217},{7667,1218},{7671,1219},{7676,1220},{7678,1221},{7686,1222},{7692,1223},{7694,1224},{7698,1225},{7712,1226},{7713,1227},{7714,1228},{7723,1229},{7726,1230},{7729,1231}
    };
    if (Utils::Contains(fifaHairMapping, (UInt)fifaPlayer->internal.hairtypecode))
        player->mAppearance.mHairStyle = fifaHairMapping[fifaPlayer->internal.hairtypecode];

    // 0 - blonde, 1 - black, 2 - medium blonde, 3 - darkbrown, 4 - platinumblonde, 5 - lightbrown, 6 - brown, 7 - red
    if (fifaPlayer->m_gameId >= 23) {
        switch (fifaPlayer->internal.haircolorcode) {
        case 17:
        case 18:
            player->mAppearance.mHairColor = 0; // blonde
            break;
        case 0:
        case 10:
        case 11:
        case 21:
        case 22:
        case 32:
        case 33:
        case 34:
            player->mAppearance.mHairColor = 1; // black
            break;
        case 4:
        case 16:
            player->mAppearance.mHairColor = 2; // medium blonde
            break;
        case 3:
        case 13:
            player->mAppearance.mHairColor = 3; // dark brown
            break;
        case 15:
            player->mAppearance.mHairColor = 4; // platinum blonde
            break;
        case 1:
        case 2:
        case 5:
        case 19:
        case 20:
            player->mAppearance.mHairColor = 5; // light brown
            break;
        case 6:
        case 12:
            player->mAppearance.mHairColor = 6; // brown
            break;
        case 7:
        case 14:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            player->mAppearance.mHairColor = 7; // red
            break;
        case 8:
        case 9:
        case 23:
        case 24:
            player->mAppearance.mHairColor = 8; // gray
            break;
        }
    }
    else {
        switch (fifaPlayer->internal.haircolorcode) {
        case 0:
            player->mAppearance.mHairColor = 0; // blonde
            break;
        case 1:
            player->mAppearance.mHairColor = 1; // black
            break;
        case 2:
        case 9:
            player->mAppearance.mHairColor = 2; // medium blonde
            break;
        case 3:
            player->mAppearance.mHairColor = 3; // dark brown
            break;
        case 4:
            player->mAppearance.mHairColor = 4; // platinum blonde
            break;
        case 5:
            player->mAppearance.mHairColor = 5; // light brown
            break;
        case 6:
            player->mAppearance.mHairColor = 6; // brown
            break;
        case 7:
        case 12:
        case 13:
            player->mAppearance.mHairColor = 7; // red
            break;
        case 8:
            player->mAppearance.mHairColor = 8; // gray
            break;
        }
    }

    static Map<UInt, UInt> fifaHeadMapping = {
        // direct mapping
        {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{39,26},{40,27},{41,28},{42,29},{43,30},{44,31},{46,32},{47,33},{48,34},{49,35},{50,36},{51,37},{52,38},{53,39},{54,40},{55,41},{56,42},{57,43},{58,44},{59,45},{60,46},{61,47},{62,48},{63,49},{64,50},{65,51},{66,52},{67,53},{68,54},{69,55},{500,56},{501,57},{502,58},{503,59},{504,60},{505,61},{506,62},{507,63},{508,64},{509,65},{510,66},{511,67},{512,68},{513,69},{514,70},{515,71},{516,72},{517,73},{518,74},{519,75},{520,76},{521,77},{522,78},{523,79},{524,80},{525,81},{526,82},{527,83},{528,84},{529,85},{530,86},{531,87},{532,88},{533,89},{534,90},{535,91},{536,92},{537,93},{538,94},{539,95},{540,96},{541,97},{542,98},{543,99},{544,100},{545,101},{546,102},{547,103},{548,104},{549,105},{550,106},{551,107},{552,108},{553,109},{554,110},{555,111},{556,112},{557,113},{558,114},{559,115},{560,116},{561,117},{562,118},{563,119},{565,120},{567,121},{568,122},{600,123},{601,124},{602,125},{603,126},{604,127},{605,128},{606,129},{607,130},{608,131},{1000,132},{1001,133},{1002,134},{1003,135},{1004,136},{1005,137},{1006,138},{1007,139},{1008,140},{1009,141},{1010,142},{1011,143},{1012,144},{1013,145},{1014,146},{1015,147},{1016,148},{1017,149},{1018,150},{1019,151},{1020,152},{1021,153},{1022,154},{1023,155},{1024,156},{1025,157},{1026,158},{1027,159},{1030,160},{1031,161},{1033,162},{1035,163},{1036,164},{1037,165},{1038,166},{1039,167},{1040,168},{1041,169},{1042,170},{1043,171},{1044,172},{1045,173},{1046,174},{1047,175},{1048,176},{1049,177},{1050,178},{1051,179},{1052,180},{1500,181},{1501,182},{1502,183},{1503,184},{1504,185},{1505,186},{1506,187},{1507,188},{1508,189},{1509,190},{1510,191},{1511,192},{1512,193},{1513,194},{1514,195},{1515,196},{1516,197},{1517,198},{1518,199},{1519,200},{1520,201},{1521,202},{1522,203},{1523,204},{1524,205},{1525,206},{1526,207},{1527,208},{1528,209},{1530,210},{1531,211},{1532,212},{1533,213},{1534,214},{1535,215},{1536,216},{1540,217},{1541,218},{1542,219},{1543,220},{1545,221},{1546,222},{1547,223},{1548,224},{1549,225},{1550,226},{1551,227},{1552,228},{1553,229},{1554,230},{1555,231},{1556,232},{2000,233},{2001,234},{2002,235},{2003,236},{2004,237},{2005,238},{2006,239},{2007,240},{2008,241},{2009,242},{2010,243},{2011,244},{2012,245},{2013,246},{2014,247},{2015,248},{2016,249},{2017,250},{2019,251},{2020,252},{2021,253},{2022,254},{2023,255},{2024,256},{2025,257},{2026,258},{2027,259},{2028,260},{2029,261},{2032,262},{2500,263},{2501,264},{2502,265},{2503,266},{2504,267},{2505,268},{2506,269},{2507,270},{2508,271},{2509,272},{2510,273},{2511,274},{2512,275},{2513,276},{2514,277},{2515,278},{2516,279},{2517,280},{2518,281},{2520,282},{2521,283},{2522,284},{3000,285},{3001,286},{3002,287},{3003,288},{3004,289},{3005,290},{3500,291},{3501,292},{3502,293},{3503,294},{3504,295},{3505,296},{3507,297},{3509,298},{3510,299},{3512,300},{3513,301},{3514,302},{3515,303},{3516,304},{3517,305},{3518,306},{3519,307},{3520,308},{4000,309},{4001,310},{4002,311},{4003,312},{4008,313},{4009,314},{4010,315},{4011,316},{4012,317},{4013,318},{4014,319},{4015,320},{4016,321},{4017,322},{4018,323},{4019,324},{4020,325},{4021,326},{4022,327},{4023,328},{4024,329},{4500,330},{4501,331},{4502,332},{4525,333},{5000,334},{5001,335},{5002,336},{5003,337},{5500,338},{5501,339},{5502,340},{5503,341},{5504,342},{5505,343},{5506,344},{5507,345},{5508,346},{5509,347},{5510,348},{5512,349},{6000,350},{6001,351},{6002,352},{6003,353},{6004,354},{6005,355},{6006,356},{6007,357},{6008,358},{6009,359},{6010,360},{6011,361},{6012,362},{6013,363},{6014,364},{6015,365},{6016,366},{6017,367},{6018,368},{6019,369},{6020,370},{6021,371},{6022,372},{6023,373},{6024,374},{6025,375},{6026,376},{6027,377},{6028,378},{6029,379},{6500,380},{6501,381},{6502,382},{6506,383},{6508,384},{6509,385},{6510,386},{7000,387},{7001,388},{7002,389},{7003,390},{7004,391},{7005,392},{7006,393},{7007,394},{7008,395},{7009,396},{7010,397},{7011,398},{7012,399},{7013,400},{7014,401},{7015,402},{7016,403},{7017,404},{7018,405},{7019,406},{7020,407},{7021,408},{7022,409},{7023,410},{7024,411},{7025,412},{7026,413},{7029,414},{7033,415},{7034,416},{7035,417},{7036,418},{7037,419},{7038,420},{7500,421},{7501,422},{7502,423},{8000,424},{8500,425},{8501,426},{9000,427},{9001,428},{9002,429},{9005,430},{9008,431},{9010,432},{9500,433},{9501,434},{9502,435},{9503,436},{9504,437},{9505,438},{9506,439},{9507,440},{9508,441},{9509,442},{9510,443},{9511,444},{9512,445},{9519,446},{9520,447},{9522,448},{9524,449},{9525,450},{9527,451},{9528,452},{10000,453},{10001,454},{10003,455},{10004,456},{10005,457},{10006,458},{10007,459},{10008,460},{10009,461},{10010,462},{10011,463},{10012,464},{10013,465},{10014,466},{10015,467},{10016,468},{10017,469},{10018,470},{10019,471},{10020,472},{10021,473},{10022,474},{10023,475},{10024,476},{10025,477},{10026,478},{10027,479},{10500,480},{10501,481},{10502,482},{10503,483},{10504,484},{10505,485},{10506,486},{10507,487},
        // missing heads
        {26,46},{27,45},{28,16},{29,49},{30,4},{31,1},{32,7},{33,24},{34,53},{35,42},{36,28},{37,2},{38,9},{1028,137},{1029,142},{1529,190},{1537,184},{1538,205},{1539,195},{2030,245},{2031,255},{2519,268},{3506,306},{3508,294},{4004,324},{4005,323},{4006,318},{4007,315},{5511,340},{5513,338},{5514,423},{5515,349},{5516,428},{5517,486},{5518,453},{5519,341},{5520,338},{5521,339},{5522,403},{5523,433},{5524,438},{5526,421},{6031,363},{6032,359},{6033,357},{6503,478},{6504,476},{6505,479},{6507,479},{6511,156},{6512,483},{6513,156},{6514,482},{6516,468},{6517,466},{6519,468},{6522,479},{7027,470},{7030,341},{7032,364},{7039,457},{7041,399},{7042,449},{7043,396},{7048,411},{9003,363},{9004,388},{9006,340},{9009,449},{9011,344},{9012,359},{9014,351},{9015,350},{9016,393},{9017,453},{9018,387},{9019,340},{9024,343},{9027,390},{9514,374},{9515,372},{9516,340},{9517,339},{9518,354},{9521,345},{9523,430},{9526,338},{9529,342},{9530,343},{9531,355},{9532,350},{9536,422},{9538,352},{9539,437},{9540,483},{10028,359},{10029,389},{10030,477}
    };
    if (Utils::Contains(fifaHeadMapping, (UInt)fifaPlayer->internal.headtypecode))
        player->mAppearance.mGenericFace = fifaHeadMapping[fifaPlayer->internal.headtypecode];
    
    player->mShoeType.SetFromInt(GetPlayerShoesIdFromFifaId(isLastVersion ? fifaPlayer->internal.shoetypecode : 0));
}
