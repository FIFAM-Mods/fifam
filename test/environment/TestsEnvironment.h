#pragma once

template<typename T>
T &GetEnvironment() {
    static T env;
    return env;
}
