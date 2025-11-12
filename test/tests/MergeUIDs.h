#pragma once
#include "FifamDbEnvironment.h"

const Bool WomenDatabase = false;

struct ClubInfo {
    UChar CountryId = 0;
    String Country;
    String Name;
    String Extinct;
    String AffiliationType;
    String MainTeamName;
    Int FootballManagerID = -1;
    UInt FIFAManagerID = 0;
    UInt FIFAID = 0;
    String Status;
    String League;
    UShort YearOF = 0;
    String City;
    UInt Reputation = 0;
    UChar LeagueLevel = 255;
    Bool Valid = false;
    Bool IsNew = false;
    Bool IsFake = false;
    UChar OldLeagueLevel = 255;
};

class MergeUIDs {
public:
    void ReadUIDs(Path const &filePath, Map<Int, ClubInfo> &uids, Set<UInt> &fakeFifamUids, Bool newFile) {
        Set<Int> uidsFoom;
        Set<UInt> uidsFifam;
        Set<UInt> uidsFifa;
        FifamReader r(filePath, 14, false, false);
        if (r.Available()) {
            r.SkipLine();
            while (!r.IsEof()) {
                if (!r.EmptyLine()) {
                    ClubInfo c;
                    String dummy;
                    r.ReadLine(
                        c.Country,
                        c.Name,
                        c.Extinct,
                        c.AffiliationType,
                        c.MainTeamName,
                        c.FootballManagerID,
                        Hexadecimal(c.FIFAManagerID),
                        c.FIFAID,
                        c.Status,
                        c.League,
                        c.YearOF,
                        c.City,
                        c.Reputation,
                        c.LeagueLevel,
                        c.Valid,
                        c.IsNew,
                        c.IsFake,
                        c.OldLeagueLevel);
                    if (!newFile && c.IsFake && c.FIFAManagerID != 0) {
                        fakeFifamUids.insert(c.FIFAManagerID);
                        continue;
                    }
                    if (c.IsFake)
                        continue;
                    if (c.FootballManagerID != -1) {
                        if (uidsFoom.contains(c.FootballManagerID)) {
                            ::Error(L"Duplicated Football Manager ID: %d\nIn file \"%s\"",
                                c.FootballManagerID, filePath.c_str());
                            continue;
                        }
                        uidsFoom.insert(c.FootballManagerID);
                        if (c.FIFAManagerID != 0) {
                            if (uidsFifam.contains(c.FIFAManagerID)) {
                                ::Error(L"Duplicated FIFA Manager ID: %08X\nIn file \"%s\"",
                                    c.FIFAManagerID, filePath.c_str());
                                continue;
                            }
                            UChar teamType = (c.FIFAManagerID >> 24) & 0xFF;
                            if (teamType != 0) {
                                ::Error(L"Not a valid FIFA Manager ID: %08X\nIn file \"%s\"",
                                    c.FIFAManagerID, filePath.c_str());
                                continue;
                            }
                            if (WomenDatabase) {
                                Bool validFifamID = (c.FIFAManagerID & 0xFFFF) > 0xA000;
                                if (!validFifamID) {
                                    ::Error(L"Not a valid FIFA Manager ID: %08X\nClub index must be greater than 0xA000\nIn file \"%s\"",
                                        c.FIFAManagerID, filePath.c_str());
                                    continue;
                                }
                            }
                            else {
                                Bool validFifamID = (c.FIFAManagerID & 0xFFFF) < 0xA000;
                                if (!validFifamID) {
                                    ::Error(L"Not a valid FIFA Manager ID: %08X\nClub index must be lower than 0xA000\nIn file \"%s\"",
                                        c.FIFAManagerID, filePath.c_str());
                                    continue;
                                }
                            }
                            uidsFifam.insert(c.FIFAManagerID);
                        }
                        if (c.FIFAID != 0) {
                            if (uidsFifa.contains(c.FIFAID)) {
                                ::Error(L"Duplicated FIFA ID: %u\nIn file \"%s\"",
                                    c.FIFAID, filePath.c_str());
                                continue;
                            }
                            uidsFifa.insert(c.FIFAID);
                        }
                        if (!newFile && c.FIFAManagerID == 0 && c.FIFAID == 0)
                            continue;
                        if (newFile) {
                            c.FIFAManagerID = 0;
                            c.FIFAID = 0;
                            c.OldLeagueLevel = 255;
                        }
                        else {
                            c.OldLeagueLevel = c.LeagueLevel;
                            c.LeagueLevel = 255;
                            c.League.clear();
                        }
                        c.Valid = newFile;
                        c.IsNew = newFile;
                        if (newFile)
                            uids[c.FootballManagerID] = c;
                        else {
                            Bool contains = Utils::Contains(uids, c.FootballManagerID);
                            if (!contains)
                                uids[c.FootballManagerID] = c;
                            else {
                                auto &newC = uids[c.FootballManagerID];
                                newC.FIFAManagerID = c.FIFAManagerID;
                                newC.FIFAID = c.FIFAID;
                                newC.IsNew = false;
                            }
                        }
                    }
                }
                else
                    r.SkipLine();
            }
        }
        else
            ::Error(L"Failed to read \"%s\"", filePath.c_str());
    }

    MergeUIDs() {
        StringA suffix = WomenDatabase ? "_women" : StringA();
        Map<Int, UInt> foomNationToFifam;
        {
            FifamReader reader(L"fifam_countries.txt", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        Int FootballManagerID = 0;
                        UInt FIFAManagerID = 0;
                        reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, FootballManagerID, dummy, dummy, FIFAManagerID);
                        if (FootballManagerID != 0 && FIFAManagerID != 0)
                            foomNationToFifam[FootballManagerID] = FIFAManagerID;
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
        Map<Int, Int> clubNationLinks;
        {
            FifamReader reader(L"foom\\fm_clubs.csv", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        Int FootballManagerID = -1, Nation = -1, ContinentalCupNation = -1;
                        reader.ReadLine(FootballManagerID, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, Nation, dummy, ContinentalCupNation);
                        if (FootballManagerID != -1) {
                            Int nationID = (Nation != -1) ? Nation : ContinentalCupNation;
                            if (nationID != -1)
                                clubNationLinks[FootballManagerID] = nationID;
                        }
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
        Map<Int, ClubInfo> uids;
        Set<UInt> fakeFifamUids;
        ReadUIDs("foom\\fifam-uids" + suffix + ".csv", uids, fakeFifamUids, true);
        for (auto &[foomId, info] : uids) {
            if (Utils::Contains(clubNationLinks, foomId)) {
                Int nationID = clubNationLinks[foomId];
                if (Utils::Contains(foomNationToFifam, nationID))
                    info.CountryId = foomNationToFifam[nationID];
            }
        }
        ReadUIDs("fifam-uids" + suffix + ".csv", uids, fakeFifamUids, false);
        Array<UInt, FifamDatabase::NUM_COUNTRIES> maxUid = {};
        for (auto &[foomId, info] : uids) {
            if (info.FIFAManagerID != 0) {
                UChar region = (info.FIFAManagerID >> 16) & 0xFF;
                if (region >= 1 && region <= FifamDatabase::NUM_COUNTRIES) {
                    if (info.FIFAManagerID > maxUid[region - 1])
                        maxUid[region - 1] = info.FIFAManagerID;
                }
            }
        }
        for (auto i : fakeFifamUids) {
            UChar region = (i >> 16) & 0xFF;
            if (region >= 1 && region <= FifamDatabase::NUM_COUNTRIES) {
                if (i > maxUid[region - 1])
                    maxUid[region - 1] = i;
            }
        }
        UShort baseIndex = WomenDatabase ? 0xA000 : 0;
        for (UInt i = 0; i < std::size(maxUid); i++) {
            if (maxUid[i] == 0) {
                UChar countryId = i + 1;
                maxUid[i] = (countryId << 16) | baseIndex;
            }
        }
        Vector<ClubInfo> vecUids;
        vecUids.reserve(uids.size());
        UInt maxIndex = WomenDatabase ? (0xFFFF - 1) : (0xA000 - 1);
        Array<Bool, FifamDatabase::NUM_COUNTRIES> maxIndexErrorShown = {};
        for (auto &[foomId, info] : uids) {
            if (info.Valid && info.CountryId != 0 && info.FIFAManagerID == 0) {
                UInt index = maxUid[info.CountryId - 1] & 0xFFFF;
                if (index == maxIndex) {
                    if (!maxIndexErrorShown[info.CountryId - 1]) {
                        ::Error(L"Reached max index for new UID (0x%08X). Can't generate new UIDs.",
                            maxUid[info.CountryId - 1]);
                        maxIndexErrorShown[info.CountryId - 1] = true;
                    }
                }
                else
                    info.FIFAManagerID = ++maxUid[info.CountryId - 1];
            }
            vecUids.push_back(info);
        }
        Utils::Sort(vecUids, [](ClubInfo const &a, ClubInfo const &b) {
            if (a.Country != b.Country)
                return a.Country < b.Country;
            if (a.LeagueLevel != b.LeagueLevel)
                return a.LeagueLevel < b.LeagueLevel;
            Bool aInLeague = (a.LeagueLevel != 255);
            Bool bInLeague = (b.LeagueLevel != 255);
            if (aInLeague && bInLeague) {
                if (a.League != b.League)
                    return a.League < b.League;
                return a.Name < b.Name;
            }
            return a.Reputation > b.Reputation;
        });
        FifamWriter w("fifam-uids-new" + suffix + ".csv");
        w.SetReplaceQuotes(false);
        auto q = [](String const &s) { return L"\"" + s + L"\""; };
        w.WriteLine(L"Country,Name,Extinct,AffiliationType,MainTeamName,FootballManagerID,FIFAManagerID,FIFAID,Status,League,YearOF,City,Reputation,LeagueLevel,Valid,IsNew,IsFake,OldLeagueLevel");
        for (auto const &c : vecUids) {
            w.WriteLine(
                q(c.Country),
                q(c.Name),
                q(c.Extinct),
                q(c.AffiliationType),
                q(c.MainTeamName),
                c.FootballManagerID,
                (c.FIFAManagerID != 0) ? Utils::Format(L"0x%08X", c.FIFAManagerID) : String(),
                (c.FIFAID != 0) ? to_wstring(c.FIFAID) : String(),
                q(c.Status),
                q(c.League),
                (c.YearOF != 0) ? to_wstring(c.YearOF) : String(),
                q(c.City),
                c.Reputation,
                c.LeagueLevel,
                c.Valid,
                c.IsNew,
                c.IsFake,
                c.OldLeagueLevel);
        }
    }
};
