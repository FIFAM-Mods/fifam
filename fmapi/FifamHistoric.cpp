#include "FifamHistoric.h"
#include "FifamUtils.h"
#include "FifamClub.h"
#include "FifamDatabase.h"

void FifamHistoric::Read(Path &historicFolder, UInt gameId, FifamDatabase *database) {
    Bool unicode = gameId >= 8;

    if (gameId >= 9) {
        FifamReader worstStartingStreakReader(historicFolder / L"WorstStartingStreak.txt", gameId, unicode);
        if (worstStartingStreakReader.Available()) {
            while (!worstStartingStreakReader.IsEof()) {
                if (worstStartingStreakReader.CheckLine(L"", true))
                    continue;
                FifamWorstStartingStreak &entry = mWorstStartingStreaks.emplace_back();
                UInt compIDInt = 0;
                UInt clubUID = 0;
                worstStartingStreakReader.ReadLine(compIDInt, Hexademical(clubUID), entry.mSeason, entry.mMatches);
                entry.mCompID.SetFromInt(compIDInt);
                FifamUtils::SaveClubIDToClubLink(entry.mClub, clubUID);
            }
        }
    }

    if (gameId >= 13) {
        FifamReader fifaWorldPlayersReader(historicFolder / L"FifaWorldPlayers.txt", gameId, unicode);
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

void FifamHistoric::Write(Path &historicFolder, UInt gameId, UShort vYear, UShort vNumber, FifamDatabase *database) {
    Bool unicode = gameId >= 8;

    if (gameId >= 9) {
        FifamWriter worstStartingStreakWriter(historicFolder / L"WorstStartingStreak.txt", gameId, vYear, vNumber, unicode);
        if (worstStartingStreakWriter.Available()) {
            for (auto const &entry : mWorstStartingStreaks) {
                UInt compIDInt =
                    FifamCompID::Translate(entry.mCompID.ToInt(), FifamDatabase::LATEST_GAME_VERSION, gameId);
                UInt clubUID = FifamUtils::DBTranslateClubID(database, entry.mClub.mPtr->mUniqueID, gameId);
                worstStartingStreakWriter.WriteLine(compIDInt, Hexademical(clubUID), entry.mSeason, entry.mMatches);
            }
        }
    }

    if (gameId >= 13) {
        FifamWriter fifaWorldPlayersWriter(historicFolder / L"FifaWorldPlayers.txt", gameId, vYear, vNumber, unicode);
        if (fifaWorldPlayersWriter.Available()) {
            fifaWorldPlayersWriter.WriteLine(L"Saison	Nat.	Verein	Alter	ID	Name/Pseudonym	Vorname");
            for (auto const &entry : mFifaWorldPlayers) {
                UInt clubUID = 0;
                if (entry.mClub.IsValid())
                    clubUID = FifamUtils::DBTranslateClubID(database, entry.mClub.mPtr->mUniqueID, gameId);
                fifaWorldPlayersWriter.Write(entry.mSeason, L"\t");
                fifaWorldPlayersWriter.Write(entry.mNationality, L"\t");
                fifaWorldPlayersWriter.Write(clubUID, L"\t");
                fifaWorldPlayersWriter.Write(entry.mPlayerPic.empty() ? L"0" : entry.mPlayerPic, L"\t");
                fifaWorldPlayersWriter.Write(entry.mLastNameOrPseudonym, L"\t");
                fifaWorldPlayersWriter.Write(entry.mFirstName.empty() ? L"-" : entry.mFirstName);
                fifaWorldPlayersWriter.WriteLine(L",");
            }
        }
    }
}
