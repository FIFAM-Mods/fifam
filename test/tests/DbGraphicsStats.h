#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "FifamNames.h"
#include "FifamCompLeague.h"

#define BADGES 1
#define PORTRAITS_FMZ 1
#define PORTRAITS_SORTITOUTSI 1
#define PORTRAITS_XXL 1

class DbGraphicsStats {
public:
    static String NationName(FifamDatabase *db, FifamNation const &id) {
        if (id.ToInt() >= 1 && id.ToInt() <= FifamDatabase::NUM_COUNTRIES && db->mCountries[id.ToInt() - 1])
            return FifamTr(db->mCountries[id.ToInt() - 1]->mName);
        return String();
    }

    static UInt PictureStatus(Vector<Path> const &paths, String filename, String filenameAlt = String()) {
        if (!filename.empty()) {
            UInt numPasses = filenameAlt.empty() ? 1 : 2;
            for (UInt n = 1; n <= numPasses; n++) {
                String f = (n == 1) ? filename : filenameAlt;
                for (auto const &p : paths) {
                    if (exists(p / f))
                        return n;
                }
            }
        }
        return 0;
    }

    DbGraphicsStats() {
        UInt gameVersion = 13;
        FifamDatabase *db = new FifamDatabase(gameVersion, L"D:\\Games\\FIFA Manager 22\\database");
        db->SetupWriteableStatus(gameVersion, false);
        Map<FifamClub *, FifamCompLeague *> clubLeague;

        for (auto const &[id, c] : db->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid() && t.IsFirstTeam())
                        clubLeague[t.mPtr] = l;
                }
            }
        }

        Vector<Path> badgesClubPathClean = {
            R"(D:\Projects\fifam\content\fm13\clean_badges\badges\badges\clubs\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_BADGES_1\badges\clubs\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_BADGES_1\badges\clubs\256x256)",
        };

        Vector<Path> badgesClubPathDvx = {
            R"(D:\Projects\fifam\content\fm13\badges\badges\clubs\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_BADGES_2\badges\clubs\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_BADGES_2\badges\clubs\256x256)",
        };

        Vector<Path> badgesLeaguePathClean = {
            R"(D:\Projects\fifam\content\fm13\clean_badges\badges\badges\Leagues\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_BADGES_1\badges\Leagues\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_BADGES_1\badges\Leagues\256x256)",
        };

        Vector<Path> badgesLeaguePathDvx = {
            R"(D:\Projects\fifam\content\fm13\badges\badges\Leagues\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_BADGES_2\badges\Leagues\256x256)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_BADGES_2\badges\Leagues\256x256)",
        };

        Vector<Path> portraitsPlayerPathSortitoutsi = {
            R"(D:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_08\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_09\portraits\club\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_PORTRAITS_1\portraits\club\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_PORTRAITS_1\portraits\club\160x160)",
        };

        Vector<Path> portraitsPlayerPathFMZ = {
            R"(D:\Projects\fifam\content\fm13\portraits_fmz\art_02\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\portraits_fmz\art_03\portraits\club\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_PORTRAITS_2\portraits\club\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_PORTRAITS_2\portraits\club\160x160)",
        };

        Vector<Path> portraitsRefereePathSortitoutsi = {
            R"(D:\Projects\fifam\content\fm13\art_02\portraits\Referees\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_PORTRAITS_1\portraits\Referees\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_PORTRAITS_1\portraits\Referees\160x160)",
        };

        Vector<Path> portraitsRefereePathFMZ = {
            R"(D:\Projects\fifam\content\fm13\portraits_fmz\art_02\portraits\Referees\160x160)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_PORTRAITS_2\portraits\Referees\160x160)",
        };

        Vector<Path> portraitsXXLPath = {
            R"(D:\Projects\fifam\content\fm13\art_12\portraits\club\512x512)",
            R"(D:\Projects\fifam\content\fm13\art_13\portraits\club\512x512)",
            R"(D:\Projects\fifam\content\fm13\art_14\portraits\club\512x512)",
            R"(D:\Projects\fifam\content\fm13\art_15\portraits\club\512x512)",
            R"(D:\Projects\fifam\content\fm13\art_16\portraits\club\512x512)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 1\_UPDATE_XXL\portraits\club\512x512)",
            //R"(C:\Users\Dmitri\Desktop\FM 24 update 2\_UPDATE_XXL\portraits\club\512x512)",
        };

        wcout << endl;
        wcout << L"Graphics statistics:" << endl;
        wcout << endl;
#if BADGES == 1
        {
            UInt numClubBadgesClean = 0, numLeagueBadgesClean = 0, numClubBadgesDvx = 0, numLeagueBadgesDvx = 0;
            for (auto const &[id, c] : db->mCompMap) {
                String uid = Utils::Format(L"%08X.tga", id.ToInt());
                String uid2;
                if (id.mRegion.ToInt() >= 249 && id.mRegion.ToInt() <= 255)
                    uid2 = Utils::Format(L"%04X.tga", id.ToInt() >> 16);
                if (PictureStatus(badgesLeaguePathClean, uid, uid2))
                    numLeagueBadgesClean++;
                if (PictureStatus(badgesLeaguePathDvx, uid, uid2))
                    numLeagueBadgesDvx++;
            }
            for (auto const &[id, c] : db->mClubsMap) {
                String uid = Utils::Format(L"%08X.tga", id);
                if (PictureStatus(badgesClubPathClean, uid))
                    numClubBadgesClean++;
                if (PictureStatus(badgesClubPathDvx, uid))
                    numClubBadgesDvx++;
            }
            wcout << L"Badges Style - Clean: " << (numClubBadgesClean + numLeagueBadgesClean) << endl;
            wcout << L"  Leagues: " << numLeagueBadgesClean << endl;
            wcout << L"  Clubs: " << numClubBadgesClean << endl;
            wcout << endl;
            wcout << L"Badges Style - DVX: " << (numClubBadgesDvx + numLeagueBadgesDvx) << endl;
            wcout << L"  Leagues: " << numLeagueBadgesDvx << endl;
            wcout << L"  Clubs: " << numClubBadgesDvx << endl;
            wcout << endl;
        }
#endif
#if PORTRAITS_XXL == 1
        {
            FifamWriter w(L"players_without_portrait_xxl.csv");
            w.WriteLine(L"id", L"foomid", L"empicsId", L"creator", L"name", L"status", L"birthdate", L"nationality", L"club", L"country", L"league", L"position", L"level", L"talent");
            UInt numXXLPortraits = 0;
            for (auto const &p : db->mPlayers) {
                String id = p->mWriteableStringID;
                String id2;
                if (p->mEmpicsId != 0) {
                    String empicsIdStr = Utils::Format(L"-%u", p->mEmpicsId);
                    if (p->mWriteableStringID.ends_with(empicsIdStr))
                        id2 = id.substr(0, id.size() - empicsIdStr.size());
                }
                UInt status = PictureStatus(portraitsXXLPath, id + L".png", id2 + L".png");
                if (status != 1) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague.contains(p->mClub))
                            clubLeagueLevel = clubLeague[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLine(p->mWriteableStringID, p->mFootballManagerID, p->mEmpicsId, p->mCreator, Quoted(p->GetName()), status, p->mBirthday.ToString(),
                        Quoted(NationName(db, p->mNationality[0])), Quoted(clubName), Quoted(clubCountry), clubLeagueLevel, p->mMainPosition.ToStr(),
                        p->GetLevel(), p->mTalent);
                }
                if (status != 0)
                    numXXLPortraits++;
            }
            wcout << L"Portraits XXL: " << numXXLPortraits << endl;
            wcout << endl;
        }
#endif
        UInt numStaffPortraitsSortitoutsi = 0;
        UInt numPlayerPortraitsSortitoutsi = 0;
        UInt numStaffPortraitsFMZ = 0;
        UInt numPlayerPortraitsFMZ = 0;
        UInt numRefereePortraitsSortitoutsi = 0;
        UInt numRefereePortraitsFMZ = 0;
#if PORTRAITS_SORTITOUTSI == 1
        {
            for (auto const &r : db->mReferees) {
                if (PictureStatus(portraitsRefereePathSortitoutsi, FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0) + L".png"))
                    numRefereePortraitsSortitoutsi++;
            }
        }
#endif
#if PORTRAITS_FMZ == 1
        {
            for (auto const &r : db->mReferees) {
                if (PictureStatus(portraitsRefereePathFMZ, FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0) + L".png"))
                    numRefereePortraitsFMZ++;
            }
        }
#endif
#if PORTRAITS_SORTITOUTSI == 1
        {
            FifamWriter w(L"staff_without_portrait_sortitoutsi.csv");
            w.WriteLine(L"id", L"foomid", L"creator", L"name", L"birthdate", L"nationality", L"club", L"country", L"league", L"position", L"level", L"talent");
            for (auto const &s : db->mStaffs) {
                UInt status = PictureStatus(portraitsPlayerPathSortitoutsi, s->mWriteableStringID + L".png");
                if (status == 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague.contains(s->mClub))
                            clubLeagueLevel = clubLeague[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLine(s->mWriteableStringID, s->mFootballManagerID, s->mCreator, Quoted(s->GetName()), s->mBirthday.ToString(), NationName(db, s->mNationality[0]), Quoted(clubName), Quoted(clubCountry), clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
                else
                    numStaffPortraitsSortitoutsi++;
            }
        }
#endif
#if PORTRAITS_FMZ == 1
        {
            FifamWriter w(L"staff_without_portrait_fmz.csv");
            w.WriteLine(L"id", L"foomid", L"creator", L"name", L"birthdate", L"nationality", L"club", L"country", L"league", L"position", L"level", L"talent");
            for (auto const &s : db->mStaffs) {
                UInt status = PictureStatus(portraitsPlayerPathFMZ, s->mWriteableStringID + L".png");
                if (status == 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague.contains(s->mClub))
                            clubLeagueLevel = clubLeague[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLine(s->mWriteableStringID, s->mFootballManagerID, s->mCreator, Quoted(s->GetName()), s->mBirthday.ToString(), NationName(db, s->mNationality[0]), Quoted(clubName), Quoted(clubCountry), clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
                else
                    numStaffPortraitsFMZ++;
            }
        }
#endif
#if PORTRAITS_SORTITOUTSI == 1
        {
            FifamWriter w(L"players_without_portrait_sortitoutsi.csv");
            w.WriteLine(L"id", L"foomid", L"empicsId", L"creator", L"name", L"status", L"birthdate", L"nationality", L"club", L"country", L"league", L"position", L"level", L"talent");
            for (auto const &p : db->mPlayers) {
                String id = p->mWriteableStringID;
                String id2;
                if (p->mEmpicsId != 0) {
                    String empicsIdStr = Utils::Format(L"-%u", p->mEmpicsId);
                    if (p->mWriteableStringID.ends_with(empicsIdStr))
                        id2 = id.substr(0, id.size() - empicsIdStr.size()) + L".png";
                }
                UInt status = PictureStatus(portraitsPlayerPathSortitoutsi, id + L".png", id2);
                if (status != 1) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague.contains(p->mClub))
                            clubLeagueLevel = clubLeague[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLine(p->mWriteableStringID, p->mFootballManagerID, p->mEmpicsId, p->mCreator, Quoted(p->GetName()), status, p->mBirthday.ToString(),
                        Quoted(NationName(db, p->mNationality[0])), Quoted(clubName), Quoted(clubCountry), clubLeagueLevel, p->mMainPosition.ToStr(),
                        p->GetLevel(), p->mTalent);
                }
                if (status != 0)
                    numPlayerPortraitsSortitoutsi++;
            }
        }
#endif
#if PORTRAITS_FMZ == 1
        {
            FifamWriter w(L"players_without_portrait_fmz.csv");
            w.WriteLine(L"id", L"foomid", L"empicsId", L"creator", L"name", L"status", L"birthdate", L"nationality", L"club", L"country", L"league", L"position", L"level", L"talent");
            for (auto const &p : db->mPlayers) {
                String id = p->mWriteableStringID;
                String id2;
                if (p->mEmpicsId != 0) {
                    String empicsIdStr = Utils::Format(L"-%u", p->mEmpicsId);
                    if (p->mWriteableStringID.ends_with(empicsIdStr))
                        id2 = id.substr(0, id.size() - empicsIdStr.size()) + L".png";
                }
                UInt status = PictureStatus(portraitsPlayerPathFMZ, id + L".png", id2);
                if (status != 1) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague.contains(p->mClub))
                            clubLeagueLevel = clubLeague[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLine(p->mWriteableStringID, p->mFootballManagerID, p->mEmpicsId, p->mCreator, Quoted(p->GetName()), status, p->mBirthday.ToString(),
                        Quoted(NationName(db, p->mNationality[0])), Quoted(clubName), Quoted(clubCountry), clubLeagueLevel, p->mMainPosition.ToStr(),
                        p->GetLevel(), p->mTalent);
                }
                if (status != 0)
                    numPlayerPortraitsFMZ++;
            }
        }
#endif
#if PORTRAITS_SORTITOUTSI == 1
        wcout << L"Portraits Sortitoutsi: " << (numPlayerPortraitsSortitoutsi + numStaffPortraitsSortitoutsi + numRefereePortraitsSortitoutsi) << endl;
        wcout << L"  Players: " << numPlayerPortraitsSortitoutsi << endl;
        wcout << L"  Staff: " << numStaffPortraitsSortitoutsi << endl;
        wcout << L"  Referees: " << numRefereePortraitsSortitoutsi << endl;
        wcout << endl;
#endif
#if PORTRAITS_FMZ == 1
        wcout << L"Portraits FMZ: " << (numPlayerPortraitsFMZ + numStaffPortraitsFMZ + numRefereePortraitsFMZ) << endl;
        wcout << L"  Players: " << numPlayerPortraitsFMZ << endl;
        wcout << L"  Staff: " << numStaffPortraitsFMZ << endl;
        wcout << L"  Referees: " << numRefereePortraitsFMZ << endl;
#endif
        delete db;
    }
};
