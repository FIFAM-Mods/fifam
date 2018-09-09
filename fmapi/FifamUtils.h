#pragma once
#include "FifamTypes.h"
#include "FifamDbWriteable.h"

class FifamDatabase;
class FifamClubLink;
class FifamPlayer;
class FifamCompID;
class FifamCompetition;
class FifamDbWriteable;

class FifamUtils {
public:
    static UChar GetConvertedRegion(UChar region, UInt gameFrom, UInt gameTo);
    static Bool ConvertRegion(UChar &region, UInt gameFrom, UInt gameTo);
    static void SaveClubIDToClubLink(FifamClubLink &link, UInt ID);
    static UInt GetSavedClubIDFromClubLink(FifamClubLink &link);
    static void SavePlayerIDToPtr(FifamPlayer *&ptr, UInt ID);
    static UInt GetSavedPlayerIDFromPtr(FifamPlayer *&ptr);
    static void SaveCompetitionIDToPtr(FifamCompetition *&ptr, UInt ID);
    static UInt GetSavedCompetitionIDFromPtr(FifamCompetition *&ptr);
    static UChar GetCountryIDFromClubID(UInt clubID);
    static String GetPlayerName(FifamPlayer *player);
    static Bool ExtractCompetitionID(FifamCompID &outID, String const &line, UInt &linePos);
    static Bool ExtractCompetitionID(FifamCompID &outID, String const &line);
    static FifamCompID ExtractCompetitionID(String &line);
    static UInt ExtractCompetitionIDs(Vector<FifamCompID> &outIDs, String const &line);
    static Vector<FifamCompID> ExtractCompetitionIDs(String const &line);
    static String JoinCompetitionIDs(Vector<FifamCompID> const &compIDs, String const &sep = L",");
    static UInt TranslateCompId(UInt id, UInt gameFrom, UInt gameTo);
    static FifamCompID TranslateCompId(FifamCompID const &compID, UInt gameFrom, UInt gameTo);
    static UInt GetCompIdForGameVersion(UInt id, UInt gameTo);
    static FifamCompID GetCompIdForGameVersion(FifamCompID const &compID, UInt gameTo);
    static void TranslateCompIdToLatestVersion(FifamCompID &compID, UInt gameFrom);
    static UInt GetWriteableID(FifamDbWriteable *dbWriteable);
    static UInt GetWriteableUniqueID(FifamDbWriteable *dbWriteable);
    static UInt GetWriteableID(FifamClubLink const &clubLink);
    static UInt GetWriteableUniqueID(FifamClubLink const &clubLink);
    static FifamDbWriteableIDsList MakeWriteableIDsList(Vector<FifamClubLink> const &clubs, bool unique = true);
    static FifamDbWriteableIDsList MakeWriteableIDsList(Vector<FifamCompetition *> const &comps, bool unique = true);
    static void SaveIDsToClubLinkList(Vector<UInt> ids, Vector<FifamClubLink> &clubs);
    static void SaveIDsToCompetitionList(Vector<UInt> ids, Vector<FifamCompetition *> &comps);
};
