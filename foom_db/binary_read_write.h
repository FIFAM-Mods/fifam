#pragma once
#include "FifamTypes.h"
#include "Date.h"

class binary_file {
    FILE *mFile = nullptr;
public:
    enum class open_mode {
        read, write
    };

    binary_file(Path const &filepath, open_mode mode) {
        open(filepath, mode);
    }

    void open(Path const &filepath, open_mode mode) {
        if (mode == open_mode::read)
            _wfopen_s(&mFile, filepath.c_str(), L"rb");
        else
            _wfopen_s(&mFile, filepath.c_str(), L"wb");
    }

    ~binary_file() {
        close();
    }

    void close() {
        if (mFile) {
            fclose(mFile);
            mFile = nullptr;
        }
    }

    bool opened() {
        return mFile != nullptr;
    }

    void read(Int &out) { fread(&out, 4, 1, mFile); }
    void read(UInt &out) { fread(&out, 4, 1, mFile); }
    void read(Short &out) { fread(&out, 2, 1, mFile); }
    void read(UShort &out) { fread(&out, 2, 1, mFile); }
    void read(Char &out) { fread(&out, 1, 1, mFile); }
    void read(UChar &out) { fread(&out, 1, 1, mFile); }
    void read(Bool &out) { fread(&out, 1, 1, mFile); }

    void read(String &out) {
        UInt size = 0;
        read(size);
        out.resize(size);
        fread(&out[0], 2, size, mFile);
    }

    void read(Date &out) {
        read(out.day);
        read(out.month);
        read(out.year);
    }

    template<typename T>
    void read(T *&out) {
        Int id = -1;
        read(id);
        *reinterpret_cast<Int *>(&out) = id;
    }

    void write(Int const &value) { fwrite(&value, 4, 1, mFile); }
    void write(UInt &value) { fwrite(&value, 4, 1, mFile); }
    void write(Short &value) { fwrite(&value, 2, 1, mFile); }
    void write(UShort &value) { fwrite(&value, 2, 1, mFile); }
    void write(Char &value) { fwrite(&value, 1, 1, mFile); }
    void write(UChar &value) { fwrite(&value, 1, 1, mFile); }
    void write(Bool &value) { fwrite(&value, 1, 1, mFile); }

    void write(String const &value) {
        write(value.size());
        fwrite(value.c_str(), 2, value.size(), mFile);
    }

    void write(Date const &value) {
        write(value.day);
        write(value.month);
        write(value.year);
    }

    template<typename T>
    void write(T *&out) {
        write(reinterpret_cast<Int>(out));
    }
};
