#include <Windows.h>
#include "Utils.h"
#include <chrono>

std::wstring Utils::TimeString(std::time_t const &time) {
    tm t = *localtime(&time);
    return Format(L"%02d.%02d.%04d %02d:%02d:%02d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900, t.tm_hour, t.tm_min, t.tm_sec);
}

std::string Utils::TimeStringA(std::time_t const &time) {
    tm t = *localtime(&time);
    return Format("%02d.%02d.%04d %02d:%02d:%02d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900, t.tm_hour, t.tm_min, t.tm_sec);
}

std::wstring Utils::TimeDifferenceSting(std::time_t const &timeEnd, std::time_t const &timeBegin) {
    time_t seconds = static_cast<time_t>(difftime(timeEnd, timeBegin));
    tm *dt = gmtime(&seconds);
    if (dt->tm_yday > 0)
        return Format(L"%dd %dh %dm %ds", dt->tm_yday, dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_hour > 0)
        return Format(L"%dh %dm %ds", dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_min > 0)
        return Format(L"%dm %ds", dt->tm_min, dt->tm_sec);
    return Format(L"%ds", dt->tm_sec);
}

std::string Utils::TimeDifferenceStingA(std::time_t const &timeEnd, std::time_t const &timeBegin) {
    time_t seconds = static_cast<time_t>(difftime(timeEnd, timeBegin));
    tm *dt = gmtime(&seconds);
    if (dt->tm_yday > 0)
        return Format("%dd %dh %dm %ds", dt->tm_yday, dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_hour > 0)
        return Format("%dh %dm %ds", dt->tm_hour, dt->tm_min, dt->tm_sec);
    if (dt->tm_min > 0)
        return Format("%dm %ds", dt->tm_min, dt->tm_sec);
    return Format("%ds", dt->tm_sec);
}

std::wstring Utils::CurrentTime() {
    return TimeString(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

std::string Utils::CurrentTimeA() {
    return TimeStringA(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

std::wstring Utils::GetQuickName(std::wstring const &firstName, std::wstring const &lastName, std::wstring const &commonName) {
    if (!commonName.empty())
        return commonName;
    if (!firstName.empty()) {
        if (!lastName.empty())
            return firstName + L" " + lastName;
        else
            return firstName;
    }
    else if (!lastName.empty())
        return lastName;
    return std::wstring();
}

std::string Utils::GetQuickNameA(std::string const &firstName, std::string const &lastName, std::string const &commonName) {
    if (!commonName.empty())
        return commonName;
    if (!firstName.empty()) {
        if (!lastName.empty())
            return firstName + " " + lastName;
        else
            return firstName;
    }
    else if (!lastName.empty())
        return lastName;
    return std::string();
}

std::vector<std::wstring> Utils::Split(std::wstring const &line, wchar_t delim, bool trim, bool skipEmpty, bool quotesHavePriority) {
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
        if (c == L'\r' || (delim != L'\n' && c == L'\n'))
            break;
        if (!inQuotes) {
            if (quotesHavePriority && c == L'"')
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

std::vector<std::string> Utils::Split(std::string const &line, char delim, bool trim, bool skipEmpty, bool quotesHavePriority) {
    std::vector<std::string> result;
    std::string currStr;
    auto AddStr = [&, trim, skipEmpty]() {
        if (trim)
            Utils::Trim(currStr);
        if (!skipEmpty || !currStr.empty())
            result.push_back(currStr);
        currStr.clear();
    };
    bool inQuotes = false;
    for (size_t i = 0; i < line.length(); i++) {
        auto c = line[i];
        if (c == '\r' || (delim != '\n' && c == '\n'))
            break;
        if (!inQuotes) {
            if (quotesHavePriority && c == '"')
                inQuotes = true;
            else if (c == delim)
                AddStr();
            else
                currStr += c;
        }
        else {
            if (c == '"')
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

bool Utils::StartsWith(std::string const &str, std::string const &what) {
    return !str.compare(0, what.size(), what);
}

bool Utils::EndsWith(std::wstring const &str, std::wstring const &what) {
    auto strLen = str.length();
    auto whatLen = what.length();
    if (whatLen > strLen)
        return false;
    return str.substr(strLen - whatLen) == what;
}

bool Utils::EndsWith(std::string const &str, std::string const &what) {
    auto strLen = str.length();
    auto whatLen = what.length();
    if (whatLen > strLen)
        return false;
    return str.substr(strLen - whatLen) == what;
}

std::wstring Utils::CharToStr(wchar_t c) {
    wchar_t str[2];
    str[0] = c;
    str[1] = 0;
    return str;
}

std::string Utils::CharToStrA(char c) {
    char str[2];
    str[0] = c;
    str[1] = 0;
    return str;
}

bool Utils::Compare(std::wstring const &str, size_t index, char c) {
    return str.length() > index && str[index] == c;
}

bool Utils::Compare(std::string const &str, size_t index, char c) {
    return str.length() > index && str[index] == c;
}

bool IsHexadecimalLetter(wchar_t c) {
    return (c >= L'a' && c <= L'f') || (c >= L'A' && c <= L'F');
}

bool IsHexadecimalLetter(char c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool Utils::IsNumber(std::wstring const &str, bool hexadecimal) {
    if (str.empty())
        return false;
    std::wstring cmpStr;
    if (hexadecimal && (StartsWith(str, L"0x") || StartsWith(str, L"0X")))
        cmpStr = str.substr(2);
    else
        cmpStr = str;
    for (wchar_t c : cmpStr) {
        if (isdigit(c))
            continue;
        if (hexadecimal && IsHexadecimalLetter(c))
            continue;
        return false;
    }
    return true;
}

bool Utils::IsNumber(std::string const &str, bool hexadecimal) {
    if (str.empty())
        return false;
    std::string cmpStr;
    if (hexadecimal && (StartsWith(str, "0x") || StartsWith(str, "0X")))
        cmpStr = str.substr(2);
    else
        cmpStr = str;
    for (char c : cmpStr) {
        if (isdigit(c))
            continue;
        if (hexadecimal && IsHexadecimalLetter(c))
            continue;
        return false;
    }
    return true;
}

int Utils::ToNumber(std::wstring const &str) {
    std::wstring trimmed = str;
    Trim(trimmed);
    return ((StartsWith(trimmed, L"0x") || StartsWith(trimmed, L"0X")) ? wcstol(trimmed.substr(2).c_str(), nullptr, 16) : wcstol(trimmed.c_str(), nullptr, 10));
}

int Utils::ToNumber(std::string const &str) {
    std::string trimmed = str;
    Trim(trimmed);
    return ((StartsWith(trimmed, "0x") || StartsWith(trimmed, "0X")) ? strtol(trimmed.substr(2).c_str(), nullptr, 16) : strtol(trimmed.c_str(), nullptr, 10));
}

void Utils::Trim(std::wstring &str) {
    size_t start = str.find_first_not_of(L" \t\r\n");
    if (start != std::wstring::npos)
        str = str.substr(start);
    size_t end = str.find_last_not_of(L" \t\r\n");
    if (end != std::wstring::npos)
        str = str.substr(0, end + 1);
}

void Utils::Trim(std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start != std::string::npos)
        str = str.substr(start);
    size_t end = str.find_last_not_of(" \t\r\n");
    if (end != std::string::npos)
        str = str.substr(0, end + 1);
}

std::wstring Utils::ToUpper(std::wstring const &str) {
    std::wstring result;
    for (size_t i = 0; i < str.length(); i++)
        result += toupper(static_cast<unsigned short>(str[i]));
    return result;
}

std::string Utils::ToUpper(std::string const &str) {
    std::string result;
    for (size_t i = 0; i < str.length(); i++)
        result += toupper(static_cast<unsigned char>(str[i]));
    return result;
}

std::wstring Utils::ToLower(std::wstring const &str) {
    std::wstring result;
    for (size_t i = 0; i < str.length(); i++)
        result += tolower(static_cast<unsigned short>(str[i]));
    return result;
}

std::string Utils::ToLower(std::string const &str) {
    std::string result;
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

float Utils::SafeConvertFloat(std::string const &str) {
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

double Utils::SafeConvertDouble(std::string const &str) {
    double result = 0.0;
    try {
        result = std::stod(str);
    }
    catch (...) {}
    return result;
}

std::wstring Utils::AtoW(std::string const &str) {
	std::wstring result;
	result.resize(str.size());
	for (unsigned int i = 0; i < str.size(); i++)
		result[i] = static_cast<wchar_t>(static_cast<unsigned char>(str[i]));
	return result;
}

std::string Utils::WtoA(std::wstring const &str) {
	std::string result;
	result.resize(str.size());
	for (unsigned int i = 0; i < str.size(); i++)
		result[i] = static_cast<char>(static_cast<unsigned char>(str[i]));
	return result;
}

void Utils::Replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void Utils::Replace(std::string &str, const std::string &from, const std::string &to) {
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
    return static_cast<int>(round(output_start + slope * (value - input_start)));
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
        else if (str[i] == L'ț')
            str[i] = L't';
        else if (str[i] == L'Ț')
            str[i] = L'T';
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

bool Utils::IsVowel(wchar_t wideChar) {
    return wideChar == L'A' || wideChar == L'a' ||
        wideChar == L'E' || wideChar == L'e' ||
        wideChar == L'I' || wideChar == L'i' ||
        wideChar == L'O' || wideChar == L'o' ||
        wideChar == L'U' || wideChar == L'u' ||
        wideChar == L'Ä' || wideChar == L'ä' ||
        wideChar == L'Ö' || wideChar == L'ö' ||
        wideChar == L'Ü' || wideChar == L'ü' ||
        wideChar == L'Y' || wideChar == L'y';
}

bool Utils::IsPunctuationMark(wchar_t wideChar) {
    return wideChar == L'.' ||
        wideChar == L',' ||
        wideChar == L':' ||
        wideChar == L';' ||
        wideChar == L'-' ||
        wideChar == L'(' ||
        wideChar == L')' ||
        wideChar == L'\'' ||
        wideChar == L'\\' ||
        wideChar == L'/';

}

std::wstring Utils::Join(std::vector<std::wstring> const &strList, wchar_t delim) {
    std::wstring result;
    bool first = true;
    for (std::wstring const &str : strList) {
        if (first)
            first = false;
        else
            result += delim;
        result += str;
    }
    return result;
}

std::string Utils::Join(std::vector<std::string> const &strList, char delim) {
    std::string result;
    bool first = true;
    for (std::string const &str : strList) {
        if (first)
            first = false;
        else
            result += delim;
        result += str;
    }
    return result;
}

std::wstring Utils::Join(std::vector<std::wstring> const &strList, std::wstring const &delim) {
    std::wstring result;
    bool first = true;
    for (std::wstring const &str : strList) {
        if (first)
            first = false;
        else
            result += delim;
        result += str;
    }
    return result;
}

std::string Utils::Join(std::vector<std::string> const &strList, std::string const &delim) {
    std::string result;
    bool first = true;
    for (std::string const &str : strList) {
        if (first)
            first = false;
        else
            result += delim;
        result += str;
    }
    return result;
}
