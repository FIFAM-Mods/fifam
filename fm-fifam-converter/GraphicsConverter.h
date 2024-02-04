#pragma once
#include "Converter.h"

class GraphicsConverter {
public:
    Bool mOnlyUpdates = false;
    Bool mOutputToGameFolder = true;

    GraphicsConverter();
    void ConvertClubBadges(foom::db *db, Map<Int, Path> const &availableBadges, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minRep = 0);
    void ConvertCompBadges(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minRep = 0);
    void ConvertTrophies(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minRep = 0);
    void ConvertPortrait(foom::person *person, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void ConvertRefereePortrait(foom::official *referee, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minCA = 0);

    Bool ConvertOneCompBadge(Path const &badgePath, Path const &outputPath, String const &badgeName, UInt gameId);
    Bool ConvertOneTrophy(Path const &trophyPath, Path const &outputPath, Path const &trophyNamePath, String const &trophyRoomFolder);

    void ConvertOneCity(Int foomClubId, Int fifamClubId, Path const &inputPath, Path const &contentPath, String const &name, Int rep, FifamWriter &writer);
    void ConvertCities(foom::db *db, Path const &inputPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minRep);

    void ConvertOneStadium(Int foomId, UInt fifamClubId, Path const &inputPath, Path const &outputPath, String const &name, String const &stadName, Int rep, FifamWriter &writer);
    void ConvertStadiums(foom::db *db, Path const &inputPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath, Int minRep, bool overview);

    void ConvertCountryFlags(FifamDatabase *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);

    void ConvertPlayerPortraitsFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void ConvertClubBadgesFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void ConvertCompBadgesFIFA(FifamDatabase *db, Path const &fifaAssetsPath, Path const &contentPath, UInt gameId, Path const &gameOutputPath);

    void CopyLeagueSplitAndRelegationBadges(FifamDatabase *db, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void DownloadClubBadgesFIFA(FifamDatabase *db, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void DownloadPlayerPortraitsFIFA(FifamDatabase *db, Path const &contentPath, UInt gameId, Path const &gameOutputPath);
    void DownloadPlayerPortraitsFIFA21(FifaDatabase *db, Path const &outputPath);

    static FifamClubTeamColor GetBadgeColor(Path const &filepath);
    void ProcessFIFAXXLPortraits(foom::db* db, Path const& gameOutputPath);
};
