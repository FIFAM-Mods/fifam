#pragma once
#include <string>
#include <vector>
#include <ctime>

namespace Utils {
    std::wstring TimeString(std::time_t const &time);
    std::wstring TimeDifferenceSting(std::time_t const &timeEnd, std::time_t const &timeBegin);
    std::wstring CurrentTime();
    std::wstring GetQuickName(std::wstring const &firstName, std::wstring const &lastName, std::wstring const &commonName);
    std::wstring CharToStr(char c);

    template<typename ...ArgTypes>
    static std::wstring Format(const std::wstring &format, ArgTypes... args) {
        static wchar_t buf[4096];
        swprintf(buf, 4096, format.c_str(), args...);
        return buf;
    }

    std::vector<std::wstring> Split(std::wstring const &str, std::wstring const &delim, bool trim = true);
    bool StartsWith(std::wstring const &str, std::wstring const &what);
    bool EndsWith(std::wstring const &str, std::wstring const &what);
    bool Compare(std::wstring const &str, size_t index, char c);
    bool IsNumber(std::wstring const &str);
    int ToNumber(std::wstring const &str);
    void Trim(std::wstring &str);
    std::wstring ToUpper(std::wstring const &str);
    std::wstring ToLower(std::wstring const &str);
    std::wstring AtoW(std::string const &str);
    std::string WtoA(std::wstring const &str);
    void Replace(std::wstring& str, const std::wstring& from, const std::wstring& to);

    template<typename T>
    unsigned int ToInt(T value) {
        return static_cast<unsigned int>(value);
    }

    template<typename T>
    T FromInt(unsigned int value) {
        return static_cast<T>(value);
    }

    template<typename T>
    T SafeConvertInt(std::wstring const &str, bool isHex = false) {
        unsigned int result = 0;
        try {
            result = static_cast<T>(std::stoul(str, 0, isHex ? 16 : 10));
        }
        catch (...) {}
        return result;
    }

    float SafeConvertFloat(std::wstring const &str);
    double SafeConvertDouble(std::wstring const &str);
};
