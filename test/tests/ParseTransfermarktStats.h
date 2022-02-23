#pragma once
#include "FifamDbEnvironment.h"
#include "Error.h"
#include "FifamTypes.h"

class ParseTransfermarktStats {
public:
    static UInt GetAge(FifamDate const &birthDate, FifamDate const &currentDate) {
        if (currentDate.year <= birthDate.year)
            return 0;
        UInt age = currentDate.year - birthDate.year - 1;
        if (birthDate.month > currentDate.month || (birthDate.month == currentDate.month && birthDate.day >= currentDate.day))
            ++age;
        return age;
    }

    ParseTransfermarktStats() {
        FifamReader r("E:\\data.txt");
        FifamWriter w("E:\\data_out.txt");
        Bool isWC = true;
        w.WriteLine(L"ID\tFirstName\tLastName\tPseudonym\tNationality\tApps\tGoals\tAge");
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, L"E:\\Games\\FIFA Manager 22\\database");
        db.SetupWriteableStatus(13);
        Map<String, Vector<FifamPlayer *>> players;
        for (auto &p : db.mPlayers) {
            String playerName;
            if (p->mPseudonym.empty())
                playerName = p->mFirstName + L" " + p->mLastName;
            else
                playerName = p->mPseudonym;
            players[playerName].push_back(p);
        }
        for (auto &pm : players) {
            std::sort(pm.second.begin(), pm.second.end(), [](FifamPlayer *a, FifamPlayer *b) {
                return a->GetLevel() >= b->GetLevel();
            });
        }
        auto dateParts = Utils::Split(Utils::Split(Utils::CurrentTime(), L' ')[0], L'.');
        FifamDate date(Utils::SafeConvertInt<UInt>(dateParts[0]), Utils::SafeConvertInt<UInt>(dateParts[1]), Utils::SafeConvertInt<UInt>(dateParts[2]));
        if (r.Available()) {
            while (!r.IsEof()) {
                r.SkipLine();
                String playerName = Utils::Split(r.ReadFullLine(), L'\t')[0];
                if (!playerName.empty()) {
                    r.SkipLine();
                    String nationName;
                    UInt age = 0;
                    UInt matches = 0;
                    UInt goals = 0;
                    if (isWC) {
                        nationName = Utils::Split(r.ReadFullLine(), L'\t')[1];
                        auto stats = Utils::Split(r.ReadFullLine(), L'\t');
                        UInt age = Utils::SafeConvertInt<UInt>(stats[0]);
                        UInt matches = Utils::SafeConvertInt<UInt>(stats[2]);
                        UInt goals = Utils::SafeConvertInt<UInt>(stats[3]);
                    }
                    else {
                        auto stats = Utils::Split(r.ReadFullLine(), L'\t');
                        nationName = stats[0];
                        age = Utils::SafeConvertInt<UInt>(stats[1]);
                        matches = Utils::SafeConvertInt<UInt>(stats[3]);
                        goals = Utils::SafeConvertInt<UInt>(stats[4]);
                    }
                    String firstName, lastName, pseudonym;
                    auto nameParts = Utils::Split(playerName, L' ');
                    if (nameParts.size() == 1)
                        pseudonym = nameParts[0];
                    else if (nameParts.size() == 2) {
                        firstName = nameParts[0];
                        lastName = nameParts[1];
                    }
                    else if (nameParts.size() >= 3) {
                        firstName = nameParts[0];
                        lastName = playerName.substr(playerName.find(L' ') + 1);
                    }
                    FifamNation nation;
                    nation.SetFromStr(nationName);
                    if (nationName == L"Cote d'Ivoire")
                        nation = FifamNation::Cote_d_Ivoire;
                    else if (nationName == L"Korea, South")
                        nation = FifamNation::Korea_Republic;
                    else if (nationName == L"Trinidad and Tobago")
                        nation = FifamNation::Trinidad_Tobago;
                    else if (nationName == L"North Macedonia")
                        nation = FifamNation::FYR_Macedonia;
                    else if (nationName == L"French Guiana")
                        nation = FifamNation::France;
                    String nationId = nation.GetWasSetFromUnknown() ? nationName : std::to_wstring(nation.ToInt());
                    String playerId;
                    if (players.contains(playerName)) {
                        auto &pm = players[playerName];
                        for (auto &p : pm) {
                            UInt page = p->GetAge(FifamDate(1, 7, 2021));
                            if (nation == p->mNationality[0] && (age == page || age == (page + 1))) {
                                playerId = p->mWriteableStringID;
                                firstName = p->mFirstName;
                                lastName = p->mLastName;
                                pseudonym = p->mPseudonym;
                                break;
                            }
                        }
                    }
                    w.WriteLineWithSeparator(L'\t', playerId, firstName, lastName, pseudonym, nationId, matches, goals, age);
                }
            }
        }
    }
};
