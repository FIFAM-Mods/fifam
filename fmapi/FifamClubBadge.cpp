#include "FifamClubBadge.h"
#include "FifamClubTeamColor.h"
#include "FifamClub.h"

void FifamClubBadge::SetColor(FifamClubBadgeShape shape, UChar shapeVariation, UChar pattern, Bool inverted, ColorPair const &color) {
    mType = FifamClubBadgeType::Color;
    mShape = shape;
    mShapeVariation = shapeVariation;
    mPattern = pattern;
    mInverted = inverted;
    mColor = color;
    Validate();
}

void FifamClubBadge::SetColor(FifamClubBadgeShape shape, UChar shapeVariation, UChar pattern, Bool inverted) {
    mType = FifamClubBadgeType::Color;
    mShape = shape;
    mShapeVariation = shapeVariation;
    mPattern = pattern;
    mInverted = inverted;
    Validate();
}

void FifamClubBadge::SetColorDefault(ColorPair const &color) {
    SetColor(FifamClubBadgeShape::Crest, 1, 1, true, color);
}

void FifamClubBadge::SetClub(String const &clubFileName) {
    mType = FifamClubBadgeType::Club;
    mClubFileName = clubFileName;
}

void FifamClubBadge::SetNoBadge() {
    mType = FifamClubBadgeType::NoBadge;
}

void FifamClubBadge::SetEALogo() {
    mType = FifamClubBadgeType::EALogo;
}

void FifamClubBadge::Validate() {
    if (mType == FifamClubBadgeType::Color) {
        if (mShape == FifamClubBadgeShape::Ball) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Rectangle) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Flag) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Pill) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Triangle) {
            if (mShapeVariation != 1 && mShapeVariation != 3)
                mShapeVariation = 1;
            if (mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Hexagon) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern != 1 && mPattern != 2)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Oval) {
            if (mShapeVariation != 1 && mShapeVariation != 2)
                mShapeVariation = 1;
            if (mShapeVariation == 1 && mPattern != 1 && mPattern != 2)
                mPattern = 1;
            if (mShapeVariation == 2 && mPattern != 1)
                mPattern = 1;
            if (mPattern == 1 && mInverted)
                mInverted = false;
        }
        else if (mShape == FifamClubBadgeShape::Diamond) {
            if (mShapeVariation != 1 && mShapeVariation != 2)
                mShapeVariation = 1;
            if (mShapeVariation == 1 && mPattern != 1 && mPattern != 2)
                mPattern = 1;
            if (mShapeVariation == 2 && mPattern != 1)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Square) {
            if (mShapeVariation != 1 && mShapeVariation != 2)
                mShapeVariation = 1;
            if (mShapeVariation == 1 && mPattern != 1 && mPattern != 2)
                mPattern = 1;
            if (mShapeVariation == 2 && mPattern != 1 && mPattern != 3)
                mPattern = 1;
        }
        else if (mShape == FifamClubBadgeShape::Round) {
            if (mShapeVariation != 1)
                mShapeVariation = 1;
            if (mPattern < 1 || mPattern == 6 || mPattern > 7)
                mPattern = 1;
            if (mInverted && (mPattern == 1 || mPattern == 2))
                mInverted = false;
        }
        else if (mShape == FifamClubBadgeShape::Crest) {
            if (mShapeVariation < 1 || mShapeVariation > 18)
                mShapeVariation = 1;
            if (mShapeVariation == 1 || mShapeVariation == 2 || mShapeVariation == 3 || mShapeVariation == 6
                || mShapeVariation == 7 || mShapeVariation == 8 || mShapeVariation == 9 || mShapeVariation == 10
                || mShapeVariation == 11 || mShapeVariation == 12 || mShapeVariation == 13 || mShapeVariation == 15
                || mShapeVariation == 16 || mShapeVariation == 17 || mShapeVariation == 18)
            {
                if (mPattern != 1)
                    mPattern = 1;
            }
            else if (mShapeVariation == 4) {
                if (mPattern < 1 || mPattern == 3 || mPattern > 5)
                    mPattern = 1;
            }
            else if (mShapeVariation == 5) {
                if (mPattern < 1 || mPattern > 4)
                    mPattern = 1;
            }
            else if (mShapeVariation == 14) {
                if (mPattern != 1 && mPattern != 4)
                    mPattern = 1;
            }
        }
    }
}

String FifamClubBadge::ToStr() {
    String result = mType.ToStr();
    if (mType == FifamClubBadgeType::Club)
        result += L" \"" + mClubFileName + L"\"";
    else if (mType == FifamClubBadgeType::Color) {
        FifamClubTeamColor teamColor;
        teamColor.SetFromInt(mColor.FindIndexInTable(FifamClub::mTeamColorsTable));
        result += L" " + mShape.ToStr() + L" " + Utils::Format(L"%02d-%02d", mShapeVariation, mPattern) + L" "
            + mColor.ToStr() + L" (" + teamColor.ToStr() + L")";
        if (mInverted)
            result += L" inverted";
    }
    return result;
}

String FifamClubBadge::GetBadgePath(UInt gameId) {
    if (mType == FifamClubBadgeType::NoBadge)
        return L"";
    else if (mType == FifamClubBadgeType::EALogo)
        return L"generic\\Badge%d\\generic256.tga";
    else if (mType == FifamClubBadgeType::Club)
        return mClubFileName;
    else {
        String line;
        if (gameId <= 9)
            line += L"Color\\Badge%d\\";
        else
            line += L"generic\\256x256\\";
        if (gameId >= 12) {
            FifamClubTeamColor teamColor;
            teamColor.SetFromInt(mColor.FindIndexInTable(FifamClub::mTeamColorsTable));
            line += mShape.ToStr() + L"_" + Utils::Format(L"%02d", mShapeVariation);
            if (mShape != FifamClubBadgeShape::Ball && (mShape != FifamClubBadgeShape::Crest || mShapeVariation > 3))
                line += L"_" + Utils::Format(L"%02d", mPattern);
            if (mInverted)
                line += L"i";
            line += L"_" + teamColor.GetShortName();
        }
        else {
            FifamClubTeamColor teamColor;
            teamColor.SetFromInt(mColor.FindIndexInTable(FifamClub::mTeamColorsTable, 0, 23));
            line += teamColor.GetShortName();
            if (gameId >= 8) {
                if (mShape == FifamClubBadgeShape::Crest && (mShapeVariation >= 1 && mShapeVariation <= 3)) {
                    if (mShapeVariation != 1)
                        line += L"_" + Utils::Format(L"%02d", mShapeVariation - 1);
                }
                else if (gameId >= 10) {
                    if (mShape == FifamClubBadgeShape::Triangle && mShapeVariation == 1) {
                        line += L"_18";
                        if (!mInverted)
                            line += L"a";
                    }
                    else if (mShape == FifamClubBadgeShape::Rectangle) {
                        line += L"_10";
                        if (!mInverted)
                            line += L"a";
                    }
                    else if (mShape == FifamClubBadgeShape::Oval) {
                        if (mShapeVariation == 1 && mPattern == 1)
                            line += L"_03";
                        else if (mShapeVariation == 1 && mPattern == 2) {
                            line += L"_26";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 3)
                            line += L"_04";
                    }
                    else if (mShape == FifamClubBadgeShape::Flag) {
                        line += L"_24";
                        if (!mInverted)
                            line += L"a";
                    }
                    else if (mShape == FifamClubBadgeShape::Diamond) {
                        if (mShapeVariation == 1 && mPattern == 1) {
                            line += L"_09";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 1 && mPattern == 2) {
                            line += L"_09";
                            if (mInverted)
                                line += L"b";
                            else
                                line += L"c";
                        }
                    }
                    else if (mShape == FifamClubBadgeShape::Square && mShapeVariation == 2 && mPattern == 1) {
                        line += L"_08";
                        if (!mInverted)
                            line += L"a";
                    }
                    else if (mShape == FifamClubBadgeShape::Round) {
                        if (mPattern == 1)
                            line += L"_05";
                        else if (mPattern == 2)
                            line += L"_05a";
                        else if (mPattern == 3) {
                            line += L"_06";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mPattern == 4) {
                            line += L"_06";
                            if (mInverted)
                                line += L"b";
                            else
                                line += L"c";
                        }
                        else if (mPattern == 5) {
                            line += L"_06";
                            if (mInverted)
                                line += L"d";
                            else
                                line += L"e";
                        }
                        else if (mPattern == 7) {
                            line += L"_07";
                            if (!mInverted)
                                line += L"a";
                        }
                    }
                    else if (mShape == FifamClubBadgeShape::Crest) {
                        if (mShapeVariation == 4 && mPattern == 1) {
                            line += L"_11";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 4 && mPattern == 2) {
                            line += L"_14";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 4 && mPattern == 4) {
                            line += L"_17";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 4 && mPattern == 5) {
                            line += L"_15";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 5 && mPattern == 1) {
                            line += L"_16";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 5 && mPattern == 2) {
                            line += L"_17";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 5 && mPattern == 3) {
                            line += L"_17";
                            if (mInverted)
                                line += L"d";
                            else
                                line += L"b";
                        }
                        else if (mShapeVariation == 5 && mPattern == 4) {
                            line += L"_17";
                            if (mInverted)
                                line += L"e";
                            else
                                line += L"c";
                        }
                        else if (mShapeVariation == 6 && mPattern == 1) {
                            line += L"_13";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 7 && mPattern == 1) {
                            line += L"_20";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 8 && mPattern == 1) {
                            line += L"_21";
                            if (mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 9 && mPattern == 1) {
                            line += L"_12";
                            if (mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 10 && mPattern == 1) {
                            line += L"_12";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 11 && mPattern == 1) {
                            line += L"_25";
                            if (mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 12 && mPattern == 1) {
                            line += L"_23";
                            if (!mInverted)
                                line += L"a";
                        }
                        else if (mShapeVariation == 13 && mPattern == 1) {
                            line += L"_22";
                            if (mInverted)
                                line += L"a";
                        }
                    }
                }
            }
        }
        line += L".tga";
        return line;
    }
}

void FifamClubBadge::SetBadgePath(String const &str) {
    if (str.empty()) {
        SetNoBadge();
        return;
    }
    String line = Utils::ToLower(str);
    String badgeName;
    if (Utils::StartsWith(line, L"color\\badge%d\\"))
        badgeName = line.substr(14);
    else if (Utils::StartsWith(line, L"generic\\badge%d\\"))
        badgeName = line.substr(16);
    else if (Utils::StartsWith(line, L"generic\\256x256\\"))
        badgeName = line.substr(16);
    else {
        SetClub(str);
        return;
    }
    auto dotPos = badgeName.find(L'.');
    if (dotPos != String::npos)
        badgeName = badgeName.substr(0, dotPos);
    if (badgeName == L"generic256") {
        SetEALogo();
        return;
    }
    SetColor(FifamClubBadgeShape::Crest, 1, 1, true, { {255, 255, 255}, { 0, 0, 0 }});
    static const WideChar *shapeNames[] = { L"ball", L"crest", L"diamond", L"flag", L"hexagon", L"oval", L"pill", L"rectangle", L"round", L"square", L"triangle" };
    Bool shapeDefined = false;
    for (UInt i = 0; i < 11; i++) {
        String compStr = String(shapeNames[i]) + L"_";
        if (Utils::StartsWith(badgeName, compStr)) {
            mShape.SetFromInt(i);
            shapeDefined = true;
            badgeName = badgeName.substr(compStr.length());
            if (badgeName.size() < 2)
                return;
            String numbVar = badgeName.substr(0, 2);
            if (!Utils::IsNumber(numbVar))
                return;
            mShapeVariation = Utils::ToNumber(numbVar);
            badgeName = badgeName.substr(2);
            if (badgeName.size() < 1)
                return;
            if (mShape != FifamClubBadgeShape::Ball && (mShape != FifamClubBadgeShape::Crest || (mShapeVariation < 1 || mShapeVariation > 3))) {
                if (badgeName[0] != L'_')
                    return;
                badgeName = badgeName.substr(1);
                if (badgeName.size() < 2)
                    return;
                String numbPat = badgeName.substr(0, 2);
                if (!Utils::IsNumber(numbPat))
                    return;
                mPattern = Utils::ToNumber(numbPat);
                badgeName = badgeName.substr(2);
                if (badgeName.size() < 1)
                    return;
            }
            if (badgeName[0] == L'i') {
                mInverted = true;
                badgeName = badgeName.substr(1);
                if (badgeName.size() < 1)
                    return;
            }
            else
                mInverted = false;
            if (badgeName[0] != L'_')
                return;
            badgeName = badgeName.substr(1);
            break;
        }
    }
    Bool colorSet = false;
    for (UInt i = 0; i < 28; i++) {
        FifamClubTeamColor tmpClr;
        tmpClr.SetFromInt(i);
        String clrName = tmpClr.GetShortName();
        if (Utils::StartsWith(badgeName, clrName)) {
            mColor.SetFromTable(FifamClub::mTeamColorsTable, i);
            colorSet = true;
            badgeName = badgeName.substr(clrName.length());
            break;
        }
    }
    if (colorSet && !shapeDefined && !badgeName.empty() && badgeName[0] == L'_') {
        badgeName = badgeName.substr(1);
        if (badgeName.size() < 2)
            return;
        
        String strVar = badgeName.substr(0, 2);
        if (!Utils::IsNumber(strVar))
            return;
        UInt var = Utils::ToNumber(strVar);
        badgeName = badgeName.substr(2);
        WideChar pat = L'\0';
        if (badgeName.length() == 1)
            pat = badgeName[0];
        if (var == 1)
            SetColor(FifamClubBadgeShape::Crest, 2, 1, false);
        else if (var == 2)
            SetColor(FifamClubBadgeShape::Crest, 3, 1, true);
        else if (var == 3)
            SetColor(FifamClubBadgeShape::Oval, 1, 1, false);
        else if (var == 4)
            SetColor(FifamClubBadgeShape::Oval, 2, 1, false);
        else if (var == 5) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Round, 1, 2, false);
            else
                SetColor(FifamClubBadgeShape::Round, 1, 1, false);
        }
        else if (var == 6) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Round, 1, 3, false);
            else if (pat == L'b')
                SetColor(FifamClubBadgeShape::Round, 1, 4, true);
            else if (pat == L'c')
                SetColor(FifamClubBadgeShape::Round, 1, 4, false);
            else if (pat == L'd')
                SetColor(FifamClubBadgeShape::Round, 1, 5, true);
            else if (pat == L'e')
                SetColor(FifamClubBadgeShape::Round, 1, 5, false);
            else
                SetColor(FifamClubBadgeShape::Round, 1, 3, true);
        }
        else if (var == 7) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Round, 1, 7, false);
            else
                SetColor(FifamClubBadgeShape::Round, 1, 7, true);
        }
        else if (var == 8) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Square, 2, 1, false);
            else
                SetColor(FifamClubBadgeShape::Square, 2, 1, true);
        }
        else if (var == 9) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Diamond, 1, 1, false);
            else if (pat == L'b')
                SetColor(FifamClubBadgeShape::Diamond, 1, 2, true);
            else if (pat == L'c')
                SetColor(FifamClubBadgeShape::Diamond, 1, 2, false);
            else
                SetColor(FifamClubBadgeShape::Diamond, 1, 1, true);
        }
        else if (var == 10) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Rectangle, 1, 1, false);
            else
                SetColor(FifamClubBadgeShape::Rectangle, 1, 1, true);
        }
        else if (var == 11) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 4, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 4, 1, true);
        }
        else if (var == 12) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 9, 1, true);
            else
                SetColor(FifamClubBadgeShape::Crest, 9, 1, false);
        }
        else if (var == 13) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 6, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 6, 1, true);
        }
        else if (var == 14) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 4, 2, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 4, 2, true);
        }
        else if (var == 15) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 4, 5, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 4, 5, true);
        }
        else if (var == 16) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 5, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 5, 1, true);
        }
        else if (var == 17) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 5, 2, false);
            else if (pat == L'b')
                SetColor(FifamClubBadgeShape::Crest, 5, 3, false);
            else if (pat == L'c')
                SetColor(FifamClubBadgeShape::Crest, 5, 4, false);
            else if (pat == L'd')
                SetColor(FifamClubBadgeShape::Crest, 5, 3, true);
            else if (pat == L'e')
                SetColor(FifamClubBadgeShape::Crest, 5, 4, true);
            else
                SetColor(FifamClubBadgeShape::Crest, 5, 2, true);
        }
        else if (var == 18) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Triangle, 1, 1, false);
            else
                SetColor(FifamClubBadgeShape::Triangle, 1, 1, true);
        }
        else if (var == 19) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 10, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 10, 1, true);
        }
        else if (var == 20) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 7, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 7, 1, true);
        }
        else if (var == 21) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 8, 1, true);
            else
                SetColor(FifamClubBadgeShape::Crest, 8, 1, false);
        }
        else if (var == 22) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 13, 1, true);
            else
                SetColor(FifamClubBadgeShape::Crest, 13, 1, false);
        }
        else if (var == 23) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 12, 1, false);
            else
                SetColor(FifamClubBadgeShape::Crest, 12, 1, true);
        }
        else if (var == 24) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Flag, 1, 1, false);
            else
                SetColor(FifamClubBadgeShape::Flag, 1, 1, true);
        }
        else if (var == 25) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Crest, 11, 1, true);
            else
                SetColor(FifamClubBadgeShape::Crest, 11, 1, false);
        }
        else if (var == 26) {
            if (pat == L'a')
                SetColor(FifamClubBadgeShape::Oval, 1, 2, false);
            else
                SetColor(FifamClubBadgeShape::Oval, 1, 2, true);
        }
    }
}

void FifamClubBadge::Read(FifamReader &reader) {
    SetBadgePath(reader.ReadFullLine());
}

void FifamClubBadge::Write(FifamWriter &writer) {
    writer.WriteLine(GetBadgePath(writer.GetGameId()));
}
