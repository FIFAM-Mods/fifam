#include "FifamHistoric.h"
#include "FifamUtils.h"
#include "FifamClub.h"

void FifamHistoric::Read(Path &historicFolder, UInt gameId, FifamDatabase *database) {
    Bool unicode = gameId >= 8;

    if (gameId >= 13) {
        FifamReader fifaWorldPlayersReader(historicFolder / L"FifaWorldPlayers.txt", gameId, unicode);
        if (fifaWorldPlayersReader.Available()) {
            fifaWorldPlayersReader.SkipLine();
            while (!fifaWorldPlayersReader.IsEof()) {
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

    FifamWriter fifaWorldPlayersWriter(historicFolder / L"FifaWorldPlayers.txt", gameId, vYear, vNumber, unicode);
    if (fifaWorldPlayersWriter.Available()) {
        fifaWorldPlayersWriter.WriteLine(L"Saison	Nat.	Verein	Alter	ID	Name/Pseudonym	Vorname");
        for (auto const &entry : mFifaWorldPlayers) {
            UInt clubUID = 0;
            if (entry.mClub.IsValid())
                clubUID = FifamUtils::DBTranslateClubID(database, entry.mClub.mPtr->mUniqueID, gameId);
            fifaWorldPlayersWriter.Write(
                entry.mSeason,
                entry.mNationality,
                clubUID, 
                (entry.mPlayerPic.empty() ? L"0" : entry.mPlayerPic),
                entry.mLastNameOrPseudonym,
                (entry.mFirstName.empty() ? L"-" : entry.mFirstName)
            );
            fifaWorldPlayersWriter.WriteLine(L",");
        }
    }
}
