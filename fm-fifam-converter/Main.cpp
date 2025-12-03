#include "Converter.h"
#include "Magick++.h"

int main() {
    Magick::InitializeMagick(NULL);
    SetCurrentDirectoryW(L"D:\\Projects\\fifam\\output\\converters");
    Converter c;
    c.Convert();
}
