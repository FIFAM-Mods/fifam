#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaStadium {
public:
    struct Internal {
        int yearbuilt = 0;
        int citycode = 0;
        int managerhometoplefty = 0;
        int managerawaytoplefty = 0;
        int subsawaybotrightx = 0;
        int subshomebotrightx = 0;
        int subsawaybotrighty = 0;
        int subsawaytopleftx = 0;
        int subshomebotrighty = 0;
        int subsawaytoplefty = 0;
        int managerhomebotrightx = 0;
        int managerawaytopleftx = 0;
        int managerhomebotrighty = 0;
        int managerhometopleftx = 0;
        int subshometoplefty = 0;
        int managerawaybotrightx = 0;
        int subshometopleftx = 0;
        int managerawaybotrighty = 0;

        int goalnetstanchioncolor2g = 0;
        int goalnetstanchioncolor2r = 0;
        int goalnetstanchioncolor1b = 0;
        int goalnetstanchioncolor2b = 0;
        int goalnetstanchioncolor1r = 0;
        std::wstring name;
        int goalnetstanchioncolor1g = 0;
        int timeofday3 = 0;
        int adboardsidelinedistance = 0;
        int stadiumgloalnetdepth = 0;
        int hometeamid = 0;
        int capacity = 0;
        int seatcolor = 0;
        int sectionfacedbydefault = 0;
        int stadhometechzonemaxz = 0;
        int policetypecode = 0;
        int timeofday4 = 0;
        int stadawaytechzonemaxx = 0;
        int stadiumpitchlength = 0;
        int stadiumgoalnetstyle = 0;
        int gmavailability = 0;
        int cityid = 0;
        int pitchtarps = 0;
        int upgradetier = 0;
        int tod4weather = 0;
        int countrycode = 0;
        int stadiummowpattern_code = 0;
        int stadhometechzonemaxx = 0;
        int colorregion = 0;
        int defaultseason = 0;
        int stadiumid = 0;
        int cameraheight = 0;
        int stadawaytechzoneminz = 0;
        int defaulttime = 0;
        int isstadiumavailable = 0;
        int stadhometechzoneminx = 0;
        int stadiumgoalnettype = 0;
        int stadiumpitchwidth = 0;
        int replay360degree = 0;
        int stadhometechzoneminz = 0;
        int inflatables = 0;
        int stadiumtype = 0;
        int islicensed = 0;
        int stadiumgoalnetpattern = 0;
        int stadawaytechzonemaxz = 0;
        int stadiumgoalnettension = 0;
        int tod1weather = 0;
        int dlc = 0;
        int tod2weather = 0;
        int adboardtype = 0;
        int rotation360degree = 0;
        int camerazoom = 0;
        int languageregion = 0;
        int adboardendlinedistance = 0;
        int iseditable = 0;
        int isdynamic = 0;
        int tod3weather = 0;
        int upgradestyle = 0;
        int defaultweather = 0;
        int genericrank = 0;
        int timeofday1 = 0;
        int timeofday2 = 0;
        int stadawaytechzoneminx = 0;
        int stadiumgoalnetwidth = 0;
        int stadiumgoalnetlength = 0;
        int goalnetads = 0;
        // @since FIFA19
        int hasunofficialname = 0;
        int hasmexicanwave = 0;
        int hascenteradboard = 0;
        int playsurfacetype = 0;
        int tifo = 0;
        // @since FIFA20
        int issmallsided = 0;
        int stadiumgoalpoststyle = 0;
        int supportsunlicensedbanners = 0;
        int supportsunlicensedtifos = 0;
        // @since FIFA21
        int presassetone = 0;
        int pitchcolor = 0;
        int pitchwear = 0;
        int presassetthree = 0;
        int pitchlinecolor = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    struct StadiumLink {
        std::wstring m_name;
        FifaTeam *m_team = nullptr;
    };

    std::wstring m_name;
    FifaTeam *m_homeTeam = nullptr;
    std::vector<StadiumLink> m_linkedTeams;
    unsigned int m_gameId = 0;
    
    FifaStadium(FifaDataFile::Line &line);
    unsigned int GetId();
    FifaStadium *RawPtr();
};
