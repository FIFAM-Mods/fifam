#pragma once
#include <string>
#include <vector>
#include <fstream>

class FifaDataFile {
    std::wifstream stream;
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
    bool Open(std::wstring filepath);
    bool NextLine(Line &outLine);
    void Close();
};