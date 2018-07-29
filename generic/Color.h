#pragma once
#include <vector>
#include <utility>
#include "Utils.h"

struct Color {
    unsigned char r, g, b;
    Color();
    Color(unsigned char red, unsigned char green, unsigned char blue);
    void Set(unsigned char red, unsigned char green, unsigned char blue);
    static double Distance(Color const &e1, Color const &e2);
    bool operator== (Color const &rhs);
    unsigned int FindIndexInTable(std::vector<Color> const &table, unsigned int tableMin = 0, unsigned int tableMax = 0);
    void SetFromTable(std::vector<Color> const &table, unsigned int index, unsigned int tableMin = 0, unsigned int tableMax = 0);
    std::wstring ToStr();
    std::wstring ToHexStr();
};

struct ColorPair : public std::pair<Color, Color> {
    ColorPair();
    ColorPair(Color const &color1, Color const &color2);
    void Set(Color const &color1, Color const &color2);
    static double Distance(ColorPair const &e1, ColorPair const &e2);
    bool operator== (ColorPair const &rhs);
    unsigned int FindIndexInTable(std::vector<ColorPair> const &table, unsigned int tableMin = 0, unsigned int tableMax = 0);
    void SetFromTable(std::vector<ColorPair> const &table, unsigned int index, unsigned int tableMin = 0, unsigned int tableMax = 0);
    std::wstring ToStr();
    std::wstring ToHexStr();
};
