#pragma once
#include "FifamDatabase.h"
#include "FifaDatabase.h"

class Converter;

class FifaConverter {
public:
    static Map<Int, Pair<UInt, UInt>> mFifaToFifamCountryId;
    static Map<UInt, Vector<UChar>> mFifaPlayerRatings;

    static UInt FifamCountryIdFromFifa(Int fifaCountryId);
    static FifamNation FifamNationFromFifa(Int fifaCountryId);
    static UInt NextPersonId();
    static void ConvertReferees(Converter *converter, FifamDatabase *fifam, FifaDatabase *fifa, UInt gameId);
    static void ConvertManager(Converter *converter, FifamDatabase *fifam, FifamClub *club, FifaManager *m, UInt gameId);
    static void ConvertTeamAttributes(FifamClub *club, FifaTeam *t);
    static void ConvertPlayer(Converter *converter, FifamClub *club, Bool reserve, FifaTeam *fifaTeam, FifaPlayer *p, FifaPlayer::Position pos, UChar number, UInt gameId);
    static void WriteCurrentPlayersToHistory(FifaDatabase *fifaDb, Path const &dbPath, UInt versionNumber);
    static void ReadHistoryRatings(Path const &dbPath);
};
