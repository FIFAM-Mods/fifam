#include "FifamCompID.h"

void FifamCompID::SetFromInt(UInt value) {
    mRegion.SetFromInt((value >> 24) & 0xFF);
    mType.SetFromInt((value >> 16) & 0xFF);
    mLevel = value & 0xFFFF;
}

UInt FifamCompID::ToInt() {
    return mLevel | (mType.ToInt() << 16) | (mRegion.ToInt() << 24);
}

void FifamCompID::FifamCompID::SetFromHexStr(String const &str) {
    SetFromInt(Utils::ToNumber(L"0x" + str));
}

String FifamCompID::ToHexStr() {
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
        if (region < 0) {
            region = -region;
            mIsTemplate = true;
        }
        mRegion.SetFromInt(region);
        mType.SetFromStr(comps[1]);
        mLevel = Utils::ToNumber(comps[2]);
    }
}

String FifamCompID::ToStr() {
    if (IsNull())
        return L"None";
    Int regionId = mRegion.ToInt();
    if (mIsTemplate)
        regionId = -regionId;
    return Utils::Format(L"{ %d, %s, %d }", regionId, mType.ToCStr(), mLevel);
}

FifamCompID::FifamCompID() {}

FifamCompID::FifamCompID(FifamCompRegion const &region, FifamCompType const &type, UShort level, Bool isTemplate) {
    Set(region, type, level, isTemplate);
}

void FifamCompID::Set(FifamCompRegion const &region, FifamCompType const &type, UShort level, Bool isTemplate) {
    mRegion = region;
    mType = type;
    mLevel = level;
    mIsTemplate = isTemplate;
}

FifamCompID::FifamCompID(FifamCompRegion const &region, String const &type, UShort level, Bool isTemplate) {
    Set(region, type, level, isTemplate);
}

void FifamCompID::Set(FifamCompRegion const &region, String const &type, UShort level, Bool isTemplate) {
    mRegion = region;
    mType.SetFromStr(type);
    mLevel = level;
    mIsTemplate = isTemplate;
}

FifamCompID::FifamCompID(UInt id) {
    SetFromInt(id);
}

bool FifamCompID::IsNull() {
    return ToInt() == 0;
}
