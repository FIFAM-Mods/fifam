#pragma once

#include <vector>
#include <string>
#include "FifaTeam.h"
#include "FifaReferee.h"
#include "FifaDataFile.h"

class FifaLeague {
public:
    struct Internal {
        int countryid;
        std::wstring leaguename;
        int level;
        int iscompetitionscarfenabled;
        int isbannerenabled;
        int leagueid;
        int iscompetitionpoleflagenabled;
        int iscompetitioncrowdcardsenabled;
        int leaguetimeslice;
        int iswithintransferwindow;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_name;
    unsigned int m_countryId;
    std::vector<FifaReferee *> m_referees;
    std::vector<FifaTeam *> m_teams;
    FifaTeam *m_champion;

    FifaLeague(FifaDataFile::Line &line);
    unsigned int GetId();
    bool IsRestOfWorld();
    bool IsManInternational();
    bool IsWomanInternational();
    bool IsInternational();
    bool IsFreeAgents();
    bool IsNationalLeague();

    template <typename T>
    void ForAllTeams(T callback) {
        for (FifaTeam *team : m_teams)
            callback(*team);
    }

    template <typename T>
    void ForAllReferees(T callback) {
        for (FifaReferee *referee : m_referees)
            callback(*referee);
    }
};
