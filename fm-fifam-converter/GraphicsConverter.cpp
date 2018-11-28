#include "GraphicsConverter.h"
#include "Magick++.h"

using namespace Magick;

GraphicsConverter::GraphicsConverter() {
    InitializeMagick(NULL);
}

void GraphicsConverter::ConvertClubBadges(foom::db * db, Path const & fmGraphicsPath, Path const & contentPath, UInt gameId, Int minRep) {
    Map<Int, Path> availableBadges;
    for (auto const &i : recursive_directory_iterator(fmGraphicsPath / L"dvx-logos" / L"clubs" / L"primary" / L"@2x")) {
        Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
        if (id > 0 && i.path().extension() == ".png")
            availableBadges[id] = i.path();
    }
    String gameFolder = Utils::Format(L"fm%d", gameId);
    Path outputPath = contentPath / gameFolder / L"badges" / L"badges" / L"clubs";
    for (auto e : db->mClubs) {
        auto &club = e.second;
        if (club.mReputation > minRep && club.mConverterData.mFifamClub) {
            FifamClub *fifamClub = (FifamClub *)club.mConverterData.mFifamClub;
            auto it = availableBadges.find(club.mID);
            if (it != availableBadges.end()) {
                Image badgeImg((*it).second.string());
                if (badgeImg.isValid() && badgeImg.baseRows() >= 360) {
                    if (badgeImg.baseColumns() != badgeImg.baseRows()) {
                        UInt newSize = Utils::Max(badgeImg.baseColumns(), badgeImg.baseRows());
                        badgeImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                    }
                    String badgeName = Utils::Format(L"%08X.tga", fifamClub->mUniqueID);
                    badgeImg.resize(Geometry(256, 256));
                    badgeImg.write(Path(outputPath / L"256x256" / badgeName).string());
                    badgeImg.resize(Geometry(128, 128));
                    badgeImg.write(Path(outputPath / L"128x128" / badgeName).string());
                    badgeImg.resize(Geometry(64, 64));
                    badgeImg.write(Path(outputPath / L"64x64" / badgeName).string());
                    badgeImg.resize(Geometry(32, 32));
                    badgeImg.write(Path(outputPath / L"32x32" / badgeName).string());
                }
            }
        }
    }
}

void GraphicsConverter::ConvertPortrait(foom::person *person, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId) {
    if (person->mConverterData.mFifamPerson) {
        Path portraitPath = fmGraphicsPath / L"sortitoutsi" / L"faces" / (std::to_wstring(person->mID) + L".png");
        if (exists(portraitPath)) {
            Image portraitImg(portraitPath.string());
            if (portraitImg.isValid() && portraitImg.baseRows() >= 180 && portraitImg.baseColumns() >= 180) {
                if (portraitImg.baseColumns() != portraitImg.baseRows()) {
                    UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
                    portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
                }
                String gameFolder = Utils::Format(L"fm%d", gameId);
                FifamPerson *fifamPerson = (FifamPerson *)person->mConverterData.mFifamPerson;
                Path outputPath = contentPath / gameFolder / L"art_02" / L"portraits" / L"club" / L"160x160" / (fifamPerson->mWriteableStringID + L".png");
                portraitImg.resize(Geometry(160, 160));
                portraitImg.write(outputPath.string());
            }
        }
    }
}

void GraphicsConverter::ConvertPortraits(foom::db *db, Path const &fmGraphicsPath, Path const &contentPath, UInt gameId, Int minCA) {
    for (auto e : db->mPlayers) {
        auto &p = e.second;
        if (p.mCurrentAbility > minCA)
            ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
    }
    for (auto e : db->mNonPlayers) {
        auto &p = e.second;
        if (p.mCurrentAbility > minCA)
            ConvertPortrait(&p, fmGraphicsPath, contentPath, gameId);
    }
}
