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

Color &Color::operator=(Color const &rhs) {
    r = rhs.r;
    g = rhs.g;
    b = rhs.b;
    return *this;
}

bool Color::operator== (Color const &rhs) const {
    return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Color::operator!= (Color const &rhs) const {
    return r != rhs.r || g != rhs.g || b != rhs.b;
}

unsigned int Color::FindIndexInTable(std::vector<std::pair<unsigned int, Color>> const &table, unsigned int tableMin, unsigned int tableMax) const {
    if (tableMax == 0)
        tableMax = 9999;
    for (auto &clr : table) {
        if (clr.first >= tableMin && clr.first <= tableMax) {
            if (*this == clr.second)
                return clr.first;
        }
    }
    unsigned int result = 0;
    double dist = 9999.0;
    for (auto &clr : table) {
        if (clr.first >= tableMin && clr.first <= tableMax) {
            auto newDist = Distance(*this, clr.second);
            if (newDist < dist) {
                dist = newDist;
                result = clr.first;
            }
        }
    }
    return result;
}

void Color::SetFromTable(std::vector<std::pair<unsigned int, Color>> const &table, unsigned int index, unsigned int tableMin, unsigned int tableMax) {
    if (!table.empty()) {
        if (tableMax == 0)
            tableMax = 9999;
        if (index >= tableMin && index <= tableMax) {
            for (auto &clr : table) {
                if (clr.first == index) {
                    *this = clr.second;
                    return;
                }
            }
        }
    }
    Set(255, 255, 255);
}

std::wstring Color::ToStr() const {
    return Utils::Format(L"(%u,%u,%u)", r, g, b);
}

std::wstring Color::ToHexStr() const {
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

ColorPair &ColorPair::operator=(ColorPair const &rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
}

bool ColorPair::operator== (ColorPair const &rhs) const {
    return (first == rhs.first && second == rhs.second) || (first == rhs.second && second == rhs.first);
}

unsigned int ColorPair::FindIndexInTable(std::vector<std::pair<unsigned int, ColorPair>> const &table, unsigned int tableMin, unsigned int tableMax) const {
    if (tableMax == 0)
        tableMax = 9999;
    for (auto &clr : table) {
        if (clr.first >= tableMin && clr.first <= tableMax) {
            if (*this == clr.second)
                return clr.first;
        }
    }
    unsigned int result = 0;
    double dist = 9999.0;
    for (auto &clr : table) {
        if (clr.first >= tableMin && clr.first <= tableMax) {
            auto newDist = Distance(*this, clr.second);
            if (newDist < dist) {
                dist = newDist;
                result = clr.first;
            }
        }
    }
    return result;
}

void ColorPair::SetFromTable(std::vector<std::pair<unsigned int, ColorPair>> const &table, unsigned int index, unsigned int tableMin, unsigned int tableMax) {
    if (!table.empty()) {
        if (tableMax == 0)
            tableMax = 9999;
        if (index >= tableMin && index <= tableMax) {
            for (auto &clr : table) {
                if (clr.first == index) {
                    *this = clr.second;
                    return;
                }
            }
        }
    }
    Set(Color(), Color());
}

std::wstring ColorPair::ToStr() const {
    return Utils::Format(L"%s:%s", first.ToStr().c_str(), second.ToStr().c_str());
}

std::wstring ColorPair::ToHexStr() const {
    return Utils::Format(L"%s:%s", first.ToHexStr().c_str(), second.ToHexStr().c_str());
}
