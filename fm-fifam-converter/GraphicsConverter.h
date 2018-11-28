#pragma once
#include "Converter.h"

class GraphicsConverter {
public:
    GraphicsConverter();
    void ConvertClubBadges(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minRep = 0);
    void ConvertPortrait(foom::person *person, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId);
    void ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minCA = 0);
};
