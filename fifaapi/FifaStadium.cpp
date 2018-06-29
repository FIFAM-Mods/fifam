#include "FifaStadium.h"
#include <sstream>
#include <string>

unsigned int FifaStadium::GetId() {
    return internal.stadiumid;
}

FifaStadium *FifaStadium::RawPtr() {
    return this;
}

void FifaStadium::Internal::Read(FifaDataFile::Line &line) {
    line >> goalnetstanchioncolor2g >> goalnetstanchioncolor2r >> goalnetstanchioncolor1b >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> name >> goalnetstanchioncolor1g >> timeofday3 >> adboardsidelinedistance >> stadiumgloalnetdepth >> hometeamid >> capacity >> seatcolor >> sectionfacedbydefault >> stadhometechzonemaxz >> policetypecode >> timeofday4 >> stadawaytechzonemaxx >> stadiumpitchlength >> stadiumgoalnetstyle >> gmavailability >> cityid >> pitchtarps >> upgradetier >> tod4weather >> countrycode >> stadiummowpattern_code >> stadhometechzonemaxx >> colorregion >> defaultseason >> stadiumid >> cameraheight >> stadawaytechzoneminz >> defaulttime >> isstadiumavailable >> stadhometechzoneminx >> stadiumgoalnettype >> stadiumpitchwidth >> replay360degree >> stadhometechzoneminz >> inflatables >> stadiumtype >> islicensed >> stadiumgoalnetpattern >> stadawaytechzonemaxz >> stadiumgoalnettension >> tod1weather >> dlc >> tod2weather >> adboardtype >> rotation360degree >> camerazoom >> languageregion >> adboardendlinedistance >> iseditable >> isdynamic >> tod3weather >> upgradestyle >> defaultweather >> genericrank >> timeofday1 >> timeofday2 >> stadawaytechzoneminx >> stadiumgoalnetwidth >> stadiumgoalnetlength >> goalnetads;
}

FifaStadium::FifaStadium(FifaDataFile::Line &line) {
    internal.Read(line);
    m_name = internal.name;
    m_homeTeam = nullptr;
}