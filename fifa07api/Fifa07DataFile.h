#pragma once
#include <string>
#include <vector>
#include <filesystem>

class Fifa07DataFile {
    FILE *file = nullptr;
    std::filesystem::path filePath;
public:
    enum Mode {
        Read, Write
    };
private:
    Mode mode = Read;
public:
    class Line {
    public:
        std::filesystem::path filePath;
        unsigned int currentParam = 0;
        std::vector<std::wstring> parameters;
        Mode mode = Read;

        void FromString(std::filesystem::path const &filepath, Mode _mode, std::wstring const &str);
        Line &operator>>(int &out);
        Line &operator>>(std::wstring &out);
        std::wstring ToString(std::filesystem::path const &filepath, Mode _mode);
    };

    std::vector<std::wstring> columns;

    ~Fifa07DataFile();
    bool Open(std::filesystem::path const &filepath, Mode _mode = Read, std::wstring const &headLine = std::wstring());
    bool NextLine(Line &outLine);
    bool BeforeLine(Line &outLine);
    void Close();
};
