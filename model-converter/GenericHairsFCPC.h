#pragma once
#include <filesystem>
#include "Renderer.h"

using namespace std;
using namespace std::filesystem;

void ConvertGenericHairsFCPC(path const &folder, Renderer *renderer = nullptr);
