#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"
#include "Error.h"

class TrToTxt {
public:
    TrToTxt() {
        Map<UInt, String> keys;
        std::wcout << L"Reading source.txt" << std::endl;
        {
            FifamReader r("source.txt", 13, false, false);
            if (r.Available()) {
                r.SkipLine();
                UInt i = 0;
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        i++;
                        auto l = r.ReadFullLine();
                        if (l.size() >= 15 && l[12] == L';') {
                            auto k = l.substr(0, 12);
                            Utils::Trim(k);
                            auto key = Utils::SafeConvertInt<UInt>(k);
                            if (keys.contains(key))
                                ::Error("Duplicated key %u at %u", key, i);
                            auto ep = l.find(L"#0", 13);
                            if (ep != String::npos)
                                keys[key] = l.substr(13, ep - 13);
                            else
                                ::Error("Wrong line in source at %u", i);
                        }
                        else
                            ::Error("Wrong line in source at %u", i);
                    }
                    else
                        r.SkipLine();
                }
            }
        }
        std::wcout << L"Reading Translations.tr" << std::endl;
        {
            FifamReader r("Translations.tr", 13, false, false);
            if (r.Available()) {
                r.SkipLine();
                UInt i = 0;
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        i++;
                        auto l = r.ReadFullLine();
                        if (l.starts_with(L"HASH#")) {
                            auto sp = l.find(L'|', 5);
                            if (sp != String::npos) {
                                auto k = l.substr(5, sp - 5);
                                Utils::Trim(k);
                                auto key = Utils::SafeConvertInt<UInt>(k);
                                String t = l.substr(sp + 1);
                                keys[key] = t;
                            }
                            else
                                ::Error("Wrong line in source at %u", i);
                        }
                        else
                            ::Error("Wrong line in source at %u", i);
                    }
                    else
                        r.SkipLine();
                }
            }
        }
        std::wcout << L"Writing Translations.txt" << std::endl;
        FifamWriter w("Translations.txt", 14, FifamVersion());
        w.SetReplaceQuotes(false);
        w.WriteLine(L"Do not remove first line");
        Vector<Pair<UInt, String>> vecKeys;
        for (auto &[key, line] : keys)
            vecKeys.emplace_back(key, line + L"#0");
        if (!vecKeys.empty())
            vecKeys.back().second += L"ì";
        for (UInt i = 0; i < vecKeys.size(); i++)
            w.WriteLine(Utils::Format(L"%12u", vecKeys[i].first) + L";" + vecKeys[i].second);
    }
};
