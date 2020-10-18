#include "FifaLeague.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>

unsigned int FifaLeague::GetId() {
    return internal.leagueid;
}

bool FifaLeague::IsRestOfWorld() {
    return GetId() == 76;
}

bool FifaLeague::IsManInternational() {
    return GetId() == 78;
}

bool FifaLeague::IsWomanInternational() {
    return GetId() == 2136;
}

bool FifaLeague::IsInternational() {
    return IsManInternational() || IsWomanInternational();
}

bool FifaLeague::IsFreeAgents() {
    return GetId() == 382;
}

bool FifaLeague::IsNationalLeague() {
    return m_countryId != 0 && m_countryId != 75 && m_countryId != 209 && m_countryId != 210 && m_countryId != 211
        && m_countryId != 216 && m_countryId != 222;
}

void FifaLeague::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 11:
        line >> countryid >> leaguename >> level >> leagueid >> buildupplay;
        break;
    case 12:
        line >> countryid >> leaguename >> level >> leagueid >> buildupplay;
        break;
    case 13:
        line >> countryid >> leaguename >> level >> leagueid >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 14:
        line >> countryid >> leaguename >> level >> leagueid >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 15:
        line >> countryid >> leaguename >> level >> leagueid >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 16:
        line >> countryid >> leaguename >> level >> leagueid >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 17:
        line >> countryid >> leaguename >> level >> leagueid >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 18:
        line >> countryid >> leaguename >> level >> iscompetitionscarfenabled >> isbannerenabled >> leagueid >> iscompetitionpoleflagenabled >> iscompetitioncrowdcardsenabled >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 19:
        line >> countryid >> leaguename >> leaguetype >> level >> iscompetitionscarfenabled >> isbannerenabled >> leagueid >> iscompetitionpoleflagenabled >> iscompetitioncrowdcardsenabled >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 20:
        line >> countryid >> leaguename >> leaguetype >> level >> iscompetitionscarfenabled >> isbannerenabled >> leagueid >> iscompetitionpoleflagenabled >> iscompetitioncrowdcardsenabled >> leaguetimeslice >> iswithintransferwindow;
        break;
    case 21:
        line >> countryid >> leaguename >> leaguetype >> level >> iscompetitionscarfenabled >> isbannerenabled >> leagueid >> iscompetitionpoleflagenabled >> iscompetitioncrowdcardsenabled >> leaguetimeslice >> iswithintransferwindow;
        break;
    }
}

FifaLeague::FifaLeague(FifaDataFile::Line &line) {
    internal.Read(line);
    m_countryId = internal.countryid;
    m_name = internal.leaguename;
    m_champion = nullptr;
}
