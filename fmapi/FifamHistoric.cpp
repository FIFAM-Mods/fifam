#include "FifamHistoric.h"
#include "FifamUtils.h"
#include "FifamClub.h"
#include "FifamDatabase.h"

void FifamHistoric::Read(Path &historicFolder, UInt gameId) {
    if (gameId >= 9) {
        FifamReader worstStartingStreakReader(historicFolder / L"WorstStartingStreak.txt", gameId);
        if (worstStartingStreakReader.Available()) {
            while (!worstStartingStreakReader.IsEof()) {
                if (worstStartingStreakReader.CheckLine(L"", true))
                    continue;
                FifamWorstStartingStreak &entry = mWorstStartingStreaks.emplace_back();
                UInt compIDInt = 0;
                UInt clubUID = 0;
                worstStartingStreakReader.ReadLine(compIDInt, Hexadecimal(clubUID), entry.mSeason, entry.mMatches);
                entry.mCompetition.SetFromInt(compIDInt);
                FifamUtils::SaveClubIDToClubLink(entry.mClub, clubUID);
            }
        }
    }

    if (gameId >= 13) {
        FifamReader fifaWorldPlayersReader(historicFolder / L"FifaWorldPlayers.txt", gameId);
        if (fifaWorldPlayersReader.Available()) {
            fifaWorldPlayersReader.SkipLine();
            while (!fifaWorldPlayersReader.IsEof()) {
                if (fifaWorldPlayersReader.CheckLine(L"", true))
                    continue;
                FifamFifaWorldPlayer &entry = mFifaWorldPlayers.emplace_back();
                UInt clubUID = 0;
                fifaWorldPlayersReader.ReadLine(entry.mSeason, entry.mNationality, clubUID, entry.mAge, entry.mPlayerPic,
                    entry.mLastNameOrPseudonym, entry.mFirstName);
                if (entry.mPlayerPic == L"0")
                    entry.mPlayerPic.clear();
                if (entry.mFirstName == L"-")
                    entry.mFirstName.clear();
                FifamUtils::SaveClubIDToClubLink(entry.mClub, clubUID);
            }
        }
    }
}

void FifamHistoric::Write(Path &historicFolder, UInt gameId, FifamVersion const &version) {
    Bool unicode = gameId >= 8;

    if (gameId >= 9) {
        FifamWriter worstStartingStreakWriter(historicFolder / L"WorstStartingStreak.txt", gameId, version, unicode);
        if (worstStartingStreakWriter.Available()) {
            for (auto const &entry : mWorstStartingStreaks) {
                UInt compIDInt = FifamUtils::GetWriteableID(entry.mCompetition, gameId);
                UInt clubUID = FifamUtils::GetWriteableUniqueID(entry.mClub);
                worstStartingStreakWriter.WriteLine(compIDInt, Hexadecimal(clubUID), entry.mSeason, entry.mMatches);
            }
        }
    }

    if (gameId >= 13) {
        FifamWriter fifaWorldPlayersWriter(historicFolder / L"FifaWorldPlayers.txt", gameId, version, unicode);
        if (fifaWorldPlayersWriter.Available()) {
            fifaWorldPlayersWriter.WriteLine(L"Saison	Nat.	Verein	Alter	ID	Name/Pseudonym	Vorname");
            for (auto const &entry : mFifaWorldPlayers) {
                fifaWorldPlayersWriter.Write(entry.mSeason, L"\t");
                fifaWorldPlayersWriter.Write(entry.mNationality, L"\t");
                fifaWorldPlayersWriter.Write(FifamUtils::GetWriteableUniqueID(entry.mClub), L"\t");
                fifaWorldPlayersWriter.Write(entry.mPlayerPic.empty() ? L"0" : entry.mPlayerPic, L"\t");
                fifaWorldPlayersWriter.Write(entry.mLastNameOrPseudonym, L"\t");
                fifaWorldPlayersWriter.Write(entry.mFirstName.empty() ? L"-" : entry.mFirstName);
                fifaWorldPlayersWriter.WriteLine(L",");
            }
        }
    }
}
