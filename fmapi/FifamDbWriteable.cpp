#include "FifamDbWriteable.h"
#include "Utils.h"

bool FifamDbWriteable::GetIsWriteable() const {
    return mWriteable;
}

void FifamDbWriteable::SetIsWriteable(Bool enable) {
    mWriteable = enable;
}

UInt FifamDbWriteable::GetWriteableID() const {
    return GetIsWriteable() ? mWriteableID : 0;
}

void FifamDbWriteable::SetWriteableID(UInt id) {
    mWriteableID = id;
}

UInt FifamDbWriteable::GetWriteableUniqueID() const {
    return GetIsWriteable() ? mWriteableUniqueID : 0;
}

void FifamDbWriteable::SetWriteableUniqueID(UInt id) {
    mWriteableUniqueID = id;
}

void FifamDbWriteableIDsList::clear() {
    mIDs.clear();
}

UInt FifamDbWriteableIDsList::size() const {
    return mIDs.size();
}

UInt FifamDbWriteableIDsList::at(UInt index) const {
    return index < size() ? mIDs[index] : 0;
}

UInt FifamDbWriteableIDsList::operator[](UInt index) const {
    return at(index);
}

void FifamDbWriteableIDsList::push_back(UInt id) {
    if (id != 0)
        mIDs.push_back(id);
}

void FifamDbWriteableIDsList::push_back_unique(UInt id) {
    if (id != 0 && !Utils::Contains(mIDs, id))
        mIDs.push_back(id);
}

Vector<UInt> FifamDbWriteableIDsList::get_array(UInt size) const {
    Vector<UInt> result;
    for (UInt i = 0; i < size; i++)
        result.push_back(at(i));
    return result;
}
