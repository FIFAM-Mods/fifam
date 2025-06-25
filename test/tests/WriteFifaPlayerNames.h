#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"
#include "FifamNames.h"

class WriteFifaPlayerNames {
public:
    WriteFifaPlayerNames() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        FifamWriter w("fifa_player_names.csv");
        w.WriteLine(L"playerid", L"uid", L"name", L"firstName", L"lastName", L"commonName", L"birthdate", L"nation", L"version", L"gender");
        db->ForAllPlayers([&](FifaPlayer &p) {
            String name = p.m_commonName;
            if (name.empty()) {
                name = p.m_lastName;
                if (name.empty())
                    name = p.m_firstName;
                else
                    name += L", " + p.m_firstName;
            }
            String uid = FifamNames::GetPersonStringId(14, p.m_firstName, p.m_lastName, p.m_commonName, p.m_birthDate, 0);
            w.WriteLine(p.GetId(), uid, Quoted(name), p.m_firstName, p.m_lastName, p.m_commonName, p.m_birthDate.ToString(), p.internal.nationality, p.m_gameId, p.internal.gender);
        });
    }
};
