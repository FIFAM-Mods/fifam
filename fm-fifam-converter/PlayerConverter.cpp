#include "Converter.h"
#include "FifamPlayerLevel.h"
#include "ConverterUtil.h"

Bool Converter::IsIconicPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        735216, // Cristiano Ronaldo
        7458500, // Messi
        19024412, // Neymar
        11133, // Buffon
        142173, // Ibrahimovic
        7458272, // Sergio Ramos
        5124470, // Pique
        85139014, // Mbappe
        8435089, // Benzema
        719601, // Lewandowski
        98028755, // Salah
        8718372, // Neuer
        653054, // Modric
        156772, // Kroos
        85028014, // Pogba
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsIntrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        85081880, // Kante
        653054 // Modric
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsExtrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        19024412, // Neymar
        8832853, // Marcelo
        71000324, // Konoplyanka
        71081391, // Zinchenko
        8085570, // Dzyuba
        189596, // Muller
        234396 // Davies
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsInsecurePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        43001238 // Balotelli
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsFansFavouritePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        735216, // Cristiano Ronaldo
        19024412, // Neymar
        11133, // Buffon
        142173, // Ibrahimovic
        98028755, // Salah
        76002390, // James
        5132312, // Bale
        78000335, // Luis Suarez
        8832853, // Marcelo
        8435089, // Benzema
        7458272, // Sergio Ramos
        824041, // Iniesta
        85028014, // Pogba
        2114068, // David Luiz
        315542, // Dani Alves
        35002219, // Ozil
        156772, // Kroos
        5124470, // Pique
        67086656, // Griezmann
        14044150, // Dybala
        8833628, // Thiago Silva
        198219, // Insigne
        231747, // Mbappe
        653054, // Modric
        29179241, // Haaland
        91013383, // Reus
        19302146 // Vini Junior
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsSensitivePlayer(Int playerId) {
    return false;
}

Bool Converter::IsLazyPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        85140178, // Dembele
        71000324, // Konoplyanka
        37003555, // Arnautovic
        727983, // Quaresma
        35002219, // Ozil
        919707, // Ben Arfa
        37024470, // Depay
        34001033, // Taarabt
        43001238, // Balotelli
        8168166 // Gervinho
    };
    return Utils::Contains(playerIDs, playerId);
}

UChar Converter::GetPlayerLevel(FifamPlayer *player, Bool includeExperience, UInt gameId) {
    return GetPlayerLevel(player, player->mMainPosition, player->mPlayingStyle, includeExperience, gameId);
}

UChar Converter::GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool includeExperience, UInt gameId) {
    if (gameId >= 13)
        return FifamPlayerLevel::GetPlayerLevel13(player, position, style, includeExperience);
    return FifamPlayerLevel::GetPlayerLevel12(player, position, style, includeExperience);
}

Bool Converter::IsPlayerRetiredFromNationalTeam(Int playerId) {
    return
        playerId == 8169332 || // Nainggolan
        playerId == 11133 || // Buffon
        playerId == 8053234 || // Akinfeev
        playerId == 156772 || // Kroos
        playerId == 19004998 || // Marlos
        playerId == 837912 // Pandev
        ;
}

Int Converter::ConvertPlayerAttribute(Int attr, UInt gameId) {
    Int originalAttr = OriginalAttrValue(attr);
    if (originalAttr <= 0)
        originalAttr = 1;
    if (originalAttr > 20)
        originalAttr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        {  5, 15 }, // 1
        { 16, 29 }, // 2
        { 30, 38 }, // 3
        { 39, 44 }, // 4
        { 45, 50 }, // 5
        { 51, 53 }, // 6
        { 54, 57 }, // 7
        { 58, 61 }, // 8
        { 62, 64 }, // 9
        { 65, 67 }, // 10
        { 68, 70 }, // 11
        { 71, 74 }, // 12
        { 75, 78 }, // 13
        { 79, 82 }, // 14
        { 83, 84 }, // 15
        { 85, 86 }, // 16
        { 87, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    static Pair<Int, Int> fmRatingAry_12[20] = {
        {  5, 15 }, // 1
        { 16, 29 }, // 2
        { 30, 39 }, // 3
        { 40, 49 }, // 4
        { 50, 59 }, // 5
        { 60, 64 }, // 6
        { 65, 67 }, // 7
        { 68, 70 }, // 8
        { 71, 72 }, // 9
        { 73, 74 }, // 10
        { 75, 76 }, // 11
        { 77, 78 }, // 12
        { 79, 80 }, // 13
        { 81, 82 }, // 14
        { 83, 84 }, // 15
        { 85, 86 }, // 16
        { 87, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    if (gameId >= 13) {
        if (fmRatingAry[originalAttr - 1].first == fmRatingAry[originalAttr - 1].second)
            return fmRatingAry[originalAttr - 1].first;
        return Random::Get(fmRatingAry[originalAttr - 1].first, fmRatingAry[originalAttr - 1].second);
    }
    else {
        if (fmRatingAry_12[originalAttr - 1].first == fmRatingAry_12[originalAttr - 1].second)
            return fmRatingAry_12[originalAttr - 1].first;
        return Random::Get(fmRatingAry_12[originalAttr - 1].first, fmRatingAry_12[originalAttr - 1].second);
    }
}

Int Converter::LinearConvertPlayerAttribute(Int attr, UInt gameId) {
    Int originalAttr = OriginalAttrValue(attr);
    if (originalAttr <= 0)
        originalAttr = 1;
    if (originalAttr > 20)
        originalAttr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        {  5, 19 }, // 1  => 1    | 1
        { 20, 29 }, // 2  => 1    | 1
        { 30, 34 }, // 3  => 1    | 2
        { 35, 39 }, // 4  => 1    | 2
        { 40, 44 }, // 5  => 2/3  | 3
        { 45, 49 }, // 6  => 2    | 3
        { 50, 54 }, // 7  => 2/3  | 4
        { 55, 59 }, // 8  => 3    | 4
        { 60, 64 }, // 9  => 3/4  | 5
        { 65, 69 }, // 10 => 4    | 5
        { 70, 71 }, // 11 => 4/5  | 6
        { 72, 74 }, // 12 => 5    | 6
        { 75, 76 }, // 13 => 5/6  | 7
        { 77, 79 }, // 14 => 6/7  | 7
        { 80, 81 }, // 15 => 7    | 8
        { 82, 84 }, // 16 => 8    | 8
        { 85, 87 }, // 17 => 8/9  | 9
        { 88, 89 }, // 18 => 9/10 | 9
        { 90, 94 }, // 19 => 10   | 10
        { 95, 99 }  // 20 => 10   | 10
    };
    if (fmRatingAry[originalAttr - 1].first == fmRatingAry[originalAttr - 1].second)
        return fmRatingAry[originalAttr - 1].first;
    return Random::Get(fmRatingAry[originalAttr - 1].first, fmRatingAry[originalAttr - 1].second);
}

FifamPlayer * Converter::CreateAndConvertPlayer(UInt gameId, foom::player * p, FifamClub * club) {
    if (!p->mNation) {
        Error(L"Player without nation\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *playerCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!playerCountry) {
        Error(L"Player without associated country\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamPlayer *player = mFifamDatabase->CreatePlayer(club, mPersonIdCounter++);
    ConvertPersonAttributes(player, p, gameId);
    if (!p->mShirtName.empty()) {
        String jerseyName = FixPersonName(p->mShirtName, gameId);
        if (!jerseyName.empty() && jerseyName.size() <= 19)
            player->mNickname = jerseyName;
    }
    player->SetProperty(L"foom::player", p);
    player->mIsRealPlayer = true;
    player->mIsBasque = p->mIsBasque;

    UInt age = player->GetAge(GetCurrentDate());

    // foot
    player->mLeftFoot = Utils::MapTo(p->mLeftFoot, 1, 100, 0, 4);
    player->mRightFoot = Utils::MapTo(p->mRightFoot, 1, 100, 0, 4);
    if (player->mLeftFoot != 4 && player->mRightFoot != 4)
        player->mRightFoot = 4;

    // appearance
    player->mHeight = Utils::Clamp(p->mHeight, 150, 220);
    if (player->mHeight <= 0) {
        if (playerCountry->mAverageHeight > 10)
            player->mHeight = Utils::Clamp(playerCountry->mAverageHeight - 10 + Random::Get(0, 20), 150, 220);
        else
            player->mHeight = 175 + Random::Get(0, 10);
    }
    player->mWeight = Utils::Clamp(p->mWeight, 50, 150);
    if (player->mWeight == 50) {
        if (playerCountry->mAverageWeight > 10)
            player->mWeight = Utils::Clamp(playerCountry->mAverageWeight - 10 + Random::Get(0, 20), 50, 150);
        else
            player->mWeight = 70 + Random::Get(0, 10);
    }

    UInt randomShoeType = Random::Get(1, 99);
    if (randomShoeType > 66)
        player->mShoeType = FifamShoeType::Blue;
    else if (randomShoeType > 33)
        player->mShoeType = FifamShoeType::White;
    else
        player->mShoeType = FifamShoeType::Black;

    AppearanceGenerator::Type ethnicity = AppearanceGenerator::EasternEuropeDark;

    if (p->mEthnicity == 0) {
        if (p->mSkinTone <= 7)
            ethnicity = AppearanceGenerator::EasternEuropeLight;
        else
            ethnicity = AppearanceGenerator::EasternEuropeDark;
    }
    else if (p->mEthnicity == 1) {
        if (p->mSkinTone < 0 || p->mSkinTone >= 8)
            ethnicity = AppearanceGenerator::WesternEuropeDark;
        else
            ethnicity = AppearanceGenerator::WesternEuropeLight;
    }
    else if (p->mEthnicity == 2) {
        if (p->mSkinTone <= 10)
            ethnicity = AppearanceGenerator::Arabic2;
        else
            ethnicity = AppearanceGenerator::Arabic1;
    }
    else if (p->mEthnicity == 3) {
        if (p->mSkinTone < 0 || p->mSkinTone >= 16)
            ethnicity = AppearanceGenerator::African1;
        else
            ethnicity = AppearanceGenerator::African2;
    }
    else if (p->mEthnicity == 4 || p->mEthnicity == 10)
        ethnicity = AppearanceGenerator::Asian;
    else if (p->mEthnicity == 5)
        ethnicity = AppearanceGenerator::Indian;
    else if (p->mEthnicity == 6 || p->mEthnicity == 8) {
        if (p->mSkinTone <= 7)
            ethnicity = AppearanceGenerator::WesternEuropeLight;
        else
            ethnicity = AppearanceGenerator::WesternEuropeDark;
    }
    else if (p->mEthnicity == 7 || p->mEthnicity == 9)
        ethnicity = AppearanceGenerator::Latin;

    // first - generate random appearance for ethnicity
    appearanceGenerator.Generate(player, ethnicity);

    static unsigned char hairIdFromEditorId[] = {
        0,26,120,132,43,25,29,92,46,28,41,16,1,121,82,60,117,123,36,63,40,72,47,114,86,150,37,106,124,65,45,105,38,31,77,88,18,118,133,115,140,61,134,64,135,137,75,155,122,101,113,24,154,148,21,39,141,151,146,129,93,149,17,143,67,70,85,2,119,156,54,57,90,144,152,102,94,42,78,97,147,116,130,145,111,131,127,107,126,104,138,158,153,128,23,112,66,157,30,89,74,136,14,22,53,35,100,13,19,142,58,62,110,55,108,68,20,10,8,69,83,98,15,71,48,7,59,139,44,95,103,73,27,84,125,81,4,34,99,109,52,96,51,12,9,6,91,56,32,80,33,11,76,49,87,3,5,79,159,50,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242
    };

    // fix hair
    if (player->mAppearance.mHairStyle == 0)
        player->mAppearance.mHairStyle = hairIdFromEditorId[5 - 1];
    else if (player->mAppearance.mHairStyle == hairIdFromEditorId[2 - 1])
        player->mAppearance.mHairStyle = hairIdFromEditorId[6 - 1];
    else if (player->mAppearance.mHairStyle == hairIdFromEditorId[3 - 1])
        player->mAppearance.mHairStyle = hairIdFromEditorId[7 - 1];
    else if (player->mAppearance.mHairStyle == hairIdFromEditorId[4 - 1])
        player->mAppearance.mHairStyle = hairIdFromEditorId[9 - 1];
    else if (player->mAppearance.mHairStyle == hairIdFromEditorId[8 - 1])
        player->mAppearance.mHairStyle = hairIdFromEditorId[10 - 1];
    else if (player->mAppearance.mHairStyle == hairIdFromEditorId[11 - 1])
        player->mAppearance.mHairStyle = hairIdFromEditorId[12 - 1];

    // fix hair color
    if (player->mAppearance.mHairColor == 8)
        player->mAppearance.mHairColor = FifamHairColor::Black;

    // fix skin type
    if (age >= 45)
        player->mAppearance.mFaceVariation = 3;
    else {
        if (age >= 35)
            player->mAppearance.mFaceVariation = FifamFaceVariation::Wrinkles;
        else if (age >= 26) {
            if (player->mAppearance.mFaceVariation != FifamFaceVariation::Freckles) {
                if (age >= 32) {
                    if (Random::Get(1, 100) > 75)
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;
                    else
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Wrinkles;
                }
                if (age >= 29) {
                    if (Random::Get(1, 100) > 40)
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;
                    else
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Wrinkles;
                }
                else {
                    if (Random::Get(1, 100) > 20)
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;
                    else
                        player->mAppearance.mFaceVariation = FifamFaceVariation::Wrinkles;
                }
            }
        }
        else {
            if (player->mAppearance.mFaceVariation == FifamFaceVariation::Wrinkles) {
                if (Random::Get(1, 100) > 5)
                    player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;
                else
                    player->mAppearance.mFaceVariation = FifamFaceVariation::Freckles;
            }
        }
    }

    // second - apply skin color and hair color (if available)
    bool skinColorSet = false;
    if (p->mSkinTone >= 0 && p->mSkinTone <= 19) {
        switch (p->mSkinTone) {
        case 0:
        case 1:
            player->mAppearance.mSkinColor = 0;
            break;
        case 2:
        case 3:
            player->mAppearance.mSkinColor = 1;
            break;
        case 4:
        case 5:
            player->mAppearance.mSkinColor = 2;
            break;
        case 6:
        case 7:
            player->mAppearance.mSkinColor = 3;
            break;
        case 8:
        case 9:
            player->mAppearance.mSkinColor = 4;
            break;
        case 10:
        case 11:
            player->mAppearance.mSkinColor = 5;
            break;
        case 12:
        case 13:
            player->mAppearance.mSkinColor = 6;
            break;
        case 14:
        case 15:
            player->mAppearance.mSkinColor = 7;
            break;
        case 16:
        case 17:
            player->mAppearance.mSkinColor = 8;
            break;
        case 18:
        case 19:
            player->mAppearance.mSkinColor = 9;
            break;
        }
        skinColorSet = true;
    }

    bool hairSet = false;
    if (p->mHairLength == 4) { // Bald
        player->mAppearance.mHairStyle = hairIdFromEditorId[1 - 1];
        hairSet = true;
    }
    else if (p->mHairLength == 0) { // Fuzz
        player->mAppearance.mHairStyle = hairIdFromEditorId[6 - 1];
        hairSet = true;
    }

    bool hairColorSet = false;
    if (p->mHairColour >= 1 && p->mHairColour <= 6) {
        if (p->mHairColour == 1 || p->mHairColour == 7 || p->mHairColour == 8)
            player->mAppearance.mHairColor = FifamHairColor::Blonde;
        else if (p->mHairColour == 2 || p->mHairColour == 9 || p->mHairColour == 10)
            player->mAppearance.mHairColor = FifamHairColor::Lightbrown;
        else if (p->mHairColour == 3 || p->mHairColour == 11 || p->mHairColour == 12)
            player->mAppearance.mHairColor = FifamHairColor::Darkbrown;
        else if (p->mHairColour == 4 || p->mHairColour == 13 || p->mHairColour == 14)
            player->mAppearance.mHairColor = FifamHairColor::Red;
        else if (p->mHairColour == 5 || p->mHairColour == 15 || p->mHairColour == 16)
            player->mAppearance.mHairColor = FifamHairColor::Black;
        else if (p->mHairColour == 6 || p->mHairColour == 17 || p->mHairColour == 18)
            player->mAppearance.mHairColor = 8;
        hairColorSet = true;
    }

    // third - convert appearance from FIFA (if available)
    if (p->mConverterData.mFifaPlayerId > 0) {
        FifaPlayer *fifaPlayer = mFifaDatabase->GetPlayer(p->mConverterData.mFifaPlayerId);
        if (fifaPlayer)
            appearanceGenerator.SetFromFifaPlayer(player, fifaPlayer);
        if (mFaceIDs.contains(p->mConverterData.mFifaPlayerId))
            player->mSpecialFace = p->mConverterData.mFifaPlayerId;
        player->mComment = L"FIFAID:" + Utils::Format(L"%d", p->mConverterData.mFifaPlayerId);
    }

    // fourth - apply custom appearance (if available)
    bool hasCustomFace = false;
    bool hasCustomHair = false;
    if (p->mConverterData.mEditorFace >= 1 && p->mConverterData.mEditorFace <= 449) {
        player->mAppearance.mGenericFace = p->mConverterData.mEditorFace - 1;
        hasCustomFace = true;
    }
    if (p->mConverterData.mEditorHair >= 1) {
        if (p->mConverterData.mEditorHair > 1000) {
            UChar realHairEditorId = p->mConverterData.mEditorHair - 1000;
            if (realHairEditorId >= 1 && realHairEditorId <= std::size(hairIdFromEditorId)) {
                player->mAppearance.mHairStyle = hairIdFromEditorId[realHairEditorId - 1];
                hasCustomHair = true;
            }
        }
        else {
            if ((UChar)p->mConverterData.mEditorHair <= std::size(g14HairEditorIdToReal)) {
                player->mAppearance.mHairStyle = g14HairEditorIdToReal[p->mConverterData.mEditorHair - 1];
                hasCustomHair = true;
            }
        }
    }
    if (p->mConverterData.mEditorBeard >= 0 && p->mConverterData.mEditorBeard <= 15)
        player->mAppearance.mBeardType = p->mConverterData.mEditorBeard;
    else if (hasCustomFace && hasCustomHair) {
        player->mAppearance.mBeardType = FifamBeardType::None;
        player->mAppearance.mBeardColor = FifamBeardColor::Black;
    }
    if (p->mConverterData.mEditorEye >= 0 && p->mConverterData.mEditorEye <= 9)
        player->mAppearance.mEyeColour = p->mConverterData.mEditorEye;
    else if (hasCustomFace && hasCustomHair)
        player->mAppearance.mEyeColour = FifamEyeColor::Brown;
    if (p->mConverterData.mEditorSkin >= 1 && p->mConverterData.mEditorSkin <= 10)
        player->mAppearance.mSkinColor = p->mConverterData.mEditorSkin - 1;
    if (hasCustomFace && hasCustomHair)
        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;

    // potential
    player->mPotential = GetPlayerLevelFromCA(p->mPotentialAbility);

    // talent
    if (gameId >= 9) {
        if (p->mOriginalPA == -10) // 170-200
            player->mTalent = 8; // 5 stars
        else if (p->mOriginalPA == -95) // 160-190
            player->mTalent = 8; // 4,5 stars
        else if (p->mOriginalPA == -9) { // 150-180
            player->mTalent = 7; // 4 stars
            if (age <= 14) {
                if (p->mOriginalCA >= 95)
                    player->mTalent = 8; // 4,5 stars
            }
            if (age == 15) {
                if (p->mOriginalCA >= 100)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 16) {
                if (p->mOriginalCA >= 115)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 17) {
                if (p->mOriginalCA >= 125)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 18) {
                if (p->mOriginalCA >= 135)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 19) {
                if (p->mOriginalCA >= 140)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 20) {
                if (p->mOriginalCA >= 150)
                    player->mTalent = 8; // 4,5 stars
            }
            else if (age == 21) {
                if (p->mOriginalCA >= 160)
                    player->mTalent = 8; // 4,5 stars
            }
        }
        else if (p->mOriginalPA == -85) { // 140-170
            player->mTalent = 6; // 3,5 stars
            if (age <= 14) {
                if (p->mOriginalCA >= 90)
                    player->mTalent = 7;
            }
            if (age == 15) {
                if (p->mOriginalCA >= 95)
                    player->mTalent = 7;
            }
            else if (age == 16) {
                if (p->mOriginalCA >= 100)
                    player->mTalent = 7;
            }
            else if (age == 17) {
                if (p->mOriginalCA >= 110)
                    player->mTalent = 7;
            }
            else if (age == 18) {
                if (p->mOriginalCA >= 115)
                    player->mTalent = 7;
            }
            else if (age == 19) {
                if (p->mOriginalCA >= 130)
                    player->mTalent = 7;
            }
            else if (age == 20) {
                if (p->mOriginalCA >= 135)
                    player->mTalent = 7;
            }
            else if (age == 21) {
                if (p->mOriginalCA >= 140)
                    player->mTalent = 7;
            }
        }
        else if (p->mOriginalPA == -8) { // 130-160
            player->mTalent = 5; // 3 stars
            if (age <= 14) {
                if (p->mOriginalCA >= 75)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 60)
                    player->mTalent = 6;
            }
            if (age == 15) {
                if (p->mOriginalCA >= 95)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 75)
                    player->mTalent = 6;
            } else if (age == 16) {
                if (p->mOriginalCA >= 105)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 85)
                    player->mTalent = 6;
            } else if (age == 17) {
                if (p->mOriginalCA >= 120)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 95)
                    player->mTalent = 6;
            } else if (age == 18) {
                if (p->mOriginalCA >= 130)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 100)
                    player->mTalent = 6;
            } else if (age == 19) {
                if (p->mOriginalCA >= 135)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 105)
                    player->mTalent = 6;
            } else if (age == 20) {
                if (p->mOriginalCA >= 140)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 115)
                    player->mTalent = 6;
            } else if (age == 21) {
                if (p->mOriginalCA >= 145)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 120)
                    player->mTalent = 6;
            } else if (age == 22) {
                if (p->mOriginalCA >= 150)
                    player->mTalent = 7;
                else if (p->mOriginalCA >= 125)
                    player->mTalent = 6;
            }
        }
        else if (p->mOriginalPA == -75) { // 120-150
            player->mTalent = 5; // 3 stars
            if (age <= 14) {
                if (p->mOriginalCA >= 65)
                    player->mTalent = 6;
            }
            if (age == 15) {
                if (p->mOriginalCA >= 80)
                    player->mTalent = 6;
            } else if (age == 16) {
                if (p->mOriginalCA >= 95)
                    player->mTalent = 6;
            } else if (age == 17) {
                if (p->mOriginalCA >= 100)
                    player->mTalent = 6;
            } else if (age == 18) {
                if (p->mOriginalCA >= 110)
                    player->mTalent = 6;
            } else if (age == 19) {
                if (p->mOriginalCA >= 115)
                    player->mTalent = 6;
            } else if (age == 20) {
                if (p->mOriginalCA >= 120)
                    player->mTalent = 6;
            } else if (age == 21) {
                if (p->mOriginalCA >= 125)
                    player->mTalent = 6;
            } else if (age == 22) {
                if (p->mOriginalCA >= 130)
                    player->mTalent = 6;
            }
        }
        else if (p->mOriginalPA == -7) { // 110-140
            player->mTalent = 5;
        }
        else {
            UInt maxTalent = 9;
            UChar *potantialAbilityRanges = nullptr;     //  1  1.5  2  2.5    3  3.5    4  4.5    5
            static UChar potentialAbilityRanges1to10[9] = { 35, 60, 80, 100, 123, 137, 157, 172, 191 }; // 190 
            potantialAbilityRanges = potentialAbilityRanges1to10;
            player->mTalent = 0;
            for (UInt i = 0; i < maxTalent; i++) {
                if (p->mPotentialAbility >= potentialAbilityRanges1to10[maxTalent - 1 - i]) {
                    player->mTalent = maxTalent - i;
                    break;
                }
            }
        }
        //if (gameId >= 13) { // 6-star talents
        //    if (p->mID == 7458500 || p->mID == 735216)
        //        player->mTalent = 10;
        //}
    }
    else {
        if (player->mPotential >= 86) // Gigantic
            player->mTalent = 8;
        if (player->mPotential >= 76) // Outstanding
            player->mTalent = 6;
        else if (player->mPotential >= 66) // Very Big
            player->mTalent = 4;
        else if (player->mPotential >= 56) // Big
            player->mTalent = 2;
        else // Normal
            player->mTalent = 0;
    }

    Vector<Pair<Int, FifamPlayerPosition>> playerPositions = {
        { p->mGoalkeeper, FifamPlayerPosition::GK },
        { p->mDefenderLeft, FifamPlayerPosition::LB },
        { p->mDefenderCentral, FifamPlayerPosition::CB },
        { p->mDefenderRight, FifamPlayerPosition::RB },
        { p->mDefensiveMidfielder, FifamPlayerPosition::DM },
        { p->mWingBackLeft, FifamPlayerPosition::LWB },
        { p->mWingBackRight, FifamPlayerPosition::RWB },
        { p->mMidfielderLeft, FifamPlayerPosition::LM },
        { p->mMidfielderCentral, FifamPlayerPosition::CM },
        { p->mMidfielderRight, FifamPlayerPosition::RM },
        { p->mAttackingMidfielderLeft, FifamPlayerPosition::LW },
        { p->mAttackingMidfielderCentral, FifamPlayerPosition::AM },
        { p->mAttackingMidfielderRight, FifamPlayerPosition::RW },
        { (p->mAttackingMidfielderCentral + p->mStriker) / 2, FifamPlayerPosition::CF },
        { p->mStriker, FifamPlayerPosition::ST }
    };
    std::sort(playerPositions.begin(), playerPositions.end(), [](Pair<Int, FifamPlayerPosition> const &a, Pair<Int, FifamPlayerPosition> const &b) {
        return a.first >= b.first;
    });
    // position prepare
    if (playerPositions[0].first != 20)
        Error(L"Player has no preferred position\nPlayer: %s\nBest pos: %s (%d)", p->mFullName.c_str(), playerPositions[0].second.ToCStr(), playerPositions[0].first);
    Bool isGoalkeeper = playerPositions[0].second == FifamPlayerPosition::GK;

    // attributes
    struct ConvertedAttributes {
        Int mAggression = 0;
        Int mAnticipation = 0;
        Int mBravery = 0;
        Int mComposure = 0;
        Int mConcentration = 0;
        Int mConsistency = 0;
        Int mVision = 0;
        Int mDecisions = 0;
        Int mDetermination = 0;
        Int mDirtiness = 0;
        Int mFlair = 0;
        Int mImportantMatches = 0;
        Int mLeadership = 0;
        Int mMovement = 0;
        Int mPositioning = 0;
        Int mTeamWork = 0;
        Int mWorkRate = 0;
        Int mAcceleration = 0;
        Int mAgility = 0;
        Int mBalance = 0;
        Int mInjuryProneness = 0;
        Int mJumpingReach = 0;
        Int mNaturalFitness = 0;
        Int mPace = 0;
        Int mStamina = 0;
        Int mStrength = 0;
        Int mCorners = 0;
        Int mCrossing = 0;
        Int mDribbling = 0;
        Int mFinishing = 0;
        Int mFirstTouch = 0;
        Int mFreeKicks = 0;
        Int mHeading = 0;
        Int mLongShots = 0;
        Int mLongThrows = 0;
        Int mMarking = 0;
        Int mPassing = 0;
        Int mPenaltyTaking = 0;
        Int mTackling = 0;
        Int mTechnique = 0;
        Int mVersatility = 0;
        Int mAerialAbility = 0;
        Int mCommandOfArea = 0;
        Int mCommunication = 0;
        Int mEccentricity = 0;
        Int mHandling = 0;
        Int mKicking = 0;
        Int mOneOnOnes = 0;
        Int mReflexes = 0;
        Int mRushingOut = 0;
        Int mTendencyToPunch = 0;
        Int mThrowing = 0;
    } attr;

    /*
    UInt playerLevel = 1;
    static Int playerLevelAry[20] = {
          0, //  1 -1
         50, //  2
         70, //  3 -2
         90, //  4
        100, //  5 -3
        105, //  6
        110, //  7 -4
        115, //  8
        125, //  9 -5
        130, // 10
        135, // 11 -6
        140, // 12
        145, // 13 -7
        150, // 14
        155, // 15 -8
        160, // 16
        165, // 17 -9
        170, // 18
        188, // 19 -10
        190, // 20
    };
    for (Int i = 19; i >= 0; i--) {
        if (p->mCurrentAbility >= playerLevelAry[i]) {
            playerLevel = i + 2;
            break;
        }
    }
    playerLevel /= 2;
    */

    attr.mAggression = ConvertPlayerAttribute(p->mAggression, gameId);
    attr.mAnticipation = ConvertPlayerAttribute(p->mAnticipation, gameId);
    attr.mBravery = ConvertPlayerAttribute(p->mBravery, gameId);
    attr.mComposure = ConvertPlayerAttribute(p->mComposure, gameId);
    attr.mConcentration = ConvertPlayerAttribute(p->mConcentration, gameId);
    attr.mConsistency = ConvertPlayerAttribute(p->mConsistency, gameId);
    attr.mVision = ConvertPlayerAttribute(p->mVision, gameId);
    attr.mDecisions = ConvertPlayerAttribute(p->mDecisions, gameId);
    attr.mDetermination = ConvertPlayerAttribute(p->mDetermination, gameId);
    attr.mDirtiness = ConvertPlayerAttribute(p->mDirtiness, gameId);
    attr.mFlair = ConvertPlayerAttribute(p->mFlair, gameId);
    attr.mImportantMatches = ConvertPlayerAttribute(p->mImportantMatches, gameId);
    attr.mLeadership = LinearConvertPlayerAttribute(p->mLeadership, gameId);
    attr.mMovement = ConvertPlayerAttribute(p->mMovement, gameId);
    attr.mPositioning = ConvertPlayerAttribute(p->mPositioning, gameId);
    attr.mTeamWork = ConvertPlayerAttribute(p->mTeamWork, gameId);
    attr.mWorkRate = ConvertPlayerAttribute(p->mWorkRate, gameId);
    attr.mAcceleration = ConvertPlayerAttribute(p->mAcceleration, gameId);
    attr.mAgility = ConvertPlayerAttribute(p->mAgility, gameId);
    attr.mBalance = ConvertPlayerAttribute(p->mBalance, gameId);
    attr.mInjuryProneness = ConvertPlayerAttribute(p->mInjuryProneness, gameId);
    attr.mJumpingReach = ConvertPlayerAttribute(p->mJumpingReach, gameId);
    attr.mNaturalFitness = ConvertPlayerAttribute(p->mNaturalFitness, gameId);
    attr.mPace = ConvertPlayerAttribute(p->mPace, gameId);
    attr.mStamina = ConvertPlayerAttribute(p->mStamina, gameId);
    attr.mStrength = ConvertPlayerAttribute(p->mStrength, gameId);
    attr.mCorners = ConvertPlayerAttribute(p->mCorners, gameId);
    attr.mCrossing = ConvertPlayerAttribute(p->mCrossing, gameId);
    attr.mDribbling = ConvertPlayerAttribute(p->mDribbling, gameId);
    attr.mFinishing = ConvertPlayerAttribute(p->mFinishing, gameId);
    attr.mFirstTouch = ConvertPlayerAttribute(p->mFirstTouch, gameId);
    attr.mFreeKicks = ConvertPlayerAttribute(p->mFreeKicks, gameId);
    attr.mHeading = ConvertPlayerAttribute(p->mHeading, gameId);
    attr.mLongShots = ConvertPlayerAttribute(p->mLongShots, gameId);
    attr.mLongThrows = ConvertPlayerAttribute(p->mLongThrows, gameId);
    attr.mMarking = ConvertPlayerAttribute(p->mMarking, gameId);
    attr.mPassing = ConvertPlayerAttribute(p->mPassing, gameId);
    attr.mPenaltyTaking = ConvertPlayerAttribute(p->mPenaltyTaking, gameId);
    attr.mTackling = ConvertPlayerAttribute(p->mTackling, gameId);
    attr.mTechnique = ConvertPlayerAttribute(p->mTechnique, gameId);
    attr.mVersatility = ConvertPlayerAttribute(p->mVersatility, gameId);
    attr.mAerialAbility = ConvertPlayerAttribute(p->mAerialAbility, gameId);
    attr.mCommandOfArea = ConvertPlayerAttribute(p->mCommandOfArea, gameId);
    attr.mCommunication = ConvertPlayerAttribute(p->mCommunication, gameId);
    attr.mEccentricity = ConvertPlayerAttribute(p->mEccentricity, gameId);
    attr.mHandling = ConvertPlayerAttribute(p->mHandling, gameId);
    attr.mKicking = ConvertPlayerAttribute(p->mKicking, gameId);
    attr.mOneOnOnes = ConvertPlayerAttribute(p->mOneOnOnes, gameId);
    attr.mReflexes = ConvertPlayerAttribute(p->mReflexes, gameId);
    attr.mRushingOut = ConvertPlayerAttribute(p->mRushingOut, gameId);
    attr.mTendencyToPunch = ConvertPlayerAttribute(p->mTendencyToPunch, gameId);
    attr.mThrowing = ConvertPlayerAttribute(p->mThrowing, gameId);

    //attributes: general
    player->mAttributes.BallControl = attr.mTechnique;
    player->mAttributes.Volleys = (UChar)ceilf((attr.mTechnique + attr.mFinishing + attr.mLongShots) / 3.0f);
    player->mAttributes.Dribbling = attr.mDribbling;
    player->mAttributes.Finishing = attr.mFinishing;
    Int shotPowerBaseAttr = attr.mLongShots;
    if (attr.mFinishing > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mFinishing;
    if (attr.mKicking > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mKicking;
    player->mAttributes.ShotPower = Utils::Clamp((UChar)ceilf((shotPowerBaseAttr * 3 + attr.mStrength) / 4.0f) +
        (p->mShootsWithPower ? 10 : 0), 1, 99);
    player->mAttributes.LongShots = attr.mLongShots;
    player->mAttributes.Crossing = attr.mCrossing;
    player->mAttributes.Passing = attr.mPassing;
    player->mAttributes.LongPassing = (UChar)ceilf((attr.mPassing + attr.mVision) / 2.0f);
    player->mAttributes.Header = attr.mHeading;
    player->mAttributes.TacklingGeneral = attr.mTackling;
    player->mAttributes.TackleStanding = (UChar)ceilf((attr.mTackling + attr.mMarking) / 2.0f);
    player->mAttributes.TackleSliding = (UChar)ceilf((attr.mTackling + attr.mDecisions) / 2.0f);
    if (p->mDivesIntoTackles)
        player->mAttributes.TackleSliding += 5;
    else if (p->mDoesNotDiveIntoTackles)
        player->mAttributes.TackleSliding -= 10;
    player->mAttributes.TackleSliding = Utils::Clamp(player->mAttributes.TackleSliding, 1, 99);
    player->mAttributes.ManMarking = attr.mMarking;
    player->mAttributes.Technique = attr.mTechnique;
    player->mAttributes.Creativity = (UChar)ceilf((attr.mFlair + attr.mVision) / 2.0f);
    player->mAttributes.Flair = attr.mFlair;
    player->mAttributes.Touch = attr.mFirstTouch;
    // attributes: physical
    player->mAttributes.Balance = attr.mBalance;
    player->mAttributes.Acceleration = attr.mAcceleration;
    player->mAttributes.Pace = attr.mPace;
    player->mAttributes.Agility = attr.mAgility;
    player->mAttributes.Jumping = attr.mJumpingReach;
    player->mAttributes.Strength = attr.mStrength;
    player->mAttributes.Stamina = attr.mStamina;
    player->mAttributes.WorkRate = attr.mWorkRate;
    player->mAttributes.ForwardRuns = (UChar)ceilf((attr.mMovement + (UChar)ceilf((attr.mPace + attr.mAcceleration) / 2.0f)) / 2.0f);
    // attributes: mental
    player->mAttributes.PosOffensive = attr.mMovement;
    player->mAttributes.PosDefensive = attr.mPositioning;
    player->mAttributes.Vision = attr.mVision;
    if (isGoalkeeper)
        player->mAttributes.Reactions = (UChar)ceilf((attr.mAnticipation + attr.mReflexes) / 2.0f);
    else
        player->mAttributes.Reactions = (UChar)ceilf((attr.mAnticipation + attr.mFirstTouch) / 2.0f);
    player->mAttributes.TacticAwareness = attr.mTeamWork;
    player->mAttributes.Aggression = attr.mAggression;
    player->mAttributes.Composure = attr.mComposure;
    player->mAttributes.Consistency = attr.mConsistency;
    player->mAttributes.Leadership = attr.mLeadership;
    player->mAttributes.Anticipation = attr.mAnticipation;
    player->mAttributes.Concentration = attr.mConcentration;
    player->mAttributes.Decision = attr.mDecisions;
    player->mAttributes.Determination = attr.mDetermination;
    player->mAttributes.TeamWork = attr.mTeamWork;
    player->mAttributes.Intimidation = attr.mAggression;
    // atributes : set pieces
    player->mAttributes.FreeKicks = attr.mFreeKicks;
    player->mAttributes.Corners = attr.mCorners;
    player->mAttributes.PenaltyShot = attr.mPenaltyTaking;
    // attributes: goalkeeping
    Int gkJumping = attr.mJumpingReach;
    if (attr.mAerialAbility > gkJumping)
        gkJumping = p->mAerialAbility;
    if (isGoalkeeper)
        player->mAttributes.Diving = (UChar)ceilf((attr.mReflexes + gkJumping) / 2.0f);
    else
        player->mAttributes.Diving = attr.mAerialAbility;
    player->mAttributes.GkCrosses = (UChar)ceilf((attr.mPassing + attr.mKicking) / 2.0f);
    player->mAttributes.Handling = attr.mHandling;
    player->mAttributes.Kicking = attr.mKicking;
    player->mAttributes.Positioning = (UChar)ceilf((attr.mRushingOut + attr.mCommandOfArea) / 2.0f);
    player->mAttributes.OneOnOne = attr.mOneOnOnes;
    player->mAttributes.Reflexes = attr.mReflexes;
    player->mAttributes.Punching = attr.mTendencyToPunch;
    player->mAttributes.ShotStopping = (UChar)ceilf((attr.mReflexes + attr.mOneOnOnes) / 2.0f);
    player->mAttributes.Throwing = attr.mThrowing;

    // position
    UInt numBestPos = 1;
    for (UInt i = 1; i < std::size(playerPositions); i++) {
        if (playerPositions[i].first == 20)
            numBestPos += 1;
        else
            break;
    }
    if (numBestPos > 1) {
        UInt bestPosIndex = 0;
        UChar bestLevel = 0;
        for (UInt i = 0; i < numBestPos; i++) {
            player->mMainPosition = playerPositions[i].second;
            player->mPositionBias[playerPositions[i].second.ToInt()] = 100;
            UChar currentLevel = GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId);
            if (currentLevel > bestLevel) {
                bestLevel = currentLevel;
                bestPosIndex = i;
            }
        }
        player->mMainPosition = playerPositions[bestPosIndex].second;
    }
    else
        player->mMainPosition = playerPositions[0].second;
    player->mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(player->mMainPosition, gameId);
    player->mPositionBias[player->mMainPosition.ToInt()] = 100;
    for (UInt i = 0; i < playerPositions.size(); i++) {
        Int value = playerPositions[i].first;
        Int bias = 40;
        if (gameId >= 13) {
            if (value == 20)
                bias = 100;
            else if (value == 19)
                bias = 99;
            else if (value == 18)
                bias = 98;
            else if (value == 17)
                bias = Random::Get(96, 97);
            else if (value == 16)
                bias = Random::Get(94, 95);
            else if (value == 15)
                bias = Random::Get(92, 93);
            else if (value == 14)
                bias = Random::Get(90, 91);
            else if (value == 13)
                bias = Random::Get(86, 89);
            else if (value == 12)
                bias = Random::Get(82, 85);
            else if (value == 11)
                bias = Random::Get(78, 81);
            else if (value == 10)
                bias = Random::Get(74, 77);
            else if (value == 9)
                bias = Random::Get(70, 73);
            else if (value == 8)
                bias = Random::Get(66, 69);
            else if (value == 7)
                bias = Random::Get(62, 65);
            else if (value == 6)
                bias = Random::Get(58, 61);
            else if (value == 5)
                bias = Random::Get(54, 57);
            else if (value == 4)
                bias = Random::Get(50, 53);
            else if (value == 3)
                bias = Random::Get(46, 49);
            else if (value == 2)
                bias = Random::Get(41, 45);
            else if (value == 1)
                bias = 40;
        }
        else {
            bias = 20;
            if (value == 20)
                bias = 100;
            else if (value == 19)
                bias = 99;
            else if (value == 18)
                bias = 98;
            else if (value == 17)
                bias = Random::Get(96, 97);
            else if (value == 16)
                bias = Random::Get(91, 95);
            else if (value == 15)
                bias = Random::Get(86, 90);
            else if (value == 14)
                bias = Random::Get(81, 85);
            else if (value == 13)
                bias = Random::Get(76, 80);
            else if (value == 12)
                bias = Random::Get(71, 75);
            else if (value == 11)
                bias = Random::Get(66, 70);
            else if (value == 10)
                bias = Random::Get(61, 65);
            else if (value == 9)
                bias = Random::Get(56, 60);
            else if (value == 8)
                bias = Random::Get(51, 55);
            else if (value == 7)
                bias = Random::Get(46, 50);
            else if (value == 6)
                bias = Random::Get(41, 45);
            else if (value == 5)
                bias = Random::Get(36, 40);
            else if (value == 4)
                bias = Random::Get(31, 35);
            else if (value == 3)
                bias = Random::Get(26, 30);
            else if (value == 2)
                bias = Random::Get(21, 25);
            else if (value == 1)
                bias = 20;
        }
        if (player->mPositionBias[playerPositions[i].second.ToInt()] < bias)
            player->mPositionBias[playerPositions[i].second.ToInt()] = (Float)bias;
    }

    Int cfPosFirst = p->mStriker;
    Int cfPosSecond = p->mAttackingMidfielderCentral;
    if (cfPosSecond > cfPosFirst)
        std::swap(cfPosFirst, cfPosSecond);
    if (cfPosFirst == 20) {
        if (cfPosSecond >= 19)
            player->mPositionBias[FifamPlayerPosition::CF] = 100;
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(98, 99);
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(96, 97);
    }
    if (player->mPositionBias[FifamPlayerPosition::CF] == 100) {
        UChar currentLevel = GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId);
        auto savedPos = player->mMainPosition;
        player->mMainPosition = FifamPlayerPosition::CF;
        if (GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId) <= currentLevel)
            player->mMainPosition = savedPos;
    }

    // validate main position with level
    auto savedBias = player->mPositionBias;
    auto savedPos = player->mMainPosition;
    UInt bestPosIndex = 0;
    UChar oldLevel = GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId);
    UChar bestLevel = oldLevel;
    for (UInt i = 0; i < numBestPos; i++) {
        player->mMainPosition = playerPositions[i].second;
        player->mPositionBias[playerPositions[i].second.ToInt()] = 100;
        UChar currentLevel = GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId);
        if (currentLevel > bestLevel) {
            bestLevel = currentLevel;
            bestPosIndex = i;
        }
    }
    player->mPositionBias = savedBias;
    player->mMainPosition = savedPos;
    if (bestLevel > oldLevel) {
        player->mMainPosition = playerPositions[bestPosIndex].second;
        player->mPositionBias[player->mMainPosition.ToInt()] = 100;
    }

    // styles table
    auto GetPlayerStylePoints = [](foom::player * a, FifamPlayerPlayingStyle style) -> Int {
        UChar s = style.ToInt();
        if (s == FifamPlayerPlayingStyle::AttackingFB)
            return (a->mCutsInsideFromLeftWing || a->mCutsInsideFromRightWing || a->mCutsInsideFromBothWings) * 100 + Utils::Max(a->mJumpingReach, a->mAcceleration);
        else if (s == FifamPlayerPlayingStyle::DefenceFB)
            return a->mStaysBackAtAllTimes * 100 + (a->mMarking + a->mTackling) / 2;
        else if (s == FifamPlayerPlayingStyle::Libero) // Builds up
            return a->mTeamWork + a->mPositioning;
        else if (s == FifamPlayerPlayingStyle::SimplePasser)
            return a->mPlaysShortSimplePasses * 100 + a->mPassing;
        else if (s == FifamPlayerPlayingStyle::BallWinner)
            return a->mDivesIntoTackles * 100 + a->mTackling;
        else if (s == FifamPlayerPlayingStyle::HardMan) // Fighter
            return a->mDivesIntoTackles * 100 + a->mAggression;
        else if (s == FifamPlayerPlayingStyle::Holding) // Closes gaps
            return a->mTeamWork + a->mWorkRate;
        else if (s == FifamPlayerPlayingStyle::BallWinnerMidfield)
            return a->mMarksOpponentTightly * 100 + (a->mTackling + a->mPositioning) / 2;
        else if (s == FifamPlayerPlayingStyle::BoxToBox)
            return (a->mWorkRate + a->mStamina);
        else if (s == FifamPlayerPlayingStyle::Busy)
            return a->mWorkRate * 2;
        else if (s == FifamPlayerPlayingStyle::PlayMaker)
            return a->mDictatesTempo * 100 + (a->mFlair + a->mVision) / 2;
        else if (s == FifamPlayerPlayingStyle::Dribbler)
            return (a->mRunsWithBallOften || a->mTriesToPlayWayOutOfTrouble) * 100 + a->mDribbling;
        else if (s == FifamPlayerPlayingStyle::Winger)
            return (a->mCutsInsideFromLeftWing || a->mCutsInsideFromRightWing || a->mCutsInsideFromBothWings || a->mHugsLine) * 100 + (a->mAcceleration + a->mPace + a->mCrossing) / 3;
        else if (s == FifamPlayerPlayingStyle::TargetMan)
            return a->mPenaltyBoxPlayer * 100 + (a->mFinishing + a->mHeading) / 2;
        else if (s == FifamPlayerPlayingStyle::PenaltyBox)
            return a->mPenaltyBoxPlayer * 100 + a->mFinishing;
        else if (s == FifamPlayerPlayingStyle::RunsChannels)
            return (a->mMovesIntoChannels) * 100 + (a->mAcceleration + a->mPace) / 2;
        else if (s == FifamPlayerPlayingStyle::PullsWideLeft)
            return (a->mRunsWithBallDownLeft && !a->mRunsWithBallDownRight) * 100 + a->mMovement;
        else if (s == FifamPlayerPlayingStyle::PullsWideRight)
            return (a->mRunsWithBallDownRight && !a->mRunsWithBallDownLeft) * 100 + a->mMovement;
        else if (s == FifamPlayerPlayingStyle::DribblerAttack)
            return (a->mRunsWithBallOften || a->mTriesToPlayWayOutOfTrouble) * 100 + a->mDribbling;
        else if (s == FifamPlayerPlayingStyle::HoldsUp)
            return a->mStopsPlay * 100 + (a->mDribbling + a->mTechnique) / 2;
        else if (s == FifamPlayerPlayingStyle::BusyAttacker)
            return a->mWorkRate * 2;
        else if (s == FifamPlayerPlayingStyle::TowerStrength)
            return a->mStrength * 2;
        else if (s == FifamPlayerPlayingStyle::DistanceShooter)
            return a->mShootsFromDistance * 100 + a->mLongShots;
        return 0;
    };

    auto bestStyles = FifamPlayerLevel::GetBestStylesForPlayer(player, gameId >= 13);
    if (bestStyles.empty())
        player->mPlayingStyle = FifamPlayerPlayingStyle::None;
    else if (bestStyles.size() == 1)
        player->mPlayingStyle = bestStyles[0];
    else {
        Int bestStyleScore = GetPlayerStylePoints(p, bestStyles[0]);
        FifamPlayerPlayingStyle bestStyle = bestStyles[0];
        for (UInt i = 1; i < bestStyles.size(); i++) {
            Int stylePoints = GetPlayerStylePoints(p, bestStyles[i]);
            if (stylePoints > bestStyleScore) {
                bestStyle = bestStyles[i];
                bestStyleScore = stylePoints;
            }
        }
        player->mPlayingStyle = bestStyle;
    }

    // player history
    player->mNationalTeamMatches = p->mInternationalApps;
    player->mNationalTeamGoals = p->mInternationalGoals;

    // history - validate years
    for (auto &h : p->mVecPlayingHistory) {
        if (h.mYear <= 0) {
            if (mErrors)
                Error(L"Wrong date in player history\nPlayer: %s\nPlayerID: %d", p->mFullName.c_str(), p->mID);
            h.mYear = 1970;
        }
    };
    // history - sort history entries
    std::sort(p->mVecPlayingHistory.begin(), p->mVecPlayingHistory.end(), [](foom::player::playing_history const &a, foom::player::playing_history const &b) {
        if (a.mYear < b.mYear) return true;
        if (b.mYear < a.mYear) return true;
        return a.mOrder < b.mOrder;
    });

    // history - separate by years
    Map<Int, Vector<foom::player::playing_history *>> playerHistoryMap;
    for (auto &h : p->mVecPlayingHistory) {
        if (h.mYear <= CURRENT_YEAR && /*!h.mYouthTeam &&*/ h.mClub) { // ignore 'future' records and youth team
            auto historyClub = GetTeamClubLink(h.mClub);
            if (historyClub.IsValid() || h.mClub->mConverterData.mAdditionalHistoryReserveTeamType != 1)
                playerHistoryMap[h.mYear].push_back(&h);
        }
    }

    auto GetPlayerHistorySeasonStartDate = [=](Int year) {
        if (year == CURRENT_YEAR)
            return FifamDate(1, 1, CURRENT_YEAR);
        return FifamDate(1, 7, year);
    };

    auto GetPlayerHistorySeasonEndDate = [=](Int year, bool hasCurrentYear) {
        if (year == CURRENT_YEAR)
            return FifamDate(30, 6, CURRENT_YEAR);
        else if (year == (CURRENT_YEAR - 1) && hasCurrentYear)
            return FifamDate(31, 12, year);
        return FifamDate(30, 6, year + 1);
    };

    // history - fill player history
    foom::player::playing_history *lastEntry = nullptr;
    Vector<FifamPlayerHistoryEntry> history;
    bool hasCurrent = playerHistoryMap.count(CURRENT_YEAR) > 0;
    Int lastTransferFee = 0;
    for (auto const &e : playerHistoryMap) {
        UInt entryIndex = 0;
        for (auto &h : e.second) {
            FifamPlayerHistoryEntry *last = nullptr;
            if (!history.empty())
                last = &history.back();
            FifamPlayerHistoryEntry *curr = nullptr;
            FifamClubLink historyClub;
            if (h->mClub)
                historyClub = GetTeamClubLink(h->mClub);
            if (!historyClub.IsValid()) {
                if (h->mClub->mConverterData.mAdditionalHistoryParentTeam && h->mClub->mConverterData.mAdditionalHistoryReserveTeamType == 0) {
                    if (h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub) {
                        historyClub = FifamClubLink(
                            (FifamClub *)(h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub), FifamClubTeamType::Reserve);
                    }
                }
            }
            bool createNewEntry = true;
            bool sameClub = false;

            if (last) {
                if (historyClub.mPtr && historyClub.mPtr == last->mClub.mPtr) {
                    sameClub = true;
                    if (h->mOnLoan == last->mLoan)
                        createNewEntry = false;
                }
                else if (lastEntry && lastEntry->mClub && h->mClub) {
                    if (h->mClub == lastEntry->mClub || h->mClub->mConverterData.mParentClub == lastEntry->mClub || lastEntry->mClub->mConverterData.mParentClub == h->mClub) {
                        sameClub = true;
                        if (h->mOnLoan == last->mLoan)
                            createNewEntry = false;
                    }
                }
            }
            Date startDate = GetPlayerHistorySeasonStartDate(h->mYear);
            Date endDate = GetPlayerHistorySeasonEndDate(h->mYear, hasCurrent);
            if (e.second.size() > 1 && endDate.month != 12) {
                if (entryIndex == (e.second.size() - 1))
                    startDate.Set(1, 1, endDate.year);
                else
                    endDate.Set(31, 12, endDate.year - 1);
            }
            if (!createNewEntry) {
                curr = last;
                curr->mEndDate = endDate;
            }
            else {
                curr = &history.emplace_back();
                curr->mClub = FifamClubLink(historyClub.mPtr);
                curr->mStartDate = startDate;
                curr->mEndDate = endDate;
                curr->mLoan = h->mOnLoan;
            }
            //if (!h->mYouthTeam) {
            bool firstTeam = true;
            if (h->mYouthTeam)
                firstTeam = false;
            else {
                if (historyClub.IsValid())
                    firstTeam = historyClub.mTeamType == FifamClubTeamType::First;
                else
                    firstTeam = !h->mClub || !h->mClub->mConverterData.mParentClub;
                if (firstTeam) { // MAYBE: also check division level?
                    curr->mMatches += h->mApps;
                    curr->mGoals += h->mGoals;
                }
                else {
                    curr->mReserveMatches += h->mApps;
                    curr->mReserveGoals += h->mGoals;
                }
            }
            //}
            if (h->mOnLoan) {
                if (h->mTransferFee > 0)
                    curr->mTransferFee = foom::db::convert_money(h->mTransferFee);
            }
            else if (lastTransferFee > 0)
                curr->mTransferFee = foom::db::convert_money(lastTransferFee);
            if (!h->mOnLoan)
                lastTransferFee = h->mTransferFee;
            if (sameClub && !curr->mLoan && last->mLoan) {
                //Int lastFee = last->mTransferFee;
                //last->mTransferFee = curr->mTransferFee;
                //curr->mTransferFee = lastFee;
            }
            lastEntry = h;
            entryIndex++;
        }
    }

    if (!history.empty()) {
        if (player->mClub) {
            FifamPlayerHistoryEntry &last = history.back();
            if (last.mClub.mPtr != player->mClub || last.mLoan != player->mStartingConditions.mLoan.Enabled()) {
                FifamPlayerHistoryEntry &curr = history.emplace_back();
                curr.mClub = FifamClubLink(player->mClub);
                curr.mStartDate = FifamDate(1, 7, CURRENT_YEAR);
                curr.mEndDate = FifamDate(1, 7, CURRENT_YEAR);
                curr.mLoan = player->mStartingConditions.mLoan.Enabled();
                if (!curr.mLoan && lastTransferFee > 0)
                    curr.mTransferFee = foom::db::convert_money(lastTransferFee);
            }
            history.back().mStillInThisClub = true;
        }
    }

    // NOTE: check this
    if (p->mCurrentAbility >= 0) {
        for (auto const &h : history)
            player->mHistory.mEntries.push_back(h);
    }

    // national/international experience
    UInt totalLeagueMatches = 0;
    for (auto &careerEntry : history)
        totalLeagueMatches += careerEntry.mMatches + careerEntry.mReserveMatches;
    UInt totalInternationalMatches = player->mNationalTeamMatches;
    if (totalLeagueMatches > 450)
        player->mNationalExperience = 6;
    else if (totalLeagueMatches > 350)
        player->mNationalExperience = 5;
    else if (totalLeagueMatches > 250)
        player->mNationalExperience = 4;
    else if (totalLeagueMatches > 150)
        player->mNationalExperience = 3;
    else if (totalLeagueMatches > 50)
        player->mNationalExperience = 2;
    else if (totalLeagueMatches > 20)
        player->mNationalExperience = 1;
    else
        player->mNationalExperience = 0;

    if (p->mInternationalApps > 85)
        player->mInternationalExperience = 4;
    else if (p->mInternationalApps >= 55)
        player->mInternationalExperience = 3;
    else if (p->mInternationalApps >= 30)
        player->mInternationalExperience = 2;
    else if (p->mInternationalApps >= 10)
        player->mInternationalExperience = 1;

    // experience
    UInt totalExperiencePoints = 0;
    totalExperiencePoints += p->mInternationalApps * 10;
    for (auto &h : p->mVecPlayingHistory) {
        if (!h.mYouthTeam && h.mApps > 0) {
            if (h.mDivision) {
                switch (h.mDivision->mCompetitionLevel) {
                case 1:
                    totalExperiencePoints += h.mApps * 5;
                    break;
                case 2:
                    totalExperiencePoints += h.mApps * 2;
                    break;
                default:
                    totalExperiencePoints += h.mApps;
                }
            }
            else
                totalExperiencePoints += h.mApps;
        }
    }
    UInt minExperiencePoints = 0;
    if (age >= 35)
        minExperiencePoints = 2500; // 18 | 3
    else if (age >= 34)
        minExperiencePoints = 2150; // 15 | 2
    else if (age >= 33)
        minExperiencePoints = 2000; // 14 | 1
    else if (age >= 32)
        minExperiencePoints = 1850; // 13 | 1
    else if (age >= 31)
        minExperiencePoints = 1700; // 12 | 1
    else if (age >= 30)
        minExperiencePoints = 1550; // 11 | 0
    else if (age >= 29)
        minExperiencePoints = 1400; // 10 | 0
    else if (age >= 28)
        minExperiencePoints = 1250; // 9 | 0
    else if (age >= 27)
        minExperiencePoints = 950; // 7 | -1
    else if (age >= 26)
        minExperiencePoints = 800; // 6 | -1
    else if (age >= 25)
        minExperiencePoints = 700; // 5 | -2
    else if (age >= 24)
        minExperiencePoints = 550; // 4 | -2
    else if (age >= 23)
        minExperiencePoints = 400; // 3 | -2
    else if (age >= 22)
        minExperiencePoints = 250; // 2 | -4
    else if (age >= 21)
        minExperiencePoints = 100; // 1 | -4

    const Float experience_mp = 1.1f;
    minExperiencePoints = UInt(Float(minExperiencePoints) * experience_mp);

    if (totalExperiencePoints < minExperiencePoints)
        totalExperiencePoints = minExperiencePoints;
    player->mGeneralExperience = Utils::MapTo(totalExperiencePoints, 0, UInt(Float(2500) * experience_mp), 0, 18);

    player->ValidateExperience(GetCurrentDate());

    //std::wcout << p->mID << std::endl;

    UChar desiredLevel = GetPlayerLevelFromCA(p->mCurrentAbility > 0 ? p->mCurrentAbility : 1);
    if (gameId == 11 || gameId == 12) {
        Int modifier = 0;
        if (player->mGeneralExperience >= 18)
            modifier = 3;
        else if (player->mGeneralExperience >= 15)
            modifier = 2;
        else if (player->mGeneralExperience >= 12)
            modifier = 1;
        else if (player->mGeneralExperience >= 9)
            modifier = 0;
        else if (player->mGeneralExperience >= 6)
            modifier = -1;
        else if (player->mGeneralExperience >= 3)
            modifier = -2;
        else
            modifier = -4;
        player->mMaxBias = (Float)(Utils::Clamp(desiredLevel - modifier, 1, 99));
    }
    else {
        UChar currLevel = GetPlayerLevel(player, true, gameId);
        Int levelDiff = desiredLevel - currLevel;
        if (levelDiff != 0) {
            Int levelCorrection = 1;
            Int levelIteration = levelDiff * 2;
            if (levelDiff < 0) {
                levelCorrection = -1;
                levelIteration *= -1;
            }
            do {
                player->ForAllAttributes([&](UChar & attr, FifamPlayerAbilityID const &attrId) {
                    if (!FifamPlayerAttributes::IsSetPiecesAttribute(attrId)) {
                        if (FifamPlayerAttributes::IsGoalkeeperAttribute(attrId)) {
                            if (player->mMainPosition == FifamPlayerPosition::GK || (levelCorrection == -1 && desiredLevel <= 20)) {
                                attr = Utils::Clamp((Int)attr + levelCorrection, 1, 99);
                                if (attr == 99 && Random::Get(0, 1) == 1)
                                    attr = 98;
                                else if (attr < 7)
                                    attr = 5 + Random::Get(0, 2);
                            }
                        }
                        else {
                            if (player->mMainPosition != FifamPlayerPosition::GK || (levelCorrection == -1 && desiredLevel <= 20)) {
                                attr = Utils::Clamp((Int)attr + levelCorrection, 1, 99);
                                if (attr == 99 && Random::Get(0, 1) == 1)
                                    attr = 98;
                                else if (attr < 7)
                                    attr = 5 + Random::Get(0, 2);
                            }
                        }
                    }
                    });
                levelIteration--;
                currLevel = GetPlayerLevel(player, true, gameId);
            } while (levelIteration && ((levelCorrection == -1 && currLevel > desiredLevel) || (levelCorrection == 1 && currLevel < desiredLevel)));
        }
    }

    player->mTacticalEducation = Utils::MapTo(player->mAttributes.TacticAwareness, 1, 99, 0, 7);

    // update potential if needed
    UChar finalLevel = GetPlayerLevel(player, true, gameId);
    if (player->mPotential < finalLevel)
        player->mPotential = finalLevel;

    // contract
    if (club) {
        if (p->mContract.mDateJoined > FmEmptyDate() && p->mContract.mDateJoined < GetCurrentDate())
            player->mContract.mJoined = GetDateAlignedToSeasonStart(p->mContract.mDateJoined, true, false);
        else
            player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        if (p->mContract.mWage > 0)
            player->mContract.mBasicSalary = foom::db::convert_money(p->mContract.mWage) * 52;
        if (p->mContract.mContractExpires.year <= CURRENT_YEAR)
            player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR);
        else
            player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(p->mContract.mContractExpires, false, true);

        if (p->mContract.mWillLeaveAtEndOfContract)
            player->mNoContractExtension = true;
        if (p->mContract.mMatchHighestEarnerClause)
            player->mContract.mHighestPaidPlayer = true;
        if (p->mContract.mAppearanceFee > 0)
            player->mContract.mMatchBonus = foom::db::convert_money(p->mContract.mAppearanceFee);
        if (player->mMainPosition == FifamPlayerPosition::GK) {
            if (p->mContract.mCleanSheetBonus > 0)
                player->mContract.mGoalsCleanSheetBonus = foom::db::convert_money(p->mContract.mCleanSheetBonus);
        }
        else {
            if (p->mContract.mGoalBonus > 0)
                player->mContract.mGoalsCleanSheetBonus = foom::db::convert_money(p->mContract.mGoalBonus);
        }
        if (p->mContract.mYearlyWageRise > 0)
            player->mContract.mSalaryIncrease = Utils::Clamp(p->mContract.mYearlyWageRise, 1, 100);
        if (p->mContract.mRelegationWageDrop > 0)
            player->mContract.mSalaryDecreaseOnRelegation = Utils::Clamp(p->mContract.mRelegationWageDrop, 1, 100);
        if (p->mContract.mOneYearExtensionAfterLeagueGamesFinalSeason > 0)
            player->mContract.mAutoExtend = Utils::Clamp(p->mContract.mOneYearExtensionAfterLeagueGamesFinalSeason, 1, 34);
        if (p->mContract.mOptionalContractExtensionByClub > 0)
            player->mContract.mOptionClub = Utils::Clamp(p->mContract.mOptionalContractExtensionByClub, 1, 2);
        if (p->mContract.mMinimumFeeReleaseClause > 0) {
            if (p->mContract.mMinimumFeeReleaseClauseExpiryDate == FmEmptyDate() || p->mContract.mMinimumFeeReleaseClauseExpiryDate > GetCurrentDate())
                player->mContract.mFixTransferFee = foom::db::convert_money(p->mContract.mMinimumFeeReleaseClause);
        }
        if (p->mContract.mSellOnFeePercentage > 0) {
            player->mContract.mSellOnPercentage = Utils::Clamp(p->mContract.mSellOnFeePercentage, 1, 100);
            player->mContract.mSellOnFee = 100;
        }
    }
    else {
        player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR);
    }

    // player agent
    if (p->mCurrentAbility >= 100) {
        Int randVal = Random::Get(0, 100);
        if (randVal >= 65) {
            if (randVal >= 90)
                player->mPlayerAgent = FifamPlayerAgent::Lawyer;
            else
                player->mPlayerAgent = FifamPlayerAgent::Agent;
        }
        else
            player->mPlayerAgent = FifamPlayerAgent::Agency;
    }
    else
        player->mPlayerAgent = FifamPlayerAgent::None;

    // national team
    FifamCountry *playerOriginalCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerID);
    if (IsPlayerRetiredFromNationalTeam(p->mID))
        player->mRetiredFromNationalTeam = true;
    else {
        if (p->mInternationalRetirement || (p->mInternationalRetirementDate > Date(1, 1, 2000) && p->mInternationalRetirementDate <= Date(1, 9, GetCurrentDate().year)))
            player->mRetiredFromNationalTeam = true;
    }
    if (!player->mRetiredFromNationalTeam && playerOriginalCountry && p->mDeclaredForNation)
        mNationalTeamPlayers[playerOriginalCountry->mId - 1].push_back(p);

    // character
    if (p->mPressure >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::StrongNerves, true);
    else if (p->mPressure <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::WeakNerves, true);

    if (p->mProfessionalism >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Professionalism, true);
    else if (p->mProfessionalism <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::ScandalProne, true);

    if (p->mAmbition >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Ambition, true);
    else if (p->mAmbition <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::LacksAmbition, true);

    if (p->mAdaptability >= 18)
        player->mCharacter.Set(FifamPlayerCharacter::Adaptability, true);

    if (player->mLanguages[3] != FifamLanguage::None)
        player->mCharacter.Set(FifamPlayerCharacter::LanguageGenius, true);
    else if (player->mLanguages[1] == FifamLanguage::None && p->mAdaptability <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::DoesntLikeNewLang, true);

    if (p->mSportsmanship >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Fairness, true);
    else if (p->mSportsmanship <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::HardMan, true);

    if (OriginalAttrValue(p->mInjuryProneness) >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::InjuryProne, true);
    else if (OriginalAttrValue(p->mInjuryProneness) <= 4) // maybe 3
        player->mCharacter.Set(FifamPlayerCharacter::LittleInjuryProne, true);

    if (p->mControversy >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::MediaDarling, true);
    else if (p->mControversy <= 1)
        player->mCharacter.Set(FifamPlayerCharacter::UninterestedInMedia, true);

    if (p->mLoyalty >= 19) // maybe 17-18
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationHigh, true);
    else if (p->mLoyalty <= 5) // maybe 4
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationLow, true);

    if (p->mHitsFreekicksWithPower)
        player->mCharacter.Set(FifamPlayerCharacter::DrivenFreeKicks, true);

    if (player->mMainPosition == FifamPlayerPosition::GK) {
        if (p->mUsesLongThrowToStartCounterAttacks || OriginalAttrValue(p->mThrowing) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrowOuts, true);
    }
    else {
        if (p->mPossessesLongFlatThrow || OriginalAttrValue(p->mLongThrows) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);

        if (OriginalAttrValue(p->mVersatility) >= 17) // maybe 15-16
            player->mCharacter.Set(FifamPlayerCharacter::Flexibility, true);
        else if (OriginalAttrValue(p->mVersatility) <= 4) // maybe 3
            player->mCharacter.Set(FifamPlayerCharacter::Inflexibility, true);

        Int teamWork = OriginalAttrValue(p->mTeamWork);
        if (teamWork > 0 && teamWork <= 8) {
            Float teamPlayerRating = (Float)p->mCurrentAbility / teamWork;
            if (teamPlayerRating >= 13.4f) // magic
                player->mCharacter.Set(FifamPlayerCharacter::Egoist, true);
        }
    }

    if (!player->mCharacter.Check(FifamPlayerCharacter::Egoist)) {
        if (OriginalAttrValue(p->mTeamWork) >= 16) // maybe 15/17
            player->mCharacter.Set(FifamPlayerCharacter::Teamplayer, true);
    }

    if (OriginalAttrValue(p->mWorkRate) >= 18) // maybe 17/19
        player->mCharacter.Set(FifamPlayerCharacter::HighTrainingWorkRate, true);
    else {
        if (!player->mCharacter.Check(FifamPlayerCharacter::Professionalism)) {
            Int workRate = OriginalAttrValue(p->mWorkRate);
            if (workRate > 0 && workRate <= 8) {
                Float workRateRating = (Float)p->mCurrentAbility / workRate;
                if (workRateRating >= 17.0f) // magic
                    player->mCharacter.Set(FifamPlayerCharacter::LazyInTraining, true);
            }
        }
    }

    if (p->mTemperament >= 20) {
        if (p->mCurrentAbility >= 80)
            player->mCharacter.Set(FifamPlayerCharacter::Composure, true);
    }
    else if (p->mTemperament <= 5)
        player->mCharacter.Set(FifamPlayerCharacter::Temperament, true);

    player->mCharacter.Set(FifamPlayerCharacter::LifestyleIcon, IsIconicPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Introvert, IsIntrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Extrovert, IsExtrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::FansFavorite, IsFansFavouritePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Diva, IsSensitivePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::NeedsAttention, IsInsecurePlayer(p->mID));
    if (!player->mCharacter.Check(FifamPlayerCharacter::HighTrainingWorkRate) && IsLazyPlayer(p->mID))
        player->mCharacter.Set(FifamPlayerCharacter::LazyInTraining, true);

    // hero
    UChar heroStatus = 0;
    if (OriginalAttrValue(p->mLeadership) > 17)
        heroStatus += OriginalAttrValue(p->mLeadership) - 17;
    if (OriginalAttrValue(p->mBravery) > 17)
        heroStatus += OriginalAttrValue(p->mBravery) - 17;
    if (player->mGeneralExperience >= 17)
        heroStatus += player->mGeneralExperience - 2;
    if (p->mWorldReputation >= 9500)
        heroStatus += 9;
    else if (p->mWorldReputation >= 8500)
        heroStatus += 3;
    else if (p->mWorldReputation >= 8000)
        heroStatus += 1;
    if (heroStatus > 0) {
        if (heroStatus >= 8)
            player->mHeroStatus = 4;
        else if (heroStatus == 7)
            player->mHeroStatus = 3;
        else if (heroStatus == 6) {
            if (Random::Get(0, 1) == 1)
                player->mHeroStatus = 6;
            else
                player->mHeroStatus = 10;
        }
        else if (heroStatus == 5)
            player->mHeroStatus = 2;
        else if (heroStatus == 4) {
            if (Random::Get(0, 1) == 1)
                player->mHeroStatus = 5;
            else
                player->mHeroStatus = 9;
        }
        else if (heroStatus == 3)
            player->mHeroStatus = 1;
        else if (heroStatus == 2)
            player->mHeroStatus = 8;
        else if (heroStatus == 1)
            player->mHeroStatus = 7;
    }

    // starting conditions

    // retirement
    for (auto &i : p->mVecRetirements) {
        if (i.mType == 1 && i.mDate > GetCurrentDate() && i.mDate.year <= (CURRENT_YEAR + 1)) {
            player->mStartingConditions.mRetirement.Setup();
            break;
        }
    }
    // bans
    for (auto &b : p->mVecBans) {
        // league ban
        if (!player->mStartingConditions.mLeagueBan.mEnabled && b.mNumberMatches > 0 && b.mStartDate == FmEmptyDate() && b.mEndDate == FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 3)
                player->mStartingConditions.mLeagueBan.Setup(b.mNumberMatches);
        }
        // ban until
        else if (!player->mStartingConditions.mBanUntil.mEnabled && b.mNumberMatches == 0 && b.mStartDate != FmEmptyDate() && b.mEndDate != FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 3 || b.mBanType == 17 || b.mBanType == 18) {
                if (b.mStartDate <= GetCurrentDate() && b.mEndDate > GetCurrentDate())
                    player->mStartingConditions.mBanUntil.Setup(b.mEndDate);
            }
        }
    }

    // injury
    for (auto &i : p->mVecInjuries) {
        if (i.mInjury && i.mStartDate != FmEmptyDate() && i.mStartDate <= GetCurrentDate() && i.mEndDate > GetCurrentDate()) {
            if (i.mInjury->mConverterData.mFIFAManagerID != 0) {
                FifamPlayerInjuryType injuryType = FifamPlayerInjuryType::MakeFromInt(i.mInjury->mConverterData.mFIFAManagerID);
                player->mStartingConditions.mInjury.Setup(i.mStartDate, i.mEndDate, injuryType);
                break;
            }
        }
    }

    // debug
    player->mEmpicsId = p->mID;

    return player;
}

FifamPlayer *Converter::CreateAndConvertFifaPlayer(UInt gameId, FifaPlayer * p, FifamClub * club) {
    return nullptr;
}

UChar Converter::GetPlayerLevelFromCA(Int ca) {
    if (false) {
        static Pair<UChar, UChar> playerCAtoLvlAry[99] = {
            { 99, 200 },
            { 98, 199 },
            { 97, 198 },
            { 96, 197 },
            { 95, 196 },
            { 94, 195 }, // 2
            { 93, 192 },
            { 92, 189 },
            { 91, 185 }, // 2
            { 90, 181 }, // 2
            { 89, 180 }, // 3
            { 88, 175 }, // 6
            { 87, 170 }, // 12
            { 86, 166 }, // 14
            { 85, 162 }, // 19
            { 84, 160 }, // 20
            { 83, 157 }, // 31
            { 82, 155 }, // 36
            { 81, 152 }, // 55
            { 80, 150 }, // 67
            { 79, 147 }, // 85
            { 78, 145 }, // 83
            { 77, 143 }, // 104
            { 76, 140 }, // 178
            { 75, 137 }, // 232 73, 74, 75
            { 74, 135 }, // 272
            { 73, 133 }, // 250
            { 72, 131 }, // 349
            { 71, 129 }, // 513
            { 70, 127 }, // 523
            { 69, 125 }, // 732 66, 67
            { 68, 122 }, // 996
            { 67, 120 }, // 1090
            { 66, 117 }, // 1568
            { 65, 115 }, // 1700
            { 64, 112 }, // 2165
            { 63, 109 }, // 2802
            { 62, 106 }, // 2998
            { 61, 103 }, // 3898
            { 60, 100 }, // 4608
            { 59,  97 }, // 4347
            { 58,  95 }, // 4762
            { 57,  92 }, // 5766
            { 56,  90 }, // 6673
            { 55,  87 }, // 7614
            { 54,  85 }, // 8247
            { 53,  82 }, // 9422
            { 52,  80 }, // 10445
            { 51,  77 }, // 9986
            { 50,  75 }, // 10890
            { 49,  72 }, // 11760
            { 48,  70 }, // 13467
            { 47,  67 }, // 11626
            { 46,  64 }, // 12639
            { 45,  61 }, // 10224
            { 44,  60 }, // 10599
            { 43,  58 },
            { 42,  56 },
            { 41,  54 },
            { 40,  52 },
            { 39,  50 },
            { 38,  48 },
            { 37,  46 },
            { 36,  44 },
            { 35,  42 },
            { 34,  40 },
            { 33,  38 },
            { 32,  36 },
            { 31,  34 },
            { 30,  32 },
            { 29,  30 },
            { 28,  28 },
            { 27,  27 },
            { 26,  26 },
            { 25,  25 },
            { 24,  24 },
            { 23,  23 },
            { 22,  22 },
            { 21,  21 },
            { 20,  20 },
            { 19,  19 },
            { 18,  18 },
            { 17,  17 },
            { 16,  16 },
            { 15,  15 },
            { 14,  14 },
            { 13,  13 },
            { 12,  12 },
            { 11,  11 },
            { 10,  10 },
            { 9,    9 },
            { 8,    8 },
            { 7,    7 },
            { 6,    6 },
            { 5,    5 },
            { 4,    4 },
            { 3,    3 },
            { 2,    2 },
            { 1,    1 }
        };
        for (UInt i = 0; i < 99; i++) {
            if (ca >= playerCAtoLvlAry[i].second)
                return playerCAtoLvlAry[i].first;
        }
        return 1;
    }
    else {
        static Pair<UChar, UChar> playerCAtoLvlAry[99] = {
            { 99, 199 },
            { 98, 198 },
            { 97, 197 },
            { 96, 196 },
            { 95, 195 },
            { 94, 192 },
            { 93, 190 },
            { 92, 188 },
            { 91, 184 },
            { 90, 180 },
            { 89, 177 },
            { 88, 174 },
            { 87, 170 },
            { 86, 166 },
            { 85, 163 },
            { 84, 160 },
            { 83, 157 },
            { 82, 154 },
            { 81, 151 },
            { 80, 149 },
            { 79, 146 }, // 147?
            { 78, 143 },
            { 77, 141 },
            { 76, 139 },
            { 75, 136 }, // 73, 74, 75
            { 74, 133 },
            { 73, 130 },
            { 72, 127 },
            { 71, 124 },
            { 70, 122 },
            { 69, 119 }, // 66, 67
            { 68, 115 },
            { 67, 111 },
            { 66, 108 },
            { 65, 105 },
            { 64, 102 },
            { 63,  99 },
            { 62,  96 },
            { 61,  93 },
            { 60,  90 },
            { 59,  88 },
            { 58,  86 },
            { 57,  84 },
            { 56,  82 },
            { 55,  80 },
            { 54,  78 },
            { 53,  76 },
            { 52,  74 },
            { 51,  72 },
            { 50,  70 },
            { 49,  68 },
            { 48,  66 },
            { 47,  64 },
            { 46,  62 },
            { 45,  60 },
            { 44,  58 },
            { 43,  56 },
            { 42,  54 },
            { 41,  52 },
            { 40,  50 },
            { 39,  48 },
            { 38,  46 },
            { 37,  44 },
            { 36,  42 },
            { 35,  40 },
            { 34,  38 },
            { 33,  36 },
            { 32,  34 },
            { 31,  32 },
            { 30,  30 },
            { 29,  29 },
            { 28,  28 },
            { 27,  27 },
            { 26,  26 },
            { 25,  25 },
            { 24,  24 },
            { 23,  23 },
            { 22,  22 },
            { 21,  21 },
            { 20,  20 },
            { 19,  19 },
            { 18,  18 },
            { 17,  17 },
            { 16,  16 },
            { 15,  15 },
            { 14,  14 },
            { 13,  13 },
            { 12,  12 },
            { 11,  11 },
            { 10,  10 },
            { 9,    9 },
            { 8,    8 },
            { 7,    7 },
            { 6,    6 },
            { 5,    5 },
            { 4,    4 },
            { 3,    3 },
            { 2,    2 },
            { 1,    1 }
        };
        for (UInt i = 0; i < 99; i++) {
            if (ca >= playerCAtoLvlAry[i].second)
                return playerCAtoLvlAry[i].first;
        }
        return 1;
    }
}
