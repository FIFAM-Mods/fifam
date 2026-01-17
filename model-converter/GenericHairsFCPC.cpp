#include "GenericHairsFCPC.h"
#include "Model.h"
#include <iostream>
#include "Magick++.h"

using namespace Magick;

#define CONVERT_MODELS
//#define CONVERT_TEXTURES

bool StartsWithIgnoreCase(std::string const &str, std::string const &prefix) {
    if (str.size() < prefix.size())
        return false;
    for (size_t i = 0; i < prefix.size(); ++i) {
        if (tolower(str[i]) != tolower(prefix[i]))
            return false;
    }
    return true;
}

struct HairColor {
    unsigned int id;
    float r, g, b;
    float brightness;
    float contrast;
    bool order;

    HairColor(unsigned int _id, float _r, float _g, float _b, float _brightness, float _contrast, bool _order) {
        id = _id;  r = _r; g = _g; b = _b; brightness = _brightness; contrast = _contrast; order = _order;
    }

    HairColor(unsigned int _id, int _r, int _g, int _b, float _brightness, float _contrast, bool _order) {
        id = _id;  r = float(_r) / 255.0f; g = float(_g) / 255.0f; b = float(_b) / 255.0f; brightness = _brightness; contrast = _contrast; order = _order;
    }
};
vector<HairColor> colors = {
    {  0,  255, 225, 180,   0.0f,   0.0f, false }, // blonde 1
    {  1,  100, 98, 96,   0.0f,   8.0f, true }, // black
    {  2,  255, 235, 200,   0.0f,   0.0f, false }, // blonde 2
    {  3,   93,  75,  55,   0.0f,   0.0f, false }, // dark brown
    {  4,  255, 240, 205,   15.0f,  0.0f, false }, // blonde 3
    {  5,  255, 200, 150,   0.0f,   0.0f, false }, // light brown
    {  6,  165, 135, 105,   0.0f,   0.0f, false }, // brown
    {  7,  255, 160, 105,   0.0f,   0.0f, false }, // red
    //{  8,  255, 255, 255,  25.0f, -25.0f, false }, // white
    {  9,  255, 255, 255,   0.0f, -20.0f, false }, // grey
    //{ 10,  130, 255, 130,   0.0f,   0.0f, false }, // green
    //{ 11,   70, 145, 255,   0.0f,   0.0f, false }, // blue
};

void ConvertGenericHairsFCPC(path const &folder, Renderer *renderer) {
    path outputDirBase = folder / "converted_hairs_fcpc";
    path outputDirHair = outputDirBase / "hair";
    path outputDirHairlod = outputDirBase / "hairlod";
    path outputDirHairTextures = outputDirBase / "hair_textures";
    create_directories(outputDirHair);
    create_directories(outputDirHairlod);
    create_directories(outputDirHairTextures);
    for (auto const &entry : directory_iterator(folder)) {
        if (entry.is_directory()) {
            auto p = entry.path();
            auto folderName = p.filename().string();
            if (folderName.starts_with("hairgen_")) {
                int hairId = stoi(folderName.substr(8));
                // hair_123_1_0_mesh.fbx - hair_mat, hairlod_mat
                // haircap_123_1_0_mesh.fbx - haircap_mat
                // hair_accessory_123_1_0_mesh.fbx - accessory_mat (+lod), accessorylod_mat, prop_mat (+lod)
                // hair_123_1_0_color.png
                // hair_123_1_0_coeff.png
                // hair_accessory_123_1_0_color.png
                // hair_accessory_123_1_0_coeff.png
                //
                // Notes:
                //
                // hair_mat can be used in haircap file instead of haircap_mat // TODO: verify this
                //
                std::cout << folderName;
                path hairMeshPath = p / ("hair_" + to_string(hairId) + "_1_0_mesh.fbx");
                path hairCapMeshPath = p / ("haircap_" + to_string(hairId) + "_1_0_mesh.fbx");
                path hairAccessoryMeshPath = p / ("hair_accessory_" + to_string(hairId) + "_1_0_mesh.fbx");
                path hairCmPath = p / ("hair_" + to_string(hairId) + "_1_0_color.png");
                path hairCoeffPath = p / ("hair_" + to_string(hairId) + "_1_0_coeff.png");
                path hairAccessoryColorTexPath = p / ("hair_accessory_" + to_string(hairId) + "_1_0_color.png");
                path hairAccessoryCoeffTexPath = p / ("hair_accessory_" + to_string(hairId) + "_1_0_coeff.png");
                path hairAccessoryAmbientTexPath = p / ("hair_accessory_" + to_string(hairId) + "_1_0_ambient.png");
                // models
                Model hairModel, haircapModel, accessoryModel;
                if (exists(hairMeshPath))
                    hairModel.ReadFbx(hairMeshPath);
                if (exists(hairCapMeshPath))
                    haircapModel.ReadFbx(hairCapMeshPath);
                if (exists(hairAccessoryMeshPath))
                    accessoryModel.ReadFbx(hairAccessoryMeshPath);
                vector<Object *> hairObjects;
                vector<Object *> hairLodObjects;
                vector<Object *> haircapObjects;
                vector<Object *> accessoryObjects;
                vector<Object *> accessoryLodObjects;
                vector<Object *> propObjects;
                auto IsValidObject = [](Object &obj) {
                    return !obj.mesh.vertices.empty() && obj.mesh.triangles.size() > 2;
                };
                for (auto &obj : hairModel.objects) {
                    if (IsValidObject(obj)) {
                        if (StartsWithIgnoreCase(obj.name, "hair_mat"))
                            hairObjects.push_back(&obj);
                        else if (StartsWithIgnoreCase(obj.name, "hairlod_mat"))
                            hairLodObjects.push_back(&obj);
                    }
                }
                for (auto &obj : haircapModel.objects) {
                    if (IsValidObject(obj)) {
                        if (StartsWithIgnoreCase(obj.name, "haircap_mat") || StartsWithIgnoreCase(obj.name, "hair_mat"))
                            haircapObjects.push_back(&obj);
                    }
                }
                for (auto &obj : accessoryModel.objects) {
                    if (IsValidObject(obj)) {
                        if (StartsWithIgnoreCase(obj.name, "accessory_mat"))
                            accessoryObjects.push_back(&obj);
                        else if (StartsWithIgnoreCase(obj.name, "accessorylod_mat"))
                            accessoryLodObjects.push_back(&obj);
                        else if (StartsWithIgnoreCase(obj.name, "prop_mat"))
                            propObjects.push_back(&obj);
                    }
                }
                auto SortByVertexCount = [](Object *a, Object *b) { return a->mesh.vertices.size() > b->mesh.vertices.size(); };
                std::sort(hairObjects.begin(), hairObjects.end(), SortByVertexCount);
                std::sort(hairLodObjects.begin(), hairLodObjects.end(), SortByVertexCount);
                std::sort(haircapObjects.begin(), haircapObjects.end(), SortByVertexCount);
                std::sort(accessoryObjects.begin(), accessoryObjects.end(), SortByVertexCount);
                std::sort(accessoryLodObjects.begin(), accessoryLodObjects.end(), SortByVertexCount);
                std::sort(propObjects.begin(), propObjects.end(), SortByVertexCount);
                Object *hair = nullptr, *hairLod = nullptr, *haircap = nullptr, *accessory = nullptr, *accessoryLod = nullptr;
                if (!hairObjects.empty())
                    hair = hairObjects[0];
                if (!hairLodObjects.empty())
                    hairLod = hairLodObjects[0];
                else if (hairObjects.size() > 1)
                    hairLod = hairObjects[hairObjects.size() - 1];
                if (!haircapObjects.empty())
                    haircap = haircapObjects[0];
                if (!accessoryObjects.empty())
                    accessory = accessoryObjects[0];
                else if (!propObjects.empty())
                    accessory = propObjects[0];
                if (!accessoryLodObjects.empty())
                    accessoryLod = accessoryLodObjects[0];
                else if (accessoryObjects.size() > 1)
                    accessoryLod = accessoryObjects[accessoryObjects.size() - 1];
                else if (propObjects.size() > 1)
                    accessoryLod = propObjects[propObjects.size() - 1];
                bool hasHair = hair || haircap;
                bool hasAccessory = accessory;
                bool combineTex = hasHair && hasAccessory;
#ifdef CONVERT_MODELS
                Model outputModelHair, outputModelHairlod;
                Skeleton *skeleton = nullptr;
                if (!hairModel.skeleton.bones.empty())
                    skeleton = &hairModel.skeleton;
                else if (!haircapModel.skeleton.bones.empty())
                    skeleton = &haircapModel.skeleton;
                else if (!accessoryModel.skeleton.bones.empty())
                    skeleton = &accessoryModel.skeleton;
                outputModelHair.skeleton = skeleton ? *skeleton : Skeleton();
                outputModelHairlod.skeleton = skeleton ? *skeleton : Skeleton();
                auto MoveUVs = [](Object &obj, bool right) {
                    unsigned int numTexCoords = NumTexCoords(obj.mesh.vertexFormat);
                    for (auto &v : obj.mesh.vertices) {
                        for (unsigned char uvSet = 0; uvSet < numTexCoords; ++uvSet) {
                            if (right)
                                v.uv[uvSet].x = v.uv[uvSet].x * 0.5f + 0.5f;
                            else
                                v.uv[uvSet].x = v.uv[uvSet].x * 0.5f;
                        }
                    }
                };
                if (haircap) {
                    auto &obj = outputModelHair.objects.emplace_back(*haircap);
                    obj.name = "sortgroup0";
                    obj.mesh.material = outputModelHair.materials.emplace_back("haircap").name;
                    if (combineTex)
                        MoveUVs(obj, false);
                }
                if (hair) {
                    auto &obj = outputModelHair.objects.emplace_back(*hair);
                    obj.name = "sortgroup1";
                    obj.mesh.material = outputModelHair.materials.emplace_back("hair").name;
                    if (combineTex)
                        MoveUVs(obj, false);
                }
                if (accessory) {
                    auto &obj = outputModelHair.objects.emplace_back(*accessory);
                    obj.name = "sortgroup2";
                    obj.mesh.material = outputModelHair.materials.emplace_back("accessory").name;
                    if (combineTex)
                        MoveUVs(obj, true);
                }
                if (hairLod) {
                    auto &obj = outputModelHairlod.objects.emplace_back(*hairLod);
                    obj.name = "sortgroup0";
                    obj.mesh.material = outputModelHairlod.materials.emplace_back("hairlod").name;
                    if (combineTex)
                        MoveUVs(obj, false);
                }
                if (accessoryLod) {
                    auto &obj = outputModelHairlod.objects.emplace_back(*accessoryLod);
                    obj.name = "sortgroup1";
                    obj.mesh.material = outputModelHairlod.materials.emplace_back("accessory").name;
                    if (combineTex)
                        MoveUVs(obj, true);
                }
                auto MeshCallback = [&](MeshData &mesh) {
                    SetNumColors(mesh.vertexFormat, 0);
                    if (NumTexCoords(mesh.vertexFormat) == 1) {
                        for (auto &v : mesh.vertices)
                            v.uv[1] = v.uv[0];
                    }
                    SetNumTexCoords(mesh.vertexFormat, 2);
                    for (auto &v : mesh.vertices)
                        v.pos *= 10000.0f;
                };
                for (auto &obj : outputModelHair.objects)
                    MeshCallback(obj.mesh);
                for (auto &obj : outputModelHairlod.objects)
                    MeshCallback(obj.mesh);
                if (!outputModelHair.objects.empty())
                    outputModelHair.WriteFbx(outputDirHair / ("m432__" + to_string(hairId) + ".fbx"));
                if (!outputModelHairlod.objects.empty())
                    outputModelHairlod.WriteFbx(outputDirHairlod / ("m728__" + to_string(-hairId) + ".fbx"));
#endif
                // textures
#ifdef CONVERT_TEXTURES
                auto ResizeImage = [](Magick::Image &image, int w, int h) {
                    if (image.columns() != w || image.rows() != h) {
                        Magick::Geometry geom(w, h);
                        geom.aspect(true);
                        image.filterType(MagickCore::LanczosFilter);
                        image.resize(geom);
                    }
                };
                auto RemoveAlpha = [](Magick::Image &image) {
                    image.type(MagickCore::ImageType::TrueColorType);
                    image.alpha(false);
                };
                const unsigned int imageRes = 512;
                unsigned int imageWidth = combineTex ? (imageRes / 2) : imageRes;
                unsigned int accessoryX = combineTex ? (imageRes / 2) : 0;
                Image outHairCmb(Magick::Geometry(imageRes, imageRes), Magick::Color(0, 0, 0));
                Image outHairCm(Magick::Geometry(imageRes, imageRes), Magick::Color(0, 0, 0));
                outHairCmb.type(MagickCore::ImageType::TrueColorType);
                outHairCm.type(MagickCore::ImageType::TrueColorType);
                if (hasAccessory) {
                    Image white(Magick::Geometry(imageWidth, imageRes), Magick::Color(255, 255, 255));
                    outHairCmb.composite(white, accessoryX, 0, MagickCore::CompositeOperator::OverCompositeOp);
                    if (exists(hairAccessoryColorTexPath)) {
                        Image accessoryColor(hairAccessoryColorTexPath.string());
                        RemoveAlpha(accessoryColor);
                        ResizeImage(accessoryColor, imageWidth, imageRes);
                        if (exists(hairAccessoryAmbientTexPath)) {
                            Image accessoryAmbient(hairAccessoryAmbientTexPath.string());
                            RemoveAlpha(accessoryAmbient);
                            ResizeImage(accessoryAmbient, imageWidth, imageRes);
                            auto accssoryR = accessoryAmbient.separate(MagickCore::RedChannel);
                            accessoryColor.composite(accssoryR, 0, 0, MagickCore::CompositeOperator::MultiplyCompositeOp);
                        }
                        outHairCm.composite(accessoryColor, accessoryX, 0, MagickCore::CompositeOperator::OverCompositeOp);
                    }
                }
                Image hairCoeffB;
                bool hasHairCoeffB = false;
                Image hairCmTexture;
                bool hasHairCm = false;
                if (hasHair) {
                    if (exists(hairCoeffPath)) {
                        Image hairCoeffTexture(hairCoeffPath.string());
                        RemoveAlpha(hairCoeffTexture);
                        ResizeImage(hairCoeffTexture, imageWidth, imageRes);
                        auto imgR = hairCoeffTexture.separate(MagickCore::RedChannel);
                        auto imgG = hairCoeffTexture.separate(MagickCore::GreenChannel);
                        hairCoeffB = hairCoeffTexture.separate(MagickCore::BlueChannel);
                        hasHairCoeffB = true;
                        outHairCmb.composite(imgG, 0, 0, MagickCore::CompositeOperator::CopyRedCompositeOp);
                        outHairCmb.composite(imgR, 0, 0, MagickCore::CompositeOperator::CopyGreenCompositeOp);
                        outHairCmb.composite(imgR, 0, 0, MagickCore::CompositeOperator::CopyBlueCompositeOp);
                    }
                    if (exists(hairCmPath)) {
                        hairCmTexture = Image(hairCmPath.string());
                        RemoveAlpha(hairCmTexture);
                        ResizeImage(hairCmTexture, imageWidth, imageRes);
                        hasHairCm = true;
                    }
                }
                outHairCmb.write((outputDirHairTextures / ("hair_cmb_-" + to_string(hairId) + ".tga")).string());
                for (auto const &clr : colors) {
                    if (hasHairCm) {
                        auto hairColor = hairCmTexture.separate(MagickCore::GreenChannel);
                        RemoveAlpha(hairColor);
                        if (clr.order && (clr.brightness != 0.0f || clr.contrast != 0.0f)) {
                            hairColor.sigmoidalContrast(true, 2);
                            hairColor.brightnessContrast(clr.brightness);
                        }
                        if (clr.r != 1.0f)
                            hairColor.evaluate(MagickCore::RedChannel, MagickCore::MultiplyEvaluateOperator, clr.r);
                        if (clr.g != 1.0f)
                            hairColor.evaluate(MagickCore::GreenChannel, MagickCore::MultiplyEvaluateOperator, clr.g);
                        if (clr.b != 1.0f)
                            hairColor.evaluate(MagickCore::BlueChannel, MagickCore::MultiplyEvaluateOperator, clr.b);
                        if (clr.order && (clr.brightness != 0.0f || clr.contrast != 0.0f)) {
                            hairColor.sigmoidalContrast(true, clr.contrast);
                            hairColor.brightnessContrast(clr.brightness);
                        }
                        else if (!clr.order && (clr.brightness != 0.0f || clr.contrast != 0.0f)) {
                            hairColor.brightnessContrast(clr.brightness, clr.contrast);
                        }
                        if (hasHairCoeffB) {
                            Image handBandImg(hairCmTexture);
                            handBandImg.composite(hairCoeffB, 0, 0, MagickCore::CompositeOperator::CopyAlphaCompositeOp);
                            hairColor.composite(handBandImg, 0, 0, MagickCore::CompositeOperator::OverCompositeOp);
                        }
                        outHairCm.composite(hairColor, 0, 0, MagickCore::CompositeOperator::OverCompositeOp);
                    }
                    outHairCm.write((outputDirHairTextures / ("hair_cm_-" + to_string(hairId) + "_" + to_string(clr.id) + ".tga")).string());
                }
#endif
                std::cout << std::endl;
            }
        }
    }
}
