#pragma once
#include "FifamDbEnvironment.h"
#include "TextFileTable.h"

class DumpCountryAppearance {
public:
    template<UInt N>
    static void AppendFlags(Vector<String> &dst, Array<UInt, N> const &ary) {
        dst.reserve(dst.size() + N * 32);
        for (UInt i = 0; i < N; ++i) {
            uint64_t v = static_cast<uint64_t>(ary[i]);
            for (UInt b = 0; b < 32; b++) {
                Bool bit_set = ((v >> b) & 1ull) != 0;
                if (bit_set)
                    dst.emplace_back(L"1");
                else
                    dst.emplace_back();
            }
        }
    }

    static void AppendFlags(Vector<String> &dst, UInt bitSet) {
        dst.reserve(dst.size() + 32);
        uint64_t v = static_cast<uint64_t>(bitSet);
        for (UInt b = 0; b < 32; b++) {
            Bool bit_set = ((v >> b) & 1ull) != 0;
            if (bit_set)
                dst.emplace_back(L"1");
            else
                dst.emplace_back();
        }
    }

    DumpCountryAppearance() {
        using namespace std;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase::mReadingOptions.mReadClubs = false;
        FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase db(13, "database_14");
        TextFileTable file;
        static Vector<String> typeNames = {
            L"Western Europe Light",
            L"Western Europe Dark",
            L"African 1",
            L"Arabic 1",
            L"African 2",
            L"Latin",
            L"Asian",
            L"Eastern Europe Light",
            L"Eastern Europe Dark",
            L"Arabic 2",
            L"Indian",
        };
        Vector<String> header = { L"Id", L"Name", L"AllAppearanceDefs" };
        for (UInt i = 0; i < typeNames.size(); i++)
            header.push_back(typeNames[i]);
        for (UInt i = 1; i <= 11; i++) {
            header.push_back(L"type" + to_wstring(i));
            header.push_back(L"percentage" + to_wstring(i));
        }
        static Vector<Pair<String, UInt>> flags = {
            { L"Head", 64 }, { L"Hair", 128 }, { L"HairColor", 32 }, { L"Beard", 32 }
        };
        for (auto const &f : flags) {
            for (UInt i = 1; i <= f.second; i++)
                header.push_back(f.first + to_wstring(i));
        }
        file.AddRow(header);
        for (auto c : db.mCountries) {
            String allAppearanceDefs;
            Vector<Pair<String, UChar>> types;
            UInt total = c->mAppearanceData.empty() ? 0 : c->mAppearanceData[c->mAppearanceData.size() - 1].mDistributionProbability;
            UInt prevValue = 0;
            for (UInt i = 0; i < c->mAppearanceData.size(); i++) {
                auto t = prevValue;
                prevValue = c->mAppearanceData[i].mDistributionProbability;
                c->mAppearanceData[i].mDistributionProbability -= t;
            }
            Utils::Sort(c->mAppearanceData, [](FifamCountryAppearanceData const &a, FifamCountryAppearanceData const &b) {
                if (a.mDistributionProbability == b.mDistributionProbability)
                    return typeNames[a.mAppearanceType - 1] < typeNames[b.mAppearanceType - 1];
                return a.mDistributionProbability > b.mDistributionProbability;
            });
            struct AppearanceData {
                UInt id = 0;
                String name;
                String percentage;
            };
            Vector<AppearanceData> data;
            Map<UInt, AppearanceData> dataById;
            if (total != 0) {
                struct RawEntry { UInt id; String name; UInt weight; UInt order; };
                std::vector<RawEntry> raw;
                raw.reserve(c->mAppearanceData.size());
                for (size_t i = 0; i < c->mAppearanceData.size(); ++i) {
                    auto const &d = c->mAppearanceData[i];
                    if (d.mDistributionProbability > 0) {
                        RawEntry re;
                        re.id = d.mAppearanceType;
                        re.name = typeNames[d.mAppearanceType - 1];
                        re.weight = d.mDistributionProbability;
                        re.order = raw.size();
                        raw.push_back(re);
                    }
                }
                if (!raw.empty()) {
                    size_t n = raw.size();
                    std::vector<Double> exactPercent(n);
                    for (size_t i = 0; i < n; ++i)
                        exactPercent[i] = (Double)raw[i].weight * 100.0 / (Double)total;
                    std::vector<Int> floorTenths(n);
                    std::vector<Double> frac(n);
                    Int sumFloor = 0;
                    for (size_t i = 0; i < n; ++i) {
                        Double tenthsExact = exactPercent[i] * 10.0;
                        Double epsTenths = tenthsExact + 1e-9;
                        Int f = (Int)std::floor(epsTenths);
                        floorTenths[i] = f;
                        frac[i] = epsTenths - (Double)f;
                        sumFloor += f;
                    }
                    Int remainder = 1000 - sumFloor;
                    Vector<UInt> idx(n);
                    for (UInt i = 0; i < n; ++i) idx[i] = i;
                    std::stable_sort(idx.begin(), idx.end(), [&](UInt a, UInt b) {
                        if (frac[a] == frac[b]) return raw[a].order < raw[b].order;
                        return frac[a] > frac[b];
                    });
                    if (remainder >= 0) {
                        for (Int k = 0; k < remainder && k < (Int)n; ++k)
                            floorTenths[idx[k]] += 1;
                    }
                    else {
                        Int diff = -remainder;
                        std::stable_sort(idx.begin(), idx.end(), [&](UInt a, UInt b) {
                            if (frac[a] == frac[b]) return raw[a].order < raw[b].order;
                            return frac[a] < frac[b];
                        });
                        for (Int k = 0; k < diff && k < (Int)n; ++k) {
                            if (floorTenths[idx[k]] > 0)
                                floorTenths[idx[k]] -= 1;
                        }
                    }
                    for (UInt i = 0; i < n; ++i) {
                        AppearanceData ad;
                        ad.id = raw[i].id;
                        ad.name = raw[i].name;
                        Double percentOne = (Double)floorTenths[i] / 10.0;
                        ad.percentage = Utils::Format(L"%.1f%%", (Float)percentOne);
                        data.push_back(ad);
                        dataById[ad.id] = ad;
                    }
                    Bool first = true;
                    for (auto const &ad : data) {
                        if (first)
                            first = false;
                        else
                            allAppearanceDefs += L", ";
                        allAppearanceDefs += ad.name + L" - " + ad.percentage;
                    }
                }
            }
            Vector<String> row = { to_wstring(c->mId), FifamTr(c->mName), allAppearanceDefs };
            for (UInt i = 1; i <= 11; i++) {
                if (Utils::Contains(dataById, i))
                    row.push_back(dataById[i].percentage);
                else
                    row.push_back(L"");
            }
            for (UInt i = 0; i < 11; i++) {
                if (i < data.size()) {
                    row.push_back(data[i].name);
                    row.push_back(data[i].percentage);
                }
                else {
                    row.push_back(L"");
                    row.push_back(L"");
                }
            }
            AppendFlags(row, c->mAppearanceFaceType);
            AppendFlags(row, c->mAppearanceHairStyle);
            AppendFlags(row, c->mAppearanceHairColor);
            AppendFlags(row, c->mAppearanceBeard);
            file.AddRow(row);
        }
        file.WriteUnicodeText("country_appearance.txt");
    }
};
