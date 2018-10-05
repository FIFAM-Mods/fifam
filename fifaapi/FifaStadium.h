#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaStadium {
public:
    struct Internal {
        int goalnetstanchioncolor2g;
        int goalnetstanchioncolor2r;
        int goalnetstanchioncolor1b;
        int goalnetstanchioncolor2b;
        int goalnetstanchioncolor1r;
        std::wstring name;
        int goalnetstanchioncolor1g;
        int timeofday3;
        int adboardsidelinedistance;
        int stadiumgloalnetdepth;
        int hometeamid;
        int capacity;
        int seatcolor;
        int sectionfacedbydefault;
        int stadhometechzonemaxz;
        int policetypecode;
        int timeofday4;
        int stadawaytechzonemaxx;
        int stadiumpitchlength;
        int stadiumgoalnetstyle;
        int gmavailability;
        int cityid;
        int pitchtarps;
        int upgradetier;
        int tod4weather;
        int countrycode;
        int stadiummowpattern_code;
        int stadhometechzonemaxx;
        int colorregion;
        int defaultseason;
        int stadiumid;
        int cameraheight;
        int stadawaytechzoneminz;
        int defaulttime;
        int isstadiumavailable;
        int stadhometechzoneminx;
        int stadiumgoalnettype;
        int stadiumpitchwidth;
        int replay360degree;
        int stadhometechzoneminz;
        int inflatables;
        int stadiumtype;
        int islicensed;
        int stadiumgoalnetpattern;
        int stadawaytechzonemaxz;
        int stadiumgoalnettension;
        int tod1weather;
        int dlc;
        int tod2weather;
        int adboardtype;
        int rotation360degree;
        int camerazoom;
        int languageregion;
        int adboardendlinedistance;
        int iseditable;
        int isdynamic;
        int tod3weather;
        int upgradestyle;
        int defaultweather;
        int genericrank;
        int timeofday1;
        int timeofday2;
        int stadawaytechzoneminx;
        int stadiumgoalnetwidth;
        int stadiumgoalnetlength;
        int goalnetads;
        // @since FIFA19
        int hasunofficialname;
        int hasmexicanwave;
        int hascenteradboard;
        int playsurfacetype;
        int tifo;

        void Read(FifaDataFile::Line &line);
    } internal;

    struct StadiumLink {
        std::wstring m_name;
        FifaTeam *m_team;
    };

    std::wstring m_name;
    FifaTeam *m_homeTeam;
    std::vector<StadiumLink> m_linkedTeams;
    
    FifaStadium(FifaDataFile::Line &line);
    unsigned int GetId();
    FifaStadium *RawPtr();
};
