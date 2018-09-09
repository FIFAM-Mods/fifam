#pragma once
#include "FifamTypes.h"
#include "FifamFifaWorldPlayer.h"
#include "FifamWorstStartingStreak.h"

class FifamHistoric {
public:
    Vector<FifamFifaWorldPlayer> mFifaWorldPlayers;
    Vector<FifamWorstStartingStreak> mWorstStartingStreaks;

    void Read(Path &historicFolder, UInt gameId);
    void Write(Path &historicFolder, UInt gameId, UShort vYear, UShort vNumber);
};
