#include "FifamPlayerAppearance.h"
#include "Utils.h"

const UChar g14HairEditorIdToReal[98] = {
    0, 25, 1, 41, 46, 47, 92, 72, 29, 31, 43, 16, 28, 88, 90, 37, 65, 77, 82, 18, 24, 40, 2, 39, 63, 61, 64, 85, 38, 86, 21, 54, 45, 75, 78, 57, 22, 89, 70, 69, 93, 19, 23, 30, 14, 20, 62, 17, 66, 67, 94, 74, 58, 87, 83, 73, 95, 36, 32, 44, 13, 15, 11, 33, 8, 9, 10, 34, 35, 53, 7, 68, 12, 56, 55, 59, 76, 80, 84, 91, 26, 4, 71, 6, 5, 42, 27, 48, 49, 81, 52, 79, 60, 97, 96, 3, 51, 50
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

enum class HairColour07{
    Black = 1, Brown = 2, MediumBlond = 3, Blonde = 4, Red = 5
};

enum class BeardType07 {
    None = 1, Goatee = 2, Moustash = 3, Shadow = 4, FullGoatee = 5, Full = 6
};

using Face07to11Info = std::tuple<Face07, FifamSkinColor, UChar, FifamFaceVariation, FifamEyeColor>;
Vector<Face07to11Info> gFace07to11Mapping = {
{ Face07::Caucasian1,   FifamSkinColor::White,     25, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian2,   FifamSkinColor::White,     20, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian3,   FifamSkinColor::White,      3, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian4,   FifamSkinColor::White,      5, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian5,   FifamSkinColor::White,     14, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian6,   FifamSkinColor::White,     24, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian7,   FifamSkinColor::White,      8, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian8,   FifamSkinColor::White,     23, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian9,   FifamSkinColor::White,     19, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian10,  FifamSkinColor::White,     26, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian11,  FifamSkinColor::White,     21, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian12,  FifamSkinColor::White,     10, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian13,  FifamSkinColor::White,     13, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian14,  FifamSkinColor::White,    125, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian15,  FifamSkinColor::White,     22, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian16,  FifamSkinColor::White,     12, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian17,  FifamSkinColor::White,     14, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian18,  FifamSkinColor::White,    126, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian19,  FifamSkinColor::White,      2, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian20,  FifamSkinColor::White,      1, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian21,  FifamSkinColor::White,     94, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur1,  FifamSkinColor::White,      9, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur2,  FifamSkinColor::White,     16, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur3,  FifamSkinColor::White,      7, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur4,  FifamSkinColor::White,      4, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur5,  FifamSkinColor::White,     17, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur6,  FifamSkinColor::White,    102, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur7,  FifamSkinColor::White,      6, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur8,  FifamSkinColor::White,    126, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur9,  FifamSkinColor::White,    121, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur10, FifamSkinColor::White,    119, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur11, FifamSkinColor::White,    124, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur12, FifamSkinColor::White,    122, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin1,       FifamSkinColor::Latin1,   103, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin2,       FifamSkinColor::Latin1,    86, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin3,       FifamSkinColor::Latin1,    88, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin4,       FifamSkinColor::Latin1,    90, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin5,       FifamSkinColor::Latin1,    74, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin6,       FifamSkinColor::Latin1,    89, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin7,       FifamSkinColor::White,     71, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin8,       FifamSkinColor::Latin1,   110, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin9,       FifamSkinColor::Latin1,    87, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin10,      FifamSkinColor::Latin1,   111, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin11,      FifamSkinColor::Latin1,    96, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin12,      FifamSkinColor::Latin1,    73, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin13,      FifamSkinColor::Latin1,    75, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin14,      FifamSkinColor::Latin1,    79, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin15,      FifamSkinColor::Latin1,    84, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin16,      FifamSkinColor::Latin1,    70, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin17,      FifamSkinColor::Latin1,    88, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin18,      FifamSkinColor::Latin1,    80, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic1,      FifamSkinColor::Latin2,    82, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic2,      FifamSkinColor::Latin2,    78, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic3,      FifamSkinColor::Latin2,    77, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic4,      FifamSkinColor::Latin2,    68, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic5,      FifamSkinColor::Latin1,    81, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic6,      FifamSkinColor::Latin2,    93, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African1,     FifamSkinColor::African2,   6, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African2,     FifamSkinColor::African3,  52, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African3,     FifamSkinColor::African3,  49, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African4,     FifamSkinColor::African2,  51, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African5,     FifamSkinColor::African2,  55, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African6,     FifamSkinColor::African2,  57, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African7,     FifamSkinColor::African2, 131, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African8,     FifamSkinColor::African2,  68, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African9,     FifamSkinColor::African2,  56, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African10,    FifamSkinColor::African2,  50, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African11,    FifamSkinColor::African2,  66, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African12,    FifamSkinColor::African1,  54, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African13,    FifamSkinColor::African2, 134, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African14,    FifamSkinColor::African2, 114, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African15,    FifamSkinColor::African2,  59, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African16,    FifamSkinColor::African2, 135, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian1,       FifamSkinColor::Asian,     39, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian2,       FifamSkinColor::Asian,     40, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian3,       FifamSkinColor::Asian,     37, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian4,       FifamSkinColor::Asian,     47, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian5,       FifamSkinColor::Asian,     45, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian6,       FifamSkinColor::Asian,     46, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian7,       FifamSkinColor::Asian,     44, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian8,       FifamSkinColor::Asian,     42, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian9,       FifamSkinColor::Asian,     43, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian10,      FifamSkinColor::Asian,     41, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian11,      FifamSkinColor::Asian,     35, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian12,      FifamSkinColor::Asian,     32, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian13,      FifamSkinColor::Asian,     29, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian14,      FifamSkinColor::Asian,     28, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian15,      FifamSkinColor::Asian,     48, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian16,      FifamSkinColor::Asian,     30, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian17,      FifamSkinColor::Asian,     38, FifamFaceVariation::Normal, FifamEyeColor::Brown }
};

using Hair07to11Info = std::tuple<UChar, UChar>;
Vector<Hair07to11Info> gHair07to11Mapping = {
{  1,  3 },
{  2, 66 },
{  3, 59 },
{  4, 69 },
{  5, 24 },
{  6, 13 },
{  7, 82 },
{  8, 49 },
{  9, 78 },
{ 10, 71 },
{ 11,  1 },
{ 12, 20 },
{ 13, 81 },
{ 14, 35 },
{ 15,  7 },
{ 16, 11 },
{ 17, 65 },
{ 18, 73 },
{ 19, 17 },
{ 20, 16 },
{ 21, 18 },
{ 22, 14 },
{ 23, 30 },
{ 24, 29 },
{ 25, 22 },
{ 26, 36 },
{ 27, 41 },
{ 28, 52 },
{ 29, 67 },
{ 30, 76 },
{ 31, 72 },
{ 32, 62 },
{ 33, 64 },
{ 34, 70 },
{ 35, 12 },
{ 36, 15 },
{ 37, 43 },
{ 38, 34 }
};

UChar gFace11to07table[7][135] = {
//  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90  91  92  93  94  95  96  97  98  99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 
{  89, 88,  3, 50,  4, 53, 49,  7, 47, 30, 34, 34, 31, 86, 34, 48, 51,  9, 27,  2, 29, 33, 26,  6,  1, 28, 50, 34, 50, 88, 50, 34, 50, 34, 50, 47, 50, 47,  1, 58, 55, 88, 58, 88, 54, 54, 54, 87, 50,  1, 34, 88, 88, 75, 88, 88, 88, 88, 90, 88, 88, 88, 88, 34, 87, 79, 50,  1, 88, 88, 35, 88, 88,  1, 34,  1,  1,  1, 88,  1, 88,  1, 88, 50, 34, 34, 50, 88, 88, 34,  8, 88, 51, 90,  1, 88, 31,  1, 50,  1, 34, 52,  1, 34, 50,  1, 13,  4, 50,  6, 26, 88, 75, 88, 75, 88, 50, 87, 56,  1, 55, 58, 50, 57, 32, 54, 88, 50, 88, 79, 88, 55, 47, 90, 88, },
{   1, 34, 31,  8,  6, 50, 50, 50, 34, 43, 34, 88, 36,  1, 34, 43, 34,  9, 34, 88, 34, 12, 34, 27, 34, 27, 34, 76, 75, 78, 50, 74, 37, 34, 73, 47, 10, 79,  8,  9, 72, 70, 71, 69, 12, 13, 11, 77, 34, 43, 34, 88, 88, 75, 88, 88, 88, 75, 90, 88, 88, 88, 88, 34, 87, 79, 50, 88, 88, 88, 87, 12, 88, 26, 34,  1,  1,  1, 88, 43, 88,  1, 43, 43, 88, 34, 50, 88, 88, 34,  8, 88,  1, 34, 43, 88, 43,  1, 50,  1, 88, 50,  1, 88, 34,  1, 34, 43, 26,  6, 26, 88, 75, 88, 75, 88, 50, 87, 50, 43,  1, 34, 50, 43, 34,  6, 88, 50, 88, 79, 88, 55, 88, 75, 88, },
{  43, 74, 43,  8, 12, 43,  8, 10, 12, 81, 81, 38, 36, 43, 81, 43, 37,  9, 81, 75, 81, 12, 81, 10, 74, 10, 82, 43, 43, 12, 74, 12, 38, 81, 12, 38, 81, 81, 75, 38, 43, 38, 13, 38, 69, 74, 81, 75, 81, 43, 83, 81, 75, 83, 75, 75, 75, 81, 75, 81, 81, 75, 75, 81, 79, 81, 43, 43, 69, 44, 82, 12, 40, 24, 41, 12, 43, 12, 42, 46, 84, 43, 43, 43, 43, 21, 37, 45, 25, 23,  8, 82,  8, 81, 43, 39, 43, 43, 43, 38, 36, 12, 20, 81, 43, 43, 13, 43, 81, 36, 38, 38, 75, 81, 83, 75, 81, 81,  8, 43, 43, 72, 12, 43, 12,  8, 75, 81, 81, 79, 81, 83, 81, 83, 81, },
{  36, 81, 43, 46, 43, 81, 37, 43, 38, 81, 81, 37, 36, 43, 81, 43, 37, 36, 81, 37, 81, 37, 36, 36, 81, 38, 81, 37, 81, 81, 81, 64, 37, 81, 43, 38, 81, 81, 84, 37, 44, 38, 64, 81, 37, 64, 81, 83, 81, 81, 83, 81, 81, 83, 83, 81, 81, 81, 80, 81, 81, 81, 81, 81, 83, 81, 43, 83, 36, 20, 82, 81, 81, 36, 81, 82, 82, 81, 82, 43, 81, 80, 43, 43, 43, 36, 43, 82, 43, 81, 37, 81, 85, 36, 43, 81, 81, 36, 37, 36, 36, 44, 38, 81, 81, 36, 20, 44, 81, 43, 81, 38, 81, 81, 83, 83, 81, 80, 81, 43, 43, 84, 43, 43, 37, 36, 85, 36, 81, 81, 81, 83, 81, 83, 81, },
{  19, 67, 19, 81, 68, 81, 19, 19, 60, 81, 81, 19, 81, 60, 66, 60, 19, 19, 81, 60, 81, 81, 82, 14, 66, 19, 81, 19, 81, 66, 66, 66, 60, 81, 60, 60, 67, 60, 60, 60, 60, 60, 67, 66, 60, 60, 66, 81, 81, 81, 81, 81, 81, 64, 60, 81, 66, 81, 81, 81, 81, 81, 81, 81, 83, 81, 60, 67, 60, 68, 82, 66, 67, 19, 66, 82, 60, 60, 82, 14, 66, 67, 81, 63, 19, 85, 68, 82, 81, 81, 19, 82, 19, 82, 19, 19, 81, 19, 60, 19, 82, 19, 60, 81, 81, 60, 81, 81, 81, 68, 81, 60, 81, 81, 83, 83, 81, 81, 19, 19, 19, 66, 68, 66, 60, 19, 60, 66, 81, 81, 81, 81, 81, 83, 81, },
{  19, 67, 19, 66, 59, 14, 60, 19, 66, 60, 66, 60, 66, 60, 66, 60, 60, 66, 66, 66, 66, 60, 66, 14, 66, 59, 66, 60, 66, 66, 66, 66, 60, 66, 60, 66, 66, 60, 66, 66, 60, 66, 67, 66, 60, 66, 66, 66, 66, 62, 17, 66, 66, 66, 18, 61, 19, 66, 67, 66, 60, 66, 66, 66, 66, 63, 67, 60, 60, 67, 60, 66, 67, 60, 66, 60, 60, 60, 60, 14, 66, 67, 66, 60, 67, 60, 60, 66, 60, 66, 60, 60, 60, 66, 60, 66, 60, 60, 59, 60, 66, 67, 60, 66, 66, 60, 67, 19, 66, 67, 66, 60, 66, 66, 66, 66, 67, 66, 67, 67, 67, 66, 67, 67, 60, 19, 60, 66, 67, 60, 59, 66, 66, 65, 68, },
{  60, 67, 60, 66, 67, 60, 60, 19, 66, 60, 66, 60, 66, 60, 66, 66, 60, 60, 66, 66, 66, 60, 66, 14, 66, 60, 66, 60, 66, 66, 66, 66, 60, 66, 66, 66, 66, 60, 66, 66, 60, 66, 67, 66, 60, 66, 66, 66, 16, 66, 66, 15, 66, 66, 60, 66, 66, 66, 66, 66, 60, 66, 66, 66, 66, 66, 60, 67, 60, 60, 60, 66, 67, 60, 66, 60, 60, 60, 19, 14, 66, 67, 60, 60, 67, 60, 60, 66, 60, 66, 60, 66, 60, 66, 60, 66, 60, 60, 59, 60, 66, 60, 60, 66, 66, 60, 67, 19, 66, 60, 60, 60, 66, 60, 66, 66, 67, 66, 19, 67, 66, 66, 67, 60, 60, 19, 60, 66, 66, 60, 66, 66, 66, 60, 66, }
};

using Hair11to07Info = std::tuple<UChar, UChar>;
Vector<Hair07to11Info> gHair11to07table = {
{  1, 11 },
{  2,  1 },
{  3,  1 },
{  4, 13 },
{  5, 15 },
{  6, 15 },
{  7, 15 },
{  8, 15 },
{  9, 15 },
{ 10, 15 },
{ 11, 16 },
{ 12, 35 },
{ 13,  6 },
{ 14, 22 },
{ 15, 36 },
{ 16, 20 },
{ 17, 19 },
{ 18, 21 },
{ 19, 35 },
{ 20, 12 },
{ 21, 19 },
{ 22, 25 },
{ 23, 27 },
{ 24,  5 },
{ 25, 22 },
{ 26, 22 },
{ 27, 22 },
{ 28, 24 },
{ 29, 24 },
{ 30, 23 },
{ 31, 14 },
{ 32, 14 },
{ 33, 38 },
{ 34, 38 },
{ 35, 14 },
{ 36, 26 },
{ 37, 27 },
{ 38, 27 },
{ 39, 27 },
{ 40, 27 },
{ 41, 27 },
{ 42, 37 },
{ 43, 37 },
{ 44, 26 },
{ 45, 31 },
{ 46,  8 },
{ 47,  8 },
{ 48, 27 },
{ 49,  8 },
{ 50,  8 },
{ 51,  8 },
{ 52, 28 },
{ 53,  8 },
{ 54, 33 },
{ 55,  4 },
{ 56,  4 },
{ 57,  8 },
{ 58, 31 },
{ 59,  3 },
{ 60, 18 },
{ 61,  4 },
{ 62, 32 },
{ 63,  9 },
{ 64, 33 },
{ 65, 17 },
{ 66,  2 },
{ 67, 29 },
{ 68,  2 },
{ 69,  4 },
{ 70, 34 },
{ 71, 10 },
{ 72, 31 },
{ 73, 18 },
{ 74, 17 },
{ 75, 17 },
{ 76, 30 },
{ 77, 18 },
{ 78,  9 },
{ 79,  9 },
{ 80,  2 },
{ 81, 13 },
{ 82,  7 },
{ 83,  7 },
{ 84, 34 },
{ 85, 33 },
{ 86,  7 },
{ 87,  7 },
{ 88, 34 },
{ 89, 18 },
{ 90,  4 },
{ 91, 33 },
{ 92, 33 },
{ 93,  8 },
{ 94, 27 },
{ 95, 33 },
{ 96, 33 },
{ 97, 33 },
{ 98,  5 },
};

void FifamPlayerAppearance::SetFrom07AppearanceInfo(AppearanceInfo07 const &data) {
    mSkinColor = FifamSkinColor::White;
    mGenericFace = 0;
    mFaceVariation = FifamFaceVariation::Normal;
    mEyeColour = FifamEyeColor::Brown;
    mHairColor = FifamHairColor::Black;
    mHairStyle = 0;
    mSideburns = false;
    mBeardType = FifamBeardType::None;
    mBeardColor = FifamBeardColor::Black;

    // Select face type
    if (data.faceId >= Utils::ToInt(Face07::Caucasian1) && data.faceId <= Utils::ToInt(Face07::Caucasian21)) {
        for (auto &faceInfo : gFace07to11Mapping) {
            if (data.faceId == Utils::ToInt(std::get<0>(faceInfo))) {
                mSkinColor = std::get<1>(faceInfo);
                mGenericFace = std::get<2>(faceInfo) - 1;
                mFaceVariation = std::get<3>(faceInfo);
                mEyeColour = std::get<4>(faceInfo);
                break;
            }
        }
    }

    // Select hair type
    if (data.hairId >= 1 && data.hairId <= 38) {
        for (auto &hairInfo : gHair07to11Mapping) {
            if (std::get<0>(hairInfo) == data.hairId) {
                mHairStyle = g14HairEditorIdToReal[std::get<1>(hairInfo) - 1];
                break;
            }
        }
    }

    // Select hair colour
    if (data.hairColor >= 1 && data.hairColor <= 5) {
        if (data.hairColor == Utils::ToInt(HairColour07::Brown))
            mHairColor = FifamHairColor::Brown;
        else if (data.hairColor == Utils::ToInt(HairColour07::MediumBlond))
            mHairColor = FifamHairColor::MediumBlond;
        else if (data.hairColor == Utils::ToInt(HairColour07::Blonde))
            mHairColor = FifamHairColor::Blonde;
        else if (data.hairColor == Utils::ToInt(HairColour07::Red))
            mHairColor = FifamHairColor::Red;
        else
            mHairColor = FifamHairColor::Black;
    }

    // Select beard type
    if (data.beardType >= 1 && data.beardType <= 6) {
        if (data.beardType == Utils::ToInt(BeardType07::Goatee))
            mBeardType = FifamBeardType::Goatee;
        else if (data.beardType == Utils::ToInt(BeardType07::Moustash))
            mBeardType = FifamBeardType::Moustash;
        else if (data.beardType == Utils::ToInt(BeardType07::Shadow))
            mBeardType = FifamBeardType::Shadow;
        else if (data.beardType == Utils::ToInt(BeardType07::FullGoatee))
            mBeardType = FifamBeardType::FullGoatee;
        else if (data.beardType == Utils::ToInt(BeardType07::Full))
            mBeardType = FifamBeardType::Full;
        else
            mBeardType = FifamBeardType::None;
    }

    // Select beard colour
    if (mBeardType != FifamBeardType::None) {
        if (mHairColor == FifamHairColor::Blonde)
            mBeardColor = FifamBeardColor::Blonde;
        else if (mHairColor == FifamHairColor::Brown)
            mBeardColor = FifamBeardColor::Brown;
        else if (mHairColor == FifamHairColor::MediumBlond)
            mBeardColor = FifamBeardColor::MediumBlonde;
        else if (mHairColor == FifamHairColor::Red)
            mBeardColor = FifamBeardColor::Red;
        else
            mBeardColor = FifamBeardColor::Black;
    }
}

FifamPlayerAppearance::AppearanceInfo07 FifamPlayerAppearance::Get07AppearanceInfo() {
    AppearanceInfo07 result;
    result.faceId = 1;
    result.hairId = 1;
    result.hairColor = Utils::ToInt(HairColour07::Black);
    result.beardType = Utils::ToInt(BeardType07::None);

    // Get face id
    if (mGenericFace < 135)
        result.faceId = gFace11to07table[mSkinColor.ToInt()][mGenericFace];

    // Get hair id
    for (auto &hairInfo : gHair11to07table) {
        if (g14HairEditorIdToReal[std::get<0>(hairInfo) - 1] == mHairStyle) {
            result.hairId = std::get<1>(hairInfo);
            break;
        }
    }

    // Get hair colour
    if (mHairColor == FifamHairColor::Blonde)
        result.hairColor = Utils::ToInt(HairColour07::Blonde);
    else if (mHairColor == FifamHairColor::MediumBlond || mHairColor == FifamHairColor::Platinumblonde)
        result.hairColor = Utils::ToInt(HairColour07::MediumBlond);
    else if (mHairColor == FifamHairColor::Brown || mHairColor == FifamHairColor::Lightbrown || mHairColor == FifamHairColor::Darkbrown)
        result.hairColor = Utils::ToInt(HairColour07::Brown);
    else if (mHairColor == FifamHairColor::Red)
        result.hairColor = Utils::ToInt(HairColour07::Red);

    // Get beard type
    if (mBeardType == FifamBeardType::FullGoatee)
        result.beardType = Utils::ToInt(BeardType07::FullGoatee);
    if (mBeardType == FifamBeardType::Full)
        result.beardType = Utils::ToInt(BeardType07::Full);
    if (mBeardType == FifamBeardType::Moustash)
        result.beardType = Utils::ToInt(BeardType07::Moustash);
    if (mBeardType == FifamBeardType::Shadow || mBeardType == FifamBeardType::Unshaven)
        result.beardType = Utils::ToInt(BeardType07::Shadow);
    if (mBeardType == FifamBeardType::Goatee)
        result.beardType = Utils::ToInt(BeardType07::Goatee);

    return result;
}
