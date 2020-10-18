#include "FifaVersion.h"
#include "FifaDatabase.h"
#include <sstream>
#include <iomanip>
#include "Error.h"

void FifaVersion::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 11:
        line >> isonline >> minor >> major;
        break;
    case 12:
        line >> exportdate >> isonline >> minor >> major;
        break;
    case 13:
        line >> exportdate >> schema >> isonline >> minor >> artificialkey >> major;
        break;
    case 14:
        line >> exportdate >> minor >> major >> schema >> isonline  >> artificialkey;
        break;
    default:
        line >> minor >> major >> schema >> isonline >> exportdate >> artificialkey;
        break;
    }
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
