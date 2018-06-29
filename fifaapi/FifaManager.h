#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaManager {
public:
    struct Internal {
        std::wstring firstname;
        std::wstring surname;
        int managerid;
        int headid;
        int height;
        int hashighqualityhead;
        int gender;
        int suitvariationid;
        int teamid;
        int suittypeid;
        int eyecolorcode;
        int headclasscode;
        int skintonecode;
        int bodytypecode;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_quickName;
    FifaTeam *m_team;

    FifaManager(FifaDataFile::Line &line);
    unsigned int GetId();
    FifaManager *RawPtr();
    bool HasRealName();
    bool IsMale();
};