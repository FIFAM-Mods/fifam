#pragma once

#include <vector>
#include <string>
#include "FifaTeam.h"
#include "FifaReferee.h"
#include "FifaDataFile.h"

class FifaLeague {
public:
    struct Internal {
        int buildupplay = 0;

        int countryid = 0;
        std::wstring leaguename;
        int level = 0;
        int iscompetitionscarfenabled = 0;
        int isbannerenabled = 0;
        int leagueid = 0;
        int iscompetitionpoleflagenabled = 0;
        int iscompetitioncrowdcardsenabled = 0;
        int leaguetimeslice = 0;
        int iswithintransferwindow = 0;
        // @since FIFA19
        int leaguetype = 0;
        // @since FIFA23
        int iswomencompetition = 0;
        // @since FC24
        int isinternationalleague = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_name;
    unsigned int m_countryId = 0;
    std::vector<FifaReferee *> m_referees;
    std::vector<FifaTeam *> m_teams;
    FifaTeam *m_champion = nullptr;
    unsigned int m_gameId = 0;

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
