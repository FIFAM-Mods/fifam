#include "FifamCompID.h"
#include "FifamUtils.h"

void FifamCompID::SetFromInt(UInt value) {
    mRegion.SetFromInt((value >> 24) & 0xFF);
    mType.SetFromInt((value >> 16) & 0xFF);
    mIndex = value & 0xFFFF;
}

UInt FifamCompID::ToInt() const {
    return mIndex | (mType.ToInt() << 16) | (mRegion.ToInt() << 24);
}

void FifamCompID::FifamCompID::SetFromHexStr(String const &str) {
    if (Utils::StartsWith(str, L"0x"))
        SetFromInt(Utils::ToNumber(str));
    else
        SetFromInt(Utils::ToNumber(L"0x" + str));
}

String FifamCompID::ToHexStr() const {
    return Utils::Format(L"%08X", ToInt());
}

void FifamCompID::SetFromStr(String const &str) {
    auto comps = Utils::Split(str, L',');
    if (comps.size() == 3) {
        if (Utils::StartsWith(comps[0], L"{"))
            comps[0] = comps[0].substr(1);
        if (Utils::EndsWith(comps[2], L"}"))
            comps[2].pop_back();
        Int region = Utils::ToNumber(comps[0]);
        if (region < 0)
            region = -region;
        mRegion.SetFromInt(region);
        mType.SetFromStr(comps[1]);
        mIndex = Utils::ToNumber(comps[2]);
    }
}

String FifamCompID::ToStr() const {
    if (IsNull())
        return L"None";
    Int regionId = mRegion.ToInt();
    return Utils::Format(L"{ %d, %s, %d }", regionId, mType.ToCStr(), mIndex);
}

FifamCompID::FifamCompID() {}

FifamCompID::FifamCompID(FifamCompRegion const &region, FifamCompType const &type, UShort index) {
    Set(region, type, index);
}

void FifamCompID::Set(FifamCompRegion const &region, FifamCompType const &type, UShort index) {
    mRegion = region;
    mType = type;
    mIndex = index;
}

FifamCompID::FifamCompID(FifamCompRegion const &region, String const &type, UShort index) {
    Set(region, type, index);
}

void FifamCompID::Set(FifamCompRegion const &region, String const &type, UShort index) {
    mRegion = region;
    mType.SetFromStr(type);
    mIndex = index;
}

FifamCompID::FifamCompID(UChar regionId, FifamCompType const & type, UShort index) {
    Set(regionId, type, index);
}

void FifamCompID::Set(UChar regionId, FifamCompType const & type, UShort index) {
    mRegion.SetFromInt(regionId);
    mType = type;
    mIndex = index;
}

FifamCompID::FifamCompID(UInt id) {
    SetFromInt(id);
}

Bool FifamCompID::IsNull() const {
    return ToInt() == 0;
}

UInt FifamCompID::Translate(UInt id, UInt gameFrom, UInt gameTo) {
    UChar region = (id >> 24) & 0xFF;
    if (FifamUtils::ConvertRegion(region, gameFrom, gameTo))
        return (id & 0xFFFFFF) | (region << 24);
    return 0;
}

Bool operator==(FifamCompID const & lhs, FifamCompID const & rhs) {
    return lhs.ToInt() == rhs.ToInt();
}

Bool operator<(FifamCompID const & lhs, FifamCompID const & rhs) {
    return lhs.ToInt() < rhs.ToInt();
}

Bool operator>(FifamCompID const & lhs, FifamCompID const & rhs) {
    return lhs.ToInt() > rhs.ToInt();
}
