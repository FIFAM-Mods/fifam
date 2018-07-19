#pragma once

#include "FifamClub.h"
#include "FifamStadium.h"
#include "FifamSponsor.h"
#include "FifamReferee.h"
#include "FifamAssociation.h"
#include "FifamTerritory.h"

class FifamDatabase;

class FifamCountry {
public:
    // @since FM07
    // @maxsize 30
    TrArray<String> mName;

    // @since FM07
    // @maxsize 4
    TrArray<String> mAbbr;

    // @since FM07
    TrArray<Bool> mUseTheForName = {};

    // @since FM07
    FifamAssociation mAssociation = FifamAssociation::None;

    // @since FM07
    FifamLanguage mFirstLanguageForNames = FifamLanguage::None;

    // @since FM07
    FifamLanguage mSecondLanguageForNames = FifamLanguage::None;

    // @since FM07
    FifamTerritory mTerritory = FifamTerritory::None;

    // Country .sav files were read
    Bool mHasCountryData = false;

    // Country id (first country = 1)
    UInt mId = 0;

    // our database
    FifamDatabase *mDatabase = nullptr;

    Vector<FifamReferee *> mReferees;

    Vector<FifamStadium *> mStadiums;

    Vector<FifamSponsor *> mSponsors;

    FifamReferee *AddReferee();
    FifamStadium *AddStadium();
    FifamSponsor *AddSponsor();

    FifamCountry(UInt id, FifamDatabase *db);
    void Read(FifamReader &reader);
    void ReadFixtures(FifamReader &reader);
    void ReadScript(FifamReader &reader);
};
