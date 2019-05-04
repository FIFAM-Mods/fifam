#pragma once
#include "FifamDatabase.h"
#include "FifaDatabase.h"

class Converter;

class FifaConverter {
public:
    static Map<Int, Pair<UInt, UInt>> mFifaToFifamCountryId;

    static UInt FifamCountryIdFromFifa(Int fifaCountryId);
    static FifamNation FifamNationFromFifa(Int fifaCountryId);
    static UInt NextPersonId();
    static void ConvertReferees(FifamDatabase *fifam, FifaDatabase *fifa);
    static void ConvertManager(FifamDatabase *fifam, FifamClub *club, FifaManager *m);
    static void ConvertTeamAttributes(FifamClub *club, FifaTeam *t);
    static void ConvertPlayer(Converter *converter, FifamClub *club, FifaTeam *fifaTeam, FifaPlayer *p, FifaPlayer::Position pos, UChar number);
};
