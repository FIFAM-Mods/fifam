#include "FifamCountryAppearanceData.h"

void FifamCountryAppearanceData::Read(FifamReader &reader) {
    reader.ReadLine(mAppearanceType);
    reader.ReadLine(mDistributionProbability);
}

void FifamCountryAppearanceData::Write(FifamWriter &writer) {
    writer.WriteLine(mAppearanceType);
    writer.WriteLine(mDistributionProbability);
}
