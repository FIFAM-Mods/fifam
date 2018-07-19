#pragma once
#include <bitset>
#include "Utils.h"
#include "FifamReadWrite.h"

template<typename T, size_t numBits = sizeof(typename T::underlying_type_t) * 8>
class Flags {
    std::bitset<numBits> _bitset;

public:
    bool Set(typename T::underlying_type_t flag, bool state) {
        if (T::Present(flag)) {
            if (state)
                _bitset |= flag;
            else
                _bitset &= ~flag;
            return true;
        }
        return false;
    }

    bool Set(std::wstring const &strFlag, bool state) {
        T flag;
        if (flag.SetFromStr(strFlag)) {
            Set(flag, state);
            return true;
        }
        return false;
    }

    bool Check(typename T::underlying_type_t flag) {
        if (T::Present(flag))
            return (_bitset & flag) == flag;
        return false;
    }

    void Set(T const &flag, bool state) {
        Set(flag.ToInt(), state);
    }

    bool Check(T const &flag) {
        return Check(flag.ToInt());
    }

    bool SetFromInt(typename T::underlying_type_t flags) {
        bool result = true;
        Clear();
        for (size_t i = 0; i < numBits; i++) {
            if ((flags & (1 << i)) && T::Present(1 << i))
                _bitset[i] = true;
            else if (result)
                result = false;
        }
        return result;
    }

    bool SetFromStrAry(std::vector<std::wstring> const &ary) {
        bool result = true;
        Clear();
        if (!ary.empty()) {
            for (auto const &str : ary) {
                if (!Set(str, true) && result)
                    result = false;
            }
        }
        return result;
    }

    bool SetFromStr(std::wstring const &str, std::wstring const &sep = L",") {
        auto ary = Utils::Split(str, sep);
        return SetFromStrAry(ary);
    }

    typename T::underlying_type_t ToInt() {
        return static_cast<typename T::underlying_type_t>(_bitset.to_ulong());
    }

    std::wstring ToStr(std::wstring const &sep = L",") {
        std::wstring result;
        bool first = true;
        for (size_t i = 0; i < numBits; i++) {
            if (_bitset[i]) {
                if (first)
                    first = false;
                else
                    result += sep;
                T tmp;
                tmp.SetFromInt(1 << i);
                result += tmp.ToStr();
            }
        }
        return result;
    }

    void Clear() { _bitset.reset(); }
    void SetAll() { _bitset.set(); }
    bool All() { return _bitset.all(); }
    bool Any() { return _bitset.any(); }
    bool None() { return _bitset.none(); }
    bool Count() { return _bitset.count(); }

    Flags &operator= (typename T::underlying_type_t flags) {
        SetFromInt(flags);
        return *this;
    }

    Flags &operator= (std::wstring const &strFlags) {
        SetFromStr(strFlags);
        return *this;
    }

    Flags &operator= (std::vector<std::wstring> const &ary) {
        SetFromStrAry(ary);
        return *this;
    }

    Flags(typename T::underlying_type_t flags) {
        SetFromInt(flags);
    }

    Flags(std::wstring const &strFlags) {
        SetFromStr(strFlags);
    }

    Flags(std::vector<std::wstring> const &ary) {
        SetFromStrAry(ary);
    }

    void Read(FifamReader &reader, String const &str) {
        if (!str.empty())
            SetFromInt(Utils::SafeConvertInt<typename T::underlying_type_t>(str));
    }

    void Write(FifamWriter &writer) {
        writer.WriteOne(ToInt()); \
    }

    /*
    flag.TranslateFrom(value, {
        { 0, Flag::Val1 },
        { 1, Flag::Val2 }
    });
    val = flag.TranslateTo({
        { Flag::Val1, 0 },
        { Flag::Val2, 1 },
        { Flag::Val3, 1 }
    });
    */
};
