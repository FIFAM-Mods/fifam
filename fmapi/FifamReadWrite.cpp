#include "FifamReadWrite.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <Windows.h>
#include "Error.h"

bool FifamFileWorker::IsVersionGreaterOrEqual(unsigned short year, unsigned short number) {
    return mVersion.IsGreaterOrEqual(year, number);
}

FifamFileWorker::FifamFileWorker(Bool unicode, UInt gameId) {
    mUnicode = unicode;
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

long FifamFileWorker::GetPosition() {
    return ftell(mFile);
}

void FifamFileWorker::SetPosition(long pos) {
    fseek(mFile, pos, SEEK_SET);
}

long FifamFileWorker::GetSize() {
    auto currPos = GetPosition();
    fseek(mFile, 0, SEEK_END);
    auto result = GetPosition();
    SetPosition(currPos);
    return result;
}


FifamWriter::FifamWriter(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber, bool unicode) :
    FifamFileWorker(unicode, gameId)
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
    String str;
    if (GetGameId() >= 8)
        str = value;
    else
        str = Utils::GetStringWithoutUnicodeChars(value);
    if (str.length() > 1) {
        String noqValue = str;
        for (UInt i = 0; i < str.length(); i++) {
            if (noqValue[i] == L'"')
                noqValue[i] = L'\'';
        }
        fputws(noqValue.c_str(), mFile);
        return;
    }
    fputws(str.c_str(), mFile);
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
    UChar day = date.day;
    UChar month = date.month;
    UShort year = date.year;
    if (year == 0) {
        day = 0;
        month = 0;
    }
    if (IsVersionGreaterOrEqual(0x2009, 0xA))
        WriteOne(Utils::Format(L"%04d-%02d-%02d", year, month, day));
    else {
        WriteStartIndex(L"DATE");
        WriteLine(day);
        WriteLine(month);
        WriteLine(year);
        WriteEndIndex(L"DATE", false);
    }
}

void FifamWriter::WriteVersion() {
    WriteStartIndex(L"VERSION");
    WriteLine(mVersion.GetIntValue());
    WriteEndIndex(L"VERSION");
}

void FifamWriter::WriteTranslationArray(FifamTrArray<String> const &ary, bool quoted, wchar_t sep) {
    size_t num_tr = 6;
    if (!IsVersionGreaterOrEqual(0x2007, 0x1A))
        num_tr = 5;
    for (size_t i = 0; i < num_tr; i++) {
        if (quoted)
            WriteOne(L"\"");
        WriteOne(ary[i]);
        if (quoted)
            WriteOne(L"\"");
        if (i != num_tr - 1)
            WriteOne(sep);
    }
}

void FifamWriter::WriteLineTranslationArray(FifamTrArray<String> const &ary, bool quoted, wchar_t sep) {
    WriteTranslationArray(ary, quoted, sep);
    WriteOne(L"\n");
}

void FifamWriter::WriteNewLine() {
    WriteOne(L"\n");
}


FifamReader::FifamReader(Path const &filename, size_t gameId, bool unicode) :
    FifamFileWorker(unicode, gameId)
{
    mFile = _wfopen(filename.c_str(), L"rt");
    if (mFile) {
        if (GetSize() >= 3) {
            UChar sign[3];
            fread(sign, 1, 3, mFile);
            if (sign[0] != 0xEF || sign[1] != 0xBB || sign[2] != 0xBF)
                SetPosition(0);
        }
    }
}

FifamReader::FifamReader(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber) :
    FifamReader(filename, gameId)
{
    mVersion.Set(vYear, vNumber);
}

bool FifamReader::IsEof() {
    return GetPosition() == GetSize();
}

wchar_t *FifamReader::GetLine() {
    static char cLine[BUFFER_SIZE];
    cLine[0] = 0;
    mLine[0] = 0;
    while (fgets(cLine, BUFFER_SIZE, mFile)) {
        if (cLine[0] != ';') {
            cLine[strcspn(cLine, "\r\n")] = 0;
            if (mUnicode)
                MultiByteToWideChar(CP_UTF8, 0, cLine, BUFFER_SIZE, mLine, BUFFER_SIZE);
            else
                MultiByteToWideChar(1250, 0, cLine, BUFFER_SIZE, mLine, BUFFER_SIZE);
            return mLine;
        }
    }
    mLine[0] = 0;
    return nullptr;
}

bool FifamReader::CheckLine(String const &str, bool skipIfTrue) {
    auto savedPos = GetPosition();
    auto line = GetLine();
    if (line && !str.compare(line)) {
        if (!skipIfTrue)
            SetPosition(savedPos);
        return true;
    }
    SetPosition(savedPos);
    return false;
}

bool FifamReader::FindLine(String const &str, bool skipIfFound, bool moveToEofIfNotFound) {
    auto savedFilePos = GetPosition();
    while (!IsEof()) {
        auto savedLinePos = GetPosition();
        auto line = GetLine();
        if (line && !str.compare(line)) {
            if (!skipIfFound)
                SetPosition(savedLinePos);
            return true;
        }
    }
    if (!moveToEofIfNotFound)
        SetPosition(savedFilePos);
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
        ReadLineWithSeparator('-', date.year, date.month, date.day);
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
            arg.year = Utils::SafeConvertInt<Short>(dateInfo[0]);
            arg.month = Utils::SafeConvertInt<Char>(dateInfo[1]);
            arg.day = Utils::SafeConvertInt<Char>(dateInfo[2]);
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
