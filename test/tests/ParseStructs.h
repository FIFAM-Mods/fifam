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
        Vector<String> dependencies;
        UInt position;
        UInt freePrevPos;
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
        UInt elCounter = 1;
        const UInt maxIterations = 100'000;
        UInt totalIterations = 0;
        Set<String> ignoredDependencies = { L"AptFileSavedInputState" };
        while (!r.IsEof()) {
            String line = r.ReadFullLine();
            Utils::Replace(line, L"@", L"");
            Utils::Replace(line, L"$", L"");
            if (Utils::EndsWith(line, L"{") && line.find(L'(') == String::npos) {
                auto parts = Utils::Split(line, L' ');
                if (parts.size() > 1 && parts[1] != L"{") {
                    if (parts[0] == L"class" || parts[0] == L"struct" || parts[0] == L"union" || parts[0] == L"enum") {
                        Vector<String> lines;
                        Vector<String> dependencies;
                        auto p1 = Utils::Split(line.substr(0, line.size() - 1), L':', true, true, false);
                        if (p1.size() == 2) {
                            p1 = Utils::Split(p1[1], L',', true, true, false);
                            for (auto p : p1) {
                                auto w = Utils::Split(p, L' ', true, true, false);
                                if (w.size() >= 2 && (w[0] == L"public" || w[0] == L"private")) {
                                    String depName;
                                    if (w.size() == 2)
                                        depName = w[1];
                                    else if (w.size() == 3 && w[1] == L"virtual")
                                        depName = w[2];
                                    if (!depName.empty() && depName != parts[1] && !Utils::Contains(dependencies, depName) && !Utils::Contains(ignoredDependencies, depName))
                                        dependencies.push_back(depName);
                                }
                            }
                        }
                        lines.push_back(line);
                        while (true) {
                            line = r.ReadFullLine();
                            Utils::Replace(line, L"@", L"");
                            Utils::Replace(line, L"$", L"");
                            if (Utils::StartsWith(line, L"};")) {
                                lines.push_back(L"};");
                                break;
                            }
                            else {
                                auto tmp = line;
                                Utils::Trim(tmp);
                                auto p2 = Utils::Split(tmp, L' ', true, true, false);
                                if (p2.size() >= 3) {
                                    if (p2[0] == L"class" || p2[0] == L"struct" || p2[0] == L"union" || p2[0] == L"enum") {
                                        if (p2[1] != parts[1] && tmp.find(L'*') == tmp.npos && tmp.find(L'&') == tmp.npos && tmp.find(L'{') == tmp.npos) {
                                            if (!Utils::Contains(dependencies, p2[1]) && !Utils::Contains(ignoredDependencies, p2[1]))
                                                dependencies.push_back(p2[1]);
                                        }
                                    }
                                }
                                lines.push_back(line);
                            }
                        }
                        UInt totalSize = 0;
                        for (auto l : lines)
                            totalSize += l.size();
                        String id = parts[1];
                        if (!Utils::Contains(ElementsMap(), id)) {
                            auto e = new Element;
                            e->name = parts[1];
                            e->type = parts[0];
                            e->lines = lines;
                            e->totalSize = totalSize;
                            e->position = elCounter * maxIterations;
                            e->freePrevPos = e->position - 1;
                            e->dependencies = dependencies;
                            elCounter++;
                            Elements().push_back(e);
                            ElementsMap()[id] = e;
                        }
                        else {
                            auto e = ElementsMap()[id];
                            if (totalSize > e->totalSize) {
                                e->lines = lines;
                                e->totalSize = totalSize;
                                e->dependencies = dependencies;
                            }
                        }
                    }
                }
            }
        }
        Set<String> missingDependencies;
        for (totalIterations = 0; totalIterations < maxIterations; totalIterations++) {
            bool reordered = false;
            for (auto i : Elements()) {
                if (!i->dependencies.empty()) {
                    for (auto dname : i->dependencies) {
                        if (Utils::Contains(ElementsMap(), dname)) {
                            Element *d = ElementsMap()[dname];
                            if (d->position > i->position) {
                                UInt prevPos = d->position;
                                d->position = i->freePrevPos--;
                                d->freePrevPos = d->position - 1;
                                reordered = true;
                                wcout << totalIterations << ". Reordered " << dname << ": " << prevPos << " => " << d->position << std::endl;
                                break;
                            }
                        }
                        else
                            missingDependencies.insert(dname);
                    }
                }
                if (reordered)
                    break;
            }
            if (reordered)
                Utils::Sort(Elements(), [](Element *a, Element *b) { return a->position < b->position; });
            else
                break;
        }
        Utils::Sort(Elements(), [](Element *a, Element *b) { return a->position < b->position; });
        FifamWriter w("types.h");
        for (auto i : Elements()) {
            wcout << i->type << L" " << i->name << std::endl;
            if (!i->dependencies.empty()) {
                w.WriteLine(L"// dependencies:");
                for (auto d : i->dependencies)
                    w.WriteLine(Utils::Format(L"// %s", d));
            }
            for (auto l : i->lines)
                w.WriteLine(l);
            w.WriteNewLine();
            delete i;
        }
        if (!missingDependencies.empty()) {
            wcout << std::endl << L"Missing dependencies:" << std::endl;
            for (auto i : missingDependencies)
                wcout << i << std::endl;
        }
        wcout << "Total iterations: " << totalIterations;
    }
};
