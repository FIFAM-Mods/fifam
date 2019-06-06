#include "KitConverter.h"
#include "FifamDatabase.h"
#include "FifamNames.h"
#include <exception>

int main() {

    KitConverter kitConverter;

    UInt gameId = 13;

    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    //FifamDatabase *db = new FifamDatabase(gameId, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database", gameId));

    kitConverter.options.OutputGameId = gameId;
    kitConverter.options.ConvertHomeKit = true;
    kitConverter.options.ConverAwayKit = false;
    kitConverter.options.ConvertGkKit = false;
    kitConverter.options.ConvertThirdKit = false;
    kitConverter.options.SaveLocation = KitConverter::Documents;
    kitConverter.options.ConvertMinikits = false;
    kitConverter.options.OnlyCustomKits = true;
    kitConverter.options.AllowCustomKits = true;
    kitConverter.options.Allow2xSize = false;
    kitConverter.options.V2 = true;

    auto GetClubIdName = [](String const &clubName) {
        String clubNameAscii = Utils::GetStringWithoutUnicodeChars(clubName);
        std::string result;
        for (auto c : clubNameAscii) {
            if (
                c != '<' &&
                c != '>' &&
                c != ':' &&
                c != '"' &&
                c != '/' &&
                c != '\\' &&
                c != '|' &&
                c != '?' &&
                c != '*' &&
                c != ' '
                )
            {
                result += static_cast<char>(c);
            }
        }
        return result;
    };

    kitConverter.ConvertClubKits("", 0, 0x150006);

    //for (auto country : db->mCountries) {
    //    if (country) {
    //        kitConverter.ConvertClubKits(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
    //        for (auto club : country->mClubs)
    //            kitConverter.ConvertClubKits(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
    //    }
    //}

    //for (auto country : db->mCountries) {
    //    if (country) {
    //        kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
    //        for (auto club : country->mClubs)
    //            kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
    //    }
    //}

    //for (auto country : db->mCountries) {
    //    if (country) {
    //        kitConverter.ConvertClubKitNumbers(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
    //        for (auto club : country->mClubs)
    //            kitConverter.ConvertClubKitNumbers(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
    //    }
    //}

    //kitConverter.ConvertClubKitNumbersCustom();
}
