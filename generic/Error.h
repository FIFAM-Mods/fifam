#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>

template <typename ...ArgTypes>
bool Error(std::string const &message, ArgTypes... args) {
    static char text[4096];
    sprintf_s(text, message.c_str(), args...);
    MessageBoxA(NULL, text, "Error", MB_ICONERROR);
    return false;
}

bool inline Message(std::string const &message) {
    MessageBoxA(NULL, message.c_str(), "Message", MB_ICONINFORMATION);
    return true;
}

template <typename ...ArgTypes>
bool Error(std::wstring const &message, ArgTypes... args) {
    static wchar_t text[4096];
    swprintf_s(text, message.c_str(), args...);
    MessageBoxW(NULL, text, L"Error", MB_ICONERROR);
    return false;
}

bool inline Message(std::wstring const &message) {
    MessageBoxW(NULL, message.c_str(), L"Message", MB_ICONINFORMATION);
    return true;
}
