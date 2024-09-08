#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"
#include "Error.h"

class Translation {
public:
    static UInt getTextHash(String const &s) {
        std::string ss = Utils::WtoA(s);
        char const *text = ss.c_str();
        char const *curr = text;
        unsigned int result = 0;
        char letter = *text;
        if (*text) {
            do {
                ++curr;
                result = letter + 65599 * result;
                letter = *curr;
            } while (*curr);
        }
        return result;
    }

    void ReadTranslationHuf(Path const &filePath, Map<UInt, String> &keys) {
        FifamReader trEng(filePath, false, false);
        if (trEng.Available()) {
            trEng.SkipLine();
            UInt i = 0;
            while (!trEng.IsEof()) {
                if (!(i % 10'000))
                    std::wcout << L"Line: " << i << std::endl;
                i++;
                auto l = trEng.ReadFullLine();
                if (l.size() >= 15 && l[12] == L';') {
                    auto k = l.substr(0, 12);
                    Utils::Trim(k);
                    auto key = Utils::SafeConvertInt<UInt>(k);
                    String t;
                    if (Utils::EndsWith(l, L"#0"))
                        t = l.substr(13, l.size() - 15);
                    else if (Utils::EndsWith(l, L"#0s") || Utils::EndsWith(l, L"#0ì"))
                        t = l.substr(13, l.size() - 16);
                    else {
                        t = l.substr(13);
                        l = trEng.ReadFullLine();
                        Bool endOfFile = false;
                        while (true) {
                            if (Utils::EndsWith(l, L"#0"))
                                break;
                            else if (Utils::EndsWith(l, L"#0s")) {
                                endOfFile = true;
                                break;
                            }
                            t += L"{BR}" + l;
                            l = trEng.ReadFullLine();
                        }
                        t += L"{BR}" + l.substr(0, l.size() - (endOfFile ? 3 : 2));
                    }
                    keys[key] = t;
                }
            }
        }
    }

    void WriteTranslation(FifamWriter &w, Map<UInt, String> &keys, String const &k) {
        auto it = keys.find(getTextHash(k));
        if (it == keys.end() || (*it).second.empty())
            return;
        String t = (*it).second;
        Utils::Replace(t, L"\"", L"\"\"");
        w.WriteLine(k, Quoted(t));
    };

    void ExtractCityDescs() {
        Map<UInt, String> keys;
        std::wcout << L"Reading Translations.csv..." << std::endl;
        ReadTranslationHuf("Translations.csv", keys);
        std::wcout << L"Writing Translations_citydesc.csv..." << std::endl;
        FifamWriter w("Translations_citydesc.csv", 14, FifamVersion());
        w.SetReplaceQuotes(false);
        WriteTranslation(w, keys, Utils::Format(L"IDS_CITYDESC_%08X", 0));
        for (UInt countryId = 1; countryId <= 207; countryId++) {
            std::wcout << FifamNation::MakeFromInt(countryId).ToStr() << std::endl;
            for (UInt clubIndex = 1; clubIndex <= 0x3FFF; clubIndex++)
                WriteTranslation(w, keys, Utils::Format(L"IDS_CITYDESC_%08X", (countryId << 16) | clubIndex));
            WriteTranslation(w, keys, Utils::Format(L"IDS_CITYDESC_%08X", (countryId << 16) | 0xFFFF));
        }
    }

    Translation() {
        ExtractCityDescs();
        return;
        Map<UInt, String> eng;
        ReadTranslationHuf(LR"(E:\Games\FIFA Manager 22\fmdata\eng\Translations.csv)", eng);
        std::wcout << L"Writing" << std::endl;
        FifamWriter w(LR"(E:\Games\FIFA Manager 22\fmdata\eng\Translations_mail_en.csv)", 14, FifamVersion());
        w.SetReplaceQuotes(false);
        for (UInt i = 0; i < 5000; i++) {
            WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_TITLE_%d", i));
            for (UInt v = 0; v < 10; v++)
                WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_TITLE_VAR_%d_%d", v, i));
            WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_TEXT_%d", i));
            for (UInt v = 0; v < 10; v++)
                WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_TEXT_VAR_%d_%d", v, i));
            WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_REMARK_%d", i));
            for (UInt v = 0; v < 3; v++)
                WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_ALT_%d_%d", v, i));
            for (UInt v = 0; v < 3; v++)
                WriteTranslation(w, eng, Utils::Format(L"IDS_EA_MAIL_ANSWER_%d_%d", v, i));
        }

    //    for (UInt i = 0; i < 3000; i++) {
    //        for (UInt v = 0; v < 20; v++)
    //            WriteTranslation(Utils::Format(L"IDS_WEBSITE_%05d_%d", i, v));
    //    }

        //std::wcout << L"Writing TM09_INVALID_%08d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++)
        //    WriteTranslation(Utils::Format(L"TM09_INVALID_%08d", i));
        //
        //std::wcout << L"Writing TM09_CATEGORY_NAME_%04d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++)
        //    WriteTranslation(Utils::Format(L"TM09_CATEGORY_NAME_%04d", i));
        //
        //std::wcout << L"Writing TM09_CATEGORY_ENUM_%04d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++)
        //    WriteTranslation(Utils::Format(L"TM09_CATEGORY_ENUM_%04d", i));
        //
        //std::wcout << L"Writing TM09_TEXTBLOCK_HEADER_%06d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++)
        //    WriteTranslation(Utils::Format(L"TM09_TEXTBLOCK_HEADER_%06d", i));
        //
        //std::wcout << L"Writing TM09_%06d_%02d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++) {
        //    for (UInt v = 0; v < 99; v++)
        //        WriteTranslation(Utils::Format(L"TM09_%06d_%02d", i, v));
        //}
        //
        //std::wcout << L"Writing TM09LIVE_%06d_%02d" << std::endl;
        //for (UInt i = 0; i <= 9999; i++) {
        //    for (UInt v = 0; v < 99; v++)
        //        WriteTranslation(Utils::Format(L"TM09LIVE_%06d_%02d", i, v));
        //}
        //
        //std::wcout << L"Writing TM09_CONDITION_%08X" << std::endl;
        //for (UInt i = 0; i <= 0xFFFF; i++)
        //    WriteTranslation(Utils::Format(L"TM09_CONDITION_%08X", i));
        //
        //std::wcout << L"Writing TM09_RESULT_%08X" << std::endl;
        //for (UInt i = 0; i <= 0xFFFF; i++)
        //    WriteTranslation(Utils::Format(L"TM09_RESULT_%08X", i));
    }
};
