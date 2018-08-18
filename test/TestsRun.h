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

void RunTests() {
    //RunTest<ReadWriteTest>();
    //RunTest<WriteDbClubsTest<FM07, External>>();
    //RunTest<WriteDbClubsTest<FM08, External>>();
    //RunTest<WriteDbClubsTest<FM09, External>>();
    //RunTest<WriteDbClubsTest<FM10, External>>();
    //RunTest<WriteDbClubsTest<FM11, External>>();
    //RunTest<WriteDbClubsTest<FM12, External>>();
    //RunTest<WriteDbClubsTest<FM13, External>>();
    //RunTest<WriteDbClubsTest<FM14, External>>();
    //RunTest<RulesTest<FM14, External>>();
    //RunTest<WriteDbTest<FM14, External>>();
    RunTest<WritePlayerHistoryTest<FM14, External>>();
}
