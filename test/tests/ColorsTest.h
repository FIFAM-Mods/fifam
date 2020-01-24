#pragma once
#include "FifamDbEnvironment.h"
#include "Color.h"
#include "FifamKit.h"

class ColorsTest {
public:
    ColorsTest() {
        auto hex = [](UInt h) {
            ::Color result;
            result.Set(h & 0xFF, (h >> 8) & 0xFF, (h >> 16) & 0xFF);
            return result;
        };

        std::wcout << L"distance: " << ::Color::Distance(hex(0xd66267), hex(0xf95a60));
    }
};
