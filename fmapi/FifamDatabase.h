#pragma once
#include "FifamPropertied.h"
#include "FifamCountry.h"
#include "FifamCupAlloc.h"
#include "FifamClubLink.h"
#include "FifamRules.h"
#include "FifamHistoric.h"
#include "FifamCompetition.h"

/*
    Minimum supported version: FM07 0x2007.0x0C
*/

class FifamDatabase : public FifamPropertied {
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
    Set<FifamCACPlayer *> mCACPlayers;
    FifamRules mRules;
    FifamHistoric mHistoric;

    using NamesMap = Map<FifamLanguage, Set<String>>;
    NamesMap mMaleNames;
    NamesMap mFemaleNames;
    NamesMap mSurnames;

    Map<FifamCompID, FifamCompetition *> mCompMap;

    static struct ReadingOptions {
        Bool mReadCountriesData = true;
        Bool mReadCountryCompetitions = true;
        Bool mReadInternationalCompetitions = true;
        Bool mReadClubs = true;
        Bool mReadPersons = true;
        UInt mGameVersionForScripts = 0;
        Bool mUseCustomFormations = false;
    } mReadingOptions;

    static struct WritingOptions {
        Bool mNonWriteablePlayersAreFreeAgents = false;
        Bool mNonWriteableStaffsAreFreeAgents = false;
        Bool mWriteAssessment = true;
        Bool mWriteFixtures = true;
        Bool mWriteExternalScripts = true;
        UInt mMaxClubsInCountry[8] = { 512, 512, 512, 1024, 1024, 1024, 1024, 1024 };
        UInt mMaxPersonsInClub[8] = { 60, 96, 99, 99, 99, 99, 256, 256 };
        Map<FifamNation, Pair<FifamNation, FifamNation>> mCountryTranslationMap;
    } mWritingOptions;

    static FifamVersion GetGameDbVersion(UInt gameId);
    static Bool IsUnicodeUsedInGameVersion(UInt gameId);

    FifamDatabase();
    FifamDatabase(UInt gameId, const Path &dbPath);
    UInt GetInternalGameCountryId(UInt gameId, UChar nationId);
    Bool IsCountryPresent(UInt gameId, UChar nationId);
    void Read(UInt gameId, Path const &dbPath);
    void Write(UInt gameId, FifamVersion const &version, Path const &dbPath);
    void Clear();
    ~FifamDatabase();
    FifamCountry *CreateCountry(UInt id);
    FifamClub *CreateClub(FifamCountry *country);
    void AddClubToMap(FifamClub *club);
    FifamPlayer *CreatePlayer(FifamClub *club, UInt id);
    FifamStaff *CreateStaff(FifamClub *club, UInt id);
    FifamCompetition *CreateCompetition(FifamCompDbType dbType, FifamCompID const &compID, String const &name = L"Competition");

    void ResolveLinksForClub(FifamClub *club, UInt gameId);
    void ResolveLinksForPlayer(FifamPlayer *player, UInt gameId);
    void ResolveLinksForStaff(FifamStaff *staff, UInt gameId);
    void ResolveLinksForCompetition(FifamCompetition *comp, UInt gameId);
    FifamClubLink ClubFromID(UInt ID);
    FifamPlayer *PlayerFromID(UInt ID);
    FifamStadium *StadiumFromID(UInt ID);
    UInt ClubToID(FifamClubLink const &clubLink);
    UInt PlayerToID(FifamPlayer const *player);
    UInt StadiumToID(FifamStadium const *stadium);
    UInt TranslateCountryEntityID(UInt ID, UInt gameFrom, UInt gameTo);
    void ResolveClubUniqueID(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    void ResolveClubUniqueID(UInt &uid, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    void ResolveClubLink(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    void ResolvePlayerPtr(FifamPlayer *&player);
    void ResolveCompetitionID(FifamCompID &comp, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    void ResolveClubLinkList(Vector<FifamClubLink> &vec, UInt gameId, bool unique = true);
    void ResolveCompetitionList(Vector<FifamCompID> &vec, UInt gameId, bool unique = true);
    void ResolveStadiumPtr(FifamStadium *&stadium, UInt gameFrom, UInt gameTo = LATEST_GAME_VERSION);
    FifamClub *GetClubFromUID(UInt uid, Bool includeNationalTeams = true);
    void GetClubFromUID(FifamClubLink &link, UInt uid, Bool includeNationalTeams = true);
    FifamCompetition *GetCompetition(FifamCompID const &compID);
    UInt GetNextFreePersonID();
    void ReadNamesFile(Path const &filepath, UInt gameId, NamesMap &outNames);
    void WriteNamesFile(Path const &filepath, UInt gameId, NamesMap &names);
    void SetupWriteableStatus(UInt gameId, Bool generateEmpicsIDs, Vector<Pair<FifamPlayer *, UInt>> &newEmpicsIDs, Vector<Pair<FifamPlayer *, UInt>> &removedEmpicsIDs);
    void SetupWriteableStatus(UInt gameId, Bool generateEmpicsIDs = true);
    void ResetWriteableStatus();
    FifamCompetition *ReadCompetition(FifamReader &reader, FifamNation nationId);
    void WriteCompetition(FifamWriter &writer, FifamCompetition *comp, FifamNation nationId);
    void ReadExternalScriptFile(Path const &filepath, String const &compKeyName, UInt gameId);
    void WriteExternalScriptFile(Path const &filepath, String const &compKeyName, UInt gameId, Vector<FifamCompEntry> const &comps, UInt startIndex);
    UInt GetClubsInCountryLimit(UInt gameId);
    UInt GetPersonsInClubLimit(UInt gameId);
    FifamCupAlloc *GetCupTemplate(FifamCupSystemType cupSystemType);
    FifamCountry *GetCountry(Int countryId);
    void ReadPlayerRelations(Path const &filepath, UInt gameId, Map<String, Vector<FifamPlayer *>> const &playerStrIDsMap);
    void WritePlayerRelations(Path const &filepath, UInt gameId);
    void RecalculatePersonIDs();
};
