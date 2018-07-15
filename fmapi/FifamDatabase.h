#pragma once
#include "FifamCountry.h"
#include "FifamCupAlloc.h"
#include <iostream>

class FifamDatabase {
public:
    static const size_t NUM_COUNTRIES = 207;

    Array<FifamCountry *, NUM_COUNTRIES> mCountries = {};

    Set<FifamClub *> mClubs;

    Set<FifamPlayer *> mPlayers;

    Set<FifamReferee *> mReferees;

    Set<FifamStaff *> mStaffs;

    Set<FifamStadium *> mStadiums;

    Set<FifamSponsor *> mSponsors;

    Vector<FifamCupAlloc *> mCupTemplates;

    FifamDatabase();
    FifamDatabase(size_t gameId, const wchar_t *dbPath);
    unsigned int GetInternalGameCountryId(size_t gameId, FifamNation nationId);
    bool IsCountryPresent(size_t gameId, FifamNation nationId);
    void Read(size_t gameId, Path const &dbPath);
    void Write(size_t gameId, unsigned short vYear, unsigned short vNumber, Path const &dbPath);
    void Clear();
    ~FifamDatabase();
};
