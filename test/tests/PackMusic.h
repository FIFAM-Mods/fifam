#pragma once
#include "FifamReadWrite.h"
#include "Error.h"
#include "Magick++.h"

class PackMusic {
public:
    const UInt version = 2021;

    struct Song {
        UInt id; // 20070105
        Char filename[8]; // S705
        Char artist[40];
        Char name[40];
        Char album[40];
        UInt unk1; // 9999
        UInt unk2; // 1
    };

    static StringA ToUTF8(String const &wstr) {
        if (wstr.empty())
            return StringA();
        Int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        StringA strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    static void ResizeImageNoAspect(Magick::Image &image, int w, int h) {
        Magick::Geometry geom(w, h);
        geom.aspect(true);
        image.resize(geom);
    }

    PackMusic() {
        using namespace Magick;
        FifamReader r("music\\eatrax.txt");
        if (r.Available()) {
            create_directories("music\\output");
            FILE *f = nullptr;
            fopen_s(&f, "music\\output\\songlist.bin", "wb");
            if (f) {
                r.SkipLine();
                UInt index = 0;
                while (!r.IsEof()) {
                    if (!r.EmptyLine()) {
                        String album, songname, artist;
                        UInt songid = 0;
                        r.ReadLineWithSeparator(L'\t', album, songname, artist, songid);
                        if (!songname.empty() && !artist.empty()) {
                            if (album.size() > 39)
                                album.resize(39);
                            if (songname.size() > 39)
                                songname.resize(39);
                            if (artist.size() > 39)
                                artist.resize(39);
                            Song song;
                            memset(&song, 0, sizeof(Song));
                            song.id = version * 10000 + 100 + index;
                            strcpy(song.filename, Utils::Format("S7%02u", index).c_str());
                            strcpy(song.artist, ToUTF8(artist).c_str());
                            strcpy(song.name, ToUTF8(songname).c_str());
                            strcpy(song.album, ToUTF8(album).c_str());
                            song.unk1 = 9999;
                            song.unk2 = 1;
                            fwrite(&song, sizeof(Song), 1, f);
                            Path albumPath = Utils::Format("music\\eatrax\\eatrax%u.dds", songid);
                            if (exists(albumPath)) {
                                Image album(albumPath.string());
                                album.alpha(false);
                                album.type(Magick::TrueColorType);
                                ResizeImageNoAspect(album, 260, 260);
                                const Int extendSize = 512 - 260;
                                // right
                                Magick::Image right(album, Magick::Geometry(1, album.rows(), album.columns() - 1, 0));
                                right.filterType(MagickCore::FilterType::PointFilter);
                                ResizeImageNoAspect(right, extendSize, album.rows());
                                // bottom
                                Magick::Image bottom(album, Magick::Geometry(album.columns(), 1, 0, album.rows() - 1));
                                bottom.filterType(MagickCore::FilterType::PointFilter);
                                ResizeImageNoAspect(bottom, album.rows(), extendSize);
                                // side
                                Magick::Image side(album, Magick::Geometry(1, 1, album.columns() - 1, album.rows() - 1));
                                side.filterType(MagickCore::FilterType::PointFilter);
                                ResizeImageNoAspect(side, extendSize, extendSize);
                                // composite
                                album.extent(Magick::Geometry(512, 512), Magick::Color(0, 0, 0), MagickCore::GravityType::NorthWestGravity);
                                album.composite(right, 260, 0, MagickCore::OverCompositeOp);
                                album.composite(bottom, 0, 260, MagickCore::OverCompositeOp);
                                album.composite(side, 260, 260, MagickCore::OverCompositeOp);
                                Path albumDir = Utils::Format("music\\output\\albums\\%u", song.id);
                                create_directories(albumDir);
                                album.write((albumDir / Utils::Format("1   @%u.tga", song.id)).string());
                                copy("music\\album\\20070100.apt", albumDir / Utils::Format("%u.apt", song.id), copy_options::overwrite_existing);
                                copy("music\\album\\20070100.const", albumDir / Utils::Format("%u.const", song.id), copy_options::overwrite_existing);
                                copy("music\\album\\20070100.o", albumDir / Utils::Format("%u.o", song.id), copy_options::overwrite_existing);
                            }
                        }
                        else
                            ::Error("Empty songname or artist");
                        index++;
                    }
                    else
                        r.SkipLine();
                }
                fclose(f);
            }
            else
                ::Error("Unable to open songlist.bin");
        }
        else
            ::Error("Unable to open eatrax.txt");
    }
};
