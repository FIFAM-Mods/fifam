#pragma once

#include <vector>
#include <string>
#include "Fifa07DataFile.h"

class Fifa07Kit {
public:
    int teamid = 0;
    int kittype = 0;
    int wetkit = 0;
    int wrinkle = 0;
    int collargeometry = 0;
    int nameplacement = 0;
    int namecolor = 0;
    int kitfont = 0;
    int jerseytype = 0;
    int jerseyfontsize = 0;
    int numberplacementback = 0;
    int jerseydoubledigitfontsize = 0;
    int numberplacementfront = 0;
    int numbercolor = 0;
    int jerseycolor1 = 0;
    int jerseycolor2 = 0;
    int jerseycolor3 = 0;
    int shorttype = 0;
    int shortcolor1 = 0;
    int shortcolor2 = 0;
    int shortcolor3 = 0;
    int shortnumberplacement = 0;
    int shortnumberfontsize = 0;
    int shortnumbercolor = 0;
    int shortnumberx = 0;
    int shortnumbery = 0;
    int socktype = 0;
    int sockcolor1 = 0;
    int sockcolor2 = 0;
    int sockcolor3 = 0;
    int jerseynumbercolorid = 0;
    int shortsnumbercolorid = 0;
    int shortsnumberhotspotid = 0;
    int kitclashavrhsv_h = 0;
    int kitclashavrhsv_s = 0;
    int kitclashavrhsv_v = 0;

    void Parse(Fifa07DataFile::Line &line);
    Fifa07Kit() {}
    Fifa07Kit(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
