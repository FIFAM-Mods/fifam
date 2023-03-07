#pragma once
#include "FifamDbEnvironment.h"

class TranslationStrip {
public:
    static void CsvToTr() {
        FifamWriter w("Translations_UKR.tr");
        FifamWriter w_eng("Translations_UKR_eng.tr");
        FifamWriter w_empty("Translations_UKR_eng.tr");
        FifamReader r("D:\\Games\\FIFA Manager 22\\fmdata\\ukr\\Translations.csv", 14, false, false);
        if (r.Available()) {
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    auto line = r.ReadFullLine();
                    if (line.size() < 15)
                        ::Error(L"Wrong line size: '%s'", line.c_str());
                    else {
                        auto i = line.find(L"#0");
                        if (i == String::npos)
                            ::Error(L"Wrong line ending: '%s'", line.c_str());
                        else {
                            auto hashValue = line.substr(0, 12);
                            Utils::Trim(hashValue);
                            auto strValue = line.substr(13, i - 13);
                            auto strTrimmed = strValue;
                            Utils::Trim(strTrimmed);
                            if (strTrimmed.empty())
                                w_empty.WriteLine(L"HASH#" + hashValue + L"|" + strValue);
                            else {
                                bool isTranslated = false;
                                for (auto c : strValue) {
                                    if (c >= 0x410 && c <= 0x44F) {
                                        w.WriteLine(L"HASH#" + hashValue + L"|" + strValue);
                                        isTranslated = true;
                                        break;
                                    }
                                }
                                if (!isTranslated)
                                    w_eng.WriteLine(L"HASH#" + hashValue + L"|" + strValue);
                            }
                        }
                    }
                }
                else
                    r.SkipLine();
            }
        }
    }

    static void CsvToTxt() {
        FifamWriter w("Translations_UKR.txt");
        FifamWriter w_eng("Translations_UKR_eng.txt");
        FifamReader r("D:\\Games\\FIFA Manager 22\\fmdata\\ukr\\Translations.csv", 14, false, false);
        if (r.Available()) {
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    auto line = r.ReadFullLine();
                    if (line.size() < 15)
                        ::Error(L"Wrong line size: '%s'", line.c_str());
                    else {
                        auto i = line.find(L"#0");
                        if (i == String::npos)
                            ::Error(L"Wrong line ending: '%s'", line.c_str());
                        else {
                            auto strValue = line.substr(13, i - 13);
                            auto strTrimmed = strValue;
                            Utils::Trim(strTrimmed);
                            if (!strTrimmed.empty()) {
                                bool isTranslated = false;
                                for (auto c : strValue) {
                                    if (c >= 0x410 && c <= 0x44F) {
                                        w.WriteLine(strValue);
                                        isTranslated = true;
                                        break;
                                    }
                                }
                                if (!isTranslated)
                                    w_eng.WriteLine(strValue);
                            }
                        }
                    }
                }
                else
                    r.SkipLine();
            }
        }
    }

    static void TxtToTr() {
        FifamReader r1("Translations.tr");
        FifamReader r2("Translations_UKR.txt");
        FifamWriter w("Translations_UKR.tr");
        if (r1.Available() && r2.Available()) {
            while (!r1.IsEof()) {
                if (!r1.EmptyLine()) {
                    auto line1 = r1.ReadFullLine();
                    auto line2 = r2.ReadFullLine();
                    auto i = line1.find(L"|");
                    if (i == String::npos)
                        ::Error(L"Wrong line: '%s'", line1.c_str());
                    else
                        w.WriteLine(line1.substr(0, i + 1) + line2);
                }
                else {
                    r1.SkipLine();
                    r2.SkipLine();
                }
            }
        }
    }

    static void TrToCsv() {

    }

    TranslationStrip() {
        CsvToTr();
    }
};
