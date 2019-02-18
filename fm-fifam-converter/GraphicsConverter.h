#pragma once
#include "Converter.h"

class GraphicsConverter {
public:
    bool mOnlyUpdates = false;

    GraphicsConverter();
    void ConvertClubBadges(foom::db *db, Map<Int, Path> const &availableBadges, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep = 0);
    void ConvertCompBadges(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep = 0);
    void ConvertTrophies(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep = 0);
    void ConvertPortrait(foom::person *person, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId);
    void ConvertRefereePortrait(foom::official *referee, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId);
    void ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minCA = 0);

    Bool ConvertOneCompBadge(Path const &badgePath, Path const &outputPath, String const &badgeName, UInt gameId);
    Bool ConvertOneTrophy(Path const &trophyPath, Path const &outputPath, Path const &trophyNamePath, String const &trophyRoomFolder);

    void ConvertOneCity(Int foomClubId , Int fifamClubId, Path const &inputPath, Path const &contentPath, String const &name, Int rep, FifamWriter &writer);
    void ConvertCities(foom::db *db, Path const &inputPath, Path const &contentPath, UInt gameId, Int minRep);
};
