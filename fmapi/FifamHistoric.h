#pragma once
#include "FifamTypes.h"
#include "FifamFifaWorldPlayer.h"
#include "FifamWorstStartingStreak.h"

class FifamDatabase;

class FifamHistoric {
public:
    Vector<FifamFifaWorldPlayer> mFifaWorldPlayers;
    Vector<FifamWorstStartingStreak> mWorstStartingStreaks;

    void Read(Path &historicFolder, UInt gameId, FifamDatabase *database);
    void Write(Path &historicFolder, UInt gameId, UShort vYear, UShort vNumber, FifamDatabase *database);
};
