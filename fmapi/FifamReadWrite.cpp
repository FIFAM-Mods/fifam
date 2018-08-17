#include "FifamReadWrite.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <Windows.h>
#include "Error.h"

void Utilities::skipBom_UTF8(FILE *file) {
    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    if (size > 2) {
        fseek(file, 0, SEEK_SET);
        unsigned char dummy[3];
        fread(dummy, 1, 3, file);
    }
}

unsigned int Utilities::extractClubId(wchar_t *str) {
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

unsigned int Utilities::getCountryId(unsigned int uid) {
    return (uid >> 16) & 0xFF;
}

unsigned int Utilities::getAdditionalFlags(unsigned int uid) {
    return (uid >> 24) & 0xFF;
}

unsigned int Utilities::getClubIdOnly(unsigned int uid) {
    return uid & 0xFFFF;
}

void Utilities::RemoveNewLine(wchar_t *line) {
    size_t ln = wcslen(line) - 1;
    if (line[ln - 1] == L'\r')
        line[ln - 1] = 0;
    else if (line[ln] == L'\n')
        line[ln] = 0;
}

void Utilities::CopyStr(wchar_t *dest, wchar_t *src, unsigned int maxLen) {
    wcsncpy(dest, src, maxLen);
    dest[maxLen - 1] = 0;
}

void Utilities::FixWrongCharacters(wchar_t *str) {
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

wchar_t *Utilities::RemoveAccented(wchar_t *str) {
    FixWrongCharacters(str);
    int size = WideCharToMultiByte(20127, 0, str, -1, NULL, 0, NULL, NULL);
    char *mb = new char[size];
    WideCharToMultiByte(20127, 0, str, -1, mb, size, NULL, NULL);
    MultiByteToWideChar(20127, 0, mb, -1, str, size);
    delete[] mb;
    return str;
}

char *Utilities::RemoveAccented(char *dest, wchar_t *str) {
    FixWrongCharacters(str);
    int size = WideCharToMultiByte(20127, 0, str, -1, NULL, 0, NULL, NULL); // 0xFDE9
    WideCharToMultiByte(20127, 0, str, -1, dest, size, NULL, NULL);
    return dest;
}

void Utilities::SkipLines(size_t num, FILE *file) {
    wchar_t line[4096];
    for (size_t i = 0; i < num; i++)
        fgetws(line, 4096, file);
}

void Utilities::ReadLine(wchar_t *line, size_t num, FILE *file) {
    for (size_t i = 0; i < num; i++)
        fgetws(line, 4096, file);
}

void Utilities::ReadLine(wchar_t *line, FILE *file) {
    fgetws(line, 4096, file);
}


bool FifamFileWorker::IsVersionGreaterOrEqual(unsigned short year, unsigned short number) {
    return mVersion.IsGreaterOrEqual(year, number);
}

FifamFileWorker::FifamFileWorker(size_t gameId) {
    mGameId = gameId;
}

FifamFileWorker::~FifamFileWorker() {
    Close();
}

void FifamFileWorker::Close() {
    if (mFile) {
        fclose(mFile);
        mFile = nullptr;
    }
}

size_t FifamFileWorker::GetGameId() {
    return mGameId;
}

FifamVersion FifamFileWorker::GetVersion() {
    return mVersion;
}

bool FifamFileWorker::Available() {
    return mFile != nullptr;
}


FifamWriter::FifamWriter(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber, bool unicode) :
    FifamFileWorker(gameId)
{
    mFile = _wfopen(filename.c_str(), unicode ? L"wb" : L"wt");
    if (mFile && unicode) {
        unsigned char sign[3];
        sign[0] = 0xEF; sign[1] = 0xBB; sign[2] = 0xBF;
        fwrite(sign, 3, 1, mFile);
        fclose(mFile);
        mFile = _wfopen(filename.c_str(), L"a");
        _setmode(_fileno(mFile), _O_U8TEXT);
    }
    mVersion.Set(vYear, vNumber);
}

void FifamWriter::WriteOne(char value) {
    fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(wchar_t value) {
    fputwc(value, mFile);
}

void FifamWriter::WriteOne(short value) {
    fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(int value) {
    fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(unsigned char value) {
    fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(unsigned short value) {
    fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(unsigned int value) {
    fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(long long int value) {
    fwprintf(mFile, L"%I64d", value);
}

void FifamWriter::WriteOne(unsigned long long int value) {
    fwprintf(mFile, L"%I64u", value);
}

void FifamWriter::WriteOne(float value) {
    fwprintf(mFile, L"%f", value);
}

void FifamWriter::WriteOne(double value) {
    fwprintf(mFile, L"%f", value);
}

void FifamWriter::WriteOne(bool value) {
    if (value)
        fputws(L"1", mFile);
    else
        fputws(L"0", mFile);
}

void FifamWriter::WriteOne(wchar_t const *value) {
    String str = value;
    WriteOne(str);
}

void FifamWriter::WriteOne(String const &value) {
    if (value.length() > 1) {
        String noqValue = value;
        for (UInt i = 0; i < value.length(); i++) {
            if (noqValue[i] == L'"')
                noqValue[i] = L'\'';
        }
        fputws(noqValue.c_str(), mFile);
        return;
    }
    fputws(value.c_str(), mFile);
}

void FifamWriter::WriteOne(Hexademical const &value) {
    fwprintf(mFile, L"%X", value());
}

void FifamWriter::WriteOne(Quoted const &value) {
    WriteOne(L"\"");
    WriteOne(value());
    WriteOne(L"\"");
}

void FifamWriter::WriteStartIndex(String const &name, bool newLine) {
    if (newLine)
        WriteLine(L"%INDEX%" + name);
    else
        WriteOne(L"%INDEX%" + name);
}

void FifamWriter::WriteEndIndex(String const &name, bool newLine) {
    if (newLine)
        WriteLine(L"%INDEXEND%" + name);
    else
        WriteOne(L"%INDEXEND%" + name);
}

void FifamWriter::WriteOne(FifamDate const &date) {
    if (IsVersionGreaterOrEqual(0x2009, 0xA))
        WriteOne(Utils::Format(L"%02d-%02d-%04d", date.day, date.month, date.year));
    else {
        WriteStartIndex(L"DATE");
        WriteLine(date.day);
        WriteLine(date.month);
        WriteLine(date.year);
        WriteEndIndex(L"DATE", false);
    }
}

void FifamWriter::WriteVersion() {
    WriteStartIndex(L"VERSION");
    WriteLine(mVersion.GetIntValue());
    WriteEndIndex(L"VERSION");
}

void FifamWriter::WriteTranslationArray(FifamTrArray<String> const &ary, wchar_t sep, bool quoted) {
    for (size_t i = 0; i < ary.size(); i++) {
        if (i < 5 || IsVersionGreaterOrEqual(0x2007, 0x1A)) {
            if (quoted)
                WriteOne(L"\"");
            WriteOne(ary[i]);
            if (quoted)
                WriteOne(L"\"");
            if (i != ary.size() - 1)
                WriteOne(sep);
        }
    }
}

void FifamWriter::WriteLineTranslationArray(FifamTrArray<String> const &ary, wchar_t sep, bool quoted) {
    WriteTranslationArray(ary, sep, quoted);
    WriteOne(L"\n");
}

void FifamWriter::WriteNewLine() {
    WriteOne(L"\n");
}


FifamReader::FifamReader(Path const &filename, size_t gameId, bool unicode) :
    FifamFileWorker(gameId)
{
    mFile = _wfopen(filename.c_str(), unicode ? L"r, ccs=UTF-8" : L"rt");
}

FifamReader::FifamReader(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber) :
    FifamReader(filename, gameId)
{
    mVersion.Set(vYear, vNumber);
}

bool FifamReader::IsEof() {
    return feof(mFile);
}

wchar_t *FifamReader::GetLine() {
    mLine[0] = 0;
    while (fgetws(mLine, BUFFER_SIZE, mFile)) {
        if (mLine[0] != L';') {
            mLine[wcscspn(mLine, L"\r\n")] = 0;
            return mLine;
        }
    }
    mLine[0] = 0;
    return nullptr;
}

bool FifamReader::CheckLine(String const &str, bool skipIfTrue) {
    auto savedPos = ftell(mFile);
    auto line = GetLine();
    if (line && !str.compare(line)) {
        if (!skipIfTrue) {
            fseek(mFile, savedPos, SEEK_SET);
        }
        return true;
    }
    return false;
}

bool FifamReader::FindLine(String const &str, bool skipIfFound, bool moveToEofIfNotFound) {
    auto savedPos = ftell(mFile);
    while (!IsEof()) {
        if (CheckLine(str, skipIfFound))
            return true;
    }
    if (!moveToEofIfNotFound)
        fseek(mFile, savedPos, SEEK_SET);
    return false;
}

void FifamReader::SkipLines(size_t count) {
    for (size_t i = 0; i < count; i++)
        GetLine();
}

void FifamReader::SkipLine() {
    SkipLines(1);
}

bool FifamReader::ReadStartIndex(String const &name, bool moveToEofIfNotFound) {
    return FindLine(L"%INDEX%" + name, true, moveToEofIfNotFound);
}

bool FifamReader::ReadEndIndex(String const &name, bool moveToEofIfNotFound) {
    return FindLine(L"%INDEXEND%" + name, true, moveToEofIfNotFound);
}

void FifamReader::ReadLine(FifamDate &date) {
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

void FifamReader::StrToArg(String const &str, unsigned char &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned char>(str);
}

void FifamReader::StrToArg(String const &str, char &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<char>(str);
}

void FifamReader::StrToArg(String const &str, unsigned short &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned short>(str);
}

void FifamReader::StrToArg(String const &str, short &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<short>(str);
}

void FifamReader::StrToArg(String const &str, unsigned int &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned int>(str);
}

void FifamReader::StrToArg(String const &str, int &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned int>(str);
}

void FifamReader::StrToArg(String const &str, unsigned long long int &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned long long int>(str);
}

void FifamReader::StrToArg(String const &str, long long int &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<long long int>(str);
}

void FifamReader::StrToArg(String const &str, float &arg) {
    arg = str.empty() ? 0.0f : Utils::SafeConvertFloat(str);
}

void FifamReader::StrToArg(String const &str, double &arg) {
    arg = str.empty() ? 0.0 : Utils::SafeConvertDouble(str);
}

void FifamReader::StrToArg(String const &str, wchar_t *arg) {
    wcscpy(arg, str.c_str());
}

void FifamReader::StrToArg(String const &str, String &arg) {
    arg = str;
}

void FifamReader::StrToArg(String const &str, FifamDate &arg) {
    if (IsVersionGreaterOrEqual(0x2009, 0xA) && !str.empty()) {
        auto dateInfo = Utils::Split(str, L'-');
        if (dateInfo.size() == 3) {
            arg.day = Utils::SafeConvertInt<char>(dateInfo[0]);
            arg.month = Utils::SafeConvertInt<char>(dateInfo[1]);
            arg.year = Utils::SafeConvertInt<short>(dateInfo[2]);
            arg.Validate();
            return;
        }
    }
    arg.MakeEmpty();
}

void FifamReader::StrToArg(String const &str, Hexademical arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<unsigned int>(str, true);
}

void FifamReader::StrToArg(String const &str, Quoted arg) {
    String modstr = str;
    RemoveQuotes(modstr);
    arg = modstr;
}

bool FifamReader::ReadVersion() {
    if (ReadStartIndex(L"VERSION")) {
        mVersion.Set(ReadLine<UInt>());
        return ReadEndIndex(L"VERSION");
    }
    return false;
}

String FifamReader::ReadFullLine() {
    auto line = GetLine();
    if (line)
        return line;
    return String();
}

bool FifamReader::ReadFullLine(String &out) {
    auto line = GetLine();
    if (line) {
        out = line;
        return true;
    }
    out.clear();
    return false;
}

void FifamReader::RemoveQuotes(String &str) {
    if (str.size() > 0) {
        if (str.front() == '"')
            str = str.substr(1);
        if (str.size() > 0 && str.back() == '"')
            str.pop_back();
    }
}

size_t FifamReader::ReadLineTranslationArray(FifamTrArray<String> &out, wchar_t sep) {
    auto result = ReadLineArray(out, sep);
    if (!IsVersionGreaterOrEqual(0x2007, 0x1A) && out.size() >= FifamTranslation::NUM_TRANSLATIONS)
        out[FifamTranslation::Polish] = out[FifamTranslation::English];
    return result;
}
