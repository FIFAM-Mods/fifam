#include "Tests.h"
#include "tests\ShowDbVersion.h"
#include "tests\ShowPlayers.h"
#include "tests\ReadWriteTest.h"
#include "tests\FlagsTest.h"
#include "tests\WriteCountries.h"
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
#include "tests\FifaWorldPlayersTest.h"

void RunTests() {
    RunTest<FifaWorldPlayersTest<FM14, External>>();
}
