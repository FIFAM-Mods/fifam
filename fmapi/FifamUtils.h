#pragma once
#include "FifamTypes.h"

class FifamDatabase;
class FifamClubLink;
class FifamPlayer;

class FifamUtils {
public:
    static UChar GetConvertedRegion(UChar region, UInt gameFrom, UInt gameTo);
    static Bool ConvertRegion(UChar &region, UInt gameFrom, UInt gameTo);
    static void SaveClubIDToClubLink(FifamClubLink &link, UInt ID);
    static UInt GetSavedClubIDFromClubLink(FifamClubLink &link);
    static void SavePlayerIDToPlayerPtr(FifamPlayer *&ptr, UInt ID);
    static UInt GetSavedPlayerIDFromPlayerPtr(FifamPlayer *&ptr);
    static UInt DBTranslateClubID(FifamDatabase *database, UInt ID, UInt gameTo);
    static UInt DBGetClubIDFromLink(FifamDatabase *database, FifamClubLink const &link);
    static UInt DBClubLinkToID(FifamDatabase *database, FifamClubLink const &link, UInt gameTo);
};
