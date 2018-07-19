#pragma once
#include "FifamCountry.h"
#include "FifamCupAlloc.h"
#include <iostream>

class FifamDatabase {
public:
    static const UChar NUM_COUNTRIES = 207;

    Array<FifamCountry *, NUM_COUNTRIES> mCountries = {};

    Set<FifamClub *> mClubs;

    Set<FifamPlayer *> mPlayers;

    Set<FifamReferee *> mReferees;

    Set<FifamStaff *> mStaffs;

    Set<FifamStadium *> mStadiums;

    Set<FifamSponsor *> mSponsors;

    Vector<FifamCupAlloc *> mCupTemplates;

    FifamDatabase();
    FifamDatabase(UInt gameId, const Path &dbPath);
    unsigned int GetInternalGameCountryId(UInt gameId, UChar nationId);
    bool IsCountryPresent(UInt gameId, UChar nationId);
    void Read(UInt gameId, Path const &dbPath);
    void Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath);
    void Clear();
    ~FifamDatabase();
};
