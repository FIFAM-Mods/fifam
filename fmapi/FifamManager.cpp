#include "FifamManager.h"

void FifamManager::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"MANAGER")) {
        reader.ReadFullLine(mFirstName);
        reader.ReadFullLine(mLastName);
        reader.ReadFullLine(mPseudonym);
        reader.ReadLine(mNationality);
        reader.ReadLine(mBirthDay);
        reader.ReadLine(mMotivationSkills);
        reader.ReadLine(mCoachingSkills);
        reader.ReadLine(mGoalkeepersTraining);
        reader.ReadLine(mNegotiationSkills);
        reader.ReadLine(mLanguage[0]);
        reader.ReadLine(mLanguage[1]);
        reader.ReadLine(mLanguage[2]);
        reader.ReadLine(mLanguage[3]);
        reader.ReadLine(Unknown._1);
        reader.ReadLine(Unknown._2);
        reader.ReadEndIndex(L"MANAGER");
    }
}

void FifamManager::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"MANAGER");
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    writer.WriteLine(mPseudonym);
    writer.WriteLine(mNationality);
    writer.WriteLine(mBirthDay);
    writer.WriteLine(mMotivationSkills);
    writer.WriteLine(mCoachingSkills);
    writer.WriteLine(mGoalkeepersTraining);
    writer.WriteLine(mNegotiationSkills);
    writer.WriteLine(mLanguage[0]);
    writer.WriteLine(mLanguage[1]);
    writer.WriteLine(mLanguage[2]);
    writer.WriteLine(mLanguage[3]);
    writer.WriteLine(Unknown._1);
    writer.WriteLine(Unknown._2);
    writer.WriteEndIndex(L"MANAGER");
}
