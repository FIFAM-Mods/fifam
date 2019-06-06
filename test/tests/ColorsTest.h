#pragma once
#include "FifamDbEnvironment.h"
#include "Color.h"
#include "FifamKit.h"

class ColorsTest {
public:
    ColorsTest() {
        ::Color green = { 0, 128, 96 }; // green

        std::wcout << L"color: " << green.FindIndexInTable(FifamKit::mShirtNumberColorTable) << std::endl;
    }
};
