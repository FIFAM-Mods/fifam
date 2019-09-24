#include "KitConverter.h"
#include "FifamDatabase.h"
#include "FifamNames.h"
#include <exception>

int main() {

    UInt gameId = 13;

    KitConverter::options.OutputGameId = gameId;
    KitConverter::options.ConvertHomeKit = true;
    KitConverter::options.ConverAwayKit = true;
    KitConverter::options.ConvertGkKit = true;
    KitConverter::options.ConvertThirdKit = true;
    KitConverter::options.SaveLocation = KitConverter::Documents;
    KitConverter::options.ConvertMinikits = true;
    KitConverter::options.OnlyCustomKits = true;
    KitConverter::options.AllowCustomKits = true;
    KitConverter::options.Allow2xSize = false;
    KitConverter::options.Force2x = false;
    KitConverter::options.V2 = true;

    KitConverter kitConverter;

    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    FifamDatabase *db = new FifamDatabase(gameId, Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database text", gameId));

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

    //kitConverter.options.Overlay = true;
    //for (int i = 7001; i <= 7008; i++) 
    //    kitConverter.ConvertFifaClubKit(i, std::to_string(i), 5, 0, "D:\\Projects\\FIFA19\\kitoverlay\\" + std::to_string(i));
    //
    //return 0;

    //kitConverter.ConvertClubKits("00150006", 0, 0x00150006);

    for (auto country : db->mCountries) {
        if (country) {
            kitConverter.ConvertClubKits(GetClubIdName(FifamTr(country->mName)), country->mNationalTeam.mFifaID, country->mNationalTeam.mUniqueID);
            for (auto club : country->mClubs) {
                kitConverter.ConvertClubKits(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
                //kitConverter.ConvertClubArmbands(GetClubIdName(FifamTr(club->mName)), club->mFifaID, club->mUniqueID);
            }
        }
    }

    //kitConverter.ConvertClubKitNumbersCustom();
    //
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
