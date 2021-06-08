#pragma once
#include "FifamDbEnvironment.h"
#include "FoomDbEnvironment.h"

vector<wstring> ids = {
    L"Stefan Kießling",
    L"Necati Ateş",
    L"Gylfi Sigurðsson",
    L"Kolbeinn Sigþórsson",
    L"Șerban Nițu",
    L"David N´Dré",
    L"Josémi Ramírez",
    L"Namiq Ələsgərov",
    L"ǻǼǽל",
    L"'`’´ ",
    L"ÞßðþƏȘșȚțəсứ"
};

wstring ID_fix(wstring const &name) {
    static char buf[1024];
    BOOL hasUnknown = FALSE;
    WideCharToMultiByte(20127, 512, name.c_str(), -1, buf, 1023, "?", &hasUnknown);
    size_t len = strlen(buf);
    if (hasUnknown == TRUE) {
        for (size_t i = 0; i < len; i++) {
            if (buf[i] == '?') {
                if (name[i] == 0xDF) buf[i] = 's';       // 'ß'
                else if (name[i] == 0x218) buf[i] = 'S'; // 'Ș'
                else if (name[i] == 0x219) buf[i] = 's'; // 'ș'
                else if (name[i] == 0x21A) buf[i] = 'T'; // 'Ț'
                else if (name[i] == 0x21B) buf[i] = 't'; // 'ț'
                else if (name[i] == 0xF0) buf[i] = 'o'; // '˰'
                else if (name[i] == 0xFE) buf[i] = 'P';  // 'þ'
            }
        }
    }
    return Utils::AtoW(buf);
}

wstring ID(wstring const &name, bool editor = false, bool replaceUnknown = true) {
    static char buf[1024];
    BOOL hasUnknown = FALSE;
    WideCharToMultiByte(20127, 512, name.c_str(), -1, buf, 1023, "?", &hasUnknown);
    size_t len = strlen(buf);
    if (replaceUnknown && hasUnknown == TRUE) {
        for (size_t i = 0; i < len; i++) {
            if (buf[i] == '?') {
                if (editor) {
                    if (name[i] == 0xDF) buf[i] = 's';       // 'ß'
                    else if (name[i] == 0x218) buf[i] = 'S'; // 'Ș'
                    else if (name[i] == 0x219) buf[i] = 's'; // 'ș'
                    else if (name[i] == 0x21A) buf[i] = 'T'; // 'Ț'
                    else if (name[i] == 0x21B) buf[i] = 't'; // 'ț'
                    else if (name[i] == 0xF0) buf[i] = 'd'; // 'ð'
                    else if (name[i] == 0xFE) buf[i] = 'P';  // 'þ'
                    else
                        buf[i] = name[i];
                }
                else {
                    if (name[i] == 0xD8) buf[i] = 'O';       // 'Ø'
                    else if (name[i] == 0xDE) buf[i] = 'P';  // 'Þ'
                    else if (name[i] == 0xDF) buf[i] = 's';  // 'ß'
                    else if (name[i] == 0xE6) buf[i] = 'a';  // 'æ'
                    else if (name[i] == 0xF0) buf[i] = 'o';  // 'ð'
                    else if (name[i] == 0xF8) buf[i] = 'o';  // 'ø'
                    else if (name[i] == 0xFE) buf[i] = 'P';  // 'þ'
                    else if (name[i] == 0x110) buf[i] = 'D'; // 'Đ'
                    else if (name[i] == 0x111) buf[i] = 'd'; // 'đ'
                    else if (name[i] == 0x131) buf[i] = 'i'; // 'ı'
                    else if (name[i] == 0x141) buf[i] = 'L'; // 'Ł'
                    else if (name[i] == 0x142) buf[i] = 'l'; // 'ł'
                    else if (name[i] == 0x153) buf[i] = 'c'; // 'œ'
                    else
                        buf[i] = name[i];
                }
            }
        }
    }
    static wchar_t buf2[1024];
    wcscpy(buf2, name.c_str());
    MultiByteToWideChar(20127, 0, buf, -1, buf2, name.size());
    if (editor) {
        for (size_t i = 0; i < wcslen(buf2); i++) {
            static unsigned char editorTable[] = {
                0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
                0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
                0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
                0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
                0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
                0x45, 0x45, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49, 0x45, 0x4E, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x58,
                0x4F, 0x55, 0x55, 0x55, 0x55, 0x59, 0x54, 0x73, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x63,
                0x65, 0x65, 0x65, 0x65, 0x69, 0x69, 0x69, 0x69, 0x65, 0x6E, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
                0x6F, 0x75, 0x75, 0x75, 0x75, 0x79, 0x74, 0x79
            };
            buf2[i] = unsigned char(editorTable[unsigned char(buf2[i])]);
        }
    }
    return buf2;
}



void FindUnsupportedSymbolsInName(map<wchar_t, unsigned int> &u, wstring const &name, bool strangeSymbols = false) {
    static char buf[1024];
    BOOL hasUnknown = FALSE;
    WideCharToMultiByte(20127, 512, name.c_str(), -1, buf, 1023, "?", &hasUnknown);
    if (!strangeSymbols && !hasUnknown)
        return;
    size_t len = strlen(buf);
    for (size_t i = 0; i < len; i++) {
        if (strangeSymbols) {
            if (buf[i] != '?') {
                if ((buf[i] < 'a' || buf[i] > 'z') && (buf[i] < 'A' || buf[i] > 'Z'))
                    u[name[i]]++;
            }
        }
        else {
            if (buf[i] == '?')
                u[name[i]]++;
        }
    }
}

void FindUnsupportedSymbols(bool strangeSymbols) {
    map<wchar_t, unsigned int> u;
    auto foomdb = GetEnvironment<FoomDbEnvironment>().GetDatabase();
    for (auto const &p : foomdb->mPlayers) {
        FindUnsupportedSymbolsInName(u, p.second.mFirstName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mSecondName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mCommonName, strangeSymbols);
    }
    for (auto const &p : foomdb->mNonPlayers) {
        FindUnsupportedSymbolsInName(u, p.second.mFirstName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mSecondName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mCommonName, strangeSymbols);
    }
    for (auto const &p : foomdb->mOfficials) {
        FindUnsupportedSymbolsInName(u, p.second.mFirstName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mSecondName, strangeSymbols);
        FindUnsupportedSymbolsInName(u, p.second.mCommonName, strangeSymbols);
    }
    FifamWriter w(strangeSymbols ? "names_strange_symbols.csv" : "names_unsupported_symbols.csv");
    w.WriteLine(L"Letter", L"Number", L"Count");
    for (auto const &i : u)
        w.WriteLineWithSeparator(L'\t', wstring(1, i.first), unsigned short(i.first), i.second);
}

class PlayerIdTest {
public:
    PlayerIdTest() {
        //FindUnsupportedSymbols(false);
        //FindUnsupportedSymbols(true);
        wcout << endl;
        for (auto const &id : ids)
            wcout << L"'" << id <<  L"' Editor: '" << ID(id, true, true) << L"' Game: '" << ID(id, false, true) << "' Raw: '" << ID(id, false, false) << "'" << endl;
    }
};
