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

    static Int convert_money(Int value);

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

    void ReaderCallback(String const &filename, Function<void(FifamReader &)> callback);

    enum class db_size { Full, Average, Small, Tiny };

    db(Path const &dbpath, bool readPersons = true, db_size playersCount = db_size::Full);
};

}
