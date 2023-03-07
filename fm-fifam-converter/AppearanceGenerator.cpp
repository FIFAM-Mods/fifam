#include "AppearanceGenerator.h"
#include "FifamPlayer.h"
#include "FifaPlayer.h"
#include "FifamPlayerAppearance.h"
#include "FifaDatabase.h"

void AppearanceGenerator::Read(Path const &filepath) {
    FifamReader reader(filepath, 0);
    if (reader.Available()) {
        reader.SkipLine();
        UInt numAppDefs = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numAppDefs; i++) {
            UInt id = reader.ReadLine<UInt>();
            AppearanceDef &def = mAppearanceDefs[id];
            UInt numTranslationNames = reader.ReadLine<UInt>();
            for (UInt n = 0; n < numTranslationNames; n++) {
                id = reader.ReadLine<UInt>();
                def.mNames[id] = reader.ReadFullLine();
            }
            def.unknown = reader.ReadLine<Int>();
            def.mHeight = reader.ReadLine<Int>();
            def.mWeight = reader.ReadLine<Int>();
            UInt numAppearanceParameters = reader.ReadLine<UInt>();
            for (UInt a = 0; a < numAppearanceParameters; a++) {
                UInt numValues = reader.ReadLine<UInt>();
                if (a < 9) {
                    def.mParameters[a].resize(numValues, { 0, 0 });
                    for (UInt v = 0; v < numValues; v++)
                        def.mParameters[a][v].first = reader.ReadLine<UChar>();
                }
                else
                    reader.SkipLines(numValues);
                numValues = reader.ReadLine<UInt>();
                if (a < 9) {
                    for (UInt v = 0; v < numValues; v++) {
                        if (v < def.mParameters[a].size()) {
                            UChar percentage = reader.ReadLine<UChar>();
                            def.mParameters[a][v].second = percentage;
                            def.mParametersSum[a] += percentage;
                        }
                        else
                            reader.SkipLine();
                    }
                }
                else
                    reader.SkipLines(numValues);
            }
        }
    }
}

UChar AppearanceGenerator::GetRandomAppearanceParam(AppearanceGenerator::AppearanceDef &def, AppearanceGenerator::Param param, UChar defaultValue) {
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

void AppearanceGenerator::Generate(FifamPlayer *player, AppearanceGenerator::Type type) {
    AppearanceDef &def = mAppearanceDefs[type];
    player->mAppearance.mSkinColor = GetRandomAppearanceParam(def, ParamSkinColor, player->mAppearance.mSkinColor);
    player->mAppearance.mGenericFace = GetRandomAppearanceParam(def, ParamFace, player->mAppearance.mGenericFace);
    player->mAppearance.mHairStyle = GetRandomAppearanceParam(def, ParamHair, player->mAppearance.mHairStyle);
    player->mAppearance.mFaceVariation = GetRandomAppearanceParam(def, ParamVariation, player->mAppearance.mFaceVariation);
    player->mAppearance.mBeardColor = GetRandomAppearanceParam(def, ParamBeardColor, player->mAppearance.mBeardColor);
    player->mAppearance.mEyeColour = GetRandomAppearanceParam(def, ParamEyeColor, player->mAppearance.mEyeColour);
    player->mAppearance.mBeardType = GetRandomAppearanceParam(def, ParamBeard, player->mAppearance.mBeardType);
    player->mAppearance.mSideburns = GetRandomAppearanceParam(def, ParamSideburns, player->mAppearance.mSideburns);
    player->mAppearance.mHairColor = GetRandomAppearanceParam(def, ParamHairColor, player->mAppearance.mHairColor);
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
    case 16:
    case 25:
    case 54:
    case 57:
    case 58:
    case 63:
    case 80:
    case 92:
    case 94:
    case 188:
    case 213:
    case 214:
    case 223:
        ShoeType = SHOE_WHITE;
        break;
    case 158:
    case 160:
    case 177:
    case 220:
    case 238:
        ShoeType = SHOE_WHITE_BLACK;
        break;
    case 24:
    case 36:
    case 37:
    case 46:
    case 56:
    case 60:
    case 65:
    case 83:
    case 84:
    case 85:
    case 91:
    case 93:
    case 108:
    case 146:
    case 157:
    case 159:
    case 161:
    case 173:
    case 175:
    case 176:
    case 179:
    case 183:
    case 185:
    case 195:
    case 197:
    case 201:
    case 225:
    case 227:
    case 228:
    case 229:
    case 234:
    case 300:
    case 302:
    case 303:
    case 306:
    case 307:
    case 308:
        ShoeType = SHOE_BLACK;
        break;
    case 28:
    case 30:
    case 40:
    case 53:
    case 59:
    case 70:
    case 71:
    case 72:
    case 74:
    case 79:
    case 88:
    case 89:
    case 147:
    case 170:
    case 172:
    case 184:
    case 189:
    case 199:
    case 205:
    case 206:
    case 221:
    case 222:
    case 226:
    case 230:
    case 231:
    case 235:
    case 301:
        ShoeType = SHOE_DARK_BLUE;
        break;
    case 38:
    case 97:
    case 143:
    case 186:
        ShoeType = SHOE_RED;
        break;
    case 18:
    case 19:
    case 32:
    case 34:
    case 98:
    case 99:
    case 139:
    case 144:
    case 145:
    case 156:
    case 191:
    case 192:
    case 310:
        ShoeType = SHOE_ORANGE;
        break;
    case 166:
    case 168:
    case 174:
        ShoeType = SHOE_GOLD;
        break;
    case 75:
    case 76:
        ShoeType = SHOE_GREEN;
        break;
    case 17:
    case 31:
    case 78:
    case 194:
    case 196:
    case 239:
    case 240:
        ShoeType = SHOE_MAGENTA;
        break;
    case 62:
    case 69:
    case 90:
    case 95:
    case 96:
    case 193:
    case 212:
    case 215:
    case 216:
        ShoeType = SHOE_BLUE;
        break;
    case 27:
    case 33:
    case 45:
    case 61:
    case 66:
    case 68:
    case 73:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 130:
    case 131:
    case 132:
    case 133:
    case 137:
    case 138:
    case 153:
    case 154:
    case 155:
    case 169:
    case 178:
    case 187:
    case 198:
    case 207:
    case 236:
    case 237:
    case 241:
    case 312:
        ShoeType = SHOE_GREY;
        break;
    case 87:
    case 142:
    case 190:
    case 200:
        ShoeType = SHOE_BLACK_RED;
        break;
    case 42:
    case 43:
    case 44:
    case 49:
    case 50:
    case 51:
    case 52:
    case 55:
    case 67:
    case 86:
    case 140:
    case 141:
    case 181:
    case 182:
    case 202:
    case 203:
    case 208:
    case 209:
    case 224:
    case 309:
    case 313:
        ShoeType = SHOE_WHITE_RED;
        break;
    case 64:
    case 77:
    case 81:
    case 82:
    case 304:
    case 311:
        ShoeType = SHOE_WHITE_BLUE;
        break;
    case 26:
    case 29:
    case 39:
    case 151:
    case 163:
    case 164:
    case 165:
    case 204:
    case 233:
        ShoeType = SHOE_BRIGHT_RED;
        break;
    case 35:
    case 171:
    case 210:
    case 211:
    case 242:
    case 305:
        ShoeType = SHOE_WHITE_LIGHT_BLUE;
        break;
    case 21:
    case 23:
    case 41:
    case 47:
    case 48:
    case 135:
    case 136:
    case 167:
    case 180:
        ShoeType = SHOE_YELLOW;
        break;
    case 15:
    case 20:
    case 22:
    case 134:
    case 148:
    case 149:
    case 150:
    case 152:
    case 162:
    case 217:
    case 218:
    case 219:
    case 232:
        ShoeType = SHOE_LIGHT_GREEN;
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
    switch (fifaPlayer->internal.skintypecode) {
    case 0:
    case 50:
    case 100:
    case 102:
    case 103:
    case 150:
        player->mAppearance.mFaceVariation = 0;
        break;
    case 1:
    case 104:
        player->mAppearance.mFaceVariation = 1;
        break;
    case 2:
    case 51:
    case 101:
    case 151:
        player->mAppearance.mFaceVariation = 2;
        break;
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

    // 0 - none
    // 1 - chin beard  _
    // 2 - kevin beard \_/
    // 3 - full goatee |_|
    // 4 - full beard \|_|/
    // 5 - tash --
    // 6 - shadow
    // 7 - goatee \/
    // 8 - unshaven
    switch (fifaPlayer->internal.facialhairtypecode) {
    case 0:
        player->mAppearance.mBeardType = 0;
        break;
    case 1:
        player->mAppearance.mBeardType = 1;
        break;
    case 2:
        player->mAppearance.mBeardType = 2;
        break;
    case 3:
        player->mAppearance.mBeardType = 3;
        break;
    case 4:
    case 28:
        player->mAppearance.mBeardType = 4;
        break;
    case 5:
    case 16:
    case 17:
        player->mAppearance.mBeardType = 5;
        break;
    case 6:
        player->mAppearance.mBeardType = 6;
        break;
    case 7:
        player->mAppearance.mBeardType = 7;
        break;
    case 8:
    case 18:
    case 19:
    case 20:
        player->mAppearance.mBeardType = 8;
        break;
    case 9:
        player->mAppearance.mBeardType = 9;
        break;
    case 10:
    case 21:
        player->mAppearance.mBeardType = 10;
        break;
    case 11:
        player->mAppearance.mBeardType = 11;
        break;
    case 12:
        player->mAppearance.mBeardType = 12;
        break;
    case 13:
        player->mAppearance.mBeardType = 13;
        break;
    case 14:
        player->mAppearance.mBeardType = 14;
        break;
    case 15:
        player->mAppearance.mBeardType = 15;
        break;
    }
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
    bool fifaHairSet = false;
    static Map<UInt, UInt> fifaHairsNew = {
        {159,45},{160,35},{161,17},{162,132},{163,27},{164,70},{165,26},{166,71},{167,66},{168,217},{169,18},{170,73},{171,10},{172,17},{173,171},{174,18},{175,35},{176,41},{177,72},{178,25},{179,38},{180,119},{181,15},{182,73},{183,15},{184,18},{185,171},{186,40},{187,25},{188,50},{189,24},{190,35},{191,24},{192,24},{193,217},{194,20},{195,10},{196,21},{197,30},{198,39},{199,30},{200,29},{201,9},{202,10},{204,73},{205,35},{206,6},{207,52},{208,18},{209,52},{211,14},{212,18},{218,15},{219,32},{220,35},{221,19},{222,27},{223,73},{224,142},{225,83},{226,84},{227,6},{228,124},{229,73},{230,35},{231,13},{232,45},{233,38},{234,36},{235,52},{236,36},{237,49},{239,15},{240,12},{241,28},{242,19},{243,19},{244,73},{245,73},{250,6},{257,104},{263,112},{264,162},{265,19},{267,208},{273,124},{274,187},{275,38},{276,96},{277,20},{278,32},{280,83},{283,25},{284,19},{285,27},{289,17},{290,8},{291,13},{292,37},{293,24},{294,28},{295,15},{296,187},{297,7},{298,41},{299,77},{376,88},{390,12},{394,14},{419,83},{431,203},{434,137},{445,40},{446,15},{450,110},{451,10},{460,197},{463,18},{586,150},{597,196},{617,199},{630,20},{631,128},{632,119},{633,82},{634,128},{635,24},{636,41},{637,101},{638,16},{639,40},{640,18},{641,69},{642,28},{643,14},{644,82},{645,48},{646,17},{647,200},{648,54},{649,82},{650,51},{651,44},{652,40},{653,89},{654,41},{655,26},{656,45},{657,39},{659,80},{660,45},{662,60},{664,211},{665,9},{666,217},{667,83},{668,48},{673,63},{677,75},{683,32},{685,18},{688,236},{691,68},{692,65},{693,73},{694,131},{695,13},{902,58}
    };
    if (fifaHairsNew.contains(fifaPlayer->internal.hairtypecode)) {
        auto hairId = fifaHairsNew[fifaPlayer->internal.hairtypecode];
        static unsigned char hairIdFromEditorId[] = { 0,26,120,132,43,25,29,92,46,28,41,16,1,121,82,60,117,123,36,63,40,72,47,114,86,150,37,106,124,65,45,105,38,31,77,88,18,118,133,115,140,61,134,64,135,137,75,155,122,101,113,24,154,148,21,39,141,151,146,129,93,149,17,143,67,70,85,2,119,156,54,57,90,144,152,102,94,42,78,97,147,116,130,145,111,131,127,107,126,104,138,158,153,128,23,112,66,157,30,89,74,136,14,22,53,35,100,13,19,142,58,62,110,55,108,68,20,10,8,69,83,98,15,71,48,7,59,139,44,95,103,73,27,84,125,81,4,34,99,109,52,96,51,12,9,6,91,56,32,80,33,11,76,49,87,3,5,79,159,50,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242 };
        if (hairId > 0 && hairId <= std::size(hairIdFromEditorId)) {
            player->mAppearance.mHairStyle = hairIdFromEditorId[hairId - 1];
            fifaHairSet = true;
        }
    }
    if (!fifaHairSet) {
        static int fifaHairMapping[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,203,213,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581 };
        for (unsigned int i = 0; i < std::size(fifaHairMapping); i++) {
            if (fifaHairMapping[i] == fifaPlayer->internal.hairtypecode) {
                player->mAppearance.mHairStyle = i;
                break;
            }
        }
    }
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

    bool fifaHeadSet = false;
    static Map<UInt, UInt> fifaHeadsNew = {
        {61,71},{68,91},{69,87},{563,38},{565,163},{567,154},{603,67},{604,215},{605,196},{606,144},{607,132},{608,297},{1042,115},{1050,186},{1550,317},{1551,266},{1552,11},{1553,192},{1554,205},{1555,136},{1556,80},{4018,12},{4019,3},{4020,111},{4021,201},{4024,193}
    };
    if (fifaHeadsNew.contains(fifaPlayer->internal.headtypecode)) {
        auto headId = fifaHeadsNew[fifaPlayer->internal.headtypecode];
        if (headId >= 1 && headId <= 449) {
            player->mAppearance.mGenericFace = headId - 1;
            fifaHeadSet = true;
        }
    }
    if (!fifaHeadSet) {
        static int fifaHeadMapping[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1500,1501,1502,1503,1504,1505,1506,1507,1508,1509,1510,1511,1512,1513,1514,1515,1516,1517,1518,1519,1520,1521,1522,1523,1524,1525,1526,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2012,2500,2501,2502,2503,2504,2505,2506,3000,3001,3002,3003,3004,3005,3500,3501,3502,3503,3504,3505,4000,4001,4002,4003,4500,4501,4502,5000,5001,5002,5003,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,1019,1020,1021,1022,1023,1024,1025,1026,1027,1527,1528,2011,2013,2014,2015,2016,2017,2019,2020,2021,2022,2023,2024,2025,2026,2027,2028,2029,2030,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,4525,3507,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,46,47,48,49,50,51,52,53,54,55,56,57,58,59,600,601,602,1028,1029,1030,1031,1033,1035,1036,1037,1038,1039,1529,1530,1531,1532,1533,1534,1535,1536,1537,1538,1539,1540,1541,1542,1543,1545,1546,1547,1548,1549,2031,2519,2520,2521,2522,3506,3508,3509,3510,3512,3513,3514,3515,3516,3517,3518,3519,4004,4005,4006,4007,4008,4009,4010,4011,4012,4013,4014,4015,4016,4017,5500,5501,5502,5503,5504,5505,5506,5507,5508,5509,5510,6000,6001,6002,6003,6004,6005,6006,6007,6008,6009,6010,6011,6012,6013,6014,6015,6016,6017,6018,6019,6020,6021,6022,6023,6024,6025,6026,6027,6028,6029,6500,6501,6502,7000,7001,7002,7003,7004,7005,7006,7007,7008,7009,7010,7011,7012,7013,7014,7015,7016,7017,7018,7019,7020,7021,7022,7023,7024,7025,7026,7500,7501,7502,8000,8001,8002,8500,8501,8502,9000,9001,9002,9500,9501,9502,9503,9504,9505,9506,9507,9508,9509,9510,9511,9512,10000,10001,10002,10003,10004,10005,10006,10007,10008,10009,10010,10011,10012,10013,10014,10015,10016,10017,10018,10019,10020,10021,10022,10023,10024,10025,10026,10027,10500,10501,10502,10503,10504,10505,10506,10507 };
        for (unsigned int i = 0; i < std::size(fifaHeadMapping); i++) {
            if (fifaHeadMapping[i] == fifaPlayer->internal.headtypecode) {
                player->mAppearance.mGenericFace = i;
                break;
            }
        }
    }
    player->mShoeType.SetFromInt(GetPlayerShoesIdFromFifaId(isLastVersion ? fifaPlayer->internal.shoetypecode : 0));
}
