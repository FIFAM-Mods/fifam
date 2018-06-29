#pragma once

#include <string>
#include <array>
#include <vector>
#include <list>

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

#include "FifamDate.h"
#include "FifamVersion.h"
#include "FifamNation.h"
#include "FifamLanguage.h"
#include "FifamTranslation.h"
