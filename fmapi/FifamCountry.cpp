#include "FifamCountry.h"
#include "FifamDatabase.h"
#include "Error.h"

FifamCountry::FifamCountry(UInt id, FifamDatabase *db) {
    mId = id;
    mDatabase = db;
}

FifamReferee *FifamCountry::AddReferee() {
    FifamReferee *referee = new FifamReferee;
    mReferees.push_back(referee);
    if (mDatabase)
        mDatabase->mReferees.insert(referee);
    return referee;
}

FifamStadium *FifamCountry::AddStadium() {
    FifamStadium *stadium = new FifamStadium;
    mStadiums.push_back(stadium);
    if (mDatabase)
        mDatabase->mStadiums.insert(stadium);
    return stadium;
}

FifamSponsor *FifamCountry::AddSponsor() {
    FifamSponsor *sponsor = new FifamSponsor;
    mSponsors.push_back(sponsor);
    if (mDatabase)
        mDatabase->mSponsors.insert(sponsor);
    return sponsor;
}

void FifamCountry::Read(FifamReader &reader) {
    return;
    if (reader.ReadStartIndex(L"COUNTRY")) {

        reader.ReadVersion();

        if (reader.ReadStartIndex(L"COMPETITIONPART"))
            reader.ReadEndIndex(L"COMPETITIONPART");

        if (reader.ReadStartIndex(L"COMPETITION")) {
            // TODO
            reader.ReadEndIndex(L"COMPETITION");
        }

        if (reader.ReadStartIndex(L"CLUBS")) {
            // TODO
            reader.ReadEndIndex(L"CLUBS");
        }

        if (reader.ReadStartIndex(L"REFEREES")) {
            auto numReferees = reader.ReadLine<UInt>();
            for (UInt i = 0; i < numReferees; i++)
                AddReferee()->Read(reader);
            reader.ReadEndIndex(L"REFEREES");
        }

        if (reader.ReadStartIndex(L"SPONSORS")) {
            auto numStadiums = reader.ReadLine<UInt>();
            for (UInt i = 0; i < numStadiums; i++)
                AddSponsor()->Read(reader);
            reader.ReadEndIndex(L"SPONSORS");
        }

        if (reader.ReadStartIndex(L"STAFFS"))
            reader.ReadEndIndex(L"STAFFS");

        if (reader.ReadStartIndex(L"STADIUMS")) {
            auto numSponsors = reader.ReadLine<UInt>();
            for (UInt i = 0; i < numSponsors; i++)
                AddStadium()->Read(reader);
            reader.ReadEndIndex(L"STADIUMS");
        }

        if (reader.ReadStartIndex(L"MANAGER"))
            reader.ReadEndIndex(L"MANAGER");

        if (reader.ReadStartIndex(L"ADDMANAGER")) {
            // TODO
            reader.ReadEndIndex(L"ADDMANAGER");
        }

        if (reader.ReadStartIndex(L"PLAYERPOOL")) {
            // TODO
            reader.ReadEndIndex(L"PLAYERPOOL");
        }

        if (reader.ReadStartIndex(L"COUNTRY_MIST")) {
            // TODO
            reader.ReadEndIndex(L"COUNTRY_MIST");
        }

        reader.ReadEndIndex(L"COUNTRY");
    }
}

void FifamCountry::ReadFixtures(FifamReader &reader) {

}

void FifamCountry::ReadScript(FifamReader &reader) {

}
