#include "Color.h"

Color::Color() {
    r = g = b = 255;
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue) {
    Set(red, green, blue);
}

void Color::Set(unsigned char red, unsigned char green, unsigned char blue) {
    r = red;
    g = green;
    b = blue;
}

double Color::Distance(Color const &e1, Color const &e2) {
    long rmean = ((long)e1.r + (long)e2.r) / 2;
    long r = (long)e1.r - (long)e2.r;
    long g = (long)e1.g - (long)e2.g;
    long b = (long)e1.b - (long)e2.b;
    return sqrt((((512 + rmean)*r*r) >> 8) + 4 * g*g + (((767 - rmean)*b*b) >> 8));
}

bool Color::operator== (Color const &rhs) {
    return r == rhs.r && g == rhs.g && b == rhs.b;
}

unsigned int Color::FindIndexInTable(std::vector<Color> const &table, unsigned int tableMin, unsigned int tableMax) {
    if (tableMax == 0 || tableMax >= table.size())
        tableMax = table.size() - 1;
    for (unsigned int i = tableMin; i <= tableMax; i++) {
        if (*this == table[i])
            return i;
    }
    unsigned int result = 0;
    double dist = 1000.0;
    for (unsigned int i = tableMin; i <= tableMax; i++) {
        auto newDist = Distance(*this, table[i]);
        if (newDist < dist) {
            dist = newDist;
            result = i;
        }
    }
    return result;
}

void Color::SetFromTable(std::vector<Color> const &table, unsigned int index, unsigned int tableMin, unsigned int tableMax) {
    if (!table.empty()) {
        if (tableMax == 0 || tableMax >= table.size())
            tableMax = table.size() - 1;
        if (index >= tableMin && index <= tableMax) {
            *this = table[index];
            return;
        }
    }
    Set(255, 255, 255);
}

std::wstring Color::ToStr() {
    return Utils::Format(L"(%u,%u,%u)", r, g, b);
}

std::wstring Color::ToHexStr() {
    return Utils::Format(L"#%02X%02X%02X", r, g, b);
}

ColorPair::ColorPair() {}

ColorPair::ColorPair(Color const &color1, Color const &color2) {
    Set(color1, color2);
}

void ColorPair::Set(Color const &color1, Color const &color2) {
    first = color1;
    second = color2;
}

double ColorPair::Distance(ColorPair const &e1, ColorPair const &e2) {
    auto dist1 = (Color::Distance(e1.first, e2.first) + Color::Distance(e1.second, e2.second)) / 2.0;
    auto dist2 = (Color::Distance(e1.first, e2.second) + Color::Distance(e1.second, e2.first)) / 2.0;
    return fmin(dist1, dist2);
}

bool ColorPair::operator== (ColorPair const &rhs) {
    return (first == rhs.first && second == rhs.second) || (first == rhs.second && second == rhs.first);
}

unsigned int ColorPair::FindIndexInTable(std::vector<ColorPair> const &table, unsigned int tableMin, unsigned int tableMax) {
    if (tableMax == 0 || tableMax >= table.size())
        tableMax = table.size() - 1;
    for (unsigned int i = tableMin; i <= tableMax; i++) {
        if (*this == table[i])
            return i;
    }
    unsigned int result = 0;
    double dist = 1000.0;
    for (unsigned int i = tableMin; i <= tableMax; i++) {
        auto newDist = Distance(*this, table[i]);
        if (newDist < dist) {
            dist = newDist;
            result = i;
        }
    }
    return result;
}

void ColorPair::SetFromTable(std::vector<ColorPair> const &table, unsigned int index, unsigned int tableMin, unsigned int tableMax) {
    if (!table.empty()) {
        if (tableMax == 0 || tableMax >= table.size())
            tableMax = table.size() - 1;
        if (index >= tableMin && index <= tableMax) {
            *this = table[index];
            return;
        }
    }
    Set(Color(), Color());
}

std::wstring ColorPair::ToStr() {
    return Utils::Format(L"%s:%s", first.ToStr().c_str(), second.ToStr().c_str());
}

std::wstring ColorPair::ToHexStr() {
    return Utils::Format(L"%s:%s", first.ToHexStr().c_str(), second.ToHexStr().c_str());
}
