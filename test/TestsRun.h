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
#include "Translation.h"
#include "tests\GetUnlinkedFaces.h"
#include "tests\UpdateDbNames.h"
#include "tests\Fifa07Convert.h"
#include "tests\Fifa07Kits.h"

void RunTests() {
    RunTest<Fifa07Kits>();
}
