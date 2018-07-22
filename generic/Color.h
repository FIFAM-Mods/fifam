#pragma once

struct ColorRGB {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;

    ColorRGB();
    ColorRGB(unsigned char red, unsigned char green, unsigned char blue);
};
