#pragma once
#include <string>
#include <vector>
#include <filesystem>

class FifaDataFile {
    FILE *file = nullptr;
public:
    class Line {
    public:
        unsigned int currentParam = 0;
        std::vector<std::wstring> parameters;

        void FromString(std::wstring const &str);
        Line &operator>>(int &out);
        Line &operator>>(std::wstring &out);
    };

    std::vector<std::wstring> columns;

    ~FifaDataFile();
    bool Open(std::filesystem::path const &filepath);
    bool NextLine(Line &outLine);
    void Close();
};
