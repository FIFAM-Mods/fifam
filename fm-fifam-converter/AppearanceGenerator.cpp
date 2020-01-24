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
    player->mAppearance.mSkinColor.SetFromInt(GetRandomAppearanceParam(def, ParamSkinColor, player->mAppearance.mSkinColor.ToInt()));
    player->mAppearance.mGenericFace = GetRandomAppearanceParam(def, ParamFace, player->mAppearance.mGenericFace);
    player->mAppearance.mHairStyle = GetRandomAppearanceParam(def, ParamHair, player->mAppearance.mHairStyle);
    player->mAppearance.mFaceVariation.SetFromInt(GetRandomAppearanceParam(def, ParamVariation, player->mAppearance.mFaceVariation.ToInt()));
    player->mAppearance.mBeardColor.SetFromInt(GetRandomAppearanceParam(def, ParamBeardColor, player->mAppearance.mBeardColor.ToInt()));
    player->mAppearance.mEyeColour.SetFromInt(GetRandomAppearanceParam(def, ParamEyeColor, player->mAppearance.mEyeColour.ToInt()));
    player->mAppearance.mBeardType.SetFromInt(GetRandomAppearanceParam(def, ParamBeard, player->mAppearance.mBeardType.ToInt()));
    player->mAppearance.mSideburns = GetRandomAppearanceParam(def, ParamSideburns, player->mAppearance.mSideburns);
    player->mAppearance.mHairColor.SetFromInt(GetRandomAppearanceParam(def, ParamHairColor, player->mAppearance.mHairColor.ToInt()));
}

enum ShoeType {
    SHOE_WHITE = 1,
    SHOE_BLACK = 2,
    SHOE_BLUE = 3,
    SHOE_RED = 4,
    SHOE_YELLOW = 5,
    SHOE_WHITE_BLUE = 6,
    SHOE_WHITE_BLACK = 7,
    SHOE_WHITE_LIGHT_BLUE = 8,
    SHOE_GREEN = 10,
    SHOE_BLACK_RED = 11,
    SHOE_GREY = 12,
    SHOE_WHITE_RED = 13,
    SHOE_LIGHT_YELLOW = 14,
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
    case 20:
    case 21:
    case 35:
    case 48:
    case 63:
    case 64:
    case 95:
    case 107:
    case 108:
    case 125:
    case 134:
    case 140:
    case 141:
    case 147:
    case 159:
    case 160:
    case 161:
    case 162:
    case 189:
    case 193:
    case 214:
    case 243:
    case 253:
        ShoeType = SHOE_WHITE;
        break;
    case 19:
    case 60:
    case 150:
    case 151:
        ShoeType = SHOE_WHITE_BLACK;
        break;
    case 17:
    case 39:
    case 45:
    case 50:
    case 61:
    case 62:
    case 72:
    case 88:
    case 89:
    case 93:
    case 99:
    case 102:
    case 111:
    case 112:
    case 113:
    case 115:
    case 118:
    case 120:
    case 126:
    case 130:
    case 131:
    case 132:
    case 146:
    case 154:
    case 163:
    case 165:
    case 166:
    case 171:
    case 172:
    case 174:
    case 177:
    case 180:
    case 183:
    case 191:
    case 195:
    case 200:
    case 203:
    case 210:
    case 211:
    case 216:
    case 219:
    case 220:
    case 232:
    case 234:
    case 235:
        ShoeType = SHOE_BLACK;
        break;
    case 22:
    case 24:
    case 90:
    case 96:
    case 103:
    case 114:
    case 121:
    case 128:
    case 137:
    case 156:
    case 212:
    case 222:
    case 231:
        ShoeType = SHOE_DARK_BLUE;
        break;
    case 36:
    case 145:
    case 148:
    case 157:
    case 215:
        ShoeType = SHOE_RED;
        break;
    case 34:
    case 46:
    case 47:
    case 58:
    case 97:
    case 100:
    case 116:
    case 123:
    case 124:
    case 196:
    case 204:
    case 209:
    case 217:
    case 218:
    case 225:
    case 227:
    case 248:
        ShoeType = SHOE_ORANGE;
        break;
    case 23:
    case 25:
    case 71:
    case 92:
    case 98:
    case 105:
    case 136:
    case 155:
    case 158:
    case 182:
    case 190:
    case 233:
        ShoeType = SHOE_YELLOW;
        break;
    case 15:
    case 37:
    case 49:
    case 59:
    case 70:
    case 86:
    case 87:
    case 106:
    case 109:
    case 119:
    case 127:
    case 138:
    case 142:
    case 194:
    case 202:
    case 252:
        ShoeType = SHOE_GREEN;
        break;
    case 26:
    case 175:
    case 184:
    case 250:
    case 251:
        ShoeType = SHOE_MAGENTA;
        break;
    case 27:
    case 28:
    case 38:
    case 69:
    case 91:
    case 94:
    case 153:
    case 167:
    case 226:
    case 228:
    case 236:
        ShoeType = SHOE_BLUE;
        break;
    case 33:
    case 54:
    case 57:
    case 73:
    case 122:
    case 143:
    case 144:
    case 164:
    case 176:
    case 178:
    case 185:
    case 186:
    case 199:
    case 206:
    case 208:
    case 221:
    case 223:
    case 224:
    case 237:
    case 240:
    case 241:
    case 249:
        ShoeType = SHOE_GREY;
        break;
    case 51:
    case 110:
    case 135:
    case 139:
    case 149:
    case 152:
    case 181:
    case 187:
    case 188:
    case 229:
        ShoeType = SHOE_BLACK_RED;
        break;
    case 101:
    case 104:
    case 179:
    case 197:
    case 198:
    case 230:
        ShoeType = SHOE_WHITE_RED;
        break;
    case 133:
    case 168:
    case 169:
    case 201:
    case 207:
    case 213:
    case 244:
    case 246:
        ShoeType = SHOE_WHITE_BLUE;
        break;
    case 18:
    case 117:
    case 170:
    case 173:
    case 192:
    case 205:
    case 245:
    case 247:
        ShoeType = SHOE_BRIGHT_RED;
        break;
    default:
        ShoeType = Random::Get(1, 19);
    }
    return ShoeType;
}

void AppearanceGenerator::SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer) {
    bool isLastVersion = fifaPlayer->m_gameId == FIFA_DATABASE_LATEST_GAME_VERSION;
    switch (fifaPlayer->internal.skintonecode) {
    case 1:
    case 2:
    case 3:
        player->mAppearance.mSkinColor = FifamSkinColor::White;
        break;
    case 4:
        player->mAppearance.mSkinColor = FifamSkinColor::Asian;
        break;
    case 5:
        player->mAppearance.mSkinColor = FifamSkinColor::Latin1;
        break;
    case 6:
    case 7:
        player->mAppearance.mSkinColor = FifamSkinColor::Latin2;
        break;
    case 8:
        player->mAppearance.mSkinColor = FifamSkinColor::African1;
        break;
    case 9:
        player->mAppearance.mSkinColor = FifamSkinColor::African2;
        break;
    case 10:
        player->mAppearance.mSkinColor = FifamSkinColor::African3;
        break;
    }
    switch (fifaPlayer->internal.skintypecode) {
    case 0:
        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;
        break;
    case 1:
        player->mAppearance.mFaceVariation = FifamFaceVariation::Freckles;
        break;
    case 2:
        player->mAppearance.mFaceVariation = FifamFaceVariation::Wrinkles;
        break;
    }
    // 0 - light blue 1 - brown 2 - grey-green 3 - green 4 - green-blue 5 -grey 6 - blue
    switch (fifaPlayer->internal.eyecolorcode) {
    case 1:
        player->mAppearance.mEyeColour = FifamEyeColor::Blue;
        break;
    case 2:
        player->mAppearance.mEyeColour = FifamEyeColor::LightBlue;
        break;
    case 3:
    case 4:
    case 9:
        player->mAppearance.mEyeColour = FifamEyeColor::Brown;
        break;
    case 5:
        player->mAppearance.mEyeColour = FifamEyeColor::GrayGreen;
        break;
    case 6:
    case 7:
        player->mAppearance.mEyeColour = FifamEyeColor::Green;
        break;
    case 8:
    case 10:
        player->mAppearance.mEyeColour = FifamEyeColor::Grey;
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
        player->mAppearance.mBeardType = FifamBeardType::None;
        break;
    case 1:
        player->mAppearance.mBeardType = FifamBeardType::ChinBeard;
        break;
    case 2:
    case 11:
        player->mAppearance.mBeardType = FifamBeardType::KevinBeard;
        break;
    case 3:
    case 9:
    case 12:
        player->mAppearance.mBeardType = FifamBeardType::FullGoatee;
        break;
    case 4:
    case 8:
        player->mAppearance.mBeardType = FifamBeardType::Full;
        break;
    case 5:
    case 17:
        player->mAppearance.mBeardType = FifamBeardType::Moustash;
        break;
    case 6:
    case 13: // or none?
    case 14: // or none?
    case 15:
    case 16: // or none?
        player->mAppearance.mBeardType = FifamBeardType::Shadow;
        break;
    case 7:
        player->mAppearance.mBeardType = FifamBeardType::Goatee;
        break;
    }
    // 0 - black, 1 - blonde, 2 - brown, 3 - medium blonde, 4 - red
    switch (fifaPlayer->internal.facialhaircolorcode) {
    case 0:
        player->mAppearance.mBeardColor = FifamBeardColor::Black;
        break;
    case 1:
        player->mAppearance.mBeardColor = FifamBeardColor::Blonde;
        break;
    case 2:
        player->mAppearance.mBeardColor = FifamBeardColor::Brown;
        break;
    case 3:
        player->mAppearance.mBeardColor = FifamBeardColor::MediumBlonde;
        break;
    case 4:
        player->mAppearance.mBeardColor = FifamBeardColor::Red;
        break;
    }
    if (fifaPlayer->internal.hairtypecode <= 96) {
        switch (fifaPlayer->internal.hairtypecode) {
        case 42:
            player->mAppearance.mHairStyle = 78;
            break;
        case 60:
            player->mAppearance.mHairStyle = 43;
            break;
            //case 50:
            //    player->mAppearance.mHairStyle = 78;
            //    break;
        default:
            player->mAppearance.mHairStyle = fifaPlayer->internal.hairtypecode;
        }
    }
    else {
        switch (fifaPlayer->internal.hairtypecode) {
        case 97:
            player->mAppearance.mHairStyle = g14HairEditorIdToReal[23 - 1];
            break;
        case 98:
            player->mAppearance.mHairStyle = 62;
            break;
        case 99:
            player->mAppearance.mHairStyle = 68;
            break;
        case 100:
            player->mAppearance.mHairStyle = 17;
            break;
        case 101:
            player->mAppearance.mHairStyle = 86;
            break;
        case 102:
            player->mAppearance.mHairStyle = 70;
            break;
        case 103:
            player->mAppearance.mHairStyle = 13;
            break;
        case 104:
            player->mAppearance.mHairStyle = 83;
            break;
        case 105:
            player->mAppearance.mHairStyle = 65;
            break;
        case 106:
            player->mAppearance.mHairStyle = 24;
            break;
        case 107:
            player->mAppearance.mHairStyle = 93;
            break;
        case 108:
            player->mAppearance.mHairStyle = 83;
            break;
        case 109:
            player->mAppearance.mHairStyle = 27;
            break;
        case 110:
            player->mAppearance.mHairStyle = 71;
            break;
        case 111:
            player->mAppearance.mHairStyle = 67;
            break;
        case 112:
            player->mAppearance.mHairStyle = 17;
            break;
        case 113:
            player->mAppearance.mHairStyle = 24;
            break;
        case 114:
            player->mAppearance.mHairStyle = 47;
            break;
        case 115:
            player->mAppearance.mHairStyle = 24;
            break;
        case 116:
            player->mAppearance.mHairStyle = 69;
            break;
        case 117:
            player->mAppearance.mHairStyle = 43;
            break;
        case 118:
            player->mAppearance.mHairStyle = 78;
            break;
        case 119:
            player->mAppearance.mHairStyle = 78;
            break;
        case 120:
            player->mAppearance.mHairStyle = 1;
            break;
        case 121:
            player->mAppearance.mHairStyle = 65;
            break;
        case 122:
            player->mAppearance.mHairStyle = 65;
            break;
        case 123:
            player->mAppearance.mHairStyle = 88;
            break;
        case 124:
            player->mAppearance.mHairStyle = 88;
            break;
        case 125:
            player->mAppearance.mHairStyle = 78;
            break;
        case 126:
            player->mAppearance.mHairStyle = 64;
            break;
        case 127:
            player->mAppearance.mHairStyle = 40;
            break;
        case 128:
            player->mAppearance.mHairStyle = 66;
            break;
        case 129:
            player->mAppearance.mHairStyle = 57;
            break;
        case 130:
            player->mAppearance.mHairStyle = 67;
            break;
        case 131:
            player->mAppearance.mHairStyle = 66;
            break;
        case 132:
            player->mAppearance.mHairStyle = 88;
            break;
        case 133:
            player->mAppearance.mHairStyle = 86;
            break;
        case 134:
            player->mAppearance.mHairStyle = 78;
            break;
        case 135:
            player->mAppearance.mHairStyle = 65;
            break;
        case 136:
            player->mAppearance.mHairStyle = 95;
            break;
        case 137:
            player->mAppearance.mHairStyle = 65;
            break;
        case 138:
            player->mAppearance.mHairStyle = 19;
            break;
        case 139:
            player->mAppearance.mHairStyle = 13;
            break;
        case 140:
            player->mAppearance.mHairStyle = 86;
            break;
        case 141:
            player->mAppearance.mHairStyle = 78;
            break;
        case 142:
            player->mAppearance.mHairStyle = 38;
            break;
        case 143:
            player->mAppearance.mHairStyle = 38;
            break;
        case 144:
            player->mAppearance.mHairStyle = 14;
            break;
        case 145:
            player->mAppearance.mHairStyle = 77;
            break;
        case 146:
            player->mAppearance.mHairStyle = 77;
            break;
        case 147:
            player->mAppearance.mHairStyle = 77;
            break;
        case 148:
            player->mAppearance.mHairStyle = 65;
            break;
        case 149:
            player->mAppearance.mHairStyle = 78;
            break;
        case 150:
            player->mAppearance.mHairStyle = 31;
            break;
        case 151:
            player->mAppearance.mHairStyle = 78;
            break;
        case 152:
            player->mAppearance.mHairStyle = 18;
            break;
        case 153:
            player->mAppearance.mHairStyle = 20;
            break;
        case 154:
            player->mAppearance.mHairStyle = 75;
            break;
        case 155:
            player->mAppearance.mHairStyle = 39;
            break;
        case 156:
            player->mAppearance.mHairStyle = 90;
            break;
        case 157:
            player->mAppearance.mHairStyle = 64;
            break;
        case 158:
            player->mAppearance.mHairStyle = 93;
            break;
        case 267:
            player->mAppearance.mHairStyle = g14HairEditorIdToReal[83 - 1];
            break;
        case 283:
        case 284:
        case 298:
            player->mAppearance.mHairStyle = g14HairEditorIdToReal[17 - 1];
            break;
        case 295:
            player->mAppearance.mHairStyle = g14HairEditorIdToReal[44 - 1];
            break;
        }
    }
    // 0 - blonde, 1 - black, 2 - medium blonde, 3 - darkbrown, 4 - platinumblonde, 5 - lightbrown, 6 - brown, 7 - red
    switch (fifaPlayer->internal.haircolorcode) {
    case 0:
        player->mAppearance.mHairColor = FifamHairColor::Blonde;
        break;
    case 1:
        player->mAppearance.mHairColor = FifamHairColor::Black;
        break;
    case 2:
    case 9:
        player->mAppearance.mHairColor = FifamHairColor::MediumBlond;
        break;
    case 3:
        player->mAppearance.mHairColor = FifamHairColor::Darkbrown;
        break;
    case 4:
    case 8:
        player->mAppearance.mHairColor = FifamHairColor::Platinumblonde;
        break;
    case 5:
        player->mAppearance.mHairColor = FifamHairColor::Lightbrown;
        break;
    case 6:
        player->mAppearance.mHairColor = FifamHairColor::Brown;
        break;
    case 7:
    case 12:
    case 13:
        player->mAppearance.mHairColor = FifamHairColor::Red;
        break;
    }

    // 0-25
    // 26-46
    // 47-65
    // 66-94
    // 93-103
    // 105-111
    // 112-117
    // 118-123
    // 124-127
    // 130
    // 131-134

    if (fifaPlayer->internal.headtypecode <= 25)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode;
    else if (fifaPlayer->internal.headtypecode >= 2000 && fifaPlayer->internal.headtypecode <= 2010)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 2000 + 93;
    else if (fifaPlayer->internal.headtypecode >= 4000 && fifaPlayer->internal.headtypecode <= 4003)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 4000 + 124;
    else if (fifaPlayer->internal.headtypecode >= 3500 && fifaPlayer->internal.headtypecode <= 3505)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 3500 + 118;
    else if (fifaPlayer->internal.headtypecode >= 1500 && fifaPlayer->internal.headtypecode <= 1527)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 1500 + 66;
    else if (fifaPlayer->internal.headtypecode >= 2500 && fifaPlayer->internal.headtypecode <= 2506)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 2500 + 105;
    else if (fifaPlayer->internal.headtypecode >= 1000 && fifaPlayer->internal.headtypecode <= 1018)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 1000 + 47;
    else if (fifaPlayer->internal.headtypecode >= 5000 && fifaPlayer->internal.headtypecode <= 5003)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 5000 + 131;
    else if (fifaPlayer->internal.headtypecode == 4525 || fifaPlayer->internal.headtypecode == 4502)
        player->mAppearance.mGenericFace = 130;
    else if (fifaPlayer->internal.headtypecode >= 500 && fifaPlayer->internal.headtypecode <= 520)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 500 + 26;
    else if (fifaPlayer->internal.headtypecode >= 3000 && fifaPlayer->internal.headtypecode <= 3005)
        player->mAppearance.mGenericFace = fifaPlayer->internal.headtypecode - 3000 + 112;
    else if (fifaPlayer->internal.headtypecode == 1528)
        player->mAppearance.mGenericFace = 104;
    else if (fifaPlayer->internal.headtypecode == 4500)
        player->mAppearance.mGenericFace = 128;
    else if (fifaPlayer->internal.headtypecode == 4501)
        player->mAppearance.mGenericFace = 129;
    else if (fifaPlayer->internal.headtypecode == 2023)
        player->mAppearance.mGenericFace = 22;
    else if (fifaPlayer->internal.headtypecode == 2015)
        player->mAppearance.mGenericFace = 111;
    else if (fifaPlayer->internal.headtypecode == 2016)
        player->mAppearance.mGenericFace = 100;

    player->mShoeType.SetFromInt(GetPlayerShoesIdFromFifaId(isLastVersion ? fifaPlayer->internal.shoetypecode : 0));
}
