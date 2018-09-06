#pragma once
#include "Utils.h"
#include "FifamReadWrite.h"

class FifamBaseFlags {};

template<typename T, UInt numBits = sizeof(typename T::underlyingtype_t) * 8>
class FifamFlags : FifamBaseFlags {
public:
    using underlyingtype_t = typename T::underlyingtype_t;
private:
    BitSet<numBits> _bitset;
    underlyingtype_t _unknown = 0;
    Bool _wasSetFromUnknown = false;
public:
    FifamFlags() {}

    Bool GetWasSetFromUnknown() const {
        return _wasSetFromUnknown;
    }

    typename underlyingtype_t GetUnknown() const {
        return _unknown;
    }

    Bool Set(underlyingtype_t flag, Bool state) {
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

    Bool Set(String const &strFlag, Bool state) {
        T flag;
        if (flag.SetFromStr(strFlag)) {
            Set(flag, state);
            return true;
        }
        return false;
    }

    Bool Check(underlyingtype_t flag) const {
        if (T::Present(flag))
            return (_bitset.to_ullong() & flag) == flag;
        return false;
    }

    void Set(T const &flag, Bool state) {
        Set(flag.ToInt(), state);
    }

    Bool Check(T const &flag) const {
        return Check(flag.ToInt());
    }

    Bool SetFromInt(underlyingtype_t flags) {
        Bool result = true;
        Clear();
        for (UInt i = 0; i < numBits; i++) {
            if ((flags & (1ull << i)) && T::Present(1ull << i))
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

    Bool SetFromStrAry(Vector<String> const &ary) {
        Bool result = true;
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

    Bool SetFromStr(String const &str, WideChar sep = L',') {
        auto ary = Utils::Split(str, sep);
        return SetFromStrAry(ary);
    }

    typename underlyingtype_t ToInt() const {
        return static_cast<underlyingtype_t>(_bitset.to_ullong());
    }

    String ToStr(String const &sep = L",") const {
        String result;
        Bool first = true;
        for (UInt i = 0; i < numBits; i++) {
            if (_bitset[i]) {
                if (first)
                    first = false;
                else
                    result += sep;
                T tmp;
                tmp.SetFromInt(1ull << i);
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
    Bool All() const { return _bitset.all(); }
    Bool Any() const { return _bitset.any(); }
    Bool None() const { return _bitset.none(); }
    Bool Count() const { return _bitset.count(); }

    FifamFlags &operator=(underlyingtype_t flags) {
        SetFromInt(flags);
        return *this;
    }

    FifamFlags &operator=(String const &strFlags) {
        SetFromStr(strFlags);
        return *this;
    }

    FifamFlags &operator=(Vector<String> const &ary) {
        SetFromStrAry(ary);
        return *this;
    }

    FifamFlags(underlyingtype_t flags) {
        SetFromInt(flags);
    }

    FifamFlags(String const &strFlags) {
        SetFromStr(strFlags);
    }

    FifamFlags(Vector<String> const &ary) {
        SetFromStrAry(ary);
    }

    void Read(FifamReader &reader, String const &str) {
        if (!str.empty())
            SetFromInt(Utils::SafeConvertInt<typename T::underlyingtype_t>(str));
    }

    void Write(FifamWriter &writer) {
        writer.WriteOne(ToInt()); \
    }
};

template<typename F>
void FifamCheckFlags(F const &f) {
    if (f.GetWasSetFromUnknown())
        Error("Flags %s were set from unknown value: %u", typeid(f).name(), f.GetUnknown());
}
