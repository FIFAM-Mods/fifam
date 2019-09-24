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
        // @since FIFA19
        int faceposerpreset;
        // @since FIFA20
        std::wstring commonname;
        int skintypecode;
        int haircolorcode;
        int facialhairtypecode;
        int hairtypecode;
        int headtypecode;
        int seasonaloutfitid;
        int weight;
        int headassetid;
        int ethnicity;
        int eyebrowcode;
        int personalityid;
        int nationality;
        int sideburnscode;
        int headvariation;
        int outfitid;
        int hairstylecode;
        int facialhaircolorcode;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_commonName;
    std::wstring m_quickName;
    FifaTeam *m_team;

    FifaManager(FifaDataFile::Line &line);
    unsigned int GetId();
    FifaManager *RawPtr();
    bool HasRealName();
    bool IsMale();
};
