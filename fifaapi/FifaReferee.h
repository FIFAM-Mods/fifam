#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaLeague;

class FifaReferee {
public:
    struct Internal {
        int lowmedhairtypeid = 0;
        int stylecode = 0;
        int proxyhaircolorid = 0;
        int hairstateid = 0;
        int proxyheadclass = 0;
        int hairlinecode = 0;
        int haireffecttypecode = 0;
        int hairpartcode = 0;
        int sweatid = 0;
        int hairvariationid = 0;
        int homecitycode = 0;
        int wrinkleid = 0;

        std::wstring firstname;
        std::wstring surname;
        int haircolorcode = 0;
        int facialhairtypecode = 0;
        int hairtypecode = 0;
        int cardstrictness = 0;
        int shoecolorcode2 = 0;
        int headtypecode = 0;
        int foulstrictness = 0;
        int height = 0;
        int leagueid = 0;
        int shoetypecode = 0;
        int birthdate = 0;
        int sockheightcode = 0;
        int socklengthcode = 0;
        int weight = 0;
        int gender = 0;
        int eyebrowcode = 0;
        int eyecolorcode = 0;
        int jerseysleevelengthcode = 0;
        int headclasscode = 0;
        int sideburnscode = 0;
        int skintypecode = 0;
        int isreal = 0;
        int headvariation = 0;
        int skintonecode = 0;
        int shortstyle = 0;
        int refereeid = 0;
        int nationalitycode = 0;
        int shoedesigncode = 0;
        int shoecolorcode1 = 0;
        int hairstylecode = 0;
        int bodytypecode = 0;
        int facialhaircolorcode = 0;
        // @since FIFA19
        int headassetid = 0;
        int faceposerpreset = 0;
        // @since FIFA20
        int smallsidedshoetypecode = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_quickName;
    FifaLeague *m_mainLeague = nullptr;
    std::vector<FifaLeague *> m_linkedLeagues;
    unsigned int m_gameId = 0;

    FifaReferee(FifaDataFile::Line &line);
    unsigned int GetId();
    bool IsMale();
    bool IsReal();
};
