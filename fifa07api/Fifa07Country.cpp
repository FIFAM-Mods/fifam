#include "Fifa07Country.h"
#include <sstream>
#include <string>

unsigned int Fifa07Country::GetId() {
    return countryid;
}

void Fifa07Country::Parse(Fifa07DataFile::Line &line) {
    line >> countryid >> countryname >> continentid;
}

Fifa07Country::Fifa07Country(Fifa07DataFile::Line &line) {
    Parse(line);
}
