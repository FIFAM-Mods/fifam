#pragma once
#include <iostream>
#include <chrono>
#include "Utils.h"

template<typename Test>
void RunTest() {
    std::string testName = typeid(Test).name();
    if (!testName.compare(0, 6, "class "))
        testName = testName.substr(6);
    auto tBegin = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::wcout << Utils::TimeString(tBegin) << " Running test: \"" << testName.c_str() << "\"" <<  std::endl;
    {
        Test test;
    }
    auto tEnd = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::wcout << std::endl << Utils::TimeString(tEnd) << " Finished test \"" << testName.c_str() <<
        "\", total time: " <<Utils::TimeDifferenceSting(tEnd, tBegin) << std::endl << std::endl;
}
