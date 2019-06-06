#pragma once

#include "FifamVersion.h"
#include "FifamDate.h"
#include "Utils.h"
#include "Hexadecimal.h"
#include "Quoted.h"
#include "IntPtr.h"
#include "OptionalInt.h"
#include "FifamTranslation.h"
#include <iostream>

class FifamEnum;
class FifamBaseFlags;

class FifamFileWorker {
protected:
    UInt mGameId = 0;
    FifamVersion mVersion;
public:
    Bool IsVersionGreaterOrEqual(UShort year, UShort number);
    FifamFileWorker();
    FifamFileWorker(UInt gameId);
    ~FifamFileWorker();
    UInt GetGameId();
    FifamVersion GetVersion();
    void SetVersion(FifamVersion const &version);
    void SetVersion(UShort year, UShort number);
};

class FifamWriter : public FifamFileWorker {
    FILE *mFile = nullptr;
    String *mOutputStr = nullptr;
    Bool mUnicode = true;
    Bool mReplaceQuotes = true;
public:
    FifamWriter(Path const &filename, UInt gameId, FifamVersion const &version, Bool unicode = true);
    FifamWriter(String *outputString, UInt gameId, FifamVersion const &version, Bool unicode = true);
    void SetReplaceQuotes(bool replace);
    ~FifamWriter();
    void Close();
    Bool Available();
    void WriteOne(Char value);
    void WriteOne(WideChar value);
    void WriteOne(Short value);
    void WriteOne(Int value);
    void WriteOne(UChar value);
    void WriteOne(UShort value);
    void WriteOne(UInt value);
    void WriteOne(Int64 value);
    void WriteOne(UInt64 value);
    void WriteOne(Float value);
    void WriteOne(Double value);
    void WriteOne(Bool value);
    void WriteOne(WideChar const *value);
    void WriteOne(String const &value);
    void WriteOne(Hexadecimal const &value);
    void WriteOne(Quoted const &value);
    void WriteOne(FifamDate const &date);
    void WriteOne(Date const &date);

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    void WriteOne(T const &value) {
        WriteOne(static_cast<std::underlying_type_t<T>>(Utils::ToInt(value)));
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<FifamEnum, T>>, typename = void>
    void WriteOne(T value) {
        value.Write(*this);
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<FifamBaseFlags, T>>, typename = void, typename = void>
    void WriteOne(T const &value) {
        WriteOne(value.ToInt());
    }

    void WriteStartIndex(String const &name, Bool newLine = true);
    void WriteEndIndex(String const &name, Bool newLine = true);
    void WriteVersion();
private:
    template<typename One>
    void WriteArgs(Char sep, One &&arg) {
        WriteOne(std::forward<One>(arg));
    }

    template<typename One, typename... Other>
    void WriteArgs(Char sep, One&& arg, Other&&... other) {
        WriteArgs(sep, std::forward<One>(arg));
        WriteOne(Utils::CharToStr(sep));
        WriteArgs(sep, std::forward<Other>(other)...);
    }

    template<typename One>
    void WriteArgs(String sep, One &&arg) {
        WriteOne(std::forward<One>(arg));
    }

    template<typename One, typename... Other>
    void WriteArgs(String sep, One&& arg, Other&&... other) {
        WriteArgs(sep, std::forward<One>(arg));
        WriteOne(sep);
        WriteArgs(sep, std::forward<Other>(other)...);
    }

public:
    template <typename... ArgTypes>
    void Write(ArgTypes&&... args) {
        WriteArgs(',', std::forward<ArgTypes>(args)...);
    }

    template <typename... ArgTypes>
    void WriteLine(ArgTypes&&... args) {
        WriteArgs(',', std::forward<ArgTypes>(args)...);
        WriteOne(L"\n");
    }

    // writes { arg, arg, ... } line
    template<typename... ArgTypes>
    void WritePackedLine(ArgTypes&&... args) {
        WriteOne(L"{ ");
        WriteArgs(',', std::forward<ArgTypes>(args)...);
        WriteOne(L" }");
        WriteOne(L"\n");
    }

    template <typename... ArgTypes>
    void WriteLineWithSeparator(Char sep, ArgTypes&&... args) {
        WriteArgs(sep, std::forward<ArgTypes>(args)...);
        WriteOne(L"\n");
    }

    template <typename... ArgTypes>
    void WriteLineWithSeparator(String sep, ArgTypes&&... args) {
        WriteArgs(sep, std::forward<ArgTypes>(args)...);
        WriteOne(L"\n");
    }

    template<typename Container, typename T = typename Container::value_type>
    void WriteArray(Container const &ary, WideChar sep = L',') {
        for (UInt i = 0; i < ary.size(); i++) {
            WriteOne(ary[i]);
            if (i != ary.size() - 1)
                WriteOne(sep);
        }
    }

    template<typename Container, typename T = typename Container::value_type>
    void WriteLineArray(Container const &ary, WideChar sep = L',') {
        WriteArray(ary, sep);
        WriteOne(L"\n");
    }

    template<typename T>
    void WriteTranslationArray(FifamTrArray<T> const &ary, WideChar sep = L',') {
        Vector<T> vec;
        UInt num_tr = 6;
        if (!IsVersionGreaterOrEqual(0x2007, 0x1A))
            num_tr = 5;
        for (UInt i = 0; i < num_tr; i++)
            vec.push_back(ary[i]);
        WriteArray(vec, sep);
    }

    template<typename T>
    void WriteLineTranslationArray(FifamTrArray<T> const &ary, WideChar sep = L',') {
        WriteTranslationArray(ary, sep);
        WriteOne(L"\n");
    }

    void WriteTranslationArray(FifamTrArray<String> const &ary, Bool quoted = true, WideChar sep = L',');
    void WriteLineTranslationArray(FifamTrArray<String> const &ary, Bool quoted = true, WideChar sep = L',');

    template<typename T>
    void WritePackedLineArray(Vector<T> const &ary, WideChar sep = L',') {
        WriteOne(L"{ ");
        for (UInt i = 0; i < ary.size(); i++) {
            WriteOne(ary[i]);
            if (i != ary.size() - 1)
                WriteOne(sep);
        }
        WriteOne(L" }");
        WriteOne(L"\n");
    }

    void WriteNewLine();
};

class FifamReader : public FifamFileWorker {
    UInt mCurrentLine = 0;
    Vector<String> mLines;
    Bool mRemoveQuotes = true;
public:
    void Close();
    Bool Available();
    Bool IsEof();
    UInt GetPosition();
    void SetPosition(UInt pos);
    UInt GetSize();
    FifamReader(Path const &filename, UInt gameId, Bool linesWithComments = true, Bool removeQuotes = true);
    FifamReader(Path const &filename, UInt gameId, FifamVersion const &version, Bool linesWithComments = true, Bool removeQuotes = true);
    FifamReader(String *inputString, UInt gameId, Bool linesWithComments = true, Bool removeQuotes = true);
    FifamReader(String *inputString, UInt gameId, FifamVersion const &version, Bool linesWithComments = true, Bool removeQuotes = true);
    void Open(Path const &filename, UInt gameId, Bool linesWithComments = true, Bool removeQuotes = true);
    void Open(Path const &filename, UInt gameId, FifamVersion const &version, Bool linesWithComments = true, Bool removeQuotes = true);
    void Open(String *inputString, UInt gameId, Bool linesWithComments = true, Bool removeQuotes = true);
    void Open(String *inputString, UInt gameId, FifamVersion const &version, Bool linesWithComments = true, Bool removeQuotes = true);
    ~FifamReader();
private:
    WideChar const *GetLine();
    Bool GetLine(String &out);
public:
    Bool CheckLine(String const &str, Bool skipIfTrue = false);
    Bool FindLine(String const &str, Bool skipIfFound = false, Bool moveToEofIfNotFound = false);
    void SkipLines(UInt count);
    void SkipLine();
    Bool EmptyLine();
    Bool ReadStartIndex(String const &name, Bool moveToEofIfNotFound = false);
    Bool ReadEndIndex(String const &name, Bool moveToEofIfNotFound = true);
    void ReadLine(FifamDate &date);
private:
    void StrToArg(String const &str, Bool &arg);
    void StrToArg(String const &str, UChar &arg);
    void StrToArg(String const &str, Char &arg);
    void StrToArg(String const &str, UShort &arg);
    void StrToArg(String const &str, Short &arg);
    void StrToArg(String const &str, UInt &arg);
    void StrToArg(String const &str, Int &arg);
    void StrToArg(String const &str, UInt64 &arg);
    void StrToArg(String const &str, Int64 &arg);
    void StrToArg(String const &str, Float &arg);
    void StrToArg(String const &str, Double &arg);
    void StrToArg(String const &str, WideChar *arg);
    void StrToArg(String const &str, String &arg);
    void StrToArg(String const &str, FifamDate &arg);
    void StrToArg(String const &str, Hexadecimal arg);
    void StrToArg(String const &str, Quoted arg);
    void StrToArg(String const &str, OptionalInt arg);
    void StrToArg(String const &str, Date &arg);
    template<typename T>
    void StrToArg(String const &str, IntPtr<T> const &arg) {
        arg = str.empty() ? 0 : Utils::SafeConvertInt<UInt>(str);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    void StrToArg(String const &str, T &arg) {
        arg = Utils::FromInt<T>(str.empty() ? 0 : Utils::SafeConvertInt<std::underlying_type_t<T>>(str));
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<FifamEnum, T>>, typename = void>
    void StrToArg(String const &str, T &arg) {
        arg.Read(*this, str);
    }

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<FifamBaseFlags, T>>, typename = void, typename = void>
    void StrToArg(String const &str, T &arg) {
        if (!str.empty())
            arg.SetFromInt(Utils::SafeConvertInt<typename T::underlyingtype_t>(str));
        else
            arg.Clear();
    }

    template<typename One>
    void ReadOneArg(Vector<String> strArgs, UInt &currArg, One &&one) {
        if (strArgs.size() > currArg)
            StrToArg(strArgs[currArg++], std::forward<One>(one));
        else
            StrToArg(String(), std::forward<One>(one));
    }

    template<typename One, typename... Other>
    void ReadOneArg(Vector<String> strArgs, UInt &currArg, One&& one, Other&&... other) {
        ReadOneArg(strArgs, currArg, std::forward<One>(one));
        ReadOneArg(strArgs, currArg, std::forward<Other>(other)...);
    }

public:
    template<typename... ArgTypes>
    void ReadLine(ArgTypes&&... args) {
        String line;
        GetLine(line);
        auto strArgs = Utils::Split(line, L',');
        UInt currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    // reads { arg, arg, ... } line
    template<typename... ArgTypes>
    void ReadPackedLine(ArgTypes&&... args) {
        String line;
        GetLine(line);
        auto startPos = line.find_first_of(L'{');
        if (startPos != String::npos)
            line = substr(startPos + 1);
        auto endPos = line.find_first_of(L'}');
        if (endPos != String::npos)
            line = substr(0, endPos);
        auto strArgs = Utils::Split(line, L',');
        UInt currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename... ArgTypes>
    void ReadLineWithSeparator(WideChar sep, ArgTypes&&... args) {
        String line;
        GetLine(line);
        auto strArgs = Utils::Split(line, sep);
        UInt currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename Container>
    UInt ReadLineArray(Container &out, WideChar sep = L',') {
        String line;
        GetLine(line);
        Vector<String> strArgs = Utils::Split(line, sep);
        UInt count = Utils::Min(out.size(), strArgs.size());
        for (UInt i = 0; i < count; i++)
            StrToArg(strArgs[i], out[i]);
        return Utils::Max(out.size(), strArgs.size());
    }

    template<typename T>
    UInt ReadLineTranslationArray(FifamTrArray<T> &out, WideChar sep = L',') {
        auto result = ReadLineArray(out, sep);
        if (!IsVersionGreaterOrEqual(0x2007, 0x1A) && out.size() >= FifamTranslation::NUM_TRANSLATIONS)
            out[FifamTranslation::Polish] = out[FifamTranslation::English];
        return result;
    }

    void RemoveQuotes(String &str);
    UInt ReadLineTranslationArray(FifamTrArray<String> &out, WideChar sep = L',');

    template<typename T>
    Vector<T> ReadLineArray(WideChar sep = L',', Bool skipEmpty = false) {
        String line;
        GetLine(line);
        Vector<T> ary;
        Vector<String> strArgs = Utils::Split(line, sep, true, skipEmpty);
        ary.resize(strArgs.size());
        for (UInt i = 0; i < strArgs.size(); i++)
            StrToArg(strArgs[i], ary[i]);
        return ary;
    }

    template<typename T>
    Vector<T> ReadPackedLineArray(WideChar sep = L',') {
        String line;
        GetLine(line);
        auto startPos = line.find_first_of(L'{');
        if (startPos != String::npos)
            line = line.substr(startPos + 1);
        auto endPos = line.find_first_of(L'}');
        if (endPos != String::npos)
            line = line.substr(0, endPos);
        Vector<T> ary;
        Vector<String> strArgs = Utils::Split(line, sep);
        ary.resize(strArgs.size());
        for (UInt i = 0; i < strArgs.size(); i++)
            StrToArg(strArgs[i], ary[i]);
        return ary;
    }

    template<typename T>
    T ReadLine() {
        T out;
        ReadLine(out);
        return out;
    }

    Bool ReadVersion();

    String ReadFullLine();
    Bool ReadFullLine(String &out);
};
