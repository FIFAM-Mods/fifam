#include "Converter.h"
#include "Magick++.h"

int main() {
    Magick::InitializeMagick(NULL);
    Converter c;
    c.Convert();
}
