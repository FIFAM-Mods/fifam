#include "Utils.h"
#include <chrono>
#include <ctime>

std::wstring Utils::TimeString(std::time_t const &time) {
    tm t = *localtime(&time);
    return Format(L"%02d.%02d.%04d %02d:%02d:%02d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900, t.tm_hour, t.tm_min, t.tm_sec);
}

std::wstring Utils::TimeDifferenceSting(std::time_t const &timeEnd, std::time_t const &timeBegin) {
    time_t seconds = static_cast<time_t>(difftime(timeEnd, timeBegin));
    tm *dt = gmtime(&seconds);
    if (dt->tm_yday > 0)
        return Format(L"%dd %dh %dm %ds", dt->tm_yday, dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_yday > 0)
        return Format(L"%dh %dm %ds", dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_yday > 0)
        return Format(L"%dm %ds", dt->tm_min, dt->tm_sec);
    return Format(L"%ds", dt->tm_sec);
}

std::wstring Utils::CurrentTime() {
    return TimeString(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

std::wstring Utils::GetQuickName(std::wstring const &firstName, std::wstring const &lastName, std::wstring const &commonName) {
    if (commonName.length() > 0)
        return commonName;
    if (firstName.length() > 0) {
        if (lastName.length() > 0)
            return firstName + L" " + lastName;
        else
            return firstName;
    }
    else {
        if (lastName.length() > 0)
            return lastName;
    }
    return std::wstring();
}

std::vector<std::wstring> Utils::Split(std::wstring const &str, std::wstring const &delim) {
    std::vector<std::wstring> result;
    if (str.length() > 0) {
        size_t b = 0;
        size_t e = str.find(delim);
        while (e != std::wstring::npos) {
            result.push_back(str.substr(b, e - b));
            b = e + delim.length();
            e = str.find(delim, b);
        }
        result.push_back(str.substr(b));
    }
    return result;
}

bool Utils::StartsWith(std::wstring const &str, std::wstring const &what) {
    return !str.compare(0, what.size(), what);
}

bool Utils::EndsWith(std::wstring const &str, std::wstring const &what) {
    auto strLen = str.length();
    auto whatLen = what.length();
    if (whatLen > strLen)
        return false;
    return str.substr(strLen - whatLen) == what;
}

std::wstring Utils::CharToStr(char c) {
    wchar_t str[2];
    str[0] = c;
    str[1] = 0;
    return str;
}
