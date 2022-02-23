#pragma once

#include <vector>
#include <string>
#include "Fifa07DataFile.h"
#include "Fifa07Propertied.h"

class Fifa07League : public Fifa07Propertied {
public:
    int leagueid = 0;
    std::wstring leaguename;
    int level = 0;
    int countryid = 0;
    int leagueprestige = 0;
    
    int final_stadium = 0; // written only when not 0
    std::vector<int> teams;

    void Parse(Fifa07DataFile::Line &line);
    Fifa07League() {}
    Fifa07League(Fifa07DataFile::Line &line);
    Fifa07League(int _leagueid, std::wstring const &_leaguename, int _level, int _countryid, int _leagueprestige);
    unsigned int GetId();
};
