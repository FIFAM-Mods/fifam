#include "Error.h"
#include "FifaDataFile.h"
#include <codecvt>
#include <sstream>
#include <stdio.h>

void FifaDataFile::Line::FromString(std::filesystem::path const &filepath, std::wstring const &str) {
    filePath = filepath;
    currentParam = 0;
    parameters.clear();
    std::wstring param;
    std::wstringstream stream(str);
    while (getline(stream, param, L'\t'))
        parameters.push_back(param);
}

FifaDataFile::Line &FifaDataFile::Line::operator>>(int &out) {
    if (currentParam >= parameters.size())
        Error(L"FifaDataFile::Line::operator>>(int &):\nreached eop (total parameters: %d)\nin %s", parameters.size(), filePath.c_str());
    try {
        out = stoi(parameters[currentParam++]);
    }
    catch (std::exception &e) {
        Error(e.what());
        out = 0;
    }
    return *this;
}

FifaDataFile::Line &FifaDataFile::Line::operator>>(std::wstring &out) {
    if (currentParam >= parameters.size())
        Error(L"FifaDataFile::Line::operator>>(std::wstring &):\nreached eop (total parameters: %d)\nin %s", parameters.size(), filePath.c_str());
    out = parameters[currentParam++];
    return *this;
}

FifaDataFile::~FifaDataFile() {
    Close();
}

bool FifaDataFile::Open(std::filesystem::path const &filepath) {
    Close();
    _wfopen_s(&file, filepath.c_str(), L"r,ccs=UNICODE");
    if (!file)
        return false;
    filePath = filepath;
    Line header;
    if (NextLine(header)) {
        columns.resize(header.parameters.size());
        std::copy(header.parameters.begin(), header.parameters.end(), columns.begin());
    }
    return true;
}

bool FifaDataFile::NextLine(Line &outLine) {
    if (!file)
        return false;
    static wchar_t line[4096];
    line[0] = L'\0';
    while (true) {
        if (!fgetws(line, 4096, file))
            break;
        std::wstring wstr = line;
        if (wstr.size() > 0) {
            outLine.FromString(filePath, wstr);
            return true;
        }
    }
    return false;
}

void FifaDataFile::Close() {
    columns.clear();
    if (file) {
        fclose(file);
        file = nullptr;
    }
}
