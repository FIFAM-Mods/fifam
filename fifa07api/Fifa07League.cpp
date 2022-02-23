#include "Fifa07League.h"
#include <sstream>
#include <string>

unsigned int Fifa07League::GetId() {
    return leagueid;
}

void Fifa07League::Parse(Fifa07DataFile::Line &line) {
    line >> leagueid >> leaguename >> level >> countryid >> leagueprestige;
}

Fifa07League::Fifa07League(Fifa07DataFile::Line &line) {
    Parse(line);
}

Fifa07League::Fifa07League(int _leagueid, std::wstring const &_leaguename, int _level, int _countryid, int _leagueprestige) {
    leagueid = _leagueid;
    leaguename = _leaguename;
    level = _level;
    countryid = _countryid;
    leagueprestige = _leagueprestige;
}
