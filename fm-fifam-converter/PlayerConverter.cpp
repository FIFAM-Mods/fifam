#include "Converter.h"
#include "FifamPlayerLevel.h"
#include "ConverterUtil.h"
#include "Random.h"

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

namespace weight_calculator {

enum Ethnicity {
    Ethnicity_Unknown = -1,
    Ethnicity_Northern_European = 0,
    Ethnicity_Mediterranean_or_Hispanic = 1,
    Ethnicity_North_African_or_Middle_Eastern = 2,
    Ethnicity_African_or_Caribbean = 3,
    Ethnicity_Asian = 4,
    Ethnicity_South_East_Asian = 5,
    Ethnicity_Pacific_Islander = 6,
    Ethnicity_Native_American = 7,
    Ethnicity_Native_Australian = 8,
    Ethnicity_Mixed_Race = 9,
    Ethnicity_East_Asian = 10,
};

enum BodyType {
    Body_Ectomorph = 1,
    Body_EctoMesomorph = 2,
    Body_Mesomorph = 3,
    Body_MesoEndomorph = 4,
    Body_Endomorph = 5
};

Double clamp_double(Double v, Double lo, Double hi) {
    return Utils::Max(lo, Utils::Min(hi, v));
}

Int height_band_from_cm(Int cm) {
    if (cm < 165) return 0;
    if (cm < 175) return 1;
    if (cm < 185) return 2;
    if (cm < 195) return 3;
    return 4;
}

Double age_bmi_offset(Int age) {
    if (age <= 20) return -0.2;
    if (age <= 24) return  0.0;
    if (age <= 28) return  0.3;
    if (age <= 32) return  0.6;
    if (age <= 36) return  0.9;
    return 1.2;
}

Double bodytype_bmi_offset(Int bt) {
    switch (bt) {
    case Body_Ectomorph:     return -2.5;
    case Body_EctoMesomorph: return -1.0;
    case Body_Mesomorph:     return  0.0;
    case Body_MesoEndomorph: return  2.0;
    case Body_Endomorph:     return  4.0;
    default:                 return  0.0;
    }
}

Double ethnicity_bmi_offset(Int e) {
    switch (e) {
    case Ethnicity_Northern_European:               return 0.0;
    case Ethnicity_Mediterranean_or_Hispanic:       return 0.5;
    case Ethnicity_North_African_or_Middle_Eastern: return 0.3;
    case Ethnicity_African_or_Caribbean:            return -0.5;
    case Ethnicity_Asian:                           return 1.0;
    case Ethnicity_South_East_Asian:                return 0.8;
    case Ethnicity_Pacific_Islander:                return -0.3;
    case Ethnicity_Native_American:                 return 0.3;
    case Ethnicity_Native_Australian:               return 0.5;
    case Ethnicity_Mixed_Race:                      return 0.0;
    case Ethnicity_East_Asian:                      return 1.0;
    case Ethnicity_Unknown:
    default:                                        return 0.0;
    }
}

Double position_bmi_offset(FifamPlayerPosition pos) {
    switch (pos.ToInt()) {
    case FifamPlayerPosition::GK: return 0.6;
    case FifamPlayerPosition::CB: return 0.4;
    case FifamPlayerPosition::SW: return 0.4;
    case FifamPlayerPosition::DM: return 0.2;
    case FifamPlayerPosition::RB:
    case FifamPlayerPosition::LB:
    case FifamPlayerPosition::RWB:
    case FifamPlayerPosition::LWB: return 0.0;
    case FifamPlayerPosition::CF:
    case FifamPlayerPosition::ST: return 0.2;
    case FifamPlayerPosition::RW:
    case FifamPlayerPosition::LW:
    case FifamPlayerPosition::RM:
    case FifamPlayerPosition::LM: return -0.3;
    case FifamPlayerPosition::CM: return 0.0;
    case FifamPlayerPosition::AM: return -0.1;
    case FifamPlayerPosition::ANC: return 0.0;
    case FifamPlayerPosition::None:
    default: return 0.0;
    }
}

struct KgRange { int min; int max; };

const KgRange BASE_RANGES[5][6] = {
    // band 0 (150-164)
    { {0,0}, {50,62}, {56,68}, {58,70}, {65,78}, {72,86} },
    // band 1 (165-174)
    { {0,0}, {55,66}, {60,72}, {62,76}, {70,84}, {76,92} },
    // band 2 (175-184)
    { {0,0}, {60,72}, {66,78}, {68,82}, {76,92}, {84,100} },
    // band 3 (185-194)
    { {0,0}, {66,78}, {72,84}, {75,90}, {84,102}, {92,110} },
    // band 4 (195-210)
    { {0,0}, {72,86}, {80,94}, {82,98}, {92,112}, {100,120} }
};

int GuessWeightFromAttributes(Int body, Int age, Int ethnicity, Int height, Bool women, FifamPlayerPosition pos) {
    Int h_cm = Utils::Max(140, Utils::Min(220, height));
    Int a = Utils::Max(15, Utils::Min(50, age));
    Int bt = Utils::Max(1, Utils::Min(5, body));
    Double BMI_BASE = women ? 20.5 : 22.8;
    Double d_bt = bodytype_bmi_offset(bt);
    Double d_eth = ethnicity_bmi_offset(ethnicity);
    Double d_age = age_bmi_offset(a);
    Double d_pos = position_bmi_offset(pos);
    Double bmi_target = BMI_BASE + d_bt + d_eth + d_age + d_pos;
    bmi_target = clamp_double(bmi_target, 18.5, 30.0);
    Double h_m = clamp_double(h_cm / 100.0, 1.40, 2.20);
    Double raw_mass = bmi_target * (h_m * h_m);
    Int band = height_band_from_cm(h_cm);
    KgRange base = BASE_RANGES[band][static_cast<Int>(bt)];
    Double S_eth = (std::abs(d_eth) >= 1.0) ? 0.03 : ((std::abs(d_eth) >= 0.4) ? 0.015 : 0.0);
    Double S_age = (a <= 28) ? 0.0 : ((a <= 32) ? 0.02 : ((a <= 36) ? 0.035 : 0.05));
    Double S = S_eth + S_age;
    Int kg_min_adj, kg_max_adj;
    if (d_eth < 0.0) {
        kg_min_adj = static_cast<Int>(std::lround(base.min * (1.0 - S)));
        kg_max_adj = static_cast<Int>(std::lround(base.max * (1.0 - S)));
    }
    else {
        kg_min_adj = static_cast<Int>(std::lround(base.min * (1.0 + S)));
        kg_max_adj = static_cast<Int>(std::lround(base.max * (1.0 + S)));
    }
    if (kg_max_adj - kg_min_adj < 2) kg_max_adj = kg_min_adj + 2;
    Double clamped_mass = clamp_double(raw_mass, static_cast<Double>(kg_min_adj), static_cast<Double>(kg_max_adj));
    Double min_w = women ? 45.0 : 55.0;
    Double max_w = women ? 110.0 : 140.0;
    clamped_mass = clamp_double(clamped_mass, min_w, max_w);
    return static_cast<Int>(std::lround(clamped_mass));
}

}

FifamPlayer *Converter::CreateAndConvertPlayer(UInt gameId, foom::player * p, FifamClub * club) {
    if (!p->mNation) {
        Alert(AL_ERROR, L"Player without nation\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *playerCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!playerCountry) {
        Alert(AL_ERROR, L"Player without associated country\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
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
    player->mIsBasque = p->IsBasque();

    UInt age = player->GetAge(GetCurrentDate());

    // foot
    player->mLeftFoot = Utils::MapTo(p->mLeftFoot, 1, 100, 0, 4);
    player->mRightFoot = Utils::MapTo(p->mRightFoot, 1, 100, 0, 4);
    if (player->mLeftFoot != 4 && player->mRightFoot != 4)
        player->mRightFoot = 4;

    // appearance
    if (p->mHeight <= 0) {
        if (playerCountry->mAverageHeight > 10)
            player->mHeight = Utils::Clamp(playerCountry->mAverageHeight - 10 + Random::Get(0, 20), 150, 220);
        else
            player->mHeight = 175 + Random::Get(0, 10);
    }
    else
        player->mHeight = Utils::Clamp(p->mHeight, 150, 220);

    UInt randomShoeType = Random::Get(1, 99);
    if (randomShoeType > 66)
        player->mShoeType = FifamShoeType::Blue;
    else if (randomShoeType > 33)
        player->mShoeType = FifamShoeType::White;
    else
        player->mShoeType = FifamShoeType::Black;

    FifamCountry *appearanceCountry = nullptr;
    if (p->mNationOfBirth)
        appearanceCountry = mFifamDatabase->GetCountry(p->mNationOfBirth->mConverterData.mFIFAManagerReplacementID);
    if (!appearanceCountry)
        appearanceCountry = playerCountry;

    static Map<UInt, FifamAppearanceDefs::Type> CountryMainAppearanceType;
    if (CountryMainAppearanceType.empty()) {
        // TODO: verify this
        for (UInt countryId = 1; countryId <= FifamDatabase::NUM_COUNTRIES; countryId++) {
            FifamCountry *country = mFifamDatabase->GetCountry(countryId);
            if (country && !country->mAppearanceData.empty()) {
                UInt maxValue = 0;
                FifamAppearanceDefs::Type type;
                UInt prevValue = 0;
                for (UInt i = 0; i < country->mAppearanceData.size(); i++) {
                    UInt thisValue = country->mAppearanceData[i].mDistributionProbability - prevValue;
                    if (thisValue > maxValue) {
                        type = (FifamAppearanceDefs::Type)appearanceCountry->mAppearanceData[i].mAppearanceType;
                        maxValue = thisValue;
                    }
                    prevValue = country->mAppearanceData[i].mDistributionProbability;
                }
                if (maxValue > 0)
                    CountryMainAppearanceType[countryId] = type;
            }
        }
    }

    auto GetCountryMainAppearanceType = [](FifamCountry *country) {
        if (Utils::Contains(CountryMainAppearanceType, country->mId))
            return CountryMainAppearanceType[country->mId];
        return (FifamAppearanceDefs::Type)0;
    };

    FifamAppearanceDefs::Type countryEthnicity = GetCountryMainAppearanceType(appearanceCountry);

    FifamAppearanceDefs::Type ethnicity = FifamAppearanceDefs::WesternEuropeDark;
    if (!appearanceCountry->mAppearanceData.empty()) {
        UInt totalProbability = appearanceCountry->mAppearanceData[appearanceCountry->mAppearanceData.size() - 1].mDistributionProbability;
        if (totalProbability > 0) {
            UInt rnd = Random::Get(0, totalProbability - 1);
            for (UInt i = 0; i < appearanceCountry->mAppearanceData.size(); i++) {
                if (rnd < appearanceCountry->mAppearanceData[i].mDistributionProbability) {
                    ethnicity = (FifamAppearanceDefs::Type)appearanceCountry->mAppearanceData[i].mAppearanceType;
                    break;
                }
            }
        }
    }

    if (p->mEthnicity == 0) { // Northern European
        Bool isEasternEurope = countryEthnicity == FifamAppearanceDefs::EasternEuropeLight || countryEthnicity == FifamAppearanceDefs::EasternEuropeDark;
        if (p->mSkinTone <= 7)
            ethnicity = isEasternEurope ? FifamAppearanceDefs::EasternEuropeLight : FifamAppearanceDefs::WesternEuropeLight;
        else
            ethnicity = isEasternEurope ? FifamAppearanceDefs::EasternEuropeDark : FifamAppearanceDefs::WesternEuropeDark;
    }
    else if (p->mEthnicity == 1) { // Mediterranean/Hispanic
        if (countryEthnicity == FifamAppearanceDefs::Latin)
            ethnicity = FifamAppearanceDefs::Latin;
        else if (p->mSkinTone <= 5)
            ethnicity = FifamAppearanceDefs::WesternEuropeLight;
        else if (p->mSkinTone <= 11)
            ethnicity = FifamAppearanceDefs::WesternEuropeDark;
        else
            ethnicity = FifamAppearanceDefs::Latin;
    }
    else if (p->mEthnicity == 2) { // North African/Middle Eastern
        if (p->mSkinTone <= 7)
            ethnicity = FifamAppearanceDefs::Arabic2;
        else
            ethnicity = FifamAppearanceDefs::Arabic1;
    }
    else if (p->mEthnicity == 3) { // African/Caribbean
        if (p->mSkinTone < 0 || p->mSkinTone >= 16)
            ethnicity = FifamAppearanceDefs::African1;
        else
            ethnicity = FifamAppearanceDefs::African2;
    }
    else if (p->mEthnicity == 4) { // Asian
        ethnicity = FifamAppearanceDefs::Indian;
    }
    else if (p->mEthnicity == 5 || p->mEthnicity == 10) { // South East Asian or East Asian
        ethnicity = FifamAppearanceDefs::Asian;
    }
    else if (p->mEthnicity == 6 || p->mEthnicity == 8) { // Pacific Islander or Native Australian
        if (p->mSkinTone <= 5)
            ethnicity = FifamAppearanceDefs::WesternEuropeLight;
        else if (p->mSkinTone <= 11)
            ethnicity = FifamAppearanceDefs::WesternEuropeDark;
    }
    else if (p->mEthnicity == 7) {  // Native American
        ethnicity = FifamAppearanceDefs::Latin;
    }
    else if (p->mEthnicity == 9) {  // Mixed Race
        // -
    }

    // first - generate random appearance for ethnicity
    appearanceGenerator.Generate(player, ethnicity, age);

    // fix hair color
    if (player->mAppearance.mHairColor == 8)
        player->mAppearance.mHairColor = FifamHairColor::Black;

    // fix skin type
    enum NewFaceVariation {
        MaleNormal = 0,
        MaleFreckles = 1,
        MaleWrinkles = 2,
        FemaleNormal = 3,
        FemaleFreckles = 4,
        FemaleWrinkles = 5,
        FemaleMakeup1 = 6,
        FemaleMakeup2 = 7
    };
    if (p->mGender != 1) {
        static Map<UChar, UChar> FaceVariationFemaleToMale = {
            { NewFaceVariation::FemaleNormal, NewFaceVariation::MaleNormal },
            { NewFaceVariation::FemaleFreckles, NewFaceVariation::MaleFreckles },
            { NewFaceVariation::FemaleWrinkles, NewFaceVariation::MaleWrinkles },
            { NewFaceVariation::FemaleMakeup1, NewFaceVariation::MaleNormal },
            { NewFaceVariation::FemaleMakeup2, NewFaceVariation::MaleNormal },
        };
        if (Utils::Contains(FaceVariationFemaleToMale, player->mAppearance.mFaceVariation))
            player->mAppearance.mFaceVariation = FaceVariationFemaleToMale[player->mAppearance.mFaceVariation];
        if (age >= 35)
            player->mAppearance.mFaceVariation = NewFaceVariation::MaleWrinkles;
        else if (age >= 26) {
            if (player->mAppearance.mFaceVariation != NewFaceVariation::MaleFreckles) {
                if (age >= 32) {
                    if (Random::Get(1, 100) > 75)
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleWrinkles;
                }
                if (age >= 29) {
                    if (Random::Get(1, 100) > 40)
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleWrinkles;
                }
                else {
                    if (Random::Get(1, 100) > 20)
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::MaleWrinkles;
                }
            }
        }
        else {
            if (player->mAppearance.mFaceVariation == NewFaceVariation::MaleWrinkles) {
                if (Random::Get(1, 100) > 5)
                    player->mAppearance.mFaceVariation = NewFaceVariation::MaleNormal;
                else
                    player->mAppearance.mFaceVariation = NewFaceVariation::MaleFreckles;
            }
        }
    }
    else {
        static Map<UChar, UChar> FaceVariationMaleToFemale = {
            { NewFaceVariation::MaleNormal, NewFaceVariation::FemaleNormal },
            { NewFaceVariation::MaleFreckles, NewFaceVariation::MaleFreckles },
            { NewFaceVariation::MaleWrinkles, NewFaceVariation::MaleWrinkles },
        };
        if (Utils::Contains(FaceVariationMaleToFemale, player->mAppearance.mFaceVariation))
            player->mAppearance.mFaceVariation = FaceVariationMaleToFemale[player->mAppearance.mFaceVariation];
        if (age >= 35) {
            if (Random::Get(1, 100) < 80)
                player->mAppearance.mFaceVariation = NewFaceVariation::FemaleWrinkles;
        }
        else if (age >= 26) {
            if (player->mAppearance.mFaceVariation != NewFaceVariation::FemaleFreckles) {
                if (age >= 32) {
                    if (Random::Get(1, 100) < 50)
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleWrinkles;
                }
                if (age >= 29) {
                    if (Random::Get(1, 100) <  70)
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleWrinkles;
                }
                else {
                    if (Random::Get(1, 100) < 90)
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleNormal;
                    else
                        player->mAppearance.mFaceVariation = NewFaceVariation::FemaleWrinkles;
                }
            }
        }
        else {
            if (player->mAppearance.mFaceVariation == NewFaceVariation::FemaleWrinkles) {
                if (Random::Get(1, 100) < 95)
                    player->mAppearance.mFaceVariation = NewFaceVariation::FemaleNormal;
                else
                    player->mAppearance.mFaceVariation = NewFaceVariation::FemaleFreckles;
            }
        }
    }
    // second - apply skin color, hair and hair color (if available)
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
    }

    if (p->mHairLength >= 0 && p->mHairLength <= 3) {
        Int hairStyle = -1;
        static Vector<Int> defaultShortMale = { 294,1,488,605,600,495,512,241,476,219 };
        static Vector<Int> defaultMediumMale = { 95,218,232,626,72,73 };
        static Vector<Int> defaultLongMale = { 55,78,507,857,12,15 };
        static Vector<Int> defaultShortFemale = { 316,327,556,255,265,266,454,400 };
        static Vector<Int> defaultMediumFemale = { 299,344,557,321,326,432,589 };;
        static Vector<Int> defaultLongFemale = { 420,412,416,547,402,403,413,436,531,537 };
        switch (p->mHairLength) {
        case 0: // Bald
            hairStyle = 0;
            break;
        case 1:// Short
            hairStyle = appearanceGenerator.GetRandomAppearanceHairstyleByLength(ethnicity, AppearanceGenerator::HairShort,
                Random::Select(p->mGender == 1 ? defaultShortFemale : defaultShortMale));
            break;
        case 2:// Medium
            hairStyle = appearanceGenerator.GetRandomAppearanceHairstyleByLength(ethnicity, AppearanceGenerator::HairMedium,
                Random::Select(p->mGender == 1 ? defaultMediumFemale : defaultMediumMale));
            break;
        case 3:// Long
            hairStyle = appearanceGenerator.GetRandomAppearanceHairstyleByLength(ethnicity, AppearanceGenerator::HairLong,
                Random::Select(p->mGender == 1 ? defaultLongFemale : defaultLongMale));
            break;
        }
        if (hairStyle != -1)
            player->mAppearance.mHairStyle = (UShort)hairStyle;
    }

    if (p->mHairColour >= 0 && p->mHairColour <= 6) {
        switch (p->mHairColour) {
        case 0: // Black
            player->mAppearance.mHairColor = FifamHairColor::Black;
            break;
        case 1: // Blonde
            player->mAppearance.mHairColor = FifamHairColor::Blonde;
            break;
        case 2: // Brown
            player->mAppearance.mHairColor = FifamHairColor::Darkbrown;
            break;
        case 3: // Grey
            player->mAppearance.mHairColor = 8;
            break;
        case 4: // Light Blonde
            player->mAppearance.mHairColor = FifamHairColor::Platinumblonde;
            break;
        case 5: // Light Brown
            player->mAppearance.mHairColor = FifamHairColor::Lightbrown;
            break;
        case 6: // Red
            player->mAppearance.mHairColor = FifamHairColor::Red;
            break;
        }
        player->mAppearance.SetBeardColorFromHairColor();
    }

    // third - convert appearance from FIFA (if available)
    if (p->mConverterData.mFifaPlayerId > 0) {
        FifaPlayer *fifaPlayer = mFifaDatabase->GetPlayer(p->mConverterData.mFifaPlayerId);
        if (fifaPlayer)
            appearanceGenerator.SetFromFifaPlayer(player, fifaPlayer);
        if (Utils::Contains(mFaceIDs, (UInt)p->mConverterData.mFifaPlayerId))
            player->mSpecialFace = p->mConverterData.mFifaPlayerId;
        //player->mComment = L"FIFAID:" + Utils::Format(L"%d", p->mConverterData.mFifaPlayerId);
        player->mFifaID = p->mConverterData.mFifaPlayerId;
    }

    // fourth - apply custom appearance (if available)
    bool hasCustomFace = false;
    bool hasCustomHair = false;
    static Map<StringA, UInt> HeadIdFromEditorName = {
        {"Female - African 1",380},{"Female - African 2",381},{"Female - African 3",382},{"Female - African 4",383},{"Female - African 5",384},{"Female - African 6",385},{"Female - African 7",386},{"Female - African North 1",453},{"Female - African North 10",462},{"Female - African North 11",463},{"Female - African North 12",464},{"Female - African North 13",465},{"Female - African North 14",466},{"Female - African North 15",467},{"Female - African North 16",468},{"Female - African North 17",469},{"Female - African North 18",470},{"Female - African North 19",471},{"Female - African North 2",454},{"Female - African North 20",472},{"Female - African North 21",473},{"Female - African North 22",474},{"Female - African North 23",475},{"Female - African North 24",476},{"Female - African North 25",477},{"Female - African North 26",478},{"Female - African North 27",479},{"Female - African North 3",455},{"Female - African North 4",456},{"Female - African North 5",457},{"Female - African North 6",458},{"Female - African North 7",459},{"Female - African North 8",460},{"Female - African North 9",461},{"Female - African South 1",425},{"Female - African South 2",426},{"Female - African South Eur 1",480},{"Female - African South Eur 2",481},{"Female - African South Eur 3",482},{"Female - African South Eur 4",483},{"Female - African South Eur 5",484},{"Female - African South Eur 6",485},{"Female - African South Eur 7",486},{"Female - African South Eur 8",487},{"Female - Arabic 1",424},{"Female - Asian 1",350},{"Female - Asian 10",359},{"Female - Asian 11",360},{"Female - Asian 12",361},{"Female - Asian 13",362},{"Female - Asian 14",363},{"Female - Asian 15",364},{"Female - Asian 16",365},{"Female - Asian 17",366},{"Female - Asian 18",367},{"Female - Asian 19",368},{"Female - Asian 2",351},{"Female - Asian 20",369},{"Female - Asian 21",370},{"Female - Asian 22",371},{"Female - Asian 23",372},{"Female - Asian 24",373},{"Female - Asian 25",374},{"Female - Asian 26",375},{"Female - Asian 27",376},{"Female - Asian 28",377},{"Female - Asian 29",378},{"Female - Asian 3",352},{"Female - Asian 30",379},{"Female - Asian 4",353},{"Female - Asian 5",354},{"Female - Asian 6",355},{"Female - Asian 7",356},{"Female - Asian 8",357},{"Female - Asian 9",358},{"Female - Caucasian 1",338},{"Female - Caucasian 10",347},{"Female - Caucasian 11",348},{"Female - Caucasian 12",349},{"Female - Caucasian 2",339},{"Female - Caucasian 3",340},{"Female - Caucasian 4",341},{"Female - Caucasian 5",342},{"Female - Caucasian 6",343},{"Female - Caucasian 7",344},{"Female - Caucasian 8",345},{"Female - Caucasian 9",346},{"Female - East European 1",421},{"Female - East European 2",422},{"Female - East European 3",423},{"Female - Latin 1",387},{"Female - Latin 10",396},{"Female - Latin 11",397},{"Female - Latin 12",398},{"Female - Latin 13",399},{"Female - Latin 14",400},{"Female - Latin 15",401},{"Female - Latin 16",402},{"Female - Latin 17",403},{"Female - Latin 18",404},{"Female - Latin 19",405},{"Female - Latin 2",388},{"Female - Latin 20",406},{"Female - Latin 21",407},{"Female - Latin 22",408},{"Female - Latin 23",409},{"Female - Latin 24",410},{"Female - Latin 25",411},{"Female - Latin 26",412},{"Female - Latin 27",413},{"Female - Latin 28",414},{"Female - Latin 29",415},{"Female - Latin 3",389},{"Female - Latin 30",416},{"Female - Latin 31",417},{"Female - Latin 32",418},{"Female - Latin 33",419},{"Female - Latin 34",420},{"Female - Latin 4",390},{"Female - Latin 5",391},{"Female - Latin 6",392},{"Female - Latin 7",393},{"Female - Latin 8",394},{"Female - Latin 9",395},{"Female - North European 1",433},{"Female - North European 10",442},{"Female - North European 11",443},{"Female - North European 12",444},{"Female - North European 13",445},{"Female - North European 14",446},{"Female - North European 15",447},{"Female - North European 16",448},{"Female - North European 17",449},{"Female - North European 18",450},{"Female - North European 19",451},{"Female - North European 2",434},{"Female - North European 20",452},{"Female - North European 3",435},{"Female - North European 4",436},{"Female - North European 5",437},{"Female - North European 6",438},{"Female - North European 7",439},{"Female - North European 8",440},{"Female - North European 9",441},{"Female - South European 1",427},{"Female - South European 2",428},{"Female - South European 3",429},{"Female - South European 4",430},{"Female - South European 5",431},{"Female - South European 6",432},{"Male - African 1",132},{"Male - African 10",141},{"Male - African 11",142},{"Male - African 12",143},{"Male - African 13",144},{"Male - African 14",145},{"Male - African 15",146},{"Male - African 16",147},{"Male - African 17",148},{"Male - African 18",149},{"Male - African 19",150},{"Male - African 2",133},{"Male - African 20",151},{"Male - African 21",152},{"Male - African 22",153},{"Male - African 23",154},{"Male - African 24",155},{"Male - African 25",156},{"Male - African 26",157},{"Male - African 27",158},{"Male - African 28",159},{"Male - African 29",160},{"Male - African 3",134},{"Male - African 30",161},{"Male - African 31",162},{"Male - African 32",163},{"Male - African 33",164},{"Male - African 34",165},{"Male - African 35",166},{"Male - African 36",167},{"Male - African 37",168},{"Male - African 38",169},{"Male - African 39",170},{"Male - African 4",135},{"Male - African 40",171},{"Male - African 41",172},{"Male - African 42",173},{"Male - African 43",174},{"Male - African 44",175},{"Male - African 45",176},{"Male - African 46",177},{"Male - African 47",178},{"Male - African 48",179},{"Male - African 49",180},{"Male - African 5",136},{"Male - African 6",137},{"Male - African 7",138},{"Male - African 8",139},{"Male - African 9",140},{"Male - African North 1",330},{"Male - African North 2",331},{"Male - African North 3",332},{"Male - African North 4",333},{"Male - African South 1",285},{"Male - African South 2",286},{"Male - African South 3",287},{"Male - African South 4",288},{"Male - African South 5",289},{"Male - African South 6",290},{"Male - African South Eur 1",334},{"Male - African South Eur 2",335},{"Male - African South Eur 3",336},{"Male - African South Eur 4",337},{"Male - Arabic 1",126},{"Male - Arabic 10",266},{"Male - Arabic 11",267},{"Male - Arabic 12",268},{"Male - Arabic 13",269},{"Male - Arabic 14",270},{"Male - Arabic 15",271},{"Male - Arabic 16",272},{"Male - Arabic 17",273},{"Male - Arabic 18",274},{"Male - Arabic 19",275},{"Male - Arabic 2",127},{"Male - Arabic 20",276},{"Male - Arabic 21",277},{"Male - Arabic 22",278},{"Male - Arabic 23",279},{"Male - Arabic 24",280},{"Male - Arabic 25",281},{"Male - Arabic 26",282},{"Male - Arabic 27",283},{"Male - Arabic 28",284},{"Male - Arabic 3",128},{"Male - Arabic 4",129},{"Male - Arabic 5",130},{"Male - Arabic 6",131},{"Male - Arabic 7",263},{"Male - Arabic 8",264},{"Male - Arabic 9",265},{"Male - Asian 1",56},{"Male - Asian 10",65},{"Male - Asian 11",66},{"Male - Asian 12",67},{"Male - Asian 13",68},{"Male - Asian 14",69},{"Male - Asian 15",70},{"Male - Asian 16",71},{"Male - Asian 17",72},{"Male - Asian 18",73},{"Male - Asian 19",74},{"Male - Asian 2",57},{"Male - Asian 20",75},{"Male - Asian 21",76},{"Male - Asian 22",77},{"Male - Asian 23",78},{"Male - Asian 24",79},{"Male - Asian 25",80},{"Male - Asian 26",81},{"Male - Asian 27",82},{"Male - Asian 28",83},{"Male - Asian 29",84},{"Male - Asian 3",58},{"Male - Asian 30",85},{"Male - Asian 31",86},{"Male - Asian 32",87},{"Male - Asian 33",88},{"Male - Asian 34",89},{"Male - Asian 35",90},{"Male - Asian 36",91},{"Male - Asian 37",92},{"Male - Asian 38",93},{"Male - Asian 39",94},{"Male - Asian 4",59},{"Male - Asian 40",95},{"Male - Asian 41",96},{"Male - Asian 42",97},{"Male - Asian 43",98},{"Male - Asian 44",99},{"Male - Asian 45",100},{"Male - Asian 46",101},{"Male - Asian 47",102},{"Male - Asian 48",103},{"Male - Asian 49",104},{"Male - Asian 5",60},{"Male - Asian 50",105},{"Male - Asian 51",106},{"Male - Asian 52",107},{"Male - Asian 53",108},{"Male - Asian 54",109},{"Male - Asian 55",110},{"Male - Asian 56",111},{"Male - Asian 57",112},{"Male - Asian 58",113},{"Male - Asian 59",114},{"Male - Asian 6",61},{"Male - Asian 60",115},{"Male - Asian 61",116},{"Male - Asian 62",117},{"Male - Asian 63",118},{"Male - Asian 64",119},{"Male - Asian 65",120},{"Male - Asian 66",121},{"Male - Asian 67",122},{"Male - Asian 68",123},{"Male - Asian 69",124},{"Male - Asian 7",62},{"Male - Asian 70",125},{"Male - Asian 8",63},{"Male - Asian 9",64},{"Male - Caucasian 1",0},{"Male - Caucasian 10",9},{"Male - Caucasian 11",10},{"Male - Caucasian 12",11},{"Male - Caucasian 13",12},{"Male - Caucasian 14",13},{"Male - Caucasian 15",14},{"Male - Caucasian 16",15},{"Male - Caucasian 17",16},{"Male - Caucasian 18",17},{"Male - Caucasian 19",18},{"Male - Caucasian 2",1},{"Male - Caucasian 20",19},{"Male - Caucasian 21",20},{"Male - Caucasian 22",21},{"Male - Caucasian 23",22},{"Male - Caucasian 24",23},{"Male - Caucasian 25",24},{"Male - Caucasian 26",25},{"Male - Caucasian 27",26},{"Male - Caucasian 28",27},{"Male - Caucasian 29",28},{"Male - Caucasian 3",2},{"Male - Caucasian 30",29},{"Male - Caucasian 31",30},{"Male - Caucasian 32",31},{"Male - Caucasian 33",32},{"Male - Caucasian 34",33},{"Male - Caucasian 35",34},{"Male - Caucasian 36",35},{"Male - Caucasian 37",36},{"Male - Caucasian 38",37},{"Male - Caucasian 39",38},{"Male - Caucasian 4",3},{"Male - Caucasian 40",39},{"Male - Caucasian 41",40},{"Male - Caucasian 42",41},{"Male - Caucasian 43",42},{"Male - Caucasian 44",43},{"Male - Caucasian 45",44},{"Male - Caucasian 46",45},{"Male - Caucasian 47",46},{"Male - Caucasian 48",47},{"Male - Caucasian 49",48},{"Male - Caucasian 5",4},{"Male - Caucasian 50",49},{"Male - Caucasian 51",50},{"Male - Caucasian 52",51},{"Male - Caucasian 53",52},{"Male - Caucasian 54",53},{"Male - Caucasian 55",54},{"Male - Caucasian 56",55},{"Male - Caucasian 6",5},{"Male - Caucasian 7",6},{"Male - Caucasian 8",7},{"Male - Caucasian 9",8},{"Male - East European 1",233},{"Male - East European 10",242},{"Male - East European 11",243},{"Male - East European 12",244},{"Male - East European 13",245},{"Male - East European 14",246},{"Male - East European 15",247},{"Male - East European 16",248},{"Male - East European 17",249},{"Male - East European 18",250},{"Male - East European 19",251},{"Male - East European 2",234},{"Male - East European 20",252},{"Male - East European 21",253},{"Male - East European 22",254},{"Male - East European 23",255},{"Male - East European 24",256},{"Male - East European 25",257},{"Male - East European 26",258},{"Male - East European 27",259},{"Male - East European 28",260},{"Male - East European 29",261},{"Male - East European 3",235},{"Male - East European 30",262},{"Male - East European 4",236},{"Male - East European 5",237},{"Male - East European 6",238},{"Male - East European 7",239},{"Male - East European 8",240},{"Male - East European 9",241},{"Male - Latin 1",181},{"Male - Latin 10",190},{"Male - Latin 11",191},{"Male - Latin 12",192},{"Male - Latin 13",193},{"Male - Latin 14",194},{"Male - Latin 15",195},{"Male - Latin 16",196},{"Male - Latin 17",197},{"Male - Latin 18",198},{"Male - Latin 19",199},{"Male - Latin 2",182},{"Male - Latin 20",200},{"Male - Latin 21",201},{"Male - Latin 22",202},{"Male - Latin 23",203},{"Male - Latin 24",204},{"Male - Latin 25",205},{"Male - Latin 26",206},{"Male - Latin 27",207},{"Male - Latin 28",208},{"Male - Latin 29",209},{"Male - Latin 3",183},{"Male - Latin 30",210},{"Male - Latin 31",211},{"Male - Latin 32",212},{"Male - Latin 33",213},{"Male - Latin 34",214},{"Male - Latin 35",215},{"Male - Latin 36",216},{"Male - Latin 37",217},{"Male - Latin 38",218},{"Male - Latin 39",219},{"Male - Latin 4",184},{"Male - Latin 40",220},{"Male - Latin 41",221},{"Male - Latin 42",222},{"Male - Latin 43",223},{"Male - Latin 44",224},{"Male - Latin 45",225},{"Male - Latin 46",226},{"Male - Latin 47",227},{"Male - Latin 48",228},{"Male - Latin 49",229},{"Male - Latin 5",185},{"Male - Latin 50",230},{"Male - Latin 51",231},{"Male - Latin 52",232},{"Male - Latin 6",186},{"Male - Latin 7",187},{"Male - Latin 8",188},{"Male - Latin 9",189},{"Male - North European 1",309},{"Male - North European 10",318},{"Male - North European 11",319},{"Male - North European 12",320},{"Male - North European 13",321},{"Male - North European 14",322},{"Male - North European 15",323},{"Male - North European 16",324},{"Male - North European 17",325},{"Male - North European 18",326},{"Male - North European 19",327},{"Male - North European 2",310},{"Male - North European 20",328},{"Male - North European 21",329},{"Male - North European 3",311},{"Male - North European 4",312},{"Male - North European 5",313},{"Male - North European 6",314},{"Male - North European 7",315},{"Male - North European 8",316},{"Male - North European 9",317},{"Male - South European 1",291},{"Male - South European 10",300},{"Male - South European 11",301},{"Male - South European 12",302},{"Male - South European 13",303},{"Male - South European 14",304},{"Male - South European 15",305},{"Male - South European 16",306},{"Male - South European 17",307},{"Male - South European 18",308},{"Male - South European 2",292},{"Male - South European 3",293},{"Male - South European 4",294},{"Male - South European 5",295},{"Male - South European 6",296},{"Male - South European 7",297},{"Male - South European 8",298},{"Male - South European 9",299}
    };
    if (!p->mConverterData.mEditorFace.empty() && Utils::Contains(HeadIdFromEditorName, p->mConverterData.mEditorFace)) {
        player->mAppearance.mGenericFace = HeadIdFromEditorName[p->mConverterData.mEditorFace];
        hasCustomFace = true;
    }
    static Map<UInt, UInt> HairIdFromEditorId = {
        {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{26,26},{27,27},{28,28},{29,29},{30,30},{31,31},{32,32},{33,33},{34,34},{35,35},{36,36},{37,37},{38,38},{39,39},{40,40},{41,41},{42,42},{43,43},{44,44},{45,45},{46,46},{47,47},{48,48},{49,49},{51,50},{52,51},{53,52},{54,53},{55,54},{56,55},{57,56},{58,57},{59,58},{60,59},{61,60},{62,61},{63,62},{64,63},{65,64},{66,65},{67,66},{68,67},{69,68},{70,69},{71,70},{72,71},{73,72},{74,73},{75,74},{76,75},{77,76},{78,77},{79,78},{80,79},{81,80},{82,81},{83,82},{84,83},{85,84},{86,85},{87,86},{88,87},{89,88},{90,89},{91,90},{92,91},{93,92},{94,93},{95,94},{96,95},{97,96},{98,97},{99,98},{100,99},{101,100},{102,101},{103,102},{104,103},{105,104},{106,105},{107,106},{108,107},{109,108},{110,109},{111,110},{112,111},{113,112},{114,113},{115,114},{116,115},{117,116},{118,117},{119,118},{120,119},{121,120},{122,121},{123,122},{124,123},{125,124},{126,125},{127,126},{128,127},{129,128},{130,129},{131,130},{132,131},{133,132},{134,133},{135,134},{136,135},{137,136},{138,137},{139,138},{140,139},{141,140},{142,141},{143,142},{144,143},{145,144},{146,145},{147,146},{148,147},{149,148},{150,149},{151,150},{152,151},{153,152},{154,153},{155,154},{156,155},{157,156},{158,157},{159,158},{160,159},{161,160},{162,161},{163,162},{164,163},{165,164},{166,165},{167,166},{168,167},{169,168},{170,169},{171,170},{172,171},{173,172},{174,173},{175,174},{176,175},{177,176},{178,177},{179,178},{180,179},{181,180},{182,181},{183,182},{184,183},{185,184},{186,185},{187,186},{188,187},{189,188},{190,189},{191,190},{192,191},{193,192},{194,193},{195,194},{196,195},{197,196},{198,197},{199,198},{200,199},{201,200},{202,201},{203,202},{204,203},{205,204},{206,205},{207,206},{208,207},{209,208},{210,209},{211,210},{212,211},{218,212},{219,213},{220,214},{221,215},{222,216},{223,217},{224,218},{225,219},{226,220},{227,221},{228,222},{229,223},{230,224},{231,225},{232,226},{233,227},{234,228},{235,229},{236,230},{237,231},{238,232},{239,233},{240,234},{241,235},{242,236},{243,237},{244,238},{245,239},{246,240},{250,241},{251,242},{253,243},{255,244},{256,245},{257,246},{258,247},{260,248},{261,249},{262,250},{263,251},{264,252},{265,253},{266,254},{267,255},{268,256},{269,257},{270,258},{271,259},{272,260},{273,261},{274,262},{275,263},{276,264},{277,265},{278,266},{279,267},{280,268},{281,269},{282,270},{283,271},{284,272},{285,273},{286,274},{288,275},{289,276},{290,277},{291,278},{292,279},{293,280},{294,281},{295,282},{296,283},{297,284},{298,285},{299,286},{374,287},{375,288},{376,289},{381,290},{385,291},{386,292},{389,293},{390,294},{392,295},{394,296},{398,297},{399,298},{418,299},{419,300},{421,301},{431,302},{432,303},{433,304},{434,305},{435,306},{436,307},{437,308},{438,309},{444,310},{445,311},{446,312},{447,313},{448,314},{450,315},{451,316},{452,317},{460,318},{461,319},{462,320},{463,321},{492,322},{494,323},{500,324},{501,325},{502,326},{503,327},{504,328},{505,329},{506,330},{507,331},{508,332},{509,333},{510,334},{511,335},{512,336},{513,337},{514,338},{515,339},{516,340},{517,341},{518,342},{519,343},{520,344},{521,345},{522,346},{523,347},{524,348},{525,349},{526,350},{527,351},{528,352},{529,353},{530,354},{531,355},{532,356},{533,357},{534,358},{535,359},{536,360},{537,361},{538,362},{539,363},{540,364},{541,365},{542,366},{543,367},{544,368},{545,369},{546,370},{547,371},{548,372},{549,373},{550,374},{551,375},{552,376},{553,377},{554,378},{555,379},{556,380},{557,381},{558,382},{559,383},{560,384},{561,385},{562,386},{563,387},{564,388},{565,389},{566,390},{567,391},{568,392},{569,393},{570,394},{571,395},{572,396},{573,397},{574,398},{575,399},{576,400},{577,401},{578,402},{579,403},{580,404},{581,405},{582,406},{583,407},{584,408},{585,409},{586,410},{587,411},{588,412},{589,413},{590,414},{591,415},{592,416},{593,417},{594,418},{595,419},{596,420},{597,421},{598,422},{599,423},{600,424},{601,425},{602,426},{603,427},{604,428},{605,429},{606,430},{607,431},{608,432},{609,433},{610,434},{611,435},{612,436},{613,437},{614,438},{615,439},{616,440},{617,441},{618,442},{619,443},{620,444},{621,445},{622,446},{623,447},{624,448},{625,449},{626,450},{627,451},{628,452},{629,453},{630,454},{631,455},{632,456},{633,457},{634,458},{635,459},{636,460},{637,461},{638,462},{639,463},{640,464},{641,465},{642,466},{643,467},{644,468},{645,469},{646,470},{647,471},{648,472},{649,473},{650,474},{651,475},{652,476},{653,477},{654,478},{655,479},{656,480},{657,481},{659,482},{660,483},{661,484},{662,485},{663,486},{664,487},{665,488},{666,489},{667,490},{668,491},{669,492},{670,493},{671,494},{672,495},{673,496},{674,497},{675,498},{676,499},{677,500},{678,501},{679,502},{680,503},{681,504},{682,505},{683,506},{684,507},{685,508},{686,509},{687,510},{688,511},{689,512},{690,513},{691,514},{692,515},{693,516},{694,517},{695,518},{696,519},{697,520},{698,521},{720,522},{721,523},{722,524},{723,525},{724,526},{725,527},{726,528},{727,529},{728,530},{729,531},{730,532},{731,533},{732,534},{733,535},{734,536},{735,537},{736,538},{737,539},{738,540},{739,541},{740,542},{741,543},{742,544},{743,545},{744,546},{745,547},{746,548},{747,549},{748,550},{749,551},{810,552},{811,553},{815,554},{816,555},{828,556},{831,557},{832,558},{845,559},{881,560},{890,561},{891,562},{892,563},{900,564},{901,565},{902,566},{903,567},{904,568},{905,569},{906,570},{907,571},{908,572},{909,573},{910,574},{911,575},{912,576},{913,577},{914,578},{915,579},{916,580},{917,581},{918,582},{919,583},{920,584},{921,585},{922,586},{923,587},{924,588},{925,589},{926,590},{927,591},{928,592},{930,593},{931,594},{932,595},{1059,596},{1060,597},{1061,598},{1062,599},{1063,600},{1064,601},{1065,602},{1066,603},{1067,604},{1068,605},{1069,606},{1070,607},{1071,608},{1072,609},{1073,610},{1074,611},{1075,612},{1076,613},{1077,614},{1078,615},{1079,616},{1080,617},{1081,618},{1082,619},{1083,620},{1084,621},{1085,622},{1086,623},{1087,624},{1088,625},{1089,626},{1090,627},{1091,628},{1092,629},{1093,630},{1094,631},{1095,632},{1096,633},{1099,634},{1106,635},{1157,636},{1281,637},{1282,638},{1283,639},{1284,640},{1285,641},{1316,642},{1317,643},{1337,644},{1338,645},{1341,646},{1368,647},{1370,648},{1373,649},{1374,650},{1377,651},{1378,652},{1384,653},{1385,654},{1386,655},{2000,656},{2001,657},{2002,658},{2003,659},{2004,660},{2005,661},{2006,662},{2007,663},{2008,664},{2009,665},{2010,666},{2011,667},{2012,668},{2013,669},{2014,670},{2015,671},{2016,672},{2017,673},{2018,674},{2019,675},{2020,676},{2021,677},{2022,678},{2023,679},{2024,680},{2025,681},{2026,682},{2027,683},{2028,684},{2029,685},{2030,686},{2031,687},{2032,688},{2033,689},{2034,690},{2035,691},{2036,692},{2037,693},{2038,694},{2039,695},{2040,696},{2041,697},{2042,698},{2043,699},{2044,700},{2045,701},{2046,702},{2047,703},{2048,704},{2049,705},{2050,706},{2051,707},{2052,708},{2053,709},{2054,710},{2055,711},{2056,712},{2057,713},{2058,714},{2059,715},{2060,716},{2061,717},{2062,718},{2063,719},{2064,720},{2065,721},{2066,722},{2067,723},{2068,724},{3000,725},{3001,726},{3002,727},{3003,728},{3004,729},{3005,730},{3006,731},{3007,732},{3008,733},{3009,734},{3010,735},{3011,736},{3012,737},{3013,738},{3014,739},{3015,740},{3016,741},{3017,742},{3018,743},{3019,744},{3020,745},{3021,746},{3022,747},{3025,748},{3026,749},{3027,750},{3028,751},{3029,752},{3030,753},{3031,754},{3032,755},{3033,756},{3034,757},{3035,758},{3036,759},{3037,760},{3038,761},{3039,762},{3040,763},{3041,764},{3042,765},{3043,766},{3044,767},{3045,768},{3046,769},{3047,770},{3048,771},{3049,772},{3050,773},{3051,774},{3052,775},{3053,776},{3055,777},{3056,778},{3057,779},{3058,780},{3059,781},{3060,782},{3061,783},{3062,784},{3063,785},{3064,786},{3065,787},{3066,788},{3067,789},{3068,790},{3069,791},{3070,792},{3071,793},{3072,794},{3073,795},{3074,796},{3075,797},{3076,798},{3077,799},{3078,800},{3079,801},{3080,802},{3081,803},{3082,804},{3083,805},{3084,806},{3085,807},{3086,808},{3087,809},{3088,810},{3089,811},{3090,812},{3091,813},{3092,814},{3093,815},{3094,816},{3095,817},{3096,818},{3097,819},{3098,820},{3100,821},{3101,822},{3102,823},{3103,824},{3104,825},{3105,826},{3106,827},{3107,828},{3108,829},{3109,830},{3110,831},{3111,832},{3112,833},{3113,834},{3114,835},{3115,836},{3116,837},{3117,838},{3118,839},{3119,840},{3120,841},{3121,842},{3122,843},{3123,844},{3124,845},{3125,846},{3126,847},{3127,848},{3128,849},{3129,850},{3130,851},{3131,852},{3132,853},{3133,854},{3134,855},{3135,856},{3136,857},{3137,858},{3138,859},{3139,860},{3140,861},{3141,862},{3142,863},{3143,864},{3144,865},{3145,866},{3146,867},{3147,868},{3148,869},{3149,870},{3150,871},{3151,872},{3152,873},{3153,874},{3154,875},{3155,876},{3156,877},{3157,878},{3158,879},{3159,880},{3160,881},{3161,882},{3162,883},{3163,884},{3164,885},{3165,886},{3166,887},{3167,888},{3168,889},{3169,890},{3170,891},{3171,892},{3172,893},{3173,894},{3174,895},{3175,896},{3176,897},{3177,898},{3178,899},{3179,900},{3180,901},{3181,902},{3182,903},{3183,904},{3184,905},{3185,906},{3186,907},{3187,908},{3188,909},{3189,910},{3190,911},{3191,912},{3192,913},{3193,914},{3194,915},{3195,916},{3196,917},{3197,918},{3198,919},{3199,920},{3200,921},{3201,922},{3202,923},{3203,924},{3204,925},{3205,926},{3206,927},{3207,928},{3208,929},{3209,930},{3210,931},{3211,932},{3212,933},{3213,934},{3214,935},{3215,936},{3216,937},{3217,938},{3218,939},{3219,940},{3220,941},{3221,942},{3232,943},{3233,944},{3234,945},{3237,946},{3238,947},{3239,948},{3240,949},{3241,950},{3242,951},{3244,952},{3245,953},{3246,954},{3247,955},{3248,956},{3249,957},{3250,958},{3251,959},{3252,960},{3253,961},{3254,962},{3255,963},{3256,964},{3257,965},{3258,966},{3259,967},{3260,968},{3261,969},{3262,970},{3263,971},{3264,972},{3265,973},{3266,974},{3267,975},{3268,976},{3269,977},{3270,978},{3271,979},{3272,980},{3273,981},{3274,982},{3275,983},{3276,984},{3277,985},{3278,986},{3279,987},{3280,988},{3281,989},{3282,990},{3283,991},{3284,992},{3285,993},{3298,994},{3300,995},{4000,996},{4001,997},{4002,998},{4003,999},{4004,1000},{4005,1001},{4006,1002},{4007,1003},{4008,1004},{4009,1005},{4010,1006},{4011,1007},{4012,1008},{4013,1009},{4014,1010},{4015,1011},{4016,1012},{4017,1013},{4018,1014},{4019,1015},{4020,1016},{4021,1017},{4022,1018},{4023,1019},{4024,1020},{4025,1021},{4026,1022},{4027,1023},{4028,1024},{4029,1025},{4030,1026},{5000,1027},{5001,1028},{5002,1029},{5003,1030},{5004,1031},{5005,1032},{5006,1033},{5007,1034},{5008,1035},{5009,1036},{5010,1037},{5011,1038},{5012,1039},{5013,1040},{5014,1041},{5015,1042},{5016,1043},{5017,1044},{6000,1045},{6001,1046},{6002,1047},{6003,1048},{6004,1049},{6005,1050},{6006,1051},{6007,1052},{6008,1053},{6009,1054},{6010,1055},{6011,1056},{6012,1057},{6013,1058},{6014,1059},{6015,1060},{6016,1061},{6017,1062},{6018,1063},{6019,1064},{6020,1065},{6021,1066},{6022,1067},{6023,1068},{6024,1069},{6025,1070},{6026,1071},{6033,1072},{6035,1073},{6036,1074},{6037,1075},{6038,1076},{6039,1077},{6040,1078},{6041,1079},{6042,1080},{7007,1081},{7010,1082},{7013,1083},{7014,1084},{7018,1085},{7021,1086},{7025,1087},{7027,1088},{7034,1089},{7036,1090},{7037,1091},{7038,1092},{7045,1093},{7046,1094},{7047,1095},{7050,1096},{7051,1097},{7052,1098},{7054,1099},{7056,1100},{7057,1101},{7060,1102},{7070,1103},{7072,1104},{7080,1105},{7082,1106},{7083,1107},{7087,1108},{7095,1109},{7103,1110},{7117,1111},{7129,1112},{7136,1113},{7140,1114},{7145,1115},{7147,1116},{7155,1117},{7161,1118},{7163,1119},{7167,1120},{7181,1121},{7182,1122},{7183,1123},{7192,1124},{7195,1125},{7198,1126},{7251,1127},{7254,1128},{7257,1129},{7260,1130},{7262,1131},{7263,1132},{7264,1133},{7268,1134},{7270,1135},{7271,1136},{7273,1137},{7275,1138},{7277,1139},{7284,1140},{7286,1141},{7287,1142},{7288,1143},{7289,1144},{7292,1145},{7293,1146},{7294,1147},{7295,1148},{7296,1149},{7297,1150},{7298,1151},{7299,1152},{7300,1153},{7301,1154},{7302,1155},{7304,1156},{7306,1157},{7307,1158},{7310,1159},{7320,1160},{7322,1161},{7330,1162},{7332,1163},{7333,1164},{7337,1165},{7345,1166},{7353,1167},{7367,1168},{7379,1169},{7383,1170},{7386,1171},{7390,1172},{7395,1173},{7397,1174},{7405,1175},{7411,1176},{7413,1177},{7417,1178},{7431,1179},{7432,1180},{7433,1181},{7442,1182},{7445,1183},{7448,1184},{7538,1185},{7541,1186},{7544,1187},{7545,1188},{7549,1189},{7552,1190},{7556,1191},{7558,1192},{7565,1193},{7567,1194},{7568,1195},{7569,1196},{7576,1197},{7577,1198},{7578,1199},{7581,1200},{7582,1201},{7583,1202},{7585,1203},{7587,1204},{7588,1205},{7591,1206},{7601,1207},{7603,1208},{7611,1209},{7613,1210},{7614,1211},{7618,1212},{7626,1213},{7634,1214},{7648,1215},{7660,1216},{7664,1217},{7667,1218},{7671,1219},{7676,1220},{7678,1221},{7686,1222},{7692,1223},{7694,1224},{7698,1225},{7712,1226},{7713,1227},{7714,1228},{7723,1229},{7726,1230},{7729,1231}
    };
    if (p->mConverterData.mEditorHair != -1 && Utils::Contains(HairIdFromEditorId, (UInt)p->mConverterData.mEditorHair)) {
        player->mAppearance.mHairStyle = HairIdFromEditorId[p->mConverterData.mEditorHair];
        hasCustomHair = true;
    }
    static Map<StringA, UInt> BeardIdFromEditorName = {
        {"Clean Shaved",0},{"Chin Stubble Light",1},{"Chin Strap",2},{"Goatee",3},{"Casual Beard",4},{"Partial Goatee",5},{"Stubble",6},{"Tuft",7},{"Full Beard",8},{"Light Goatee",9},{"Moustache",10},{"Light Chin Curtain",11},{"Full Goatee",12},{"Chin Curtain",13},{"Beard",14},{"Patchy Beard",15} 
    };
    if (!p->mConverterData.mEditorBeard.empty()) {
        if (Utils::Contains(BeardIdFromEditorName, p->mConverterData.mEditorBeard))
            player->mAppearance.mBeardType = BeardIdFromEditorName[p->mConverterData.mEditorBeard];
        else if (hasCustomFace && hasCustomHair)
            player->mAppearance.mBeardType = FifamBeardType::None;
    }
    static Map<StringA, UInt> EyeIdFromEditorName = {
        {"LightBlue",0},{"Brown",1},{"Gray-Green",2},{"Green",3},{"Green-Blue",4},{"Grey",5},{"Blue",6},{"Light Brown",7},{"Dark Brown",8},{"Yellow",9}
    };
    if (!p->mConverterData.mEditorEye.empty()) {
        if (Utils::Contains(EyeIdFromEditorName, p->mConverterData.mEditorEye))
            player->mAppearance.mEyeColour = EyeIdFromEditorName[p->mConverterData.mEditorEye];
        else if (hasCustomFace && hasCustomHair)
            player->mAppearance.mEyeColour = FifamEyeColor::Brown;
    }
    if (p->mConverterData.mEditorSkin >= 1 && p->mConverterData.mEditorSkin <= 10)
        player->mAppearance.mSkinColor = p->mConverterData.mEditorSkin - 1;
    if (hasCustomFace && hasCustomHair)
        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;

    if (p->mGender == 1)
        player->mAppearance.mBeardType = FifamBeardType::None;
    if (p->mWearsHijab)
        player->mAppearance.mHairStyle = HairIdFromEditorId[1059];

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
        Alert(AL_ERROR, L"Player has no preferred position\nPlayer: %s\nBest pos: %s (%d)", p->mFullName.c_str(), playerPositions[0].second.ToCStr(), playerPositions[0].first);
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

    // weight
    UChar weight = 0;
    // 1) get weight from FIFA
    if (p->mConverterData.mFifaPlayerId > 0) {
        FifaPlayer *fifaPlayer = mFifaDatabase->GetPlayer(p->mConverterData.mFifaPlayerId);
        if (fifaPlayer)
            weight = fifaPlayer->internal.weight;
    }
    // 2) get weight from previous version
    if (weight == 0 && p->mConverterData.mWeight != 0)
        weight = p->mConverterData.mWeight;
    // 3) try to convert body type to weight
    if (weight == 0)
        weight = weight_calculator::GuessWeightFromAttributes(p->mBodyType, age, p->mEthnicity, player->mHeight, p->mGender, player->mMainPosition);
    // 4) generate random weight
    if (weight == 0) {
        if (playerCountry->mAverageWeight > 10)
            weight = playerCountry->mAverageWeight - 10 + Random::Get(0, 20);
        else
            weight = 70 + Random::Get(0, 10);
    }
    player->mWeight = Utils::Clamp(weight, 45, 150);

    // player history
    player->mNationalTeamMatches = p->mInternationalApps;
    player->mNationalTeamGoals = p->mInternationalGoals;

    // history - validate years
    for (auto &h : p->mVecPlayingHistory) {
        if (h.mYear <= 0) {
            Alert(AL_ERROR, L"Wrong date in player history\nPlayer: %s\nPlayerID: %d", p->mFullName.c_str(), p->mID);
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
    if (!p->mConverterData.mCharacterFlags.empty())
        player->mCharacter.SetFromStrAry(p->mConverterData.mCharacterFlags);
    if (!player->mCharacter.Check(FifamPlayerCharacter::StrongNerves) && !player->mCharacter.Check(FifamPlayerCharacter::WeakNerves)) {
        if (p->mPressure >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::StrongNerves, true);
        else if (p->mPressure <= 6)
            player->mCharacter.Set(FifamPlayerCharacter::WeakNerves, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Professionalism) && !player->mCharacter.Check(FifamPlayerCharacter::ScandalProne)) {
        if (p->mProfessionalism >= 15) {
            if (!player->mCharacter.Check(FifamPlayerCharacter::LazyInTraining))
                player->mCharacter.Set(FifamPlayerCharacter::Professionalism, true);
        }
        else if (p->mProfessionalism <= 6) {
            if (!player->mCharacter.Check(FifamPlayerCharacter::Introvert))
                player->mCharacter.Set(FifamPlayerCharacter::ScandalProne, true);
        }
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Ambition) && !player->mCharacter.Check(FifamPlayerCharacter::LacksAmbition)) {
        if (p->mAmbition >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::Ambition, true);
        else if (p->mAmbition <= 6)
            player->mCharacter.Set(FifamPlayerCharacter::LacksAmbition, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Adaptability)) {
        if (p->mAdaptability >= 18)
            player->mCharacter.Set(FifamPlayerCharacter::Adaptability, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::LanguageGenius) && !player->mCharacter.Check(FifamPlayerCharacter::DoesntLikeNewLang)) {
        if (player->mLanguages[3] != FifamLanguage::None)
            player->mCharacter.Set(FifamPlayerCharacter::LanguageGenius, true);
        else if (player->mLanguages[1] == FifamLanguage::None && p->mAdaptability <= 6)
            player->mCharacter.Set(FifamPlayerCharacter::DoesntLikeNewLang, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Fairness) && !player->mCharacter.Check(FifamPlayerCharacter::HardMan)) {
        if (p->mSportsmanship >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::Fairness, true);
        else if (p->mSportsmanship <= 6)
            player->mCharacter.Set(FifamPlayerCharacter::HardMan, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::InjuryProne) && !player->mCharacter.Check(FifamPlayerCharacter::LittleInjuryProne)) {
        if (OriginalAttrValue(p->mInjuryProneness) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::InjuryProne, true);
        else if (OriginalAttrValue(p->mInjuryProneness) <= 4) // maybe 3
            player->mCharacter.Set(FifamPlayerCharacter::LittleInjuryProne, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::MediaDarling) && !player->mCharacter.Check(FifamPlayerCharacter::UninterestedInMedia)) {
        if (p->mControversy >= 15) {
            if (!player->mCharacter.Check(FifamPlayerCharacter::Introvert))
                player->mCharacter.Set(FifamPlayerCharacter::MediaDarling, true);
        }
        else if (p->mControversy <= 1)
            player->mCharacter.Set(FifamPlayerCharacter::UninterestedInMedia, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::IdentificationHigh) && !player->mCharacter.Check(FifamPlayerCharacter::IdentificationLow)) {
        if (p->mLoyalty >= 19) // maybe 17-18
            player->mCharacter.Set(FifamPlayerCharacter::IdentificationHigh, true);
        else if (p->mLoyalty <= 5) // maybe 4
            player->mCharacter.Set(FifamPlayerCharacter::IdentificationLow, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::DrivenFreeKicks)) {
        if (p->mHitsFreekicksWithPower)
            player->mCharacter.Set(FifamPlayerCharacter::DrivenFreeKicks, true);
    }
    if (player->mMainPosition == FifamPlayerPosition::GK) {
        if (!player->mCharacter.Check(FifamPlayerCharacter::LongThrowOuts)) {
            if (p->mUsesLongThrowToStartCounterAttacks || OriginalAttrValue(p->mThrowing) >= 15)
                player->mCharacter.Set(FifamPlayerCharacter::LongThrowOuts, true);
        }
    }
    else {
        if (!player->mCharacter.Check(FifamPlayerCharacter::LongThrows)) {
            if (p->mPossessesLongFlatThrow || OriginalAttrValue(p->mLongThrows) >= 15)
                player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);
        }
        if (!player->mCharacter.Check(FifamPlayerCharacter::Flexibility) && !player->mCharacter.Check(FifamPlayerCharacter::Inflexibility)) {
            if (OriginalAttrValue(p->mVersatility) >= 17) // maybe 15-16
                player->mCharacter.Set(FifamPlayerCharacter::Flexibility, true);
            else if (OriginalAttrValue(p->mVersatility) <= 4) // maybe 3
                player->mCharacter.Set(FifamPlayerCharacter::Inflexibility, true);
        }
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Egoist) && !player->mCharacter.Check(FifamPlayerCharacter::Teamplayer)) {
        Int teamWork = OriginalAttrValue(p->mTeamWork);
        if (teamWork > 0 && teamWork <= 8) {
            Float teamPlayerRating = (Float)p->mCurrentAbility / teamWork;
            if (teamPlayerRating >= 13.4f) // magic
                player->mCharacter.Set(FifamPlayerCharacter::Egoist, true);
        }
        else if (teamWork >= 16) // maybe 15/17
            player->mCharacter.Set(FifamPlayerCharacter::Teamplayer, true);
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::HighTrainingWorkRate) && !player->mCharacter.Check(FifamPlayerCharacter::LazyInTraining)) {
        Int workRate = OriginalAttrValue(p->mWorkRate);
        if (workRate >= 18) // maybe 17/19
            player->mCharacter.Set(FifamPlayerCharacter::HighTrainingWorkRate, true);
        else {
            if (!player->mCharacter.Check(FifamPlayerCharacter::Professionalism)) {
                if (workRate > 0 && workRate <= 8) {
                    Float workRateRating = (Float)p->mCurrentAbility / workRate;
                    if (workRateRating >= 17.0f) // magic
                        player->mCharacter.Set(FifamPlayerCharacter::LazyInTraining, true);
                }
            }
        }
    }
    if (!player->mCharacter.Check(FifamPlayerCharacter::Composure) && !player->mCharacter.Check(FifamPlayerCharacter::Temperament)) {
        if (p->mTemperament >= 20) {
            if (p->mCurrentAbility >= 80)
                player->mCharacter.Set(FifamPlayerCharacter::Composure, true);
        }
        else if (p->mTemperament <= 5)
            player->mCharacter.Set(FifamPlayerCharacter::Temperament, true);
    }

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
    if (p->mSlowRetirement)
        player->mStartingConditions.mRetirement.Setup();
    else {
        for (auto &i : p->mVecRetirements) {
            if (i.mType == 1 && i.mDate > GetCurrentDate() && i.mDate.year <= (CURRENT_YEAR + 1)) {
                player->mStartingConditions.mRetirement.Setup();
                break;
            }
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
    static Date TransferWindowCloseDate = Date(1, 9, GetCurrentDate().year);
    UInt maxDuration = 0;
    foom::player::injury *selectedInjury = nullptr;
    for (auto &i : p->mVecInjuries) {
        if (i.mInjury && i.mStartDate != FmEmptyDate() && i.mStartDate < TransferWindowCloseDate && i.mEndDate > GetCurrentDate()) {
            if (i.mInjury->mConverterData.mFIFAManagerID != 0) {
                UInt duration = FifamDate(i.mEndDate) - FifamDate(i.mStartDate);
                if (i.mStartDate < GetCurrentDate() || duration >= 62) {
                    maxDuration = duration;
                    selectedInjury = &i;
                }
            }
        }
    }
    if (selectedInjury) {
        FifamPlayerInjuryType injuryType = FifamPlayerInjuryType::MakeFromInt(selectedInjury->mInjury->mConverterData.mFIFAManagerID);
        if (selectedInjury->mStartDate >= GetCurrentDate()) {
            static FifamDate PrevSeasonDate = Date(30, 6, GetCurrentDate().year);
            FifamDate endDate = selectedInjury->mEndDate;
            UInt endDateShift = FifamDate(selectedInjury->mStartDate) - PrevSeasonDate;
            endDate.SetDays(endDate.GetDays() - endDateShift);
            player->mStartingConditions.mInjury.Setup(PrevSeasonDate, endDate, injuryType);
        }
        else
            player->mStartingConditions.mInjury.Setup(selectedInjury->mStartDate, selectedInjury->mEndDate, injuryType);
    }

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
