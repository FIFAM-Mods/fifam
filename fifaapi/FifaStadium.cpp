#include "FifaStadium.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>

unsigned int FifaStadium::GetId() {
    return internal.stadiumid;
}

FifaStadium *FifaStadium::RawPtr() {
    return this;
}

void FifaStadium::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 11:
        line >> yearbuilt >> stadiumid >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> policetypecode >> timeofday4 >> stadiumpitchlength >> stadiumgoalnetstyle >> tod4weather >> countrycode >> stadiummowpattern_code >> citycode >> stadiumpitchwidth >> stadiumtype >> tod1weather >> dlc >> tod2weather >> adboardendlinedistance >> tod3weather >> timeofday1 >> timeofday2 >> stadiumgoalnetwidth >> stadiumgoalnetlength;
        break;
    case 12:
        line >> yearbuilt >> stadiumid >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> policetypecode >> timeofday4 >> stadiumpitchlength >> stadiumgoalnetstyle >> tod4weather >> countrycode >> stadiummowpattern_code >> cameraheight >> stadiumgoalnettype >> citycode >> stadiumpitchwidth >> stadiumtype >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> camerazoom >> adboardendlinedistance >> iseditable >> tod3weather >> genericrank >> timeofday1 >> timeofday2 >> stadiumgoalnetwidth >> stadiumgoalnetlength;
        break;
    case 13:
        line >> yearbuilt >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> managerhometoplefty >> capacity >> seatcolor >> managerawaytoplefty >> subsawaybotrightx >> sectionfacedbydefault >> subshomebotrightx >> policetypecode >> subsawaybotrighty >> timeofday4 >> subsawaytopleftx >> stadiumpitchlength >> stadiumgoalnetstyle >> subshomebotrighty >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> subsawaytoplefty >> managerhomebotrightx >> stadiumid >> cameraheight >> managerawaytopleftx >> managerhomebotrighty >> stadiumgoalnettype >> stadiumpitchwidth >> stadiumtype >> managerhometopleftx >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> camerazoom >> adboardendlinedistance >> subshometoplefty >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> genericrank >> timeofday1 >> timeofday2 >> stadiumgoalnetwidth >> managerawaybotrightx >> subshometopleftx >> stadiumgoalnetlength >> managerawaybotrighty;
        break;
    case 14:
        line >> yearbuilt >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> managerhometoplefty >> capacity >> seatcolor >> managerawaytoplefty >> subsawaybotrightx >> sectionfacedbydefault >> subshomebotrightx >> policetypecode >> subsawaybotrighty >> timeofday4 >> subsawaytopleftx >> stadiumpitchlength >> stadiumgoalnetstyle >> subshomebotrighty >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> subsawaytoplefty >> managerhomebotrightx >> stadiumid >> cameraheight >> managerawaytopleftx >> managerhomebotrighty >> stadiumgoalnettype >> stadiumpitchwidth >> stadiumtype >> managerhometopleftx >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> camerazoom >> adboardendlinedistance >> subshometoplefty >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> genericrank >> timeofday1 >> timeofday2 >> stadiumgoalnetwidth >> managerawaybotrightx >> subshometopleftx >> stadiumgoalnetlength >> managerawaybotrighty;
        break;
    case 15:
        line >> yearbuilt >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> policetypecode >> timeofday4 >> stadiumpitchlength >> stadiumgoalnetstyle >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadiumid >> cameraheight >> stadiumgoalnettype >> stadiumpitchwidth >> stadiumtype >> stadiumgoalnetpattern >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> camerazoom >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> genericrank >> timeofday1 >> timeofday2 >> stadiumgoalnetwidth >> stadiumgoalnetlength;
        break;
    case 16:
        line >> yearbuilt >> name >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> stadiumid >> cameraheight >> stadawaytechzoneminz >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> stadhometechzoneminz >> stadiumtype >> islicensed >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> camerazoom >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> genericrank >> timeofday1 >> timeofday2 >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength;
        break;
    case 17:
        line >> yearbuilt >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> stadiumid >> cameraheight >> stadawaytechzoneminz >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> stadhometechzoneminz >> stadiumtype >> islicensed >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> camerazoom >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> genericrank >> timeofday1 >> timeofday2 >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength;
        break;
    case 18:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> cameraheight >> stadawaytechzoneminz >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> timeofday2 >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength >> goalnetads;
        break;
    case 19:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> cameraheight >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> timeofday2 >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength >> goalnetads;
        break;
    case 20:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> issmallsided >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> cameraheight >> stadiumgoalpoststyle >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength >> goalnetads;
        break;
    case 21:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> presassetone >> issmallsided >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> pitchcolor >> policetypecode >> timeofday4 >> pitchwear >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> cameraheight >> stadiumgoalpoststyle >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> presassetthree >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> pitchlinecolor >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength >> goalnetads;
        break;
    case 22:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> presassetfour >> presassetone >> issmallsided >> supportsgoalnetshape >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> pitchcolor >> policetypecode >> timeofday4 >> pitchwear >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> tunnellocation >> cameraheight >> stadiumgoalpoststyle >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> ismirrored >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> presassetthree >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> pitchlinecolor >> stadawaytechzoneminx >> goalnetads;
        break;
    case 23:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> presassetfour >> presassetone >> issmallsided >> supportsgoalnetshape >> dronetype >> capacity >> hasintrosnownightvideo >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> pitchcolor >> policetypecode >> timeofday4 >> pitchwear >> stadawaytechzonemaxx >> stadiumpitchlength >> hasintrorainnightvideo >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> tunnellocation >> cameraheight >> stadiumgoalpoststyle >> hasintrodrydayvideo >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> ismirrored >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> hasintroraindayvideo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> hasintrodrynightvideo >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> presassetthree >> iseditable >> isdynamic >> tod3weather >> frontsettype >> hasintrosnowdayvideo >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> pitchlinecolor >> stadawaytechzoneminx >> hasintrovideo >> goalnetads;
        break;
    case 24:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> presassetfour >> presassetone >> issmallsided >> supportsgoalnetshape >> dronetype >> capacity >> hasintrosnownightvideo >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> pitchcolor >> policetypecode >> timeofday4 >> pitchwear >> stadawaytechzonemaxx >> stadiumpitchlength >> hasintrorainnightvideo >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> hasdroneshots >> tunnellocation >> cameraheight >> stadiumgoalpoststyle >> hasintrodrydayvideo >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> ismirrored >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> hasintroraindayvideo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> hasintrodrynightvideo >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> presassetthree >> iseditable >> isdynamic >> tod3weather >> frontsettype >> hasintrosnowdayvideo >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> pitchlinecolor >> stadawaytechzoneminx >> hasintrovideo >> goalnetads;
        break;
    case 25:
        line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> presassetfour >> presassetone >> issmallsided >> supportsgoalnetshape >> dronetype >> capacity >> hasintrosnownightvideo >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> hasunofficialname >> pitchcolor >> policetypecode >> timeofday4 >> pitchwear >> stadawaytechzonemaxx >> stadiumpitchlength >> hasintrorainnightvideo >> stadiumgoalnetstyle >> gmavailability >> cityid >> playsurfacetype >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> hasdroneshots >> tunnellocation >> cameraheight >> stadiumgoalpoststyle >> hasintrodrydayvideo >> stadawaytechzoneminz >> hasmexicanwave >> hascenteradboard >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> ismirrored >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> tifo >> hasintroraindayvideo >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> hasintrodrynightvideo >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> presassetthree >> iseditable >> isdynamic >> tod3weather >> frontsettype >> hasintrosnowdayvideo >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> supportsunlicensedbanners >> timeofday2 >> supportsunlicensedtifos >> pitchlinecolor >> stadawaytechzoneminx >> hasintrovideo >> enhancedcrowdcardsupport >> goalnetads;
        break;
    }
}

FifaStadium::FifaStadium(FifaDataFile::Line &line) {
    internal.Read(line);
    m_name = internal.name;
    m_homeTeam = nullptr;
}
