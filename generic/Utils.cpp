#include <Windows.h>
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

std::vector<std::wstring> Utils::Split(std::wstring const &line, wchar_t delim, bool trim, bool skipEmpty) {
    std::vector<std::wstring> result;
    std::wstring currStr;
    auto AddStr = [&,trim,skipEmpty]() {
        if (trim)
            Utils::Trim(currStr);
        if (!skipEmpty || !currStr.empty())
            result.push_back(currStr);
        currStr.clear();
    };
    bool inQuotes = false;
    for (size_t i = 0; i < line.length(); i++) {
        auto c = line[i];
        if (c == L'\r' || c == L'\n')
            break;
        if (!inQuotes) {
            if (c == L'"')
                inQuotes = true;
            else if (c == delim)
                AddStr();
            else
                currStr += c;
        }
        else {
            if (c == L'"')
                inQuotes = false;
            else
                currStr += c;
        }
    }
    AddStr();
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

int Utils::Clamp(int value, int min, int max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    return value;
}

int Utils::MapTo(int value, int input_start, int input_end, int output_start, int output_end) {
    value = Clamp(value, input_start, input_end);
    double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
    return static_cast<int>(output_start + (floor(slope * (value - input_start)) + 0.5));
}

std::wstring Utils::GetStringWithoutUnicodeChars(std::wstring const &src) {
    std::wstring str = src;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == L'ð')
            str[i] = L'o';
        else if (str[i] == L'ß')
            str[i] = L's';
        else if (str[i] == L'Þ')
            str[i] = L'P';
    }
    int mbSize = WideCharToMultiByte(20127, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    char *mb = new char[mbSize];
    WideCharToMultiByte(20127, 0, str.c_str(), -1, mb, mbSize, NULL, NULL);
    int wcSize = MultiByteToWideChar(20127, 0, mb, -1, NULL, 0);
    wchar_t *wc = new wchar_t[wcSize];
    MultiByteToWideChar(20127, 0, mb, -1, wc, wcSize);
    str = wc;
    delete[] mb;
    delete[] wc;
    return str;
}
