#pragma once
#include "FifamDbEnvironment.h"

using namespace std;

class FilesToFolders {
public:
    FilesToFolders() {
        for (auto const& i : directory_iterator(current_path())) {
            auto p = i.path();
            if (is_regular_file(p) && p.extension() == ".gltf") {
                wstring modelname = p.stem().c_str();
                if (Utils::EndsWith(modelname, L"_m"))
                    modelname = modelname.substr(0, modelname.size() - 2);
                create_directory(modelname);
                rename(p, path(modelname) / (modelname + L".gltf"));
                path facepath = L"tp01@" + modelname + L"_f.png";
                if (exists(facepath))
                    rename(facepath, path(modelname) / "tp01.png");
                path hairpath = L"tp02@" + modelname + L"_h.png";
                if (exists(hairpath))
                    rename(hairpath, path(modelname) / "tp02.png");
            }
        }
    }
};
