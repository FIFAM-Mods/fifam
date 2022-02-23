#pragma once
#include <string>
#include "Fifa07DataFile.h"

class Fifa07Formation {
public:
    int formationid = 0;
    std::wstring formationname;
    int position1 = 0;
    int position2 = 0;
    int position3 = 0;
    int position4 = 0;
    int position5 = 0;
    int position6 = 0;
    int position7 = 0;
    int position8 = 0;
    int position9 = 0;
    int position10 = 0;
    int position11 = 0;
    int position12 = 0;
    int position13 = 0;
    // 08
    int offset1x = 0;
    int offset1y = 0;
    int offset2x = 0;
    int offset2y = 0;
    int offset3x = 0;
    int offset3y = 0;
    int offset4x = 0;
    int offset4y = 0;
    int offset5x = 0;
    int offset5y = 0;
    int offset6x = 0;
    int offset6y = 0;
    int offset7x = 0;
    int offset7y = 0;
    int offset8x = 0;
    int offset8y = 0;
    int offset9x = 0;
    int offset9y = 0;
    int offset10x = 0;
    int offset10y = 0;
    int offset11x = 0;
    int offset11y = 0;
    int offensivedir0_1 = 0;
    int offensivedir0_2 = 0;
    int offensivedir0_3 = 0;
    int offensivedir0_4 = 0;
    int offensivedir0_5 = 0;
    int offensivedir0_6 = 0;
    int offensivedir0_7 = 0;
    int offensivedir0_8 = 0;
    int offensivedir0_9 = 0;
    int offensivedir0_10 = 0;
    int offensivedir0_11 = 0;
    int offensivedir1_1 = 0;
    int offensivedir1_2 = 0;
    int offensivedir1_3 = 0;
    int offensivedir1_4 = 0;
    int offensivedir1_5 = 0;
    int offensivedir1_6 = 0;
    int offensivedir1_7 = 0;
    int offensivedir1_8 = 0;
    int offensivedir1_9 = 0;
    int offensivedir1_10 = 0;
    int offensivedir1_11 = 0;
    int defensivedir0_1 = 0;
    int defensivedir0_2 = 0;
    int defensivedir0_3 = 0;
    int defensivedir0_4 = 0;
    int defensivedir0_5 = 0;
    int defensivedir0_6 = 0;
    int defensivedir0_7 = 0;
    int defensivedir0_8 = 0;
    int defensivedir0_9 = 0;
    int defensivedir0_10 = 0;
    int defensivedir0_11 = 0;
    int defensivedir1_1 = 0;
    int defensivedir1_2 = 0;
    int defensivedir1_3 = 0;
    int defensivedir1_4 = 0;
    int defensivedir1_5 = 0;
    int defensivedir1_6 = 0;
    int defensivedir1_7 = 0;
    int defensivedir1_8 = 0;
    int defensivedir1_9 = 0;
    int defensivedir1_10 = 0;
    int defensivedir1_11 = 0;
    
    void Parse(Fifa07DataFile::Line &line);

    Fifa07Formation() {}
    Fifa07Formation(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
