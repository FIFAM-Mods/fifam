#include "FifamPlayerGenerator.h"
#include "Random.h"
#include "Error.h"

FifamDate FifamPlayerGenerator::GetRandomPlayerBirthday(UShort currentYear) {
    static UChar daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    UShort year = Random::Get(currentYear - 28, currentYear - 22);
    UChar month = Random::Get(1, 12);
    UChar day = 1;
    if (month < 1 || month > 12)
        Error("FifamPlayerGenerator::GetRandomPlayerBirthday(): invalid Random::Get() result");
    else
        day = Random::Get(1, daysInMonth[month - 1]);
    return FifamDate(day, month, year);
}
