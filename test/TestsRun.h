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

void RunTests() {
    //RunTest<GeoCoordsTest>();
    RunTest<WriteDbClubsTest<FM07, Default>>();

    //RunTest<WriteDbCountriesTest<FM07, External>>();
    //RunTest<WriteDbCountriesTest<FM08, External>>();
    //RunTest<WriteDbCountriesTest<FM09, External>>();
    //RunTest<WriteDbCountriesTest<FM10, External>>();
    //RunTest<WriteDbCountriesTest<FM11, External>>();
    //RunTest<WriteDbCountriesTest<FM12, External>>();
    //RunTest<WriteDbCountriesTest<FM13, External>>();
    //RunTest<WriteDbCountriesTest<FM14, External>>();
}
