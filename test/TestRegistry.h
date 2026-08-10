#pragma once
#include <string>
#include <functional>
#include <map>

class TestRegistry {
public:
    static TestRegistry &Instance() {
        static TestRegistry instance;
        return instance;
    }
    void Register(const std::wstring &name, std::function<void()> fn) {
        tests[name] = fn;
    }
    std::function<void()> Get(const std::wstring &name) const {
        auto it = tests.find(name);
        return it != tests.end() ? it->second : nullptr;
    }
    const std::map<std::wstring, std::function<void()>> &All() const { return tests; }
private:
    std::map<std::wstring, std::function<void()>> tests;
};

template<typename Test>
struct TestRegistrar {
    explicit TestRegistrar(const std::wstring &name) {
        TestRegistry::Instance().Register(name, [] { RunTest<Test>(); });
    }
};

#define REGISTER_TEST(TestClass) \
    static TestRegistrar<TestClass> _reg_##TestClass(L#TestClass);
