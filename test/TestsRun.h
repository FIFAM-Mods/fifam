#include "Tests.h"
#include "tests\ShowDbVersion.h"
#include "tests\ShowPlayers.h"
#include "tests\ReadWriteTest.h"
#include "tests\FifamDbTest.h"

void RunTests() {
    //RunTest<ReadWriteTest>();
    RunTest<FifamDbTest<7, false>>();
}
