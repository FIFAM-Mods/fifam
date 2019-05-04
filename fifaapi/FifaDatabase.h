#pragma once
#include "FifaVersion.h"
#include "FifaPlayer.h"
#include "FifaTeam.h"
#include "FifaLeague.h"
#include "FifaStadium.h"
#include "FifaReferee.h"
#include "FifaManager.h"
#include "FifaKit.h"
#include "FifaDataFile.h"
#include <map>
#include <functional>
#include <filesystem>

#define FIFA_DATABASE_LATEST_GAME_VERSION 19

class FifaDatabase {
public:
    FifaVersion m_version;
    std::map<unsigned int, FifaLeague *> m_leagues;
    std::map<unsigned int, FifaTeam *> m_teams;
    std::map<unsigned int, FifaPlayer *> m_players;
    std::map<unsigned int, FifaManager *> m_managers;
    std::map<unsigned int, FifaReferee *> m_referees;
    std::map<unsigned int, FifaStadium *> m_stadiums;
    std::map<unsigned int, FifaKit *> m_kits;

    std::filesystem::path m_path;

    static unsigned int m_currentGameVersion;
    static const unsigned int m_firstSupportedGameVersion = 18;
    static const unsigned int m_lastSupportedGameVersion = FIFA_DATABASE_LATEST_GAME_VERSION;

    FifaDatabase(std::filesystem::path const &path);
    ~FifaDatabase();

    template <typename T>
    void AddEntity(std::map<unsigned int, T *> &list, FifaDataFile::Line &line) {
        T *entity = new T(line);
        auto oldEntity = list.find(entity->GetId());
        if (oldEntity != list.end()) {
            delete list[entity->GetId()];
        }
        list[entity->GetId()] = entity;
    }

    template <typename T>
    T *GetFromList(std::map<unsigned int, T *> &list, unsigned int id) {
        auto entity = list.find(id);
        if (entity != list.end())
            return list[id];
        return nullptr;
    }

    template <typename T>
    void ClearList(std::map<unsigned int, T *> &list) {
        for (auto i : list)
            delete i.second;
    }

    std::wstring GetVersionString();
    std::wstring GetVersionDateString();
    FifaDate GetVersionDate();
    FifaLeague *GetLeague(unsigned int leagueId);
    FifaTeam *GetTeam(unsigned int teamId);
    FifaPlayer *GetPlayer(unsigned int playerId);
    FifaManager *GetManager(unsigned int managerId);
    FifaReferee *GetReferee(unsigned int refereeId);
    FifaStadium *GetStadium(unsigned int stadiumId);
    FifaKit *GetKit(unsigned int kitId);

    template <typename T>
    void ForAllLeagues(T callback) {
        for (auto const &it : m_leagues)
            callback(*it.second);
    }

    template <typename T>
    void ForAllPlayers(T callback) {
        for (auto const &it : m_players)
            callback(*it.second);
    }

    template <typename T>
    void ForAllMalePlayers(T callback) {
        for (auto const &it : m_players) {
            if (it.second->IsMale())
                callback(*it.second);
        }
    }

    template <typename T>
    void ForAllFemalePlayers(T callback) {
        for (auto const &it : m_players) {
            if (!it.second->IsMale())
                callback(*it.second);
        }
    }

    template <typename T>
    void ForAllTeams(T callback) {
        for (auto const &it : m_teams)
            callback(*it.second);
    }

    template <typename T>
    void ForAllReferees(T callback) {
        for (auto const &it : m_referees)
            callback(*it.second);
    }

    template <typename T>
    void ForAllStadiums(T callback) {
        for (auto const &it : m_stadiums)
            callback(*it.second);
    }

    template <typename T>
    void ForAllManagers(T callback) {
        for (auto const &it : m_managers)
            callback(*it.second);
    }

    template <typename T>
    void ForAllKits(T callback) {
        for (auto const &it : m_kits)
            callback(*it.second);
    }
};
