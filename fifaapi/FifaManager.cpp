#include "FifaManager.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>
#include "Utils.h"

unsigned int FifaManager::GetId() {
    return internal.managerid;
}

FifaManager *FifaManager::RawPtr() {
    return this;
}

void FifaManager::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 18:
        line >> firstname >> surname >> managerid >> headid >> height >> hashighqualityhead >> gender >> suitvariationid >> teamid >> suittypeid >> eyecolorcode >> headclasscode >> skintonecode >> bodytypecode;
        break;
    case 19:
        line >> firstname >> surname >> managerid >> headid >> height >> hashighqualityhead >> gender >> suitvariationid >> faceposerpreset >> teamid >> suittypeid >> eyecolorcode >> headclasscode >> skintonecode >> bodytypecode;
        break;
    }
}

bool FifaManager::HasRealName() {
    return m_firstName.length() > 0 || m_lastName.length() > 0;
}

FifaManager::FifaManager(FifaDataFile::Line &line) {
    internal.Read(line);
    m_firstName = internal.firstname;
    m_lastName = internal.surname;
    m_quickName = Utils::GetQuickName(m_firstName, m_lastName, L"");
    m_team = nullptr;
}

bool FifaManager::IsMale() {
    return internal.gender == 0;
}
