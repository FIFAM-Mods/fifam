#include "Converter.h"
#include "FifamNames.h"
#include "FifamPlayerGenerator.h"

String Converter::FixPersonName(String const &name, UInt gameId) {
    String result;
    for (auto c : name) {
        if (c != L',' && c != '|') {
            if (gameId >= 13) {
                if (c == L'ș')
                    result += L"s";
                else
                    result += c;
            }
            else {
                if (c == L'ș')
                    result += L"s";
                else if (c == L'þ')
                    result += L"th";
                else if (c == L'Ț')
                    result += L"T";
                else if (c == L'ț')
                    result += L"t";
                else if (c == L'Ș')
                    result += L'S';
                else if (c == L'ð')
                    result += L'o';
                else
                    result += c;
            }
        }
    }
    return result;
}

void Converter::ConvertPersonAttributes(FifamPerson * person, foom::person * p, UInt gameId) {
    p->mConverterData.mFifamPerson = person;
    FifamCountry *personCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    // names
    person->mFirstName = FifamNames::LimitPersonName(FixPersonName(p->mFirstName, gameId), 15);
    person->mLastName = FifamNames::LimitPersonName(FixPersonName(p->mSecondName, gameId), 19);
    if (!p->mCommonName.empty())
        person->mPseudonym = FifamNames::LimitPersonName(FixPersonName(p->mCommonName, gameId), (mCurrentGameId > 7) ? 29 : 19);
    
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
