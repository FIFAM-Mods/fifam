#pragma once
#include <string>
#include <map>
#include <any>

class Fifa07Propertied {
    std::map<std::string, std::any> mProperties;
public:
    inline bool HasProperty(std::string const &name) const {
        return mProperties.find(name) != mProperties.end();
    }

    template <typename T>
    T GetProperty(std::string const &name, T const &defaultValue = T()) const {
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
    void SetProperty(std::string const &name, T value) {
        mProperties[name] = value;
    }

    inline void ClearProperties() {
        mProperties.clear();
    }
};
