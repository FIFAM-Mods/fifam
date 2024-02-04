#include "Converter.h"
#include "FifamNames.h"
#include "FifamPlayerGenerator.h"

String Converter::FixPersonName(String const &name, UInt gameId) {
    String result;
    static Set<WideChar> restrictedChars = { L',', L'|', L':', L'<', L'>', L'/', L'\\', L'?', L'*', 0x327, 0x301, 0x308, 0x30C, 0x200E };
    for (auto c : name) {
        if (!restrictedChars.contains(c)) {
            if (c == L'Ị')
                result += L'I';
            else if (c == L'ị')
                result += L'i';
            else if (c == L'Ọ')
                result += L'O';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'þ')
                result += L"th";
            else if (c == L'Ə')
                result += L'A';
            else if (c == L'ə')
                result += L'a';
            else if (c == L'ü')
                result += L'u';
            else if (c == L'ứ')
                result += L'u';
            else if (c == L'Ș')
                result += L'S';
            else if (c == L'ș')
                result += L's';
            else if (c == L'ë')
                result += L'e';
            else if (c == L'с')
                result += L'c';
            else if (c == L'´')
                result += L'\'';
            else if (gameId >= 13)
                result += c;
            else {
                if (c == L'Ț')
                    result += L"T";
                else if (c == L'ț')
                    result += L"t";
                else if (c == L'ð')
                    result += L'd';
                else
                    result += c;
            }
        }
    }
    return result;
}

void Converter::ConvertPersonAttributes(FifamPerson * person, foom::person * p, UInt gameId) {
    p->mConverterData.mFifamPerson = person;
    person->mCreator = 2;
    person->mFootballManagerID = p->mID;
    FifamCountry *personCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    // names
    person->mFirstName = FifamNames::LimitPersonName(FixPersonName(p->mFirstName, gameId), 15);
    person->mLastName = FifamNames::LimitPersonName(FixPersonName(p->mSecondName, gameId), 19);
    if (!p->mCommonName.empty())
        person->mPseudonym = FifamNames::LimitPersonName(FixPersonName(p->mCommonName, gameId), (mCurrentGameId > 7) ? 29 : 19);
    // nationality
    static Map<Int, Int> nationInfoToPriority = {
        { 83, 2010  },
        { 89, 2000  },
        { 80, 70    },
        { 85, 60    },
        { 0 , 50    },
        { -1, 40    },
        { 87, 30    },
        { 81, 20    },
        { 84, 10    },
        { 88, -1010 },
        { 90, -1020 },
        { 82, -1030 },
        { 86, -1040 }
    };
    auto PriorityForNation = [&](Int info, UInt number) {
        Int result = nationInfoToPriority.contains(info) ? nationInfoToPriority[info] : nationInfoToPriority[-1];
        if (number == 0)
            result += 1000;
        else
            result += Utils::Min(number, 9u);
        return result;
    };
    Vector<Pair<UInt, Int>> playerNations;
    if (p->mNation) {
        Int nationInfo = -1;
        if (p->mIsPlayer)
            nationInfo = ((foom::player *)p)->mNationalityInfo;
        playerNations.emplace_back(p->mNation->mConverterData.mFIFAManagerReplacementID, PriorityForNation(nationInfo, 0));
    }
    for (UInt i = 0; i < p->mVecSecondNations.size(); i++) {
        auto &s = p->mVecSecondNations[i];
        if (s.mNation->mConverterData.mFIFAManagerID != 0) {
            bool contains = false;
            for (auto const &e : playerNations) {
                if (e.first == s.mNation->mConverterData.mFIFAManagerID) {
                    contains = true;
                    break;
                }
            }
            if (!contains)
                playerNations.emplace_back(s.mNation->mConverterData.mFIFAManagerID, PriorityForNation(s.mInfo, i + 1));
        }
    }
    if (playerNations.empty())
        person->mNationality[0] = FifamNation::England;
    else if (playerNations.size() == 1)
        person->mNationality[0].SetFromInt(playerNations[0].first);
    else {
        std::sort(playerNations.begin(), playerNations.end(), [](Pair<UInt, Int> const &a, Pair<UInt, Int> const &b) {
            return a.second >= b.second;
        });
        person->mNationality[0].SetFromInt(playerNations[0].first);
        person->mNationality[1].SetFromInt(playerNations[1].first);
    }
    // languages
    Vector<Pair<UInt, UInt>> playerLanguages;
    if (p->mLanguage && p->mLanguage->mConverterData.mFIFAManagerID != 0)
        playerLanguages.emplace_back(p->mLanguage->mConverterData.mFIFAManagerID, 99);
    else
        playerLanguages.emplace_back(personCountry->mLanguages[0].ToInt(), 99);
    for (auto &l : p->mVecLanguages) {
        if (l.mLanguage && l.mProficiency >= 5 && !l.mCannotSpeakLanguage && l.mLanguage->mConverterData.mFIFAManagerID != 0) {
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
    return gameId >= 13 || !p->mGender;
}

Bool Converter::IsConvertable(foom::official * o, UInt gameId) {
    return gameId >= 13 || !o->mGender;
}
