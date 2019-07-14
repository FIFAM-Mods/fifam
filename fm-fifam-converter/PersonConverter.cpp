#include "Converter.h"
#include "FifamNames.h"
#include "FifamPlayerGenerator.h"

void Converter::FixPersonName(String &name) {
    String originalName = name;
    name.clear();
    for (auto c : originalName) {
        if (c != L',' && c != '|') {
            if (c == L'ș')
                c = L's';
            name += c;
        }
    }
}

void Converter::ConvertPersonAttributes(FifamPerson * person, foom::person * p) {
    p->mConverterData.mFifamPerson = person;
    FifamCountry *personCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    // names
    person->mFirstName = FifamNames::LimitPersonName(p->mFirstName, 15);
    person->mLastName = FifamNames::LimitPersonName(p->mSecondName, 19);
    if (!p->mCommonName.empty())
        person->mPseudonym = FifamNames::LimitPersonName(p->mCommonName, (mCurrentGameId > 7) ? 29 : 19);
    FixPersonName(person->mFirstName);
    FixPersonName(person->mLastName);
    FixPersonName(person->mPseudonym);
    // nationality
    Vector<UInt> playerNations;
    if (p->mNation)
        playerNations.push_back(p->mNation->mConverterData.mFIFAManagerReplacementID);
    for (auto &n : p->mVecSecondNations) {
        if (n->mConverterData.mFIFAManagerID != 0 && !Utils::Contains(playerNations, n->mConverterData.mFIFAManagerID))
            playerNations.push_back(n->mConverterData.mFIFAManagerID);
    }
    if (playerNations.empty())
        person->mNationality[0] = FifamNation::England;
    else {
        person->mNationality[0].SetFromInt(playerNations[0]);
        if (playerNations.size() > 1)
            person->mNationality[1].SetFromInt(playerNations[1]);
    }
    // languages
    Vector<Pair<UInt, UInt>> playerLanguages;
    if (p->mLanguage && p->mLanguage->mConverterData.mFIFAManagerID != 0)
        playerLanguages.emplace_back(p->mLanguage->mConverterData.mFIFAManagerID, 99);
    else
        playerLanguages.emplace_back(personCountry->mLanguages[0].ToInt(), 99);
    for (auto &l : p->mVecLanguages) {
        if (l.mProficiency >= 5 && !l.mCannotSpeakLanguage && l.mLanguage->mConverterData.mFIFAManagerID != 0) {
            if (std::count_if(playerLanguages.begin(), playerLanguages.end(), [&](Pair<UInt, UInt> const &a) {
                return a.first == l.mLanguage->mConverterData.mFIFAManagerID;
                }) == 0)
            {
                playerLanguages.emplace_back(l.mLanguage->mConverterData.mFIFAManagerID, l.mProficiency);
            }
        }
    }
    std::sort(playerLanguages.begin(), playerLanguages.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
        return a.second > b.second;
        });
    UInt numLanguages = Utils::Min(4u, playerLanguages.size());
    for (UInt i = 0; i < numLanguages; i++)
        person->mLanguages[i].SetFromInt(playerLanguages[i].first);
    // birthdate
    if (p->mDateOfBirth > FmEmptyDate())
        person->mBirthday = p->mDateOfBirth;
    else
        person->mBirthday = FifamPlayerGenerator::GetRandomPlayerBirthday(CURRENT_YEAR);
}

Bool Converter::IsConvertable(foom::person * p, UInt gameId) {
    return gameId >= 13 || !p->mFemale;
}

Bool Converter::IsConvertable(foom::official * o, UInt gameId) {
    return gameId >= 13 || !o->mFemale;
}
