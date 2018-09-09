#pragma once
#include "FifamTypes.h"

class FifamDbWriteable {
    Bool mWriteable = true;
    UInt mWriteableID = 0;
    UInt mWriteableUniqueID = 0;
public:
    bool GetIsWriteable() const;
    void SetIsWriteable(Bool enable);
    UInt GetWriteableID() const;
    void SetWriteableID(UInt id);
    UInt GetWriteableUniqueID() const;
    void SetWriteableUniqueID(UInt id);
};

class FifamDbWriteableIDsList {
    Vector<UInt> mIDs;
public:
    void clear();
    UInt size() const;
    UInt at(UInt index) const;
    UInt operator[](UInt index) const;
    void push_back(UInt id);
    void push_back_unique(UInt id);
    Vector<UInt> get_array(UInt size) const;
};
