#pragma once
#include "FifamTypes.h"

class FifamPropertied {
    Map<String, Any> mProperties;
public:
    inline bool HasProperty(String const &name) {
        return mProperties.find(name) != mProperties.end();
    }

    template <typename T>
    T GetProperty(String const &name, T const &defaultValue = T()) {
        auto it = mProperties.find(name);
        if (it != mProperties.end()) {
            try {
                return std::any_cast<T>((*it).second);
            }
            catch (...) {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    template <typename T>
    void SetProperty(String const &name, T value) {
        mProperties[name] = value;
    }

    inline void ClearProperties() {
        mProperties.clear();
    }
};
