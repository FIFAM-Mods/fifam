#pragma once
#include "FifamReadWrite.h"
#include "db_map.h"
#include "foom_nation.h"
#include "foom_comp.h"
#include "foom_club.h"
#include "foom_language.h"
#include "foom_region.h"
#include "foom_injury.h"
#include "foom_official.h"
#include "foom_player.h"
#include "foom_non_player.h"
#include "foom_derby.h"
#include "foom_city.h"
#include "foom_stadium.h"
#include "foom_stage_name.h"
#include <iostream>

namespace foom {

struct db {
    Path mDbPath;
    db_map<language> mLanguages;
    db_map<injury> mInjuries;
    db_map<stage_name> mStageNames;
    db_map<stadium> mStadiums;
    db_map<derby> mDerbies;
    db_map<city> mCities;
    db_map<comp> mComps;
    db_map<nation> mNations;
    db_map<region> mRegions;
    db_map<club> mClubs;
    db_map<player> mPlayers;
    db_map<non_player> mNonPlayers;
    db_map<official> mOfficials;

    template<typename T> T *get(Int id) { return nullptr; }
    template<> nation *get<nation>(Int id) { return map_find_ptr(mNations, id); }
    template<> city *get<city>(Int id) { return map_find_ptr(mCities, id); }
    template<> club *get<club>(Int id) { return map_find_ptr(mClubs, id); }
    template<> comp *get<comp>(Int id) { return map_find_ptr(mComps, id); }
    template<> language *get<language>(Int id) { return map_find_ptr(mLanguages, id); }
    template<> injury *get<injury>(Int id) { return map_find_ptr(mInjuries, id); }
    template<> stage_name *get<stage_name>(Int id) { return map_find_ptr(mStageNames, id); }
    template<> stadium *get<stadium>(Int id) { return map_find_ptr(mStadiums, id); }
    template<> derby *get<derby>(Int id) { return map_find_ptr(mDerbies, id); }
    template<> region *get<region>(Int id) { return map_find_ptr(mRegions, id); }
    template<> player *get<player>(Int id) { return map_find_ptr(mPlayers, id); }
    template<> non_player *get<non_player>(Int id) { return map_find_ptr(mNonPlayers, id); }
    template<> official *get<official>(Int id) { return map_find_ptr(mOfficials, id); }

    template<> team *get<team>(Int id) {
        team *result = map_find_ptr(mNations, id);
        if (!result)
            result = map_find_ptr(mClubs, id);
        return result;
    }

    template<> person *get<person>(Int id) {
        person *result = map_find_ptr(mPlayers, id);
        if (!result)
            result = map_find_ptr(mNonPlayers, id);
        return result;
    }

    template<typename T>
    void resolve(T *&entity) {
        Int id = (Int)entity;
        if (id == -1)
            entity = nullptr;
        else
            entity = get<T>(id);
    }

    void ReaderCallback(String const &filename, Function<void(FifamReader &)> callback) {
        std::wcout << L"Reading " << filename << L"..." << std::endl;
        FifamReader reader(mDbPath / (filename + L".csv"), 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine())
                    callback(reader);
                else
                    reader.SkipLine();
            }
        }
    }

    db(Path const &dbpath) {
        mDbPath = dbpath;
        std::wcout << L"Reading foom database..." << std::endl;
        // read nations
        ReaderCallback(L"fm_nations", [&](FifamReader &reader) {
            nation n;
            reader.ReadLine(n.mID, n.mName, n.mShortName, n.mNickname, n.mHashtag, n.mThreeLetterName, n.mNationalityName, n.mContinent, IntPtr(n.mCapitalCity), IntPtr(n.mNationalStadium), n.mReputation, n.mAttendance, n.mMinimumAttendance, n.mMaximumAttendance, n.mYearsToGainNationality, n.mFIFAMember, n.mYearAssociationFormed, n.mRankingPoints, n.mYouthRating, n.mEuroCoeff1, n.mEuroCoeff2, n.mEuroCoeff3, n.mEuroCoeff4, n.mEuroCoeff5, n.mEuroCoeff6, n.mPreferredFormation, n.mSecondPreferredFormation, IntPtr(n.mCaptain), IntPtr(n.mViceCaptain));
            mNations[n.mID] = n;
        });
        ReaderCallback(L"fm_nation_kits", [&](FifamReader &reader) {
            Int nationID = -1;
            kit k;
            reader.ReadLine(nationID, k.mKit, k.mType, k.mKitStyle, k.mBackOfShirtStyle, k.mForeground.r, k.mForeground.g, k.mForeground.b, k.mBackground.r, k.mBackground.g, k.mBackground.b, k.mOutline.r, k.mOutline.g, k.mOutline.b, k.mNumberColour.r, k.mNumberColour.g, k.mNumberColour.b, k.mNumberOutlineColour.r, k.mNumberOutlineColour.g, k.mNumberOutlineColour.b, IntPtr(k.mCompetition), k.mYear, k.mAlternativeKitNumber);
            map_find(mNations, nationID).mVecKits.push_back(k);
        });
        ReaderCallback(L"fm_nation_languages", [&](FifamReader &reader) {
            Int nationID = -1;
            nation::language l;
            reader.ReadLine(nationID, IntPtr(l.mLanguage), l.mPercentage);
            map_find(mNations, nationID).mVecLanguages.push_back(l);
        });
        ReaderCallback(L"fm_nation_region_transfer_preferences", [&](FifamReader &reader) {
            Int nationID = -1;
            nation::region_transfer_preference r;
            reader.ReadLine(nationID, IntPtr(r.mRegion), r.mRating, r.mMinimumAge, r.mMaximumAge);
            map_find(mNations, nationID).mVecRegionTransferPreferences.push_back(r);
        });
        // read competitions
        ReaderCallback(L"fm_comps", [&](FifamReader &reader) {
            comp c;
            reader.ReadLine(c.mID, c.mName, c.mShortName, IntPtr(c.mNation), c.mContinent, IntPtr(c.mParentCompetition), c.mReputation, c.mUsesSeatedStadiumsOnly, c.mIsExtinct, c.mType, c.mCompetitionLevel);
            mComps[c.mID] = c;
        });
        ReaderCallback(L"fm_comp_teams", [&](FifamReader &reader) {
            Int compID = -1;
            team *teamID = nullptr;
            reader.ReadLine(compID, IntPtr(teamID));
            map_find(mComps, compID).mVecTeams.push_back(teamID);
        });
        ReaderCallback(L"fm_comp_history", [&](FifamReader &reader) {
            Int compID;
            comp::history h;
            reader.ReadLine(compID, h.mYear, IntPtr(h.mFirstPlaced), IntPtr(h.mSecondPlaced), IntPtr(h.mThirdPlaced), IntPtr(h.mPlayOffWinner), IntPtr(h.mStageName), h.mYearOrder, h.mNoDataForYear);
            map_find(mComps, compID).mVecHistory.push_back(h);
        });
        // read clubs
        ReaderCallback(L"fm_clubs", [&](FifamReader &reader) {
            club c;
            reader.ReadLine(c.mID, c.mName, c.mShortName, c.mSixLetterName, c.mNickname, c.mHashtag, c.mExtinct, IntPtr(c.mNation), IntPtr(c.mBasedNation), IntPtr(c.mContinentalCupNation), IntPtr(c.mCity), c.mYearFounded, c.mStatus, c.mReputation, c.mLatitude, c.mLongitude, c.mChairmanTitle, c.mOwnershipType, IntPtr(c.mStadium), c.mAttendance, c.mMinimumAttendance, c.mMaximumAttendance, c.mTraining, c.mYouthCoaching, c.mYouthFacilities, c.mYouthRecruitment, c.mYouthImportance, c.mCorporateFacilities, c.mBalance, c.mTransferBudget, c.mSugarDaddy, c.mNumberOfSeasonTicketHolders, IntPtr(c.mDivision), IntPtr(c.mLastDivision), c.mLastPosition, IntPtr(c.mNextDivision), c.mLastPositionForNextDivision, IntPtr(c.mSecondaryDivision), c.mEuroCoeff1, c.mEuroCoeff2, c.mEuroCoeff3, c.mEuroCoeff4, c.mEuroCoeff5, c.mPreferredFormation, c.mSecondPreferredFormation, IntPtr(c.mRecordWinOpposition), c.mRecordWinTG, c.mRecordWinOG, c.mRecordWinYear, IntPtr(c.mRecordDefeatOpposition), c.mRecordDefeatTG, c.mRecordDefeatOG, c.mRecordDefeatYear, IntPtr(c.mRecordAttendanceOpposition), c.mRecordAttendance, c.mRecordAttendanceYear, IntPtr(c.mCaptain), IntPtr(c.mViceCaptain));
            mClubs[c.mID] = c;
        });
        // TODO

        // read players
        // read non players
        // read officials
        ReaderCallback(L"fm_officials", [&](FifamReader &reader) {
            official o;
            reader.ReadLine(o.mID, o.mFirstName, o.mSecondName, o.mCommonName, o.mFullName, o.mFemale, IntPtr(o.mNation), o.mCurrentAbility, o.mPotentialAbility, o.mReputation, o.mUEFACategory, o.mFIFACategory, o.mContinentalOfficial, o.mAllowingFlow, o.mDiscipline, o.mImportantMatches, o.mPressure, o.mRefereeing, o.mRunningLine, o.mTimeKeeping);
            mOfficials[o.mID] = o;
        });
        // read regions
        ReaderCallback(L"fm_regions", [&](FifamReader &reader) {
            region r;
            reader.ReadLine(r.mID, r.mName);
            mRegions[r.mID] = r;
        });
        // read cities
        ReaderCallback(L"fm_cities", [&](FifamReader &reader) {
            city c;
            reader.ReadLine(c.mID, c.mName, IntPtr(c.mNation), c.mInhabitants, c.mLatitude, c.mLongitude);
            mCities[c.mID] = c;
        });
        // read stadiums
        ReaderCallback(L"fm_stadiums", [&](FifamReader &reader) {
            stadium s;
            reader.ReadLine(s.mID, s.mName, IntPtr(s.mNation), IntPtr(s.mCity), IntPtr(s.mOwner), s.mCapacity, s.mSeatingCapacity, s.mLatitude, s.mLongitude, s.mUsedByNationalTeam);
            mStadiums[s.mID] = s;
        });
        // read derbies
        ReaderCallback(L"fm_derbies", [&](FifamReader &reader) {
            derby d;
            reader.ReadLine(d.mID, d.mName, d.mShortName, d.mAlternativeName, d.mAlternativeShortName, IntPtr(d.mTeam1), IntPtr(d.mTeam2), d.mWorldReputation, d.mNationalReputation);
            mDerbies[d.mID] = d;
        });
        // read languages
        ReaderCallback(L"fm_languages", [&](FifamReader &reader) {
            language l;
            reader.ReadLine(l.mID, l.mName, IntPtr(l.mNation));
            mLanguages[l.mID] = l;
        });
        // read injuries
        ReaderCallback(L"fm_injuries", [&](FifamReader &reader) {
            injury i;
            reader.ReadLine(i.mID, i.mName, i.mSentenceName, i.mCategory, i.mMinimumDays, i.mExtraDays, i.mInactiveRatio, i.mOccuranceRatio, i.mMatchInjuryPercentage, i.mGoalkeeperPercentage, i.mCentralDefenderPercentage, i.mRecurring, i.mContagious, i.mCause, i.mSeriousness, i.mAgeRule, i.mSeason, i.mTreatmentByPhysion, i.mTreatmentBySpecialist, i.mTreatmentByInjection, i.mTreatmentByIsolation, i.mAllowToPlayWithProtectiveEquipment);
            mInjuries[i.mID] = i;
        });
        // read stage names
        ReaderCallback(L"fm_stagenames", [&](FifamReader &reader) {
            stage_name s;
            reader.ReadLine(s.mID, s.mName, s.mShortName, s.mPluralName);
            mStageNames[s.mID] = s;
        });

        // resolving
        std::wcout << L"Resolving foom database links..." << std::endl;
        for (auto &entry : mNations) {
            nation &n = entry.second;
            resolve(n.mCapitalCity);
            resolve(n.mNationalStadium);
            resolve(n.mCaptain);
            resolve(n.mViceCaptain);
            for (auto &l : n.mVecLanguages)
                resolve(l.mLanguage);
            for (auto &r : n.mVecRegionTransferPreferences)
                resolve(r.mRegion);
            for (auto &k : n.mVecKits)
                resolve(k.mCompetition);
        }
        for (auto &entry : mComps) {
            comp &c = entry.second;
            resolve(c.mNation);
            resolve(c.mParentCompetition);
            for (auto &t : c.mVecTeams)
                resolve(t);
            for (auto &h : c.mVecHistory) {
                resolve(h.mFirstPlaced);
                resolve(h.mSecondPlaced);
                resolve(h.mThirdPlaced);
                resolve(h.mPlayOffWinner);
                resolve(h.mStageName);
            }
        }
        for (auto &entry : mClubs) {
            club &c = entry.second;
            resolve(c.mNation);
            resolve(c.mBasedNation);
            resolve(c.mContinentalCupNation);
            resolve(c.mCity);
            resolve(c.mStadium);
            resolve(c.mDivision);
            resolve(c.mLastDivision);
            resolve(c.mNextDivision);
            resolve(c.mSecondaryDivision);
            resolve(c.mRecordWinOpposition);
            resolve(c.mRecordDefeatOpposition);
            resolve(c.mRecordAttendanceOpposition);
            resolve(c.mCaptain);
            resolve(c.mViceCaptain);

        }
        for (auto &entry : mOfficials) {
            official &o = entry.second;
            resolve(o.mNation);
        }
        for (auto &entry : mCities) {
            city &c = entry.second;
            resolve(c.mNation);
        }
        for (auto &entry : mStadiums) {
            stadium &s = entry.second;
            resolve(s.mNation);
            resolve(s.mCity);
            resolve(s.mOwner);
        }
        for (auto &entry : mDerbies) {
            derby &d = entry.second;
            resolve(d.mTeam1);
            resolve(d.mTeam2);
        }
        for (auto &entry : mLanguages) {
            language &l = entry.second;
            resolve(l.mNation);
        }
    }
};

}
