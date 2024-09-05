#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <filesystem>

namespace Utils {
    std::wstring TimeString(std::time_t const &time);
    std::wstring TimeDifferenceSting(std::time_t const &timeEnd, std::time_t const &timeBegin);
    std::wstring CurrentTime();
    std::wstring GetQuickName(std::wstring const &firstName, std::wstring const &lastName, std::wstring const &commonName);
    std::wstring CharToStr(wchar_t c);

    std::string TimeStringA(std::time_t const &time);
    std::string TimeDifferenceStingA(std::time_t const &timeEnd, std::time_t const &timeBegin);
    std::string CurrentTimeA();
    std::string GetQuickNameA(std::string const &firstName, std::string const &lastName, std::string const &commonName);
    std::string CharToStrA(char c);

    class FormatterUtilsUnicode {
    public:
        template<typename T> static T const &Arg(T const &arg) { return arg; }
        static wchar_t const *Arg(std::wstring const &arg) { return arg.c_str(); }
        static wchar_t const *Arg(std::filesystem::path const &arg) { return arg.c_str(); }
    };

    class FormatterUtilsAscii {
    public:
        template<typename T> static T const &Arg(T const &arg) { return arg; }
        static char const *Arg(std::string const &arg) { return arg.c_str(); }
        static char const *Arg(std::filesystem::path const &arg) { return arg.string().c_str(); }
    };

    template<typename ...ArgTypes>
    static std::wstring Format(const std::wstring &format, ArgTypes... args) {
        static wchar_t buf[4096];
        swprintf(buf, 4096, format.c_str(), FormatterUtilsUnicode::Arg(args)...);
        return buf;
    }

    template<typename ...ArgTypes>
    static std::string Format(const std::string &format, ArgTypes... args) {
        static char buf[4096];
        snprintf(buf, 4096, format.c_str(), FormatterUtilsAscii::Arg(args)...);
        return buf;
    }

    std::wstring AtoW(std::string const &str);
    std::string WtoA(std::wstring const &str);

    std::vector<std::wstring> Split(std::wstring const &str, wchar_t delim, bool trim = true, bool skipEmpty = false, bool quotesHavePriority = true);
    std::wstring Join(std::vector<std::wstring> const &strList, wchar_t delim);
    std::wstring Join(std::vector<std::wstring> const &strList, std::wstring const &delim);
    bool StartsWith(std::wstring const &str, std::wstring const &what);
    bool EndsWith(std::wstring const &str, std::wstring const &what);
    bool Compare(std::wstring const &str, size_t index, char c);
    bool IsNumber(std::wstring const &str, bool hexadecimal = false);
    int ToNumber(std::wstring const &str);
    void Trim(std::wstring &str);
    std::wstring ToUpper(std::wstring const &str);
    std::wstring ToLower(std::wstring const &str);
    void Replace(std::wstring& str, const std::wstring& from, const std::wstring& to);

    std::vector<std::string> Split(std::string const &str, char delim, bool trim = true, bool skipEmpty = false, bool quotesHavePriority = true);
    std::string Join(std::vector<std::string> const &strList, char delim);
    std::string Join(std::vector<std::string> const &strList, std::string const &delim);
    bool StartsWith(std::string const &str, std::string const &what);
    bool EndsWith(std::string const &str, std::string const &what);
    bool Compare(std::string const &str, size_t index, char c);
    bool IsNumber(std::string const &str, bool hexadecimal = false);
    int ToNumber(std::string const &str);
    void Trim(std::string &str);
    std::string ToUpper(std::string const &str);
    std::string ToLower(std::string const &str);
    void Replace(std::string &str, const std::string &from, const std::string &to);

    template<typename T>
    unsigned int ToInt(T value) {
        return static_cast<unsigned int>(value);
    }

    template<typename T>
    T FromInt(unsigned int value) {
        return static_cast<T>(value);
    }

    template<typename T>
    T SafeConvertInt(std::wstring const &str, bool isHex = false) {
        T result = 0;
        try {
            result = static_cast<T>(std::stoull(str, 0, isHex ? 16 : 10));
        }
        catch (...) {}
        return result;
    }

    float SafeConvertFloat(std::wstring const &str);
    double SafeConvertDouble(std::wstring const &str);

    template<typename T>
    T SafeConvertInt(std::string const &str, bool isHex = false) {
        T result = 0;
        try {
            result = static_cast<T>(std::stoull(str, 0, isHex ? 16 : 10));
        }
        catch (...) {}
        return result;
    }

    float SafeConvertFloat(std::string const &str);
    double SafeConvertDouble(std::string const &str);

    int Clamp(int value, int min, int max);
    int MapTo(int value, int input_start, int input_end, int output_start, int output_end);

    template<typename T>
    T Min(T const &a, T const &b) {
        if (a < b)
            return a;
        return b;
    }

    template<typename T>
    T Max(T const &a, T const &b) {
        if (a > b)
            return a;
        return b;
    }

    template<typename A, typename B>
    bool CheckFlag(A flagsInt, B flag) {
        return (flagsInt & flag) == flag;
    }

    template<typename A, typename B>
    void SetFlag(A &flagsInt, B flag, bool enable) {
        if (enable)
            flagsInt |= flag;
        else
            flagsInt &= ~flag;
    }

    std::wstring GetStringWithoutUnicodeChars(std::wstring const &src);

    template<typename Container, typename ItemType>
    bool Contains(Container const &container, ItemType const &item) {
        return std::find(container.begin(), container.end(), item) != container.end();
    }

	template<typename KeyType, typename ValueType>
	bool Contains(std::map<KeyType, ValueType> const &container, KeyType const &item) {
		return container.find(item) != container.end();
	}

	template<typename ItemType>
	bool Contains(std::set<ItemType> const &container, ItemType const &item) {
		return container.find(item) != container.end();
	}

    template<typename Container, typename ItemType>
    void Remove(Container &container, ItemType const &item) {
        container.erase(std::remove(container.begin(), container.end(), item), container.end());
    }

	template<typename KeyType, typename ValueType>
	void Remove(std::map<KeyType, ValueType> const &container, KeyType const &item) {
		container.erase(item);
	}

	template<typename ItemType>
	void Remove(std::set<ItemType> const &container, ItemType const &item) {
		container.erase(item);
	}

	template<typename Container, typename Callback>
	void Sort(Container &container, Callback callback) {
		std::sort(container.begin(), container.end(), callback);
	}

    template<typename T>
    std::vector<T> VecToArray(std::vector<T> const &vec, size_t size) {
        auto result = vec;
        result.resize(size);
        return result;
    }

    bool IsVowel(wchar_t wideChar);

    bool IsPunctuationMark(wchar_t wideChar);

    template<typename ValueType, typename IdType>
    std::pair<ValueType, IdType> GetMaxElementId(std::vector<std::pair<ValueType, IdType>> const &elements) {
        std::pair<ValueType, IdType> result;
        if (!elements.empty()) {
            result = elements[0];
            for (size_t i = 1; i < elements.size(); i++) {
                if (elements[i].first > result.first)
                    result = elements[i];
            }
        }
        return result;
    }

    template<typename ValueType>
    ValueType GetMaxElement(std::vector<ValueType> const &elements) {
        ValueType result = {};
        if (!elements.empty()) {
            result = elements[0];
            for (size_t i = 1; i < elements.size(); i++) {
                if (elements[i] > result)
                    result = elements[i];
            }
        }
        return result;
    }

    template<typename ValueType>
    ValueType GetMinElement(std::vector<ValueType> const &elements) {
        ValueType result;
        if (!elements.empty()) {
            result = elements[0];
            for (size_t i = 1; i < elements.size(); i++) {
                if (elements[i] < result)
                    result = elements[i];
            }
        }
        return result;
    }
};
