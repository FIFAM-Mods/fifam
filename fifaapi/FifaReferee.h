#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaLeague;

class FifaReferee {
public:
    struct Internal {
        std::wstring firstname;
        std::wstring surname;
        int haircolorcode;
        int facialhairtypecode;
        int hairtypecode;
        int cardstrictness;
        int shoecolorcode2;
        int headtypecode;
        int foulstrictness;
        int height;
        int leagueid;
        int shoetypecode;
        int birthdate;
        int sockheightcode;
        int socklengthcode;
        int weight;
        int gender;
        int eyebrowcode;
        int eyecolorcode;
        int jerseysleevelengthcode;
        int headclasscode;
        int sideburnscode;
        int skintypecode;
        int isreal;
        int headvariation;
        int skintonecode;
        int shortstyle;
        int refereeid;
        int nationalitycode;
        int shoedesigncode;
        int shoecolorcode1;
        int hairstylecode;
        int bodytypecode;
        int facialhaircolorcode;
        // @since FIFA19
        int headassetid;
        int faceposerpreset;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_quickName;
    FifaLeague *m_mainLeague;
    std::vector<FifaLeague *> m_linkedLeagues;

    FifaReferee(FifaDataFile::Line &line);
    unsigned int GetId();
    bool IsMale();
    bool IsReal();
};
