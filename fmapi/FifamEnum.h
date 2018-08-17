#pragma once
#include "FifamTypes.h"
#include "Utils.h"
#include "FifamReadWrite.h"
#include "Error.h"

class FifamEnum {};

#define ENUM_BEGIN(typeName, underlyingType) \
class typeName : FifamEnum { \
public: \
    using underlyingtype_t = underlyingType; \
private: \
    using _MemberType = std::pair<underlyingtype_t, wchar_t const *>; \
    using _MembersContainer = std::vector<_MemberType>; \
    underlyingtype_t _value = 0; \
    bool _wasSetFromUnkown = false; \
    underlyingtype_t _unknownValue = 0; \
\
    static bool &_hasGaps() { \
        static bool var_hasGaps = true; \
        return var_hasGaps; \
    } \
    static underlyingtype_t &_first() { \
        static underlyingtype_t var_first = 0; \
        return var_first; \
    } \
    static underlyingtype_t &_last() { \
        static underlyingtype_t var_last = 0; \
        return var_last; \
    } \
    static bool &_hasDefault() { \
        static bool var_hasDefault = false; \
        return var_hasDefault; \
    } \
    static underlyingtype_t &_defaultValue() { \
        \
        static underlyingtype_t var_defaultValue = 0; \
        return var_defaultValue; \
    } \
    static _MembersContainer &_members() { \
        static _MembersContainer m; \
        return m; \
    } \
    static underlyingtype_t _InitEnumMember(underlyingtype_t id, wchar_t const *name) { \
        _members().emplace_back(id, name); \
        return id; \
    } \
    static bool _InitEnum() { \
        _hasGaps() = true; \
        if (!_members().empty()) { \
            std::sort(_members().begin(), _members().end(), [](_MemberType const &a, _MemberType const &b) { \
                return a.first < b.first; \
            }); \
            underlyingtype_t it = _members().front().first; \
            for (auto const &m : _members()) { \
                if (m.first == it || (m.first - it) == 1) \
                    it = m.first; \
                else \
                    return true; \
            } \
            _hasGaps() = false; \
            _first() = _members().front().first; \
            _last() = _members().back().first; \
            return false; \
        } \
        return true; \
    } \
    static bool _InitDefaultValue(underlyingtype_t value) { \
        _defaultValue() = value; \
        _hasDefault() = true; \
        return true; \
    }

#define _ENUM_INITIALIZER_NAME(a,b)  a##b

#define _ENUM_MEMBER_X(id, idname, strname, ln) \
public: \
    enum : underlyingtype_t { idname = id }; \
private: \
    inline static underlyingtype_t _ENUM_INITIALIZER_NAME(_initializer, ln) = _InitEnumMember(id, strname);

#define ENUM_MEMBER(id, idname, strname) _ENUM_MEMBER_X(id, idname, strname, __LINE__)

#define ENUM_DEFAULT_VALUE(idname) \
    inline static bool _initializer1x = _InitDefaultValue(idname);

#define ENUM_END(typeName) \
private: \
    inline static bool _initializer0x = _InitEnum(); \
public: \
    typeName() { \
        if (_hasDefault()) \
            _value = _defaultValue(); \
        else \
            _value = _first(); \
    } \
    static bool Present(underlyingtype_t value) { \
        if (!_hasGaps()) \
            return value >= _first() && value <= _last(); \
        for (auto const &m : _members()) { \
            if (m.first == value) \
                return true; \
        } \
        return false; \
    } \
    underlyingtype_t ToInt() const { return _value; } \
    std::wstring ToStr() const { \
        return ToCStr(); \
    } \
    wchar_t const *ToCStr() const { \
        for (auto const &m : _members()) { \
            if (m.first == _value) \
                return m.second; \
        } \
        return L""; \
    } \
    bool GetWasSetFromUnknown() const { return _wasSetFromUnkown; } \
    void SetUnknown(underlyingtype_t value) { \
        _wasSetFromUnkown = true; \
        _unknownValue = value; \
    } \
    underlyingtype_t GetUnknown() const { return _unknownValue; } \
    bool SetFromInt(underlyingtype_t value) { \
        if (Present(value)) { \
            _value = value; \
            _wasSetFromUnkown = false; \
            return true; \
        } \
        if (_hasDefault()) \
            _value = _defaultValue(); \
        _wasSetFromUnkown = true; \
        _unknownValue = value; \
        return false; \
    } \
    bool SetFromInt(underlyingtype_t value, underlyingtype_t defaultValue) { \
        if (Present(value)) { \
            _value = value; \
            _wasSetFromUnkown = false; \
            return true; \
        } \
        if (Present(defaultValue)) \
            _value = defaultValue; \
        else if (_hasDefault()) \
            _value = _defaultValue(); \
        _wasSetFromUnkown = true; \
        _unknownValue = value; \
        return false; \
    } \
    bool SetFromStr(std::wstring const &str) { \
        for (auto const &m : _members()) { \
            if (m.second == str) { \
                _value = m.first; \
                _wasSetFromUnkown = false; \
                return true; \
            } \
        } \
        if (_hasDefault()) \
            _value = _defaultValue(); \
        _wasSetFromUnkown = true; \
       return false; \
    } \
    bool SetFromStr(std::wstring const &str, std::wstring &unkStr) { \
        SetFromStr(str); \
        if (_wasSetFromUnkown) \
            unkStr = str; \
        else \
            unkStr.clear(); \
        return !_wasSetFromUnkown; \
    } \
    bool SetDefaultValue() { \
        if (_hasDefault()) { \
            _value = _defaultValue(); \
            return true; \
        } \
        return false; \
    } \
    template<typename ToType> \
    ToType TranslateTo(const Vector<std::pair<underlyingtype_t, ToType>> &table, ToType defaultValue = 0) { \
        for (size_t i = 0; i < table.size(); i++) { \
            if (table[i].second == _value) \
                return table[i].first; \
        } \
        return defaultValue; \
    } \
    template<typename FromType> \
    bool TranslateFrom(FromType fromValue, const Vector<std::pair<FromType, underlyingtype_t>> &table) { \
        for (size_t i = 0; i < table.size(); i++) { \
            if (table[i].first == fromValue) { \
                SetFromInt(table[i].second); \
                return true; \
            } \
        } \
        SetDefaultValue(); \
        return false; \
    } \
    typeName &operator= (const typeName &rhs) { \
        _value = rhs._value; \
        _wasSetFromUnkown = rhs._wasSetFromUnkown; \
        _unknownValue = rhs._unknownValue; \
        return *this; \
    } \
    typeName(const typeName &rhs) { \
        _value = rhs._value; \
        _wasSetFromUnkown = rhs._wasSetFromUnkown; \
        _unknownValue = rhs._unknownValue; \
    } \
    typeName &operator= (const std::wstring &rhs) { \
        SetFromStr(rhs); \
        return *this; \
    } \
    typeName(const std::wstring &rhs) { \
        SetFromStr(rhs); \
    } \
    template<typename T, typename I = std::enable_if_t<std::is_enum_v<T>>> \
    typeName &operator= (T rhs) { \
        SetFromInt(rhs); \
        return *this; \
    } \
    template<typename T, typename I = std::enable_if_t<std::is_enum_v<T>>> \
    typeName(T rhs) { \
        SetFromInt(rhs); \
    } \
    static typeName MakeFromInt(underlyingtype_t value) { \
        typeName result; \
        result.SetFromInt(value); \
        return result; \
    } \
}; \
 \
inline bool operator== (const typeName &a, const typeName &b) { \
    return a.ToInt() == b.ToInt(); \
} \
template<typename T> \
inline bool operator== (const typeName &a, const T &b) { \
    return a.ToInt() == b; \
} \
inline bool operator!= (const typeName &a, const typeName &b) { \
    return a.ToInt() != b.ToInt(); \
} \
template<typename T> \
inline bool operator!= (const typeName &a, const T &b) { \
    return a.ToInt() != b; \
} \
inline bool operator< (const typeName &a, const typeName &b) { \
    return a.ToInt() < b.ToInt(); \
} \
template<typename T> \
inline bool operator< (const typeName &a, const T &b) { \
    return a.ToInt() < b; \
} \
template<typename T> \
inline bool operator< (const T &a, const typeName &b) { \
    return a < b.ToInt(); \
} \
inline bool operator> (const typeName &a, const typeName &b) { \
    return a.ToInt() > b.ToInt(); \
} \
template<typename T> \
inline bool operator> (const typeName &a, const T &b) { \
    return a.ToInt() > b; \
} \
template<typename T> \
inline bool operator> (const T &a, const typeName &b) { \
    return a > b.ToInt(); \
} \
inline bool operator<= (const typeName &a, const typeName &b) { \
    return a.ToInt() <= b.ToInt(); \
} \
template<typename T> \
inline bool operator<= (const typeName &a, const T &b) { \
    return a.ToInt() <= b; \
} \
template<typename T> \
inline bool operator<= (const T &a, const typeName &b) { \
    return a <= b.ToInt(); \
} \
inline bool operator>= (const typeName &a, const typeName &b) { \
    return a.ToInt() >= b.ToInt(); \
} \
template<typename T> \
inline bool operator>= (const typeName &a, const T &b) { \
    return a.ToInt() >= b; \
} \
template<typename T> \
inline bool operator>= (const T &a, const typeName &b) { \
    return a >= b.ToInt(); \
}

#define ENUM_DEFAULT_READ_WRITE \
public: \
    void Read(FifamReader &reader, String const &str) { \
        if (!str.empty()) \
            SetFromInt(Utils::SafeConvertInt<underlyingtype_t>(str)); \
        else \
            SetDefaultValue(); \
    } \
    void Write(FifamWriter &writer) { \
        writer.WriteOne(_value); \
    }

#define ENUM_DEFAULT_READ_WRITE_STRING \
public: \
    void Read(FifamReader &reader, String const &str) { \
        SetFromStr(str); \
    } \
    void Write(FifamWriter &writer) { \
        writer.WriteOne(ToStr()); \
    } \
private:

#define ENUM_READ(reader, str) \
public: \
    void Read(FifamReader & reader, String const & str)

#define ENUM_WRITE(writer) \
public: \
    void Write(FifamWriter & writer)

template<typename E>
void FifamCheckEnum(E const &e) {
    if (e.GetWasSetFromUnknown())
        Error("Enum %s was set from unknown value: %u", typeid(e).name(), e.GetUnknown());
}
