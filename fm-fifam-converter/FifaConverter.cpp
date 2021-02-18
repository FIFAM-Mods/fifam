#include "FifaConverter.h"
#include "Random.h"
#include "Converter.h"
#include "FifamNames.h"
#include "AppearanceGenerator.h"
#include "FifamPlayerLevel.h"
#include "ConverterUtil.h"

Map<Int, Pair<UInt, UInt>> FifaConverter::mFifaToFifamCountryId;
Map<UInt, Vector<UChar>> FifaConverter::mFifaPlayerRatings;

enum eTrait1 {
    TRAIT1_INFLEXIBLE = 1, // 1 - Inflexible
    TRAIT1_LONG_THROW_IN = 2, // 2 - Long Throw-in
    TRAIT1_POWER_FREE_KICK = 4, // 4 - Power Free-Kick
    TRAIT1_DIVER = 8, // 8 - Diver
    TRAIT1_INJURY_PRONE = 16, // 16 - Injury Prone
    TRAIT1_INJURY_FREE = 32, // 32 - Injury-Free
    TRAIT1_AVOIDS_USING_WEAKER_FOOT = 64, // 64 - Avoids Using Weaker Foot
    TRAIT1_DIVES_INTO_TACKLES = 128, // 128 - Dives Into Tackles
    TRAIT1_TRIES_TO_BEAT_DEFENSIVE_LINE = 256, // 256 - Tries To Beat Defensive Line
    TRAIT1_SELFISH = 512, // 512 - Selfish
    TRAIT1_LEADERSHIP = 1024, // 1024 - Leadership
    TRAIT1_ARGUES_WITH_OFFICIALS = 2048, // 2048 - Argues With Officials
    TRAIT1_EARLY_CROSSER = 4096, // 4096 - Early Crosser
    TRAIT1_FINESSE_SHOT = 8192, // 8192 - Finesse Shot
    TRAIT1_FLAIR = 16384, // 16384 - Flair
    TRAIT1_LONG_PASSER = 32768, // 32768 - Long Passer
    TRAIT1_LONG_SHOT_TAKER = 65536, // 65536 - Long Shot Taker
    TRAIT1_TECHNICAL_DRIBBLER = 131072, // 131072 - Technical Dribbler
    TRAIT1_PLAYMAKER = 262144, // 262144 - Playmaker
    TRAIT1_GK_UP_FOR_CORNERS = 524288, // 524288 - GK Up for Corners
    TRAIT1_PUNCHER = 1048576, // 1048576 - Puncher
    TRAIT1_GK_LONG_THROW = 2097152, // 2097152 - GK Long Throw
    TRAIT1_POWER_HEADER = 4194304, // 4194304 - Power Header
    TRAIT1_1_ON_1_RUSH = 8388608, // 8388608 - 1-on-1 Rush
    TRAIT1_GIANT_THROW_IN = 16777216, // 16777216 - Giant Throw-in
    TRAIT1_OUTSIDE_FOOT_SHOT = 33554432, // 33554432 - Outside Foot Shot
    TRAIT1_FANS_FAVOURITE = 67108864, // 67108864 - Fan's Favourite
    TRAIT1_SWERVE_PASS = 134217728, // 134217728 - Swerve Pass
    TRAIT1_SECOND_WIND = 268435456, // 268435456 - Second Wind
    TRAIT1_ACROBATIC_CLEARANCE = 536870912 // 536870912 - Acrobatic Clearance
};

enum eTrait2 {
    TRAIT2_SKILLED_DRIBBLING = 1, // 1 - Skilled Dribbling
    TRAIT2_FANCY_PASSES = 2, // 2 - Fancy Passes
    TRAIT2_FANCY_FLICKS = 4, // 4 - Fancy Flicks
    TRAIT2_STUTTER_PENALTY = 8, // 8 - Stutter Penalty
    TRAIT2_CHIPPED_PENALTY = 16, // 16 - Chipped Penalty
    TRAIT2_BICYCLE_KICKS = 32, // 32 - Bicycle Kicks
    TRAIT2_DIVING_HEADER = 64, // 64 - Diving Header
    TRAIT2_DRIVEN_PASS = 128, // 128 - Driven Pass
    TRAIT2_GK_FLAT_KICK = 256, // 256 - GK Flat Kick
    TRAIT2_ONE_CLUB_PLAYER = 512, // 512 - One Club Player
    TRAIT2_TEAM_PLAYER = 1024 // 1024 - Team Player
};

UInt FifaConverter::FifamCountryIdFromFifa(Int fifaCountryId) {
    auto it = mFifaToFifamCountryId.find(fifaCountryId);
    if (it != mFifaToFifamCountryId.end())
        return (*it).second.first;
    return 0;
}

FifamNation FifaConverter::FifamNationFromFifa(Int fifaCountryId) {
    auto it = mFifaToFifamCountryId.find(fifaCountryId);
    if (it != mFifaToFifamCountryId.end())
        return FifamNation::MakeFromInt((*it).second.second);
    return FifamNation::None;
}

UInt FifaConverter::NextPersonId() {
    static UInt personId = 1;
    return personId++;
}

void FifaConverter::ConvertReferees(Converter *converter, FifamDatabase * fifam, FifaDatabase * fifa, UInt gameId) {
    fifa->ForAllReferees([&](FifaReferee &r) {
        if (r.m_gameId == FifaDatabase::m_lastSupportedGameVersion && r.internal.gender == 0) {
            UInt fifamCountryId = FifamCountryIdFromFifa(r.internal.nationalitycode);
            if (fifamCountryId != 0) {
                FifamCountry *fifamCountry = fifam->GetCountry(fifamCountryId);
                if (fifamCountry) {
                    FifamReferee *referee = fifamCountry->AddReferee();
                    referee->mFirstName = FifamNames::LimitName(converter->FixPersonName(r.m_firstName, gameId), 19);
                    referee->mLastName = FifamNames::LimitName(converter->FixPersonName(r.m_lastName, gameId), 19);
                    if (r.internal.foulstrictness == 1 && r.internal.cardstrictness == 1)
                        referee->mType = FifamRefereeType::WorldClassReferee;
                    else if (r.internal.foulstrictness == 2 && r.internal.cardstrictness == 2)
                        referee->mType = FifamRefereeType::Coward;
                    else if (r.internal.foulstrictness == 0 && r.internal.cardstrictness == 0)
                        referee->mType = FifamRefereeType::DoesNotInterfereMuchInTheGame;
                    else if (r.internal.foulstrictness + r.internal.cardstrictness == 3)
                        referee->mType = FifamRefereeType::SeverePerson;
                    else if (r.internal.isreal)
                        referee->mType = FifamRefereeType::WorldClassReferee;
                    else
                        referee->mType = FifamRefereeType::None;
                }
            }
        }
    });
}

void FifaConverter::ConvertManager(Converter *converter, FifamDatabase * fifam, FifamClub * club, FifaManager * m, UInt gameId) {
    if (m->m_gameId == FifaDatabase::m_lastSupportedGameVersion && m->internal.gender == 0 && !m->internal.surname.empty()) {
        FifamStaff *manager = fifam->CreateStaff(club, NextPersonId());
        manager->mClubPosition = FifamClubStaffPosition::Manager;
        manager->mFirstName = FifamNames::LimitName(converter->FixPersonName(m->internal.firstname, gameId), 15);
        manager->mLastName = FifamNames::LimitName(converter->FixPersonName(m->internal.surname, gameId), 15);
        manager->mPseudonym = FifamNames::LimitName(converter->FixPersonName(m->internal.commonname, gameId), 19);

        if (manager->mFirstName.empty()) {
            auto nameParts = Utils::Split(manager->mLastName, L' ', true, true);
            if (nameParts.size() > 1) {
                manager->mFirstName = nameParts[0];
                for (UInt i = 1; i < nameParts.size(); i++) {
                    if (i != 1)
                        manager->mLastName += L" ";
                    manager->mLastName += nameParts[i];
                }
            }
        }
        manager->mNationality[0] = FifamNationFromFifa(m->internal.nationality);
        manager->mBirthday.Set(1, 1, 1970);
        manager->mManagerFavouriteFormation = club->mPreferredFormations[0];
        FifamCountry *country = fifam->GetCountry(manager->mNationality[0].ToInt());
        manager->mLanguages = country ? country->mLanguages : club->mCountry->mLanguages;
        UInt level = (club->mInternationalPrestige + 10) / 2;
        UInt maxLevel = Utils::Clamp(level + 1, 1, 15);
        UInt minLevel = Utils::Clamp(level - 1, 1, 15);
        manager->mManagerCoachingSkills = Random::Get(minLevel, maxLevel);
        manager->mManagerGoalkeepersTraining = Random::Get(minLevel, maxLevel);
        manager->mManagerMotivationSkills = Random::Get(minLevel, maxLevel);
        manager->mManagerNegotiationSkills = Random::Get(minLevel, maxLevel);
    }
}

void FifaConverter::ConvertTeamAttributes(FifamClub * club, FifaTeam * t) {
    if (t->internal.transferbudget > 0)
        club->mInitialCapital = t->internal.transferbudget;
    club->mYouthBoardingSchool = t->internal.youthdevelopment;
    club->mMerchandising = t->internal.profitability / 2;
}

FifamPlayerPosition FifaPositionToFifam(Int pos) {
    switch (pos) {
    case FifaPlayer::POS_GK:
        return FifamPlayerPosition::GK;
    case FifaPlayer::POS_RWB:
        return FifamPlayerPosition::RWB;
    case FifaPlayer::POS_RB:
        return FifamPlayerPosition::RB;
    case FifaPlayer::POS_SW:
    case FifaPlayer::POS_RCB:
    case FifaPlayer::POS_CB:
    case FifaPlayer::POS_LCB:
        return FifamPlayerPosition::CB;
    case FifaPlayer::POS_LWB:
        return FifamPlayerPosition::LWB;
    case FifaPlayer::POS_LB:
        return FifamPlayerPosition::LB;
    case FifaPlayer::POS_RDM:
    case FifaPlayer::POS_CDM:
    case FifaPlayer::POS_LDM:
        return FifamPlayerPosition::DM;
    case FifaPlayer::POS_RM:
        return FifamPlayerPosition::RM;
    case FifaPlayer::POS_RCM:
    case FifaPlayer::POS_CM:
    case FifaPlayer::POS_LCM:
        return FifamPlayerPosition::CM;
    case FifaPlayer::POS_LM:
        return FifamPlayerPosition::LM;
    case FifaPlayer::POS_RAM:
    case FifaPlayer::POS_CAM:
    case FifaPlayer::POS_LAM:
        return FifamPlayerPosition::AM;
    case FifaPlayer::POS_RF:
    case FifaPlayer::POS_CF:
    case FifaPlayer::POS_LF:
        return FifamPlayerPosition::CF;
    case FifaPlayer::POS_RW:
        return FifamPlayerPosition::RW;
    case FifaPlayer::POS_RS:
    case FifaPlayer::POS_ST:
    case FifaPlayer::POS_LS:
        return FifamPlayerPosition::ST;
    case FifaPlayer::POS_LW:
        return FifamPlayerPosition::LW;
    }
    return FifamPlayerPosition::None;
}

void FifaConverter::ConvertPlayer(Converter * converter, FifamClub * club, Bool reserve, FifaTeam *fifaTeam, FifaPlayer * p, FifaPlayer::Position pos, UChar number, UInt gameId) {
    FifamNation playerNation = FifamNationFromFifa(p->internal.nationality);
    if (playerNation == FifamNation::None) {
        Error(Utils::Format(L"FIFA player %s has no nationality", p->m_quickName));
        return;
    }
    FifamCountry *playerCountry = converter->mFifamDatabase->GetCountry(playerNation.ToInt());
    if (!playerCountry) {
        Error(Utils::Format(L"Country %s (%d) is not available (FIFA player %s)", playerNation.ToStr(), playerNation.ToInt(), p->m_quickName));
        return;
    }
    FifamPlayer *player = converter->mFifamDatabase->CreatePlayer(club, NextPersonId());
    player->SetProperty(L"fifaplayer", p);
    player->mEmpicsId = p->GetId();
    // basic info
    player->mIsRealPlayer = true;
    player->mInReserveTeam = reserve;
    player->mNationality[0] = playerNation;

    auto FixedFIFAName = [](String const &n) -> String {
        String result;
        for (auto c : n) {
            
                result.push_back(c);
        }
        return result;
    };

    player->mFirstName = FifamNames::LimitPersonName(converter->FixPersonName(p->m_firstName, gameId), 15);
    player->mLastName = FifamNames::LimitPersonName(converter->FixPersonName(p->m_lastName, gameId), 19);
    if (!p->m_commonName.empty())
        player->mPseudonym = FifamNames::LimitPersonName(converter->FixPersonName(p->m_commonName, gameId), (converter->mCurrentGameId > 7) ? 29 : 19);
    player->mLanguages = playerCountry->mLanguages;
    player->mBirthday = p->m_birthDate;
    if (p->internal.preferredfoot == 1) {
        player->mRightFoot = 4;
        player->mLeftFoot = Utils::Clamp(p->internal.weakfootabilitytypecode - 1, 0, 4);
    }
    else {
        player->mLeftFoot = 4;
        player->mRightFoot = Utils::Clamp(p->internal.weakfootabilitytypecode - 1, 0, 4);
    }
    player->mHeight = Utils::Clamp(p->internal.height, 150, 220);
    player->mWeight = Utils::Clamp(p->internal.weight, 50, 150);

    // team info
    if (!player->mInReserveTeam)
        player->mShirtNumberFirstTeam = number;
    else
        player->mShirtNumberReserveTeam = number;
    if (fifaTeam && fifaTeam->internal.captainid == p->GetId())
        player->mIsCaptain = true;

    if (p->GetInternationalTeam())
        player->mCurrentlyInNationalTeam = true;

    // contract
    player->mContract.mJoined = FifaDate(p->internal.playerjointeamdate);
    player->mContract.mJoined.day = 1;
    player->mContract.mJoined.month = 7;
    player->mContract.mValidUntil = FifaDate(p->internal.contractvaliduntil);
    player->mContract.mValidUntil.day = 30;
    player->mContract.mValidUntil.month = 6;
    if (player->mContract.mValidUntil.year <= converter->GetCurrentDate().year)
        player->mContract.mValidUntil.year = converter->GetCurrentDate().year + 1;

    // position
    auto playerPos = FifaPositionToFifam(p->internal.preferredposition1);
    if (playerPos != FifamPlayerPosition::None)
        player->mMainPosition = playerPos;

    player->mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(player->mMainPosition, converter->mCurrentGameId);

    Bool hasPos4 = false;
    Set<FifamPlayerPosition> usedPositions;
    player->mPositionBias[player->mMainPosition.ToInt()] = 100;
    usedPositions.insert(player->mMainPosition);
    if (p->internal.preferredposition2 != FifaPlayer::POS_SUB && p->internal.preferredposition2 != FifaPlayer::POS_RES) {
        FifamPlayerPosition pos2 = FifaPositionToFifam(p->internal.preferredposition2);
        if (pos2 != FifamPlayerPosition::None && !Utils::Contains(usedPositions, pos2)) {
            player->mPositionBias[pos2.ToInt()] = 99;
            usedPositions.insert(pos2);
            if (p->internal.preferredposition3 != FifaPlayer::POS_SUB && p->internal.preferredposition3 != FifaPlayer::POS_RES) {
                FifamPlayerPosition pos3 = FifaPositionToFifam(p->internal.preferredposition3);
                if (pos3 != FifamPlayerPosition::None && !Utils::Contains(usedPositions, pos3)) {
                    player->mPositionBias[pos3.ToInt()] = 98;
                    usedPositions.insert(pos3);
                    if (p->internal.preferredposition4 != FifaPlayer::POS_SUB && p->internal.preferredposition4 != FifaPlayer::POS_RES) {
                        FifamPlayerPosition pos4 = FifaPositionToFifam(p->internal.preferredposition4);
                        if (pos4 != FifamPlayerPosition::None && !Utils::Contains(usedPositions, pos4)) {
                            player->mPositionBias[pos4.ToInt()] = 97;
                            usedPositions.insert(pos4);
                            hasPos4 = true;
                        }
                    }
                }
            }
        }
    }
    Float biasValue = hasPos4 ? 96.5f : 97.0f;

    if (fifaTeam) {
        FifamPlayerPosition clubPos = FifaPositionToFifam(pos);
        if (clubPos != FifamPlayerPosition::None && !Utils::Contains(usedPositions, clubPos)) {
            player->mPositionBias[clubPos.ToInt()] = biasValue;
            usedPositions.insert(clubPos);
            biasValue -= 0.5f;
        }
    }
    for (FifaPlayer::LinkedTeam &i : p->m_linkedTeams) {
        if (i.m_team->m_league && i.m_team->m_league->IsInternational()) {
            FifamPlayerPosition nationalTeamPos = FifaPositionToFifam(i.m_position);
            if (nationalTeamPos != FifamPlayerPosition::None && !Utils::Contains(usedPositions, nationalTeamPos))
                player->mPositionBias[nationalTeamPos.ToInt()] = biasValue;
            break;
        }
    }

    player->mPotential = p->internal.potential;
    auto it = mFifaPlayerRatings.find(p->GetId());
    if (it != mFifaPlayerRatings.end()) {
        auto vecRatings = (*it).second;
        if (vecRatings.size() > 1) {
            std::sort(vecRatings.begin(), vecRatings.end(), std::greater<UChar>());
            if (vecRatings[1] > player->mPotential)
                player->mPotential = vecRatings[1];
        }
    }

    if (player->mPotential >= 94)
        player->mTalent = 9;
    else if (player->mPotential >= 88)
        player->mTalent = 8;
    else if (player->mPotential >= 84)
        player->mTalent = 7;
    else if (player->mPotential >= 78)
        player->mTalent = 6;
    else if (player->mPotential >= 72)
        player->mTalent = 5;
    else if (player->mPotential >= 68)
        player->mTalent = 4;
    else if (player->mPotential >= 64)
        player->mTalent = 3;
    else if (player->mPotential >= 59)
        player->mTalent = 2;
    else
        player->mTalent = 1;

    auto FifaStat = [](UChar stat) {
        return stat; // (UChar)((Float)stat * 1.025f);
    };

    player->mAttributes.Acceleration = p->internal.acceleration;
    player->mAttributes.Aggression = p->internal.aggression;
    player->mAttributes.Agility = p->internal.agility;
    player->mAttributes.Anticipation = (p->internal.vision + p->internal.reactions + 1) / 2;
    player->mAttributes.Balance = p->internal.balance;
    player->mAttributes.BallControl = p->internal.ballcontrol;
    player->mAttributes.Composure = p->internal.composure;
    player->mAttributes.Concentration = (p->internal.composure + p->internal.reactions + 1) / 2;
    player->mAttributes.Corners = (p->internal.freekickaccuracy + p->internal.crossing + 1) / 2;
    player->mAttributes.Creativity = Utils::Clamp(p->internal.vision - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Crossing = p->internal.crossing;
    player->mAttributes.Decision = Utils::Clamp(p->internal.composure - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Determination = Utils::Clamp(p->internal.composure - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Diving = p->internal.gkdiving;
    player->mAttributes.Dribbling = p->internal.dribbling;
    player->mAttributes.Finishing = p->internal.finishing;
    player->mAttributes.Flair = Utils::Clamp(p->internal.vision - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.ForwardRuns = Utils::Clamp(p->internal.positioning - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.FreeKicks = p->internal.freekickaccuracy;
    player->mAttributes.GkCrosses = Utils::Clamp(p->internal.gkkicking - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Handling = p->internal.gkhandling;
    player->mAttributes.Header = p->internal.headingaccuracy;
    player->mAttributes.Intimidation = Utils::Clamp(p->internal.aggression - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Kicking = p->internal.gkkicking;
    player->mAttributes.LongShots = p->internal.longshots;
    player->mAttributes.ManMarking = p->internal.marking;
    player->mAttributes.OneOnOne = Utils::Clamp(p->internal.gkreflexes - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Pace = p->internal.sprintspeed;
    player->mAttributes.PenaltyShot = p->internal.penalties;
    player->mAttributes.PosDefensive = p->internal.interceptions;
    player->mAttributes.Positioning = p->internal.gkpositioning;
    player->mAttributes.PosOffensive = p->internal.positioning;
    player->mAttributes.Punching = Utils::Clamp(p->internal.gkhandling - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Reactions = p->internal.reactions;
    player->mAttributes.Reflexes = p->internal.gkreflexes;
    player->mAttributes.ShotStopping = (p->internal.gkreflexes + p->internal.gkdiving + 1) / 2;
    player->mAttributes.Stamina = p->internal.stamina;
    player->mAttributes.Strength = p->internal.strength;
    player->mAttributes.TackleSliding = p->internal.slidingtackle;
    player->mAttributes.TackleStanding = p->internal.standingtackle;
    player->mAttributes.TacklingGeneral = (p->internal.slidingtackle + p->internal.standingtackle + 1) / 2;
    player->mAttributes.Technique = (p->internal.ballcontrol + p->internal.dribbling + 1) / 2;
    player->mAttributes.Touch = Utils::Clamp(p->internal.ballcontrol - 2 + Random::Get(0, 4), 1, 99);
    player->mAttributes.Vision = p->internal.vision;
    player->mAttributes.Volleys = p->internal.volleys;
    player->mTacticalEducation = Utils::Clamp(p->internal.overallrating - 2 + Random::Get(0, 4), 1, 99);

    if (player->mMainPosition == FifamPlayerPosition::GK) {
        player->mAttributes.Consistency = Utils::Clamp(p->internal.overallrating - 2 + Random::Get(0, 4), 1, 99);
        player->mAttributes.Jumping = Utils::Max(p->internal.gkdiving, p->internal.jumping);
        player->mAttributes.LongPassing = (p->internal.overallrating + p->internal.gkkicking + 1) / 2;
        player->mAttributes.Passing = Utils::Clamp(player->mAttributes.LongPassing - 2 + Random::Get(0, 4), 1, 99);
        player->mAttributes.ShotPower = Utils::Clamp(p->internal.gkkicking - 2 + Random::Get(0, 4), 1, 99);
        player->mAttributes.TacticAwareness = Utils::Clamp(p->internal.overallrating - 2 + Random::Get(0, 4), 1, 99);
    }
    else {
        player->mAttributes.Consistency = Utils::Clamp(p->internal.balance - 2 + Random::Get(0, 4), 1, 99);
        player->mAttributes.Jumping = p->internal.jumping;
        player->mAttributes.LongPassing = p->internal.longpassing;
        player->mAttributes.Passing = p->internal.shortpassing;
        player->mAttributes.ShotPower = p->internal.shotpower;
        if (p->internal.interceptions > p->internal.positioning)
            player->mAttributes.TacticAwareness = (p->internal.vision + p->internal.interceptions + 1) / 2;
        else
            player->mAttributes.TacticAwareness = (p->internal.vision + p->internal.positioning + 1) / 2;
    }
    player->mAttributes.TeamWork = (UChar)(player->mAttributes.Anticipation * 0.8f + player->mAttributes.Stamina * 0.2f);
    if (player->mMainPosition == FifamPlayerPosition::GK)
        player->mAttributes.TeamWork /= 2;
    player->mAttributes.WorkRate = (UChar)(player->mAttributes.Stamina * 0.8f + player->mAttributes.TeamWork * 0.2f);
    if (p->internal.trait1 & TRAIT1_GK_LONG_THROW)
        player->mAttributes.Throwing = 90 + Random::Get(0, 5);
    else if (player->mMainPosition == FifamPlayerPosition::GK)
        player->mAttributes.Throwing = Random::Get(50, 65) + p->internal.overallrating / 3;
    else
        player->mAttributes.Throwing = Random::Get(6, 14);

    // leadership
    Bool clubCaptain = fifaTeam? (fifaTeam->internal.captainid == p->GetId()) : false;
    Bool natTeamCaptain = p->GetInternationalTeam() ? p->GetInternationalTeam()->internal.captainid == p->GetId() : false;
    if (p->internal.trait1 & TRAIT1_LEADERSHIP) {
        if (clubCaptain && natTeamCaptain)
            player->mAttributes.Leadership = 99;
        else if (clubCaptain || natTeamCaptain)
            player->mAttributes.Leadership = 93 + Random::Get(0, 5);
        else
            player->mAttributes.Leadership = 86 + Random::Get(0, 6);
    }
    else if (clubCaptain && natTeamCaptain)
        player->mAttributes.Leadership = 90;
    else if (clubCaptain || natTeamCaptain)
        player->mAttributes.Leadership = 80 + Random::Get(0, 5);
    else
        player->mAttributes.Leadership = 20 + Random::Get(0, 30) + p->internal.overallrating / 3;

    if (player->mMainPosition == FifamPlayerPosition::LB ||
        player->mMainPosition == FifamPlayerPosition::LM ||
        player->mMainPosition == FifamPlayerPosition::LW ||
        player->mMainPosition == FifamPlayerPosition::LWB)
    {
        if (player->mLeftFoot < 4)
            player->mLeftFoot = 4;
    }
    else if (player->mMainPosition == FifamPlayerPosition::RB ||
        player->mMainPosition == FifamPlayerPosition::RM ||
        player->mMainPosition == FifamPlayerPosition::RW ||
        player->mMainPosition == FifamPlayerPosition::RWB)
    {
        if (player->mRightFoot < 4)
            player->mRightFoot = 4;
    }

    // hero status
    UChar heroStatus = 0;
    if (p->internal.overallrating >= 90)
        heroStatus += 2;
    if (clubCaptain)
        heroStatus += 1;
    if (natTeamCaptain)
        heroStatus += 1;
    if (p->internal.trait2 & TRAIT2_TEAM_PLAYER)
        heroStatus += 1;
    if (p->internal.trait1 & TRAIT1_LEADERSHIP)
        heroStatus += 2;
    if (heroStatus > 4)
        heroStatus = 4;
    player->mHeroStatus = heroStatus;

    //Experience
    auto age = player->GetAge(converter->GetCurrentDate());
    if (age >= 33) {
        player->mNationalExperience = 6;
        player->mInternationalExperience = 2;
    }
    else if (age >= 30) {
        player->mNationalExperience = 5;
        player->mInternationalExperience = 2;
    }
    else if (age >= 27) {
        player->mNationalExperience = 4;
        player->mInternationalExperience = 1;
    }
    else if (age >= 24) {
        player->mNationalExperience = 3;
        player->mInternationalExperience = 1;
    }
    else if (age >= 22) {
        player->mNationalExperience = 2;
        player->mInternationalExperience = 0;
    }
    else if (age >= 20) {
        player->mNationalExperience = 1;
        player->mInternationalExperience = 0;
    }
    else {
        player->mNationalExperience = 0;
        player->mInternationalExperience = 0;
    }
    if (p->internal.overallrating >= 85)
        player->mInternationalExperience += 2;
    else if (p->internal.overallrating >= 82)
        player->mInternationalExperience += 1;
    player->mGeneralExperience = Utils::Clamp((player->mInternationalExperience + 1) * 2 + player->mNationalExperience + player->mNationalExperience / 2, 0, 18);

    // rating
    player->mMaxBias = Float(p->internal.overallrating);

    UChar desiredLevel = p->internal.overallrating;
    UChar currLevel = FifamPlayerLevel::GetPlayerLevel12(player, player->mMainPosition, player->mPlayingStyle, true);
    Int levelDiff = desiredLevel - currLevel;
    if (levelDiff != 0) {
        Int levelCorrection = 1;
        Int levelIteration = levelDiff * 4;
        if (levelDiff < 0) {
            levelCorrection = -1;
            levelIteration *= -1;
        }
        do {
            player->mMaxBias += levelCorrection;
            levelIteration--;
            currLevel = FifamPlayerLevel::GetPlayerLevel12(player, player->mMainPosition, player->mPlayingStyle, true);
        } while (levelIteration && ((levelCorrection == -1 && currLevel > desiredLevel) || (levelCorrection == 1 && currLevel < desiredLevel)));
    }

    // traits
    if (p->internal.trait1 & TRAIT1_INFLEXIBLE)
        player->mCharacter.Set(FifamPlayerCharacter::Inflexibility, true);
    if (p->internal.trait1 & TRAIT1_LONG_THROW_IN)
        player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);
    if (p->internal.trait1 & TRAIT1_GIANT_THROW_IN)
        player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);
    if (p->internal.trait1 & TRAIT1_GK_LONG_THROW)
        player->mCharacter.Set(FifamPlayerCharacter::LongThrowOuts, true);
    if (p->internal.trait1 & TRAIT1_INJURY_FREE)
        player->mCharacter.Set(FifamPlayerCharacter::LittleInjuryProne, true);
    if (p->internal.trait1 & TRAIT1_INJURY_PRONE)
        player->mCharacter.Set(FifamPlayerCharacter::InjuryProne, true);
    if (p->internal.trait2 & TRAIT2_TEAM_PLAYER)
        player->mCharacter.Set(FifamPlayerCharacter::Teamplayer, true);
    if (p->internal.trait1 & TRAIT1_SELFISH)
        player->mCharacter.Set(FifamPlayerCharacter::Egoist, true);
    if (p->internal.trait1 & TRAIT1_FANS_FAVOURITE)
        player->mCharacter.Set(FifamPlayerCharacter::FansFavorite, true);
    if (p->internal.trait1 & TRAIT1_ARGUES_WITH_OFFICIALS)
        player->mCharacter.Set(FifamPlayerCharacter::Critic, true);
    if (p->internal.trait1 & TRAIT1_POWER_FREE_KICK)
        player->mCharacter.Set(FifamPlayerCharacter::DrivenFreeKicks, true);
    if (p->internal.trait2 & TRAIT2_ONE_CLUB_PLAYER)
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationHigh, true);

    player->mPlayingStyle = FifamPlayerLevel::GetBestStyleForPlayer(player, false);

    // appearance
    UInt randomShoeType = Random::Get(1, 99);
    if (randomShoeType > 66)
        player->mShoeType = FifamShoeType::Blue;
    else if (randomShoeType > 33)
        player->mShoeType = FifamShoeType::White;
    else
        player->mShoeType = FifamShoeType::Black;
    AppearanceGenerator gen;
    gen.SetFromFifaPlayer(player, p);

    // loan
    if (p->m_loanedFrom && p->m_loanEndDate > Date(1, 7, 2019)) {
        FifamClub *loanClub = nullptr;
        for (auto c : converter->mFifamDatabase->mClubs) {
            if (c->mFifaID == p->m_loanedFrom->GetId()) {
                loanClub = c;
                break;
            }
        }
        player->mStartingConditions.mLoan.Setup(FifamDate(1, 7, 2019), GetDateAlignedToSeasonEnd(p->m_loanEndDate, true, true), FifamClubLink(loanClub), 0);
    }
}

void FifaConverter::WriteCurrentPlayersToHistory(FifaDatabase *fifaDb, Path const &dbPath, UInt versionNumber) {
    Path filePath = dbPath / L"fifa_player_ratings" / Utils::Format(L"%d.bin", versionNumber);
    FILE *file = fopen(filePath.string().c_str(), "wb");
#pragma pack(push, 1)
    struct playerRatingInfo {
        unsigned int id;
        unsigned char rating;
        unsigned char potential;
    };
#pragma pack(pop)
    fifaDb->ForAllPlayers([&](FifaPlayer &p) {
        if (p.m_gameId == FifaDatabase::m_lastSupportedGameVersion) {
            playerRatingInfo info;
            info.id = p.GetId();
            info.rating = p.internal.overallrating;
            info.potential = p.internal.potential;
            fwrite(&info, 6, 1, file);
        }
    });
    fclose(file);
}

void FifaConverter::ReadHistoryRatings(Path const &dbPath) {
#pragma pack(push, 1)
    struct playerRatingInfo {
        unsigned int id;
        unsigned char rating;
        unsigned char potential;
    };
#pragma pack(pop)
    for (auto const &p : directory_iterator(dbPath / L"fifa_player_ratings")) {
        if (p.path().extension() == L".bin") {
            FILE *file = fopen(p.path().string().c_str(), "rb");
            fseek(file, 0, SEEK_END);
            UInt size = ftell(file) / 6;
            fseek(file, 0, SEEK_SET);
            for (UInt i = 0; i < size; i++) {
                playerRatingInfo info;
                fread(&info, 6, 1, file);
                mFifaPlayerRatings[info.id].push_back(info.rating);
            }
            fclose(file);
        }
    }
}

