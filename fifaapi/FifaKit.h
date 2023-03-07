#pragma once

#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaKit {
public:
    struct Internal {
        int shortsnumbercolor = 0;
        int numbercolor = 0;

        int shortsnumberplacementcode = 0;
        int shortsnumbercolorprimg = 0;
        int teamcolorsecb = 0;
        int shortsrenderingdetailmaptype = 0;
        int jerseyfrontnumberplacementcode = 0;
        int jerseynumbercolorsecr = 0;
        int jerseynumbercolorprimr = 0;
        int jerseynumbercolorprimg = 0;
        int shortsnumbercolorsecb = 0;
        int teamcolorprimg = 0;
        int shortsnumbercolorterb = 0;
        int shortsnumbercolorprimr = 0;
        int numberfonttype = 0;
        int teamcolortertb = 0;
        int jerseynumbercolorterg = 0;
        int shortsnumbercolorprimb = 0;
        int jerseynamefonttype = 0;
        int jerseynamelayouttype = 0;
        int jerseynumbercolorterr = 0;
        int jerseynumbercolorprimb = 0;
        int jerseyshapestyle = 0;
        int jerseybacknameplacementcode = 0;
        int teamcolorprimr = 0;
        int jerseynamecolorg = 0;
        int teamcolorsecg = 0;
        int shortsnumbercolorsecg = 0;
        int teamcolortertr = 0;
        int jerseynumbercolorsecg = 0;
        int renderingmaterialtype = 0;
        int shortsnumbercolorterr = 0;
        int teamcolorsecr = 0;
        int jerseycollargeometrytype = 0;
        int shortsnumbercolorterg = 0;
        int jerseynamecolorr = 0;
        int teamcolorprimb = 0;
        int jerseyrenderingdetailmaptype = 0;
        int shortsnumberfonttype = 0;
        int jerseynumbercolorsecb = 0;
        int jerseynamecolorb = 0;
        int jerseynumbercolorterb = 0;
        int teamcolortertg = 0;
        int shortsnumbercolorsecr = 0;
        int jerseybacknamefontcase = 0;
        int chestbadge = 0;
        int teamkittypetechid = 0;
        int powid = 0;
        int isinheritbasedetailmap = 0;
        int islocked = 0;
        int teamkitid = 0;
        int teamcolorprimpercent = 0;
        int jerseyrightsleevebadge = 0;
        int teamcolorsecpercent = 0;
        int jerseyleftsleevebadge = 0;
        int year = 0;
        int captainarmband = 0;
        int teamtechid = 0;
        int isembargoed = 0;
        int hasadvertisingkit = 0;
        int dlc = 0;
        int teamcolortertpercent = 0;
        int shortstyle = 0;
        int jerseyfit = 0;
        // @since FIFA23:
        int armbandtype = 0;
        int jerseyrestriction = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    FifaTeam *m_team = nullptr;
    unsigned int m_gameId = 0;

    FifaKit(FifaDataFile::Line &line);
    unsigned int GetId();
};
