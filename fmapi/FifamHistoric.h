#pragma once
#include "FifamTypes.h"
#include "FifamFifaWorldPlayer.h"

class FifamDatabase;

class FifamHistoric {
public:
    Vector<FifamFifaWorldPlayer> mFifaWorldPlayers;

    void Read(Path &historicFolder, UInt gameId, FifamDatabase *database);
    void Write(Path &historicFolder, UInt gameId, UShort vYear, UShort vNumber, FifamDatabase *database);
};
