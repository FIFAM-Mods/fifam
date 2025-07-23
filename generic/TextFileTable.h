#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>

enum eEncoding {
    ENCODING_ANSI,
    ENCODING_UTF8,
    ENCODING_UTF8_BOM,
    ENCODING_UTF16LE_BOM,
    ENCODING_UTF16BE_BOM
};

class TextFileTable {
    std::vector<std::vector<std::wstring>> mCells;

    static std::wstring Unquoted(std::wstring const &str);
    static std::wstring Quoted(std::wstring const &str, wchar_t separator);
    size_t NumRowsToWrite() const;
    template <typename T>
    static std::wstring to_wstring(T const &value) {
        if constexpr (std::is_same_v<std::decay_t<T>, std::wstring>)
            return value;
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
            return std::wstring(value.begin(), value.end());
        else if constexpr (std::is_arithmetic_v<std::decay_t<T>> || std::is_integral_v<std::decay_t<T>>)
            return std::to_wstring(value);
        else {
            std::wstringstream wss;
            wss << value;
            return wss.str();
        }
    }
public:
    size_t NumRows() const;
    size_t NumColumns(size_t row) const;
    bool IsConsistent() const;
    size_t MaxColumns() const;
    std::vector<std::vector<std::wstring>> const &Rows() const;
    std::vector<std::wstring> const &Row(size_t row) const;
    std::wstring const &Cell(size_t column, size_t row) const;
    void AddRow(std::vector<std::wstring> const &row);
    template <typename... ArgTypes>
    void AddRow(ArgTypes... args) {
        std::vector<std::wstring> vec;
        (vec.push_back(to_wstring(args)), ...);
        mCells.push_back(vec);
    }
    void Clear();
    bool Read(std::filesystem::path const &filename, wchar_t separator = L',');
    bool ReadCSV(std::filesystem::path const &filename, wchar_t separator = L',');
    bool ReadTSV(std::filesystem::path const &filename);
    bool ReadUnicodeText(std::filesystem::path const &filename);
    bool Write(std::filesystem::path const &filename, wchar_t separator = L',', eEncoding encoding = ENCODING_UTF8_BOM);
    bool WriteCSV(std::filesystem::path const &filename, wchar_t separator = L',');
    bool WriteTSV(std::filesystem::path const &filename);
    bool WriteUnicodeText(std::filesystem::path const &filename);
};
