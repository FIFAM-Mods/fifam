#include "Fifa07Referee.h"
#include "Fifa07Database.h"
#include <sstream>
#include <string>
#include "Utils.h"

unsigned int Fifa07Referee::GetId() {
    return refereeid;
}

void Fifa07Referee::Parse(Fifa07DataFile::Line &line) {
    line >> refereeid >> firstname >> surname >> refereeheadid >> isinternationalreferee >> nationality >> facetypeid >> haircolorid >> hairtypeid >> facialhairstyleid >> lowmedhairtypeid;
}

Fifa07Referee::Fifa07Referee(Fifa07DataFile::Line &line) {
    Parse(line);
}
