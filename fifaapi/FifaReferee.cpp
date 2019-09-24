#include "FifaReferee.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>
#include "Utils.h"

unsigned int FifaReferee::GetId() {
    return internal.refereeid;
}

void FifaReferee::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 18:
        line >> firstname >> surname >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> sockheightcode >> socklengthcode >> weight >> gender >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> skintypecode >> isreal >> headvariation >> skintonecode >> shortstyle >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 19:
        line >> firstname >> surname >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> skintypecode >> isreal >> headvariation >> skintonecode >> shortstyle >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 20:
        line >> firstname >> surname >> skintypecode >> bodytypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> facialhaircolorcode;
        break;
    }
}

FifaReferee::FifaReferee(FifaDataFile::Line &line) {
    internal.Read(line);
    m_firstName = internal.firstname;
    m_lastName = internal.surname;
    m_quickName = Utils::GetQuickName(m_firstName, m_lastName, L"");
    m_mainLeague = nullptr;
}

bool FifaReferee::IsMale() {
    return internal.gender == 0;
}

bool FifaReferee::IsReal() {
    return internal.isreal == 1;
}
