#include "Error.h"
#include "Fifa07DataFile.h"
#include <codecvt>
#include <sstream>
#include <stdio.h>

void Fifa07DataFile::Line::FromString(std::filesystem::path const &filepath, Mode _mode, std::wstring const &str) {
    filePath = filepath;
    mode = _mode;
    currentParam = 0;
    parameters.clear();
    std::wstring param;
    std::wstringstream stream(str);
    while (getline(stream, param, L'\t'))
        parameters.push_back(param);
}

std::wstring Fifa07DataFile::Line::ToString(std::filesystem::path const &filepath, Mode _mode) {
    filePath = filepath;
    mode = _mode;
    std::wstring result;
    if (!parameters.empty()) {
        bool first = true;
        for (auto const &p : parameters) {
            if (first)
                first = false;
            else
                result += L'\t';
            result += p;
        }
    }
    return result;
}

Fifa07DataFile::Line &Fifa07DataFile::Line::operator>>(int &out) {
    if (mode == Read) {
        if (currentParam >= parameters.size())
            Error(L"Fifa07DataFile::Line::operator>>(int &):\nreached eop (total parameters: %d)\nin %s", parameters.size(), filePath.c_str());
        try {
            out = stoi(parameters[currentParam++]);
        }
        catch (std::exception &e) {
            Error(e.what());
            out = 0;
        }
    }
    else if (mode == Write) {
        try {
            parameters.push_back(std::to_wstring(out));
        }
        catch (std::exception &e) {
            Error(e.what());
            parameters.push_back(L"0");
        }
    }
    return *this;
}

Fifa07DataFile::Line &Fifa07DataFile::Line::operator>>(std::wstring &out) {
    if (mode == Read) {
        if (currentParam >= parameters.size())
            Error(L"Fifa07DataFile::Line::operator>>(std::wstring &):\nreached eop (total parameters: %d)\nin %s", parameters.size(), filePath.c_str());
        out = parameters[currentParam++];
    }
    else if (mode == Write)
        parameters.push_back(out);
    return *this;
}

Fifa07DataFile::~Fifa07DataFile() {
    Close();
}

bool Fifa07DataFile::Open(std::filesystem::path const &filepath, Mode _mode, std::wstring const &headLine) {
    Close();
    if (_mode == Read)
        _wfopen_s(&file, filepath.c_str(), L"r,ccs=UNICODE");
    else if (_mode == Write)
        _wfopen_s(&file, filepath.c_str(), L"w,ccs=UNICODE");
    if (!file)
        return false;
    filePath = filepath;
    mode = _mode;
    if (_mode == Read) {
        Line header;
        if (NextLine(header)) {
            columns.resize(header.parameters.size());
            std::copy(header.parameters.begin(), header.parameters.end(), columns.begin());
        }
    }
    else if (_mode == Write) {
        if (!headLine.empty()) {
            fputws(headLine.c_str(), file);
            fputws(L"\n", file);
        }
    }
    return true;
}

bool Fifa07DataFile::BeforeLine(Line &outLine) {
    if (!file)
        return false;
    outLine.filePath = filePath;
    outLine.mode = mode;
    return true;
}

bool Fifa07DataFile::NextLine(Line &outLine) {
    if (!file)
        return false;
    if (mode == Read) {
        static wchar_t line[4096];
        line[0] = L'\0';
        while (true) {
            if (!fgetws(line, 4096, file))
                break;
            std::wstring wstr = line;
            if (wstr.size() > 0) {
                outLine.FromString(filePath, mode, wstr);
                return true;
            }
        }
    }
    else if (mode == Write) {
        fputws(outLine.ToString(filePath, mode).c_str(), file);
        fputws(L"\n", file);
        outLine.parameters.clear();
        return true;
    }
    return false;
}

void Fifa07DataFile::Close() {
    columns.clear();
    if (file) {
        fclose(file);
        file = nullptr;
    }
}
