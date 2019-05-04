#include "FifaKit.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>

void FifaKit::Internal::Read(FifaDataFile::Line & line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 19:
        line >> shortsnumberplacementcode >> shortsnumbercolorprimg >> teamcolorsecb >> shortsrenderingdetailmaptype >> jerseyfrontnumberplacementcode >> jerseynumbercolorsecr >> jerseynumbercolorprimr >> jerseynumbercolorprimg >> shortsnumbercolorsecb >> teamcolorprimg >> shortsnumbercolorterb >> shortsnumbercolorprimr >> numberfonttype >> teamcolortertb >> jerseynumbercolorterg >> shortsnumbercolorprimb >> jerseynamefonttype >> jerseynamelayouttype >> jerseynumbercolorterr >> jerseynumbercolorprimb >> jerseyshapestyle >> jerseybacknameplacementcode >> teamcolorprimr >> jerseynamecolorg >> teamcolorsecg >> shortsnumbercolorsecg >> teamcolortertr >> jerseynumbercolorsecg >> renderingmaterialtype >> shortsnumbercolorterr >> teamcolorsecr >> jerseycollargeometrytype >> shortsnumbercolorterg >> jerseynamecolorr >> teamcolorprimb >> jerseyrenderingdetailmaptype >> shortsnumberfonttype >> jerseynumbercolorsecb >> jerseynamecolorb >> jerseynumbercolorterb >> teamcolortertg >> shortsnumbercolorsecr >> jerseybacknamefontcase >> chestbadge >> teamkittypetechid >> powid >> isinheritbasedetailmap >> islocked >> teamkitid >> teamcolorprimpercent >> jerseyrightsleevebadge >> teamcolorsecpercent >> jerseyleftsleevebadge >> year >> captainarmband >> teamtechid >> isembargoed >> hasadvertisingkit >> dlc >> teamcolortertpercent >> shortstyle >> jerseyfit;
        break;
    }
}

FifaKit::FifaKit(FifaDataFile::Line & line) {
    internal.Read(line);
    m_team = nullptr;
}

unsigned int FifaKit::GetId() {
    return internal.teamkitid;
}
