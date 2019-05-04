#pragma once
#include "FifamDbEnvironment.h"
#include "FifamNames.h"
#include "FifamCompLeague.h"
#include "FifamCompRound.h"

template<typename Game>
class ExportXML {
public:
    static String GetPersonNameLastFirst(String const &last, String const &first) {
            return last + L", " + first;
    }

    static String GetPersonName(String const &last, String const &first, String const &pseudonym) {
        if (!pseudonym.empty())
            return pseudonym;
        return GetPersonNameLastFirst(last, first);
    }

    static String GetPersonName(FifamPerson *person) {
        return GetPersonName(person->mLastName, person->mFirstName, person->mPseudonym);
    }

    ExportXML() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, Default>>().GetDatabase();

        FifamWriter writer(L"graphics_db.xml", 14, 0, 0, true);
        writer.SetReplaceQuotes(false);
        if (writer.Available()) {
            writer.WriteLine(L"<database name=\"Universal Converter Database\" version=\"2019\" update=\"2\" >");
            for (auto country : db->mCountries) {
                if (country) {
                    writer.WriteLine(Utils::Format(L"    <country name=\"%s\" continent=\"%d\" id=\"%d\" >",
                        FifamTr(country->mName), country->mContinent.ToInt(), country->mId));
                    
                    auto countryComps = country->GetCompetitions(true);
                    for (auto comp : countryComps) {
                        if (comp.second->GetDbType() == FifamCompDbType::League) {
                            FifamCompLeague *league = comp.second->AsLeague();
                            for (auto club : league->mTeams) {
                                if (club.IsValid()) {
                                    if (club.IsFirstTeam()) {
                                        auto clubLeague = club.mPtr->GetProperty<FifamCompLeague *>(L"league", nullptr);
                                        if (!clubLeague)
                                            club.mPtr->SetProperty<FifamCompLeague *>(L"league", league);
                                    }
                                    else if (club.IsReserveTeam()) {
                                        auto clubLeague = club.mPtr->GetProperty<FifamCompLeague *>(L"reserveleague", nullptr);
                                        if (!clubLeague)
                                            club.mPtr->SetProperty<FifamCompLeague *>(L"reserveleague", league);
                                    }
                                }
                            }
                        }
                    }
                    
                    std::sort(country->mClubs.begin(), country->mClubs.end(), [](FifamClub *a, FifamClub *b) {
                        return FifamTr(a->mName) < FifamTr(b->mName);
                    });
                    for (auto club : country->mClubs) {
                        writer.Write(Utils::Format(L"        <club name=\"%s\" id=\"%08X\"",
                            FifamTr(club->mName), club->mUniqueID));
                        auto clubLeague = club->GetProperty<FifamCompLeague *>(L"league", nullptr);
                        if (clubLeague)
                            writer.Write(Utils::Format(L" league=\"%s\"", clubLeague->mID.ToHexStr()));
                        auto reserveClubLeague = club->GetProperty<FifamCompLeague *>(L"reserveleague", nullptr);
                        if (reserveClubLeague)
                            writer.Write(Utils::Format(L" reserveleague=\"%s\"", reserveClubLeague->mID.ToHexStr()));
                        if (club->mKit.mSpecialKitId)
                            writer.Write(Utils::Format(L" kit=\"%d\"", club->mKit.mSpecialKitId));
                        if (!club->mPlayers.empty() || !club->mStaffs.empty()) {
                            writer.WriteLine(L" >");
                            std::sort(club->mPlayers.begin(), club->mPlayers.end(), [](FifamPlayer *a, FifamPlayer *b) {
                                return a->GetStringUniqueId(Game::id(), false) < b->GetStringUniqueId(Game::id(), false);
                            });
                            for (auto player : club->mPlayers) {
                                writer.Write(Utils::Format(L"            <player name=\"%s\" id=\"%s\"",
                                    GetPersonName(player), player->GetStringUniqueId(Game::id(), false)));
                                if (player->mEmpicsId)
                                    writer.Write(Utils::Format(L" empics=\"%d\"", player->mEmpicsId));
                                if (player->mInYouthTeam)
                                    writer.Write(L" team=\"2\"");
                                else if (player->mInReserveTeam)
                                    writer.Write(L" team=\"1\"");
                                writer.WriteLine(L" />");
                            }
                            std::sort(club->mStaffs.begin(), club->mStaffs.end(), [](FifamStaff *a, FifamStaff *b) {
                                return a->GetStringUniqueId(Game::id()) < b->GetStringUniqueId(Game::id());
                            });
                            for (auto staff : club->mStaffs) {
                                writer.Write(Utils::Format(L"            <staff name=\"%s\" id=\"%s\"",
                                    GetPersonName(staff), staff->GetStringUniqueId(Game::id())));
                                if (staff->mClubPosition == FifamClubStaffPosition::Manager)
                                    writer.Write(L" job=\"1\"");
                                else if (staff->mClubPosition == FifamClubStaffPosition::ChiefExec)
                                    writer.Write(L" job=\"2\"");
                                else if (staff->mClubPosition == FifamClubStaffPosition::President)
                                    writer.Write(L" job=\"3\"");
                                writer.WriteLine(L" />");
                            }
                            writer.WriteLine(L"        </club>");
                        }
                        else
                            writer.WriteLine(L" />");
                    }
                    writer.Write(Utils::Format(L"        <nationalteam id=\"%08X\"", country->mNationalTeam.mUniqueID));
                    if (country->mNationalTeam.mKit.mSpecialKitId)
                        writer.Write(Utils::Format(L" kit=\"%d\"", country->mNationalTeam.mKit.mSpecialKitId));
                    if (!country->mNationalTeam.mStaffs.empty()) {
                        writer.WriteLine(L" >");
                        std::sort(country->mNationalTeam.mStaffs.begin(), country->mNationalTeam.mStaffs.end(), [](FifamStaff *a, FifamStaff *b) {
                            return a->GetStringUniqueId(Game::id()) < b->GetStringUniqueId(Game::id());
                        });
                        for (auto staff : country->mNationalTeam.mStaffs) {
                            writer.Write(Utils::Format(L"            <staff name=\"%s\" id=\"%s\"",
                                GetPersonName(staff), staff->GetStringUniqueId(Game::id())));
                            if (staff->mClubPosition == FifamClubStaffPosition::Manager)
                                writer.Write(L" job=\"1\"");
                            else if (staff->mClubPosition == FifamClubStaffPosition::ChiefExec)
                                writer.Write(L" job=\"2\"");
                            else if (staff->mClubPosition == FifamClubStaffPosition::President)
                                writer.Write(L" job=\"3\"");
                            writer.WriteLine(L" />");
                        }
                        writer.WriteLine(L"        </nationalteam>");
                    }
                    else
                        writer.WriteLine(L" />");
                    std::sort(country->mReferees.begin(), country->mReferees.end(), [](FifamReferee *a, FifamReferee *b) {
                        String nameA = FifamNames::GetPersonStringId(Game::id(), a->mFirstName, a->mLastName, String(), FifamDate(), 0);
                        String nameB = FifamNames::GetPersonStringId(Game::id(), a->mFirstName, a->mLastName, String(), FifamDate(), 0);
                        return nameA < nameB;
                    });
                    for (auto referee : country->mReferees) {
                        writer.WriteLine(Utils::Format(L"        <referee name=\"%s\" id=\"%s\" />",
                            GetPersonName(referee->mLastName, referee->mFirstName, String()),
                            FifamNames::GetPersonStringId(Game::id(), referee->mFirstName, referee->mLastName, String(), FifamDate(), 0)));
                    }
                    Vector<FifamStaff *> countryStaffs;
                    for (auto staff : db->mStaffs) {
                        if (!staff->mClub && staff->mLinkedCountry.ToInt() == country->mId)
                            countryStaffs.push_back(staff);
                    }
                    std::sort(countryStaffs.begin(), countryStaffs.end(), [](FifamStaff *a, FifamStaff *b) {
                        return a->GetStringUniqueId(Game::id()) < b->GetStringUniqueId(Game::id());
                    });
                    for (auto staff : countryStaffs) {
                        writer.Write(Utils::Format(L"        <staff name=\"%s\" id=\"%s\"",
                            GetPersonName(staff), staff->GetStringUniqueId(Game::id())));
                        if (staff->mClubPosition == FifamClubStaffPosition::Manager)
                            writer.Write(L" job=\"1\"");
                        else if (staff->mClubPosition == FifamClubStaffPosition::ChiefExec)
                            writer.Write(L" job=\"2\"");
                        else if (staff->mClubPosition == FifamClubStaffPosition::President)
                            writer.Write(L" job=\"3\"");
                        writer.WriteLine(L" />");
                    }
                    for (auto comp : countryComps) {
                        if (comp.second->GetDbType() != FifamCompDbType::Pool) {
                            writer.Write(Utils::Format(L"        <competition name=\"%s\" id=\"%s\"", FifamTr(comp.second->mName), comp.first.ToHexStr()));
                            if (comp.second->GetDbType() == FifamCompDbType::Cup)
                                writer.Write(L" type=\"cup\"");
                            else if (comp.second->GetDbType() == FifamCompDbType::League) {
                                writer.Write(Utils::Format(L" type=\"league\" level=\"%d\"", comp.second->AsLeague()->mLeagueLevel));

                            }
                            else if (comp.second->GetDbType() == FifamCompDbType::Round)
                                writer.Write(Utils::Format(L" type=\"round\" round=\"%d\"", comp.second->AsRound()->mRoundType.ToInt()));
                            writer.WriteLine(L" />");
                        }
                    }
                    for (UInt l = 0; l < country->mLeagueLevelNames.size(); l++)
                        writer.WriteLine(Utils::Format(L"        <leaguelevel name=\"%s\" level=\"%d\" />", FifamTr(country->mLeagueLevelNames[l]), l));
                    writer.WriteLine(L"    </country>");
                }
            }
            Vector<FifamPlayer *> freeAgents;
            for (auto player : db->mPlayers) {
                if (!player->mClub)
                    freeAgents.push_back(player);
            }
            std::sort(freeAgents.begin(), freeAgents.end(), [](FifamPlayer *a, FifamPlayer *b) {
                return a->GetStringUniqueId(Game::id(), false) < b->GetStringUniqueId(Game::id(), false);
            });
            for (auto player : freeAgents) {
                writer.Write(Utils::Format(L"    <player name=\"%s\" id=\"%s\"",
                    GetPersonName(player), player->GetStringUniqueId(Game::id(), false)));
                if (player->mEmpicsId)
                    writer.Write(Utils::Format(L" empics=\"%d\"", player->mEmpicsId));
                writer.WriteLine(L"/>");
            }
            for (auto comp : db->mCompMap) {
                if (comp.second->mID.mRegion.ToInt() >= 249 && comp.second->GetDbType() != FifamCompDbType::Pool) {
                    writer.Write(Utils::Format(L"    <competition name=\"%s\" id=\"%s\"", FifamTr(comp.second->mName), comp.first.ToHexStr()));
                    if (comp.second->GetDbType() == FifamCompDbType::Cup)
                        writer.Write(L" type=\"cup\"");
                    else if (comp.second->GetDbType() == FifamCompDbType::League)
                        writer.Write(L" type=\"league\"");
                    else if (comp.second->GetDbType() == FifamCompDbType::Round)
                        writer.Write(Utils::Format(L" type=\"round\" round=\"%d\"", comp.second->AsRound()->mRoundType.ToInt()));
                    writer.WriteLine(L"/>");
                }
            }
            writer.WriteLine(L"</database>");
        }
    }
};
