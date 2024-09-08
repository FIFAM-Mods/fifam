#pragma once
#include "FifamReadWrite.h"
#include "Error.h"

class TranslationHashes {
public:
    enum Op { Add, Remove };

    bool Convert(Op op) {
        Path p = R"(D:\Games\FIFA Manager 22\plugins\ucp\Translations_UKR.tr)";
        FifamReader r(p, false, false);
        Vector<String> lines;
        lines.push_back(r.ReadFullLine());
        UInt lineId = 1;
        while (!r.IsEof()) {
            lineId++;
            if (!r.EmptyLine()) {
                String line = r.ReadFullLine();
                if (op == Add) {
                    auto bp = line.find(L'|');
                    if (bp == String::npos)
                        return ::Error("No Hash at line %d", lineId);
                    auto hashStr = line.substr(0, bp);
                    if (!Utils::StartsWith(hashStr, L"HASH#"))
                        return ::Error("Wrong Hash format at line %d", lineId);
                    lines.push_back(line + L" (" + hashStr.substr(5) + L")");
                }
                else {
                    auto bp = line.rfind(L" (");
                    if (bp == String::npos)
                        return ::Error("No ID at line %d", lineId);
                    lines.push_back(line.substr(0, bp));
                }
            }
            else
                r.SkipLine();
        }
        FifamWriter w(p);
        for (auto line : lines)
            w.WriteLine(line);
        return true;
    }

    TranslationHashes() {
        Convert(Remove);
    }
};
