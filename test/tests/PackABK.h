#pragma once
#include "FifamTypes.h"
#include "Error.h"

class PackABK {
public:
    template<typename RawType, typename Type>
    static RawType *Ptr(Type *p, unsigned int offset = 0) {
        return reinterpret_cast<RawType *>(reinterpret_cast<unsigned int>(p) + offset);
    }

    PackABK() {
        FILE *tf = nullptr;
        fopen_s(&tf, "abk\\template\\template.abk", "rb");
        if (tf) {
            fseek(tf, 0, SEEK_END);
            auto fileSizeOld = ftell(tf);
            fseek(tf, 0, SEEK_SET);
            UChar *dataOld = new UChar[fileSizeOld];
            fread(dataOld, fileSizeOld, 1, tf);
            fclose(tf);
            UInt bankOffset = *Ptr<UInt>(dataOld, 32);
            UInt bankSizeOld = *Ptr<UInt>(dataOld, 36);
            for (auto const &i : directory_iterator("abk")) {
                Path p = i.path();
                if (p.extension().string() == ".bnk") {
                    FILE *f = nullptr;
                    _wfopen_s(&f, p.c_str(), L"rb");
                    if (f) {
                        fseek(f, 0, SEEK_END);
                        auto fileSizeBNK = ftell(f);
                        fseek(f, 0, SEEK_SET);
                        UChar *dataBNK = new UChar[fileSizeBNK];
                        fread(dataBNK, fileSizeBNK, 1, f);
                        fclose(f);
                        p.replace_extension(".abk");
                        FILE *out = nullptr;
                        _wfopen_s(&out, p.c_str(), L"wb");
                        if (out) {
                            Int bankSizeDiff = fileSizeBNK - bankSizeOld;
                            Vector<UChar> dataNew(fileSizeOld + bankSizeDiff);
                            memcpy(dataNew.data(), dataOld, bankOffset);
                            memcpy(dataNew.data() + bankOffset, dataBNK, fileSizeBNK);
                            memcpy(dataNew.data() + bankOffset + fileSizeBNK, &dataOld[bankOffset + bankSizeOld], fileSizeOld - bankSizeOld - bankOffset);
                            *Ptr<UInt>(dataNew.data(), 20) = dataNew.size();
                            *Ptr<UInt>(dataNew.data(), 36) = fileSizeBNK;
                            *Ptr<UInt>(dataNew.data(), 48) = *Ptr<UInt>(dataNew.data(), 48) + bankSizeDiff;
                            *Ptr<UInt>(dataNew.data(), 52) = *Ptr<UInt>(dataNew.data(), 52) + bankSizeDiff;
                            *Ptr<UInt>(dataNew.data(), 56) = *Ptr<UInt>(dataNew.data(), 56) + bankSizeDiff;
                            UInt eventsOffset = *Ptr<UInt>(dataNew.data(), 56);
                            UInt numEvents = *Ptr<UInt>(dataNew.data(), eventsOffset);
                            for (UInt i = 0; i < numEvents; i++)
                                *Ptr<UInt>(dataNew.data(), eventsOffset + 4 + i * 12 + 4)  = *Ptr<UInt>(dataNew.data(), eventsOffset + 4 + i * 12 + 4) + bankSizeDiff;
                            fwrite(dataNew.data(), dataNew.size(), 1, out);
                            fclose(out);
                        }
                        else
                            ::Error("Unable to open " + p.filename().string());
                    }
                    else
                        ::Error("Unable to open " + p.filename().string());
                }
            }
        }
        else
            ::Error("Unable to open template.abk");
    }
};
