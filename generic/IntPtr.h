#pragma once

template<typename T>
class IntPtr {
    volatile T **pPtr = nullptr;
public:
    explicit IntPtr(T *&link) {
        pPtr = (volatile T **)&link;
    }

    void operator=(int value) const {
        *(int *)pPtr = value;
    }
};
