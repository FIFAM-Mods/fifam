#pragma once
#include "Error.h"
#include "FifamReadWrite.h"

class FifaCommentaryFile {
public:
    FifaCommentaryFile() {
        Map<String, Vector<Vector<UInt>>> commentary;
        FifamReader r("commentary.txt");
        while (!r.IsEof()) {
            if (!r.EmptyLine()) {
                auto line = r.ReadFullLine();
                if (!Utils::EndsWith(line, L".wav")) {
                    ::Error("Not a .wav file");
                    continue;
                }
                else
                    line = line.substr(0, line.size() - 4);
                Vector<UInt> vars;
                auto parts = Utils::Split(line, L'_', false, false, false);
                String key;
                bool first = true;
                for (auto const &part : parts) {
                    if (!first)
                        key += L"_";
                    else
                        first = false;
                    if (Utils::IsNumber(part)) {
                        UInt value = Utils::SafeConvertInt<UInt>(part);
                        vars.push_back(value);
                        key += Utils::Format(L"%%%d", vars.size());
                    }
                    else
                        key += part;
                }
                commentary[key].push_back(vars);
            }
            else
                r.SkipLine();
        }
        FifamWriter w("commentary_sorted.txt");
        for (auto [key, vec] : commentary) {
            if (vec.size() == 1 && vec[0].size() == 0)
                w.WriteLine(key);
            else {
                if (vec.size() > 1) {
                    std::sort(vec.begin(), vec.end(), [](Vector<UInt> const &a, Vector<UInt> const &b) {
                        for (UInt i = 0; i < a.size(); i++) {
                            if (a[i] < b[i])
                                return true;
                            if (a[i] > b[i])
                                return false;
                        }
                        return true;
                    });
                }
                for (auto const &p : vec) {
                    auto formattedKey = key;
                    for (UInt f = 0; f < p.size(); f++)
                        Utils::Replace(formattedKey, Utils::Format(L"%%%d", f + 1), Utils::Format(L"%d", p[f]));
                    w.WriteLine(formattedKey + L".wav");
                }
            }
        }
    }
};
