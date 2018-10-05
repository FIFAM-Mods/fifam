#include "Json.h"
#include "Utils.h"

/*
unsigned int &OrderingSignsCount() {
    static unsigned int count = 0;
    return count;
}


void JSONBegin() {
    OrderingSignsCount() = 0;
}

std::wstring OrderedName(std::wstring const &name) {
    std::wstring result = Utils::Format(L"$ORD%010d$%s", OrderingSignsCount(), name.c_str());
    OrderingSignsCount()++;
    return result;
}

void RemoveOrderedSigns(std::wstring &str) {
    std::wstring newstr;
    size_t b = 0;
    size_t e = str.find(L"$ORD");
    while (e != std::wstring::npos) {
        if (e > b)
            newstr.append(str.substr(b, e - b));
        b = str.find(L"$", e + 4);
        if (b == std::wstring::npos)
            break;
        b += 1;
        e = str.find(L"$ORD", b);
    }
    if (b != std::wstring::npos) {
        auto len = str.length();
        if (b < len)
            newstr.append(str.substr(b));
    }
    str = newstr;
}

void JSONEnd() {
    OrderingSignsCount() = 0;
}

template<typename T, typename A = std::enable_if_t<std::is_arithmetic_v<T>>>
void JSONGet(Json const &root, std::string const &key, T &out) {
    try {
        out = root[key].get<T>();
    }
    catch(...) {
        out = 0;
    }
}

void JSONGet(Json const &root, std::string const &key, std::wstring &out) {
    try {
        out = root[key].get<std::wstring>();
    }
    catch (...) {
        out.clear();
    }
}

json jsonReadFromFile(char const *filepath) {
    json result;
    FILE *f = qfopen(filepath, "rb");
    if (f) {
        qfseek(f, 0, SEEK_END);
        auto length = qftell(f);
        qfseek(f, 0, SEEK_SET);
        auto buffer = new char[length + 1];
        qfread(f, buffer, length);
        buffer[length] = '\0';
        qfclose(f);
        try {
            result = json::parse(buffer);
        }
        catch (std::exception &e) {
            warning("Unable to read json from file \n%s\n%s", filepath, e.what());
        }
        delete[] buffer;
    }
    return result;
}

bool jsonWriteToFile(json const &j, char const *filepath) {
    auto outFile = qfopen(filepath, "wt");
    if (outFile) {
        bool result = true;
        try {
            qstring jsonStr = j.dump(4).c_str();
            jsonRemoveOrderingSigns(jsonStr);
            qfputs(jsonStr.c_str(), outFile);
        }
        catch (std::exception &e) {
            warning("Unable to write json data to file\n%s\n%s", filepath, e.what());
            result = false;
        }
        qfclose(outFile);
        return result;
    }
    warning("Unable to open '%s'", filepath);
    return false;
}
*/