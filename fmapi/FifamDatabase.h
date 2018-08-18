#pragma once
#include "FifamCountry.h"
#include "FifamCupAlloc.h"
#include "FifamClubLink.h"
#include "FifamRules.h"
#include <iostream>

/*
    Minimum supported version: FM07 0x2007.0x0C
*/

class FifamDatabase {
public:
    static const UChar NUM_COUNTRIES = 207;
    static const UInt LATEST_GAME_VERSION = 14;

    Array<FifamCountry *, NUM_COUNTRIES> mCountries = {};
    Map<UInt, FifamClub *> mClubsMap;
    Map<UInt, FifamPerson *> mPersonsMap; // players and staffs
    Set<FifamClub *> mClubs;
    Set<FifamPlayer *> mPlayers;
    Set<FifamReferee *> mReferees;
    Set<FifamStaff *> mStaffs;
    Set<FifamStadium *> mStadiums;
    Set<FifamSponsor *> mSponsors;
    Vector<FifamCupAlloc *> mCupTemplates;
    FifamRules mRules;

    FifamDatabase();
    FifamDatabase(UInt gameId, const Path &dbPath);
    unsigned int GetInternalGameCountryId(UInt gameId, UChar nationId);
    bool IsCountryPresent(UInt gameId, UChar nationId);
    void Read(UInt gameId, Path const &dbPath);
    void Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath);
    void Clear();
    ~FifamDatabase();
    FifamClub *CreateClub(FifamCountry *country);
    void AddClubToMap(FifamClub *club);
    FifamPlayer *CreatePlayer(FifamClub *club, UInt id);
    FifamStaff *CreateStaff(FifamClub *club, UInt id);

    void ResolveClubLinks(FifamClub *club, UInt gameId);
    void ResolvePlayerLinks(FifamPlayer *player, UInt gameId);
    FifamClubLink ClubFromID(UInt ID);
    FifamPlayer *PlayerFromID(UInt ID);
    UInt ClubToID(FifamClubLink const &clubLink);
    UInt PlayerToID(FifamPlayer const *player);
    UInt TranslateClubID(UInt ID, UInt gameFrom, UInt gameTo);
};
