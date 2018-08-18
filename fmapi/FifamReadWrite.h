#pragma once

#include "FifamVersion.h"
#include "FifamDate.h"
#include "Utils.h"
#include "Hexademical.h"
#include "Quoted.h"
#include "FifamTranslation.h"

class FifamEnum;
class FifamBaseFlags;

class Utilities {
public:
    static void skipBom_UTF8(FILE *file);
    static unsigned int extractClubId(wchar_t *str);
    static unsigned int getCountryId(unsigned int uid);
    static unsigned int getAdditionalFlags(unsigned int uid);
    static unsigned int getClubIdOnly(unsigned int uid);
    static void RemoveNewLine(wchar_t *line);
    static void CopyStr(wchar_t *dest, wchar_t *src, unsigned int maxLen);
    static void FixWrongCharacters(wchar_t *str);
    static wchar_t *RemoveAccented(wchar_t *str);
    static char *RemoveAccented(char *dest, wchar_t *str);
    static void SkipLines(size_t num, FILE *file);
    static void ReadLine(wchar_t *line, size_t num, FILE *file);
    static void ReadLine(wchar_t *line, FILE *file);
};

class FifamFileWorker {
protected:
    FILE *mFile = nullptr;
    size_t mGameId = 0;
    FifamVersion mVersion;

public:
    bool IsVersionGreaterOrEqual(unsigned short year, unsigned short number);
    FifamFileWorker(size_t gameId);
    ~FifamFileWorker();
    void Close();
    size_t GetGameId();
    FifamVersion GetVersion();
    bool Available();
};

class FifamWriter : public FifamFileWorker {
public:
    FifamWriter(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber, bool unicode = true);
    void WriteOne(char value);
    void WriteOne(wchar_t value);
    void WriteOne(short value);
    void WriteOne(int value);
    void WriteOne(unsigned char value);
    void WriteOne(unsigned short value);
    void WriteOne(unsigned int value);
    void WriteOne(long long int value);
    void WriteOne(unsigned long long int value);
    void WriteOne(float value);
    void WriteOne(double value);
    void WriteOne(bool value);
    void WriteOne(wchar_t const *value);
    void WriteOne(String const &value);
    void WriteOne(Hexademical const &value);
    void WriteOne(Quoted const &value);

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

    void WriteStartIndex(String const &name, bool newLine = true);
    void WriteEndIndex(String const &name, bool newLine = true);
    void WriteOne(FifamDate const &date);
    void WriteVersion();
private:
    template<typename One>
    void WriteArgs(char sep, One &&arg) {
        WriteOne(std::forward<One>(arg));
    }

    template<typename One, typename... Other>
    void WriteArgs(char sep, One&& arg, Other&&... other) {
        WriteArgs(sep, std::forward<One>(arg));
        WriteOne(Utils::CharToStr(sep));
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
    void WriteLineWithSeparator(char sep, ArgTypes&&... args) {
        WriteArgs(sep, std::forward<ArgTypes>(args)...);
        WriteOne(L"\n");
    }

    template<typename Container, typename T = typename Container::value_type>
    void WriteArray(Container const &ary, wchar_t sep = L',') {
        for (size_t i = 0; i < ary.size(); i++) {
            WriteOne(ary[i]);
            if (i != ary.size() - 1)
                WriteOne(sep);
        }
    }

    template<typename Container, typename T = typename Container::value_type>
    void WriteLineArray(Container const &ary, wchar_t sep = L',') {
        WriteArray(ary, sep);
        WriteOne(L"\n");
    }

    template<typename T>
    void WriteTranslationArray(FifamTrArray<T> const &ary, wchar_t sep = L',') {
        Vector<T> vec;
        for (size_t i = 0; i < ary.size(); i++) {
            if (i < 5 || IsVersionGreaterOrEqual(0x2007, 0x1A))
                vec.push_back(ary[i]);
        }
        WriteArray(vec, sep);
    }

    template<typename T>
    void WriteLineTranslationArray(FifamTrArray<T> const &ary, wchar_t sep = L',') {
        WriteTranslationArray(ary, sep);
        WriteOne(L"\n");
    }

    void WriteTranslationArray(FifamTrArray<String> const &ary, bool quoted = true, wchar_t sep = L',');
    void WriteLineTranslationArray(FifamTrArray<String> const &ary, bool quoted = true, wchar_t sep = L',');

    template<typename T>
    void WritePackedLineArray(Vector<T> const &ary, wchar_t sep = L',') {
        WriteOne(L"{ ");
        for (size_t i = 0; i < ary.size(); i++) {
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
    static const size_t BUFFER_SIZE = 4096;
    wchar_t mLine[BUFFER_SIZE];
public:
    bool IsEof();
    FifamReader(Path const &filename, size_t gameId, bool unicode = true);
    FifamReader(Path const &filename, size_t gameId, unsigned short vYear, unsigned short vNumber);
private:
    wchar_t *GetLine();
public:
    bool CheckLine(String const &str, bool skipIfTrue = false);
    bool FindLine(String const &str, bool skipIfFound = false, bool moveToEofIfNotFound = false);
    void SkipLines(size_t count);
    void SkipLine();
    bool ReadStartIndex(String const &name, bool moveToEofIfNotFound = false);
    bool ReadEndIndex(String const &name, bool moveToEofIfNotFound = true);
    void ReadLine(FifamDate &date);
private:
    void StrToArg(String const &str, unsigned char &arg);
    void StrToArg(String const &str, char &arg);
    void StrToArg(String const &str, unsigned short &arg);
    void StrToArg(String const &str, short &arg);
    void StrToArg(String const &str, unsigned int &arg);
    void StrToArg(String const &str, int &arg);
    void StrToArg(String const &str, unsigned long long int &arg);
    void StrToArg(String const &str, long long int &arg);
    void StrToArg(String const &str, float &arg);
    void StrToArg(String const &str, double &arg);
    void StrToArg(String const &str, wchar_t *arg);
    void StrToArg(String const &str, String &arg);
    void StrToArg(String const &str, FifamDate &arg);
    void StrToArg(String const &str, Hexademical arg);
    void StrToArg(String const &str, Quoted arg);

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
    void ReadOneArg(Vector<String> strArgs, size_t &currArg, One &&one) {
        if (strArgs.size() > currArg)
            StrToArg(strArgs[currArg++], std::forward<One>(one));
        else
            StrToArg(String(), std::forward<One>(one));
    }

    template<typename One, typename... Other>
    void ReadOneArg(Vector<String> strArgs, size_t &currArg, One&& one, Other&&... other) {
        ReadOneArg(strArgs, currArg, std::forward<One>(one));
        ReadOneArg(strArgs, currArg, std::forward<Other>(other)...);
    }

public:
    template<typename... ArgTypes>
    void ReadLine(ArgTypes&&... args) {
        GetLine();
        auto strArgs = Utils::Split(mLine, L',');
        size_t currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    // reads { arg, arg, ... } line
    template<typename... ArgTypes>
    void ReadPackedLine(ArgTypes&&... args) {
        GetLine();
        String line = mLine;
        auto startPos = line.find_first_of(L'{');
        if (startPos != String::npos)
            line = substr(startPos + 1);
        auto endPos = line.find_first_of(L'}');
        if (endPos != String::npos)
            line = substr(0, endPos);
        auto strArgs = Utils::Split(line, L",");
        size_t currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename... ArgTypes>
    void ReadLineWithSeparator(wchar_t sep, ArgTypes&&... args) {
        GetLine();
        auto strArgs = Utils::Split(mLine, sep);
        size_t currArg = 0;
        ReadOneArg(strArgs, currArg, std::forward<ArgTypes>(args)...);
    }

    template<typename Container>
    size_t ReadLineArray(Container &out, wchar_t sep = L',') {
        GetLine();
        Vector<String> strArgs = Utils::Split(mLine, sep);
        size_t count = Utils::Min(out.size(), strArgs.size());
        for (size_t i = 0; i < count; i++)
            StrToArg(strArgs[i], out[i]);
        return Utils::Max(out.size(), strArgs.size());
    }

    template<typename T>
    size_t ReadLineTranslationArray(FifamTrArray<T> &out, wchar_t sep = L',') {
        auto result = ReadLineArray(out, sep);
        if (!IsVersionGreaterOrEqual(0x2007, 0x1A) && out.size() >= FifamTranslation::NUM_TRANSLATIONS)
            out[FifamTranslation::Polish] = out[FifamTranslation::English];
        return result;
    }

    void RemoveQuotes(String &str);
    size_t ReadLineTranslationArray(FifamTrArray<String> &out, wchar_t sep = L',');

    template<typename T>
    Vector<T> ReadLineArray(wchar_t sep = L',', bool skipEmpty = false) {
        GetLine();
        Vector<T> ary;
        Vector<String> strArgs = Utils::Split(mLine, sep, skipEmpty);
        ary.resize(strArgs.size());
        for (size_t i = 0; i < strArgs.size(); i++)
            StrToArg(strArgs[i], ary[i]);
        return ary;
    }

    template<typename T>
    Vector<T> ReadPackedLineArray(wchar_t sep = L',') {
        GetLine();
        String line = mLine;
        auto startPos = line.find_first_of(L'{');
        if (startPos != String::npos)
            line = line.substr(startPos + 1);
        auto endPos = line.find_first_of(L'}');
        if (endPos != String::npos)
            line = line.substr(0, endPos);
        Vector<T> ary;
        Vector<String> strArgs = Utils::Split(line, sep);
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

    bool ReadVersion();

    String ReadFullLine();
    bool ReadFullLine(String &out);
};
