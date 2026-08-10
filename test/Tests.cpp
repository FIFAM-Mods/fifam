#include <Windows.h>
#include "Tests.h"
#include "TestsRun.h"
#include "FifaDatabase.h"
#include <fcntl.h>
#include <io.h>
#include "Magick++.h"

FifaDatabase *gFifaDb;

int main(int argc, char *argv[]) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    Magick::InitializeMagick(NULL);
    if (argc < 2) {
        std::wcout << L"Usage: Tests.exe <TestName>\n\nAvailable tests:\n";
        for (auto &[name, fn] : TestRegistry::Instance().All())
            std::wcout << L"  " << name << std::endl;
        system("pause");
        return 1;
    }
    auto fn = TestRegistry::Instance().Get(Utils::AtoW(argv[1]));
    if (!fn) {
        std::wcout << L"Unknown test: " << argv[1] << std::endl;
        return 1;
    }
    fn();
    system("pause");
    return 0;
}
