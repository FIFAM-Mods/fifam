#include "FifaAgent.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>
#include "Utils.h"

unsigned int FifaAgent::GetId() {
    return internal.agentid;
}

void FifaAgent::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 23:
    case 24:
        line >> firstname >> commonname >> surname >> skintypecode >> haircolorcode >> facialhairtypecode >> hairtypecode >> agentid >> headtypecode >> height >> seasonaloutfitid >> weight >> hashighqualityhead >> gender >> headassetid >> ethnicity >> faceposerpreset >> eyebrowcode >> eyecolorcode >> personalityid >> headclasscode >> nationality >> sideburnscode >> headvariation >> skintonecode >> outfitid >> hairstylecode >> bodytypecode >> facialhaircolorcode;
        break;
    }
}

FifaAgent::FifaAgent(FifaDataFile::Line &line) {
    internal.Read(line);
    m_firstName = internal.firstname;
    m_lastName = internal.surname;
    m_commonName = internal.commonname;
    m_quickName = Utils::GetQuickName(m_firstName, m_lastName, m_commonName);
}

bool FifaAgent::IsMale() {
    return internal.gender == 0;
}
