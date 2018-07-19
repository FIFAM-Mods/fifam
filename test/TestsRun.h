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
    RunTest<WriteDbTest<FM07, Default>>();
}
