enum class FifamSkinColor {
    White,
    Asian,
    Latin1,
    Latin2,
    African1,
    African2,
    African3
};

enum class Face07 {
    Caucasian1 = 1,
    Caucasian2,
    Caucasian3,
    Caucasian4,
    Caucasian5,
    Caucasian6,
    Caucasian7,
    Asian1,
    Asian2,
    Asian3,
    Asian4,
    Asian5,
    Asian6,
    African1,
    African2,
    African3,
    African4,
    African5,
    African6,
    Latin1,
    Latin2,
    Latin3,
    Latin4,
    Latin5,
    Latin6,
    Caucasian8,
    Caucasian9,
    Caucasian10,
    Caucasian11,
    Caucasian12,
    Caucasian13,
    Caucasian14,
    Caucasian15,
    Caucasian16,
    Latin7,
    Latin8,
    Latin9,
    Latin10,
    Latin11,
    Latin12,
    Latin13,
    Latin14,
    Latin15,
    Latin16,
    Latin17,
    Latin18,
    EasternEur1,
    EasternEur2,
    EasternEur3,
    EasternEur4,
    EasternEur5,
    EasternEur6,
    EasternEur7,
    EasternEur8,
    EasternEur9,
    EasternEur10,
    EasternEur11,
    EasternEur12,
    African7,
    African8,
    African9,
    African10,
    African11,
    African12,
    African13,
    African14,
    African15,
    African16,
    Asian7,
    Asian8,
    Asian9,
    Asian10,
    Asian11,
    Asian12,
    Asian13,
    Asian14,
    Asian15,
    Asian16,
    Asian17,
    Arabic1,
    Arabic2,
    Arabic3,
    Arabic4,
    Arabic5,
    Arabic6,
    Caucasian17,
    Caucasian18,
    Caucasian19,
    Caucasian20,
    Caucasian21
};

using Face07to11Info = std::tuple<Face07, FifamSkinColor, unsigned char>;
std::vector<Face07to11Info> gFace07to11Mapping = {
{ Face07::Caucasian1,   FifamSkinColor::White,     25 },
{ Face07::Caucasian2,   FifamSkinColor::White,     20 },
{ Face07::Caucasian3,   FifamSkinColor::White,      3 },
{ Face07::Caucasian4,   FifamSkinColor::White,      5 },
{ Face07::Caucasian5,   FifamSkinColor::White,     14 },
{ Face07::Caucasian6,   FifamSkinColor::White,     24 },
{ Face07::Caucasian7,   FifamSkinColor::White,      8 },
{ Face07::Caucasian8,   FifamSkinColor::White,     23 },
{ Face07::Caucasian9,   FifamSkinColor::White,     19 },
{ Face07::Caucasian10,  FifamSkinColor::White,     26 },
{ Face07::Caucasian11,  FifamSkinColor::White,     21 },
{ Face07::Caucasian12,  FifamSkinColor::White,     10 },
{ Face07::Caucasian13,  FifamSkinColor::White,     13 },
{ Face07::Caucasian14,  FifamSkinColor::White,    125 },
{ Face07::Caucasian15,  FifamSkinColor::White,     22 },
{ Face07::Caucasian16,  FifamSkinColor::White,     12 },
{ Face07::Caucasian17,  FifamSkinColor::White,     14 },
{ Face07::Caucasian18,  FifamSkinColor::White,    126 },
{ Face07::Caucasian19,  FifamSkinColor::White,      2 },
{ Face07::Caucasian20,  FifamSkinColor::White,      1 },
{ Face07::Caucasian21,  FifamSkinColor::White,     94 },
{ Face07::EasternEur1,  FifamSkinColor::White,      9 },
{ Face07::EasternEur2,  FifamSkinColor::White,     16 },
{ Face07::EasternEur3,  FifamSkinColor::White,      7 },
{ Face07::EasternEur4,  FifamSkinColor::White,      4 },
{ Face07::EasternEur5,  FifamSkinColor::White,     17 },
{ Face07::EasternEur6,  FifamSkinColor::White,    102 },
{ Face07::EasternEur7,  FifamSkinColor::White,      6 },
{ Face07::EasternEur8,  FifamSkinColor::White,    126 },
{ Face07::EasternEur9,  FifamSkinColor::White,    121 },
{ Face07::EasternEur10, FifamSkinColor::White,    119 },
{ Face07::EasternEur11, FifamSkinColor::White,    124 },
{ Face07::EasternEur12, FifamSkinColor::White,    122 },
{ Face07::Latin1,       FifamSkinColor::Latin1,   103 },
{ Face07::Latin2,       FifamSkinColor::Latin1,    86 },
{ Face07::Latin3,       FifamSkinColor::Latin1,    88 },
{ Face07::Latin4,       FifamSkinColor::Latin1,    90 },
{ Face07::Latin5,       FifamSkinColor::Latin1,    74 },
{ Face07::Latin6,       FifamSkinColor::Latin1,    89 },
{ Face07::Latin7,       FifamSkinColor::White,     71 },
{ Face07::Latin8,       FifamSkinColor::Latin1,   110 },
{ Face07::Latin9,       FifamSkinColor::Latin1,    87 },
{ Face07::Latin10,      FifamSkinColor::Latin1,   111 },
{ Face07::Latin11,      FifamSkinColor::Latin1,    96 },
{ Face07::Latin12,      FifamSkinColor::Latin1,    73 },
{ Face07::Latin13,      FifamSkinColor::Latin1,    75 },
{ Face07::Latin14,      FifamSkinColor::Latin1,    79 },
{ Face07::Latin15,      FifamSkinColor::Latin1,    84 },
{ Face07::Latin16,      FifamSkinColor::Latin1,    70 },
{ Face07::Latin17,      FifamSkinColor::Latin1,    88 },
{ Face07::Latin18,      FifamSkinColor::Latin1,    80 },
{ Face07::Arabic1,      FifamSkinColor::Latin2,    82 },
{ Face07::Arabic2,      FifamSkinColor::Latin2,    78 },
{ Face07::Arabic3,      FifamSkinColor::Latin2,    77 },
{ Face07::Arabic4,      FifamSkinColor::Latin2,    68 },
{ Face07::Arabic5,      FifamSkinColor::Latin1,    81 },
{ Face07::Arabic6,      FifamSkinColor::Latin2,    93 },
{ Face07::African1,     FifamSkinColor::African2,   6 },
{ Face07::African2,     FifamSkinColor::African3,  52 },
{ Face07::African3,     FifamSkinColor::African3,  49 },
{ Face07::African4,     FifamSkinColor::African2,  51 },
{ Face07::African5,     FifamSkinColor::African2,  55 },
{ Face07::African6,     FifamSkinColor::African2,  57 },
{ Face07::African7,     FifamSkinColor::African2, 131 },
{ Face07::African8,     FifamSkinColor::African2,  68 },
{ Face07::African9,     FifamSkinColor::African2,  56 },
{ Face07::African10,    FifamSkinColor::African2,  50 },
{ Face07::African11,    FifamSkinColor::African2,  66 },
{ Face07::African12,    FifamSkinColor::African1,  54 },
{ Face07::African13,    FifamSkinColor::African2, 134 },
{ Face07::African14,    FifamSkinColor::African2, 114 },
{ Face07::African15,    FifamSkinColor::African2,  59 },
{ Face07::African16,    FifamSkinColor::African2, 135 },
{ Face07::Asian1,       FifamSkinColor::Asian,     39 },
{ Face07::Asian2,       FifamSkinColor::Asian,     40 },
{ Face07::Asian3,       FifamSkinColor::Asian,     37 },
{ Face07::Asian4,       FifamSkinColor::Asian,     47 },
{ Face07::Asian5,       FifamSkinColor::Asian,     45 },
{ Face07::Asian6,       FifamSkinColor::Asian,     46 },
{ Face07::Asian7,       FifamSkinColor::Asian,     44 },
{ Face07::Asian8,       FifamSkinColor::Asian,     42 },
{ Face07::Asian9,       FifamSkinColor::Asian,     43 },
{ Face07::Asian10,      FifamSkinColor::Asian,     41 },
{ Face07::Asian11,      FifamSkinColor::Asian,     35 },
{ Face07::Asian12,      FifamSkinColor::Asian,     32 },
{ Face07::Asian13,      FifamSkinColor::Asian,     29 },
{ Face07::Asian14,      FifamSkinColor::Asian,     28 },
{ Face07::Asian15,      FifamSkinColor::Asian,     48 },
{ Face07::Asian16,      FifamSkinColor::Asian,     30 },
{ Face07::Asian17,      FifamSkinColor::Asian,     38 }
};

unsigned char gFace11to07table[7][135] = {
{ 1,34,31,8,6,50,50,50,34,50,34,88,8,1,34,88,34,9,34,88,34,34,34,27,34,50,50,34,50,88,50,34,50,34,50,47,50,47,1,58,55,88,58,88,54,54,54,87,50,1,34,88,88,75,88,88,88,88,90,88,88,88,88,34,87,79,50,1,88,88,87,88,88,1,34,1,1,1,88,1,88,1,88,50,34,34,50,88,88,34,8,88,51,34,1,88,31,1,50,1,34,50,1,34,50,1,13,4,50,6,26,88,75,88,75,88,50,87,50,1,6,34,50,88,1,6,88,50,88,79,88,55,47,90,88 },
{ 1,34,31,8,6,50,50,50,34,43,34,88,36,1,34,43,34,9,34,88,34,12,34,27,34,27,34,34,50,88,50,34,37,34,34,47,50,47,34,7,55,88,58,88,50,54,54,87,34,43,34,88,88,75,88,88,88,75,90,88,88,88,88,34,87,79,50,88,88,88,87,12,88,26,34,1,1,1,88,43,88,1,43,43,88,34,50,88,88,34,8,88,1,34,43,88,43,1,50,1,88,50,1,88,34,1,34,43,26,6,26,88,75,88,75,88,50,87,50,43,1,34,50,43,34,6,88,50,88,79,88,55,88,75,88 },
{ 43,74,43,8,12,43,8,10,12,81,81,38,36,43,81,43,37,9,81,75,81,12,81,10,74,10,82,43,43,12,74,12,38,81,12,38,81,81,75,38,43,38,13,38,69,74,81,75,81,43,83,81,75,83,75,75,75,81,75,81,81,75,75,81,79,81,43,43,69,12,82,12,12,37,74,12,43,12,82,43,81,43,43,43,43,36,43,82,43,81,8,82,8,81,43,12,43,43,43,38,36,12,38,81,43,43,13,43,81,12,77,38,75,81,83,75,81,81,8,43,43,72,12,43,12,8,75,81,81,79,81,83,81,83,81 },
{ 36,81,43,46,43,81,37,43,38,81,81,37,36,43,81,43,37,36,81,37,81,37,36,36,81,38,81,37,81,81,81,64,37,81,43,38,81,81,84,37,44,38,64,81,37,64,81,83,81,81,83,81,81,83,83,81,81,81,80,81,81,81,81,81,83,81,43,84,36,20,82,81,81,36,81,82,37,43,82,43,81,81,43,43,43,36,43,82,43,81,37,81,37,36,43,81,81,36,37,36,36,44,38,81,81,36,20,44,81,43,81,38,81,81,83,83,81,80,81,43,43,84,43,43,37,36,85,36,81,81,81,83,81,83,81 },
{ 19,67,19,81,68,81,19,19,60,81,81,19,81,60,66,60,19,19,81,60,81,81,82,14,66,19,81,19,81,66,66,66,60,81,60,60,67,60,60,60,60,60,67,66,60,60,66,81,81,81,81,81,81,83,60,81,66,81,81,81,81,81,81,81,83,81,60,67,60,68,82,66,67,19,66,82,60,60,82,14,66,67,81,63,19,85,68,82,81,81,19,82,19,82,19,19,81,19,60,19,82,19,60,81,81,60,81,81,81,68,81,60,81,81,83,83,81,81,19,19,19,66,68,66,60,19,60,66,81,81,81,81,81,83,81 },
{ 19,67,19,66,59,60,60,19,66,60,66,60,66,60,66,60,60,66,66,66,66,60,66,14,66,59,66,60,66,66,66,66,60,66,60,66,66,60,66,66,60,66,67,66,60,66,66,66,66,66,66,66,66,66,60,66,66,66,66,66,60,66,66,66,66,66,67,66,60,67,60,66,67,60,66,60,60,60,60,14,66,67,66,60,67,60,60,66,60,66,60,60,60,66,60,66,60,60,59,60,66,67,60,66,66,60,67,19,66,67,66,60,66,60,66,66,67,66,67,67,67,66,67,67,60,19,60,66,67,60,66,66,66,60,66 },
{ 60,67,60,66,67,60,60,19,66,60,66,60,66,60,66,66,60,60,66,66,66,60,66,14,66,60,66,60,66,66,66,66,60,66,66,66,66,60,66,66,60,66,67,66,60,66,66,66,66,66,66,66,66,66,60,66,66,66,66,66,60,66,66,66,66,66,60,67,60,60,60,66,67,60,66,60,60,60,19,14,66,67,60,60,67,60,60,66,60,66,60,66,60,66,60,66,60,60,59,60,66,60,60,66,66,60,67,19,66,60,60,60,66,60,66,66,67,66,19,67,66,66,67,60,60,19,60,66,66,60,66,66,66,60,66 }
};

int main() {
    for (auto const &t : gFace07to11Mapping)
        gFace11to07table[(size_t)std::get<1>(t)][(size_t)std::get<2>(t) - 1] = (unsigned char)std::get<0>(t);
    for (unsigned char face = 0; face < 135; face++)
        printf("%3d ", face + 1);
    std::cout << std::endl;
    for (size_t skin = 0; skin < 7; skin++) {
        std::cout << "{ ";
        for (size_t face = 0; face < 135; face++)
            printf("%3d,", gFace11to07table[skin][face]);
        std::cout << " },";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}