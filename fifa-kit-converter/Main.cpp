#include "KitConverter.h"
#include "FifamDatabase.h"
#include <exception>

int main() {

    KitConverter kitConverter;

    UInt gameId = 14;

    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    FifamDatabase *db = new FifamDatabase(gameId, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", gameId));

    kitConverter.options.OutputGameId = gameId;
    kitConverter.options.ConvertGkKit = false;
    kitConverter.options.ConvertThirdKit = false;
    kitConverter.options.SaveLocation = gameId >= 9 ? KitConverter::Documents : KitConverter::User;

    for (auto country : db->mCountries) {
        if (country->mNationalTeam.mFifaID)
            kitConverter.ConvertClubKits(country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
        for (auto club : country->mClubs) {
            if (club->mFifaID)
                kitConverter.ConvertClubKits(club->mFifaID, club->mUniqueID);
        }
    }
}
