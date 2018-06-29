#pragma once

#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <Windows.h>
#include "FifamVersion.h"
#include "FifamDate.h"
#include "Utils.h"
#include "Hexademical.h"
#include "Quoted.h"
#include "Error.h"

class Utilities {
public:
    static void skipBom_UTF8(FILE *file) {
        fseek(file, 0, SEEK_END);
        unsigned int size = ftell(file);
        if (size > 2) {
            fseek(file, 0, SEEK_SET);
            unsigned char dummy[3];
            fread(dummy, 1, 3, file);
        }
    }

    static unsigned int extractClubId(wchar_t *str) {
        unsigned int id = 0;
        if (wcslen(str) > 2 && str[0] == '0' && str[1] == '0') {
            if (str[2] == '0')
                swscanf_s(&str[2], L"%X", &id);
            else
                swscanf_s(&str[3], L"%X", &id);
        }
        else
            swscanf_s(str, L"%X", &id);
        return id;
    }

    static unsigned int getCountryId(unsigned int uid) {
        return (uid >> 16) & 0xFF;
    }

    static unsigned int getAdditionalFlags(unsigned int uid) {
        return (uid >> 24) & 0xFF;
    }

    static unsigned int getClubIdOnly(unsigned int uid) {
        return uid & 0xFFFF;
    }

    static void RemoveNewLine(wchar_t *line) {
        size_t ln = wcslen(line) - 1;
        if (line[ln - 1] == L'\r')
            line[ln - 1] = 0;
        else if (line[ln] == L'\n')
            line[ln] = 0;
    }

    static void CopyStr(wchar_t *dest, wchar_t *src, unsigned int maxLen) {
        wcsncpy(dest, src, maxLen);
        dest[maxLen - 1] = 0;
    }

    static void FixWrongCharacters(wchar_t *str) {
        size_t len = wcslen(str);
        for (size_t i = 0; i < len; i++) {
            if (str[i] == L'ð')
                str[i] = L'o';
            else if (str[i] == L'ß')
                str[i] = L's';
            else if (str[i] == L'Þ')
                str[i] = L'P';
        }
    }

    static wchar_t *RemoveAccented(wchar_t *str) {
        FixWrongCharacters(str);
        int size = WideCharToMultiByte(20127, 0, str, -1, NULL, 0, NULL, NULL);
        char *mb = new char[size];
        WideCharToMultiByte(20127, 0, str, -1, mb, size, NULL, NULL);
        MultiByteToWideChar(20127, 0, mb, -1, str, size);
        delete[] mb;
        return str;
    }

    static char *RemoveAccented(char *dest, wchar_t *str) {
        FixWrongCharacters(str);
        int size = WideCharToMultiByte(20127, 0, str, -1, NULL, 0, NULL, NULL); // 0xFDE9
        WideCharToMultiByte(20127, 0, str, -1, dest, size, NULL, NULL);
        return dest;
    }

    void SkipLines(size_t num, FILE *file) {
        wchar_t line[4096];
        for (size_t i = 0; i < num; i++)
            fgetws(line, 4096, file);
    }

    void ReadLine(wchar_t *line, size_t num, FILE *file) {
        for (size_t i = 0; i < num; i++)
            fgetws(line, 4096, file);
    }

    void ReadLine(wchar_t *line, FILE *file) {
        fgetws(line, 4096, file);
    }
};

class FifamFileWorker {
protected:
    FILE *mFile = nullptr;
    size_t mGameId = 0;
    FifamVersion mVersion;

public:
    bool IsVersionGreaterOrEqual(unsigned short year, unsigned short number) {
        return mVersion.IsGreaterOrEqual(year, number);
    }

    FifamFileWorker(size_t gameId) {
        mGameId = gameId;
    }

    ~FifamFileWorker() {
        Close();
    }

    void Close() {
        if (mFile) {
            fclose(mFile);
            mFile = nullptr;
        }
    }

    size_t GetGameId() {
        return mGameId;
    }

    FifamVersion GetVersion() {
        return mVersion;
    }

    bool Available() {
        return mFile != nullptr;
    }
};

class FifamWriter : public FifamFileWorker {
public:
    FifamWriter(String const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber) :
        FifamFileWorker(gameId)
    {
        mFile = _wfopen(filename.c_str(), gameId > 7 ? L"wb" : L"wt");
        if (mFile && gameId >= 8) {
            unsigned char sign[3];
            sign[0] = 0xEF; sign[1] = 0xBB; sign[2] = 0xBF;
            fwrite(sign, 3, 1, mFile);
            fclose(mFile);
            mFile = _wfopen(filename.c_str(), L"a");
            _setmode(_fileno(mFile), _O_U8TEXT);
        }
        mVersion.Set(vYear, vNumber);
    }

    void Write(char value) {
        fwprintf(mFile, L"%d", value);
    }

    void Write(short value) {
        fwprintf(mFile, L"%d", value);
    }

    void Write(int value) {
        fwprintf(mFile, L"%d", value);
    }

    void Write(unsigned char value) {
        fwprintf(mFile, L"%u", value);
    }

    void Write(unsigned short value) {
        fwprintf(mFile, L"%u", value);
    }

    void Write(unsigned int value) {
        fwprintf(mFile, L"%u", value);
    }

    void Write(long long int value) {
        fwprintf(mFile, L"%I64d", value);
    }

    void Write(unsigned long long int value) {
        fwprintf(mFile, L"%I64u", value);
    }

    void Write(float value) {
        fwprintf(mFile, L"%f", value);
    }

    void Write(double value) {
        fwprintf(mFile, L"%f", value);
    }

    void Write(bool value) {
        if (value)
            fputws(L"1", mFile);
        else
            fputws(L"0", mFile);
    }

    void Write(wchar_t const *value) {
        fputws(value, mFile);
    }

    void Write(String const &value) {
        fputws(value.c_str(), mFile);
    }

    void Write(Hexademical const &value) {
        fwprintf(mFile, L"%x", value());
    }

    void Write(Quoted const &value) {
        Write(L"\""); Write(value()); Write(L"\"");
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    void Write(T &value) {
        Write(static_cast<std::underlying_type_t<T>>(Utils::ToInt(value)));
    }

    void WriteStartIndex(wchar_t const *name, bool newLine = true) {
        if (newLine)
            WriteLine(String(L"%INDEX%") + name);
        else
            Write(String(L"%INDEX%") + name);
    }

    void WriteEndIndex(wchar_t const *name, bool newLine = true) {
        if (newLine)
            WriteLine(String(L"%INDEXEND%") + name);
        else
            Write(String(L"%INDEXEND%") + name);
    }

    void Write(FifamDate const &date) {
        if (IsVersionGreaterOrEqual(0x2009, 0xA))
            Write(Utils::Format(L"%02d-%02d-%04d", date.day, date.month, date.year));
        else {
            WriteStartIndex(L"DATE");
            WriteLine(date.day);
            WriteLine(date.month);
            WriteLine(date.year);
            WriteEndIndex(L"DATE", false);
        }
    }

    void WriteVersion() {
        WriteStartIndex(L"VERSION");
        WriteLine(mVersion.GetIntValue());
        WriteEndIndex(L"VERSION");
    }

private:
    template<typename One>
    void WriteArgs(char sep, One &&arg) {
        Write(std::forward<One>(arg));
    }

    template<typename One, typename... Other>
    void WriteArgs(char sep, One&& arg, Other&&... other) {
        WriteArgs(sep, std::forward<One>(arg));
        Write(Utils::CharToStr(sep));
        WriteArgs(sep, std::forward<Other>(other)...);
    }

public:
    template <typename... ArgTypes>
    void WriteLine(ArgTypes&&... args) {
        WriteArgs(',', std::forward<ArgTypes>(args)...);
        Write(L"\n");
    }

    template <typename... ArgTypes>
    void WriteLineWithSeparator(char sep, ArgTypes&&... args) {
        WriteArgs(sep, std::forward<ArgTypes>(args)...);
        Write(L"\n");
    }

    template<typename T>
    void WriteLineArray(std::vector<T> const &ary, char sep = ',') {
        for (size_t i = 0; i < ary.size(); i++) {
            Write(ary[i]);
            if (i != ary.size() - 1)
                Write(Utils::CharToStr(sep));
        }
        Write(L"\n");
    }
};

class FifamReader : public FifamFileWorker {
    static const size_t BUFFER_SIZE = 4096;
    wchar_t mLine[BUFFER_SIZE];
public:
    bool IsEof() { return feof(mFile); }

    FifamReader(String const &filename, size_t gameId) :
        FifamFileWorker(gameId)
    {
        mFile = _wfopen(filename.c_str(), gameId > 8 ? L"r, ccs=UTF-8" : L"rt");
    }

    FifamReader(String const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber) :
        FifamReader(filename, gameId)
    {
        mVersion.Set(vYear, vNumber);
    }

private:
    wchar_t *GetLine() {
        mLine[0] = 0;
        if (fgetws(mLine, BUFFER_SIZE, mFile)) {
            mLine[wcscspn(mLine, L"\r\n")] = 0;
            return mLine;
        }
        return nullptr;
    }

public:
    bool CheckLine(String const &str, bool skipIfTrue = false) {
        auto savedPos = ftell(mFile);
        auto line = GetLine();
        if (line && !str.compare(line)) {
            if (!skipIfTrue) {
                //Message(Utils::Format(L"Old pos: %d\nNew pos: %d\nLine: %s", savedPos, ftell(mFile), line));
                fseek(mFile, savedPos, SEEK_SET);
            }
            return true;
        }
        return false;
    }

    bool FindLine(String const &str, bool skipIfFound = false, bool moveToEofIfNotFound = false) {
        auto savedPos = ftell(mFile);
        while (!IsEof()) {
            if (CheckLine(str, skipIfFound))
                return true;
        }
        if (!moveToEofIfNotFound)
            fseek(mFile, savedPos, SEEK_SET);
        return false;
    }

    void SkipLines(size_t count) {
        for (size_t i = 0; i < count; i++)
            GetLine();
    }

    void SkipLine() {
        SkipLines(1);
    }

    void ReadStartIndex(wchar_t const *name) { SkipLine(); }
    void ReadEndIndex(wchar_t const *name) { SkipLine(); }

    void ReadLine(FifamDate &date) {
        if (IsVersionGreaterOrEqual(0x2009, 0xA))
            ReadLineWithSeparator('-', date.day, date.month, date.year);
        else {
            ReadStartIndex(L"DATE");
            ReadLine(date.day);
            ReadLine(date.month);
            ReadLine(date.year);
            ReadEndIndex(L"DATE");
        }
        date.Validate();
    }

private:
    template<typename T>
    T SafeConvertInt(String const &str, bool isHex = false) {
        unsigned int result = 0;
        try {
            result = static_cast<T>(std::stoul(str, 0, isHex? 16 : 10));
        }
        catch(...) {}
        return result;
    }

    float SafeConvertFloat(String const &str) {
        float result = 0.0f;
        try {
            result = std::stof(str);
        }
        catch (...) {}
        return result;
    }

    double SafeConvertDouble(String const &str) {
        double result = 0.0;
        try {
            result = std::stod(str);
        }
        catch (...) {}
        return result;
    }

    void StrToArg(String const &str, unsigned char &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned char>(str);
    }

    void StrToArg(String const &str, char &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<char>(str);
    }

    void StrToArg(String const &str, unsigned short &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned short>(str);
    }

    void StrToArg(String const &str, short &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<short>(str); }

    void StrToArg(String const &str, unsigned int &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned int>(str);
    }

    void StrToArg(String const &str, int &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned int>(str);
    }

    void StrToArg(String const &str, unsigned long long int &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned long long int>(str);
    }

    void StrToArg(String const &str, long long int &arg) {
        arg = str.empty() ? 0 : SafeConvertInt<long long int>(str);
    }

    void StrToArg(String const &str, float &arg) {
        arg = str.empty() ? 0.0f : SafeConvertFloat(str);
    }

    void StrToArg(String const &str, double &arg) {
        arg = str.empty() ? 0.0 : SafeConvertDouble(str);
    }

    void StrToArg(String const &str, wchar_t *arg) {
        wcscpy(arg, str.c_str());
    }

    void StrToArg(String const &str, String &arg) {
        arg = str;
    }

    void StrToArg(String const &str, FifamDate &arg) {
        if (IsVersionGreaterOrEqual(0x2009, 0xA) && !str.empty()) {
            auto dateInfo = Utils::Split(str, L"-");
            if (dateInfo.size() == 3) {
                arg.day = SafeConvertInt<char>(dateInfo[0]);
                arg.month = SafeConvertInt<char>(dateInfo[1]);
                arg.year = SafeConvertInt<short>(dateInfo[2]);
                arg.Validate();
                return;
            }
        }
        arg.MakeEmpty();
    }

    void StrToArg(String const &str, Hexademical arg) {
        arg = str.empty() ? 0 : SafeConvertInt<unsigned int>(str, true);
    }

    void StrToArg(String const &str, Quoted arg) {
        String modstr = str;
        if (modstr.size() > 0) {
            if (modstr.front() == '"')
                modstr = modstr.substr(1);
            if (modstr.size() > 0 && modstr.back() == '"')
                modstr.pop_back();
        }
        arg = modstr;
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    void StrToArg(String const &str, T &arg) {
        Utils::FromInt<T>(str.empty()? 0 : SafeConvertInt<std::underlying_type_t<T>>(str));
    }

    template<typename One>
    void ReadOneArg(std::vector<String> strArgs, size_t &currArg, One &&one) {
        if (strArgs.size() > currArg)
            StrToArg(strArgs[currArg++], std::forward<One>(one));
        else
            StrToArg(String(), std::forward<One>(one));
    }

    template<typename One, typename... Other>
    void ReadOneArg(std::vector<String> strArgs, size_t &currArg, One&& one, Other&&... other) {
        ReadOneArg(strArgs, currArg, std::forward<One>(one));
        ReadOneArg(strArgs, currArg, std::forward<Other>(other)...);
    }

public:
    template<typename... ArgTypes>
    void ReadLine(ArgTypes&&... args) {
        GetLine();
        auto strArgs = Utils::Split(mLine, L",");
        size_t currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename... ArgTypes>
    void ReadLineWithSeparator(char sep, ArgTypes&&... args) {
        GetLine();
        auto strArgs = Utils::Split(mLine, Utils::CharToStr(sep));
        size_t currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename T>
    std::vector<T> ReadLineArray(char sep = ',') {
        GetLine();
        std::vector<T> ary;
        std::vector<String> strArgs = Utils::Split(mLine, Utils::CharToStr(sep));
        ary.resize(strArgs.size());
        for (size_t i = 0; i < strArgs.size(); i++)
            StrToArg(strArgs[i], ary[i]);
        return ary;
    }

    template<typename T>
    T ReadLine() {
        T out;
        ReadLine(out);
        return out;
    }

    void ReadVersion() {
        if (mVersion.GetIntValue() != 0)
            SkipLines(3);
        else {
            SkipLine();
            mVersion.Set(ReadLine<unsigned int>());
            SkipLine();
        }
    }
};
