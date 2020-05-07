﻿#include "FifamReadWrite.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <Windows.h>
#include "Error.h"

Bool FifamFileWorker::IsVersionGreaterOrEqual(UShort year, UShort number) {
    return mVersion.IsGreaterOrEqual(year, number);
}

FifamFileWorker::FifamFileWorker() {
    mGameId = 0;
}

FifamFileWorker::FifamFileWorker(UInt gameId) {
    mGameId = gameId;
}

FifamFileWorker::~FifamFileWorker() {

}

UInt FifamFileWorker::GetGameId() {
    return mGameId;
}

FifamVersion FifamFileWorker::GetVersion() {
    return mVersion;
}

void FifamFileWorker::SetVersion(FifamVersion const & version) {
    mVersion = version;
}

void FifamFileWorker::SetVersion(UShort year, UShort number) {
    mVersion.Set(year, number);
}


FifamWriter::FifamWriter(Path const &filename, UInt gameId, FifamVersion const &version, Bool unicode) :
    FifamFileWorker(gameId)
{
    auto parentPath = filename.parent_path();
    if (!parentPath.empty() && !exists(parentPath))
        create_directories(parentPath);
    mFile = _wfopen(filename.c_str(), unicode ? L"wb" : L"wt");
    if (mFile && unicode) {
        UChar sign[3];
        sign[0] = 0xEF; sign[1] = 0xBB; sign[2] = 0xBF;
        fwrite(sign, 3, 1, mFile);
        fclose(mFile);
        mFile = _wfopen(filename.c_str(), L"a");
        if (mFile)
            _setmode(_fileno(mFile), _O_U8TEXT);
    }
    mVersion = version;
}

FifamWriter::FifamWriter(Path const &filename) : FifamWriter(filename, 14, FifamVersion(), true) {}

FifamWriter::FifamWriter(String *outputString, UInt gameId, FifamVersion const &version, Bool unicode) :
    FifamFileWorker(gameId)
{
    mOutputStr = outputString;
    mVersion = version;
}

FifamWriter::FifamWriter(String *outputString) : FifamWriter(outputString, 14, FifamVersion(), true) {}

void FifamWriter::SetReplaceQuotes(bool replace) {
    mReplaceQuotes = replace;
}

FifamWriter::~FifamWriter() {
    Close();
}

Bool FifamWriter::Available() {
    if (mOutputStr)
        return true;
    return mFile != nullptr;
}

void FifamWriter::Close() {
    if (mFile) {
        fclose(mFile);
        mFile = nullptr;
    }
}

void FifamWriter::WriteOne(Char value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%d", value);
    else
        fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(WideChar value) {
    if (mOutputStr)
        *mOutputStr += value;
    else
        fputwc(value, mFile);
}

void FifamWriter::WriteOne(Short value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%d", value);
    else
        fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(Int value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%d", value);
    else
        fwprintf(mFile, L"%d", value);
}

void FifamWriter::WriteOne(UChar value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%u", value);
    else
        fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(UShort value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%u", value);
    else
        fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(UInt value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%u", value);
    else
        fwprintf(mFile, L"%u", value);
}

void FifamWriter::WriteOne(Int64 value) {
    fwprintf(mFile, L"%I64d", value);
}

void FifamWriter::WriteOne(UInt64 value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%I64u", value);
    else
        fwprintf(mFile, L"%I64u", value);
}

void FifamWriter::WriteOne(Float value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%f", value);
    else
        fwprintf(mFile, L"%f", value);
}

void FifamWriter::WriteOne(Double value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%f", value);
    else
        fwprintf(mFile, L"%f", value);
}

void FifamWriter::WriteOne(Bool value) {
    if (mOutputStr) {
        if (value)
            *mOutputStr += L"1";
        else
            *mOutputStr += L"0";
    }
    else {
        if (value)
            fputws(L"1", mFile);
        else
            fputws(L"0", mFile);
    }
}

void FifamWriter::WriteOne(WideChar const *value) {
    String str = value;
    WriteOne(str);
}

void FifamWriter::WriteOne(Char const *value) {
    WriteOne(Utils::AtoW(value));
}

void FifamWriter::WriteOne(String const &value) {
    String str;
    if (GetGameId() >= 8)
        str = value;
    else
        str = Utils::GetStringWithoutUnicodeChars(value);
    if (str.length() > 1 && mReplaceQuotes) {
        String noqValue = str;
        for (UInt i = 0; i < str.length(); i++) {
            if (noqValue[i] == L'"')
                noqValue[i] = L'\'';
        }
        if (mOutputStr)
            *mOutputStr += noqValue;
        else
            fputws(noqValue.c_str(), mFile);
        return;
    }
    if (mOutputStr)
        *mOutputStr += str;
    else
        fputws(str.c_str(), mFile);
}

void FifamWriter::WriteOne(StringA const &value) {
    WriteOne(Utils::AtoW(value));
}

void FifamWriter::WriteOne(Hexadecimal const &value) {
    if (mOutputStr)
        *mOutputStr += Utils::Format(L"%X", value);
    else
        fwprintf(mFile, L"%X", value());
}

void FifamWriter::WriteOne(Quoted const &value) {
    WriteOne(L"\"");
    WriteOne(value());
    WriteOne(L"\"");
}

void FifamWriter::WriteOne(QuotedA const &value) {
    WriteOne(L"\"");
    WriteOne(value());
    WriteOne(L"\"");
}

void FifamWriter::WriteStartIndex(String const &name, Bool newLine) {
    if (newLine)
        WriteLine(L"%INDEX%" + name);
    else
        WriteOne(L"%INDEX%" + name);
}

void FifamWriter::WriteStartIndex(StringA const &name, Bool newLine) {
    WriteStartIndex(Utils::AtoW(name), newLine);
}

void FifamWriter::WriteEndIndex(String const &name, Bool newLine) {
    if (newLine)
        WriteLine(L"%INDEXEND%" + name);
    else
        WriteOne(L"%INDEXEND%" + name);
}

void FifamWriter::WriteEndIndex(StringA const &name, Bool newLine) {
    WriteEndIndex(Utils::AtoW(name), newLine);
}

void FifamWriter::WriteOne(FifamDate const &date) {
    UChar day = date.day;
    UChar month = date.month;
    UShort year = date.year;
    if (year == 0) {
        if (GetGameId() >= 11) {
            day = 0;
            month = 0;
        }
        else {
            year = 2000;
            day = 1;
            month = 1;
        }
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

void FifamWriter::WriteOne(Date const &date) {
    WriteOne(Utils::Format(L"%02d.%02d.%04d", date.day, date.month, date.year));
}

void FifamWriter::WriteVersion() {
    WriteStartIndex(L"VERSION");
    WriteLine(mVersion.GetIntValue());
    WriteEndIndex(L"VERSION");
}

void FifamWriter::WriteTranslationArray(FifamTrArray<String> const &ary, Bool quoted, WideChar sep) {
    UInt num_tr = 6;
    if (IsVersionGreaterOrEqual(0x2007, 0x1A))
        num_tr = 6;
    else if (IsVersionGreaterOrEqual(0x2005, 0))
        num_tr = 5;
    else if (IsVersionGreaterOrEqual(0x2004, 0))
        num_tr = 4;
    else
        num_tr = 3;
    for (UInt i = 0; i < num_tr; i++) {
        if (quoted)
            WriteOne(L"\"");
        WriteOne(ary[i]);
        if (quoted)
            WriteOne(L"\"");
        if (i != num_tr - 1)
            WriteOne(sep);
    }
}

void FifamWriter::WriteLineTranslationArray(FifamTrArray<String> const &ary, Bool quoted, WideChar sep) {
    WriteTranslationArray(ary, quoted, sep);
    WriteOne(L"\n");
}

void FifamWriter::WriteNewLine() {
    WriteOne(L"\n");
}

void FifamReader::Open(Path const &filename, UInt gameId, Bool linesWithComments, Bool removeQuotes) {
    mGameId = gameId;
    mRemoveQuotes = removeQuotes;
    FILE *file = _wfopen(filename.c_str(), L"rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        Long fileSizeWithBom = ftell(file);
        fseek(file, 0, SEEK_SET);

        enum class encoding { ascii, utf8, utf16le, utf16be } enc = encoding::ascii;

        Long numBytesToSkip = 0;
        if (fileSizeWithBom >= 2) {
            unsigned char bom[3];
            bom[0] = 0;
            fread(&bom, 1, 2, file);
            fseek(file, 0, SEEK_SET);
            if (bom[0] == 0xFE && bom[1] == 0xFF) {
                enc = encoding::utf16be;
                numBytesToSkip = 2;
            }
            else if (bom[0] == 0xFF && bom[1] == 0xFE) {
                enc = encoding::utf16le;
                numBytesToSkip = 2;
            }
            else if (fileSizeWithBom >= 3) {
                bom[0] = 0;
                fread(&bom, 1, 3, file);
                fseek(file, 0, SEEK_SET);
                if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
                    enc = encoding::utf8;
                    numBytesToSkip = 3;
                }
            }
        }
        Long totalSize = fileSizeWithBom - numBytesToSkip;
        Char *fileData = new Char[totalSize];
        fseek(file, numBytesToSkip, SEEK_SET);
        fread(fileData, 1, totalSize, file);
        fclose(file);
        Long numWideChars = 0;
        switch (enc) {
        case encoding::ascii:
            numWideChars = totalSize;
            break;
        case encoding::utf8:
            numWideChars = MultiByteToWideChar(CP_UTF8, 0, fileData, totalSize, 0, 0);
            break;
        case encoding::utf16le:
        case encoding::utf16be:
            numWideChars = totalSize / 2;
            break;
        }

        WideChar *data = new WideChar[numWideChars];
        memset(data, 0, numWideChars * sizeof(WideChar));

        switch (enc) {
        case encoding::ascii:
            MultiByteToWideChar(0, 0, fileData, totalSize, data, numWideChars);
            break;
        case encoding::utf8:
            MultiByteToWideChar(CP_UTF8, 0, fileData, totalSize, data, numWideChars);
            break;
        case encoding::utf16le:
        case encoding::utf16be:
            memcpy(data, fileData, totalSize);
            break;
        }
        delete[] fileData;

        if (enc == encoding::utf16be) {
            for (Long i = 0; i < numWideChars; i++)
                data[i] = (data[i] >> 8) | (data[i] << 8);
        }

        String currentLine;
        Bool inComment = false;
        for (Long i = 0; i < numWideChars; i++) {
            if (data[i] == L'\n') {
                if (inComment)
                    inComment = false;
                else {
                    mLines.push_back(currentLine);
                    currentLine.clear();
                }
            }
            else if (data[i] == L'\r') {
                if ((i + 1) < numWideChars && data[i + 1] == L'\n')
                    i++;
                if (inComment)
                    inComment = false;
                else {
                    mLines.push_back(currentLine);
                    currentLine.clear();
                }
            }
            else if (!inComment) {
                if (data[i] == L';' && (i == 0 || linesWithComments)) {
                    if (!currentLine.empty()) {
                        mLines.push_back(currentLine);
                        currentLine.clear();
                    }
                    inComment = true;
                }
                else
                    currentLine += data[i];
            }
        }
        if (!inComment)
            mLines.push_back(currentLine);
        delete[] data;
    }
}

void FifamReader::Open(Path const &filename, UInt gameId, FifamVersion const &version, Bool linesWithComments, Bool removeQuotes) {
    Open(filename, gameId, linesWithComments, removeQuotes);
    mVersion = version;
}

void FifamReader::Open(String *inputString, UInt gameId, Bool linesWithComments, Bool removeQuotes) {
    mGameId = gameId;
    mRemoveQuotes = removeQuotes;
    String currentLine;
    Bool inComment = false;
    for (UInt i = 0; i < inputString->length(); i++) {
        if ((*inputString)[i] == L'\n') {
            if (inComment)
                inComment = false;
            else {
                mLines.push_back(currentLine);
                currentLine.clear();
            }
        }
        else if ((*inputString)[i] == L'\r') {
            if ((i + 1) < inputString->length() && (*inputString)[i + 1] == L'\n')
                i++;
            if (inComment)
                inComment = false;
            else {
                mLines.push_back(currentLine);
                currentLine.clear();
            }
        }
        else if (!inComment) {
            if ((*inputString)[i] == L';' && (i == 0 || linesWithComments)) {
                if (!currentLine.empty()) {
                    mLines.push_back(currentLine);
                    currentLine.clear();
                }
                inComment = true;
            }
            else
                currentLine += (*inputString)[i];
        }
    }
    if (!inComment)
        mLines.push_back(currentLine);
}

void FifamReader::Open(String *inputString, UInt gameId, FifamVersion const &version, Bool linesWithComments, Bool removeQuotes) {
    Open(inputString, gameId, linesWithComments, removeQuotes);
    mVersion = version;
}

FifamReader::FifamReader(Path const &filename, UInt gameId, Bool linesWithComments, Bool removeQuotes) {
    Open(filename, gameId, linesWithComments, removeQuotes);
}

FifamReader::FifamReader(Path const &filename, UInt gameId, FifamVersion const &version, Bool linesWithComments, Bool removeQuotes) {
    Open(filename, gameId, version, linesWithComments, removeQuotes);
}

FifamReader::FifamReader(String *inputString, UInt gameId, Bool linesWithComments, Bool removeQuotes) {
    Open(inputString, gameId, linesWithComments, removeQuotes);
}

FifamReader::FifamReader(String *inputString, UInt gameId, FifamVersion const &version, Bool linesWithComments, Bool removeQuotes) {
    Open(inputString, gameId, version, linesWithComments, removeQuotes);
}

FifamReader::FifamReader(Path const &filename) : FifamReader(filename, 14) {}

FifamReader::FifamReader(String *inputString) : FifamReader(inputString, 14) {}

FifamReader::~FifamReader() {
    Close();
}

void FifamReader::Close() {
    mLines.clear();
    mCurrentLine = 0;
}

Bool FifamReader::Available() {
    return !mLines.empty();
}

Bool FifamReader::IsEof() {
    return GetPosition() == GetSize();
}

UInt FifamReader::GetPosition() {
    return mCurrentLine;
}

void FifamReader::SetPosition(UInt pos) {
    mCurrentLine = pos;
}

UInt FifamReader::GetSize() {
    return mLines.size();
}

WideChar const *FifamReader::GetLine() {
    if (mCurrentLine < mLines.size())
        return mLines[mCurrentLine++].c_str();
    return nullptr;
}

Bool FifamReader::GetLine(String &out) {
    auto line = GetLine();
    if (line) {
        out = line;
        return true;
    }
    out.clear();
    return false;
}

Bool FifamReader::GetLine(StringA &out) {
    String outW;
    if (GetLine(outW)) {
        out = Utils::WtoA(outW);
        return true;
    }
    out.clear();
    return false;
}

Bool FifamReader::EmptyLine() {
    if (mCurrentLine < mLines.size())
        return mLines[mCurrentLine].empty();
    return true;
}

Bool FifamReader::CheckLine(String const &str, Bool skipIfTrue) {
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

Bool FifamReader::CheckLine(StringA const &str, Bool skipIfTrue) {
    return CheckLine(Utils::AtoW(str), skipIfTrue);
}

Bool FifamReader::FindLine(String const &str, Bool skipIfFound, Bool moveToEofIfNotFound) {
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

Bool FifamReader::FindLine(StringA const &str, Bool skipIfFound, Bool moveToEofIfNotFound) {
    return FindLine(Utils::AtoW(str), skipIfFound, moveToEofIfNotFound);
}

void FifamReader::SkipLines(UInt count) {
    for (UInt i = 0; i < count; i++)
        GetLine();
}

void FifamReader::SkipLine() {
    SkipLines(1);
}

Bool FifamReader::ReadStartIndex(String const &name, Bool moveToEofIfNotFound) {
    return FindLine(L"%INDEX%" + name, true, moveToEofIfNotFound);
}

Bool FifamReader::ReadStartIndex(StringA const &name, Bool moveToEofIfNotFound) {
    return ReadStartIndex(name, moveToEofIfNotFound);
}

Bool FifamReader::ReadEndIndex(String const &name, Bool moveToEofIfNotFound) {
    return FindLine(L"%INDEXEND%" + name, true, moveToEofIfNotFound);
}

Bool FifamReader::ReadEndIndex(StringA const &name, Bool moveToEofIfNotFound) {
    return ReadEndIndex(name, moveToEofIfNotFound);
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

void FifamReader::StrToArg(String const &str, Bool &arg) {
    arg = str.empty() ? false : Utils::SafeConvertInt<Bool>(str);
}

void FifamReader::StrToArg(String const &str, UChar &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UChar>(str);
}

void FifamReader::StrToArg(String const &str, Char &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<Char>(str);
}

void FifamReader::StrToArg(String const &str, UShort &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UShort>(str);
}

void FifamReader::StrToArg(String const &str, Short &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<Short>(str);
}

void FifamReader::StrToArg(String const &str, UInt &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UInt>(str);
}

void FifamReader::StrToArg(String const &str, Int &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UInt>(str);
}

void FifamReader::StrToArg(String const &str, UInt64 &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UInt64>(str);
}

void FifamReader::StrToArg(String const &str, Int64 &arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<Int64>(str);
}

void FifamReader::StrToArg(String const &str, Float &arg) {
    arg = str.empty() ? 0.0f : Utils::SafeConvertFloat(str);
}

void FifamReader::StrToArg(String const &str, Double &arg) {
    arg = str.empty() ? 0.0 : Utils::SafeConvertDouble(str);
}

void FifamReader::StrToArg(String const &str, WideChar *arg) {
    wcscpy(arg, str.c_str());
}

void FifamReader::StrToArg(String const &str, Char *arg) {
    strcpy(arg, Utils::WtoA(str).c_str());
}

void FifamReader::StrToArg(String const &str, String &arg) {
    arg = str;
}

void FifamReader::StrToArg(String const &str, StringA &arg) {
    arg = Utils::WtoA(str);
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

void FifamReader::StrToArg(String const &str, Date &arg) {
    auto dateInfo = Utils::Split(str, L'.');
    if (dateInfo.size() == 3) {
        arg.year = Utils::SafeConvertInt<Short>(dateInfo[2]);
        arg.month = Utils::SafeConvertInt<Char>(dateInfo[1]);
        arg.day = Utils::SafeConvertInt<Char>(dateInfo[0]);
        return;
    }
    arg.MakeEmpty();
}

void FifamReader::StrToArg(String const &str, Hexadecimal arg) {
    arg = str.empty() ? 0 : Utils::SafeConvertInt<UInt>(str, true);
}

void FifamReader::StrToArg(String const &str, Quoted arg) {
    String modstr = str;
    if (mRemoveQuotes)
        RemoveQuotes(modstr);
    arg = modstr;
}

void FifamReader::StrToArg(String const &str, QuotedA arg) {
    String modstr = str;
    if (mRemoveQuotes)
        RemoveQuotes(modstr);
    arg = Utils::WtoA(modstr);
}

void FifamReader::StrToArg(String const &str, OptionalInt arg) {
    String trimmed = str;
    Utils::Trim(trimmed);
    if (!trimmed.empty())
        arg = Utils::SafeConvertInt<UInt>(str);
}

Bool FifamReader::ReadVersion() {
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

StringA FifamReader::ReadFullLineA() {
    auto line = GetLine();
    if (line)
        return Utils::WtoA(line);
    return StringA();
}

Bool FifamReader::ReadFullLine(String &out) {
    auto line = GetLine();
    if (line) {
        out = line;
        return true;
    }
    out.clear();
    return false;
}

Bool FifamReader::ReadFullLine(StringA &out) {
    auto line = GetLine();
    if (line) {
        out = Utils::WtoA(line);
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

UInt FifamReader::ReadLineTranslationArray(FifamTrArray<String> &out, WideChar sep) {
    auto ary = ReadLineArray<String>(sep);
    if (ary.size() == 1) {
        for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++)
            out[i] = ary[0];
    }
    else {
        for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
            if (ary.size() > i)
                out[i] = ary[i];
            else
                out[i].clear();
        }
        if (!IsVersionGreaterOrEqual(0x2004, 0))
            out[FifamTranslation::Spanish] = out[FifamTranslation::English];
        if (!IsVersionGreaterOrEqual(0x2005, 0))
            out[FifamTranslation::Italian] = out[FifamTranslation::English];
        if (!IsVersionGreaterOrEqual(0x2007, 0x1A))
            out[FifamTranslation::Polish] = out[FifamTranslation::English];
    }
    return ary.size();
}
