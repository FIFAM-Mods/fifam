#pragma once
#include <string>
#include "Fifa07DataFile.h"

class Fifa07Stadium {
public:
    int stadiumid = 0;
    std::wstring name;
    int hometeamid = 0;
    int capacity = 0;
    int yearbuilt = 0;
    int seatcolor = 0;
    int stadiumtype = 0;
    int track = 0;
    int netcolor = 0;
    int mowpattern = 0;
    int pitchsurroundcode = 0;
    int shadowleft = 0;
    int shadowright = 0;
    int localizationcode = 0;
    int hasovercast = 0;
    int hasclearday = 0;
    int hasnighttime = 0;
    int hassunset = 0;
    int hasretractableroof = 0;
    int stadiumcondition = 0;
    // 08
    std::wstring airportcode;
    std::wstring airportname;
    
    void Parse(Fifa07DataFile::Line &line);

    Fifa07Stadium() {}
    Fifa07Stadium(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
