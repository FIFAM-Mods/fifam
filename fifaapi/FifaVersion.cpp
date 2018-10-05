#include "FifaVersion.h"
#include <sstream>
#include <iomanip>
#include "Error.h"

void FifaVersion::Internal::Read(FifaDataFile::Line &line) {
    line >> minor >> major >> schema >> isonline >> exportdate >> artificialkey;
}

std::wstring FifaVersion::GetVersionString() {
    std::wstringstream stream;
    stream << internal.major << L"." << internal.minor;
    return stream.str();
}

FifaDate FifaVersion::GetDate() {
    return FifaDate(internal.exportdate);
}

std::wstring FifaVersion::GetDateString() {
    FifaDate date(internal.exportdate);
    return date.ToString();
}

void FifaVersion::FromLine(FifaDataFile::Line &line) {
    internal.Read(line);
}
