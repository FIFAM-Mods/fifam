#pragma once

#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"

class CopyPortraitsLight {
public:
    static void Copy(Path const &from, Path const &to) {
        try {
            copy(from, to, copy_options::skip_existing);
        }
        catch (std::exception &e) {
            ::Error(e.what());
        }
    }

    static bool CopyPlayerPortrait(Path const &base, String const &strId, UInt empicsId, Path const &output) {
        String filename = strId + L".png";
        String filenameWithEmpics = strId + L"-" + Utils::Format(L"%d", empicsId) + L".png";
        Path p = base / L"art_02\\portraits\\club\\160x160" / filenameWithEmpics;
        if (exists(p)) {
            Copy(p, output / filenameWithEmpics);
            return true;
        }
        p = base / L"art_03\\portraits\\club\\160x160" / filenameWithEmpics;
        if (exists(p)) {
            Copy(p, output / filenameWithEmpics);
            return true;
        }
        p = base / L"art_06\\portraits\\club\\160x160" / filenameWithEmpics;
        if (exists(p)) {
            Copy(p, output / filenameWithEmpics);
            return true;
        }
        p = base / L"art_07\\portraits\\club\\160x160" / filenameWithEmpics;
        if (exists(p)) {
            Copy(p, output / filenameWithEmpics);
            return true;
        }
        p = base / L"art_02\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_03\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_06\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_07\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        return false;
    }

    static bool CopyStaffPortrait(Path const &base, String const &strId, Path const &output) {
        String filename = strId + L".png";
        Path p = base / L"art_02\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_03\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_06\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        p = base / L"art_07\\portraits\\club\\160x160" / filename;
        if (exists(p)) {
            Copy(p, output / filename);
            return true;
        }
        return false;
    }

    CopyPortraitsLight() {

        Path base = L"D:\\Projects\\fifam\\content\\fm13";
        Path output = L"D:\\Projects\\fifam\\content\\fm13_light\\art_02\\portraits\\club\\160x160";

        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();

        const UInt MAX_PLAYERS = 15'000;
        const UInt MAX_STAFF = 600;
        const UInt MAX_EMPLOYEE = 400;

        UInt numPlayers = 0;
        UInt numStaff = 0;
        UInt numEmployee = 0;

        Set<FifamPlayer *> addedPlayers;

        std::wcout << L"Portraits/players" << std::endl;

        //for (FifamPlayer *p : db->mPlayers) {
        //    if (numPlayers >= MAX_PLAYERS)
        //        break;
        //    if (p->GetLevel() >= 67 && !Utils::Contains(addedPlayers, p) && CopyPlayerPortrait(base, p->GetStringUniqueId(14, false), p->mEmpicsId, output)) {
        //        addedPlayers.insert(p);
        //        numPlayers++;
        //    }
        //}

        for (FifamCountry *country : db->mCountries) {
            std::wcout << FifamTr(country->mName) << std::endl;
            if (country && country->mId == FifamNation::Italy) {
                for (FifamClub *c : country->mClubs) {
                    std::wcout << FifamTr(c->mName) << std::endl;
                    for (FifamPlayer *p : c->mPlayers) {
                        if (numPlayers >= MAX_PLAYERS)
                            break;
                        if (p->GetLevel() >= 67 && !Utils::Contains(addedPlayers, p) && CopyPlayerPortrait(base, p->GetStringUniqueId(14, false), p->mEmpicsId, output)) {
                            addedPlayers.insert(p);
                            numPlayers++;
                        }
                    }
                }
            }
        }

        //for (FifamPlayer *p : db->mPlayers) {
        //    if (numPlayers >= MAX_PLAYERS)
        //        break;
        //    if (p->GetLevel() >= 67 && !Utils::Contains(addedPlayers, p) && CopyPlayerPortrait(base, p->GetStringUniqueId(14, false), p->mEmpicsId, output)) {
        //        addedPlayers.insert(p);
        //        numPlayers++;
        //    }
        //}

        ::Error(L"done with players");

        Vector<FifamClub *> clubs;
        std::copy(db->mClubs.begin(), db->mClubs.end(), std::back_inserter(clubs));

        std::sort(clubs.begin(), clubs.end(), [](FifamClub * a, FifamClub * b) { return a->mInternationalPrestige * 100 + a->mNationalPrestige > b->mInternationalPrestige * 100 + b->mNationalPrestige; });

        std::wcout << L"Portraits/players/clubs" << std::endl;

        for (FifamClub *c : clubs) {
            std::wcout << L"Club: " << FifamTr(c->mName) << L" (" << (c->mInternationalPrestige * 100 + c->mNationalPrestige) <<  L")" << std::endl;
            if (numPlayers >= MAX_PLAYERS)
                break;
            for (FifamPlayer *p : c->mPlayers) {
                if (!Utils::Contains(addedPlayers, p) && CopyPlayerPortrait(base, p->GetStringUniqueId(14, false), p->mEmpicsId, output)) {
                    addedPlayers.insert(p);
                    numPlayers++;
                }
            }
        }

        Vector<FifamStaff *> staff;
        Vector<FifamStaff *> employee;

        for (FifamStaff *s : db->mStaffs) {
            if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                employee.push_back(s);
            else
                staff.push_back(s);
        }

        std::sort(staff.begin(), staff.end(), [](FifamStaff * a, FifamStaff * b) { return a->GetStaffLevel() > b->GetStaffLevel(); });
        std::sort(employee.begin(), employee.end(), [](FifamStaff * a, FifamStaff * b) { return a->GetManagerLevel() > b->GetManagerLevel(); });

        std::wcout << L"Portraits/staff" << std::endl;

        for (FifamStaff *s : staff) {
            if (numStaff >= MAX_STAFF)
                break;
            if (CopyStaffPortrait(base, s->GetStringUniqueId(14), output))
                numStaff++;
        }

        std::wcout << L"Portraits/employee" << std::endl;

        for (FifamStaff *s : employee) {
            if (numEmployee >= MAX_EMPLOYEE)
                break;
            if (CopyStaffPortrait(base, s->GetStringUniqueId(14), output))
                numEmployee++;
        }
    }
};
