#pragma once
#include "FifamCountry.h"
#include "FifamCupAlloc.h"
#include "FifamClubLink.h"
#include "FifamRules.h"
#include "FifamHistoric.h"

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
    Vector<FifamStaff *> mStaffs; // TODO: replace with Set
    Set<FifamStadium *> mStadiums;
    Set<FifamSponsor *> mSponsors;
    Vector<FifamCupAlloc *> mCupTemplates;
    Set<FifamCACPlayer *> mCACPlayers;
    FifamRules mRules;
    FifamHistoric mHistoric;

    using NamesMap = Map<FifamLanguage, Set<String>>;
    NamesMap mMaleNames;
    NamesMap mFemaleNames;
    NamesMap mSurnames;

    Map<FifamCompID, FifamCompetition *> mCompMap;

    FifamDatabase();
    FifamDatabase(UInt gameId, const Path &dbPath);
    UInt GetInternalGameCountryId(UInt gameId, UChar nationId);
    Bool IsCountryPresent(UInt gameId, UChar nationId);
    void Read(UInt gameId, Path const &dbPath);
    void Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath);
    void Clear();
    ~FifamDatabase();
    FifamClub *CreateClub(FifamCountry *country);
    void AddClubToMap(FifamClub *club);
    FifamPlayer *CreatePlayer(FifamClub *club, UInt id);
    FifamStaff *CreateStaff(FifamClub *club, UInt id);

    void ResolveLinksForClub(FifamClub *club, UInt gameId);
    void ResolveLinksForPlayer(FifamPlayer *player, UInt gameId);
    void ResolveLinksForStaff(FifamStaff *staff, UInt gameId);
    FifamClubLink ClubFromID(UInt ID);
    FifamPlayer *PlayerFromID(UInt ID);
    UInt ClubToID(FifamClubLink const &clubLink);
    UInt PlayerToID(FifamPlayer const *player);
    UInt TranslateClubID(UInt ID, UInt gameFrom, UInt gameTo);
    void ResolveClubLink(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    void ResolvePlayerLink(FifamPlayer *&player);
    FifamClub *GetClubFromUID(UInt uid);
    void GetClubFromUID(FifamClubLink &link, UInt uid);
    UInt GetNextFreePersonID();
    void ReadNamesFile(Path const &filepath, UInt gameId, NamesMap &outNames);
    void WriteNamesFile(Path const &filepath, UInt gameId, NamesMap &names);
};
