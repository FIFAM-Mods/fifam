#pragma once
#include <string>
#include "Fifa07DataFile.h"
#include "Fifa07Propertied.h"

class Fifa07Country;

class Fifa07Referee : public Fifa07Propertied {
public:
    int refereeid = 0;
    std::wstring firstname;
    std::wstring surname;
    int refereeheadid = 0;
    int isinternationalreferee = 0;
    int nationality = 0;
    int facetypeid = 0;
    int haircolorid = 0;
    int hairtypeid = 0;
    int facialhairstyleid = 0;
    int lowmedhairtypeid = 0;
    
    void Parse(Fifa07DataFile::Line &line);

    Fifa07Country *country = nullptr;

    Fifa07Referee() {}
    Fifa07Referee(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
