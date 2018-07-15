#pragma once

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <filesystem>
#include <new>
#include "Error.h"

using namespace std::experimental::filesystem;

using Byte = unsigned char;
using Char = char;
using UChar = unsigned char;
using Short = short;
using UShort = unsigned short;
using Int = int;
using UInt = unsigned int;
using Int64 = long long int;
using UInt64 = unsigned long long int;
using Float = float;
using Double = double;
using Bool = unsigned char;
using String = std::wstring;
template<typename T, size_t S>
using Array = std::array<T, S>;
template<typename T>
using Vector = std::vector<T>;
template<typename T>
using List = std::list<T>;
template<typename KeyType, typename ValueType>
using Map = std::map<KeyType, ValueType>;
template<typename T>
using Set = std::set<T>;
using Path = path;
template<typename T>
using NameIdTable = std::vector<std::pair<const wchar_t *, T>>;

template<class T, size_t N>
constexpr size_t ArraySize(T(&)[N]) { return N; }

template<class T, size_t N>
constexpr T ArrayElement(const T(&ary)[N], int index, T defaultValue) {
    if (index >= 0 && N > index)
        return ary[index];
    return defaultValue;
}

template<class T, size_t N>
constexpr int ArrayIndex(const T(&ary)[N], T value) {
    for (size_t i = 0; i < N; i++) {
        if (ary[i] == value)
            return static_cast<int>(i);
    }
    return 0;
}

template<typename T>
constexpr String IdToName(NameIdTable<T> const &table, T id, String defaultName = String()) {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].second == id)
            return table[i].first;
    }
    return defaultName;
}

template<typename T>
constexpr bool IdToName(String &out, NameIdTable<T> const &table, T id) {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].second == id) {
            out = table[i].first;
            return true;
        }
    }
    return false;
}

template<typename T>
constexpr T NameToId(NameIdTable<T> const &table, String const &name, T defaultId) {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].first == name)
            return table[i].second;
    }
    return defaultId;
}

template<typename T>
constexpr bool NameToId(T &out, NameIdTable<T> const &table, String const &name) {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].first == name) {
            out = table[i].second;
            return true;
        }
    }
    return false;
}

template<typename T>
constexpr bool NameToId_Flag(T &out, NameIdTable<T> const &table, String const &name) {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].first == name) {
            out |= table[i].second;
            return true;
        }
    }
    return false;
}

template<typename T>
NameIdTable<T> const &GetNameIdTable() {
    static NameIdTable<T> table;
    return table;
}

template<typename T>
String IdToName(T id, String defaultName = String()) {
    return IdToName(GetNameIdTable<T>(), id, defaultName);
}

template<typename T>
bool IdToName(String &out, T id) {
    return IdToName(out, GetNameIdTable<T>(), id);
}

template<typename T>
T NameToId(String const &name, T defaultId) {
    return NameToId(GetNameIdTable<T>(), defaultId);
}

template<typename T>
bool NameToId(T &out, String const &name) {
    return NameToId(out, GetNameIdTable<T>(), name);
}

template<typename T>
bool NameToId_Flag(T &out, String const &name) {
    return NameToId_Flag(out, GetNameIdTable<T>(), name);
}

template<typename T, typename Cb>
size_t IterateFlags(T flags, Cb callback) {
    using U = std::underlying_type_t<T>;
    U uflags = static_cast<U>(flags);
    size_t numIters = 0;
    for (size_t i = 0; i < sizeof(T) * 8; i++) {
        if (uflags & (1 << i)) {
            callback(static_cast<T>(uflags & (1 << i)));
            numIters++;
        }
    }
    return numIters;
}

template<typename T>
void ClearFlags(T &flags) {
    flags = static_cast<T>(0);
}

template<typename T, typename ...ArgTypes>
void ResetInstance(T *instance, ArgTypes... args) {
    new (instance) T(args...);
}

#include "FifamDate.h"
#include "FifamVersion.h"
#include "FifamNation.h"
#include "FifamLanguage.h"
#include "FifamTranslation.h"
#include "FifamFormation.h"
#include "FifamRoundID.h"
#include "FifamBeg.h"
#include "FifamCupAllocType.h"
#include "FifamTerritory.h"
#include "FifamAssociation.h"

template<typename T>
using TrArray = Array<T, FifamTranslation::NUM_TRANSLATIONS>;

template<typename T>
T Tr(TrArray<T> const &trAry) {
    return trAry[FifamTranslation::English];
}

inline wchar_t const *Tr_c(TrArray<String> const &trAry) {
    return trAry[FifamTranslation::English].c_str();
}
