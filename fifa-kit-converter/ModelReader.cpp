#include "ModelReader.h"
#include "FifamReadWrite.h"
#include "Error.h"

bool ReadObjModel(Model &model, wchar_t const *path) {
    struct Pos {
        float x, y, z;
    };
    struct UV {
        float u, v;
    };
    FifamReader reader(path, 14);
    if (reader.Available()) {
        std::vector<Pos> positions;
        std::vector<UV> texcoords;
        unsigned int lineNumber = 0;
        while (!reader.IsEof()) {
            lineNumber++;
            auto line = reader.ReadFullLine();
            if (!line.empty() && line[0] != L'#') {
                if (Utils::StartsWith(line, L"o ")) {
                    auto p = Utils::Split(line, L' ');
                    if (p.size() == 2) {
                        auto &part = model.parts.emplace_back();
                        part.name = p[1];
                    }
                    else {
                        ::Error(L"obj parsing error: unable to get model part name");
                        return false;
                    }
                }
                else if (Utils::StartsWith(line, L"v ")) {
                    if (model.parts.empty()) {
                        ::Error(L"obj parsing error: model part name was not found");
                        return false;
                    }
                    auto p = Utils::Split(line, L' ');
                    if (p.size() == 4) {
                        auto &v = positions.emplace_back();
                        v.x = Utils::SafeConvertFloat(p[1]);
                        v.y = Utils::SafeConvertFloat(p[2]);
                        v.z = Utils::SafeConvertFloat(p[3]);
                    }
                    else {
                        ::Error(L"obj parsing error: unable to get vertex position");
                        return false;
                    }
                }
                else if (Utils::StartsWith(line, L"vt ")) {
                    if (model.parts.empty()) {
                        ::Error(L"obj parsing error: model part name was not found");
                        return false;
                    }
                    auto p = Utils::Split(line, L' ');
                    if (p.size() == 3) {
                        auto &v = texcoords.emplace_back();
                        v.u = Utils::SafeConvertFloat(p[1]);
                        v.v = Utils::SafeConvertFloat(p[2]);
                    }
                    else {
                        ::Error(L"obj parsing error: unable to get vertex texture coordinates");
                        return false;
                    }
                }
                else if (Utils::StartsWith(line, L"f ")) {
                    if (model.parts.empty()) {
                        ::Error(L"obj parsing error: model part name was not found");
                        return false;
                    }
                    auto p = Utils::Split(line, L' ');
                    if (p.size() == 4) {
                        auto &t = model.parts.back().tris.emplace_back();
                        for (unsigned int i = 0; i < 3; i++) {
                            auto tp = Utils::Split(p[i + 1], L'/');
                            if (tp.size() == 3) {
                                int index = Utils::SafeConvertInt<int>(tp[0]);
                                if (index == 0) {
                                    ::Error(L"obj parsing error: incorrect vertex index (0) (at line %d)", lineNumber);
                                    return false;
                                }
                                index--;
                                if (index < (int)positions.size()) {
                                    t.v[i].x = positions[index].x;
                                    t.v[i].y = positions[index].y;
                                    t.v[i].z = positions[index].z;
                                }
                                else {
                                    ::Error(L"obj parsing error: incorrect vertex index (at line %d)", lineNumber);
                                    return false;
                                }
                                index = Utils::SafeConvertInt<int>(tp[1]);
                                if (index == 0) {
                                    ::Error(L"obj parsing error: incorrect vertex index (0) (at line %d)", lineNumber);
                                    return false;
                                }
                                index--;
                                if (index < (int)positions.size()) {
                                    t.v[i].u = texcoords[index].u;
                                    t.v[i].v = texcoords[index].v;
                                }
                                else {
                                    ::Error(L"obj parsing error: incorrect vertex index (at line %d)", lineNumber);
                                    return false;
                                }
                            }
                            else {
                                ::Error(L"obj parsing error: unable to get triangle vertex data");
                                return false;
                            }
                        }
                    }
                    else {
                        ::Error(L"obj parsing error: unable to get triangle vertices");
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
