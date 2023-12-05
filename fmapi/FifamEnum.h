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
    using _MemberType = Pair<underlyingtype_t, WideChar const *>; \
    struct _SetComparator { \
        bool operator()(_MemberType const& lhs, _MemberType const& rhs) { \
            return lhs.first < rhs.first; \
        } \
    }; \
    using _MembersContainer = Set<_MemberType, _SetComparator>; \
    underlyingtype_t _value = 0; \
    Bool _wasSetFromUnkown = false; \
    underlyingtype_t _unknownValue = 0; \
\
    static Bool &_hasGaps() { \
        static Bool var_hasGaps = true; \
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
    static Bool &_hasDefault() { \
        static Bool var_hasDefault = false; \
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
    static underlyingtype_t _InitEnumMember(underlyingtype_t id, WideChar const *name) { \
        _members().insert(std::make_pair(id, name)); \
        return id; \
    } \
    static Bool _InitEnum() { \
        if (_members().empty()) \
             _hasGaps() = false; \
        else { \
            _hasGaps() = true; \
            underlyingtype_t it = (*(_members().begin())).first; \
            for (auto const &m : _members()) { \
                if (m.first == it || (m.first - it) == 1) \
                    it = m.first; \
                else \
                    return true; \
            } \
            _hasGaps() = false; \
            _first() = (*(_members().begin())).first; \
            _last() = (*(_members().rbegin())).first; \
            return false; \
        } \
        return true; \
    } \
    static Bool _InitDefaultValue(underlyingtype_t value) { \
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
    inline static Bool _initializer1x = _InitDefaultValue(idname);

#define ENUM_END(typeName) \
private: \
    inline static Bool _initializer0x = _InitEnum(); \
public: \
    typeName() { \
        if (_hasDefault()) \
            _value = _defaultValue(); \
        else \
            _value = _first(); \
    } \
    static Bool Present(underlyingtype_t value) { \
        if (!_hasGaps()) \
            return value >= _first() && value <= _last(); \
        for (auto const &m : _members()) { \
            if (m.first == value) \
                return true; \
        } \
        return false; \
    } \
    static Bool Present(String const &value) { \
        for (auto const &m : _members()) { \
            if (m.second == value) \
                return true; \
        } \
        return false; \
    } \
    static void ForAllValues(Function<void(typeName const &value)> callback) { \
        for (auto const &m : _members()) \
            callback(MakeFromInt(m.first)); \
    } \
    underlyingtype_t ToInt() const { return _value; } \
    String ToStr() const { \
        return ToCStr(); \
    } \
    WideChar const *ToCStr() const { \
        for (auto const &m : _members()) { \
            if (m.first == _value) \
                return m.second; \
        } \
        return L""; \
    } \
    Bool GetWasSetFromUnknown() const { return _wasSetFromUnkown; } \
    void SetUnknown(underlyingtype_t value) { \
        _wasSetFromUnkown = true; \
        _unknownValue = value; \
    } \
    underlyingtype_t GetUnknown() const { return _unknownValue; } \
    Bool SetFromInt(underlyingtype_t value) { \
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
    Bool SetFromInt(underlyingtype_t value, underlyingtype_t defaultValue) { \
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
    Bool SetFromStr(String const &str) { \
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
    Bool SetFromStr(String const &str, String &unkStr) { \
        SetFromStr(str); \
        if (_wasSetFromUnkown) \
            unkStr = str; \
        else \
            unkStr.clear(); \
        return !_wasSetFromUnkown; \
    } \
    Bool SetDefaultValue() { \
        if (_hasDefault()) { \
            _value = _defaultValue(); \
            return true; \
        } \
        return false; \
    } \
    template<typename ToType> \
    ToType TranslateTo(const Vector<Pair<underlyingtype_t, ToType>> &table, ToType defaultValue = 0) { \
        for (UInt i = 0; i < table.size(); i++) { \
            if (table[i].second == _value) \
                return table[i].first; \
        } \
        return defaultValue; \
    } \
    template<typename FromType> \
    Bool TranslateFrom(FromType fromValue, const Vector<Pair<FromType, underlyingtype_t>> &table) { \
        for (UInt i = 0; i < table.size(); i++) { \
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
    typeName &operator= (const String &rhs) { \
        SetFromStr(rhs); \
        return *this; \
    } \
    typeName(const String &rhs) { \
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
inline Bool operator== (const typeName &a, const typeName &b) { \
    return a.ToInt() == b.ToInt(); \
} \
inline Bool operator== (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() == b; \
} \
inline Bool operator!= (const typeName &a, const typeName &b) { \
    return a.ToInt() != b.ToInt(); \
} \
inline Bool operator!= (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() != b; \
} \
inline Bool operator< (const typeName &a, const typeName &b) { \
    return a.ToInt() < b.ToInt(); \
} \
inline Bool operator< (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() < b; \
} \
inline Bool operator< (const typeName::underlyingtype_t &a, const typeName &b) { \
    return a < b.ToInt(); \
} \
inline Bool operator> (const typeName &a, const typeName &b) { \
    return a.ToInt() > b.ToInt(); \
} \
inline Bool operator> (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() > b; \
} \
inline Bool operator> (const typeName::underlyingtype_t &a, const typeName &b) { \
    return a > b.ToInt(); \
} \
inline Bool operator<= (const typeName &a, const typeName &b) { \
    return a.ToInt() <= b.ToInt(); \
} \
inline Bool operator<= (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() <= b; \
} \
inline Bool operator<= (const typeName::underlyingtype_t &a, const typeName &b) { \
    return a <= b.ToInt(); \
} \
inline Bool operator>= (const typeName &a, const typeName &b) { \
    return a.ToInt() >= b.ToInt(); \
} \
inline Bool operator>= (const typeName &a, const typeName::underlyingtype_t &b) { \
    return a.ToInt() >= b; \
} \
inline Bool operator>= (const typeName::underlyingtype_t &a, const typeName &b) { \
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
