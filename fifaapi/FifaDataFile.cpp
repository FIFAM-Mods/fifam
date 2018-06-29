#include "FifaDataFile.h"
#include <codecvt>
#include <sstream>
#include "Error.h"

void FifaDataFile::Line::FromString(std::wstring const &str) {
    currentParam = 0;
    parameters.clear();
    std::wstring param;
    std::wstringstream stream(str);
    while (getline(stream, param, L'\t'))
        parameters.push_back(param);
}

FifaDataFile::Line &FifaDataFile::Line::operator>>(int &out) {
    if (currentParam >= parameters.size())
        Error("s: reached eop (total parameters: %d)", __FUNCTION__, parameters.size());
    out = stoi(parameters[currentParam++]);
    return *this;
}

FifaDataFile::Line &FifaDataFile::Line::operator>>(std::wstring &out) {
    if (currentParam >= parameters.size())
        Error("s: reached eop (total parameters: %d)", __FUNCTION__, parameters.size());
    out = parameters[currentParam++];
    return *this;
}

FifaDataFile::~FifaDataFile() {
    Close();
}

bool FifaDataFile::Open(std::wstring filepath) {
    columns.clear();
    stream.open(filepath, std::ios::binary);
    if (!stream.is_open())
        return false;
    stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf16<wchar_t, 0xffff, std::consume_header>));
    Line header;
    if (NextLine(header)) {
        columns.resize(header.parameters.size());
        std::copy(header.parameters.begin(), header.parameters.end(), columns.begin());
    }
    return true;
}

bool FifaDataFile::NextLine(Line &outLine) {
    std::wstring line;
    while (true) {
        if (!getline(stream, line))
            break;
        if (line.size() > 0) {
            outLine.FromString(line);
            return true;
        }
    }
    return false;
}

void FifaDataFile::Close() {
    columns.clear();
    stream.close();
}