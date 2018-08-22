#include "FifamUtils.h"
#include "FifamClubLink.h"
#include "FifamDatabase.h"
#include "FifamPlayer.h"

UChar FifamUtils::GetConvertedRegion(UChar region, UInt gameFrom, UInt gameTo) {
    if (gameFrom < 8) {
        if (region == 206)
            region = 207;
    }
    if (gameTo < 8) {
        if (region == 206)
            region = 0;
        else if (region == 207)
            region = 206;
    }
    return region;
}

Bool FifamUtils::ConvertRegion(UChar &region, UInt gameFrom, UInt gameTo) {
    Bool result = true;
    if (gameFrom < 8) {
        if (region == 206)
            region = 207;
    }
    if (gameTo < 8) {
        if (region == 206) {
            region = 0;
            result = false;
        }
        else if (region == 207)
            region = 206;
    }
    return result;
}

void FifamUtils::SaveClubIDToClubLink(FifamClubLink &link, UInt ID) {
    link.mPtr = reinterpret_cast<FifamClub *>(ID);
}

UInt FifamUtils::GetSavedClubIDFromClubLink(FifamClubLink &link) {
    return reinterpret_cast<UInt>(link.mPtr);
}

void FifamUtils::SavePlayerIDToPlayerPtr(FifamPlayer *&ptr, UInt ID) {
    ptr = reinterpret_cast<FifamPlayer *>(ID);
}

UInt FifamUtils::GetSavedPlayerIDFromPlayerPtr(FifamPlayer *&ptr) {
    return reinterpret_cast<UInt>(ptr);
}

UInt FifamUtils::DBTranslateClubID(FifamDatabase *database, UInt ID, UInt gameTo) {
    if (database && ID != 0)
        return database->TranslateClubID(ID, FifamDatabase::LATEST_GAME_VERSION, gameTo);
    return 0;
}

UInt FifamUtils::DBClubLinkToID(FifamDatabase *database, FifamClubLink const &link, UInt gameTo) {
    if (database && link.IsValid())
        return database->TranslateClubID(database->ClubToID(link), FifamDatabase::LATEST_GAME_VERSION, gameTo);
    return 0;
}

UChar FifamUtils::GetCountryIDFromClubID(UInt clubID) {
    return (clubID >> 16) & 0xFF;
}

String FifamUtils::GetPlayerName(FifamPlayer *player) {
    if (player)
        return player->GetName();
    return L"";
}
