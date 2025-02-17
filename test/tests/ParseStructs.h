#pragma once
#include "FifamReadWrite.h"
#include <iostream>

class ParseStructs {
public:
    struct Element {
        String type;
        String name;
        Vector<String> lines;
        UInt totalSize = 0;
    };

    static Vector<Element *> &Elements() {
        static Vector<Element *> elements;
        return elements;
    }

    static Map<String, Element *> &ElementsMap() {
        static Map<String, Element *> elementsMap;
        return elementsMap;
    }

    ParseStructs() {
        FifamReader r("fifa_d.txt", 13, false, false);
        while (!r.IsEof()) {
            String line = r.ReadFullLine();
            std::erase(line, L'@');
            std::erase(line, L'$');
            if (Utils::EndsWith(line, L"{") && line.find(L'(') == String::npos) {
                auto parts = Utils::Split(line, L' ');
                if (parts.size() > 1 && parts[1] != L"{") {
                    if (parts[0] == L"class" || parts[0] == L"struct" || parts[0] == L"union" || parts[0] == L"enum") {
                        Vector<String> lines;
                        lines.push_back(line);
                        while (true) {
                            line = r.ReadFullLine();
                            if (Utils::StartsWith(line, L"};")) {
                                lines.push_back(L"};");
                                break;
                            }
                            else
                                lines.push_back(line);
                        }
                        UInt totalSize = 0;
                        for (auto l : lines)
                            totalSize += l.size();
                        String id = parts[0] + L" " + parts[1];
                        if (!Utils::Contains(ElementsMap(), id)) {
                            auto e = new Element;
                            e->name = parts[1];
                            e->type = parts[0];
                            e->lines = lines;
                            e->totalSize = totalSize;
                            Elements().push_back(e);
                            ElementsMap()[id] = e;
                        }
                        else {
                            auto e = ElementsMap()[id];
                            if (totalSize > e->totalSize) {
                                e->lines = lines;
                                e->totalSize = totalSize;
                            }
                        }
                    }
                }
            }
        }
        FifamWriter w("types.h");
        for (auto i : Elements()) {
            wcout << i->type << L" " << i->name << std::endl;
            for (auto l : i->lines)
                w.WriteLine(l);
            w.WriteNewLine();
            delete i;
        }
    }
};