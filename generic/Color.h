#pragma once
#include <vector>
#include <utility>
#include "Utils.h"

struct Color {
    unsigned char r, g, b;
    Color();
    Color(unsigned char red, unsigned char green, unsigned char blue);
    Color(unsigned int intValue);
    void Set(unsigned char red, unsigned char green, unsigned char blue);
    void Set(unsigned int intValue);
    static double Distance(Color const &e1, Color const &e2);
    Color &operator=(Color const &rhs);
    bool operator== (Color const &rhs) const;
    bool operator!= (Color const &rhs) const;
    unsigned int FindIndexInTable(std::vector<std::pair<unsigned int, Color>> const &table, unsigned int tableMin = 0, unsigned int tableMax = 0) const;
    void SetFromTable(std::vector<std::pair<unsigned int, Color>> const &table, unsigned int index, unsigned int tableMin = 0, unsigned int tableMax = 0);
    std::wstring ToStr() const;
    std::wstring ToHexStr() const;
    std::string ToStrA() const;
    std::string ToHexStrA() const;
    unsigned int ToInt() const;
};

bool operator<(::Color const &a, ::Color const &b);

using GenColor = ::Color;

struct ColorPair : public std::pair<::Color, ::Color> {
    ColorPair();
    ColorPair(::Color const &color1, ::Color const &color2);
    void Set(::Color const &color1, ::Color const &color2);
    static double Distance(ColorPair const &e1, ColorPair const &e2);
    ColorPair &operator=(ColorPair const &rhs);
    bool operator== (ColorPair const &rhs) const;
    unsigned int FindIndexInTable(std::vector<std::pair<unsigned int, ColorPair>> const &table, unsigned int tableMin = 0, unsigned int tableMax = 0) const;
    void SetFromTable(std::vector<std::pair<unsigned int, ColorPair>> const &table, unsigned int index, unsigned int tableMin = 0, unsigned int tableMax = 0);
    std::wstring ToStr() const;
    std::wstring ToHexStr() const;
    std::string ToStrA() const;
    std::string ToHexStrA() const;
};

using GenColorPair = ::ColorPair;
