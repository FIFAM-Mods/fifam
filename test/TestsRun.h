#include "Tests.h"
#include "tests\ShowDbVersion.h"
#include "tests\ShowPlayers.h"
#include "tests\ReadWriteTest.h"
#include "tests\FlagsTest.h"
#include "tests\WriteDbCountriesTest.h"
#include "tests\WriteDbClubsTest.h"
#include "tests\ColorsTest.h"
#include "tests\BadgeTest.h"
#include "tests\GeoCoordsTest.h"
#include "tests\CompIDTest.h"
#include "tests\WriteDbTest.h"
#include "tests\RulesTest.h"
#include "tests\WritePlayerHistoryTest.h"
#include "tests\WritePlayerContractTest.h"
#include "tests\WritePlayersTest.h"
#include "tests\HistoricTest.h"
#include "tests\CompetitionsTest.h"
#include "tests\CreateFlagImages.h"
#include "tests\FifaConverter.h"
#include "tests\FoomBasicTest.h"
#include "tests\PlayerRelationsTest.h"
#include "tests\WritePlayerIDs.h"
#include "tests\CalendarDatabase.h"
#include "tests\ExportXML.h"
#include "tests\ExportCompInfo.h"
#include "tests\GenerateKitColors.h"
#include "tests\ExportClubIDs.h"
#include "tests\GenerateKitConfig.h"
#include "tests\CopyPortraitsLight.h"
#include "tests\ExportPlayerAccessories.h"
#include "tests\Parameters.h"
#include "tests\DbConvert.h"
#include "tests\ExportJapanLeaguePlayers.h"
#include "tests\MissingPortraits.h"
#include "tests\DatabaseRewrite.h"
#include "tests\CreateStarheadsFile.h"
#include "tests\Translation.h"
#include "tests\GetUnlinkedFaces.h"
#include "tests\UpdateDbNames.h"
#include "tests\Fifa07Convert.h"
#include "tests\Fifa07Kits.h"
#include "tests\RenameCityPictures.h"
#include "tests\BadgesFM07toFIFA07.h"
#include "tests\CompareDB.h"
#include "tests\DbGraphicsStats.h"
#include "tests\WriteDbTest.h"
#include "tests\Fifa07Portraits.h"
#include "tests\PlayerIdTest.h"
#include "tests\FifaCommentaryFile.h"
#include "tests\TeamColors.h"
#include "tests\DatesTest.h"
#include "tests\DbCommands.h"
#include "tests\PngFix.h"
#include "tests\ParseTransfermarktStats.h"
#include "tests\PackMusic.h"
#include "tests\PackABK.h"
#include "tests\FM07toFIFA07.h"
#include "tests\TranslationStrip.h"
#include "tests\FilesToFolders.h"
#include "tests\Fm07DbFix.h"
#include "tests\FifaXXlPortraits.h"
#include "tests\AdboardsFIFA07.h"
#include "tests\WriteBadgesLaLiga.h"
#include "tests\TranslationHashes.h"
#include "tests\WriteFifaPlayerNames.h"
#include "tests\CopyPortraitsFromPreviousPatch.h"
#include "tests\ClubNames.h"
#include "tests\FixScoutCountries.h"
#include "tests\FixCompetitionBadges.h"
#include "tests\CreateWorldCupDatabase.h"
#include "tests\FixCityLocations.h"
#include "tests\CopyTeamKits.h"
#include "tests\TrToTxt.h"
#include "tests\RenameHeadFiles.h"
#include "tests\UpdateKitConfig.h"
#include "tests\CopyPlayers.h"
#include "tests\CopyPortraitsNoID.h"
#include "tests\AssignPlayerFaces.h"
#include "tests\PlayerNames.h"

void RunTests() {
    RunTest<PlayerNames>();
}
