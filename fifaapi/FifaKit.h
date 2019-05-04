#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaKit {
public:
    struct Internal {
        int shortsnumberplacementcode;
        int shortsnumbercolorprimg;
        int teamcolorsecb;
        int shortsrenderingdetailmaptype;
        int jerseyfrontnumberplacementcode;
        int jerseynumbercolorsecr;
        int jerseynumbercolorprimr;
        int jerseynumbercolorprimg;
        int shortsnumbercolorsecb;
        int teamcolorprimg;
        int shortsnumbercolorterb;
        int shortsnumbercolorprimr;
        int numberfonttype;
        int teamcolortertb;
        int jerseynumbercolorterg;
        int shortsnumbercolorprimb;
        int jerseynamefonttype;
        int jerseynamelayouttype;
        int jerseynumbercolorterr;
        int jerseynumbercolorprimb;
        int jerseyshapestyle;
        int jerseybacknameplacementcode;
        int teamcolorprimr;
        int jerseynamecolorg;
        int teamcolorsecg;
        int shortsnumbercolorsecg;
        int teamcolortertr;
        int jerseynumbercolorsecg;
        int renderingmaterialtype;
        int shortsnumbercolorterr;
        int teamcolorsecr;
        int jerseycollargeometrytype;
        int shortsnumbercolorterg;
        int jerseynamecolorr;
        int teamcolorprimb;
        int jerseyrenderingdetailmaptype;
        int shortsnumberfonttype;
        int jerseynumbercolorsecb;
        int jerseynamecolorb;
        int jerseynumbercolorterb;
        int teamcolortertg;
        int shortsnumbercolorsecr;
        int jerseybacknamefontcase;
        int chestbadge;
        int teamkittypetechid;
        int powid;
        int isinheritbasedetailmap;
        int islocked;
        int teamkitid;
        int teamcolorprimpercent;
        int jerseyrightsleevebadge;
        int teamcolorsecpercent;
        int jerseyleftsleevebadge;
        int year;
        int captainarmband;
        int teamtechid;
        int isembargoed;
        int hasadvertisingkit;
        int dlc;
        int teamcolortertpercent;
        int shortstyle;
        int jerseyfit;

        void Read(FifaDataFile::Line &line);
    } internal;

    FifaTeam *m_team;

    FifaKit(FifaDataFile::Line &line);
    unsigned int GetId();
};
