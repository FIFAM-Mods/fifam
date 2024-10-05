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
    case 11:
        line >> birthdate >> firstname >> shoetypecode >> surname >> refereeid >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> lowmedhairtypeid >> stylecode >> proxyhaircolorid >> headtypecode >> height >> hairstateid >> leagueid >> sockheightcode >> weight >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> homecitycode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 12:
        line >> birthdate >> firstname >> shoetypecode >> surname >> refereeid >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> sockheightcode >> weight >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> homecitycode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 13:
        line >> birthdate >> firstname >> shoetypecode >> surname >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> sockheightcode >> weight >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> refereeid >> homecitycode >> shoedesigncode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 14:
        line >> birthdate >> firstname >> shoetypecode >> surname >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> sockheightcode >> weight >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> refereeid >> homecitycode >> shoedesigncode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 15:
        line >> firstname >> shoetypecode >> surname >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> birthdate >> sockheightcode >> weight >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> isreal >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> refereeid >> homecitycode >> shoedesigncode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 16:
        line >> firstname >> shoetypecode >> surname >> nationalitycode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> birthdate >> sockheightcode >> weight >> gender >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> isreal >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> refereeid >> homecitycode >> shoedesigncode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 17:
        line >> firstname >> shoetypecode >> surname >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> stylecode >> proxyhaircolorid >> headtypecode >> foulstrictness >> height >> hairstateid >> leagueid >> birthdate >> sockheightcode >> weight >> gender >> proxyheadclass >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> hairlinecode >> headclasscode >> haireffecttypecode >> sideburnscode >> skintypecode >> isreal >> hairpartcode >> sweatid >> skintonecode >> hairvariationid >> refereeid >> homecitycode >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> bodytypecode >> wrinkleid >> facialhaircolorcode;
        break;
    case 18:
        line >> firstname >> surname >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> sockheightcode >> socklengthcode >> weight >> gender >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> skintypecode >> isreal >> headvariation >> skintonecode >> shortstyle >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 19:
        line >> firstname >> surname >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> skintypecode >> isreal >> headvariation >> skintonecode >> shortstyle >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 20:
        line >> firstname >> surname >> skintypecode >> bodytypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> facialhaircolorcode;
        break;
    case 21:
        line >> firstname >> surname >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 22:
        line >> firstname >> surname >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 23:
        line >> firstname >> surname >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 24:
        line >> firstname >> surname >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> cardstrictness >> shoecolorcode2 >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> socklengthcode >> weight >> gender >> headassetid >> faceposerpreset >> eyebrowcode >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    case 25:
        line >> firstname >> surname >> eyebrowcode >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> lipcolor >> cardstrictness >> shoecolorcode2 >> skinsurfacepack >> headtypecode >> foulstrictness >> height >> leagueid >> shoetypecode >> birthdate >> skinmakeup >> socklengthcode >> weight >> eyedetail >> gender >> headassetid >> faceposerpreset >> eyecolorcode >> jerseysleevelengthcode >> headclasscode >> sideburnscode >> isreal >> headvariation >> skintonecode >> shortstyle >> smallsidedshoetypecode >> refereeid >> nationalitycode >> skincomplexion >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> facialhaircolorcode;
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
