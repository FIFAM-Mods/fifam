#pragma once
#include <string>
#include <vector>

struct Model {
    struct Vertex {
        float x, y, z, u, v;
    };
    struct Triangle {
        Vertex v[3];
    };
    struct ModelPart {
        std::wstring name;
        std::vector<Triangle> tris;
    };

    std::vector<ModelPart> parts;
};

bool ReadObjModel(Model &model, wchar_t const *path);
