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

std::vector<std::wstring> Utils::Split(std::wstring const &str, std::wstring const &delim, bool trim) {
    std::vector<std::wstring> result;
    if (str.length() > 0) {
        size_t b = 0;
        size_t e = str.find(delim);
        std::wstring res;
        while (e != std::wstring::npos) {
            res = str.substr(b, e - b);
            if (trim)
                Trim(res);
            result.push_back(res);
            b = e + delim.length();
            e = str.find(delim, b);
        }
        res = str.substr(b);
        if (trim)
            Trim(res);
        result.push_back(res);
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


bool Utils::Compare(std::wstring const &str, size_t index, char c) {
    return str.length() > index && str[index] == c;
}

bool Utils::IsNumber(std::wstring const &str) {
    if (str.empty())
        return false;
    for (wchar_t c : str) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int Utils::ToNumber(std::wstring const &str) {
    std::wstring trimmed = str;
    Trim(trimmed);
    return (StartsWith(trimmed, L"0x") ? wcstol(trimmed.substr(2).c_str(), nullptr, 16) : wcstol(trimmed.c_str(), nullptr, 10));
}

void Utils::Trim(std::wstring &str) {
    size_t start = str.find_first_not_of(L" \t\r\n");
    if (start != std::wstring::npos)
        str = str.substr(start);
    size_t end = str.find_last_not_of(L" \t\r\n");
    if (end != std::wstring::npos)
        str = str.substr(0, end + 1);
}

std::wstring Utils::ToUpper(std::wstring const &str) {
    std::wstring result;
    for (size_t i = 0; i < str.length(); i++)
        result += toupper(static_cast<unsigned char>(str[i]));
    return result;
}

std::wstring Utils::ToLower(std::wstring const &str) {
    std::wstring result;
    for (size_t i = 0; i < str.length(); i++)
        result += tolower(static_cast<unsigned char>(str[i]));
    return result;
}

float Utils::SafeConvertFloat(std::wstring const &str) {
    float result = 0.0f;
    try {
        result = std::stof(str);
    }
    catch (...) {}
    return result;
}

double Utils::SafeConvertDouble(std::wstring const &str) {
    double result = 0.0;
    try {
        result = std::stod(str);
    }
    catch (...) {}
    return result;
}

std::wstring Utils::AtoW(std::string const &str) {
    return std::wstring(str.begin(), str.end());
}

std::string Utils::WtoA(std::wstring const &str) {
    return std::string(str.begin(), str.end());
}

void Utils::Replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
