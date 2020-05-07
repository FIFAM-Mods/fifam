#include <string>
#include <vector>
#include <map>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

int main() {
    struct CommentEntry {
        path filepath;
        unsigned short filesize = 0;
        unsigned short offset = 0;
        bool high = false;
    };
    map<int, vector<CommentEntry>> comments;
    for (auto const &p : directory_iterator(current_path())) {
        auto const &path = p.path();
        if (path.extension() == ".asf" || path.extension() == ".ASF") {
            auto filename = path.stem().string();
            if (filename.starts_with("PLAYER_NAMES_")) {
                int playerId = atoi(filename.substr(13).c_str());
                if (playerId > 0) {
                    CommentEntry entry;
                    entry.filepath = path;
                    if (filename.find("_HIGH") != string::npos)
                        entry.high = true;
                    comments[playerId].push_back(entry);
                }
            }
        }
    }
    auto writeHeader = [](FILE *f, int id, vector<CommentEntry> const &files, unsigned short soundsSize, unsigned int flags) {
        static unsigned short soundType = 0x3CB;
        fwrite(&soundType, 2, 1, f);
        static unsigned char customDataSize = 1;
        fwrite(&customDataSize, 1, 1, f);
        unsigned char numSounds = files.size();
        fwrite(&numSounds, 1, 1, f);
        fwrite(&id, 4, 1, f);
        static unsigned char unknown1 = 0;
        fwrite(&unknown1, 1, 1, f);
        static unsigned char offsetMultiplier = 0;
        fwrite(&offsetMultiplier, 1, 1, f);
        fwrite(&soundsSize, 2, 1, f);
        static unsigned int unknown2 = 0;
        fwrite(&unknown2, 4, 1, f);
        for (auto const &c : files) {
            unsigned short offset = _byteswap_ushort(c.offset);
            fwrite(&offset, 2, 1, f);
            fwrite(&c.high, 1, 1, f);
        }
        unsigned int a = (files.size() * 3) % 4;
        if (a != 0) {
            static unsigned char align = 0x70;
            for (unsigned int i = 0; i < (4 - a); i++)
                fwrite(&align, 1, 1, f);
        }
        fwrite(&flags, 4, 1, f);
        return 0;

    };
    for (auto &[id, files] : comments) {
        sort(files.begin(), files.end(), [](CommentEntry const &a, CommentEntry const &b) {
            return a.filepath.string() < b.filepath.string();
        });
        static char pnfilename[32];
        sprintf_s(pnfilename, "pn%05d", id);
        unsigned int flags = 0;
        for (auto const &c : files) {
            if (c.high)
                flags |= 2;
            else
                flags |= 1;
        }
        static unsigned char zero = 0;
        unsigned int lastOffset = 0;
        {
            FILE *f = fopen((string(pnfilename) + ".dat").c_str(), "wb");
            for (auto &c : files) {
                FILE *cf = fopen(c.filepath.string().c_str(), "rb");
                fseek(cf, 0, SEEK_END);
                c.filesize = ftell(cf);
                c.offset = lastOffset;
                unsigned char *cdata = new unsigned char[c.filesize];
                fseek(cf, 0, SEEK_SET);
                fread(cdata, c.filesize, 1, cf);
                fclose(cf);
                fwrite(cdata, c.filesize, 1, f);
                delete[] cdata;
                unsigned int a = c.filesize % 0x100;
                if (a != 0) {
                    a = 0x100 - a;
                    for (unsigned int i = 0; i < a; i++)
                        fwrite(&zero, 1, 1, f);
                }
                lastOffset += (c.filesize + a) / 0x100;
            }
            for (unsigned int i = 0; i < 216; i++)
                fwrite(&zero, 1, 1, f);
            fwrite("HrIn", 4, 1, f);
            auto headerSize = writeHeader(f, id, files, lastOffset, flags);
            fwrite("HrSz", 4, 1, f);
            fwrite(&headerSize, 4, 1, f);
            fclose(f);
        }
        {
            FILE *f = fopen((string(pnfilename) + ".hdr").c_str(), "wb");
            writeHeader(f, id, files, lastOffset, flags);
            fclose(f);
        }
    }
}
