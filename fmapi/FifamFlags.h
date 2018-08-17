#pragma once
#include <bitset>
#include "Utils.h"
#include "FifamReadWrite.h"

class FifamBaseFlags {};

template<typename T, size_t numBits = sizeof(typename T::underlyingtype_t) * 8>
class FifamFlags : FifamBaseFlags {
public:
    using underlyingtype_t = typename T::underlyingtype_t;
private:
    std::bitset<numBits> _bitset;
    underlyingtype_t _unknown = 0;
    bool _wasSetFromUnknown = false;
public:
    FifamFlags() {}

    bool GetWasSetFromUnknown() const {
        return _wasSetFromUnknown;
    }

    typename underlyingtype_t GetUnknown() const {
        return _unknown;
    }

    bool Set(underlyingtype_t flag, bool state) {
        if (T::Present(flag)) {
            if (state)
                _bitset |= flag;
            else
                _bitset &= ~flag;
            return true;
        }
        else {
            _unknown |= flag;
            _wasSetFromUnknown = true;
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

    bool Check(underlyingtype_t flag) const {
        if (T::Present(flag))
            return (_bitset & flag) == flag;
        return false;
    }

    void Set(T const &flag, bool state) {
        Set(flag.ToInt(), state);
    }

    bool Check(T const &flag) const {
        return Check(flag.ToInt());
    }

    bool SetFromInt(underlyingtype_t flags) {
        bool result = true;
        Clear();
        for (size_t i = 0; i < numBits; i++) {
            if ((flags & (1 << i)) && T::Present(1 << i))
                _bitset[i] = true;
            else if (result)
                result = false;
        }
        if (!result) {
            _unknown = flags;
            _wasSetFromUnknown = true;
        }
        else
            _wasSetFromUnknown = false;
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
        if (!result) {
            _unknown = 0;
            _wasSetFromUnknown = true;
        }
        else
            _wasSetFromUnknown = false;
        return result;
    }

    bool SetFromStr(std::wstring const &str, std::wstring const &sep = L",") {
        auto ary = Utils::Split(str, sep);
        return SetFromStrAry(ary);
    }

    typename underlyingtype_t ToInt() const {
        return static_cast<underlyingtype_t>(_bitset.to_ulong());
    }

    std::wstring ToStr(std::wstring const &sep = L",") const {
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

    void Clear() {
        _bitset.reset();
        _wasSetFromUnknown = false;

    }
    void SetAll() {
        _bitset.set();
        _wasSetFromUnknown = false;
    }
    bool All() const { return _bitset.all(); }
    bool Any() const { return _bitset.any(); }
    bool None() const { return _bitset.none(); }
    bool Count() const { return _bitset.count(); }

    FifamFlags &operator= (underlyingtype_t flags) {
        SetFromInt(flags);
        return *this;
    }

    FifamFlags &operator= (std::wstring const &strFlags) {
        SetFromStr(strFlags);
        return *this;
    }

    FifamFlags &operator= (std::vector<std::wstring> const &ary) {
        SetFromStrAry(ary);
        return *this;
    }

    FifamFlags(underlyingtype_t flags) {
        SetFromInt(flags);
    }

    FifamFlags(std::wstring const &strFlags) {
        SetFromStr(strFlags);
    }

    FifamFlags(std::vector<std::wstring> const &ary) {
        SetFromStrAry(ary);
    }

    void Read(FifamReader &reader, String const &str) {
        if (!str.empty())
            SetFromInt(Utils::SafeConvertInt<typename T::underlyingtype_t>(str));
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

template<typename F>
void FifamCheckFlags(F const &f) {
    if (f.GetWasSetFromUnknown())
        Error("Flags %s were set from unknown value: %u", typeid(f).name(), f.GetUnknown());
}
