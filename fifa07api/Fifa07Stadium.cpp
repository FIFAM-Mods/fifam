#include "Fifa07Stadium.h"
#include "Fifa07Database.h"
#include <sstream>
#include <string>

unsigned int Fifa07Stadium::GetId() {
    return stadiumid;
}

void Fifa07Stadium::Parse(Fifa07DataFile::Line &line) {
    switch (Fifa07Database::m_currentGameVersion) {
    case 7:
        line >> stadiumid >> name >> hometeamid >> capacity >> yearbuilt >> seatcolor >> stadiumtype >> track >> netcolor >> mowpattern >> pitchsurroundcode >> shadowleft >> shadowright >> localizationcode >> hasovercast >> hasclearday >> hasnighttime >> hassunset >> hasretractableroof >> stadiumcondition; 
        break;
    case 8:
        line >> stadiumid >> name >> airportcode >> airportname >> hometeamid >> capacity >> yearbuilt >> seatcolor >> stadiumtype >> track >> netcolor >> mowpattern >> pitchsurroundcode >> shadowleft >> shadowright >> localizationcode >> hasovercast >> hasclearday >> hasnighttime >> hassunset >> hasretractableroof >> stadiumcondition;
        break;
    }
}

Fifa07Stadium::Fifa07Stadium(Fifa07DataFile::Line &line) {
    Parse(line);
}
