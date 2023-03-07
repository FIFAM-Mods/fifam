#include <Windows.h>
#include "Tests.h"
#include "TestsRun.h"
#include "FifaDatabase.h"
#include <fcntl.h>
#include <io.h>
#include "Magick++.h"

FifaDatabase *gFifaDb;

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    Magick::InitializeMagick(NULL);
    RunTests();
    system("pause");
    return 0;
}
