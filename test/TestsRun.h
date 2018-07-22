#include "Tests.h"
#include "tests\ShowDbVersion.h"
#include "tests\ShowPlayers.h"
#include "tests\ReadWriteTest.h"
#include "tests\FlagsTest.h"
#include "tests\WriteCountries.h"
#include "tests\WriteDbTest.h"

void RunTests() {
    //RunTest<ReadWriteTest>();
    //RunTest<FlagsTest<FM07, Default>>();
    //RunTest<WriteCountries<FM13, Default>>();

    //RunTest<WriteDbTest<FM07, External>>();
    //RunTest<WriteDbTest<FM08, External>>();
    //RunTest<WriteDbTest<FM09, External>>();
    //RunTest<WriteDbTest<FM10, External>>();
    //RunTest<WriteDbTest<FM11, External>>();
    //RunTest<WriteDbTest<FM12, External>>();
    //RunTest<WriteDbTest<FM13, External>>();
    RunTest<WriteDbTest<FM14, External>>();
}
