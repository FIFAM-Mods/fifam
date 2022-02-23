#include "Fifa07Formation.h"
#include "Fifa07Database.h"
#include <sstream>
#include <string>
#include "Utils.h"

unsigned int Fifa07Formation::GetId() {
    return formationid;
}

void Fifa07Formation::Parse(Fifa07DataFile::Line &line) {
    switch (Fifa07Database::m_currentGameVersion) {
    case 7:
        line >> formationid >> formationname >> position1 >> position2 >> position3 >> position4 >> position5 >> position6 >> position7 >> position8 >> position9 >> position10 >> position11 >> position12 >> position13;
        break;
    case 8:
        line >> formationid >> formationname >> position1 >> position2 >> position3 >> position4 >> position5 >> position6 >> position7 >> position8 >> position9 >> position10 >> position11 >> position12 >> position13 >> offset1x >> offset1y >> offset2x >> offset2y >> offset3x >> offset3y >> offset4x >> offset4y >> offset5x >> offset5y >> offset6x >> offset6y >> offset7x >> offset7y >> offset8x >> offset8y >> offset9x >> offset9y >> offset10x >> offset10y >> offset11x >> offset11y >> offensivedir0_1 >> offensivedir0_2 >> offensivedir0_3 >> offensivedir0_4 >> offensivedir0_5 >> offensivedir0_6 >> offensivedir0_7 >> offensivedir0_8 >> offensivedir0_9 >> offensivedir0_10 >> offensivedir0_11 >> offensivedir1_1 >> offensivedir1_2 >> offensivedir1_3 >> offensivedir1_4 >> offensivedir1_5 >> offensivedir1_6 >> offensivedir1_7 >> offensivedir1_8 >> offensivedir1_9 >> offensivedir1_10 >> offensivedir1_11 >> defensivedir0_1 >> defensivedir0_2 >> defensivedir0_3 >> defensivedir0_4 >> defensivedir0_5 >> defensivedir0_6 >> defensivedir0_7 >> defensivedir0_8 >> defensivedir0_9 >> defensivedir0_10 >> defensivedir0_11 >> defensivedir1_1 >> defensivedir1_2 >> defensivedir1_3 >> defensivedir1_4 >> defensivedir1_5 >> defensivedir1_6 >> defensivedir1_7 >> defensivedir1_8 >> defensivedir1_9 >> defensivedir1_10 >> defensivedir1_11;
        break;
    }
}

Fifa07Formation::Fifa07Formation(Fifa07DataFile::Line &line) {
    Parse(line);
}
