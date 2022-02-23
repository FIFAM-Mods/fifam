#pragma once
#include <string>
#include "Fifa07DataFile.h"
#include "Fifa07Propertied.h"

class Fifa07Country : public Fifa07Propertied {
public:
    int countryid = 0;
    std::wstring countryname;
    int continentid = 0;

    void Parse(Fifa07DataFile::Line &line);

    Fifa07Country() {}
    Fifa07Country(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
