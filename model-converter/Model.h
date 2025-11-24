#pragma once
#include <Windows.h>
#undef min
#undef max
#include <string>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <variant>
#include <unordered_set>
#include <filesystem>
#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>
#pragma push_macro("isnan")
#pragma push_macro("finite")
#include <fbxsdk.h>
#pragma pop_macro("finite")
#pragma pop_macro("isnan")
#include <iostream>

using Matrix4x4 = FbxAMatrix;
using Quaternion = FbxQuaternion;

struct Vector2 {
    float x, y;
    Vector2() { x = y = 0.0f; };
    Vector2(float X, float Y) { x = X; y = Y; };
    void Set(float X, float Y) { x = X; y = Y; }
    float SquareLength() const { return x * x + y * y; }
    float Length() const { return std::sqrt(SquareLength()); }
    Vector2 &Normalize() {
        float l = Length();
        if (l != 0.0f) *this /= Length();
        return *this;
    }
    Vector2 const &operator+=(Vector2 const &o) { x += o.x; y += o.y; return *this; }
    Vector2 const &operator-=(Vector2 const &o) { x -= o.x; y -= o.y; return *this; }
    Vector2 const &operator*=(float f) { x *= f; y *= f; return *this; }
    Vector2 const &operator/=(float f) { x /= f; y /= f; return *this; }
    float operator[](unsigned int i) const { return (i == 1) ? y : x; }
    bool operator==(Vector2 const &other) const { return x == other.x && y == other.y; }
    bool operator!=(Vector2 const &other) const { return x != other.x || y != other.y; }
    bool Equal(Vector2 const &other, float epsilon) const { return std::abs(x - other.x) <= epsilon && std::abs(y - other.y) <= epsilon; }
    Vector2 &operator=(float f) { x = y = f; return *this; }
    Vector2 const SymMul(Vector2 const &o) { return Vector2(x * o.x, y * o.y); }
    FbxDouble2 ToFbx() const { return FbxDouble2(x, y); }
    Vector2 &operator=(FbxDouble2 const &v) { x = (float)v[0]; y = (float)v[1]; return *this; }
};

inline Vector2 operator+(Vector2 const &v1, Vector2 const &v2) { return Vector2(v1.x + v2.x, v1.y + v2.y); }
inline Vector2 operator-(Vector2 const &v1, Vector2 const &v2) { return Vector2(v1.x - v2.x, v1.y - v2.y); }
inline float operator*(Vector2 const &v1, Vector2 const &v2) { return v1.x * v2.x + v1.y * v2.y; }
inline Vector2 operator*(float f, Vector2 const &v) { return Vector2(f * v.x, f * v.y); }
inline Vector2 operator*(Vector2 const &v, float f) { return Vector2(f * v.x, f * v.y); }
inline Vector2 operator/(const Vector2 &v, float f) { return v * (1 / f); }
inline Vector2 operator/(Vector2 const &v, Vector2 const &v2) { return Vector2(v.x / v2.x, v.y / v2.y); }
inline Vector2 operator-(const Vector2 &v) { return Vector2(-v.x, -v.y); }

struct Vector3 {
    float x, y, z;
    Vector3() { x = y = z = 0.0f; };
    Vector3(float X, float Y, float Z) { x = X; y = Y; z = Z; };
    void Set(float X, float Y, float Z) { x = X; y = Y; z = Z; };
    float SquareLength() const { return x * x + y * y + z * z; }
    float Length() const { return std::sqrt(SquareLength()); }
    Vector3 &Normalize() {
        float l = Length();
        if (l != 0.0f) *this /= Length();
        return *this;
    }
    Vector3 &NormalizeSafe() {
        float len = Length();
        if (len > 0.0f) *this /= len;
        return *this;
    }
    Vector3 const &operator+=(Vector3 const &o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3 const &operator-=(Vector3 const &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3 const &operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
    Vector3 const &operator/=(float f) {
        if (f == 0.0f) return *this;
        float invF = 1.0f / f;
        x *= invF; y *= invF; z *= invF;
        return *this;
    }
    float operator[](unsigned int i) const {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return x;
    }
    float &operator[](unsigned int i) {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return x;
    }
    bool operator==(Vector3 const &other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!= (Vector3 const &other) const { return x != other.x || y != other.y || z != other.z; }
    bool Equal(Vector3 const &other, float epsilon) const {
        return std::abs(x - other.x) <= epsilon && std::abs(y - other.y) <= epsilon && std::abs(z - other.z) <= epsilon;
    }
    bool operator<(Vector3 const &other) const { return x != other.x ? x < other.x : y != other.y ? y < other.y : z < other.z; }
    Vector3 const SymMul(Vector3 const &o) { return Vector3(x * o.x, y * o.y, z * o.z); }
    FbxDouble3 ToFbx() const { return FbxDouble3(x, y, z); }
    Vector3 &operator=(FbxDouble3 const &v) { x = (float)v[0]; y = (float)v[1]; z = (float)v[2]; return *this; }
};

inline Vector3 operator-(Vector3 const &v) { return Vector3(-v.x, -v.y, -v.z); }
inline Vector3 operator+(Vector3 const &v1, Vector3 const &v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
inline Vector3 operator-(Vector3 const &v1, Vector3 const &v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
inline float operator*(Vector3 const &v1, Vector3 const &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline Vector3 operator*(float f, Vector3 const &v) { return Vector3(f * v.x, f * v.y, f * v.z); }
inline Vector3 operator*(Vector3 const &v, float f) { return Vector3(f * v.x, f * v.y, f * v.z); }
inline Vector3 operator/(Vector3 const &v, float f) { return v * (1 / f); }
inline Vector3 operator/(Vector3 const &v, Vector3 const &v2) { return Vector3(v.x / v2.x, v.y / v2.y, v.z / v2.z); }
inline Vector3 operator^(Vector3 const &v1, Vector3 const &v2) {
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

struct Vector4 {
    float x, y, z, w;
    Vector4() { x = y = z = w = 0.0f; };
    Vector4(float X, float Y, float Z, float W) { x = X; y = Y; z = Z; w = W; };
    void Set(float X, float Y, float Z, float W) { x = X; y = Y; z = Z; w = W; };
    float operator[](unsigned int i) const {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        return x;
    }
    float &operator[](unsigned int i) {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        return x;
    }
    FbxDouble4 ToFbx() const { return FbxDouble4(x, y, z, w); }
    Vector4 &operator=(FbxDouble4 const &v) { x = (float)v[0]; y = (float)v[1]; z = (float)v[2]; w = (float)v[3]; return *this; }
};

struct RGBA {
    unsigned char r, g, b, a;
    inline RGBA() { r = g = b = a = 255; }
    inline RGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A) { r = R; g = G; b = B; a = A; }
    inline void Set(unsigned char R, unsigned char G, unsigned char B, unsigned char A) { r = R; g = G; b = B; a = A; }
    inline void Set(float R, float G, float B, float A) {
        r = (unsigned char)(R * 255.0f); g = (unsigned char)(G * 255.0f);
        b = (unsigned char)(B * 255.0f); a = (unsigned char)(A * 255.0f);
    }
    inline void Set(double R, double G, double B, double A) {
        r = (unsigned char)(R * 255.0); g = (unsigned char)(G * 255.0);
        b = (unsigned char)(B * 255.0); a = (unsigned char)(A * 255.0);
    }
    inline RGBA const &operator+=(RGBA const &o) { r += o.r; g += o.g; b += o.b; a += o.a; return *this; }
    inline RGBA const &operator-=(RGBA const &o) { r -= o.r; g -= o.g; b -= o.b; a -= o.a; return *this; }
    inline RGBA const &operator*=(RGBA const &o) { r *= o.r; g *= o.g; b *= o.b; a *= o.a; return *this; }
    inline RGBA const &operator/=(RGBA const &o) { r /= o.r; g /= o.g; b /= o.b; a /= o.a; return *this; }
    inline unsigned char operator[](unsigned int i) const {
        switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        }
        return r;
    }
    inline unsigned char &operator[](unsigned int i) {
        switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        }
        return r;
    }
    inline bool operator==(RGBA const &other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
    inline bool operator!=(RGBA const &other) const { return r != other.r || g != other.g || b != other.b || a != other.a; }
    inline bool operator<(RGBA const &other) const {
        return r < other.r || (r == other.r && (g < other.g || (g == other.g && (b < other.b || (b == other.b && (a < other.a))))));
    }
    inline bool IsBlack() const {
        static const float epsilon = 10e-3f;
        return std::fabs(r) < epsilon && std::fabs(g) < epsilon && std::fabs(b) < epsilon;
    }
    FbxDouble4 ToFbx() const {
        return FbxDouble4((double)r / 255.0, (double)g / 255.0, (double)b / 255.0, (double)a / 255.0);
    }
};

inline RGBA operator+(RGBA const &v1, RGBA const &v2) { return RGBA(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.a + v2.a); }
inline RGBA operator-(RGBA const &v1, RGBA const &v2) { return RGBA(v1.r - v2.r, v1.g - v2.g, v1.b - v2.b, v1.a - v2.a); }
inline RGBA operator*(RGBA const &v1, RGBA const &v2) { return RGBA(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.a * v2.a); }
inline RGBA operator/(RGBA const &v1, RGBA const &v2) { return RGBA(v1.r / v2.r, v1.g / v2.g, v1.b / v2.b, v1.a / v2.a); }
inline RGBA operator+(RGBA const &v, unsigned char f) { return RGBA(f + v.r, f + v.g, f + v.b, f + v.a); }
inline RGBA operator-(RGBA const &v, unsigned char f) { return RGBA(v.r - f, v.g - f, v.b - f, v.a - f); }
inline RGBA operator+(unsigned char f, RGBA const &v) { return RGBA(f + v.r, f + v.g, f + v.b, f + v.a); }
inline RGBA operator-(unsigned char f, RGBA const &v) { return RGBA(f - v.r, f - v.g, f - v.b, f - v.a); }

using PropertyValue = std::variant<int, float, double, std::string, Vector2, Vector3, Vector4>;

enum VertexFormat {
    V_Position = 0,
    V_Normal = 1,
    V_Tangent = 2,
    V_Binormal = 4,
    V_1TexCoord = 8 * 1,
    V_2TexCoords = 8 * 2,
    V_3TexCoords = 8 * 3,
    V_4TexCoords = 8 * 4,
    V_5TexCoords = 8 * 5,
    V_6TexCoords = 8 * 6,
    V_7TexCoords = 8 * 7,
    V_8TexCoords = 8 * 8,
    V_1Color = 128 * 1,
    V_2Colors = 128 * 2,
    V_3Colors = 128 * 3,
    V_4Colors = 128 * 4,
    V_5Colors = 128 * 5,
    V_6Colors = 128 * 6,
    V_7Colors = 128 * 7,
    V_8Colors = 128 * 8,
    V_1Bone = 2048 * 1,
    V_2Bones = 2048 * 2,
    V_3Bones = 2048 * 3,
    V_4Bones = 2048 * 4,
    V_5Bones = 2048 * 5,
    V_6Bones = 2048 * 6,
    V_7Bones = 2048 * 7,
    V_8Bones = 2048 * 8
};

#define NumTexCoords(format) (((format) >> 3) & 0xF)
#define NumColors(format) (((format) >> 7) & 0xF)
#define NumBones(format) (((format) >> 11) & 0xF)

inline void SetNumTexCoords(uint32_t &format, uint8_t n) {
    format = (format & ~(0xF << 3)) | ((n & 0xF) << 3);
}
inline void SetNumColors(uint32_t &format, uint8_t n) {
    format = (format & ~(0xF << 7)) | ((n & 0xF) << 7);
}
inline void SetNumBones(uint32_t &format, uint8_t n) {
    format = (format & ~(0xF << 11)) | ((n & 0xF) << 11);
}

struct Vertex { // 144 bytes per vertex
    Vector3 pos, normal, tangent, binormal;
    Vector2 uv[8];
    RGBA colors[8];
    float boneWeights[8];
    uint16_t boneIndices[8];
};

struct MeshData {
    uint32_t vertexFormat;
    std::vector<Vertex> vertices;
    std::vector<std::array<uint32_t, 3>> triangles;
    std::string material;
    std::array<std::string, 8> uvLayerNames;
    std::array<std::string, 8> colorLayerNames;

    MeshData() {
        vertexFormat = 0;
    }
};

struct Material {
    std::string name, texture, normalMap;
    RGBA color;
    std::map<std::string, PropertyValue> properties;

    Material() {}
    Material(std::string const &n) : name(n) {}
    Material(std::string const &n, std::string const &tex) : name(n), texture(tex) {}
    Material(std::string const &n, std::string const &tex, RGBA const &col) : name(n), texture(tex), color(col) {}
};

struct Texture {
    std::string name;
    std::string filename;
    std::map<std::string, PropertyValue> properties;

};

struct Object {
    std::string name, parent;
    Matrix4x4 transform;
    MeshData mesh;
    std::map<std::string, PropertyValue> properties;
};

struct Bone {
    std::string name, parent;
    Matrix4x4 transform;
    std::map<std::string, PropertyValue> properties;
};

struct Skeleton {
    std::vector<Bone> bones;
    std::map<std::string, PropertyValue> properties;
};

enum ErrorCode {
    ERROR_NONE = 0,
    ERROR_UNABLE_TO_CREATE_FBX_MANAGER,
    ERROR_UNABLE_TO_LOAD_SCENE,
    ERROR_UNABLE_TO_LOAD_COMPLETE_SCENE,
    ERROR_UNABLE_TO_FIND_SCENE_ROOT_NODE,
};

inline void Error(std::string const &text) {
    MessageBoxA(NULL, text.c_str(), "Error", MB_ICONERROR);
}

const size_t MAX_VERTICES_UINT16 = 65'535;
const size_t MAX_VERTICES_INT16 = 32'767;

struct ModelOptions {
    size_t VertexLimit = 0;
    float ScaleFactor = 1.0f;
    size_t BonesPerVertex = 0;
    bool PreTransformVertices = false;
    bool GenerateNormals = false;
    bool GenerateTangents = false;
    bool FlipWindingOrder = false;
    bool FlipUVs = false;
};;

struct Model {
    std::string name;
    std::vector<Object> objects;
    std::vector<Material> materials;
    std::vector<Texture> textures;
    Skeleton skeleton;
    std::map<std::string, PropertyValue> properties;

    std::string GenerateObjectName() const {
        std::string baseName = "object";
        std::set<std::string> existingNames;
        for (const auto &obj : objects)
            existingNames.insert(obj.name);
        if (!existingNames.count(baseName))
            return baseName;
        for (size_t index = 1; ; ++index) {
            std::ostringstream oss;
            oss << baseName << '.' << std::setw(3) << std::setfill('0') << index;
            std::string candidate = oss.str();
            if (!existingNames.count(candidate))
                return candidate;
        }
        return std::string();
    }

    void Clear() {
        name.clear();
        objects.clear();
        materials.clear();
        textures.clear();
        skeleton.bones.clear();
        properties.clear();
    }

    // Assumes your Bone and Skeleton types:
    // struct Bone { std::string name, parent; Matrix4x4 transform; std::map<std::string, PropertyValue> properties; };
    // struct Skeleton { std::vector<Bone> bones; std::map<std::string, PropertyValue> properties; };

    void DumpSkeletonHierarchy(const Skeleton &skeleton) {
        std::cout << std::endl << std::endl << "Skeleton Hierarchy:" << std::endl;  
        if (skeleton.bones.empty()) {
            std::cout << "(no bones)\n";
            return;
        }

        // name -> index for quick lookup
        std::unordered_map<std::string, size_t> nameToIndex;
        nameToIndex.reserve(skeleton.bones.size());
        for (size_t i = 0; i < skeleton.bones.size(); ++i)
            nameToIndex[skeleton.bones[i].name] = i;

        // build parent -> children lists (preserve insertion order)
        std::unordered_map<std::string, std::vector<std::string>> children;
        std::vector<std::string> roots;
        roots.reserve(skeleton.bones.size());
        std::unordered_set<std::string> seenRoot;

        for (const Bone &b : skeleton.bones) {
            const std::string &name = b.name;
            const std::string &parent = b.parent;
            if (!parent.empty() && nameToIndex.find(parent) != nameToIndex.end()) {
                children[parent].push_back(name);
            }
            else {
                // parent missing or empty -> treat as root (preserve first-seen order)
                if (seenRoot.insert(name).second)
                    roots.push_back(name);
            }
        }

        // If we found no roots (odd case), fallback to printing all bones in original order
        if (roots.empty()) {
            for (const Bone &b : skeleton.bones)
                std::cout << b.name << '\n';
            return;
        }

        // recursive emitter
        std::function<void(const std::string &, int)> emit = [&](const std::string &nm, int depth) {
            std::cout << std::string(depth * 2, ' ') << nm << '\n';
            auto it = children.find(nm);
            if (it == children.end()) return;
            for (const std::string &childName : it->second)
                emit(childName, depth + 1);
        };

        // emit each root
        for (const std::string &r : roots)
            emit(r, 0);
    }

    int ReadFbx(std::filesystem::path const &filename, ModelOptions const &options = ModelOptions()) {
        FbxManager *sdkManager = FbxManager::Create();
        if (!sdkManager) return ERROR_UNABLE_TO_LOAD_SCENE;
        FbxIOSettings *ios = FbxIOSettings::Create(sdkManager, IOSROOT);
        sdkManager->SetIOSettings(ios);
        FbxImporter *importer = FbxImporter::Create(sdkManager, "");
        bool importOk = importer->Initialize(filename.string().c_str(), -1, sdkManager->GetIOSettings());
        if (!importOk) {
            importer->Destroy();
            ios->Destroy();
            sdkManager->Destroy();
            return ERROR_UNABLE_TO_LOAD_SCENE;
        }
        FbxScene *scene = FbxScene::Create(sdkManager, "scene");
        if (!scene) {
            importer->Destroy();
            ios->Destroy();
            sdkManager->Destroy();
            return ERROR_UNABLE_TO_LOAD_SCENE;
        }
        if (!importer->Import(scene)) {
            importer->Destroy();
            ios->Destroy();
            sdkManager->Destroy();
            return ERROR_UNABLE_TO_LOAD_SCENE;
        }
        importer->Destroy();
        // triangulate
        FbxGeometryConverter geomConv(sdkManager);
        if (!geomConv.Triangulate(scene, true)) {
            // continue anyway; many files will triangulate successfully
        }
        if (!scene) {
            ios->Destroy();
            sdkManager->Destroy();
            return ERROR_UNABLE_TO_LOAD_COMPLETE_SCENE;
        }
        FbxNode *root = scene->GetRootNode();
        if (!root) {
            ios->Destroy();
            sdkManager->Destroy();
            return ERROR_UNABLE_TO_FIND_SCENE_ROOT_NODE;
        }
        name = filename.stem().string();
        // Helper: make unique texture names
        std::map<std::string, size_t> texKeyToIndex;
        auto MakeUniqueTexName = [&](const std::string &base) {
            static std::set<std::string> usedTexNames;
            std::string n = base;
            if (n.empty()) n = "texture";
            std::string unique = n;
            size_t i = 1;
            while (usedTexNames.count(unique)) {
                unique = n + "." + std::to_string(i++);
            }
            usedTexNames.insert(unique);
            return unique;
        };
        auto AddTextureEntry = [&](const std::string &key, const std::string &filenameStr) -> size_t {
            auto it = texKeyToIndex.find(key);
            if (it != texKeyToIndex.end()) return it->second;
            Texture t;
            t.name = MakeUniqueTexName(std::filesystem::path(filenameStr).stem().string());
            t.filename = filenameStr;
            size_t idx = textures.size();
            textures.push_back(t);
            texKeyToIndex[key] = idx;
            return idx;
        };
        // --- Materials & textures ---
        std::map<FbxSurfaceMaterial *, std::string> materialToName;
        for (int mi = 0; mi < scene->GetMaterialCount(); ++mi) {
            FbxSurfaceMaterial *fmat = scene->GetMaterial(mi);
            Material mat;
            std::string matName = fmat->GetName() ? fmat->GetName() : ("mat_" + std::to_string(mi));
            mat.name = matName;
            FbxProperty diffuseProp = fmat->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProp.IsValid()) {
                FbxDouble3 col = diffuseProp.Get<FbxDouble3>();
                mat.color.r = (uint8_t)std::clamp<int>(int(col[0] * 255.0), 0, 255);
                mat.color.g = (uint8_t)std::clamp<int>(int(col[1] * 255.0), 0, 255);
                mat.color.b = (uint8_t)std::clamp<int>(int(col[2] * 255.0), 0, 255);
                mat.color.a = 255;
            }
            // helper to fetch textures from a property (diffuse, normal, bump, etc.)
            auto TryGetTextureFromProperty = [&](FbxProperty &prop) -> std::string {
                if (!prop.IsValid()) return std::string();
                int layeredTexCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
                if (layeredTexCount > 0) {
                    // take first layered texture -> first file texture
                    FbxLayeredTexture *lTex = prop.GetSrcObject<FbxLayeredTexture>(0);
                    if (lTex && lTex->GetSrcObjectCount<FbxFileTexture>() > 0) {
                        FbxFileTexture *ft = lTex->GetSrcObject<FbxFileTexture>(0);
                        if (ft) {
                            std::string fname = ft->GetFileName() ? ft->GetFileName() : ft->GetName();
                            size_t tidx = AddTextureEntry(fname.empty() ? ft->GetName() : fname, fname);
                            return textures[tidx].name;
                        }
                    }
                }
                else {
                    int fileTexCount = prop.GetSrcObjectCount<FbxFileTexture>();
                    if (fileTexCount > 0) {
                        FbxFileTexture *ft = prop.GetSrcObject<FbxFileTexture>(0);
                        if (ft) {
                            std::string fname = ft->GetFileName() ? ft->GetFileName() : ft->GetName();
                            if (fname.empty()) {
                                std::string emb = std::string("embedded_") + (ft->GetName() ? ft->GetName() : "tex");
                                size_t tidx = AddTextureEntry(emb, emb);
                                return textures[tidx].name;
                            }
                            else {
                                size_t tidx = AddTextureEntry(fname, fname);
                                return textures[tidx].name;
                            }
                        }
                    }
                }
                return std::string();
            };
            // diffuse texture
            FbxProperty dprop = fmat->FindProperty(FbxSurfaceMaterial::sDiffuse);
            mat.texture = TryGetTextureFromProperty(dprop);
            // try normal map: common properties include NormalMap, Bump
            FbxProperty nprop = fmat->FindProperty("NormalMap");
            if (!nprop.IsValid()) nprop = fmat->FindProperty(FbxSurfaceMaterial::sBump);
            mat.normalMap = TryGetTextureFromProperty(nprop);
            materials.push_back(mat);
            materialToName[fmat] = mat.name;
        }
        // --- Collect full skeleton (clusters + explicit skeleton nodes + controlled ancestors) ---
        std::set<FbxNode *> clusterLinks;
        std::set<FbxNode *> explicitSkeletonNodes;
        // 1) collect cluster links (nodes that have skin clusters)
        for (int geomIndex = 0; geomIndex < scene->GetGeometryCount(); ++geomIndex) {
            FbxGeometry *geom = scene->GetGeometry(geomIndex);
            FbxMesh *mesh = FbxCast<FbxMesh>(geom);
            if (!mesh)
                continue;
            for (int d = 0; d < mesh->GetDeformerCount(FbxDeformer::eSkin); ++d) {
                FbxSkin *skin = static_cast<FbxSkin *>(mesh->GetDeformer(d, FbxDeformer::eSkin));
                if (!skin)
                    continue;
                for (int c = 0; c < skin->GetClusterCount(); ++c) {
                    FbxCluster *cluster = skin->GetCluster(c);
                    if (!cluster)
                        continue;
                    FbxNode *link = cluster->GetLink();
                    if (link)
                        clusterLinks.insert(link);
                }
            }
        }
        // 2) collect explicit skeleton nodes anywhere in the scene
        std::function<void(FbxNode *)> findSkeletonAttrs;
        findSkeletonAttrs = [&](FbxNode *node) {
            if (!node)
                return;
            FbxNodeAttribute *attr = node->GetNodeAttribute();
            if (attr && attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
                explicitSkeletonNodes.insert(node);
            for (int i = 0; i < node->GetChildCount(); ++i)
                findSkeletonAttrs(node->GetChild(i));
        };
        findSkeletonAttrs(root);
        // convenience helpers
        auto isSkeletonNode = [&](FbxNode *n)->bool {
            if (!n) return
                false;
            FbxNodeAttribute *a = n->GetNodeAttribute();
            return (a && a->GetAttributeType() == FbxNodeAttribute::eSkeleton);
        };
        auto hasSingleChild = [&](FbxNode *n)->bool {
            if (!n) return
                false;
            return n->GetChildCount() == 1;
        };
        // 3) decide skeleton roots & nodes to include
        std::set<FbxNode *> bonesToInclude;
        // If exporter explicitly marked joints, use their top-level skeleton nodes as roots
        std::vector<FbxNode *> skeletonRoots;
        if (!explicitSkeletonNodes.empty()) {
            // pick explicit skeleton nodes that are not children of another explicit skeleton node
            for (FbxNode *n : explicitSkeletonNodes) {
                FbxNode *p = n->GetParent();
                if (!p || explicitSkeletonNodes.find(p) == explicitSkeletonNodes.end()) {
                    skeletonRoots.push_back(n);
                }
            }
            // include full subtrees of those roots (descendants)
            std::function<void(FbxNode *)> addDescendants = [&](FbxNode *n) {
                if (!n) return;
                if (bonesToInclude.insert(n).second == false) return; // already inserted
                for (int i = 0; i < n->GetChildCount(); ++i) addDescendants(n->GetChild(i));
            };
            for (FbxNode *rootJoint : skeletonRoots) addDescendants(rootJoint);
        }
        else if (!clusterLinks.empty()) {
            // No explicit eSkeleton nodes: build minimal subtree covering clusterLinks,
            // but avoid including single-child non-skeleton wrappers (common exporter root).
            bonesToInclude.insert(clusterLinks.begin(), clusterLinks.end());
            // For each cluster link, climb ancestors but stop early on single-child non-skeleton wrappers.
            for (FbxNode *ln : clusterLinks) {
                FbxNode *cur = ln->GetParent();
                while (cur) {
                    // if already included, stop climbing
                    if (bonesToInclude.count(cur))
                        break;
                    // if cur is a skeleton-like node, include and continue climbing
                    if (isSkeletonNode(cur)) {
                        bonesToInclude.insert(cur);
                        cur = cur->GetParent();
                        continue;
                    }
                    // if cur has multiple children then it's probably a meaningful parent -> include it
                    if (cur->GetChildCount() > 1) {
                        bonesToInclude.insert(cur);
                        cur = cur->GetParent();
                        continue;
                    }
                    // cur is a non-skeleton node with single child (likely exporter wrapper) - stop here
                    // do NOT include this wrapper node to avoid RootNode becoming a root bone.
                    break;
                }
            }
            // Determine roots: nodes in bonesToInclude that have parent not in bonesToInclude
            for (FbxNode *n : bonesToInclude) {
                FbxNode *p = n->GetParent();
                if (!p || bonesToInclude.find(p) == bonesToInclude.end()) skeletonRoots.push_back(n);
            }
        }
        // 4) Build skeleton.bones in deterministic preorder traversal from skeletonRoots
        skeleton.bones.clear();
        // --- Prepare maps used during traversal ---
        std::map<FbxNode *, std::string> nodeToBoneName;
        std::map<FbxNode *, uint16_t> nodeToIndex;
        std::set<std::string> usedBoneNames;

        auto MakeUniqueBoneName = [&](const std::string &base) {
            std::string nm = base.empty() ? "bone" : base;
            std::string unique = nm;
            int i = 1;
            while (usedBoneNames.count(unique)) unique = nm + "." + std::to_string(i++);
            usedBoneNames.insert(unique);
            return unique;
        };

        // --- emitPreorder: create bone entries and record node->index as we go ---
        std::function<void(FbxNode *)> emitPreorder = [&](FbxNode *n) {
            if (!n) return;
            if (bonesToInclude.find(n) == bonesToInclude.end()) return;

            // create bone entry
            Bone b;
            std::string bname = n->GetName() ? n->GetName() : std::string();
            b.name = MakeUniqueBoneName(bname);

            // local transform
            FbxAMatrix m = n->EvaluateLocalTransform();
            b.transform.SetRow(0, FbxVector4(m.Get(0, 0), m.Get(0, 1), m.Get(0, 2), m.Get(0, 3)));
            b.transform.SetRow(1, FbxVector4(m.Get(1, 0), m.Get(1, 1), m.Get(1, 2), m.Get(1, 3)));
            b.transform.SetRow(2, FbxVector4(m.Get(2, 0), m.Get(2, 1), m.Get(2, 2), m.Get(2, 3)));
            b.transform.SetRow(3, FbxVector4(m.Get(3, 0), m.Get(3, 1), m.Get(3, 2), m.Get(3, 3)));

            // append and record index
            uint16_t thisIndex = static_cast<uint16_t>(skeleton.bones.size());
            skeleton.bones.push_back(b);
            nodeToBoneName[n] = skeleton.bones[thisIndex].name;
            nodeToIndex[n] = thisIndex;
            // recurse children (preserve FBX child order)
            for (int i = 0; i < n->GetChildCount(); ++i)
                emitPreorder(n->GetChild(i));
        };
        // --- Run traversal from determined skeletonRoots (computed earlier) ---
        // (skeletonRoots was computed by the collector logic before)
        if (!skeletonRoots.empty()) {
            for (FbxNode *rootNode : skeletonRoots) emitPreorder(rootNode);
        }
        else {
            // Fallback: if you computed roots from bonesToInclude earlier, iterate them:
            // find nodes in bonesToInclude that have parent not in bonesToInclude
            std::vector<FbxNode *> fallbackRoots;
            for (FbxNode *n : bonesToInclude) {
                FbxNode *p = n->GetParent();
                if (!p || bonesToInclude.find(p) == bonesToInclude.end()) fallbackRoots.push_back(n);
            }
            for (FbxNode *r : fallbackRoots) emitPreorder(r);
        }
        // --- AFTER traversal: set parent names using nodeToIndex/nodeToBoneName ---
        for (auto &pair : nodeToBoneName) {
            FbxNode *n = pair.first;
            uint16_t idx_ = nodeToIndex[n];
            if (n->GetParent() && nodeToBoneName.count(n->GetParent())) {
                skeleton.bones[idx_].parent = nodeToBoneName[n->GetParent()];
            }
            else {
                skeleton.bones[idx_].parent = "";
            }
        }
        // --- Build final globalBoneIndex (name -> index) ---
        std::map<std::string, uint16_t> globalBoneIndex;
        for (uint16_t i = 0; i < skeleton.bones.size(); ++i)
            globalBoneIndex[skeleton.bones[i].name] = i;

        //DumpSkeletonHierarchy(skeleton);

        // --- Node traversal and object creation ---
        std::set<std::string> usedNodeNames;
        auto MakeUniqueObjectName = [&](std::string const &objName) {
            std::string uniqueName = objName;
            if (objName.empty()) {
                size_t i = 1;
                while (usedNodeNames.count("noname_" + std::to_string(i))) ++i;
                uniqueName = "noname_" + std::to_string(i);
            }
            else if (usedNodeNames.count(objName)) {
                size_t i = 1;
                do {
                    uniqueName = objName + "." + (i < 10 ? "00" : (i < 100 ? "0" : "")) + std::to_string(i);
                    ++i;
                } while (usedNodeNames.count(uniqueName));
            }
            usedNodeNames.insert(uniqueName);
            return uniqueName;
        };

        auto CreateObject = [&](std::string const &objName, std::string const &parentName, const FbxAMatrix &transform) -> Object & {
            Object &obj = objects.emplace_back();
            obj.name = MakeUniqueObjectName(objName);
            obj.parent = parentName;
            obj.transform.SetRow(0, FbxVector4(transform.Get(0, 0), transform.Get(0, 1), transform.Get(0, 2), transform.Get(0, 3)));
            obj.transform.SetRow(1, FbxVector4(transform.Get(1, 0), transform.Get(1, 1), transform.Get(1, 2), transform.Get(1, 3)));
            obj.transform.SetRow(2, FbxVector4(transform.Get(2, 0), transform.Get(2, 1), transform.Get(2, 2), transform.Get(2, 3)));
            obj.transform.SetRow(3, FbxVector4(transform.Get(3, 0), transform.Get(3, 1), transform.Get(3, 2), transform.Get(3, 3)));
            return obj;
        };
        // helper to get mesh material name
        auto GetMaterialNameForMesh = [&](FbxMesh *fbxMesh, int materialIndex) -> std::string {
            FbxNode *owningNode = fbxMesh->GetNode();
            if (!owningNode) return std::string();
            if (materialIndex < owningNode->GetMaterialCount()) {
                FbxSurfaceMaterial *mat = owningNode->GetMaterial(materialIndex);
                if (mat && materialToName.count(mat)) return materialToName[mat];
            }
            return std::string();
        };
        // PVKey for hashing polygon-vertex attribute set (use indices where possible)
        struct PVKey {
            int cp = -1; // control point index (position)
            int normalIndex = -1;
            int tangentIndex = -1;
            int binormalIndex = -1;
            int uvIndex[8];
            int colIndex[8];

            bool operator==(PVKey const &o) const noexcept {
                if (cp != o.cp) return false;
                if (normalIndex != o.normalIndex) return false;
                if (tangentIndex != o.tangentIndex) return false;
                if (binormalIndex != o.binormalIndex) return false;
                for (int i = 0; i < 8; ++i) if (uvIndex[i] != o.uvIndex[i]) return false;
                for (int i = 0; i < 8; ++i) if (colIndex[i] != o.colIndex[i]) return false;
                return true;
            }
        };
        struct PVKeyHash {
            size_t operator()(PVKey const &k) const noexcept {
                size_t h = std::hash<int>()(k.cp + 0x9e3779b1);
                h ^= std::hash<int>()(k.normalIndex + 0x9e3779b1) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                h ^= std::hash<int>()(k.tangentIndex + 0x9e3779b1) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                h ^= std::hash<int>()(k.binormalIndex + 0x9e3779b1) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                for (int i = 0; i < 8; ++i) h ^= std::hash<int>()(k.uvIndex[i] + 1) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                for (int i = 0; i < 8; ++i) h ^= std::hash<int>()(k.colIndex[i] + 1) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                return h;
            }
        };
        // Find node by pointer search function
        std::function<void(FbxNode *, std::string const &)> NodeCallback;
        NodeCallback = [&](FbxNode *node, std::string const &parentName) {
            if (!node) return;
            std::string nodeName = node->GetName() ? node->GetName() : std::string();
            std::string newParent = parentName;

            // skip nodes that are pure bones (in globalBoneIndex) -> we do not create objects for them
            if (bonesToInclude.find(node) == bonesToInclude.end()) {
                // create object for node
                FbxAMatrix localTransform = node->EvaluateLocalTransform();
                Object &nodeObj = CreateObject(nodeName, parentName, localTransform);
                // CollectNodeProperties(node, nodeObj.properties); // if you have this helper
                newParent = nodeObj.name;

                // iterate attributes on the node and find mesh attributes
                for (int attrIndex = 0; attrIndex < node->GetNodeAttributeCount(); ++attrIndex) {
                    FbxNodeAttribute *attr = node->GetNodeAttributeByIndex(attrIndex);
                    if (!attr) continue;
                    if (attr->GetAttributeType() != FbxNodeAttribute::eMesh) continue;
                    FbxMesh *fbxMesh = FbxCast<FbxMesh>(attr);
                    if (!fbxMesh) continue;

                    // If node has one mesh attribute, use the nodeObj. If multiple mesh attributes, create per-mesh object.
                    Object *objPtr = &nodeObj;
                    if (node->GetNodeAttributeCount() > 1) {
                        std::string meshName = fbxMesh->GetName() ? fbxMesh->GetName() : (nodeObj.name + "_mesh" + std::to_string(attrIndex));
                        objPtr = &CreateObject(meshName, nodeObj.name, FbxAMatrix()); // identity transform
                    }
                    MeshData &meshData = objPtr->mesh;

                    // --- detect channels ---
                    uint32_t vertexFormat = 0;
                    vertexFormat |= V_Position;
                    bool hasNormals = fbxMesh->GetElementNormalCount() > 0;
                    if (hasNormals) vertexFormat |= V_Normal;
                    bool hasTangents = fbxMesh->GetElementTangentCount() > 0;
                    bool hasBinormals = fbxMesh->GetElementBinormalCount() > 0;
                    if (hasTangents) vertexFormat |= V_Tangent;
                    if (hasBinormals) vertexFormat |= V_Binormal;

                    int numUVChannels = fbxMesh->GetElementUVCount();
                    SetNumTexCoords(vertexFormat, (numUVChannels > 8) ? 8 : (uint8_t)numUVChannels);
                    int numColorChannels = fbxMesh->GetElementVertexColorCount();
                    SetNumColors(vertexFormat, (numColorChannels > 8) ? 8 : (uint8_t)numColorChannels);

                    // Build bone lists per control point (unchanged from previous)
                    std::vector<std::vector<std::pair<uint16_t, float>>> vertexBoneLists;
                    int controlPointCount = fbxMesh->GetControlPointsCount();
                    vertexBoneLists.resize(controlPointCount);

                    for (int defIndex = 0; defIndex < fbxMesh->GetDeformerCount(FbxDeformer::eSkin); ++defIndex) {
                        FbxSkin *skin = static_cast<FbxSkin *>(fbxMesh->GetDeformer(defIndex, FbxDeformer::eSkin));
                        if (!skin)
                            continue;
                        for (int c = 0; c < skin->GetClusterCount(); ++c) {
                            FbxCluster *cluster = skin->GetCluster(c);
                            if (!cluster)
                                continue;
                            FbxNode *link = cluster->GetLink();
                            if (!link)
                                continue;
                            std::string bname = link->GetName();
                            if (!globalBoneIndex.count(bname))
                                continue;
                            uint16_t gb = globalBoneIndex[bname];

                            // cluster provides control point indices + weights
                            const int *cpIndices = cluster->GetControlPointIndices();
                            const double *cpWeights = cluster->GetControlPointWeights();
                            int cpCount = cluster->GetControlPointIndicesCount();
                            for (int wi = 0; wi < cpCount; ++wi) {
                                int cpIndex = cpIndices[wi];
                                float weight = (float)cpWeights[wi];
                                if (cpIndex >= 0 && cpIndex < (int)vertexBoneLists.size()) {
                                    vertexBoneLists[cpIndex].emplace_back(gb, weight);
                                }
                            }
                        }
                    }
                    // Prepare element pointers (may be null)
                    FbxLayerElementNormal *nElem = (fbxMesh->GetElementNormalCount() > 0) ? fbxMesh->GetElementNormal(0) : nullptr;
                    FbxLayerElementTangent *tElem = (fbxMesh->GetElementTangentCount() > 0) ? fbxMesh->GetElementTangent(0) : nullptr;
                    FbxLayerElementBinormal *bElem = (fbxMesh->GetElementBinormalCount() > 0) ? fbxMesh->GetElementBinormal(0) : nullptr;
                    // UV and color elements arrays:
                    int nUV = NumTexCoords(vertexFormat);
                    std::vector<FbxLayerElementUV *> uvElems;
                    uvElems.resize(nUV);
                    for (int u = 0; u < nUV; ++u) {
                        if (u < fbxMesh->GetElementUVCount())
                            uvElems[u] = fbxMesh->GetElementUV(u);
                        else
                            uvElems[u] = nullptr;
                    }
                    int nCol = NumColors(vertexFormat);
                    std::vector<FbxLayerElementVertexColor *> colElems;
                    colElems.resize(nCol);
                    for (int c = 0; c < nCol; ++c) {
                        if (c < fbxMesh->GetElementVertexColorCount())
                            colElems[c] = fbxMesh->GetElementVertexColor(c);
                        else
                            colElems[c] = nullptr;
                    }
                    // --- Build per-vertex (pos+attrs) list and index buffer ---
                    std::vector<Vertex> outVertices;
                    outVertices.reserve(fbxMesh->GetControlPointsCount() * 2); // heuristic
                    std::vector<std::array<uint32_t, 3>> outTriangles;
                    outTriangles.reserve(fbxMesh->GetPolygonCount());

                    std::unordered_map<PVKey, uint32_t, PVKeyHash> keyToIndex;

                    // helper: get start index in polygon-vertex index array for polygon p
                    auto getPolyVertexBaseIndex = [&](int p) -> int { return fbxMesh->GetPolygonVertexIndex(p); };

                    unsigned int maxBonesUsed = 0;
                    // iterate polygons (triangulated earlier with FbxGeometryConverter)
                    const int polyCount = fbxMesh->GetPolygonCount();
                    for (int p = 0; p < polyCount; ++p) {
                        int polySize = fbxMesh->GetPolygonSize(p);
                        if (polySize != 3) // skip if not triangle (or implement a fan triangulation)
                            continue;
                        int basePV = getPolyVertexBaseIndex(p);

                        uint32_t triIdx[3];

                        for (int pv = 0; pv < 3; ++pv) {
                            int cp = fbxMesh->GetPolygonVertex(p, pv); // control point index
                            PVKey key;
                            key.cp = cp;
                            for (int i = 0; i < 8; ++i)
                                key.uvIndex[i] = -1;
                            for (int i = 0; i < 8; ++i)
                                key.colIndex[i] = -1;
                            key.normalIndex = -1;
                            key.tangentIndex = -1;
                            key.binormalIndex = -1;

                            // normal index
                            if (nElem) {
                                if (nElem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                    key.normalIndex = cp;
                                else { // eByPolygonVertex or others
                                    if (nElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                                        key.normalIndex = nElem->GetIndexArray().GetAt(basePV + pv);
                                    else // eDirect
                                        key.normalIndex = basePV + pv;
                                }
                            }
                            // tangent/binormal indices
                            if (tElem) {
                                if (tElem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                    key.tangentIndex = cp;
                                else
                                    key.tangentIndex = (tElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect) ? tElem->GetIndexArray().GetAt(basePV + pv) : basePV + pv;
                            }
                            if (bElem) {
                                if (bElem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                    key.binormalIndex = cp;
                                else
                                    key.binormalIndex = (bElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect) ? bElem->GetIndexArray().GetAt(basePV + pv) : basePV + pv;
                            }
                            // UV indices for each UV layer
                            for (int u = 0; u < nUV; ++u) {
                                FbxLayerElementUV *uvE = uvElems[u];
                                if (!uvE) {
                                    key.uvIndex[u] = -1;
                                    continue;
                                }
                                if (uvE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                    key.uvIndex[u] = cp;
                                else { // eByPolygonVertex
                                    if (uvE->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                                        key.uvIndex[u] = uvE->GetIndexArray().GetAt(basePV + pv);
                                    else // eDirect
                                        key.uvIndex[u] = basePV + pv;
                                }
                            }
                            // Color indices
                            for (int c = 0; c < nCol; ++c) {
                                FbxLayerElementVertexColor *colE = colElems[c];
                                if (!colE) {
                                    key.colIndex[c] = -1;
                                    continue;
                                }
                                if (colE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                    key.colIndex[c] = cp;
                                else {
                                    if (colE->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                                        key.colIndex[c] = colE->GetIndexArray().GetAt(basePV + pv);
                                    else
                                        key.colIndex[c] = basePV + pv;
                                }
                            }
                            // lookup/create vertex
                            auto it = keyToIndex.find(key);
                            if (it != keyToIndex.end()) {
                                triIdx[pv] = it->second;
                            }
                            else {
                                uint32_t newIndex = (uint32_t)outVertices.size();
                                keyToIndex.emplace(key, newIndex);
                                Vertex &V = outVertices.emplace_back();
                                FbxVector4 cpPos = fbxMesh->GetControlPoints()[cp];
                                V.pos = Vector3((float)cpPos[0], (float)cpPos[1], (float)cpPos[2]);
                                // normals
                                if (nElem) {
                                    int dIndex;
                                    if (nElem->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                        dIndex = cp;
                                    else {
                                        if (nElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                                            dIndex = nElem->GetIndexArray().GetAt(basePV + pv);
                                        else
                                            dIndex = basePV + pv;
                                    }
                                    if (dIndex >= 0 && dIndex < nElem->GetDirectArray().GetCount()) {
                                        FbxVector4 nn = nElem->GetDirectArray().GetAt(dIndex);
                                        V.normal = Vector3((float)nn[0], (float)nn[1], (float)nn[2]);
                                    }
                                    else { // fallback: SDK helper
                                        FbxVector4 nn; if (fbxMesh->GetPolygonVertexNormal(p, pv, nn)) V.normal = Vector3((float)nn[0], (float)nn[1], (float)nn[2]);
                                        else V.normal = Vector3(0, 0, 0);
                                    }
                                }
                                // tangents/binormals
                                if (tElem) {
                                    int dIndex = (tElem->GetMappingMode() == FbxGeometryElement::eByControlPoint) ? cp :
                                        (tElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect ? tElem->GetIndexArray().GetAt(basePV + pv) : basePV + pv);
                                    if (dIndex >= 0 && dIndex < tElem->GetDirectArray().GetCount()) {
                                        FbxVector4 tt = tElem->GetDirectArray().GetAt(dIndex);
                                        V.tangent = Vector3((float)tt[0], (float)tt[1], (float)tt[2]);
                                    }
                                }
                                if (bElem) {
                                    int dIndex = (bElem->GetMappingMode() == FbxGeometryElement::eByControlPoint) ? cp :
                                        (bElem->GetReferenceMode() == FbxLayerElement::eIndexToDirect ? bElem->GetIndexArray().GetAt(basePV + pv) : basePV + pv);
                                    if (dIndex >= 0 && dIndex < bElem->GetDirectArray().GetCount()) {
                                        FbxVector4 bb = bElem->GetDirectArray().GetAt(dIndex);
                                        V.binormal = Vector3((float)bb[0], (float)bb[1], (float)bb[2]);
                                    }
                                }
                                // UVs
                                for (int u = 0; u < nUV; ++u) {
                                    FbxLayerElementUV *uvE = uvElems[u];
                                    if (!uvE) continue;
                                    int dIndex;
                                    if (uvE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                        dIndex = cp;
                                    else { // by polygon-vertex
                                        int uvIdx = fbxMesh->GetTextureUVIndex(p, pv);
                                        if (uvE->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
                                            dIndex = uvE->GetIndexArray().GetAt(basePV + pv);
                                        else
                                            dIndex = uvIdx;
                                    }
                                    if (dIndex >= 0 && dIndex < uvE->GetDirectArray().GetCount()) {
                                        FbxVector2 uvt = uvE->GetDirectArray().GetAt(dIndex);
                                        V.uv[u] = Vector2((float)uvt[0], (float)uvt[1]);
                                    }
                                    else { // try SDK helper
                                        FbxVector2 uvt; bool unmapped = false;
                                        if (fbxMesh->GetPolygonVertexUV(p, pv, uvE->GetName(), uvt, unmapped) && !unmapped)
                                            V.uv[u] = Vector2((float)uvt[0], (float)uvt[1]);
                                    }
                                }
                                // Colors
                                for (int c = 0; c < nCol; ++c) {
                                    FbxLayerElementVertexColor *colE = colElems[c];
                                    if (!colE)
                                        continue;
                                    int dIndex;
                                    if (colE->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                                        dIndex = cp;
                                    else {
                                        dIndex = (colE->GetReferenceMode() == FbxLayerElement::eIndexToDirect) ? colE->GetIndexArray().GetAt(basePV + pv) : basePV + pv;
                                    }
                                    if (dIndex >= 0 && dIndex < colE->GetDirectArray().GetCount()) {
                                        FbxColor fc = colE->GetDirectArray().GetAt(dIndex);
                                        V.colors[c].Set(fc.mRed, fc.mGreen, fc.mBlue, fc.mAlpha);
                                    }
                                }
                                // Bones
                                if (!vertexBoneLists.empty() && cp >= 0 && cp < (int)vertexBoneLists.size()) {
                                    auto bl = vertexBoneLists[cp];
                                    if (!bl.empty()) {
                                        std::sort(bl.begin(), bl.end(), [](auto &a, auto &b) { return a.second > b.second; });
                                        unsigned int take = std::min<unsigned int>(8, (unsigned int)bl.size());
                                        float sum = 0.0f;
                                        for (unsigned int bi = 0; bi < take; ++bi) {
                                            V.boneIndices[bi] = bl[bi].first;
                                            V.boneWeights[bi] = bl[bi].second;
                                            sum += V.boneWeights[bi];
                                        }
                                        if (sum > 0.0f) {
                                            for (unsigned int bi = 0; bi < take; ++bi)
                                                V.boneWeights[bi] /= sum;
                                        }
                                        for (unsigned int bi = take; bi < 8; ++bi) {
                                            V.boneIndices[bi] = 0;
                                            V.boneWeights[bi] = 0.0f;
                                        }
                                        if (take > maxBonesUsed)
                                            maxBonesUsed = take;
                                    }
                                    else {
                                        for (unsigned int bi = 0; bi < 8; ++bi) {
                                            V.boneIndices[bi] = 0;
                                            V.boneWeights[bi] = 0.0f;
                                        }
                                    }
                                }
                                else {
                                    for (unsigned int bi = 0; bi < 8; ++bi) {
                                        V.boneIndices[bi] = 0;
                                        V.boneWeights[bi] = 0.0f;
                                    }
                                }
                                triIdx[pv] = newIndex;
                            }
                        }
                        outTriangles.push_back({ triIdx[0], triIdx[1], triIdx[2] });
                    }
                    meshData.vertices = std::move(outVertices);
                    meshData.triangles = std::move(outTriangles);
                    if (maxBonesUsed > 8)
                        maxBonesUsed = 8;
                    SetNumBones(vertexFormat, maxBonesUsed);
                    meshData.vertexFormat = vertexFormat;
                    for (int u = 0; u < nUV && u < 8; ++u)
                        if (uvElems[u]) meshData.uvLayerNames[u] = uvElems[u]->GetName() ? uvElems[u]->GetName() : std::string();
                    for (int c = 0; c < nCol && c < 8; ++c)
                        if (colElems[c]) meshData.colorLayerNames[c] = colElems[c]->GetName() ? colElems[c]->GetName() : std::string();
                    meshData.material = GetMaterialNameForMesh(fbxMesh, 0);
                }
            }
            for (int i = 0; i < node->GetChildCount(); ++i)
                NodeCallback(node->GetChild(i), newParent);
        };
        for (int c = 0; c < root->GetChildCount(); ++c)
            NodeCallback(root->GetChild(c), "");
        ios->Destroy();
        sdkManager->Destroy();
        return ERROR_NONE;
    }

    int WriteFbx(std::filesystem::path const &filename, bool ascii = false) {
        auto getObjectIndex = [&](std::string const &name) {
            for (size_t i = 0; i < objects.size(); i++) {
                if (objects[i].name == name)
                    return (int)i;
            }
            return -1;
        };
        auto getMaterialIndex = [&](std::string const &name) {
            for (size_t i = 0; i < materials.size(); i++) {
                if (materials[i].name == name)
                    return (int)i;
            }
            return -1;
        };
        auto getBoneIndex = [&](std::string const &name) {
            for (size_t i = 0; i < skeleton.bones.size(); i++) {
                if (skeleton.bones[i].name == name)
                    return (int)i;
            }
            return -1;
        };
        auto attachProperties = [](FbxObject *obj, std::map<std::string, PropertyValue> const &props) {
            for (auto &[key, value] : props) {
                std::visit([&](auto &&val) {
                    using T = std::decay_t<decltype(val)>;
                    FbxProperty prop;
                    if constexpr (std::is_same_v<T, int>) {
                        prop = FbxProperty::Create(obj, FbxIntDT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val);
                    }
                    else if constexpr (std::is_same_v<T, float>) {
                        prop = FbxProperty::Create(obj, FbxFloatDT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val);
                    }
                    else if constexpr (std::is_same_v<T, double>) {
                        prop = FbxProperty::Create(obj, FbxDoubleDT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val);
                    }
                    else if constexpr (std::is_same_v<T, std::string>) {
                        prop = FbxProperty::Create(obj, FbxStringDT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(FbxString(val.c_str()));
                    }
                    else if constexpr (std::is_same_v<T, Vector2>) {
                        prop = FbxProperty::Create(obj, FbxDouble2DT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val.ToFbx());
                    }
                    else if constexpr (std::is_same_v<T, Vector3>) {
                        prop = FbxProperty::Create(obj, FbxDouble3DT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val.ToFbx());
                    }
                    else if constexpr (std::is_same_v<T, Vector4>) {
                        prop = FbxProperty::Create(obj, FbxDouble4DT, key.c_str());
                        prop.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
                        prop.Set(val.ToFbx());
                    }
                }, value);
            }
        };
        FbxManager *fbxManager = FbxManager::Create();
        if (!fbxManager)
            return ERROR_UNABLE_TO_CREATE_FBX_MANAGER;
        FbxScene *fbxScene = FbxScene::Create(fbxManager, filename.stem().string().c_str());
        attachProperties(fbxScene, properties);
        // create nodes
        std::vector<FbxNode *> fbxNodes(objects.size());
        for (size_t nodeIdx = 0; nodeIdx < objects.size(); nodeIdx++) {
            Object const &obj = objects[nodeIdx];
            FbxNode *fbxNode = FbxNode::Create(fbxScene, obj.name.c_str());
            attachProperties(fbxNode, obj.properties);
            fbxNode->LclTranslation.Set(obj.transform.GetT());
            fbxNode->LclRotation.Set(obj.transform.GetR());
            fbxNode->LclScaling.Set(obj.transform.GetS());
            fbxNodes[nodeIdx] = fbxNode;
        }
        // setup nodes hierarchy
        FbxNode *root = fbxScene->GetRootNode();
        for (size_t nodeIdx = 0; nodeIdx < objects.size(); nodeIdx++) {
            int parentIndex = getObjectIndex(objects[nodeIdx].parent);
            if (parentIndex >= 0 && parentIndex < (int)objects.size())
                fbxNodes[parentIndex]->AddChild(fbxNodes[nodeIdx]);
            else
                root->AddChild(fbxNodes[nodeIdx]);
        }
        bool isSkinned = !skeleton.bones.empty();
        // create textures
        std::map<std::string, FbxFileTexture *> texByName;
        std::vector<FbxFileTexture *> fbxTextures(textures.size());
        for (size_t texIdx = 0; texIdx < textures.size(); texIdx++) {
            Texture const &tex = textures[texIdx];
            FbxFileTexture *fbxTex = FbxFileTexture::Create(fbxScene, tex.name.c_str());
            attachProperties(fbxTex, tex.properties);
            fbxTex->SetFileName(tex.filename.c_str());
            fbxTextures[texIdx] = fbxTex;
            texByName[tex.name] = fbxTex;
        }
        // create materials
        std::vector<FbxSurfaceMaterial *> fbxMaterials(materials.size());
        for (size_t matIdx = 0; matIdx < materials.size(); matIdx++) {
            const Material &mat = materials[matIdx];
            FbxSurfacePhong *fbxMat = FbxSurfacePhong::Create(fbxScene, mat.name.c_str());
            if (mat.color.r != 255 || mat.color.g != 255 || mat.color.b != 255)
                fbxMat->Diffuse.Set(FbxDouble3(double(mat.color.r) / 255.0, double(mat.color.g) / 255.0, double(mat.color.b) / 255.0));
            if (mat.color.a != 255)
                fbxMat->TransparencyFactor.Set(1.0 - (double(mat.color.a) / 255.0));
            if (!mat.texture.empty() && texByName.contains(mat.texture))
                fbxMat->Diffuse.ConnectSrcObject(texByName[mat.texture]);
            if (!mat.normalMap.empty() && texByName.contains(mat.normalMap))
                fbxMat->Bump.ConnectSrcObject(texByName[mat.normalMap]);
            attachProperties(fbxMat, mat.properties);
            fbxMaterials[matIdx] = fbxMat;
        }
        std::vector<FbxMesh *> fbxMeshes;
        // create meshes
        for (size_t nodeIdx = 0; nodeIdx < objects.size(); nodeIdx++) {
            Object const &obj = objects[nodeIdx];
            FbxMesh *fbxMesh = nullptr;
            if (!obj.mesh.triangles.empty()) {
                fbxMesh = FbxMesh::Create(fbxScene, obj.name.c_str());
                const MeshData &mesh = obj.mesh;
                if (mesh.vertices.empty())
                    fbxMesh->InitControlPoints(0);
                else {
                    int cpCount = (int)mesh.vertices.size();
                    fbxMesh->InitControlPoints(cpCount);
                    FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
                    for (int i = 0; i < cpCount; ++i) {
                        const Vertex &v = mesh.vertices[i];
                        controlPoints[i] = FbxVector4((double)v.pos.x, (double)v.pos.y, (double)v.pos.z, 1.0);
                    }
                }
                int numPolygonVertices = (int)mesh.triangles.size() * 3;
                FbxLayerElementNormal *leNormal = nullptr;
                if (mesh.vertexFormat & V_Normal) {
                    leNormal = fbxMesh->CreateElementNormal();
                    leNormal->SetMappingMode(FbxLayerElement::eByPolygonVertex);
                    leNormal->SetReferenceMode(FbxLayerElement::eDirect);
                }
                FbxLayerElementTangent *leTangent = nullptr;
                if (mesh.vertexFormat & V_Tangent) {
                    leTangent = fbxMesh->CreateElementTangent();
                    leTangent->SetMappingMode(FbxLayerElement::eByPolygonVertex);
                    leTangent->SetReferenceMode(FbxLayerElement::eDirect);
                }
                FbxLayerElementBinormal *leBinormal = nullptr;
                if (mesh.vertexFormat & V_Binormal) {
                    leBinormal = fbxMesh->CreateElementBinormal();
                    leBinormal->SetMappingMode(FbxLayerElement::eByPolygonVertex);
                    leBinormal->SetReferenceMode(FbxLayerElement::eDirect);
                }
                std::vector<FbxLayerElementUV *> uvLayers;
                for (unsigned char uvSet = 0; uvSet < NumTexCoords(mesh.vertexFormat); ++uvSet) {
                    std::string uvName = (!mesh.uvLayerNames[uvSet].empty()) ? mesh.uvLayerNames[uvSet] : "UV" + std::to_string(uvSet);
                    FbxLayerElementUV *leUV = fbxMesh->CreateElementUV(uvName.c_str());
                    leUV->SetMappingMode(FbxLayerElement::eByPolygonVertex);
                    leUV->SetReferenceMode(FbxLayerElement::eDirect);
                    uvLayers.push_back(leUV);
                }
                std::vector<FbxLayerElementVertexColor *> colorLayers;
                for (unsigned char c = 0; c < NumColors(mesh.vertexFormat); ++c) {
                    FbxLayerElementVertexColor *leVC = fbxMesh->CreateElementVertexColor();
                    if (!mesh.colorLayerNames[c].empty())
                        leVC->SetName(mesh.colorLayerNames[c].c_str());
                    leVC->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
                    leVC->SetReferenceMode(FbxGeometryElement::eDirect);
                    colorLayers.push_back(leVC);
                }
                for (size_t t = 0; t < mesh.triangles.size(); ++t) {
                    uint32_t i0 = mesh.triangles[t][0];
                    uint32_t i1 = mesh.triangles[t][1];
                    uint32_t i2 = mesh.triangles[t][2];
                    fbxMesh->BeginPolygon(-1, -1, false);
                    fbxMesh->AddPolygon((int)i0);
                    fbxMesh->AddPolygon((int)i1);
                    fbxMesh->AddPolygon((int)i2);
                    fbxMesh->EndPolygon();
                    const unsigned int cornerIdx[3] = { i0, i1, i2 };
                    for (int corner = 0; corner < 3; ++corner) {
                        const Vertex &v = mesh.vertices[cornerIdx[corner]];
                        if (leNormal)
                            leNormal->GetDirectArray().Add(FbxVector4((double)v.normal.x, (double)v.normal.y, (double)v.normal.z));
                        if (leTangent)
                            leTangent->GetDirectArray().Add(FbxVector4((double)v.tangent.x, (double)v.tangent.y, (double)v.tangent.z));
                        if (leBinormal)
                            leBinormal->GetDirectArray().Add(FbxVector4((double)v.binormal.x, (double)v.binormal.y, (double)v.binormal.z));
                        for (unsigned int uvSet = 0; uvSet < uvLayers.size(); ++uvSet) {
                            const Vector2 &uv = v.uv[uvSet];
                            uvLayers[uvSet]->GetDirectArray().Add(FbxVector2((double)uv.x, (double)uv.y));
                        }
                        for (unsigned int cIdx = 0; cIdx < colorLayers.size(); ++cIdx) {
                            const RGBA &col = v.colors[cIdx];
                            colorLayers[cIdx]->GetDirectArray().Add(FbxColor((double)col.r / 255.0,
                                (double)col.g / 255.0, (double)col.b / 255.0, (double)col.a / 255.0));
                        }
                    }
                }
                if (!uvLayers.empty()) {
                    FbxLayer *layer = fbxMesh->GetLayer(0);
                    if (!layer) {
                        fbxMesh->CreateLayer();
                        layer = fbxMesh->GetLayer(0);
                    }
                    layer->SetUVs(uvLayers[0], FbxLayerElement::eTextureDiffuse);
                }

                if (!mesh.material.empty()) {
                    int materialIndex = getMaterialIndex(mesh.material);
                    if (materialIndex != -1) {
                        fbxNodes[nodeIdx]->AddMaterial(fbxMaterials[materialIndex]);
                        FbxLayerElementMaterial *leMat = fbxMesh->CreateElementMaterial();
                        leMat->SetMappingMode(FbxLayerElement::eByPolygon);
                        leMat->SetReferenceMode(FbxLayerElement::eIndexToDirect);
                        for (size_t p = 0; p < mesh.triangles.size(); ++p)
                            leMat->GetIndexArray().Add(materialIndex);
                    }
                }
                fbxNodes[nodeIdx]->SetNodeAttribute(fbxMesh);
            }
            fbxMeshes.push_back(fbxMesh);
        }
        // skeleton
        if (isSkinned) {
            // create bones
            std::vector<FbxNode *> boneNodes;
            boneNodes.resize(skeleton.bones.size());
            for (size_t boneIdx = 0; boneIdx < skeleton.bones.size(); boneIdx++) {
                const Bone &bone = skeleton.bones[boneIdx];
                FbxSkeleton *fbxSkeleton = FbxSkeleton::Create(fbxScene, bone.name.c_str());
                attachProperties(fbxSkeleton, skeleton.properties);
                int parentIndex = getBoneIndex(bone.parent);
                fbxSkeleton->SetSkeletonType(parentIndex == -1 ? FbxSkeleton::EType::eRoot : FbxSkeleton::EType::eLimbNode);
                FbxNode *fbxNode = FbxNode::Create(fbxScene, bone.name.c_str());
                attachProperties(fbxNode, bone.properties);
                fbxNode->SetNodeAttribute(fbxSkeleton);
                fbxNode->LclTranslation.Set(bone.transform.GetT());
                fbxNode->LclRotation.Set(bone.transform.GetR());
                fbxNode->LclScaling.Set(bone.transform.GetS());
                boneNodes[boneIdx] = fbxNode;
            }
            // setup bones hierarchy
            for (size_t boneIdx = 0; boneIdx < skeleton.bones.size(); boneIdx++) {
                int parentIndex = getBoneIndex(skeleton.bones[boneIdx].parent);
                if (parentIndex == -1)
                    root->AddChild(boneNodes[boneIdx]);
                else
                    boneNodes[parentIndex]->AddChild(boneNodes[boneIdx]);
            }
            // setup skinning
            for (size_t nodeIdx = 0; nodeIdx < objects.size(); nodeIdx++) {
                if (fbxMeshes[nodeIdx]) {
                    Object const &obj = objects[nodeIdx];
                    FbxNode *meshNode = fbxNodes[nodeIdx];
                    MeshData const &mesh = obj.mesh;
                    FbxMesh *fbxMesh = fbxMeshes[nodeIdx];
                    FbxSkin *fbxSkin = FbxSkin::Create(fbxScene, "Skin");
                    std::vector<FbxCluster *> fbxClusters(skeleton.bones.size());
                    for (size_t boneIdx = 0; boneIdx < skeleton.bones.size(); boneIdx++) {
                        FbxCluster *fbxCluster = FbxCluster::Create(fbxScene, ("Cluster_" + skeleton.bones[boneIdx].name).c_str());
                        fbxCluster->SetLink(boneNodes[boneIdx]);
                        fbxCluster->SetLinkMode(FbxCluster::eNormalize);
                        fbxClusters[boneIdx] = fbxCluster;
                    }
                    for (size_t boneIdx = 0; boneIdx < skeleton.bones.size(); boneIdx++) {
                        for (size_t v = 0; v < mesh.vertices.size(); v++) {
                            for (size_t wi = 0; wi < NumBones(mesh.vertexFormat); wi++) {
                                if (mesh.vertices[v].boneIndices[wi] == boneIdx) {
                                    float w = mesh.vertices[v].boneWeights[wi];
                                    if (w > 0.0f)
                                        fbxClusters[boneIdx]->AddControlPointIndex((int)v, (double)w);
                                }
                            }
                        }
                    }
                    for (size_t boneIdx = 0; boneIdx < skeleton.bones.size(); boneIdx++) {
                        fbxClusters[boneIdx]->SetTransformMatrix(meshNode->EvaluateGlobalTransform());
                        fbxClusters[boneIdx]->SetTransformLinkMatrix(boneNodes[boneIdx]->EvaluateGlobalTransform());
                        fbxSkin->AddCluster(fbxClusters[boneIdx]);
                    }
                    fbxMesh->AddDeformer(fbxSkin);
                }
            }
        }
        // export
        FbxExporter *fbxExporter = FbxExporter::Create(fbxManager, "");
        char *pFilenameUtf8 = nullptr;
        FbxWCToUTF8(filename.c_str(), pFilenameUtf8);
        int fileFormat = -1;
        if (ascii) {
            int numFormats = fbxManager->GetIOPluginRegistry()->GetWriterFormatCount();
            for (int i = 0; i < numFormats; ++i) {
                if (fbxManager->GetIOPluginRegistry()->WriterIsFBX(i)) {
                    FbxString desc = fbxManager->GetIOPluginRegistry()->GetWriterFormatDescription(i);
                    if (desc.Find("ascii") >= 0) {
                        fileFormat = i;
                        break;
                    }
                }
            }
        }
        bool exportStatus = fbxExporter->Initialize(pFilenameUtf8, fileFormat, fbxManager->GetIOSettings());
        FbxFree(pFilenameUtf8);
        if (exportStatus)
            fbxExporter->Export(fbxScene);
        fbxExporter->Destroy();
        fbxManager->Destroy();
        return ERROR_NONE;
    }

    Model &operator+=(Model const &other) {
        auto make_unique_name = [](std::string const &base, std::unordered_set<std::string> &existing) {
            if (existing.find(base) == existing.end()) {
                existing.insert(base);
                return base;
            }
            for (int i = 1; i <= 999; ++i) {
                std::ostringstream oss;
                oss << base << '.' << std::setw(3) << std::setfill('0') << i;
                std::string candidate = oss.str();
                if (existing.find(candidate) == existing.end()) {
                    existing.insert(candidate);
                    return candidate;
                }
            }
            std::string fallback = base + "_dup";
            int suffix = 0;
            while (existing.find(fallback + std::to_string(suffix)) != existing.end()) ++suffix;
            fallback += std::to_string(suffix);
            existing.insert(fallback);
            return fallback;
        };
        auto bone_equal = [](Bone const &A, Bone const &B) {
            if (A.name != B.name)
                return false;
            if (A.parent != B.parent)
                return false;
            if (A.properties.size() != B.properties.size()) // TODO: check this
                return false;
            for (auto const &[k, v] : A.properties) {
                auto it = B.properties.find(k);
                if (it == B.properties.end())
                    return false;
            }
            return true;
        };
        auto skeleton_equal = [&bone_equal](Skeleton const &S1, Skeleton const &S2) {
            if (S1.bones.size() != S2.bones.size())
                return false;
            for (size_t i = 0; i < S1.bones.size(); ++i) {
                if (!bone_equal(S1.bones[i], S2.bones[i]))
                    return false;
            }
            return true;
        };
        std::unordered_set<std::string> existing_object_names;
        existing_object_names.reserve(objects.size() + other.objects.size());
        for (auto const &o : objects) existing_object_names.insert(o.name);
        for (auto obj : other.objects) {
            if (existing_object_names.find(obj.name) == existing_object_names.end()) {
                existing_object_names.insert(obj.name);
                objects.push_back(std::move(obj));
            }
            else {
                std::string newname = make_unique_name(obj.name, existing_object_names);
                obj.name = std::move(newname);
                objects.push_back(std::move(obj));
            }
        }
        std::unordered_set<std::string> material_names;
        material_names.reserve(materials.size() + other.materials.size());
        for (auto const &m : materials) material_names.insert(m.name);
        for (auto const &m : other.materials) {
            if (material_names.insert(m.name).second)
                materials.push_back(m);
        }
        std::unordered_set<std::string> texture_names;
        texture_names.reserve(textures.size() + other.textures.size());
        for (auto const &t : textures) texture_names.insert(t.name);
        for (auto const &t : other.textures) {
            if (texture_names.insert(t.name).second)
                textures.push_back(t);
        }
        bool this_has = !skeleton.bones.empty();
        bool other_has = !other.skeleton.bones.empty();
        if (!this_has && other_has)
            skeleton = other.skeleton;
        else if (this_has && other_has && !skeleton_equal(skeleton, other.skeleton))
            throw std::runtime_error("Cannot merge models: skeletons differ");
        for (auto const &[k, v] : other.properties) {
            if (properties.find(k) == properties.end())
                properties.emplace(k, v);
        }
        return *this;
    }
};

inline Model operator+(Model lhs, Model const &rhs) {
    lhs += rhs;
    return lhs;
}
