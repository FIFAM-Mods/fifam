#include "FifamUtils.h"
#include "FifamClubLink.h"
#include "FifamDatabase.h"
#include "FifamPlayer.h"
#include "FifamCompID.h"

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
    link.mTeamType = FifamClubTeamType::First;
    link.mPtr = reinterpret_cast<FifamClub *>(ID);
}

UInt FifamUtils::GetSavedClubIDFromClubLink(FifamClubLink &link) {
    return reinterpret_cast<UInt>(link.mPtr);
}

void FifamUtils::SavePlayerIDToPtr(FifamPlayer *&ptr, UInt ID) {
    ptr = reinterpret_cast<FifamPlayer *>(ID);
}

UInt FifamUtils::GetSavedPlayerIDFromPtr(FifamPlayer *&ptr) {
    return reinterpret_cast<UInt>(ptr);
}

void FifamUtils::SaveCompetitionIDToPtr(FifamCompetition *& ptr, UInt ID) {
    ptr = reinterpret_cast<FifamCompetition *>(ID);
}

UInt FifamUtils::GetSavedCompetitionIDFromPtr(FifamCompetition *& ptr) {
    return reinterpret_cast<UInt>(ptr);
}

UChar FifamUtils::GetCountryIDFromClubID(UInt clubID) {
    return (clubID >> 16) & 0xFF;
}

String FifamUtils::GetPlayerName(FifamPlayer *player) {
    if (player)
        return player->GetName();
    return L"";
}

Bool FifamUtils::ExtractCompetitionID(FifamCompID &outID, String const &line, UInt &linePos) {
    UInt brace1Posn = line.find(linePos, L'{');
    if (brace1Posn != String::npos) {
        UInt brace2Posn = line.find(brace1Posn + 1, L'}');
        if (brace2Posn != String::npos) {
            outID.SetFromStr(line.substr(brace1Posn, brace2Posn - brace1Posn + 1));
            linePos = brace2Posn + 1;
            return true;
        }
    }
    return false;
}

Bool FifamUtils::ExtractCompetitionID(FifamCompID &outID, String const &line) {
    UInt linePos = 0;
    return ExtractCompetitionID(outID, line, linePos);
}

FifamCompID FifamUtils::ExtractCompetitionID(String &line) {
    FifamCompID result;
    ExtractCompetitionID(result, line);
    return result;
}

UInt FifamUtils::ExtractCompetitionIDs(Vector<FifamCompID> &outIDs, String const &line) {
    outIDs.clear();
    UInt linePos = 0;
    FifamCompID compID;
    while (ExtractCompetitionID(compID, line, linePos))
        outIDs.push_back(compID);
    return outIDs.size();
}

Vector<FifamCompID> FifamUtils::ExtractCompetitionIDs(String const &line) {
    Vector<FifamCompID> result;
    ExtractCompetitionIDs(result, line);
    return result;
}

String FifamUtils::JoinCompetitionIDs(Vector<FifamCompID> const &compIDs, String const &sep) {
    String result;
    for (UInt i = 0; i < compIDs.size(); i++) {
        if (i != 0)
            result += sep;
        result += compIDs[i].ToStr();
    }
    return result;
}

UInt FifamUtils::TranslateCompId(UInt id, UInt gameFrom, UInt gameTo) {
    UChar region = (id >> 24) & 0xFF;
    if (ConvertRegion(region, gameFrom, gameTo))
        return (id & 0xFFFFFF) | (region << 24);
    return 0;
}

FifamCompID FifamUtils::TranslateCompId(FifamCompID const &compID, UInt gameFrom, UInt gameTo) {
    return FifamCompID(TranslateCompId(compID.ToInt(), gameFrom, gameTo));
}

UInt FifamUtils::GetCompIdForGameVersion(UInt id, UInt gameTo) {
    return TranslateCompId(id, FifamDatabase::LATEST_GAME_VERSION, gameTo);
}

FifamCompID FifamUtils::GetCompIdForGameVersion(FifamCompID const &compID, UInt gameTo) {
    return TranslateCompId(compID, FifamDatabase::LATEST_GAME_VERSION, gameTo);
}

void FifamUtils::TranslateCompIdToLatestVersion(FifamCompID &compID, UInt gameFrom) {
    compID = TranslateCompId(compID, gameFrom, FifamDatabase::LATEST_GAME_VERSION);
}

UInt FifamUtils::GetWriteableID(FifamDbWriteable *dbWriteable) {
    return dbWriteable ? dbWriteable->GetWriteableID() : 0;
}

UInt FifamUtils::GetWriteableUniqueID(FifamDbWriteable *dbWriteable) {
    return dbWriteable ? dbWriteable->GetWriteableUniqueID() : 0;
}

UInt FifamUtils::GetWriteableID(FifamClubLink const &clubLink) {
    UInt id = GetWriteableID(clubLink.mPtr);
    if (id != 0)
        id |= (clubLink.mTeamType.ToInt() << 24);
    return id;
}

UInt FifamUtils::GetWriteableUniqueID(FifamClubLink const &clubLink) {
    return GetWriteableUniqueID(clubLink.mPtr);
}

FifamDbWriteableIDsList FifamUtils::MakeWriteableIDsList(Vector<FifamClubLink> const &clubs, bool unique) {
    FifamDbWriteableIDsList list;
    for (UInt i = 0; i < clubs.size(); i++) {
        if (unique)
            list.push_back_unique(FifamUtils::GetWriteableID(clubs[i]));
        else
            list.push_back(FifamUtils::GetWriteableID(clubs[i]));
    }
    return list;
}

FifamDbWriteableIDsList FifamUtils::MakeWriteableIDsList(Vector<FifamCompetition *> const &comps, bool unique) {
    FifamDbWriteableIDsList list;
    for (UInt i = 0; i < comps.size(); i++) {
        if (unique)
            list.push_back_unique(FifamUtils::GetWriteableID(comps[i]));
        else
            list.push_back(FifamUtils::GetWriteableID(comps[i]));
    }
    return list;
}

void FifamUtils::SaveIDsToClubLinkList(Vector<UInt> ids, Vector<FifamClubLink> &clubs) {
    clubs.resize(ids.size());
    for (UInt i = 0; i < clubs.size(); i++)
        SaveClubIDToClubLink(clubs[i], ids[i]);
}

void FifamUtils::SaveIDsToCompetitionList(Vector<UInt> ids, Vector<FifamCompetition *> &comps) {
    comps.resize(ids.size());
    for (UInt i = 0; i < comps.size(); i++)
        SaveCompetitionIDToPtr(comps[i], ids[i]);
}
