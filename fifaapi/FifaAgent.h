#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaAgent {
public:
    struct Internal {
        std::wstring firstname;
        std::wstring commonname;
        std::wstring surname;
        int skintypecode = 0;
        int haircolorcode = 0;
        int facialhairtypecode = 0;
        int hairtypecode = 0;
        int agentid = 0;
        int headtypecode = 0;
        int height = 0;
        int seasonaloutfitid = 0;
        int weight = 0;
        int hashighqualityhead = 0;
        int gender = 0;
        int headassetid = 0;
        int ethnicity = 0;
        int faceposerpreset = 0;
        int eyebrowcode = 0;
        int eyecolorcode = 0;
        int personalityid = 0;
        int headclasscode = 0;
        int nationality = 0;
        int sideburnscode = 0;
        int headvariation = 0;
        int skintonecode = 0;
        int outfitid = 0;
        int hairstylecode = 0;
        int bodytypecode = 0;
        int facialhaircolorcode = 0;
        // @since FC25
        int lipcolor = 0;
        int skinsurfacepack = 0;
        int skinmakeup = 0;
        int eyedetail = 0;
        int skincomplexion = 0;
        // @since FC26
        int facepsdlayer0 = 0;
        int facepsdlayer1 = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_commonName;
    std::wstring m_quickName;
    unsigned int m_gameId = 0;

    FifaAgent(FifaDataFile::Line &line);
    unsigned int GetId();
    bool IsMale();
};
