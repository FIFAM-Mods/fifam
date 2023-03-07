#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaManager {
public:
    struct Internal {
        int birthdate = 0;
        int haireffecttypecode = 0;
        int nationalitycode = 0;

        std::wstring firstname;
        std::wstring surname;
        int managerid = 0;
        int headid = 0;
        int height = 0;
        int hashighqualityhead = 0;
        int gender = 0;
        int suitvariationid = 0;
        int teamid = 0;
        int suittypeid = 0;
        int eyecolorcode = 0;
        int headclasscode = 0;
        int skintonecode = 0;
        int bodytypecode = 0;
        // @since FIFA19
        int faceposerpreset = 0;
        // @since FIFA20
        std::wstring commonname;
        int skintypecode = 0;
        int haircolorcode = 0;
        int facialhairtypecode = 0;
        int hairtypecode = 0;
        int headtypecode = 0;
        int seasonaloutfitid = 0;
        int weight = 0;
        int headassetid = 0;
        int ethnicity = 0;
        int eyebrowcode = 0;
        int personalityid = 0;
        int nationality = 0;
        int sideburnscode = 0;
        int headvariation = 0;
        int outfitid = 0;
        int hairstylecode = 0;
        int facialhaircolorcode = 0;
        // @since FIFA23
        int managerjointeamdate = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_commonName;
    std::wstring m_quickName;
    FifaTeam *m_team = nullptr;
    unsigned int m_gameId = 0;

    FifaManager(FifaDataFile::Line &line);
    unsigned int GetId();
    FifaManager *RawPtr();
    bool HasRealName();
    bool IsMale();
};
