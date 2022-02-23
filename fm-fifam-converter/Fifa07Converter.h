#pragma once
#include "FifamDatabase.h"
#include "Fifa07Database.h"
#include "foom_nation.h"

class Converter;

class Fifa07Converter {
public:
    Converter *mConverter = nullptr;
    Map<UInt, Int> mFoomCountryIdToFifa;
    Vector<Fifa07Team> mTeams;
    Vector<Fifa07Player> mPlayers;
    Vector<Fifa07League> mLeagues;
    Vector<Fifa07Country> mCountries;
    Vector<Fifa07Tournament> mTournaments;
    Vector<Fifa07Referee> mReferees;
    Vector<Fifa07Formation> mFormations;
    Vector<Fifa07Stadium> mStadiums;
    UInt FoomCountryToFifaId(foom::nation *n);

    void Convert(Converter *c, Path const &dbPath);
};
