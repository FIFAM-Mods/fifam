#include "Converter.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifamCompCup.h"
#include "FifamNames.h"
#include "FifamUtils.h"
#include "FifamPlayerGenerator.h"
#include "FifamPlayerLevel.h"
#include "Random.h"
#include "ConverterUtil.h"
#include "GraphicsConverter.h"
#include "FifaConverter.h"

#define DB_SIZE Full
const Bool READ_FOOM_PERSONS = true;

void Converter::ReadAdditionalInfo(Path const &infoPath, UInt gameId) {
    {
        std::wcout << L"Reading fifam_countries.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_countries.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::nation::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, FootballManagerID, info.mFIFAID, dummy, info.mFIFAManagerID, info.mFIFAManagerReplacementID, info.mFIFATeamID);
                    if (FootballManagerID != 0) {
                        if (gameId >= 11 && FootballManagerID == 217945) { // Kosovo
                            info.mFIFAManagerID = 207;
                            info.mFIFAManagerReplacementID = 207;
                            info.mFIFAID = 219;
                        }
                        //else if (gameId <= 7 && FootballManagerID == 62002127) { // Montenegro
                        //    info.mFIFAManagerID = 207;
                        //    info.mFIFAManagerReplacementID = 207;
                        //    info.mFIFAID = 15;
                        //}
                        map_find(mFoomDatabase->mNations, FootballManagerID).mConverterData = info;
                        FifaConverter::mFifaToFifamCountryId[info.mFIFAID] = { info.mFIFAManagerID, info.mFIFAManagerReplacementID };
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam-uids.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam-uids.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            Map<UInt, UInt> uidsMap;
            Map<UInt, UInt> uniqueUIDsMap;
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::club::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, dummy, FootballManagerID, Hexadecimal(info.mFIFAManagerID), info.mFIFAID);
                    map_find(mFoomDatabase->mClubs, FootballManagerID).mConverterData = info;
                    uidsMap[FootballManagerID] = info.mFIFAManagerID;

                    if (info.mFIFAManagerID > 0) {
                        auto it = uniqueUIDsMap.find(info.mFIFAManagerID);
                        if (it != uniqueUIDsMap.end()) {
                            Error(L"Duplicated Unique ID: %08X\nClub1: %d\nClub2: %d", info.mFIFAManagerID, (*it).second, FootballManagerID);
                        }
                        else
                            uniqueUIDsMap[info.mFIFAManagerID] = FootballManagerID;
                    }
                }
                else
                    reader.SkipLine();
            }
            /*
            Array<UInt, FifamDatabase::NUM_COUNTRIES> maxUid = {};
            for (auto[foomId, fifamId] : uidsMap) {
                if (fifamId > 0) {
                    UChar region = (fifamId >> 16) & 0xFF;
                    if (region >= 1 && region <= 207) {
                        if (fifamId > maxUid[region - 1])
                            maxUid[region - 1] = fifamId;
                    }
                }
            }
            for (auto &e : uidsMap) {
                if (e.second == 0) {
                    foom::club *c = mFoomDatabase->get<foom::club>(e.first);
                    if (c) {
                        foom::nation *n = c->mNation;
                        if (!n)
                            n = c->mContinentalCupNation;
                        if (n) {
                            UInt countryId = n->mConverterData.mFIFAManagerID;
                            if (n->mID == 217945)
                                countryId = 207; // Kosovo
                            if (countryId > 0 && countryId <= FifamDatabase::NUM_COUNTRIES)
                                e.second = ++maxUid[countryId - 1];
                        }
                    }
                }
            }
            FifamWriter clubsUIDsWriter(L"out_club_uids.txt", 14, 0, 0);
            for (auto &e : uidsMap) {
                if (e.second == 0)
                    clubsUIDsWriter.WriteLine(Utils::Format(L"%u\t", e.first));
                else
                    clubsUIDsWriter.WriteLine(Utils::Format(L"%u\t%08X", e.first, e.second));
            }
            */
        }
    }
    {
        std::wcout << L"Reading fm_additional_reserve_teams_for_history.txt..." << std::endl;
        FifamReader reader(infoPath / L"fm_additional_reserve_teams_for_history.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int parentID = -1;
                    Int childID = -1;
                    Int childType = -1;
                    reader.ReadLineWithSeparator(L'\t', parentID, childID, childType);
                    if (parentID > 0 && childID > 0 && (childType == 0 || childType == 1)) {
                        foom::club *parentClub = mFoomDatabase->get<foom::club>(parentID);
                        if (parentClub) {
                            foom::club *childClub = mFoomDatabase->get<foom::club>(childID);
                            if (childClub) {
                                childClub->mConverterData.mAdditionalHistoryParentTeam = parentClub;
                                childClub->mConverterData.mAdditionalHistoryReserveTeamType = childType;
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_languages.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_languages.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::language::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mLanguages, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_regions.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_regions.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::region::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mRegions, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_injuries.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_injuries.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    foom::injury::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(FootballManagerID, info.mFIFAManagerID);
                    map_find(mFoomDatabase->mInjuries, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_divisions.txt..." << std::endl;
        FifamReader reader(infoPath / (gameId > 7 ? L"fifam_divisions.txt" : L"fifam_divisions_07.txt"), 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    DivisionInfo d;
                    String e, type, sorting;
                    reader.ReadLineWithSeparator(L'\t', e, d.mNationID, d.mName, d.mShortName, OptionalInt(d.mID), type, d.mLevel, e, e,
                        d.mTeams, d.mRep, d.mPriority, d.mOrder, OptionalInt(d.mRounds), d.mPromoted, d.mRelegated, d.mStartDate,
                        d.mEndDate, d.mWinterBreakStart, d.mWinterBreakEnd, OptionalInt(d.mNumSubs), OptionalInt(d.mForeignersLimit),
                        OptionalInt(d.mNonEuSigns), OptionalInt(d.mDomesticPlayers), OptionalInt(d.mU21Players),
                        OptionalInt(d.mReserveTeamsAllowed), sorting, d.mAttendanceMp, d.mTransfersMp, d.mTvBonus, d.mWinBouns, d.mPlaceBonus);
                    
                    type = Utils::ToLower(type);
                    if (type == L"league")
                        d.mType = DivisionInfo::League;
                    else if (type == L"level")
                        d.mType = DivisionInfo::Level;
                    else {
                        Error(L"Unknown DivisionInfo type: %s", type.c_str());
                        continue;
                    }

                    Utils::Trim(sorting);
                    sorting = Utils::ToLower(sorting);

                    if (sorting == L"dm")
                        d.mSorting = FifamEqualPointsSorting::DirectMatches;
                    else if (sorting == L"g")
                        d.mSorting = FifamEqualPointsSorting::Goals;
                    else
                        d.mSorting = FifamEqualPointsSorting::GoalDiff;

                    if (d.mLevel <= 0)
                        d.mLevel = 256;
                    else
                        d.mLevel -= 1;

                    mDivisions.push_back(d);
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_cups.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_cups.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    CupInfo c;
                    String strDummy, strType, strTemplateType;
                    reader.ReadLineWithSeparator(L'\t', strDummy, c.mNationID, c.mName, c.mShortName, c.mID, c.mLevel, strType, strDummy, c.mReputation, c.mPriority, strTemplateType, c.mRoundDesc[7], c.mRoundDesc[6], c.mRoundDesc[5], c.mRoundDesc[4], c.mRoundDesc[3], c.mRoundDesc[2], c.mRoundDesc[1], c.mRoundDesc[0], c.mStructure, c.mMaxLevel, c.mNumSubs, c.mBonus, c.mTvBonus, c.mStartDate, c.mEndDate);
                    if (c.mPriority < 1)
                        continue;
                    c.mTemplateType.SetFromStr(strTemplateType);
                    if (c.mTemplateType.GetWasSetFromUnknown()) {
                        Error(L"Unknown cup template type: %s\nCup name: %s\nCup ID: %d", strTemplateType.c_str(), c.mName.c_str(), c.mID);
                        continue;
                    }
                    if (c.mLevel < 0)
                        c.mLevel = 0;
                    if (strType == L"Domestic Main Cup")
                        c.mType = CupInfo::FA;
                    else if (strType == L"Super Cup")
                        c.mType = CupInfo::Supercup;
                    else
                        c.mType = CupInfo::League;
                    mCups.push_back(c);
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fixtures..." << std::endl;
        for (auto const &p : recursive_directory_iterator(infoPath / L"fixtures")) {
            if (is_regular_file(p.path()) && p.path().extension() == ".txt") {
                UInt leagueId = Utils::SafeConvertInt<UInt>(p.path().filename().c_str());
                if (leagueId > 0) {
                    Vector<FixtureInfo> &fixtures = mFixturesPerLeague[leagueId];
                    fixtures.clear();
                    FifamReader reader(p.path(), 0);
                    if (reader.Available()) {
                        while (!reader.IsEof()) {
                            if (!reader.EmptyLine()) {
                                FixtureInfo f;
                                String dateStr, dummy;
                                reader.ReadLineWithSeparator(L' ', dateStr, dummy, IntPtr(f.mTeam1), IntPtr(f.mTeam2));
                                mFoomDatabase->resolve(f.mTeam1);
                                mFoomDatabase->resolve(f.mTeam2);
                                auto dateParts = Utils::Split(dateStr, L'/', true, false);
                                if (dateParts.size() >= 3) {
                                    f.mDate.Set(Utils::SafeConvertInt<UInt>(dateParts[0]), Utils::SafeConvertInt<UInt>(dateParts[1]),
                                        Utils::SafeConvertInt<UInt>(dateParts[2]));
                                }
                                else
                                    Error(L"Wrong fixture date in fixture file\nFixture file: %s", p.path().filename().c_str());
                                fixtures.push_back(f);
                            }
                            else
                                reader.SkipLine();
                        }
                    }
                    reader.Close();
                }
            }
        }
    }
    if (gameId > 7) {
        std::wcout << L"Reading fm-fifa-players..." << std::endl;
        FifamReader reader(infoPath / L"fm-fifa-players.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    String foomId, fifaId, editorFace, editorHair, editorBeard, editorSkin, editorEye;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, dummy, dummy, dummy,
                        foomId, fifaId, editorSkin, editorFace, editorHair, editorBeard, editorEye);
                    if (!foomId.empty()) {
                        UInt playerId = Utils::SafeConvertInt<Int>(foomId);
                        if (playerId != -1) {
                            foom::player *foomPlayer = mFoomDatabase->get<foom::player>(playerId);
                            if (foomPlayer) {
                                auto SetPlayerParameter = [](auto &param, String const &str) {
                                    if (!str.empty()) {
                                        UInt paramId = Utils::SafeConvertInt<Int>(str);
                                        if (paramId != -1)
                                            param = paramId;
                                    }
                                };
                                SetPlayerParameter(foomPlayer->mConverterData.mFifaPlayerId, fifaId);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorFace, editorFace);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorHair, editorHair);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorBeard, editorBeard);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorSkin, editorSkin);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorEye, editorEye);
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_club_colors..." << std::endl;
        FifamReader reader(infoPath / L"fifam_club_colors.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int teamId = -1;
                    String colorStr;
                    reader.ReadLine(teamId, colorStr);
                    if (teamId != -1) {
                        colorStr = Utils::ToUpper(colorStr);
                        ClubColor color;
                        if (colorStr == L"KIT") {
                            color.type = ClubColor::KIT;
                            mClubColorsMap[teamId] = color;
                        }
                        else if (colorStr == L"TEAM") {
                            color.type = ClubColor::TEAM;
                            mClubColorsMap[teamId] = color;
                        }
                        else {
                            auto colors = Utils::Split(colorStr, L'-');
                            if (colors.size() == 2) {
                                bool failed = false;
                                for (UInt i = 0; i < 2; i++) {
                                    auto rgb = Utils::Split(colors[i], L',');
                                    if (rgb.size() != 3) {
                                        failed = true;
                                        break;
                                    }
                                    color.rgb[i].Set(Utils::SafeConvertInt<UChar>(rgb[0]),
                                        Utils::SafeConvertInt<UChar>(rgb[1]), Utils::SafeConvertInt<UChar>(rgb[2]));
                                }
                                if (!failed) {
                                    color.type = ClubColor::RGB;
                                    mClubColorsMap[teamId] = color;
                                }
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_penalty_points..." << std::endl;
        FifamReader reader(infoPath / L"fifam_penalty_points.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int teamId = -1;
                    Int points = 0;
                    reader.ReadLine(teamId, points);
                    if (teamId != -1 && points < 0)
                        mPenaltyPointsMap[teamId] = points;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_names..." << std::endl;
        FifamReader reader(infoPath / L"fifam_names.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int teamId = -1;
                    String longName, shortName, abbr;
                    String d;
                    reader.ReadLineWithSeparator(L'\t', d, d, d, d, d, d, d, teamId, longName, shortName, abbr);
                    if (teamId != -1) {
                        if (!longName.empty())
                            mNamesMap[teamId] = longName;
                        if (!shortName.empty())
                            mShortNamesMap[teamId] = shortName;
                        if (!abbr.empty())
                            mAbbreviationMap[teamId] = abbr;
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
}

void Converter::Convert(UInt gameId, Bool writeToGameFolder) {

}

void Converter::Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder, Bool fromFifaDatabase) {
    mCurrentGameId = gameId;
    mFromFifaDatabase = fromFifaDatabase;
    // read reference database here
    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;

    mFifamDatabase = new FifamDatabase(originalGameId, originalDb);

    FifamDatabase::mReadingOptions.mReadClubs = true;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = !fromFifaDatabase;

    mReferenceDatabase = new FifamDatabase(referenceGameId, referenceDb);

    mFifaDatabase = new FifaDatabase(L"D:\\Projects\\fifam\\db\\fifa");
    mFoomDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom", fromFifaDatabase? false : READ_FOOM_PERSONS, foom::db::db_size::DB_SIZE);

    Path infoPath = L"D:\\Projects\\fifam\\db";
    Path graphicsPath = L"D:\\Documents\\Sports Interactive\\Football Manager 2019\\graphics";
    Path contentPath = L"D:\\Projects\\fifam\\content";

    ReadAdditionalInfo(infoPath, gameId);
    appearanceGenerator.Read(infoPath / L"AppearanceDefs.sav");

    if (!mFromFifaDatabase) {
        std::wcout << L"Scanning badges folder..." << std::endl;
        for (auto const &i : recursive_directory_iterator(graphicsPath / L"dvx-logos" / L"clubs" / L"primary" / L"@2x")) {
            Int id = Utils::SafeConvertInt<Int>(i.path().filename().c_str());
            if (id > 0 && i.path().extension() == ".png")
                mAvailableBadges[id] = i.path();
        }
    }

    std::wcout << L"Constructing cities lists..." << std::endl;
    for (auto &[key, city] : mFoomDatabase->mCities) {
        if (city.mNation)
            city.mNation->mConverterData.mCities.push_back(&city);
    }
    for (auto &[key, nation] : mFoomDatabase->mNations) {
        std::sort(nation.mConverterData.mCities.begin(), nation.mConverterData.mCities.end(),
            [](foom::city *a, foom::city *b) {
                return a->mInhabitants > b->mInhabitants;
        });
    }

    std::wcout << L"Creating reserve clubs..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        for (auto &r : c.mVecReserveTeamsToCreate) {
            if (r.mDivision) {
                UInt newClubId = (*(mFoomDatabase->mClubs.rbegin())).first + 1;
                foom::club newClub;
                newClub.mID = newClubId;
                newClub.mName = c.mName;
                newClub.mShortName = c.mShortName;
                newClub.mIsReserveDummyClub = true;
                newClub.mIsReserveToCreateClub = true;
                mFoomDatabase->mClubs[newClubId] = newClub;

                foom::club::reserve_team rt;
                rt.mReserveTeamType = r.mReserveTeamType;
                rt.mReserveClub = &mFoomDatabase->mClubs[newClubId];
                c.mVecReserveTeams.push_back(rt);

                r.mDivision->mVecTeams.push_back(&mFoomDatabase->mClubs[newClubId]);
            }
        }
    }

    std::wcout << L"Finding reserve clubs..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // for all affiliated clubs
        for (auto &a : c.mVecAffiliations) {
            if (a.mAffiliatedClub && a.mIsMainClub) {
                if (a.mStartDate <= GetCurrentDate() && (a.mEndDate == FmEmptyDate() || a.mEndDate > GetCurrentDate())) {
                    foom::club *child = nullptr;
                    foom::club::converter_data::child_type childType;
                    if (a.mAffiliationType == 4 || a.mAffiliationType == 6 || a.mAffiliationType == 8) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second;
                    }
                    else if ((a.mAffiliationType == 1 || a.mAffiliationType == 3 || a.mAffiliationType == 21) && a.mPlayersMoveFreely && a.mSameBoard) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::second_affiliated;
                    }
                    else if (a.mAffiliationType == 5 || a.mAffiliationType == 7) {
                        child = a.mAffiliatedClub;
                        childType = foom::club::converter_data::child_type::third;
                    }
                    if (child) {
                        if (child->mConverterData.mParentClub) {
                        #ifdef SHOW_RESERVE_CLUBS_WARNINGS
                            Error(L"Affiliated B/C club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                                child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
                        #endif
                        }
                        else {
                            foom::club::converter_data::child_club childInfo;
                            childInfo.mClub = child;
                            childInfo.mIsAffiliated = true;
                            childInfo.mType = childType;
                            c.mConverterData.mChildClubs.push_back(childInfo);
                            child->mConverterData.mParentClub = &c;
                            child->mConverterData.mChildType = childType;
                        }
                    }
                }
            }
        }
        // for all reserve clubs
        for (auto &r : c.mVecReserveTeams) {
            if (r.mReserveClub) {
                foom::club *child = nullptr;
                foom::club::converter_data::child_type childType;
                if (r.mReserveTeamType == 1 || r.mReserveTeamType == 3 || r.mReserveTeamType == 14 || r.mReserveTeamType == 15 || r.mReserveTeamType == 16) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::second;
                }
                else if (r.mReserveTeamType == 13 || r.mReserveTeamType == 17) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::third;
                }
                else if (r.mReserveTeamType == 9) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u23;
                }
                else if (r.mReserveTeamType == 10 || r.mReserveTeamType == 30) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u21;
                }
                else if (r.mReserveTeamType == 18) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u20;
                }
                else if (r.mReserveTeamType == 11) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u19;
                }
                else if (r.mReserveTeamType == 12) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u18;
                }
                else if (r.mReserveTeamType == 21) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u21_b;
                }
                else if (r.mReserveTeamType == 36) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u19_b;
                }
                else if (r.mReserveTeamType == 37) {
                    child = r.mReserveClub;
                    childType = foom::club::converter_data::child_type::u18_b;
                }
                if (child) {
                    if (child->mConverterData.mParentClub) {
                    #ifdef SHOW_RESERVE_CLUBS_WARNINGS
                        Error(L"Reserve club has more than 1 parent club\nClub: '%s'\nClubParent1: '%s'\nClubParent2: '%s'",
                            child->mName.c_str(), child->mConverterData.mParentClub->mName.c_str(), c.mName.c_str());
                    #endif
                    }
                    else {
                        foom::club::converter_data::child_club childInfo;
                        childInfo.mClub = child;
                        childInfo.mIsAffiliated = false;
                        childInfo.mType = childType;
                        c.mConverterData.mChildClubs.push_back(childInfo);
                        child->mConverterData.mParentClub = &c;
                        child->mConverterData.mChildType = childType;
                    }
                }
            }
        }
        // sort reserve clubs - by team types (affiliated clubs have bigger priority)
        if (c.mConverterData.mChildClubs.size() > 1) {
            std::sort(c.mConverterData.mChildClubs.begin(), c.mConverterData.mChildClubs.end(),
                [](foom::club::converter_data::child_club const &a, foom::club::converter_data::child_club const &b) {
                if (a.mType < b.mType) return true;
                if (b.mType < a.mType) return false;
                if (a.mIsAffiliated) return true;
                if (b.mIsAffiliated) return false;
                return false;
            });

            // DEBUG
            ///String message = Utils::Format(L"Club '%s' reserve teams:", c.mName.c_str());
            ///for (UInt i = 0; i < c.mConverterData.mChildClubs.size(); i++)
            ///    message += Utils::Format(L"\n%d. '%s'", i + 1, c.mConverterData.mChildClubs[i].mClub->mName.c_str());
            ///Message(message);
        }
    }
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // child club has child clubs
        if (c.mConverterData.mParentClub && !c.mConverterData.mChildClubs.empty()) {
        #ifdef SHOW_RESERVE_CLUBS_WARNINGS
            Error(L"Reserve club has reserve teams\nClub: '%s'", c.mName.c_str());
        #endif
        }
    }

    std::wcout << L"Processing competitions history..." << std::endl;
    for (auto &entry : mFoomDatabase->mComps) {
        auto &c = entry.second;
        if (c.mNation) {
            if (c.mType == 0) // league
                c.mNation->mConverterData.mMainLeagues.push_back(&c);
            else if (c.mType == 2) // cup
                c.mNation->mConverterData.mMainCups.push_back(&c);
            else if (c.mType == 3) // league cup
                c.mNation->mConverterData.mLeagueCups.push_back(&c);
            else if (c.mType == 5) // supercup
                c.mNation->mConverterData.mSuperCups.push_back(&c);
        }
        // get nation champion
        for (auto &h : c.mVecHistory) {
            if (h.mYear > 0 && h.mYear <= CURRENT_YEAR && h.mFirstPlaced) {
                if (h.mFirstPlaced->mIsNation) {
                    foom::nation *nation = (foom::nation *)h.mFirstPlaced;
                    foom::nation *nationRunnerUp = (foom::nation *)h.mSecondPlaced;
                    if (c.mID == 1301385) { // World Cup
                        nation->mConverterData.mWorldCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mWorldCupFinals.insert(h.mYear);
                    }
                    else if (c.mType == 11) { // Main continental international finals
                        nation->mConverterData.mContinentalCupWins.insert(h.mYear);
                        if (nationRunnerUp)
                            nationRunnerUp->mConverterData.mContinentalCupFinals.insert(h.mYear);
                    }
                }
            }
        }
    }

    UInt MAX_LEAGUE_NAME_LENGTH = 63;
    UInt MAX_CUP_NAME_LENGTH = 63;
    UInt MAX_POOL_NAME_LENGTH = 58;
    UInt MAX_COMP_NAME_LENGTH = 29;

    if (gameId <= 7) {
        MAX_LEAGUE_NAME_LENGTH = 29;
        MAX_CUP_NAME_LENGTH = 29;
        MAX_POOL_NAME_LENGTH = 29;
        MAX_COMP_NAME_LENGTH = 29;
    }

    // convert nations, national teams, leagues
    std::wcout << L"Converting nations, national teams and leagues..." << std::endl;
    for (auto &entry : mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto country = mFifamDatabase->GetCountry(nation.mConverterData.mFIFAManagerID);
        if (country) {
            ConvertNationInfo(country, &nation, gameId);
            country->SetProperty(L"foom::nation", &nation);
            CreateStaffMembersForClub(gameId, &nation, &country->mNationalTeam, true);
            country->mNationalTeam.SetProperty(L"foom::nation", &nation);

            Map<UInt, DivisionInfo *> divLeagueLevels;
            Map<UInt, Vector<DivisionInfo *>> divLeagues;

            // find all division competitions for this country
            Int maxLevel = -1;
            Bool simulateLeague = false;
            for (auto &divInfo : mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    Int minPriority = (gameId <= 7) ? 4 : 1;
                    if (divInfo.mPriority >= minPriority) {
                        simulateLeague = true;
                        if (divInfo.mType == DivisionInfo::League)
                            divLeagues[divInfo.mLevel].push_back(&divInfo);
                        else if (divInfo.mType == DivisionInfo::Level)
                            divLeagueLevels[divInfo.mLevel] = &divInfo;
                        if (divInfo.mLevel > maxLevel)
                            maxLevel = divInfo.mLevel;
                    }
                    else if (divInfo.mPriority >= 1)
                        simulateLeague = true;
                }
            }

            country->SetProperty<Bool>(L"SimulateLeague", simulateLeague);

            Vector<FifamCompLeague *> createdLeagues;
            Vector<FifamCompPool *> createdPools;
            Vector<FifamCompCup *> createdCups;

            if (maxLevel >= 0) {

                bool countryUsesSpecialScript =
                    country->mId == FifamNation::England ||
                    country->mId == FifamNation::Germany ||
                    country->mId == FifamNation::Scotland ||
                    country->mId == FifamNation::Spain;

                // sort leagues
                for (auto &e : divLeagues) {
                    if (e.second.size() > 1) {
                        std::sort(e.second.begin(), e.second.end(), [](DivisionInfo *a, DivisionInfo *b) {
                            return a->mOrder < b->mOrder;
                        });
                    }
                }

                // create leagues

                mNextFreeUID[country->mId - 1] = 0x4001;
                mNumTeamsInLeagueSystem[country->mId - 1] = 0;
                Int minLevelWithReserveTeams = -1;

                UInt leagueIndex = 0;
                for (auto &leagues : divLeagues) {
                    UInt teamCounter = 0;
                    for (DivisionInfo *lg : leagues.second) {
                        FifamCompID leagueID = FifamCompID(country->mId, FifamCompType::League, leagueIndex++);
                        String compName;
                        if (lg->mName.length() <= MAX_LEAGUE_NAME_LENGTH)
                            compName = lg->mName;
                        else
                            compName = FifamNames::LimitName(lg->mShortName, MAX_LEAGUE_NAME_LENGTH);
                        FifamCompLeague *league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, leagueID, compName)->AsLeague();

                        lg->mCompID = leagueID;

                        // convert - old

                        foom::comp *comp = mFoomDatabase->get<foom::comp>(lg->mID);
                        if (!comp) {
                            Error(L"Competition is not available\nCompetitionName: %s\nCompetitionID: %d", lg->mName.c_str(), lg->mID);
                            continue;
                        }

                        league->mCompetitionLevel = lg->mLevel;
                        league->mLeagueLevel = lg->mLevel;
                        league->mNumSubsAllowed = 3;
                        league->mNumTeams = lg->mTeams;
                        league->mNumRelegatedTeams = lg->mRelegated;

                        // rounds (auto-calculation)

                        if (lg->mRounds > 0)
                            league->mNumRounds = lg->mRounds;
                        else if (league->mNumTeams >= 15)
                            league->mNumRounds = 2;
                        else if (league->mNumTeams >= 11)
                            league->mNumRounds = 3;
                        else
                            league->mNumRounds = 4;

                        league->mRoundType = FifamRoundID::Group1;
                        league->mTeams.resize(lg->mTeams);

                        if (comp->mReputation <= 20) {
                            if (comp->mReputation > 10) {
                                league->mTransferMarketMp = comp->mReputation;
                                league->mAttendanceMp = comp->mReputation;
                            }
                            else {
                                league->mTransferMarketMp = 10;
                                league->mAttendanceMp = 10;
                            }
                        }
                        else {
                            league->mTransferMarketMp = Utils::Min(200, (Int)((Float)(comp->mReputation - 20) * 1.3f));
                            league->mAttendanceMp = Utils::Min(120, (Int)((Float)(comp->mReputation - 20) * 0.69f));
                        }
                        league->mFreeAgentsCanBeTranferredAtAnyTime = true;
                        league->mForeignFreeAgentsCanBeTransferredAtAnyTime = true;
                        league->mShortTermLoansTotalLimit = 15;
                        league->mShortTermLoansSimultaneosLimit = 15;
                        league->mLongTermLoansTotalLimit = 15;
                        league->mLongTermLoansSimultaneosLimit = 15;
                        league->mOverallTotalLoansLimit = 15;
                        league->mOverallTotalSimultaneosLoansLimit = 15;
                        league->mLoanedPlayersPerSeason = 15;
                        league->mLoanPlayerPerSquad = 15;
                        league->mSimLoanOtherLeague = 15;
                        league->mSimLoanSameLeague = 15;
                        league->mLoanOtherLeagueCount = 15;
                        league->mLoanSameLeagueCount = 15;

                        comp->mConverterData.mLeague = league;
                        league->SetProperty(L"foom::comp", comp);
                        league->SetProperty(L"foom::id", lg->mID);
                        league->SetProperty(L"foom::reputation", lg->mRep);
                        league->SetProperty(L"startDate", lg->mStartDate);
                        league->SetProperty(L"endDate", lg->mEndDate);
                        league->SetProperty(L"winterBreakStart", lg->mWinterBreakStart);
                        league->SetProperty(L"winterBreakEnd", lg->mWinterBreakEnd);
                        createdLeagues.push_back(league);
                        mLeaguesSystem[country->mId - 1][league->mLeagueLevel].push_back(league);

                        league->mEqualPointsSorting = lg->mSorting;
                        if (lg->mTvBonus >= 0)
                            league->mBonuses[3] = AlignMoneyValue(lg->mTvBonus, 1'000);
                        if (lg->mWinBouns >= 0)
                            league->mBonuses[1] = AlignMoneyValue(lg->mWinBouns, 1'000);
                        if (lg->mPlaceBonus >= 0)
                            league->mBonuses[2] = AlignMoneyValue(lg->mPlaceBonus, 1'000);
                        if (lg->mAttendanceMp > 0)
                            league->mAttendanceMp = lg->mAttendanceMp;
                        if (lg->mTransfersMp > 0)
                            league->mTransferMarketMp = lg->mTransfersMp;
                        if (lg->mNumSubs > 0)
                            league->mNumSubsAllowed = lg->mNumSubs;
                        if (lg->mDomesticPlayers > 0)
                            league->mMinDomesticPlayerCount = lg->mDomesticPlayers;
                        if (lg->mU21Players > 0)
                            league->mMinU21PlayerCount = lg->mU21Players;
                        if (lg->mNonEuSigns > 0)
                            league->mMaxNumberOfNonEUSigns = lg->mNonEuSigns;

                        league->mInstructions.PushBack(new FifamInstruction::GET_POOL(
                            FifamCompID(country->mId, FifamCompType::Pool, leagues.first), teamCounter, lg->mTeams));

                        // add teams
                        Bool hasReserveTeams = lg->mReserveTeamsAllowed == 1;
                        UInt numLeaguePromotedTeams = 0;
                        UInt numLeagueRelegatedTeams = 0;

                        if (comp->mVecTeams.size() != lg->mTeams) {
                            Error(L"Different number of teams in league: %d / %d\nCompetitionName: %s\nCompetitionID: %d",
                                comp->mVecTeams.size(), lg->mTeams, lg->mName.c_str(), lg->mID);
                            continue;
                        }

                        for (auto entry : comp->mVecTeams) {
                            foom::club *team = (foom::club *)entry;
                            if (!team)
                                continue;
                            Int teamRep = team->mReputation;
                            if (teamRep < 0)
                                teamRep = 0;
                            else if (teamRep > 10'000)
                                teamRep = 10'000;
                            if (!team->mIsReserveDummyClub) {
                                if (team->mLastDivision) {
                                    if (team->mLastDivision != comp) {
                                        if (team->mLastDivision->mCompetitionLevel == 0 || team->mLastDivision->mCompetitionLevel > comp->mCompetitionLevel) {
                                            numLeaguePromotedTeams++;
                                            if (team->mLastPosition != 0)
                                                team->mConverterData.mLeaguePos = 100'000 + team->mLastPosition;
                                            else
                                                team->mConverterData.mLeaguePos = 100'000 + 1'000 + (10'000 - teamRep);
                                        }
                                        else {
                                            numLeagueRelegatedTeams++;
                                            if (team->mLastPosition != 0)
                                                team->mConverterData.mLeaguePos = -100'000 - (1'000 - team->mLastPosition);
                                            else
                                                team->mConverterData.mLeaguePos = -1 - teamRep;
                                        }
                                    }
                                    else {
                                        if (team->mLastPosition != 0)
                                            team->mConverterData.mLeaguePos = team->mLastPosition;
                                        else
                                            team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                                    }
                                }
                                else {
                                    numLeaguePromotedTeams++;
                                    team->mConverterData.mLeaguePos = 200'000 + (10'000 - teamRep);
                                }
                            }
                            else {
                                team->mConverterData.mLeaguePos = 1'000 + (10'000 - teamRep);
                            }
                        }
                        UInt leagueClubCounter = 0;
                        std::sort(comp->mVecTeams.begin(), comp->mVecTeams.end(), [](foom::team *a, foom::team *b) {
                            if (!a) return false;
                            if (!b) return true;
                            return ((foom::club *)a)->mConverterData.mLeaguePos < ((foom::club *)b)->mConverterData.mLeaguePos;
                        });
                        for (auto entry : comp->mVecTeams) {
                            foom::club *team = (foom::club *)entry;
                            if (!team) {
                                Error(L"Invalid club pointer in league\nLeague: %s", lg->mName.c_str());
                                continue;
                            }
                            FifamClub *club = nullptr;
                            foom::club *mainTeam = nullptr; // for reserve teams
                            FifamClubTeamType teamType = FifamClubTeamType::First;
                            Bool createNewClub = false;
                            Bool isExtinct = team->mExtinct;
                            if (team->mConverterData.mParentClub) {
                                isExtinct = team->mConverterData.mParentClub->mExtinct;
                                hasReserveTeams = true;
                            }
                            if (isExtinct) {
                                Error(L"Extinct club in the league\nClub: '%s'\nLeague: '%s'",
                                    team->mName.c_str(), lg->mName.c_str());
                            }
                            else if (team->mConverterData.mFifamClub) {
                                Message(Utils::Format(L"Team already present in other league\nClub: '%s'\nLeague: '%s'",
                                    team->mName.c_str(), lg->mName.c_str()));
                                club = (FifamClub *)team->mConverterData.mFifamClub;
                                if (team->mConverterData.mParentClub)
                                    teamType = FifamClubTeamType::Reserve;
                            }
                            else if (team->mConverterData.mParentClub) {
                                if (!team->mConverterData.mParentClub->mConverterData.mFifamClub) {
                                    Error(L"Reserve club appears before the first team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                        team->mConverterData.mParentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                }
                                else {
                                    if (!team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB
                                        && team->mConverterData.mParentClub->mNation == &nation)
                                    {
                                        club = (FifamClub *)team->mConverterData.mParentClub->mConverterData.mFifamClub;
                                        teamType = FifamClubTeamType::Reserve;
                                        team->mConverterData.mParentClub->mConverterData.mMainChildClubInDB = team;
                                        team->mConverterData.mParentClubInDB = team->mConverterData.mParentClub;
                                    }
                                    else {
                                        foom::club *parentClub = team->mConverterData.mParentClub;
                                        if (!team->mIsReserveDummyClub) {
                                            team->mConverterData.mParentClub->mConverterData.mChildClubs.erase(std::remove_if(
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.begin(),
                                                team->mConverterData.mParentClub->mConverterData.mChildClubs.end(),
                                                [=](foom::club::converter_data::child_club const &a) {
                                                return a.mClub == team;
                                            }), team->mConverterData.mParentClub->mConverterData.mChildClubs.end());
                                            mainTeam = team->mConverterData.mParentClub;
                                            team->mConverterData.mOriginalStoredParentClub = team->mConverterData.mParentClub;
                                            team->mConverterData.mParentClub = nullptr;
                                            createNewClub = true;
                                        }
                                        else {
                                            Error(L"Reserve club can't be created because it's a dummy club\nMainClub: '%s'\nReserveTeamID: %d\nLeague: '%s'",
                                                team->mConverterData.mParentClub->mName.c_str(), team->mID, lg->mName.c_str());
                                        }
                                        //Error(L"Reserve club already present for team in the league\nMainClub: '%s'\nReserveClub: '%s'\nLeague: '%s'",
                                        //    parentClub->mName.c_str(), team->mName.c_str(), lg->mName.c_str());
                                    }
                                }
                            }
                            else
                                createNewClub = true;
                            if (createNewClub)
                                club = CreateAndConvertClub(gameId, team, mainTeam, country, lg, true);
                            // put team to league
                            if (club) {
                                league->mTeams[leagueClubCounter].mPtr = club;
                                league->mTeams[leagueClubCounter].mTeamType = teamType;
                                club->SetProperty(L"league", league);
                                if (team->mConverterData.mLeaguePos >= 100'000) {
                                    if (numLeaguePromotedTeams <= (comp->mVecTeams.size() / 2)) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
                                    }
                                }
                                else if (team->mConverterData.mLeaguePos < 0) {
                                    if (numLeagueRelegatedTeams <= (comp->mVecTeams.size() / 2)) {
                                        if (teamType == FifamClubTeamType::First)
                                            club->mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                        else
                                            club->mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
                                    }
                                }
                                leagueClubCounter++;
                            }
                        }

                        if (hasReserveTeams) {
                            if (minLevelWithReserveTeams == -1 || minLevelWithReserveTeams > league->mLeagueLevel)
                                minLevelWithReserveTeams = league->mLeagueLevel;
                        }

                        bool gotFixtures = false;
                        Date leagueStart, leagueEnd;

                        if (mFixturesPerLeague.count(comp->mID) > 0) {
                            auto const &fixtures = mFixturesPerLeague[comp->mID];
                            UInt leagueTotalMatches = league->GetTotalNumMatches();
                            UInt leagueMatchdaysInRound = league->GetNumMatchdaysInRound();
                            UInt leagueMatchesInMatchday = league->GetNumMatchesInMatchday();
                            if (leagueTotalMatches > 0 && fixtures.size() == leagueTotalMatches) {
                                gotFixtures = true;
                                league->mFixtures.resize(league->mNumRounds * leagueMatchdaysInRound);
                                Map<FifamClubLink, UInt> leagueTeamsMap;
                                for (UInt i = 0; i < league->mTeams.size(); i++)
                                    leagueTeamsMap[league->mTeams[i]] = i + 1;
                                UInt matchId = 0;
                                UInt matchdayId = 0;
                                for (UInt r = 0; r < league->mNumRounds; r++) {
                                    if (!gotFixtures)
                                        break;
                                    for (UInt md = 0; md < leagueMatchdaysInRound; md++) {
                                        if (!gotFixtures)
                                            break;
                                        league->mFixtures[matchdayId].resize(leagueMatchesInMatchday);
                                        for (UInt d = 0; d < leagueMatchesInMatchday; d++) {
                                            if (fixtures.size() <= matchId) {
                                                Error(L"Incorrect fixtures list size\nLeague: %s", comp->mName.c_str());
                                                gotFixtures = false;
                                                break;
                                            }
                                            UInt team1id = 0, team2id = 0;
                                            if (!fixtures[matchId].mTeam1 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam1)) == 0) {
                                                //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam1 ? GetTeamClubLink(fixtures[matchId].mTeam1).GetTeamName().c_str() : L"none");
                                                gotFixtures = false;
                                                break;
                                            }
                                            team1id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam1)];
                                            if (!fixtures[matchId].mTeam2 || leagueTeamsMap.count(GetTeamClubLink(fixtures[matchId].mTeam2)) == 0) {
                                                //Error(L"Incorrect fixtures team (match %d)\nLeague: %s\nTeam: %s", matchId + 1, comp->mName.c_str(), fixtures[matchId].mTeam2 ? GetTeamClubLink(fixtures[matchId].mTeam2).GetTeamName().c_str() : L"none");
                                                gotFixtures = false;
                                                break;
                                            }
                                            team2id = leagueTeamsMap[GetTeamClubLink(fixtures[matchId].mTeam2)];
                                            league->mFixtures[matchdayId][d] = { team1id, team2id };
                                            matchId++;
                                        }
                                        matchdayId++;
                                    }
                                }
                            }
                            //else {
                            //    Error(L"Error fixtures list size\nLeague: %s\n%d/%d", comp->mName.c_str(), fixtures.size(), leagueTotalMatches);
                            //}
                        }

                        if (!gotFixtures)
                            league->GenerateFixtures();
                        league->GenerateCalendar(13, 0, 0, 0);

                        // TODO: translate date to calendar day

                        mNumTeamsInLeagueSystem[country->mId - 1] += lg->mTeams;

                        teamCounter += lg->mTeams;
                    }
                }

                // create league levels
                for (Int i = 0; i <= maxLevel; i++) {
                    DivisionInfo *level = nullptr;
                    Int levelRep = 0;
                    String levelName;
                    auto itLevel = divLeagueLevels.find(i);
                    // if level exists
                    if (itLevel != divLeagueLevels.end()) {
                        level = divLeagueLevels[i];
                        if (level->mName.length() < 29)
                            levelName = level->mName;
                        else
                            levelName = level->mShortName;
                        levelRep = level->mRep;
                    }
                    else { // if level does not exist
                        Vector<String> divsOnLevelNames;
                        auto itLeagues = divLeagues.find(i);
                        if (itLeagues != divLeagues.end()) {
                            for (auto &div : divLeagues[i])
                                divsOnLevelNames.push_back(div->mName);
                            if (!divsOnLevelNames.empty())
                                levelName = FifamNames::GetLeagueLevelName(divsOnLevelNames, i + 1);
                            if (divLeagues[i].size() > 0)
                                levelRep = divLeagues[i][0]->mRep;
                        }
                        else
                            Error(L"No league levels or leagues at level %d in country %s", i, FifamTr(country->mName).c_str());
                    }
                    // setup level information
                    auto &levelNames = country->mLeagueLevelNames.emplace_back();
                    FifamTrSetAll<String>(levelNames, FifamNames::LimitName(levelName, 28));

                    DivisionInfo *firstDivOnLevel = nullptr;

                    auto itFirstLeague = divLeagues.find(i);
                    if (itFirstLeague != divLeagues.end() && divLeagues[i].size() > 0)
                        firstDivOnLevel = divLeagues[i][0];

                    if (firstDivOnLevel) {
                        country->mLeagueLevels[i].mEqualPointsSorting = firstDivOnLevel->mSorting;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = firstDivOnLevel->mForeignersLimit;
                        country->mLeagueLevels[i].mNumRelegatedTeams = firstDivOnLevel->mRelegated;
                    }
                    else {
                        country->mLeagueLevels[i].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                        country->mLeagueLevels[i].mNumNonEUPlayersAllowed = 0;
                        country->mLeagueLevels[i].mNumRelegatedTeams = 0;
                    }

                    country->mLeagueLevels[i].mRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));

                    if (i == 0)
                        country->mAveragePlayerRating = country->mLeagueLevels[i].mRating;

                    // setup pool

                    FifamCompID poolID = FifamCompID(country->mId, FifamCompType::Pool, i);
                    FifamCompPool *pool = mFifamDatabase->CreateCompetition(FifamCompDbType::Pool, poolID, FifamNames::LimitName(levelName, MAX_POOL_NAME_LENGTH) + L" Pool")->AsPool();
                    pool->mCompetitionLevel = i;

                    createdPools.push_back(pool);

                    if (level)
                        level->mCompID = poolID;

                    if (i > 0)
                        pool->mCompConstraints.push_back(FifamCompID(country->mId, FifamCompType::Pool, i - 1));

                    // higher level - relegated teams
                    if (i > 0) {
                        UInt higherLevel = i - 1;
                        if (divLeagues.find(higherLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[higherLevel]) {
                                if (l->mRelegated > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                        l->mCompID, l->mTeams - l->mRelegated + 1, l->mRelegated));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", higherLevel, FifamTr(country->mName).c_str());
                    }

                    // this level
                    if (divLeagues.find(i) != divLeagues.end()) {
                        // apply pool sorting if 2 or more leagues on this level
                        if (divLeagues[i].size() > 1)
                            pool->mSorting = FifamPoolSorting::MapLeague;

                        for (DivisionInfo *l : divLeagues[i]) {
                            pool->mNumTeams += l->mTeams;
                            Int numTeamsFromThisLeague = l->mTeams - (l->mPromoted + l->mRelegated);
                            if (numTeamsFromThisLeague > 0) {
                                pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                                    l->mCompID, l->mPromoted + 1, numTeamsFromThisLeague));
                            }
                        }
                    }
                    else
                        Error(L"No leagues at level %d\nCountry: %s", i, FifamTr(country->mName).c_str());

                    // next level - promoted teams
                    if (i != maxLevel) {
                        UInt lowerLevel = i + 1;
                        if (divLeagues.find(lowerLevel) != divLeagues.end()) {
                            for (DivisionInfo *l : divLeagues[lowerLevel]) {
                                if (l->mPromoted > 0) {
                                    pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(l->mCompID, 1, l->mPromoted));
                                }
                            }
                        }
                        else
                            Error(L"No leagues at level %d\nCountry: %s", lowerLevel, FifamTr(country->mName).c_str());
                    }
                    else { // CaC league & spare
                        pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(
                            FifamCompID(country->mId, FifamCompType::League, leagueIndex), 1, 1));
                        pool->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                    }
                }

                if (minLevelWithReserveTeams >= 0) {
                    country->mLeagueLevelWithReserveTeams = minLevelWithReserveTeams;
                    for (auto pool : createdPools) {
                        if (pool->mCompetitionLevel >= minLevelWithReserveTeams)
                            pool->mReserveTeamsAllowed = true;
                    }
                }
                else
                    country->mLeagueLevelWithReserveTeams = maxLevel + 1;
            }
            else {
                country->mLeagueLevels[0].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                country->mLeagueLevels[0].mNumNonEUPlayersAllowed = 0;
                country->mLeagueLevels[0].mNumRelegatedTeams = 0;
                country->mLeagueLevels[0].mRating = 0;
                
                Int levelRep = -1;

                for (auto &divInfo : mDivisions) {
                    if (divInfo.mNationID == nation.mID && divInfo.mType == DivisionInfo::League && divInfo.mRep > levelRep)
                        levelRep = divInfo.mRep;
                }

                if (levelRep > 0) {
                    country->mAveragePlayerRating = (UChar)(roundf((
                        (Float)levelRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));
                }
            }

            for (auto &e : mLeaguesSystem[country->mId - 1]) {
                for (auto &l : e.second)
                    l->SetProperty<Int>(L"NumLeaguesOnLevel", e.second.size());
            }

            if (mNumTeamsInLeagueSystem[country->mId - 1] > 0) {
                Int cupsMinPriority = 1;
                Int maxFACups = 1;
                Int maxLeagueCups = 4;
                Int maxSuperCups = 1;
                Int numCreatedFACups = 0;
                Int numCreatedLeagueCups = 0;
                Int numCreatedSuperCups = 0;
                if (gameId <= 7)
                    cupsMinPriority = 2;
                Int startMatchdaySuperCup = 3;
                Int startMatchdayFaCup = startMatchdaySuperCup + 2;
                Int startMatchdayLeagueCup = startMatchdayFaCup + 16;

                // find all cups in this country
                for (auto &cupInfo : mCups) {
                    if (cupInfo.mNationID == nation.mID && cupInfo.mPriority >= cupsMinPriority) {
                        Int nextAvailableCompIndex = -1;
                        FifamCompType cupCompType = FifamCompType::LeagueCup;
                        if (cupInfo.mType == CupInfo::FA) {
                            if (numCreatedFACups < maxFACups) {
                                nextAvailableCompIndex = numCreatedFACups;
                                numCreatedFACups++;
                                cupCompType = FifamCompType::FaCup;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::League) {
                            if (numCreatedLeagueCups < maxLeagueCups) {
                                nextAvailableCompIndex = numCreatedLeagueCups;
                                numCreatedLeagueCups++;
                                cupCompType = FifamCompType::LeagueCup;
                            }
                        }
                        else if (cupInfo.mType == CupInfo::Supercup) {
                            if (numCreatedSuperCups < maxSuperCups) {
                                nextAvailableCompIndex = numCreatedSuperCups;
                                cupCompType = FifamCompType::SuperCup;
                            }
                        }
                        if (nextAvailableCompIndex != -1) {
                            if (cupInfo.mTemplateType == FifamCupSystemType::None) {
                                Error(L"Unknown cup template type\nCup: %s", cupInfo.mName.c_str());
                                continue;
                            }
                            FifamCupAlloc *cupTemplate = mFifamDatabase->GetCupTemplate(cupInfo.mTemplateType);
                            if (!cupTemplate) {
                                Error(L"Cup template not found\nCup: %s\nTemplate: %s", cupInfo.mName.c_str(), cupInfo.mTemplateType.ToCStr());
                                continue;
                            }
                            FifamCompID cupID = { (UChar)country->mId, cupCompType, (UShort)nextAvailableCompIndex };
                            String cupName;
                            if (cupInfo.mName.length() <= MAX_CUP_NAME_LENGTH)
                                cupName = cupInfo.mName;
                            else
                                cupName = FifamNames::LimitName(cupInfo.mShortName, MAX_CUP_NAME_LENGTH);
                            FifamCompCup *cup = mFifamDatabase->CreateCompetition(FifamCompDbType::Cup, cupID, cupName)->AsCup();
                            createdCups.push_back(cup);
                            cup->SetProperty(L"foom::id", cupInfo.mID);
                            cup->SetProperty(L"foom::reputation", cupInfo.mReputation);
                            if (cupInfo.mLevel < 0 || cupInfo.mLevel > 15)
                                cup->mCompetitionLevel = 0;
                            else
                                cup->mCompetitionLevel = cupInfo.mLevel;
                            if (cupInfo.mNumSubs <= 0)
                                cup->mNumSubsAllowed = 3;
                            else
                                cup->mNumSubsAllowed = Utils::Clamp(cupInfo.mNumSubs, 1, 7);
                            cup->mCupTemplate = cupInfo.mTemplateType;
                            Int numRounds = 0;
                            Int startRoundIndex = -1;
                            for (Int i = 7; i >= 0; i--) {
                                if (cupInfo.mRoundDesc[i].empty())
                                    break;
                                startRoundIndex = i;
                                numRounds++;
                            }
                            if (numRounds > 0 && startRoundIndex >= 0) {
                                cup->mRounds.resize(numRounds);
                                bool failed = false;
                                Int r = 0;
                                for (Int i = startRoundIndex; i <= 7; i++) {
                                    if (cupInfo.mRoundDesc[i].empty()) {
                                        Error(L"Empty cup description\nCountry: %s\nCup: %s", FifamTr(country->mName).c_str(), cupInfo.mName.c_str());
                                        failed = true;
                                        break;
                                    }
                                    cup->mRounds[r].mFlags.Clear();
                                    for (UInt c = 0; c < cupInfo.mRoundDesc[i].length(); c++) {
                                        if (c == 0) {
                                            if (cupInfo.mRoundDesc[i][c] == '1')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_1stLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '2')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::_2ndLeg, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'R')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithReplay, true);
                                            else {
                                                Error(L"Wrong round description in cup\nCup: %s\nRound: %d\nDescription: %s", cupInfo.mName.c_str(), i, cupInfo.mRoundDesc[i].c_str());
                                                failed = true;
                                                break;
                                            }
                                        }
                                        else {
                                            if (cupInfo.mRoundDesc[i][c] == 'E')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithExtraTime, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'P')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithPenalty, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'G')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithGoldenGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'S')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithSilverGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == 'A')
                                                cup->mRounds[r].mFlags.Set(FifamBeg::WithoutAwayGoal, true);
                                            else if (cupInfo.mRoundDesc[i][c] == '-' && (c + 1) < cupInfo.mRoundDesc[i].length()) {
                                                if (cupInfo.mRoundDesc[i][c + 1] == 'P')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoPremierLeagueTeams, true);
                                                else if (cupInfo.mRoundDesc[i][c + 1] == 'S')
                                                    cup->mRounds[r].mFlags.Set(FifamBeg::NoShuffle, true);
                                                c++;
                                            }
                                            else if (cupInfo.mRoundDesc[i][c] == '+') {
                                                Int newTeamsInRound = Utils::SafeConvertInt<Int>(cupInfo.mRoundDesc[i].substr(c + 1));
                                                if (newTeamsInRound > 0)
                                                    cup->mRounds[r].mNewTeamsRound = newTeamsInRound;
                                                break;
                                            }
                                        }
                                    }
                                    if (failed)
                                        break;
                                    r++;
                                }
                                if (failed)
                                    cup->mRounds.clear();
                                else if (!cup->mRounds.empty()) {
                                    UInt teamsFromPrevRound = 0;
                                    UInt currBeg = 0;
                                    UInt lastRoundWithAddedTeams = 0;
                                    for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                        cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                        cup->mRounds[r].mTeamsRound = cup->mRounds[r].mNewTeamsRound + teamsFromPrevRound;
                                        teamsFromPrevRound = cup->mRounds[r].mTeamsRound / 2;
                                        cup->mRounds[r].mStartBeg = currBeg;
                                        currBeg += teamsFromPrevRound;
                                        cup->mRounds[r].mEndBeg = currBeg;
                                        if (cup->mRounds[r].mNewTeamsRound)
                                            lastRoundWithAddedTeams = r;
                                    }
                                    UInt numRawRounds = cup->mRounds.size() - lastRoundWithAddedTeams;
                                    // final rounds
                                    Int numFinalRounds = 0;
                                    for (Int r = (Int)cup->mRounds.size() - 1; r >= (Int)lastRoundWithAddedTeams; r--) {
                                        if (numFinalRounds >= 4)
                                            break;
                                        cup->mRounds[r].mRoundID.SetFromInt(15 - numFinalRounds);
                                        numFinalRounds++;
                                    }
                                    Int numRoundsLeft = cup->mRounds.size() - numFinalRounds;
                                    // quali rounds
                                    Int numQualiRounds = numRoundsLeft > 5 ? 8 - numRoundsLeft : 0;
                                    for (Int r = 0; r < numQualiRounds; r++)
                                        cup->mRounds[r].mRoundID.SetFromInt(r + 1);
                                    // other rounds
                                    numRoundsLeft -= numQualiRounds;
                                    for (Int r = 0; r < numRoundsLeft; r++)
                                        cup->mRounds[r + numQualiRounds].mRoundID.SetFromInt(r + 5);
                                }
                            }
                            if (cup->mRounds.empty()) {
                                // use template
                                UInt numRounds = cupTemplate->mRounds.size();
                                cup->mRounds.resize(numRounds);
                                for (UInt i = 0; i < numRounds; i++) {
                                    cup->mRounds[i].mTeamsRound = cupTemplate->mRounds[i]->mTeamsRound;
                                    cup->mRounds[i].mNewTeamsRound = cupTemplate->mRounds[i]->mNewTeamsRound;
                                    cup->mRounds[i].mStartBeg = cupTemplate->mRounds[i]->mStartBeg;
                                    cup->mRounds[i].mEndBeg = cupTemplate->mRounds[i]->mEndBeg;
                                    cup->mRounds[i].mFlags = cupTemplate->mRounds[i]->mFlags;
                                    cup->mRounds[i].mRoundID = cupTemplate->mRounds[i]->mRoundID;
                                }
                            }
                            cup->mNumTeams = 0;
                            if (cupInfo.mBonus <= 0)
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mReputation * cupInfo.mReputation * 75, 1'000);
                            else
                                cupInfo.mBonus = AlignMoneyValue(cupInfo.mBonus, 1'000);
                            if (cupInfo.mTvBonus <= 0)
                                cupInfo.mTvBonus = AlignMoneyValue((cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation * cupInfo.mReputation) / 320, 1'000);
                            else
                                cupInfo.mTvBonus = AlignMoneyValue(cupInfo.mTvBonus, 1'000);
                            UInt numMatchdays = 0;
                            for (UInt r = 0; r < cup->mRounds.size(); r++) {
                                cup->mNumTeams += cup->mRounds[r].mNewTeamsRound;
                                cup->mRounds[r].mBonuses[1] = cupInfo.mBonus;
                                cup->mRounds[r].mBonuses[3] = cupInfo.mTvBonus;
                                UInt roundId = cup->mRounds.size() - r - 1;
                                if (roundId != 0) {
                                    UInt divValue = 1 << roundId;
                                    cup->mRounds[r].mBonuses[1] = AlignMoneyValue(cup->mRounds[r].mBonuses[1] / divValue, 1'000u);
                                    cup->mRounds[r].mBonuses[3] = AlignMoneyValue(cup->mRounds[r].mBonuses[3] / divValue, 1'000u);
                                }
                                if (cup->mRounds[r].mFlags.Check(FifamBeg::_2ndLeg) || cup->mRounds[r].mFlags.Check(FifamBeg::WithReplay))
                                    numMatchdays += 2;
                                else
                                    numMatchdays += 1;
                            }
                            if (!cupInfo.mStructure.empty()) {
                                UInt currentLevel = 0;
                                currentLevel = cupInfo.mMaxLevel;
                                for (auto c : cupInfo.mStructure) {
                                    switch (c) {
                                    case L'-':
                                        if (mLeaguesSystem[country->mId - 1][currentLevel].size() > 0)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(mLeaguesSystem[country->mId - 1][currentLevel][0]->mID, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'|':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(currentLevel, 1, 24));
                                        currentLevel++;
                                        break;
                                    case L'W':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                        break;
                                    case L'S':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                        break;
                                    case L'C':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'L':
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    case L'R':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                        break;
                                    case L'N':
                                        if (gameId >= 10)
                                            cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::League, 0 }));
                                        break;
                                    }
                                }
                            }
                            if (cup->mInstructions.Empty()) {
                                if (cupInfo.mType == CupInfo::Supercup) {
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::FaCup, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_CHAMP({ country->mId, FifamCompType::League, 0 }));
                                    if (gameId >= 10)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_RUNNER_UP({ country->mId, FifamCompType::FaCup, 0 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(0, 1, 24));
                                }
                                else {
                                    for (auto const &level : mLeaguesSystem[country->mId - 1]) {
                                        if (level.first >= (UInt)cupInfo.mMaxLevel) {
                                            if (level.first == 0 && level.second.size() == 1)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_X_TO_Y(level.second[0]->mID, 1, 24));
                                            else if (level.second.size() > 0)
                                                cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(level.first, 1, 24));
                                        }
                                    }
                                    if (cupInfo.mType != CupInfo::League)
                                        cup->mInstructions.PushBack(new FifamInstruction::GET_CC_FA_WINNER({ country->mId, FifamCompType::FaCup, 1 }));
                                    cup->mInstructions.PushBack(new FifamInstruction::GET_TAB_SPARE());
                                }
                            }
                            if (cup->mRounds.empty()) {
                                Error(L"Cup has no rounds\nCountry: %s\nCup: %s",
                                    FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else if (cup->mNumTeams > 128) {
                                Error(L"Cup has more than 128 teams: %d\nCountry: %s\nCup: %s",
                                    cup->mNumTeams, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str());
                            }
                            else {
                                for (unsigned int ir = 0; ir < cup->mRounds.size(); ir++) {
                                    if (cup->mRounds[ir].mTeamsRound == 0 || (cup->mRounds[ir].mTeamsRound % 2)) {
                                        Error(L"Wrong teams count in cup round: %d\nCountry: %s\nCup: %s\nRound: %d",
                                            cup->mRounds[ir].mTeamsRound, FifamTr(country->mName).c_str(), FifamTr(cup->mName).c_str(), ir);
                                    }
                                }
                            }
                            //cup->mMathcdays
                            //for (UInt r = 0; r < cup->mRounds.size(); r++) {
                            for (UInt m = 0; m < numMatchdays; m++) {
                                if (cupCompType == FifamCompType::FaCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayFaCup++);
                                else if (cupCompType == FifamCompType::LeagueCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdayLeagueCup++);
                                else if (cupCompType == FifamCompType::SuperCup)
                                    cup->mFirstSeasonMatchdays.push_back(startMatchdaySuperCup++);
                            }
                            //}
                            cup->mSecondSeasonMatchdays = cup->mFirstSeasonMatchdays;
                        }
                    }
                }
            }
            GenerateCalendar(FifamNation::MakeFromInt(country->mId), mFifamDatabase, createdLeagues, createdCups);
        }
    }

    // create spare teams (non-league teams)
    std::wcout << L"Creating spare clubs..." << std::endl;
    Array<Vector<foom::club *>, FifamDatabase::NUM_COUNTRIES> spareClubs;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &c = entry.second;
        // team is active, has associated nation, not a reserve team
        if (!c.mExtinct && c.mNation && !c.mConverterData.mParentClub) {
            // fix - set reserve teams for first teams
            if (!c.mConverterData.mChildClubs.empty() && !c.mConverterData.mMainChildClubInDB)
                c.mConverterData.mMainChildClubInDB = c.mConverterData.mChildClubs[0].mClub;
            if (!c.mConverterData.mFifamClub) {
                // if proper country ID
                auto country = mFifamDatabase->GetCountry(c.mNation->mConverterData.mFIFAManagerID);
                if (country)
                    spareClubs[country->mId - 1].push_back(&c);
            }
        }
    }

    for (auto &country : mFifamDatabase->mCountries) {
        UInt maxClubs = gameId >= 10 ? 1000 : 500;
        if (country && country->mClubs.size() < maxClubs) {
            if (!spareClubs[country->mId - 1].empty()) {
                std::sort(spareClubs[country->mId - 1].begin(), spareClubs[country->mId - 1].end(), [](foom::club *a, foom::club *b) {
                    UInt teamALeagueLevel = a->mDivision ? a->mDivision->mCompetitionLevel : 0;
                    UInt teamBLeagueLevel = b->mDivision ? b->mDivision->mCompetitionLevel : 0;
                    if (teamALeagueLevel == 0)
                        teamALeagueLevel = 9999;
                    if (teamBLeagueLevel == 0)
                        teamBLeagueLevel = 9999;
                    if (teamALeagueLevel < teamBLeagueLevel) return true;
                    if (teamBLeagueLevel < teamALeagueLevel) return false;
                    if (a->mReputation > b->mReputation) return true;
                    if (b->mReputation > a->mReputation) return false;
                    return false;
                });
                UInt numSpareClubsToAdd = 7;
                //if (gameId <= 7)
                //    numSpareClubsToAdd = 4;
                if (mNumTeamsInLeagueSystem[country->mId - 1] > 8) {
                    if (gameId > 7 || !mFromFifaDatabase)
                        numSpareClubsToAdd = mNumTeamsInLeagueSystem[country->mId - 1];
                    else {
                        numSpareClubsToAdd = 0;
                        if (!mLeaguesSystem[country->mId - 1].empty()) {
                            for (auto &ll : (*(mLeaguesSystem[country->mId - 1].rbegin())).second)
                                numSpareClubsToAdd += ll->mNumTeams / 2;
                        }
                    }
                    if (numSpareClubsToAdd > 100)
                        numSpareClubsToAdd = 100;
                }
                else if (country->GetProperty<Bool>(L"SimulateLeague")
                    || country->mId == FifamNation::Canada || country->mId == FifamNation::Cameroon
                    || country->mId == FifamNation::Cote_d_Ivoire || country->mId == FifamNation::Ghana)
                {
                    numSpareClubsToAdd = 8;
                }
                if ((numSpareClubsToAdd + country->mClubs.size()) >= maxClubs)
                    numSpareClubsToAdd = maxClubs - country->mClubs.size();
                if (numSpareClubsToAdd > spareClubs[country->mId - 1].size())
                    numSpareClubsToAdd = spareClubs[country->mId - 1].size();
                for (UInt i = 0; i < numSpareClubsToAdd; i++) {
                    foom::club *team = spareClubs[country->mId - 1][i];
                    CreateAndConvertClub(gameId, team, team, country, nullptr, mNumTeamsInLeagueSystem[country->mId - 1] == 0);
                }
            }
        }
    }

    // setup club players
    std::wcout << L"Collecting club players..." << std::endl;

    UInt maxAllowedPlayers = 80;
    UInt maxAllowedPlayersWithYouth = 180;
    UInt minPlayersForFirstTeam = 25;
    UInt maxPlayersForFirstTeam = 30;
    UInt minPlayersForReserveTeam1 = 20;
    UInt minPlayersForReserveTeam2 = 30;
    if (gameId <= 7) {
        maxAllowedPlayers = 40;
        maxAllowedPlayersWithYouth = 40;
        minPlayersForFirstTeam = 25;
        maxPlayersForFirstTeam = 25;
        minPlayersForReserveTeam1 = 10;
        minPlayersForReserveTeam2 = 15;
    }
    else if (gameId <= 12) {
        maxAllowedPlayers = 75;
        maxAllowedPlayersWithYouth = 75;
        minPlayersForFirstTeam = 25;
        maxPlayersForFirstTeam = 30;
        minPlayersForReserveTeam1 = 20;
        minPlayersForReserveTeam2 = 30;
    }

    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &player = entry.second;
        auto contractClub = player.mContract.mClub;
        // player must have a contract
        if (contractClub) {
            auto loanClub = player.mLoan.mClub;
            if (loanClub && (player.mLoan.mEndDate <= GetCurrentDate() || loanClub == contractClub))
                loanClub = nullptr;
            if (contractClub->mConverterData.mFifamClub) { // if contracted club exists in the db
                if (loanClub) { // if player is loaned
                    if (loanClub->mConverterData.mFifamClub) { // if his current club exists in the db
                        loanClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub;
                    }
                    else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned club is reserve team
                        player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                        loanClub->mConverterData.mParentClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub;
                    }
                }
                else
                    contractClub->mConverterData.mContractedPlayers.push_back(&player);
            }
            else if (contractClub->mConverterData.mParentClub && contractClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // playing in reserve team
                if (loanClub) { // if player is loaned
                    if (loanClub->mConverterData.mFifamClub) { // if his current club exists in the db
                        loanClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mParentClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub->mConverterData.mParentClub;
                    }
                    else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned club is reserve team
                        player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                        loanClub->mConverterData.mParentClub->mConverterData.mLoanedPlayers.push_back(&player);
                        contractClub->mConverterData.mParentClub->mConverterData.mLoanedOutPlayers.push_back(&player);
                        player.mConverterData.mLoanedFromClub = contractClub->mConverterData.mParentClub;
                    }
                }
                else {
                    player.mConverterData.mTeamType = contractClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                    contractClub->mConverterData.mParentClub->mConverterData.mContractedPlayers.push_back(&player);
                }
            }
            else if (loanClub) { // we can create player if he's loaned and his current club exists in the db
                if (loanClub->mConverterData.mFifamClub) { // loaned to first team
                    loanClub->mConverterData.mContractedPlayers.push_back(&player);
                }
                else if (loanClub->mConverterData.mParentClub && loanClub->mConverterData.mParentClub->mConverterData.mFifamClub) { // loaned to reserve team
                    player.mConverterData.mTeamType = loanClub->mConverterData.IsChildTypeYouthTeam() ? 2 : 1;
                    loanClub->mConverterData.mParentClub->mConverterData.mContractedPlayers.push_back(&player);
                }
            }
        }
        // check future transfer
        Date loanEndDate(1, 1, 1900);
        if (player.mConverterData.mLoanedFromClub)
            loanEndDate = player.mLoan.mEndDate;
        if (player.mFutureTransfer.mClub
            && (player.mFutureTransfer.mNewJob == 0 || player.mFutureTransfer.mNewJob == 11)
            && player.mFutureTransfer.mTransferDate > GetCurrentDate()
            && player.mFutureTransfer.mClub != player.mContract.mClub)
        {
            if (player.mFutureTransfer.mTransferDate <= loanEndDate) {
                //Error(L"Player is loaned and transferred at same time\nPlayer: %s\nPlayerID: %d", player.mFullName.c_str(), player.mID);
            }
            else {
                if (player.mFutureTransfer.mClub->mConverterData.mFifamClub) {
                    player.mFutureTransfer.mClub->mConverterData.mFutureSigns.push_back(&player);
                    player.mConverterData.mFutureClub = player.mFutureTransfer.mClub;
                }
                else if (player.mFutureTransfer.mClub->mConverterData.mParentClub && player.mFutureTransfer.mClub->mConverterData.mParentClub->mConverterData.mFifamClub) {
                    player.mFutureTransfer.mClub->mConverterData.mParentClub->mConverterData.mFutureSigns.push_back(&player);
                    player.mConverterData.mFutureClub = player.mFutureTransfer.mClub->mConverterData.mParentClub;
                }
            }
        }
    }
    
    // finish club conversion
    std::wcout << L"Finishing clubs conversion, sorting players..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        // team is present in db
        if (team.mConverterData.mFifamClub) {
            FifamClub *dst = (FifamClub *)team.mConverterData.mFifamClub;
            FifamCountry *country = dst->mCountry;
            // friendship club
            foom::club *bestFriendshipClub = nullptr;
            if (team.mConverterData.mOriginalStoredParentClub && team.mConverterData.mOriginalStoredParentClub->mConverterData.mFifamClub)
                bestFriendshipClub = team.mConverterData.mOriginalStoredParentClub;
            else {
                Int bestFriendshipRep = 0;
                // for all affiliated clubs
                for (auto &a : team.mVecAffiliations) {
                    if (a.mAffiliatedClub && a.mAffiliatedClub->mConverterData.mFifamClub && (a.mAffiliationType == 1 || a.mAffiliationType == 16)) {
                        FifamClub *affiliatedFifamClub = (FifamClub *)a.mAffiliatedClub->mConverterData.mFifamClub;
                        if (dst->mCountry->mId == affiliatedFifamClub->mCountry->mId) {
                            if (a.mStartDate <= GetCurrentDate() && (a.mEndDate == FmEmptyDate() || a.mEndDate > GetCurrentDate())) {
                                if (!bestFriendshipClub || a.mAffiliatedClub->mReputation > bestFriendshipRep) {
                                    bestFriendshipClub = a.mAffiliatedClub;
                                    bestFriendshipRep = a.mAffiliatedClub->mReputation;
                                }
                            }
                        }
                    }
                }
            }
            if (bestFriendshipClub)
                dst->mPartnershipClub.mPtr = (FifamClub *)bestFriendshipClub->mConverterData.mFifamClub;
            // rival clubs
            Vector<foom::club::rival_club *> rivalClubs;
            for (auto &r : team.mVecRivalClubs) {
                if (r.mRivalClub && r.mRivalClub->mConverterData.mFifamClub) {
                    FifamClub *rivalFifamClub = (FifamClub *)r.mRivalClub->mConverterData.mFifamClub;
                    if (dst->mCountry->mId == rivalFifamClub->mCountry->mId)
                        rivalClubs.push_back(&r);
                }
            }
            if (!rivalClubs.empty()) {
                if (rivalClubs.size() > 1) {
                    std::sort(rivalClubs.begin(), rivalClubs.end(), [](foom::club::rival_club *a, foom::club::rival_club *b) {
                        if (a->mLevel > b->mLevel) return true;
                        if (b->mLevel > a->mLevel) return false;
                        if (a->mRivalClub->mReputation > b->mRivalClub->mReputation) return true;
                        if (b->mRivalClub->mReputation > a->mRivalClub->mReputation) return false;
                        return false;
                    });
                }
                UInt numRivalClubs = Utils::Min(4u, rivalClubs.size());
                for (UInt i = 0; i < numRivalClubs; i++)
                    dst->mRivalClubs.push_back((FifamClub *)(rivalClubs[i]->mRivalClub->mConverterData.mFifamClub));
            }
            // lowest leagues
            Map<UInt, Vector<FifamCompLeague *>> availableLowestLeagues;
            FifamCompLeague *clubLeague = dst->GetProperty<FifamCompLeague *>(L"league", nullptr);
            if (clubLeague && clubLeague->mLeagueLevel > 0 && mLeaguesSystem[country->mId - 1][clubLeague->mLeagueLevel].size() > 1)
                availableLowestLeagues[clubLeague->mLeagueLevel].push_back(clubLeague);
            if (!team.mVecRegionalDivisions.empty()) {
                std::sort(team.mVecRegionalDivisions.begin(), team.mVecRegionalDivisions.end(),
                [](foom::club::regional_division const &a, foom::club::regional_division const &b) {
                    return a.mLevel < b.mLevel;
                });
                for (auto const &r : team.mVecRegionalDivisions) {
                    if (r.mDivision && r.mDivision->mConverterData.mLeague) {
                        FifamCompLeague *league = (FifamCompLeague *)r.mDivision->mConverterData.mLeague;
                        if (league->mID.mRegion.ToInt() == country->mId) {
                            if (league->mLeagueLevel > 0 && mLeaguesSystem[country->mId - 1][league->mLeagueLevel].size() > 1)
                                availableLowestLeagues[league->mLeagueLevel].push_back(league);
                        }
                    }
                }
            }
            for (auto &le : availableLowestLeagues) {
                if (dst->mLowestLeagues.size() >= 3)
                    break;
                if (le.second.size() == 1)
                    dst->mLowestLeagues.push_back(le.second[0]->mID);
                else {
                    for (FifamCompLeague *l : le.second) {
                        if (l == clubLeague) {
                            dst->mLowestLeagues.push_back(l->mID);
                            break;
                        }
                    }
                }
            }

            UInt numPlayersInCurrentSquad = team.mConverterData.mContractedPlayers.size()
                + team.mConverterData.mLoanedPlayers.size() + team.mConverterData.mFutureSigns.size();
            UInt numFuturePlayers = team.mConverterData.mFutureSigns.size();
            UInt possibleFutureSquadSize = numPlayersInCurrentSquad + team.mConverterData.mLoanedOutPlayers.size();

            if (possibleFutureSquadSize > maxAllowedPlayers) {
                // create possible future squad
                Vector<foom::player *> possibleFutureSquad;

                if (!team.mConverterData.mContractedPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mContractedPlayers.begin(), team.mConverterData.mContractedPlayers.end());
                if (!team.mConverterData.mLoanedOutPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mLoanedOutPlayers.begin(), team.mConverterData.mLoanedOutPlayers.end());
                if (!team.mConverterData.mLoanedPlayers.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mLoanedPlayers.begin(), team.mConverterData.mLoanedPlayers.end());
                if (!team.mConverterData.mFutureSigns.empty())
                    possibleFutureSquad.insert(possibleFutureSquad.end(), team.mConverterData.mFutureSigns.begin(), team.mConverterData.mFutureSigns.end());

                // sort by CA/PA
                std::sort(possibleFutureSquad.begin(), possibleFutureSquad.end(), [](foom::player *a, foom::player *b) {
                    if (a->mCurrentAbility > b->mCurrentAbility) return true;
                    if (b->mCurrentAbility > a->mCurrentAbility) return false;
                    if (a->mPotentialAbility > b->mPotentialAbility) return true;
                    if (b->mPotentialAbility > a->mPotentialAbility) return false;
                    return false;
                });

                for (UInt i = maxAllowedPlayers; i < possibleFutureSquad.size(); i++)
                    possibleFutureSquad[i]->mConverterData.mFitsIntoDbLimit = false;
            }
        }
    }

    std::wcout << L"Calculating clubs national prestige..." << std::endl;
    for (FifamCountry *country : mFifamDatabase->mCountries) {
        if (country && !country->mClubs.empty()) {
            std::sort(country->mClubs.begin(), country->mClubs.end(), [](FifamClub *a, FifamClub *b) {
                foom::club *foomClubA = a->GetProperty<foom::club *>(L"foom::club");
                foom::club *foomClubB = b->GetProperty<foom::club *>(L"foom::club");
                if (foomClubA->mReputation > foomClubB->mReputation) return true;
                if (foomClubB->mReputation > foomClubA->mReputation) return false;
                if (a->mHistory.mLeagueWinYears.size() > b->mHistory.mLeagueWinYears.size()) return true;
                if (b->mHistory.mLeagueWinYears.size() > a->mHistory.mLeagueWinYears.size()) return false;
                return false;
            });
            Int maxRep = country->mClubs[0]->GetProperty<foom::club *>(L"foom::club")->mReputation;
            if (maxRep <= 0)
                maxRep = 1;
            for (UInt i = 0; i < country->mClubs.size(); i++) {
                foom::club *foomClub = country->mClubs[i]->GetProperty<foom::club *>(L"foom::club");
                Int rep = foomClub->mReputation;
                Int natRep = (Int)(9300.0f * ((Float)rep / (Float)maxRep));
                static Pair<Int, UChar> nationalRepTable[20] = {
                    { 9000, 20 },
                    { 8750, 19 },
                    { 8500, 18 },
                    { 8250, 17 },
                    { 8000, 16 },
                    { 7750, 15 },
                    { 7500, 14 },
                    { 7250, 13 },
                    { 7000, 12 },
                    { 6700, 11 },
                    { 6400, 10 },
                    { 6000,  9 },
                    { 5750,  8 },
                    { 5500,  7 },
                    { 5000,  6 },
                    { 4500,  5 },
                    { 4000,  4 },
                    { 3000,  3 },
                    { 2000,  2 },
                    { 1000,  1 }
                };
                Int natPrestige = 0;
                for (auto const &ip : nationalRepTable) {
                    if (natRep >= ip.first) {
                        natPrestige = ip.second;
                        break;
                    }
                }
                Int maxAllowedPrestigeForPlace = 13;
                if (i < 2)
                    maxAllowedPrestigeForPlace = 20;
                else if (i < 3)
                    maxAllowedPrestigeForPlace = 19;
                else if (i < 4)
                    maxAllowedPrestigeForPlace = 18;
                else if (i < 5)
                    maxAllowedPrestigeForPlace = 17;
                else if (i < 6)
                    maxAllowedPrestigeForPlace = 16;
                else if (i < 7)
                    maxAllowedPrestigeForPlace = 15;
                else if (i < 8)
                    maxAllowedPrestigeForPlace = 14;
                if (natPrestige > maxAllowedPrestigeForPlace)
                    natPrestige = maxAllowedPrestigeForPlace;

                if (!foomClub->mConverterData.mParentClub && foomClub->mConverterData.mOriginalStoredParentClub) {
                    if (natPrestige > 16)
                        natPrestige = 2;
                    else if (natPrestige > 11)
                        natPrestige = 1;
                    else
                        natPrestige = 0;
                }

                if (natPrestige > country->mClubs[i]->mNationalPrestige)
                    country->mClubs[i]->mNationalPrestige = natPrestige;
            }
        }
    }
    
    std::wcout << L"Creating players and staff..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        // team is present in db
        if (team.mConverterData.mFifamClub && team.mConverterData.mHasFifamSquad) {
            //std::wcout << L"Team: " << team.mID << std::endl;
            UInt howManyPlayersCanIAdditionalyCreate = 0;
            UInt howManyYouthPlayersCanIAdditionalyCreate = 0;
            UInt potentialSquadSize =
                std::count_if(team.mConverterData.mContractedPlayers.begin(), team.mConverterData.mContractedPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mLoanedPlayers.begin(), team.mConverterData.mLoanedPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mLoanedOutPlayers.begin(), team.mConverterData.mLoanedOutPlayers.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            }) + std::count_if(team.mConverterData.mFutureSigns.begin(), team.mConverterData.mFutureSigns.end(), [](foom::player *a) {
                return a->mConverterData.mFitsIntoDbLimit;
            });
            if (potentialSquadSize < maxAllowedPlayers)
                howManyPlayersCanIAdditionalyCreate = maxAllowedPlayers - potentialSquadSize;
            if (potentialSquadSize < maxAllowedPlayersWithYouth)
                howManyYouthPlayersCanIAdditionalyCreate = maxAllowedPlayersWithYouth - potentialSquadSize;
            FifamClub *dst = (FifamClub *)team.mConverterData.mFifamClub;

            // create players
            for (auto &p : team.mConverterData.mContractedPlayers) {
                if (p->mConverterData.mFitsIntoDbLimit) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    if (p->mConverterData.mTeamType == 1) {
                        player->mInReserveTeam = true;
                        if (p->mContract.mSquadNumber > 0)
                            player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                    }
                    else if (p->mConverterData.mTeamType == 2) {
                        if (player->GetAge(GetCurrentDate()) <= 18)
                            player->mInYouthTeam = true;
                        else {
                            player->mInReserveTeam = true;
                            if (p->mContract.mSquadNumber > 0)
                                player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                        }
                    }
                    else {
                        if (!dst->mCaptains[0] && p == team.mCaptain) {
                            player->mIsCaptain = true;
                            dst->mCaptains[0] = player;
                        }
                        if (p->mContract.mSquadNumber > 0)
                            player->mShirtNumberFirstTeam = p->mContract.mSquadNumber;
                    }
                }
                else {
                    if (FifamUtils::GetAge(p->mDateOfBirth, GetCurrentDate()) <= 18) {
                        if (howManyYouthPlayersCanIAdditionalyCreate > 0) {
                            FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                            player->mInYouthTeam = true;
                            if (howManyPlayersCanIAdditionalyCreate > 0)
                                howManyPlayersCanIAdditionalyCreate--;
                            howManyYouthPlayersCanIAdditionalyCreate--;
                        }
                    }
                    else {
                        if (howManyPlayersCanIAdditionalyCreate > 0) {
                            FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                            if (p->mConverterData.mTeamType == 1 || p->mConverterData.mTeamType == 2) {
                                player->mInReserveTeam = true;
                                if (p->mContract.mSquadNumber > 0)
                                    player->mShirtNumberReserveTeam = p->mContract.mSquadNumber;
                            }
                            else {
                                if (!dst->mCaptains[0] && p == team.mCaptain) {
                                    player->mIsCaptain = true;
                                    dst->mCaptains[0] = player;
                                }
                                if (p->mContract.mSquadNumber > 0)
                                    player->mShirtNumberFirstTeam = p->mContract.mSquadNumber;
                            }
                            howManyPlayersCanIAdditionalyCreate--;
                            if (howManyYouthPlayersCanIAdditionalyCreate > 0)
                                howManyYouthPlayersCanIAdditionalyCreate--;
                        }
                    }
                }
            }
            for (auto &p : team.mConverterData.mLoanedPlayers) {
                if (p->mConverterData.mFitsIntoDbLimit && p->mConverterData.mTeamType != 2) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    if (p->mConverterData.mTeamType == 1) {
                        player->mInReserveTeam = true;
                        if (p->mLoan.mSquadNumber > 0)
                            player->mShirtNumberReserveTeam = p->mLoan.mSquadNumber;
                    }
                    else {
                        if (!dst->mCaptains[0] && p == team.mCaptain) {
                            player->mIsCaptain = true;
                            dst->mCaptains[0] = player;
                        }
                        if (p->mLoan.mSquadNumber > 0)
                            player->mShirtNumberFirstTeam = p->mLoan.mSquadNumber;
                    }
                    Date loanStartDate = p->mLoan.mStartDate;
                    if (loanStartDate <= FmEmptyDate() || loanStartDate > GetCurrentDate())
                        loanStartDate = GetCurrentDate();
                    else
                        loanStartDate = GetDateAlignedToSeasonStart(loanStartDate, true, false);
                    Date loanEndDate = GetDateAlignedToSeasonEnd(p->mLoan.mEndDate, true, true);
                    player->mStartingConditions.mLoan.Setup(loanStartDate, loanEndDate,
                        FifamClubLink((FifamClub *)p->mConverterData.mLoanedFromClub->mConverterData.mFifamClub),
                        p->mLoan.mFeeToBuy == -1 ? -1 : foom::db::convert_money(p->mLoan.mFeeToBuy));
                    if (player->mContract.mValidUntil < loanEndDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(loanEndDate, false, true);;
                }
            }
            // TODO: check if this works
            for (auto &p : team.mConverterData.mFutureSigns) {
                if (p->mConverterData.mFitsIntoDbLimit && !p->mContract.mClub) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, p, dst);
                    player->mStartingConditions.mFutureJoin.Setup(GetDateAlignedToSeasonEnd(p->mFutureTransfer.mTransferDate, true, true));
                    if (p->mConverterData.mTeamType == 1)
                        player->mInReserveTeam = true;
                    else if (p->mConverterData.mTeamType == 2)
                        player->mInYouthTeam = true;
                    if (player->mContract.mValidUntil <= player->mStartingConditions.mFutureJoin.mDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(player->mStartingConditions.mFutureJoin.mDate, false, true);
                }
            }

            std::sort(dst->mPlayers.begin(), dst->mPlayers.end(), [=](FifamPlayer *a, FifamPlayer *b) {
                auto levelA = GetPlayerLevel(a, true, gameId);
                auto levelB = GetPlayerLevel(b, true, gameId);
                if (levelA > levelB) return true;
                if (levelB > levelA) return false;
                if (a->mTalent > b->mTalent) return true;
                if (b->mTalent > a->mTalent) return false;
                return false;
            });

            if (dst->mPlayers.size() > minPlayersForFirstTeam) {

                UInt numYouthPlayers = std::count_if(dst->mPlayers.begin(), dst->mPlayers.end(), [](FifamPlayer *a) {
                    return a->mInYouthTeam;
                });
                UInt numReservePlayers = std::count_if(dst->mPlayers.begin(), dst->mPlayers.end(), [](FifamPlayer *a) {
                    return a->mInReserveTeam;
                });
                UInt numFirstTeamPlayers = 0;

                UChar minFirstTeamRating = GetPlayerLevel(dst->mPlayers[minPlayersForFirstTeam], true, gameId);
                if (minFirstTeamRating > 0)
                    minFirstTeamRating -= 1;

                for (FifamPlayer *a : dst->mPlayers) {
                    if (!a->mInReserveTeam && !a->mInYouthTeam) {
                        if (numFirstTeamPlayers < minPlayersForFirstTeam)
                            numFirstTeamPlayers++;
                        else if (numFirstTeamPlayers < maxPlayersForFirstTeam && GetPlayerLevel(a, true, gameId) >= minFirstTeamRating)
                            numFirstTeamPlayers++;
                        else {
                            if (numReservePlayers > minPlayersForReserveTeam2) {
                                if (a->GetAge(GetCurrentDate()) <= 18)
                                    a->mInYouthTeam = true;
                                else {
                                    a->mInReserveTeam = true;
                                    numReservePlayers++;
                                }
                            }
                            else if (numReservePlayers > minPlayersForReserveTeam1) {
                                if (a->GetAge(GetCurrentDate()) <= 17)
                                    a->mInYouthTeam = true;
                                else {
                                    a->mInReserveTeam = true;
                                    numReservePlayers++;
                                }
                            }
                            else {
                                a->mInReserveTeam = true;
                                numReservePlayers++;
                            }
                        }
                    }
                }
            }

            Vector<Bool> FirstTeamNumbersUsed(100, false);
            Vector<Bool> ReserveTeamNumbersUsed(100, false);

            auto GetNextAvailableNumber = [](Vector<UChar> const &priorityNumbers, Vector<Bool> &numbers) -> UChar {
                for (UChar i : priorityNumbers) {
                    if (!numbers[i]) {
                        numbers[i] = true;
                        return i;
                    }
                }
                for (UChar i = 12; i <= 99; i++) {
                    if (!numbers[i]) {
                        numbers[i] = true;
                        return i;
                    }
                }
                return 0;
            };

            auto GetNumberForPlayer = [&](FifamPlayer *a, UChar currentNumber, Vector<Bool> &numbers) {
                if (currentNumber && !numbers[currentNumber]) {
                    numbers[currentNumber] = true;
                    return currentNumber;
                }
                UChar pos = a->mMainPosition.ToInt();
                if (pos == FifamPlayerPosition::GK)
                    return GetNextAvailableNumber({ 1, 12, 25, 23, 13, 99 }, numbers);
                else if (pos == FifamPlayerPosition::RB || pos == FifamPlayerPosition::RWB)
                    return GetNextAvailableNumber({ 2, 4, 3, 6, 5 }, numbers);
                else if (pos == FifamPlayerPosition::LB || pos == FifamPlayerPosition::LWB)
                    return GetNextAvailableNumber({ 3, 5, 2, 6, 4 }, numbers);
                else if (pos == FifamPlayerPosition::CB || pos == FifamPlayerPosition::SW)
                    return GetNextAvailableNumber({ 3, 4, 5, 2, 6 }, numbers);
                else if (pos == FifamPlayerPosition::ANC || pos == FifamPlayerPosition::DM)
                    return GetNextAvailableNumber({ 4, 6, 5, 7 }, numbers);
                else if (pos == FifamPlayerPosition::CM)
                    return GetNextAvailableNumber({ 6, 8, 7, 5 }, numbers);
                else if (pos == FifamPlayerPosition::RM || pos == FifamPlayerPosition::LM)
                    return GetNextAvailableNumber({ 7, 11, 10, 8, 6, 17 }, numbers);
                else if (pos == FifamPlayerPosition::AM)
                    return GetNextAvailableNumber({ 10, 7, 11, 8, 6, 20 }, numbers);
                else if (pos == FifamPlayerPosition::RW || pos == FifamPlayerPosition::LW)
                    return GetNextAvailableNumber({ 7, 10, 11, 9, 8, 17 }, numbers);
                else if (pos == FifamPlayerPosition::ST || pos == FifamPlayerPosition::CF)
                    return GetNextAvailableNumber({ 9, 10, 7, 11, 8, 19 }, numbers);
                return GetNextAvailableNumber({}, numbers);
            };

            for (FifamPlayer *a : dst->mPlayers) {
                if (a->mShirtNumberFirstTeam < 0 || a->mShirtNumberFirstTeam > 99)
                    a->mShirtNumberFirstTeam = 0;
                if (a->mShirtNumberReserveTeam < 0 || a->mShirtNumberReserveTeam > 99)
                    a->mShirtNumberReserveTeam = 0;
                if (a->mInYouthTeam) {
                    a->mShirtNumberFirstTeam = 0;
                    a->mShirtNumberReserveTeam = 0;
                }
                else if (a->mInReserveTeam) {
                    if (!a->mShirtNumberReserveTeam && a->mShirtNumberFirstTeam)
                        a->mShirtNumberReserveTeam = a->mShirtNumberFirstTeam;
                    a->mShirtNumberReserveTeam = GetNumberForPlayer(a, a->mShirtNumberReserveTeam, ReserveTeamNumbersUsed);
                    if (a->mShirtNumberFirstTeam) {
                        if (!FirstTeamNumbersUsed[a->mShirtNumberFirstTeam])
                            FirstTeamNumbersUsed[a->mShirtNumberFirstTeam] = true;
                        else
                            a->mShirtNumberFirstTeam = 0;
                    }
                }
                else
                    a->mShirtNumberFirstTeam = GetNumberForPlayer(a, a->mShirtNumberFirstTeam, FirstTeamNumbersUsed);
            }

            CreateStaffMembersForClub(gameId, &team, dst, false);
        }
    }

    std::wcout << L"Processing buy-back clauses..." << std::endl;
    for (auto &entry : mFoomDatabase->mClubs) {
        auto &team = entry.second;
        for (auto &b : team.mVecBuyBackClauses) {
            if (b.mBuyBackFee > 0 && b.mPlayer && b.mPlayer->mConverterData.mFifamPerson && b.mFromClub && b.mFromClub->mConverterData.mFifamClub) {
                FifamPlayer *player = (FifamPlayer *)b.mPlayer->mConverterData.mFifamPerson;
                if (!player->mContract.mBuyBackClauseEnabled) {
                    player->mContract.mBuyBackClauseEnabled = true;
                    player->mContract.mBuyBackClauseClub = (FifamClub *)b.mFromClub->mConverterData.mFifamClub;
                    player->mContract.mBuyBackClauseCost = foom::db::convert_money(b.mBuyBackFee);
                    if (player->mContract.mValidUntil.year > GetCurrentDate().year + 1)
                        player->mContract.mBuyBackClauseValidUntil = player->mContract.mValidUntil.DecreasedByOneYear();
                    else
                        player->mContract.mBuyBackClauseValidUntil = player->mContract.mValidUntil;
                }
            }
        }
    }

    // create free agents
    std::wcout << L"Creating free agents..." << std::endl;
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFitsIntoDbLimit) {
            if (!p.mConverterData.mFifamPerson) {
                if (!p.mContract.mClub && p.mCurrentAbility >= 110) {
                    FifamPlayer *player = CreateAndConvertPlayer(gameId, &p, nullptr);
                }
            }
            else {
                if (p.mConverterData.mFutureClub) {
                    FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
                    FifamDate transferDate = GetDateAlignedToSeasonStart(p.mFutureTransfer.mTransferDate, true, true);
                    FifamDate contractEndDate = GetDateAlignedToSeasonEnd(p.mFutureTransfer.mContractEndDate, true, true);
                    if (contractEndDate < transferDate)
                        contractEndDate = GetDateAlignedToSeasonEnd(transferDate, false, true);
                    else
                        contractEndDate = GetDateAlignedToSeasonEnd(contractEndDate, false, true);
                    player->mStartingConditions.mFutureTransfer.Setup(transferDate, contractEndDate,
                        FifamClubLink((FifamClub *)p.mConverterData.mFutureClub->mConverterData.mFifamClub),
                        p.mFutureTransfer.mTransferFee > 0 ? foom::db::convert_money(p.mFutureTransfer.mTransferFee) : 0);
                    if (player->mContract.mValidUntil <= transferDate)
                        player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(transferDate, false, true);
                }
            }
        }
    }

    // create spare staff
    std::wcout << L"Creating spare managers/staff..." << std::endl;
    for (auto &entry : mFoomDatabase->mNonPlayers) {
        auto &p = entry.second;
        if (IsConvertable(&p, gameId) && !p.mConverterData.mFifamPerson) {
            if (!p.mClubContract.mClub && !p.mNationContract.mNation && p.mCurrentAbility >= 100) {
                auto bestPosition = Utils::GetMaxElementId<Int, FifamClubStaffPosition>({
                    { p.mJobManager, FifamClubStaffPosition::Manager },
                    { p.mJobAssistantManager, FifamClubStaffPosition::AssistantCoach },
                    { p.mJobCoach, FifamClubStaffPosition::AssistantCoach },
                    { p.mJobScout, FifamClubStaffPosition::GeneralScout },
                    { p.mJobDirectorOfFootball, FifamClubStaffPosition::SportsDirector },
                    { p.mJobGkCoach, FifamClubStaffPosition::GoalkeeperCoach },
                    { p.mJobFitnessCoach, FifamClubStaffPosition::FitnessCoach },
                    { p.mJobSportsScientist, FifamClubStaffPosition::TeamDoctor },
                    { p.mJobPhysio, FifamClubStaffPosition::Masseur }
                });
                if (bestPosition.first >= 15 && bestPosition.first >= p.mJobDataAnalyst && bestPosition.first >= p.mJobChairman && bestPosition.first >= p.mJobHeadOfYouth)
                    FifamStaff *staff = CreateAndConvertStaff(&p, nullptr, bestPosition.second);
            }
        }
    }
    
    auto ProcessPersonFavouriteAndDislikedClubs = [](foom::person *p, FifamPerson *person) {
        if (!p->mVecFavouriteClubs.empty()) {
            Int maxRep = 0;
            FifamClub *bestFavouriteClub = nullptr;
            for (auto &f : p->mVecFavouriteClubs) {
                if (f.mClub && f.mLevel == 100 && f.mClub->mConverterData.mFifamClub) {
                    if (!bestFavouriteClub || f.mClub->mReputation > maxRep) {
                        bestFavouriteClub = (FifamClub *)f.mClub->mConverterData.mFifamClub;
                        maxRep = f.mClub->mReputation;
                    }
                }
            }
            if (bestFavouriteClub)
                person->mFavouriteClub = bestFavouriteClub;
        }
        if (!p->mVecDislikedClubs.empty()) {
            Int maxRep = 0;
            FifamClub *bestDislikedClub = nullptr;
            for (auto &d : p->mVecDislikedClubs) {
                if (d.mClub && d.mLevel == 100 && d.mClub->mConverterData.mFifamClub) {
                    if (!bestDislikedClub || d.mClub->mReputation > maxRep) {
                        bestDislikedClub = (FifamClub *)d.mClub->mConverterData.mFifamClub;
                        maxRep = d.mClub->mReputation;
                    }
                }
            }
            if (bestDislikedClub)
                person->mWouldnSignFor = bestDislikedClub;
        }
    };

    // player relations
    std::wcout << L"Setting up player relations..." << std::endl;
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson) {
            FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
            if (!p.mVecFavouritePeople.empty()) {
                for (auto f : p.mVecFavouritePeople) {
                    if (f.mPerson && f.mPerson->mIsPlayer && f.mPerson->mConverterData.mFifamPerson) {
                        if (f.mReason == 2) { // Brother
                            FifamPlayer *brother = (FifamPlayer *)f.mPerson->mConverterData.mFifamPerson;
                            player->mBrothers.insert(brother);
                            brother->mBrothers.insert(player);

                        }
                        else if (f.mReason == 4) { // Relation
                            FifamPlayer *cousin = (FifamPlayer *)f.mPerson->mConverterData.mFifamPerson;
                            player->mCousins.insert(cousin);
                            cousin->mCousins.insert(player);
                        }
                    }
                }
            }
            ProcessPersonFavouriteAndDislikedClubs(&p, player);
        }
    }
    for (auto &entry : mFoomDatabase->mNonPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson)
            ProcessPersonFavouriteAndDislikedClubs(&p, (FifamStaff *)p.mConverterData.mFifamPerson);
    }

    // process player history and experience
    std::wcout << L"Processing player history and experience..." << std::endl;
    for (auto &entry : mFoomDatabase->mPlayers) {
        auto &p = entry.second;
        if (p.mConverterData.mFifamPerson) {
            FifamPlayer *player = (FifamPlayer *)p.mConverterData.mFifamPerson;
            // history - validate years
            for (auto &h : p.mVecPlayingHistory) {
                if (h.mYear <= 0) {
                    Error(L"Wrong date in player history\nPlayer: %s\nPlayerID: %d", p.mFullName.c_str(), p.mID);
                    h.mYear = 1970;
                }
            };
            // history - sort history entries
            std::sort(p.mVecPlayingHistory.begin(), p.mVecPlayingHistory.end(), [](foom::player::playing_history const &a, foom::player::playing_history const &b) {
                if (a.mYear < b.mYear) return true;
                if (b.mYear < a.mYear) return true;
                return a.mOrder < b.mOrder;
            });

            // history - separate by years
            Map<Int, Vector<foom::player::playing_history *>> playerHistoryMap;
            for (auto &h : p.mVecPlayingHistory) {
                if (h.mYear <= CURRENT_YEAR && !h.mYouthTeam && h.mClub) { // ignore 'future' records and youth team
                    auto historyClub = GetTeamClubLink(h.mClub);
                    if (historyClub.IsValid() || h.mClub->mConverterData.mAdditionalHistoryReserveTeamType != 1)
                        playerHistoryMap[h.mYear].push_back(&h);
                }
            }

            auto GetPlayerHistorySeasonStartDate = [=](Int year) {
                if (year == CURRENT_YEAR)
                    return FifamDate(1, 1, CURRENT_YEAR);
                return FifamDate(1, 7, year);
            };

            auto GetPlayerHistorySeasonEndDate = [=](Int year, bool hasCurrentYear) {
                if (year == CURRENT_YEAR)
                    return FifamDate(30, 6, CURRENT_YEAR);
                else if (year == (CURRENT_YEAR - 1) && hasCurrentYear)
                    return FifamDate(31, 12, year);
                return FifamDate(30, 6, year + 1);
            };

            // history - fill player history
            foom::player::playing_history *lastEntry = nullptr;
            Vector<FifamPlayerHistoryEntry> history;
            bool hasCurrent = playerHistoryMap.count(CURRENT_YEAR) > 0;
            Int lastTransferFee = 0;
            for (auto const &e : playerHistoryMap) {
                UInt entryIndex = 0;
                for (auto &h : e.second) {
                    FifamPlayerHistoryEntry *last = nullptr;
                    if (!history.empty())
                        last = &history.back();
                    FifamPlayerHistoryEntry *curr = nullptr;
                    FifamClubLink historyClub;
                    if (h->mClub)
                        historyClub = GetTeamClubLink(h->mClub);
                    if (!historyClub.IsValid()) {
                        if (h->mClub->mConverterData.mAdditionalHistoryParentTeam && h->mClub->mConverterData.mAdditionalHistoryReserveTeamType == 0) {
                            if (h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub) {
                                historyClub = FifamClubLink(
                                    (FifamClub *)(h->mClub->mConverterData.mAdditionalHistoryParentTeam->mConverterData.mFifamClub), FifamClubTeamType::Reserve);
                            }
                        }
                    }
                    bool createNewEntry = true;
                    bool sameClub = false;

                    if (last) {
                        if (historyClub.mPtr && historyClub.mPtr == last->mClub.mPtr) {
                            sameClub = true;
                            if (h->mOnLoan == last->mLoan)
                                createNewEntry = false;
                        }
                        else if (lastEntry && lastEntry->mClub && h->mClub) {
                            if (h->mClub == lastEntry->mClub || h->mClub->mConverterData.mParentClub == lastEntry->mClub || lastEntry->mClub->mConverterData.mParentClub == h->mClub) {
                                sameClub = true;
                                if (h->mOnLoan == last->mLoan)
                                    createNewEntry = false;
                            }
                        }
                    }
                    Date startDate = GetPlayerHistorySeasonStartDate(h->mYear);
                    Date endDate = GetPlayerHistorySeasonEndDate(h->mYear, hasCurrent);
                    if (e.second.size() > 1 && endDate.month != 12) {
                        if (entryIndex == (e.second.size() - 1))
                            startDate.Set(1, 1, endDate.year);
                        else
                            endDate.Set(31, 12, endDate.year - 1);
                    }
                    if (!createNewEntry) {
                        curr = last;
                        curr->mEndDate = endDate;
                    }
                    else {
                        curr = &history.emplace_back();
                        curr->mClub = FifamClubLink(historyClub.mPtr);
                        curr->mStartDate = startDate;
                        curr->mEndDate = endDate;
                        curr->mLoan = h->mOnLoan;
                    }
                    if (!h->mYouthTeam) {
                        bool firstTeam = true;
                        if (historyClub.IsValid())
                            firstTeam = historyClub.mTeamType == FifamClubTeamType::First;
                        else
                            firstTeam = !h->mClub || !h->mClub->mConverterData.mParentClub;
                        if (firstTeam) { // MAYBE: also check division level?
                            curr->mMatches += h->mApps;
                            curr->mGoals += h->mGoals;
                        }
                        else {
                            curr->mReserveMatches += h->mApps;
                            curr->mReserveGoals += h->mGoals;
                        }
                    }
                    if (h->mOnLoan) {
                        if (h->mTransferFee > 0)
                            curr->mTransferFee = foom::db::convert_money(h->mTransferFee);
                    }
                    else if (lastTransferFee > 0)
                        curr->mTransferFee = foom::db::convert_money(lastTransferFee);
                    if (!h->mOnLoan)
                        lastTransferFee = h->mTransferFee;
                    if (sameClub && !curr->mLoan && last->mLoan) {
                        //Int lastFee = last->mTransferFee;
                        //last->mTransferFee = curr->mTransferFee;
                        //curr->mTransferFee = lastFee;
                    }
                    lastEntry = h;
                    entryIndex++;
                }
            }

            if (!history.empty()) {
                if (player->mClub) {
                    FifamPlayerHistoryEntry &last = history.back();
                    if (last.mClub.mPtr != player->mClub || last.mLoan != player->mStartingConditions.mLoan.Enabled()) {
                        FifamPlayerHistoryEntry &curr = history.emplace_back();
                        curr.mClub = FifamClubLink(player->mClub);
                        curr.mStartDate = FifamDate(1, 7, CURRENT_YEAR);
                        curr.mEndDate = FifamDate(1, 7, CURRENT_YEAR);
                        curr.mLoan = player->mStartingConditions.mLoan.Enabled();
                        if (!curr.mLoan && lastTransferFee > 0)
                            curr.mTransferFee = foom::db::convert_money(lastTransferFee);
                    }
                    history.back().mStillInThisClub = true;
                }
            }

            // NOTE: check this
            if (p.mCurrentAbility >= 0) {
                for (auto const &h : history)
                    player->mHistory.mEntries.insert(h);
            }

            // national/international experience
            UInt totalLeagueMatches = 0;
            for (auto &careerEntry : history)
                totalLeagueMatches += careerEntry.mMatches + careerEntry.mReserveMatches;
            UInt totalInternationalMatches = player->mNationalTeamMatches;
            if (totalLeagueMatches > 450)
                player->mNationalExperience = 6;
            else if (totalLeagueMatches > 350)
                player->mNationalExperience = 5;
            else if (totalLeagueMatches > 250)
                player->mNationalExperience = 4;
            else if (totalLeagueMatches > 150)
                player->mNationalExperience = 3;
            else if (totalLeagueMatches > 50)
                player->mNationalExperience = 2;
            else if (totalLeagueMatches > 20)
                player->mNationalExperience = 1;
            else
                player->mNationalExperience = 0;

            if (p.mInternationalApps > 85)
                player->mInternationalExperience = 4;
            else if (p.mInternationalApps >= 55)
                player->mInternationalExperience = 3;
            else if (p.mInternationalApps >= 30)
                player->mInternationalExperience = 2;
            else if (p.mInternationalApps >= 10)
                player->mInternationalExperience = 1;
        }
    }

    // add players to national teams
    std::wcout << L"Adding players to national teams..." << std::endl;
    for (auto &n : mNationalTeamPlayers) {
        if (!n.empty()) {
            if (n.size() > 1) {
                std::sort(n.begin(), n.end(), [](foom::player *a, foom::player *b) {
                    if (a->mCurrentAbility > b->mCurrentAbility) return true;
                    if (b->mCurrentAbility > a->mCurrentAbility) return false;
                    if (a->mInternationalApps > b->mInternationalApps) return true;
                    if (b->mInternationalApps > a->mInternationalApps) return false;
                    return false;
                });
            }
            UInt numPlayersInNationalTeam = 0;
            UInt numGoalkeepersInNationalTeam = 0;
            for (UInt i = 0; i < n.size(); i++) {
                Bool isGk = ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mMainPosition == FifamPlayerPosition::GK;
                if (isGk) {
                    if (numGoalkeepersInNationalTeam < 3) {
                        ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mCurrentlyInNationalTeam = true;
                        numGoalkeepersInNationalTeam++;
                    }
                }
                else {
                    if (numPlayersInNationalTeam < 22) {
                        ((FifamPlayer *)(n[i]->mConverterData.mFifamPerson))->mCurrentlyInNationalTeam = true;
                        numPlayersInNationalTeam++;
                    }
                }
                if (numGoalkeepersInNationalTeam == 3 && numPlayersInNationalTeam == 22)
                    break;
            }
        }
    }

    // convert referees
    std::wcout << L"Converting referess..." << std::endl;
    for (auto &entry : mFoomDatabase->mOfficials) {
        auto &official = entry.second;
        if (IsConvertable(&official, gameId) && official.mNation) {
            auto country = mFifamDatabase->GetCountry(official.mNation->mConverterData.mFIFAManagerReplacementID);
            if (country) {
                FifamReferee *referee = country->AddReferee();
                ConvertReferee(referee, &official);
            }
        }
    }

    // FIFA database conversion
    if (mFromFifaDatabase) {
        // referee
        FifaConverter::ConvertReferees(mFifamDatabase, mFifaDatabase);
        Set<FifaPlayer *> attachedPlayers;
        for (auto club : mFifamDatabase->mClubs) {
            if (club->mFifaID != 0) {
                FifaTeam *fifaTeam = mFifaDatabase->GetTeam(club->mFifaID);
                if (fifaTeam) {
                    if (fifaTeam->m_manager)
                        FifaConverter::ConvertManager(mFifamDatabase, club, fifaTeam->m_manager);
                    fifaTeam->ForAllPlayersEx([&](FifaPlayer &p, FifaPlayer::Position pos, UChar number) {
                        FifaConverter::ConvertPlayer(this, club, fifaTeam, &p, pos, number);
                        attachedPlayers.insert(&p);
                    });
                }
            }
        }
        mFifaDatabase->ForAllMalePlayers([&](FifaPlayer &p) {
            if (!Utils::Contains(attachedPlayers, &p)) {
                FifaConverter::ConvertPlayer(this, nullptr, nullptr, &p, FifaPlayer::POS_RES, 0);
            }
        });
    }

    // reference database
    if (mReferenceDatabase) {
        // create reference map
        Map<UInt, FifamClub *> referenceClubsByFifaID;
        for (FifamClub *c : mReferenceDatabase->mClubs) {
            if (c->mFifaID)
                referenceClubsByFifaID[c->mFifaID] = c;
        }
        // apply parameters
        for (FifamClub *c : mFifamDatabase->mClubs) {
            if (c->mFifaID) {
                auto it = referenceClubsByFifaID.find(c->mFifaID);
                if (it != referenceClubsByFifaID.end()) {
                    FifamClub *ref = (*it).second;
                    c->mMediaPressure = ref->mMediaPressure;
                    c->mAddress = ref->mAddress;
                    c->mTelephone = ref->mTelephone;
                    c->mWebsiteAndMail = ref->mWebsiteAndMail;
                    c->mFansites = ref->mFansites;
                    c->mFanMembers = ref->mFanMembers;
                    c->mAiStrategy = ref->mAiStrategy;
                    c->mLandscape = ref->mLandscape;
                    c->mTransfersCountry = ref->mTransfersCountry;
                    c->mMascotName = ref->mMascotName;
                    c->mPlayerInText = ref->mPlayerInText;
                    c->mFanName1 = ref->mFanName1;
                    c->mFanName2 = ref->mFanName2;
                    c->mTermForTeam1 = ref->mTermForTeam1;
                    c->mTermForTeam2 = ref->mTermForTeam2;
                    c->mAbbreviationArticle = ref->mAbbreviationArticle;
                    c->mPlayerInTextArticle = ref->mPlayerInTextArticle;
                    c->mFanName1Article = ref->mFanName1Article;
                    c->mClubNameUsageInPhrase = ref->mClubNameUsageInPhrase;
                    c->mClubNameUsageInPhrase2 = ref->mClubNameUsageInPhrase2;
                    c->mFanName2Article = ref->mFanName2Article;
                    c->mTermForTeam1Article =  ref->mTermForTeam1Article;
                    c->mTermForTeam2Article =  ref->mTermForTeam2Article;
                }
            }
        }
    }

    auto ProcessCompWinners = [=](Map<UShort, foom::team *> winnersMap, foom::nation *baseNation, UInt type) {
        if (!winnersMap.empty()) {
            for (auto we : winnersMap) {
                auto w = we.second;
                if (!w->mIsNation && (!baseNation || ((foom::club *)w)->mNation == baseNation)) {
                    if (type == 0)
                        ((foom::club *)w)->mConverterData.mLeagueWins.insert(we.first);
                    else if (type == 1)
                        ((foom::club *)w)->mConverterData.mCupWins.insert(we.first);
                    auto club = GetTeamClubLink(w);
                    if (club.IsValid()) {
                        if (type == 0)
                            club.mPtr->mHistory.mLeagueWinYears.push_back(we.first);
                        else if (type == 1)
                            club.mPtr->mHistory.mCupWinYears.push_back(we.first);
                        else if (type == 2)
                            club.mPtr->mHistory.mLeagueCupWinYears.push_back(we.first);
                        else if (type == 3)
                            club.mPtr->mHistory.mSuperCupsWinYears.push_back(we.first);
                        else if (type == 4)
                            club.mPtr->mHistory.mChampionsCupWinYears.push_back(we.first);
                        else if (type == 5)
                            club.mPtr->mHistory.mEuroTrophyWinYears.push_back(we.first);
                        else if (type == 7)
                            club.mPtr->mHistory.mWorldClubChampionshipWinYears.push_back(we.first);
                        else if (type == 8)
                            club.mPtr->mHistory.mWorldChampionshipWinYears.push_back(we.first);
                    }
                }
            }
        }
    };

    auto ProcessWinYearsForClub = [=](Vector<foom::comp *> const &comps, foom::nation *baseNation, UInt type) {
        if (!comps.empty())
            ProcessCompWinners(GetWinnersList(comps, type == 3 || type >= 6), baseNation, type);
    };

    std::wcout << L"Processing domestic competitions..." << std::endl;
    for (auto &ne : mFoomDatabase->mNations) {
        auto &n = ne.second;
        if (n.mConverterData.mFifamCountry) {
            ProcessWinYearsForClub(n.mConverterData.mMainLeagues, &n, 0);
            // cup
            {
                ProcessWinYearsForClub(n.mConverterData.mMainCups, &n, 1);
            }
            ProcessWinYearsForClub(n.mConverterData.mLeagueCups, &n, 2);
            ProcessWinYearsForClub(n.mConverterData.mSuperCups, &n, 3);
        }
    }

    std::wcout << L"Processing continental comps..." << std::endl;
    mFifamDatabase->mRules.mFairnessAwardWinners.clear();
    for (UInt i = 0; i < 6; i++) {
        mFifamDatabase->mRules.mContinentalCupChampions[i].mFirstCup.Clear();
        mFifamDatabase->mRules.mContinentalCupChampions[i].mSecondCup.Clear();
        mFifamDatabase->mRules.mContinentalCupChampions[i].mSuperCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mFirstCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mSecondCup.Clear();
        mFifamDatabase->mRules.mContinentalCupStadiums[i].mSuperCup.Clear();
    }

    auto ProcessContinentalComps = [&](Vector<Int> const &compIDs, FifamContinent continent, UInt type) {
        if (continent.ToInt() < 6) {
            if (type == 4)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mFirstCup.Clear();
            else if (type == 5)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSecondCup.Clear();
            else if (type == 6)
                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSuperCup.Clear();
        }
        Vector<foom::comp *> comps;
        for (Int id : compIDs) {
            foom::comp *comp = mFoomDatabase->get<foom::comp>(id);
            if (comp)
                comps.push_back(comp);
            if (!comps.empty()) {
                auto compWinners = GetWinnersList(comps, type >= 6);
                if (!compWinners.empty()) {
                    ProcessCompWinners(compWinners, nullptr, type);
                    if (continent.ToInt() < 6) {
                        if (type == 4) {
                            UShort lastYear = CURRENT_YEAR;
                            bool yearFound = compWinners.find(lastYear) != compWinners.end();
                            if (!yearFound) {
                                lastYear -= 1;
                                yearFound = compWinners.find(lastYear) != compWinners.end();
                            }
                            if (yearFound) {
                                if (!compWinners[lastYear]->mIsNation)
                                    mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mFirstCup = GetTeamClubLink(compWinners[lastYear]);
                            }
                        }
                        else if (compWinners.find(CURRENT_YEAR) != compWinners.end() && !compWinners[CURRENT_YEAR]->mIsNation) {
                            if (type == 5)
                                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSecondCup = GetTeamClubLink(compWinners[CURRENT_YEAR]);
                            else if (type == 6)
                                mFifamDatabase->mRules.mContinentalCupChampions[continent.ToInt()].mSuperCup = GetTeamClubLink(compWinners[CURRENT_YEAR]);
                        }
                    }
                }
            }
        }

    };

    ProcessContinentalComps({ 1301394 }, FifamContinent::Europe, 4);
    ProcessContinentalComps({ 1301396 }, FifamContinent::Europe, 5);
    ProcessContinentalComps({ 1301397 }, FifamContinent::Europe, 6);

    ProcessContinentalComps({ 102415 }, FifamContinent::SouthAmerica, 4);
    ProcessContinentalComps({ 317567 }, FifamContinent::SouthAmerica, 5);
    ProcessContinentalComps({ 102418 }, FifamContinent::SouthAmerica, 6);

    ProcessContinentalComps({ 51002641, 1301417 }, FifamContinent::NorthAmerica, 4);

    ProcessContinentalComps({ 127299 }, FifamContinent::Africa, 4);
    ProcessContinentalComps({ 12017574 }, FifamContinent::Africa, 5);
    ProcessContinentalComps({ 131273 }, FifamContinent::Africa, 6);

    ProcessContinentalComps({ 127286 }, FifamContinent::Asia, 4);
    ProcessContinentalComps({ 1001959 }, FifamContinent::Asia, 5);

    ProcessContinentalComps({ 127285 }, FifamContinent::Oceania, 4);

    ProcessContinentalComps({ 121092 }, FifamContinent::None, 7); // Club World Cup
    ProcessContinentalComps({ 1301393 }, FifamContinent::None, 8); // TOYOTA Cup

    for (auto &entry : mFoomDatabase->mComps) {
        auto &c = entry.second;
        if (c.mID == 1301394) { // UEFA Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mFirstCup, c.mName);
        }
        else if (c.mID == 1301396) { // UEFA Europa League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mSecondCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Europe].mSecondCup, c.mName);
        }
        else if (c.mID == 1301397) { // UEFA Super Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Europe].mSuperCup = GetCompHost(&c, true);
            if (gameId >= 12) {
                if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
                else
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, c.mName);
            }
        }
        else if (c.mID == 1301398) { // UEFA Intertoto Cup
            if (gameId <= 11) {
                if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
                else
                    FifamTrSetAll(mFifamDatabase->mRules.mEuropeanSuperCupName, c.mName);
            }
        }

        else if (c.mID == 102415) { // Copa Bridgestone Libertadores
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mFirstCup, c.mName);
        }
        else if (c.mID == 317567) { // Copa Total Sudamericana
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::SouthAmerica].mSecondCup, c.mName);
        }
        else if (c.mID == 102418) { // Recopa Santander Sudamericana
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::SouthAmerica].mSuperCup = GetCompHost(&c, true);
        }

        else if (c.mID == 51002641) { // Scotiabank CONCACAF Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::NorthAmerica].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::NorthAmerica].mFirstCup, c.mName);
        }

        else if (c.mID == 127299) { // Orange CAF Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mFirstCup, c.mName);
        }
        else if (c.mID == 12017574) { // Total CAF Confederation Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Africa].mSecondCup, c.mName);
        }
        else if (c.mID == 131273) { // Total CAF Super Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Africa].mSuperCup = GetCompHost(&c, true);
        }

        else if (c.mID == 127286) { // AFC Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Asia].mFirstCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mFirstCup, c.mName);
        }
        else if (c.mID == 1001959) { // AFC Cup
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Asia].mSecondCup = GetCompHost(&c, true);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mSecondCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Asia].mSecondCup, c.mName);
        }

        else if (c.mID == 127285) { // Fiji Airways OFC Champions League
            mFifamDatabase->mRules.mContinentalCupStadiums[FifamContinent::Oceania].mFirstCup = GetCompHost(&c);
            if (c.mName.length() > MAX_COMP_NAME_LENGTH)
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Oceania].mFirstCup, FifamNames::LimitName(c.mShortName, MAX_COMP_NAME_LENGTH));
            else
                FifamTrSetAll(mFifamDatabase->mRules.mContinentalCupNames[FifamContinent::Oceania].mFirstCup, c.mName);
        }

        else if (!c.mIsExtinct && c.mNation && c.mType == 2) { // domestic main cup
            FifamClubLink winner, runnerUp;
            GetCompWinnerAndRunnerUp(&c, winner, runnerUp, true);
            if (runnerUp.IsValid()) {
                if (runnerUp.mTeamType == FifamClubTeamType::First)
                    runnerUp.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
                else
                    runnerUp.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
            }
            if (winner.IsValid()) {
                if (winner.mTeamType == FifamClubTeamType::First)
                    winner.mPtr->mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
                else
                    winner.mPtr->mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
            }
        }
    }

    {
        std::wcout << L"Reading fifam_continental_comps.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_continental_comps.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String continentName, compTypeName;
                    Int hostTeamId = 0;
                    reader.ReadLineWithSeparator(L'\t', continentName, compTypeName, hostTeamId);
                    FifamContinent continent = continentName;
                    if (!continent.GetWasSetFromUnknown() && continent.ToInt() >= 0 && continent.ToInt() <= 5) {
                        FifamCompType compType = compTypeName;
                        if (!compType.GetWasSetFromUnknown()) {
                            if (hostTeamId != 0) {
                                foom::team *hostTeam = mFoomDatabase->get<foom::team>(hostTeamId);
                                if (compType == FifamCompType::ChampionsLeague)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mFirstCup = GetTeamClubLink(hostTeam);
                                else if (compType == FifamCompType::UefaCup)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mSecondCup = GetTeamClubLink(hostTeam);
                                if (compType == FifamCompType::SuperCup)
                                    mFifamDatabase->mRules.mContinentalCupStadiums[continent.ToInt()].mSuperCup = GetTeamClubLink(hostTeam);
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
            reader.Close();
        }
    }

    //{
    //    FifamWriter fifaPlayersWriter(L"fm-fifa-players.csv", 14, 0, 0);
    //    if (fifaPlayersWriter.Available()) {
    //        fifaPlayersWriter.WriteLine(L"Country,Level,Club,Name,Birthdate,Nationality,Rating,FoomID,FifaID");
    //        auto WriteOnePlayer = [&](foom::player *_player, foom::club *_club, Int level, FifaTeam *fifaClub) {
    //            String countryName;
    //            if (_club && _club->mNation)
    //                countryName = Quoted(_club->mNation->mShortName)();
    //            String clubName;
    //            if (_club)
    //                clubName = Quoted(_club->mShortName)();
    //            String fifaIdStr;
    //            if (_player->mNation && fifaClub) {
    //                fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
    //                    if (fifaIdStr.empty()) {
    //                        if (fifaPlayer.internal.nationality == _player->mNation->mConverterData.mFIFAID && fifaPlayer.m_birthDate == _player->mDateOfBirth)
    //                            fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
    //                    }
    //                });
    //                if (fifaIdStr.empty()) {
    //                    String foomPlayerName = Utils::GetQuickName(_player->mFirstName, _player->mSecondName, _player->mCommonName);
    //                    fifaClub->ForAllPlayers([&](FifaPlayer &fifaPlayer) {
    //                        if (fifaIdStr.empty()) {
    //                            if (fifaPlayer.m_quickName == foomPlayerName)
    //                                fifaIdStr = Utils::Format(L"%d", fifaPlayer.internal.playerid);
    //                        }
    //                    });
    //                }
    //            }
    //            String playerName;
    //            if (!_player->mCommonName.empty())
    //                playerName = _player->mCommonName;
    //            else {
    //                if (!_player->mFirstName.empty())
    //                    playerName = _player->mFirstName;
    //                if (!_player->mSecondName.empty()) {
    //                    if (!playerName.empty())
    //                        playerName += L" ";
    //                    playerName += _player->mSecondName;
    //                }
    //            }
    //            fifaPlayersWriter.WriteLine(
    //                countryName,
    //                level,
    //                clubName,
    //                Quoted(playerName),
    //                _player->mDateOfBirth,
    //                _player->mNation ? _player->mNation->mThreeLetterName : L"",
    //                _player->mCurrentAbility / 10,
    //                _player->mID,
    //                fifaIdStr);
    //        };
    //        for (auto &entry : mFoomDatabase->mClubs) {
    //            foom::club &team = entry.second;
    //            if (!team.mExtinct && team.mNation) {
    //                Int leagueLevel = 99;
    //                if (team.mDivision && team.mDivision->mCompetitionLevel > 0)
    //                    leagueLevel = team.mDivision->mCompetitionLevel;
    //
    //                FifaTeam *fifaClub = nullptr;
    //                if (team.mConverterData.mFIFAID != 0)
    //                    fifaClub = mFifaDatabase->GetTeam(team.mConverterData.mFIFAID);
    //
    //                for (foom::player *p : team.mConverterData.mContractedPlayers)
    //                    WriteOnePlayer(p, &team, leagueLevel, fifaClub);
    //                for (foom::player *p : team.mConverterData.mLoanedPlayers)
    //                    WriteOnePlayer(p, &team, leagueLevel, fifaClub);
    //
    //            }
    //        }
    //        FifaTeam *freeAgentsClub = mFifaDatabase->GetTeam(111592);
    //        for (auto &entry : mFoomDatabase->mPlayers) {
    //            foom::player p = entry.second;
    //            if (!p.mContract.mClub)
    //                WriteOnePlayer(&p, nullptr, 99, freeAgentsClub);
    //        }
    //        fifaPlayersWriter.Close();
    //    }
    //}

    FifamVersion version = FifamDatabase::GetGameDbVersion(gameId);
    std::wcout << L"Writing database..." << std::endl;
    mFifamDatabase->Write(gameId, version.GetYear(), version.GetNumber(),
        Utils::Format(writeToGameFolder ?
            L"D:\\Games\\FIFA Manager %02d\\database" :
            L"fm_test\\database",
            gameId).c_str());

    if (!mFromFifaDatabase) {
        if (gameId >= 11 && writeToGameFolder) {
            std::wcout << L"Writing derbies..." << std::endl;
            struct derby_info {
                String nameId;
                String name;
                FifamClubLink club1;
                FifamClubLink club2;
                Int reputation;
            };
            Vector<derby_info> derbies;
            for (auto &entry : mFoomDatabase->mDerbies) {
                foom::derby &d = entry.second;
                if (d.mTeam1 && d.mTeam2) {
                    derby_info i;
                    i.club1 = GetTeamClubLink(d.mTeam1, false); // NOTE: use 'true' for reserve teams
                    if (i.club1.IsValid()) {
                        i.club2 = GetTeamClubLink(d.mTeam2, false); // NOTE: use 'true' for reserve teams
                        if (i.club2.IsValid()) {
                            i.reputation = d.mWorldReputation * 1000 + d.mNationalReputation;
                            i.name = d.mName;
                            String nameId = Utils::ToUpper(Utils::GetStringWithoutUnicodeChars(d.mName));
                            i.nameId = L"IDS_DERBY_";
                            for (auto c : nameId) {
                                if (c != L'.') {
                                    if (c == L' ' || c == L'-')
                                        i.nameId.push_back(L'_');
                                    else
                                        i.nameId.push_back(c);
                                }
                            }
                            derbies.push_back(i);
                        }
                    }
                }
            }
            std::sort(derbies.begin(), derbies.end(), [](derby_info const &a, derby_info const &b) {
                return a.reputation > b.reputation;
            });
            FifamWriter derbiesWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\fmdata\\Parameter File - Derbies.txt", gameId),
                gameId, 0, false);
            if (derbiesWriter.Available()) {
                derbiesWriter.WriteLine(L"BEGIN( DERBIES )");
                derbiesWriter.WriteNewLine();
                derbiesWriter.WriteLine(L"    DERBY_COUNT = " + Utils::Format(L"%d", derbies.size()));
                derbiesWriter.WriteNewLine();
                UInt maxNameIdLength = 0;
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
                    UInt len = derbies[i].nameId.length();
                    if (len > maxNameIdLength)
                        maxNameIdLength = len;
                }
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
                    derbiesWriter.WriteLine(Utils::Format(L"    DERBY%03d = %-" + std::to_wstring(maxNameIdLength)
                        + L"s 0x%08X, 0x%08X                  // %s, %s",
                        i + 1, String(derbies[i].nameId + L",").c_str(), derbies[i].club1.mPtr->mUniqueID, derbies[i].club2.mPtr->mUniqueID,
                        Utils::GetStringWithoutUnicodeChars(derbies[i].club1.GetTeamName()).c_str(),
                        Utils::GetStringWithoutUnicodeChars(derbies[i].club2.GetTeamName()).c_str()));
                }
                derbiesWriter.WriteNewLine();
                derbiesWriter.Write(L"END");
                derbiesWriter.Close();
            }
            FifamWriter derbyNamesWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\plugins\\derbies.tr", gameId),
                gameId, 0, true);
            Set<String> writtenDerbyNames;
            if (derbyNamesWriter.Available()) {
                for (UInt i = 0; i < derbies.size(); i++) {
                    if (i >= 999) // NOTE: max possible derbies count
                        break;
                    if (!Utils::Contains(writtenDerbyNames, derbies[i].nameId)) {
                        derbyNamesWriter.WriteLine(derbies[i].nameId + L"|" + derbies[i].name);
                        writtenDerbyNames.insert(derbies[i].nameId);
                    }
                }
                derbyNamesWriter.Close();
            }
        }

        std::wcout << L"Writing female persons..." << std::endl;

        FifamWriter femaleStaffWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database\\FemaleStaff.sav", gameId), gameId, 0, true);
        femaleStaffWriter.WriteLine(L"FirstName,LastName,Pseudonym,Birthdate,Nationality,Job,ID");
        for (FifamStaff *staff : mFifamDatabase->mStaffs) {
            auto fmstaff = staff->GetProperty<foom::non_player *>(L"foom::non_player");
            if (fmstaff && fmstaff->mFemale) {
                Date birthdate = staff->mBirthday;
                femaleStaffWriter.WriteLine(Quoted(staff->mFirstName), Quoted(staff->mLastName), Quoted(staff->mPseudonym), birthdate, staff->mNationality[0],
                    staff->mClubPosition, staff->mWriteableStringID);
            }
        }

        FifamWriter femaleRefereeWriter(Utils::Format(L"D:\\Games\\FIFA Manager %02d\\database\\FemaleReferee.sav", gameId), gameId, 0, true);
        femaleRefereeWriter.WriteLine(L"FirstName,LastName,Pseudonym,Birthdate,Nationality,Job,ID");
        for (FifamReferee *ref : mFifamDatabase->mReferees) {
            auto fmofficial = ref->GetProperty<foom::official *>(L"foom::official");
            if (fmofficial && fmofficial->mFemale) {
                femaleRefereeWriter.WriteLine(Quoted(ref->mFirstName), Quoted(ref->mLastName), String(), Date(), ref->mCountry->mId,
                    0, FifamNames::GetPersonStringId(gameId, ref->mFirstName, ref->mLastName, String(), Date(), 0));
            }
        }
    }

#if 0
    GraphicsConverter graphicsConverter;
    if (!mFromFifaDatabase) {
        //graphicsConverter.mOnlyUpdates = true;
        //std::wcout << L"Converting club badges..." << std::endl;
        //graphicsConverter.ConvertClubBadges(mFoomDatabase, mAvailableBadges, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting portraits..." << std::endl;
        //graphicsConverter.ConvertPortraits(mFoomDatabase, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting competition badges..." << std::endl;
        //graphicsConverter.ConvertCompBadges(mFifamDatabase, graphicsPath, contentPath, gameId, 0);
        //std::wcout << L"Converting trophies..." << std::endl;
        //graphicsConverter.ConvertTrophies(mFifamDatabase, graphicsPath, contentPath, gameId, 0);
        //graphicsConverter.ConvertCities(mFoomDatabase, L"D:\\Downloads", L"D:\\Games\\FIFA Manager 13", gameId, 0);
        //graphicsConverter.ConvertStadiums(mFoomDatabase, L"D:\\Downloads", L"D:\\Games\\FIFA Manager 13", gameId, 0, false);
    }
    else {
        std::wcout << L"Converting FIFA club badges..." << std::endl;
        graphicsConverter.ConvertClubBadgesFIFA(mFifamDatabase, mFifaAssetsPath, contentPath, gameId);
        std::wcout << L"Converting FIFA competition badges..." << std::endl;
        graphicsConverter.ConvertCompBadgesFIFA(mFifamDatabase, mFifaAssetsPath, contentPath, gameId);
    }
#endif

    delete mReferenceDatabase;
    mReferenceDatabase = nullptr;
    delete mFifamDatabase;
    mFifamDatabase = nullptr;
    delete mFoomDatabase;
    mFoomDatabase = nullptr;
    delete mFifaDatabase;
    mFifaDatabase = nullptr;
}

FifamClub *Converter::CreateAndConvertClub(UInt gameId, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div, bool convertSquad) {
    if (!mainTeam)
        mainTeam = team;
    FifamClub *club = mFifamDatabase->CreateClub(country);
    team->mConverterData.mHasFifamSquad = convertSquad;
    club->SetProperty(L"foom::club", team);
    // uid
    club->mUniqueID = team->mConverterData.mFIFAManagerID;
    if (club->mUniqueID != 0) {
        UChar countryId = FifamUtils::GetCountryIDFromClubID(club->mUniqueID);
        if (countryId >= 1 && countryId <= FifamDatabase::NUM_COUNTRIES) {
            if (countryId != country->mId) {
                Error(L"Incorrect Club Country in UniqueID\nClub: '%s'\nUniqueID: %08X\nCountryId: %d\nIncorrectCountryId: %d",
                    team->mName.c_str(), club->mUniqueID, country->mId, countryId);
            }
        }
        else {
            Error(L"Incorrect Club UniqueID\nClub: '%s'\nUniqueID: %08X\nIncorrectCountryId: %d",
                team->mName.c_str(), club->mUniqueID, countryId);
        }
    }
    if (club->mUniqueID == 0)
        club->mUniqueID = (country->mId << 16) | mNextFreeUID[country->mId - 1]++;
    club->mFifaID = team->mConverterData.mFIFAID;

    if (mainTeam && mainTeam != team)
        ConvertReserveClub(gameId, club, team->mIsReserveToCreateClub ? mainTeam : team, mainTeam, country, div);
    else
        ConvertClub(gameId, club, team, mainTeam, country, div);

    {
        auto it = mPenaltyPointsMap.find(team->mID);
        if (it != mPenaltyPointsMap.end()) {
            club->mPenaltyType = FifamClubPenaltyType::Points;
            club->mPenaltyPoints = (*it).second;
        }
    }
    {
        auto it = mNamesMap.find(team->mID);
        if (it != mNamesMap.end()) {
            FifamTrSetAll<String>(club->mName, (*it).second);
            FifamTrSetAll<String>(club->mName2, (*it).second);
        }
    }
    {
        auto it = mShortNamesMap.find(team->mID);
        if (it != mShortNamesMap.end()) {
            FifamTrSetAll<String>(club->mShortName, (*it).second);
            FifamTrSetAll<String>(club->mShortName2, (*it).second);
        }
    }
    {
        auto it = mAbbreviationMap.find(team->mID);
        if (it != mAbbreviationMap.end())
            FifamTrSetAll<String>(club->mAbbreviation, (*it).second);
    }

    mFifamDatabase->AddClubToMap(club);
    country->mClubsMap[club->mUniqueID] = club;

    team->mConverterData.mFifamClub = club;

    return club;
}

Int StadNationalTeamUsagePriority(foom::stadium *stad) {
    switch (stad->mUsedByNationalTeam) {
    case 1:
        return 6;
    case 2:
        return 5;
    case 5:
        return 4;
    case 3:
        return 3;
    case 6:
        return 2;
    case 4:
        return 1;
    }
    return 0;
}

void Converter::ConvertNationInfo(FifamCountry *dst, foom::nation *nation, UInt gameId) {

    nation->mConverterData.mFifamCountry = dst;

    dst->mAssessmentData[5] = nation->mEuroCoeff6;
    dst->mAssessmentData[4] = nation->mEuroCoeff7;
    dst->mAssessmentData[3] = nation->mEuroCoeff8;
    dst->mAssessmentData[2] = nation->mEuroCoeff9;
    dst->mAssessmentData[1] = nation->mEuroCoeff10;
    dst->mAssessmentData[0] = nation->mEuroCoeff11;
    dst->mFifaRanking = nation->mRankingPoints;
    dst->mYearsForNaturalization = Utils::Clamp(nation->mYearsToGainNationality, 0, 9);
    // national team
    if (nation->mYearAssociationFormed != 0)
        dst->mNationalTeam.mYearOfFoundation = nation->mYearAssociationFormed;

    if (nation->mConverterData.mFIFATeamID != 0)
        dst->mNationalTeam.mFifaID = nation->mConverterData.mFIFATeamID;

    // national team stadium
    foom::stadium *nationalStadium = nation->mNationalStadium;
    if (!nationalStadium) {
        Vector<foom::stadium *> stads;
        for (auto &[stadId, stad] : mFoomDatabase->mStadiums) {
            if (stad.mNation == nation)
                stads.push_back(&stad);
        }
        if (!stads.empty()) {
            std::sort(stads.begin(), stads.end(), [](foom::stadium *a, foom::stadium *b) {
                if (StadNationalTeamUsagePriority(a) > StadNationalTeamUsagePriority(b)) return true;
                if (StadNationalTeamUsagePriority(a) < StadNationalTeamUsagePriority(b)) return false;
                if (a->mSeatingCapacity > b->mSeatingCapacity) return true;
                if (a->mSeatingCapacity < b->mSeatingCapacity) return false;
                return false;
            });
            nationalStadium = stads.front();
        }
    }

    if (nationalStadium) {
        dst->SetProperty<Int>(L"foom::stad_id", nationalStadium->mID);
        if (nationalStadium->mOwner)
            dst->SetProperty<foom::team *>(L"foom::stad_owner", nationalStadium->mOwner);
        FifamTrSetAll(dst->mNationalTeam.mStadiumName, FifamNames::LimitName(nationalStadium->mName, 29));
        if (nationalStadium->mCapacity > 0) {
            Int seatingCapacity = nationalStadium->mSeatingCapacity;
            if (seatingCapacity == 0 || seatingCapacity > nationalStadium->mCapacity)
                seatingCapacity = nationalStadium->mCapacity;
            dst->mNationalTeam.mStadiumSeatsCapacity = seatingCapacity;
            dst->mNationalTeam.mStadiumStandingsCapacity = nationalStadium->mCapacity - seatingCapacity;
        }
    }

    // wins
    dst->mNumWorldCups = (UShort)nation->mConverterData.mWorldCupWins.size();
    dst->mNumWorldCupRunnersUp = (UShort)nation->mConverterData.mWorldCupFinals.size();
    dst->mNumContinentalChampions = (UShort)nation->mConverterData.mContinentalCupWins.size();
    dst->mNumContinentalRunnersUp = (UShort)nation->mConverterData.mContinentalCupFinals.size();

    ConvertClubStadium(&dst->mNationalTeam, gameId);
    ConvertKitsAndColors(&dst->mNationalTeam, -1, nation->mVecKits, -1, nation->mBackgroundColor, nation->mForegroundColor, gameId);
    dst->mNationalTeam.mKit.mSpecialKitId = 0;
}

void Converter::ConvertClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div) {
    // name
    if (team->mName.length() < 30)
        FifamTrSetAll(dst->mName, team->mName);
    else
        FifamTrSetAll(dst->mName, FifamNames::LimitName(team->mShortName, 29));
    dst->mName2 = dst->mName;
    // short name
    FifamTrSetAll(dst->mShortName, FifamNames::LimitName(team->mShortName, 10));
    dst->mShortName2 = dst->mShortName;
    // abbr
    FifamTrSetAll(dst->mAbbreviation, FifamNames::GetClubAbbr(team->mShortName));
    // year of foundation & traditional club
    if (team->mYearFounded <= 0)
        dst->mYearOfFoundation = 2000;
    else {
        dst->mYearOfFoundation = team->mYearFounded;
        if ((CURRENT_YEAR - 90) > dst->mYearOfFoundation)
            dst->mTraditionalClub = true;
    }
    // city and settlement
    if (team->mCity)
        FifamTrSetAll(dst->mCityName, FifamNames::LimitName(team->mCity->mName, 29));
    else
        dst->mCityName = country->mNationalTeam.mCityName;
    // latitude/longitude
    if (team->mLatitude != 0 && team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;

    if (dst->mGeoCoords.mLatitude.ToInt() == 0 && dst->mGeoCoords.mLongitude.ToInt() == 0 && team->mNation) {
        for (auto &city : team->mNation->mConverterData.mCities) {
            if (city->mLatitude != 0 && city->mLongitude != 0) {
                dst->mGeoCoords.SetFromFloat(city->mLatitude, city->mLongitude);
                break;
            }
        }
    }

    // media pressure
    dst->mMediaPressure = FifamClubMediaPressure::Normal;
    // capital
    if (team->mBalance > 0 && team->mBalance < 300'000'000) {
        dst->mInitialCapital = foom::db::convert_money(team->mBalance);
    }
    else
        dst->mInitialCapital = Utils::Clamp(foom::db::convert_money(team->mBalance + team->mBalance / 2), 0, 300'000'000);
    // transfer budget
    if (team->mTransferBudget > 0) {
        if (team->mTransferBudget < 300'000'000)
            dst->mTransferBudget = foom::db::convert_money(team->mTransferBudget);
        else
            dst->mTransferBudget = Utils::Clamp(foom::db::convert_money(team->mTransferBudget + team->mTransferBudget / 2), 0, 500'000'000);
    }
    // joint-stock company
    dst->mJointStockCompany = team->mOwnershipType == 2 /*&& team->mReputation >= 5000*/; // Public Limited Company
    // rich guy controlled
    dst->mRichGuyControlled = team->mSugarDaddy >= 1 && team->mSugarDaddy <= 3; // Foreground, Background, Underwriter
    // sponsor
    foom::club::income *bestIncome = nullptr;
    Int bestIncomeValue = 0;
    foom::club::income *bestIndividualTvMoneyIncome = nullptr;
    Int bestIndividualTvMoneyValue = 0;
    for (auto &i : team->mVecIncomes) {
        if (i.mIncomeType >= 1 && i.mIncomeType <= 5) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor, Individual TV Deal
            if (i.mAmount > 0 && i.mStartDate.year <= CURRENT_YEAR && i.mEndDate.year > CURRENT_YEAR) { // is valid contract
                UShort duration = i.mEndDate.year - i.mStartDate.year;
                UShort yearsLeft = i.mEndDate.year - CURRENT_YEAR;
                auto amountPerYear = i.mAmount / duration;
                auto amountInGame = amountPerYear * yearsLeft;
                if (i.mIncomeType >= 1 && i.mIncomeType <= 4) { // Kit Sponsor, Government/Council Grant, Stadium Sponsor, General Sponsor
                    if (!bestIncome || amountInGame > bestIncomeValue) {
                        bestIncome = &i;
                        bestIncomeValue = amountInGame;
                    }
                }
                else if (i.mIncomeType == 5) { // Individual TV Deal
                    if (!bestIndividualTvMoneyIncome || amountInGame > bestIndividualTvMoneyValue) {
                        bestIndividualTvMoneyIncome = &i;
                        bestIndividualTvMoneyValue = amountInGame;
                    }
                }
            }
        }
    }
    if (bestIncome) {
        UShort duration = bestIncome->mEndDate.year - bestIncome->mStartDate.year;
        if (duration != 0) {
            dst->mSponsorAmount = foom::db::convert_money(bestIncome->mAmount / duration);
            dst->mSponsorDuration = bestIncome->mEndDate.year - CURRENT_YEAR;
            dst->mSpecialSponsor = bestIncome->mRenewIncome && !bestIncome->mFixedValue;
        }
    }
    if (bestIndividualTvMoneyIncome) {
        UShort duration = bestIndividualTvMoneyIncome->mEndDate.year - CURRENT_YEAR;
        if (duration != 0)
            dst->mIndividualTvMoney = foom::db::convert_money(bestIndividualTvMoneyIncome->mAmount / duration);
    }
    // Fans & attendance
    dst->mAverageAttendanceLastSeason = team->mAttendance;
    dst->mMaxAttendance = team->mMaximumAttendance;
    if (team->mNumberOfSeasonTicketHolders > 0)
        dst->mCountOfSoldSeasonTickets = team->mNumberOfSeasonTicketHolders;
    else
        dst->mCountOfSoldSeasonTickets = dst->mAverageAttendanceLastSeason / 2;
    dst->mPotentialFansCount = dst->mAverageAttendanceLastSeason * 2;
    if (team->mReputation > 7'000)
        dst->mPotentialFansCount += (team->mReputation - 7'000) * 50;
    if (dst->mPotentialFansCount > 250'000)
        dst->mPotentialFansCount = 250'000;

    // Stadium
    if (team->mStadium) {
        dst->SetProperty<Int>(L"foom::stad_id", team->mStadium->mID);
        FifamTrSetAll(dst->mStadiumName, FifamNames::LimitName(team->mStadium->mName, 29));
        if (team->mStadium->mCapacity > 0) {
            Int seatingCapacity = team->mStadium->mSeatingCapacity;
            if (seatingCapacity == 0 || seatingCapacity > team->mStadium->mCapacity)
                seatingCapacity = team->mStadium->mCapacity;
            dst->mStadiumSeatsCapacity = seatingCapacity;
            dst->mStadiumStandingsCapacity = team->mStadium->mCapacity - seatingCapacity;
        }
    }
    if (FifamTr(dst->mStadiumName).empty())
        FifamTrSetAll(dst->mStadiumName, FifamNames::LimitName(FifamTr(dst->mCityName), 21) + L" Stadium");
    if ((dst->mStadiumSeatsCapacity + dst->mStadiumStandingsCapacity) == 0)
        dst->mStadiumSeatsCapacity = Utils::Clamp(dst->mAverageAttendanceLastSeason * 2, 50, 50'000);

    // Club facilities
    Int clubDefaultFacilitiesValue = team->mReputation / 500;
    if (clubDefaultFacilitiesValue < 1)
        clubDefaultFacilitiesValue = 1;
    Int clubDefaultFacilitiesForUnknown = clubDefaultFacilitiesValue;
    if (clubDefaultFacilitiesForUnknown > 1)
        clubDefaultFacilitiesForUnknown -= 1;


    // club facilities is based on training facilities (66,7%) and corporate facilities (33,3%)
    dst->mClubFacilities = Utils::MapTo(team->mTraining + team->mCorporateFacilities / 2, 1, 30, 0, 6);
    Int youthFacilities = team->mYouthFacilities;
    if (youthFacilities <= 0)
        youthFacilities = clubDefaultFacilitiesForUnknown;
    dst->mYouthCentre = Utils::MapTo(youthFacilities, 1, 20, 1, 10);
    Int youthCoaching = team->mYouthCoaching;
    if (youthCoaching <= 0)
        youthCoaching = clubDefaultFacilitiesForUnknown;
    dst->mYouthBoardingSchool = Utils::MapTo(youthCoaching, 1, 20, 0, 10);
    Int trainingGrounds = team->mTraining;
    if (trainingGrounds <= 0)
        trainingGrounds = clubDefaultFacilitiesForUnknown;
    dst->mTrainingGrounds = Utils::MapTo(trainingGrounds, 1, 20, 0, 5);
    Int corporateFacilities = team->mCorporateFacilities;
    if (corporateFacilities <= 0)
        corporateFacilities = 1;
    dst->mMerchandising = Utils::MapTo(corporateFacilities, 1, 20, 0, 5);
    dst->mMedicalDepartment = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mMedia = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mResearch = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);

    // AI strategy
    Float representation = team->mCorporateFacilities >= 15 ? (team->mCorporateFacilities - 14) * 1.5f : 0;
    Float youth = (Float)((team->mTraining >= 6 && team->mYouthImportance >= 14) ? team->mYouthImportance - 13 : 0);
    Float transfers = (Float)(team->mYouthRecruitment >= 14 ? team->mYouthRecruitment - 13 : 0);
    Float incomeOrientation = (Float)((team->mBalance > 100'000 && team->mTransferBudget > 0) ? team->mBalance / team->mTransferBudget : 0);
    Float income = incomeOrientation >= 8 ? Utils::Min(7.0f, roundf(incomeOrientation - 7)) : 0;
    if (representation > 0 || youth > 0 || transfers > 0 || income > 0) {
        dst->mAiStrategy = Utils::GetMaxElementId<Float, FifamClubAiStrategy>({
            { representation, FifamClubAiStrategy::Representation },
            { youth, FifamClubAiStrategy::TrainingAndYouth },
            { transfers, FifamClubAiStrategy::ScoutingAndTransfers },
            { income, FifamClubAiStrategy::Income },
            }).second;
    }
    else
        dst->mAiStrategy = FifamClubAiStrategy::General;

    // settlement
    if (team->mCity && team->mCity->mInhabitants > 0) {
        if (team->mCity->mInhabitants >= 10) // 500'001+
            dst->mSettlement = FifamClubSettlement::City;
        else if (team->mCity->mInhabitants >= 6) // 25'001+
            dst->mSettlement = FifamClubSettlement::Town;
        else
            dst->mSettlement = FifamClubSettlement::Village;
    }
    else {
        if (dst->mStadiumSeatsCapacity <= 50000) {
            if (dst->mStadiumSeatsCapacity > 20000)
                dst->mSettlement = FifamClubSettlement::Town;
            else
                dst->mSettlement = FifamClubSettlement::Village;
        }
        else
            dst->mSettlement = FifamClubSettlement::City;
    }

    // Youth players come from
    if (team->mBasedNation)
        dst->mYouthPlayersCountry.SetFromInt(team->mBasedNation->mConverterData.mFIFAManagerReplacementID);
    else
        dst->mYouthPlayersCountry.SetFromInt(country->mId);
    // Youth players are basques
    dst->mYouthPlayersAreBasques = team->is_basque();

    // League history
    for (auto &h : team->mVecTeamLeagueHistory) {
        if (h.mDivision && h.mDivision->mCompetitionLevel >= 1 && h.mDivision->mCompetitionLevel <= 3) {
            UInt level = h.mDivision->mCompetitionLevel - 1;
            dst->mLeagueTotalPoints[level] += h.mPoints;
            dst->mLeagueTotalWins[level] += h.mGamesWon;
            dst->mLeagueTotalDraws[level] += h.mGamesDrawn;
            dst->mLeagueTotalLoses[level] += h.mGamesLost;
            dst->mLeagueTotalGoals[level] += h.mGoalsFor;
            dst->mLeagueTotalGoalsAgainst[level] += h.mGoalsAgainst;
        }
    }
    for (UInt i = 0; i < 3; i++) {
        if (dst->mLeagueTotalPoints[i] < 0)
            dst->mLeagueTotalPoints[i] = 0;
    }
    // Records
    if (team->mRecordAttendance > 0 && team->mRecordAttendanceOpposition && team->mRecordAttendanceYear > 0) {
        dst->mHistory.mRecordAttendance.mAttendance = team->mRecordAttendance;
        dst->mHistory.mRecordAttendance.mOpponentName = FifamNames::LimitName(team->mRecordAttendanceOpposition->mName, 20);
        dst->mHistory.mRecordAttendance.mYear = team->mRecordAttendanceYear;
    }
    if (team->mRecordWinYear > 0 && team->mRecordWinOpposition) {
        dst->mHistory.mRecordHomeWin.mScore1 = team->mRecordWinTG;
        dst->mHistory.mRecordHomeWin.mScore2 = team->mRecordWinOG;
        dst->mHistory.mRecordHomeWin.mOpponentName = FifamNames::LimitName(team->mRecordWinOpposition->mName, 20);
        dst->mHistory.mRecordHomeWin.mYear = team->mRecordWinYear;
    }
    if (team->mRecordDefeatYear > 0 && team->mRecordDefeatOpposition) {
        dst->mHistory.mRecordAwayDefeat.mScore1 = team->mRecordDefeatTG;
        dst->mHistory.mRecordAwayDefeat.mScore2 = team->mRecordDefeatOG;
        dst->mHistory.mRecordAwayDefeat.mOpponentName = FifamNames::LimitName(team->mRecordDefeatOpposition->mName, 20);
        dst->mHistory.mRecordAwayDefeat.mYear = team->mRecordDefeatYear;
    }

    // calculate international prestige
    static Pair<Int, UChar> internationalRep[20] = {
        { 9000, 20 },
        { 8750, 19 },
        { 8550, 18 },
        { 8300, 17 },
        { 8150, 16 },
        { 7900, 15 },
        { 7750, 14 },
        { 7500, 13 },
        { 7300, 12 },
        { 7000, 11 },
        { 6900, 10 },
        { 6600,  9 },
        { 6400,  8 },
        { 6200,  7 },
        { 6000,  6 },
        { 5800,  5 },
        { 5600,  4 },
        { 5000,  3 },
        { 4500,  2 },
        { 4000,  1 }
    };
    dst->mInternationalPrestige = 0;
    for (auto const &ip : internationalRep) {
        if (team->mReputation >= ip.first) {
            dst->mInternationalPrestige = ip.second;
            break;
        }
    }

    dst->mPreferredFormations[0] = FifamFormation::None;
    dst->mPreferredFormations[1] = FifamFormation::None;
    if (team->mPreferredFormation != 0) {
        FifamFormation firstFormation = ConvertFormationId(team->mPreferredFormation);
        if (firstFormation != FifamFormation::None) {
            UChar firstFormationId = firstFormation.GetIdForGame(gameId);
            if (firstFormationId != 0) {
                dst->mPreferredFormations[0] = firstFormation;
                if (team->mSecondPreferredFormation != 0) {
                    FifamFormation secondFormation = ConvertFormationId(team->mSecondPreferredFormation);
                    if (secondFormation != FifamFormation::None) {
                        UChar secondFormationId = secondFormation.GetIdForGame(gameId);
                        if (secondFormationId != 0 && secondFormationId != firstFormationId)
                            dst->mPreferredFormations[1] = secondFormation;
                    }
                }
            }
        }
    }

    ConvertClubStadium(dst, gameId);
    Int foomIdForGraphics = team->mID;
    if (!team->mConverterData.mParentClub && team->mConverterData.mOriginalStoredParentClub)
        foomIdForGraphics = team->mConverterData.mOriginalStoredParentClub->mID;
    ConvertKitsAndColors(dst, foomIdForGraphics, mainTeam->mVecKits, team->mBadge, team->mBackgroundColor, team->mForegroundColor, gameId);
}

void Converter::ConvertReserveClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div) {
    // name
    String secondTypeName, thirdTypeName;
    if (country && country->mId == FifamNation::Germany) {
        secondTypeName = L"II";
        thirdTypeName = L"III";
    }
    else {
        secondTypeName = L"2";
        thirdTypeName = L"3";
    }
    if (team->mName == mainTeam->mName) {
        String teamTypeName = L"Res";
        switch (team->mConverterData.mChildType) {
        case foom::club::converter_data::child_type::second:
        case foom::club::converter_data::child_type::second_affiliated:
            teamTypeName = secondTypeName;
            break;
        case foom::club::converter_data::child_type::third:
            teamTypeName = thirdTypeName;
            break;
        case foom::club::converter_data::child_type::u23:
            teamTypeName = L"U23";
            break;
        case foom::club::converter_data::child_type::u21:
        case foom::club::converter_data::child_type::u21_b:
            teamTypeName = L"U21";
            break;
        case foom::club::converter_data::child_type::u20:
            teamTypeName = L"U20";
            break;
        case foom::club::converter_data::child_type::u19:
        case foom::club::converter_data::child_type::u19_b:
            teamTypeName = L"U19";
            break;
        case foom::club::converter_data::child_type::u18:
        case foom::club::converter_data::child_type::u18_b:
            teamTypeName = L"U18";
            break;
        }
        String teamName = team->mName + L" " + teamTypeName;
        if (teamName.length() >= 30)
            teamName = FifamNames::LimitName(team->mShortName + L" " + teamTypeName, 29);
        FifamTrSetAll(dst->mName, teamName);
    }
    else {
        if (team->mName.length() < 30)
            FifamTrSetAll(dst->mName, team->mName);
        else
            FifamTrSetAll(dst->mName, FifamNames::LimitName(team->mShortName, 29));
        FifamTrSetAll(dst->mShortName, FifamNames::LimitName(team->mShortName, 10));
    }

    dst->mName2 = dst->mName;
    dst->mShortName2 = dst->mShortName;

    // abbr
    FifamTrSetAll(dst->mAbbreviation, FifamNames::GetClubAbbr(team->mShortName));
    // year of foundation & traditional club
    if (team->mYearFounded <= 0)
        dst->mYearOfFoundation = 2000;
    else
        dst->mYearOfFoundation = team->mYearFounded;

    // city and settlement
    if (team->mCity)
        FifamTrSetAll(dst->mCityName, FifamNames::LimitName(team->mCity->mName, 29));
    else if (country)
        dst->mCityName = country->mNationalTeam.mCityName;
    else
        FifamTrSetAll<String>(dst->mCityName, L"City");
    // latitude/longitude
    if (team->mLatitude != 0 && team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;

    if (dst->mGeoCoords.mLatitude.ToInt() == 0 && dst->mGeoCoords.mLongitude.ToInt() == 0 && team->mNation) {
        for (auto &city : team->mNation->mConverterData.mCities) {
            if (city->mLatitude != 0 && city->mLongitude != 0) {
                dst->mGeoCoords.SetFromFloat(city->mLatitude, city->mLongitude);
                break;
            }
        }
    }
    
    // Fans & attendance
    dst->mAverageAttendanceLastSeason = team->mAttendance / 100;
    if (dst->mAverageAttendanceLastSeason < 50)
        dst->mAverageAttendanceLastSeason = 50;
    dst->mMaxAttendance = dst->mAverageAttendanceLastSeason;
    team->mNumberOfSeasonTicketHolders = 10;
    dst->mPotentialFansCount = dst->mAverageAttendanceLastSeason;
    if (dst->mPotentialFansCount > 250'000)
        dst->mPotentialFansCount = 250'000;

    // Stadium
    FifamTrSetAll(dst->mStadiumName, FifamNames::LimitName(FifamTr(dst->mCityName), 21) + L" Stadium");
    if (team->mReputation > 8'000)
        dst->mStadiumSeatsCapacity = 3'000;
    else if (team->mReputation > 7'000)
        dst->mStadiumSeatsCapacity = 2'500;
    else if (team->mReputation > 6'000)
        dst->mStadiumSeatsCapacity = 2'000;
    else if (team->mReputation > 5'000)
        dst->mStadiumSeatsCapacity = 1'500;
    else if (team->mReputation > 4'000)
        dst->mStadiumSeatsCapacity = 1'000;
    else if (team->mReputation > 3'000)
        dst->mStadiumSeatsCapacity = 500;
    else if (team->mReputation > 2'000)
        dst->mStadiumSeatsCapacity = 250;
    else
        dst->mStadiumSeatsCapacity = 200;

    // Club facilities
    Int clubDefaultFacilitiesValue = team->mReputation / 500;
    if (clubDefaultFacilitiesValue < 1)
        clubDefaultFacilitiesValue = 1;
    Int clubDefaultFacilitiesForUnknown = clubDefaultFacilitiesValue;
    if (clubDefaultFacilitiesForUnknown > 1)
        clubDefaultFacilitiesForUnknown -= 1;


    // club facilities is based on training facilities (66,7%) and corporate facilities (33,3%)
    dst->mClubFacilities = Utils::MapTo((team->mTraining + team->mCorporateFacilities / 2) / 2, 1, 30, 0, 6);
    Int youthFacilities = team->mYouthFacilities;
    if (youthFacilities <= 0)
        youthFacilities = clubDefaultFacilitiesForUnknown;
    dst->mYouthCentre = Utils::MapTo(youthFacilities / 2, 1, 20, 1, 10);
    Int youthCoaching = team->mYouthCoaching;
    if (youthCoaching <= 0)
        youthCoaching = clubDefaultFacilitiesForUnknown;
    dst->mYouthBoardingSchool = Utils::MapTo(youthCoaching / 2, 1, 20, 0, 10);
    Int trainingGrounds = team->mTraining;
    if (trainingGrounds <= 0)
        trainingGrounds = clubDefaultFacilitiesForUnknown;
    dst->mTrainingGrounds = Utils::MapTo(trainingGrounds / 2, 1, 20, 0, 5);
    Int corporateFacilities = team->mCorporateFacilities;
    if (corporateFacilities <= 0)
        corporateFacilities = 1;
    dst->mMerchandising = 0;
    dst->mMedicalDepartment = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue / 2, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);
    dst->mMedia = 0;
    dst->mResearch = Utils::Clamp(Utils::MapTo(clubDefaultFacilitiesValue / 2, 1, 20, 0, 5) - 1 + Random::Get(0, 2), 0, 5);

    // AI strategy
    dst->mAiStrategy = FifamClubAiStrategy::TrainingAndYouth;

    // settlement
    if (team->mCity && team->mCity->mInhabitants > 0) {
        if (team->mCity->mInhabitants >= 6) // 25'001+
            dst->mSettlement = FifamClubSettlement::Town;
        else
            dst->mSettlement = FifamClubSettlement::Village;
    }
    else
        dst->mSettlement = FifamClubSettlement::Village;

    // Youth players come from
    if (team->mBasedNation)
        dst->mYouthPlayersCountry.SetFromInt(team->mBasedNation->mConverterData.mFIFAManagerReplacementID);
    else
        dst->mYouthPlayersCountry.SetFromInt(country->mId);
    // Youth players are basques
    dst->mYouthPlayersAreBasques = team->is_basque();

    dst->mInternationalPrestige = 0;
    
    ConvertKitsAndColors(dst, team->mID, mainTeam->mVecKits, team->mBadge, team->mBackgroundColor, team->mForegroundColor, gameId);
}

void Converter::ConvertClubStadium(FifamClub * dst, UInt gameId) {

    /* SPAIN */

    if (dst->mFifaID == 240) // Vicente Calderon (Madrid)
        dst->mStadiumVenue = 42;
    else if (dst->mFifaID == 241) { // Camp Nou (Barcelona)
        if (gameId != 13)
            dst->mStadiumVenue = 6;
    }
    else if (dst->mFifaID == 243) { // Santiago Bernabeu (Madrid)
        if (gameId >= 10)
            dst->mStadiumVenue = 2;
    }
    else if (dst->mFifaID == 461) { // Mestalla
        if (gameId <= 9 && gameId >= 12)
            dst->mStadiumVenue = 10;
    }

    /* ENGLAND */

    else if (dst->mFifaID == 1) { // Highbury (Arsenal), The Emirates Stadium (London)
        if (gameId <= 7)
            dst->mStadiumVenue = 7;
        else
            dst->mStadiumVenue = 156;
    }
    else if (dst->mFifaID == 5) // Stamford Bridge (London)
        dst->mStadiumVenue = 28;
    else if (dst->mFifaID == 9) // Anfield (Liverpool)
        dst->mStadiumVenue = 13;
    else if (dst->mFifaID == 11) // Old Trafford (Manchester)
        dst->mStadiumVenue = 1;
    else if (dst->mFifaID == 13) // St. James' Park (Newcastle)
        dst->mStadiumVenue = 100;
    else if (dst->mFifaID == 18) { // White Hart Lane (London), New Wembley (London)
        if (gameId >= 8) {
            if (gameId >= 13)
                dst->mStadiumVenue = 116;
            else
                dst->mStadiumVenue = 155;
        }
    }
    else if (dst->mFifaID == 1318) { // New Wembley (London)
        if (gameId >= 8 && gameId <= 12)
            dst->mStadiumVenue = 155;
    }
    else if (dst->mFifaID == 1961) { // Millennium Stadium (Cardiff)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 103;
    }

    /* ITALY */

    else if (dst->mFifaID == 44 || dst->mFifaID == 47) // San Siro (Milan)
        dst->mStadiumVenue = 5;
    else if (dst->mFifaID == 45) { // Delle Alpi (Turin)
        if (gameId <= 10)
            dst->mStadiumVenue = 8;
    }
    else if (dst->mFifaID == 46 || dst->mFifaID == 52) { // Stadio Olimpico (Rome)
        if (gameId >= 8)
            dst->mStadiumVenue = 157;
    }

    /* GERMANY */

    else if (dst->mFifaID == 21) { // Allianz Arena (Munich)
        if (gameId >= 8)
            dst->mStadiumVenue = 137;
    }
    else if (dst->mFifaID == 22) // Signal Iduna Park (Dortmund)
        dst->mStadiumVenue = 9;
    else if (dst->mFifaID == 28) // Imtech Arena (Hamburg)
        dst->mStadiumVenue = 41;
    else if (dst->mFifaID == 32) { // BayArena (Leverkusen)
        if (gameId <= 10)
            dst->mStadiumVenue = 12;
    }
    else if (dst->mFifaID == 34) { // Veltins-Arena (Gelsenkirchen)
        if (gameId >= 8)
            dst->mStadiumVenue = 30;
    }
    else if (dst->mFifaID == 36) { // Gottlieb Daimler Stadion (Stuttgart)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 133;
    }
    else if (dst->mFifaID == 166) { // Olympiastadion Berlin (Berlin)
        if (gameId >= 8)
            dst->mStadiumVenue = 135;
    }
    else if (dst->mFifaID == 485) { // HDI Arena (Hanover)
        if (gameId == 8 || gameId == 9 || gameId >= 11)
            dst->mStadiumVenue = 134;
    }
    else if (dst->mFifaID == 1824) { // Commerzbank Arena (Frankfurt)
        if (gameId == 8 || gameId == 9 || gameId >= 11)
            dst->mStadiumVenue = 138;
    }

    /* FRANCE */

    else if (dst->mFifaID == 66) // Stade Gerland (Lyon)
        dst->mStadiumVenue = 4;
    else if (dst->mFifaID == 73) { // Parc des Princes
        if (gameId != 11)
            dst->mStadiumVenue = 14;
    }
    else if (dst->mFifaID == 64) // Stade Bollaert-Delelis (Lens)
        dst->mStadiumVenue = 16;
    else if (dst->mFifaID == 219) { // Stade Velodrome
        if (gameId != 11)
            dst->mStadiumVenue = 29;
    }

    /* PORTUGAL */

    else if (dst->mFifaID == 234) { // Estadio Jose Alvalade
        if (gameId != 11)
            dst->mStadiumVenue = 107;
    }
    else if (dst->mFifaID == 236) { // Estadio do Dragao
        if (gameId != 11)
            dst->mStadiumVenue = 111;
    }
    else if (dst->mFifaID == 237) { // Estadio Jose Alvalade
        if (gameId != 11)
            dst->mStadiumVenue = 102;
    }
    else if (dst->mFifaID == 1898) { // Estadio Da Bessa (Boavista Porto)
        if (gameId == 8 || gameId == 9)
            dst->mStadiumVenue = 123;
    }

    /* NETHERLANDS */

    else if (dst->mFifaID == 245) // Amsterdam ArenA (Amsterdam)
        dst->mStadiumVenue = 15;

    /* BELGIUM */

    else if (dst->mFifaID == 229) // Constant Vanden Stock (Brussels)
        dst->mStadiumVenue = 17;

    /* TURKEY */

    else if (dst->mFifaID == 325) { // Ali Sami Yen Stadi (Istanbul)
        if (gameId <= 7)
            dst->mStadiumVenue = 101;
    }

    /* MEXICO */

    else if (dst->mFifaID == 1878) { // Azteca (Mexico City)
        if (gameId >= 8)
            dst->mStadiumVenue = 104;
    }
    else if (dst->mFifaID == 1880) { // Jalisco (Guadalajara)
        if (gameId == 10 || gameId == 11 || gameId == 13 || gameId == 14)
            dst->mStadiumVenue = 163;
    }

    /* USA */

    else if (dst->mFifaID == 697) { // Home Depot Center (Los Angeles)
        if (gameId >= 8 && gameId <= 10)
            dst->mStadiumVenue = 161;
    }

    /* SOUTH KOREA */

    else if (dst->mFifaID == 982) { // Seoul World Cup Stadium (Seoul)
        if (gameId >= 8)
            dst->mStadiumVenue = 108;
    }
    else if (dst->mFifaID == 2056) { // Daegu Stadium (Daegu)
        if (gameId >= 8)
            dst->mStadiumVenue = 110;
    }

}

void Converter::ConvertKitsAndColors(FifamClub *dst, Int foomId, Vector<foom::kit> const &kits, Int badgeType, Color const &teamBackgroundColor, Color const &teamForegroundColor, UInt gameId) {
    enum KitPart { Shirt, Icon, Text, Shorts, Socks };

    const foom::kit *tmpKitSets[5][2] = {};
    const foom::kit *tmpCompKitSets[5][2] = {};

    for (auto &k : kits) {
        if (k.mKit == 0 || k.mKit == 1) {
            if (k.mType >= 1 && k.mType <= 5 && k.mAlternativeKitNumber == 0 && (k.mYear == 0 || k.mYear == CURRENT_YEAR)) {
                if (k.mCompetition) {
                    if (!tmpCompKitSets[k.mType - 1][k.mKit])
                        tmpCompKitSets[k.mType - 1][k.mKit] = &k;
                }
                else {
                    if (!tmpKitSets[k.mType - 1][k.mKit])
                        tmpKitSets[k.mType - 1][k.mKit] = &k;
                }
            }
        }
    }

    const foom::kit *kitSets[5][2] = {};
    for (UInt k = 0; k < 2; k++) {
        for (UInt t = 0; t < 5; t++)
            kitSets[t][k] = tmpKitSets[t][k] ? tmpKitSets[t][k] : tmpCompKitSets[t][k];
    }

    Bool textColorsUsed = false;

    const foom::kit *clubClrs = nullptr;
    if (badgeType != 11 && kitSets[2][0]) {
        clubClrs = kitSets[2][0];
        textColorsUsed = true;
    }
    else {
        for (UInt i = 0; i < 5; i++) {
            if (kitSets[i][0]) {
                clubClrs = kitSets[i][0];
                break;
            }
        }
    }

    // badge and colors

    Color backgroundClr, foregroundClr;

    ClubColor clubColorInfo;
    if (mClubColorsMap.find(foomId) != mClubColorsMap.end())
        clubColorInfo = mClubColorsMap[foomId];
    else
        clubColorInfo.type = ClubColor::DEFAULT;

    auto SetColor_RGB = [&]() {
        backgroundClr = clubColorInfo.rgb[0];
        foregroundClr = clubColorInfo.rgb[1];
        dst->mClubColour.Set(clubColorInfo.rgb[0], clubColorInfo.rgb[1]);
        return true;
    };

    auto SetColor_TEAM = [&](bool forceApply = false) {
        if (forceApply || (teamBackgroundColor != Color(45, 104, 181) && teamForegroundColor != Color(250, 250, 250))) {
            backgroundClr = teamBackgroundColor;
            foregroundClr = teamForegroundColor;
            Color secondColor = foregroundClr;
            if (Color::Distance(backgroundClr, foregroundClr) < 100) {
                auto white = Color::Distance(backgroundClr, { 255, 255, 255 });
                auto black = Color::Distance(backgroundClr, { 0, 0, 0 });
                if (black < white)
                    secondColor = { 255, 255, 255 };
                else
                    secondColor = { 0, 0, 0 };
            }
            dst->mClubColour.Set(backgroundClr, secondColor);
            return true;
        }
        return false;
    };

    auto SetColor_KIT = [&]() {
        if (clubClrs) {
            backgroundClr = Color(clubClrs->mBackground.r, clubClrs->mBackground.g, clubClrs->mBackground.b);
            foregroundClr = Color(clubClrs->mForeground.r, clubClrs->mForeground.g, clubClrs->mForeground.b);
            Color outlineClr = Color(clubClrs->mOutline.r, clubClrs->mOutline.g, clubClrs->mOutline.b);
            if (textColorsUsed)
                std::swap(foregroundClr, outlineClr);
            Color secondColor = foregroundClr;
            if (Color::Distance(backgroundClr, foregroundClr) < 100) {
                if (Color::Distance(backgroundClr, outlineClr) < 100) {
                    if (Color::Distance(backgroundClr, outlineClr) < 100) {
                        auto white = Color::Distance(backgroundClr, { 255, 255, 255 });
                        auto black = Color::Distance(backgroundClr, { 0, 0, 0 });
                        if (black < white)
                            secondColor = { 255, 255, 255 };
                        else
                            secondColor = { 0, 0, 0 };
                    }
                    else
                        secondColor = outlineClr;
                }
                else
                    secondColor = outlineClr;
            }
            dst->mClubColour.Set(backgroundClr, secondColor);
            return true;
        }
        return false;
    };

    bool colorsFound = false;

    if (clubColorInfo.type == ClubColor::RGB)
        colorsFound = SetColor_RGB();
    else if (clubColorInfo.type == ClubColor::TEAM)
        colorsFound = SetColor_TEAM(true);
    else if (clubColorInfo.type == ClubColor::KIT)
        colorsFound = SetColor_KIT();
    else {
        colorsFound = SetColor_KIT();
        if (!colorsFound)
            colorsFound = SetColor_TEAM(false);
    }

    if (!colorsFound) {
        // random club colors
        UInt numTeamColors = FifamClub::mTeamColorsTable.size();
        if (numTeamColors > 0) {
            dst->mClubColour.SetFromTable(FifamClub::mTeamColorsTable, Random::Get(0, 27));
            if (Random::Get(1, 100) > 50) {
                backgroundClr = dst->mClubColour.second;
                foregroundClr = dst->mClubColour.first;
            }
            else {
                backgroundClr = dst->mClubColour.first;
                foregroundClr = dst->mClubColour.second;
            }
        }
    }

    dst->mClubColour2 = dst->mClubColour;
    dst->mMerchandiseColour = backgroundClr;
    if (Color::Distance(backgroundClr, Color(255, 255, 255)) < 100) {
        dst->mBackgroundColour = foregroundClr;
        dst->mHeaderColour = backgroundClr;
    }
    else {
        dst->mBackgroundColour = backgroundClr;
        dst->mHeaderColour = foregroundClr;
    }

    bool hasCustomBadge = false;

    if (mFromFifaDatabase) {
        if (dst->mFifaID != 0 && !dst->mIsNationalTeam) {
            if (exists(mFifaAssetsPath / L"crest" / Utils::Format(L"l%d.dds", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"crest" / Utils::Format(L"%d.png", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"_crest" / Utils::Format(L"l%d.dds", dst->mFifaID)) ||
                exists(mFifaAssetsPath / L"_crest" / Utils::Format(L"%d.png", dst->mFifaID)))
            {
                dst->mBadge.SetBadgePath(L"clubs\\Badge%d\\" + Utils::Format(L"%08X", dst->mUniqueID) + L".tga");
                hasCustomBadge = true;
            }
        }
    }
    else {
        if (gameId <= 9 && foomId != -1) {
            auto badgeIt = mAvailableBadges.find(foomId);
            if (badgeIt != mAvailableBadges.end()) {
                dst->mBadge.SetBadgePath(L"clubs\\Badge%d\\" + Utils::Format(L"%08X", dst->mUniqueID) + L".tga");
                hasCustomBadge = true;
            }
        }
    }

    if (!hasCustomBadge) {
        dst->mBadge.SetColorDefault(dst->mClubColour);

        if (badgeType != -1) {
            Bool inverted = Random::Get(0, 1);
            switch (badgeType) {
            case 0:
                dst->mBadge.SetColor(FifamClubBadgeShape::Ball, 1, 1, inverted);
                break;
            case 1:
            case 13:
                dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, 7, inverted);
                break;
            case 2:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 5, Random::Get(1, 4), inverted);
                break;
            case 3:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 2, 1, inverted);
                break;
            case 4:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 6, 1, inverted);
                break;
            case 5:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 17, 1, inverted);
                break;
            case 6:
                dst->mBadge.SetColor(FifamClubBadgeShape::Round, 1, Random::Get(1, 2), false);
                break;
            case 7:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 2, inverted);
                break;
            case 8:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 8, 1, inverted);
                break;
            case 9:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 4, 4, inverted);
                break;
            case 10:
                dst->mBadge.SetColor(FifamClubBadgeShape::Oval, 1, 1, false);
                break;
            case 11:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 10, 1, inverted);
                break;
            case 12:
                dst->mBadge.SetColor(FifamClubBadgeShape::Crest, 9, 1, inverted);
                break;
            }
        }
    }

    // home & away kits
    for (UInt i = 0; i < 2; i++) {
        auto &kit = dst->mKit.mSets[i];
        // kit - number
        if (kitSets[KitPart::Shirt][i])
            kit.mShirtNumberColor = kitSets[KitPart::Shirt][i]->mNumberColour;
        else {
            if (i == 0)
                kit.mShirtNumberColor = foregroundClr;
            else
                kit.mShirtNumberColor = backgroundClr;
        }
        // kit - armband
        kit.mCaptainArmbandColor = kit.mShirtNumberColor;
        // kit - socks
        kit.mSocks = 1;
        if (i == 0)
            kit.mSocksColors = { backgroundClr, backgroundClr };
        else
            kit.mSocksColors = { foregroundClr, foregroundClr };
        if (kitSets[KitPart::Socks][i]) {
            auto &set = kitSets[KitPart::Socks][i];
            switch (set->mKitStyle) {
            case 1:
            case 3:
            case 14:
            case 15:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mBackground };
                break;
            case 2:
            case 4:
            case 5:
            case 7:
                kit.mSocks = 1;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 6:
            case 11:
            case 12:
                kit.mSocks = 4;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 8:
            case 9:
            case 13:
                kit.mSocks = 2;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            case 10:
                kit.mSocks = 3;
                kit.mSocksColors = { set->mBackground, set->mForeground };
                break;
            }
        }
        // kit - shorts
        kit.mShorts = 1;
        if (i == 0)
            kit.mShortsColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShortsColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shorts][i]) {
            auto &set = kitSets[KitPart::Shorts][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShorts = 1;
                kit.mShortsColors = { set->mBackground, set->mBackground, set->mBackground };
                break;
            case 2:
                kit.mShorts = 6;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 3:
            case 4:
                kit.mShorts = 7;
                kit.mShortsColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 5:
                kit.mShorts = 4;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                kit.mShorts = 3;
                kit.mShortsColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            }
        }
        // kit - shirts
        kit.mShirt = 4;
        if (i == 0)
            kit.mShirtColors = { backgroundClr, foregroundClr, foregroundClr };
        else
            kit.mShirtColors = { foregroundClr, backgroundClr, backgroundClr };
        if (kitSets[KitPart::Shirt][i]) {
            auto &set = kitSets[KitPart::Shirt][i];
            switch (set->mKitStyle) {
            case 1:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 2:
                kit.mShirt = 46;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 3:
            case 48:
                if (set->mBackOfShirtStyle == 2) // Plain
                    kit.mShirt = 14;
                else
                    kit.mShirt = 41;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 4:
            case 49:
            case 51:
                kit.mShirt = 48;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 5:
                kit.mShirt = 37;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 6:
                kit.mShirt = 40;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 7:
                kit.mShirt = 49;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mBackground };
                break;
            case 8:
            case 38:
                kit.mShirt = 38;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 9:
                kit.mShirt = 36;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 10:
                kit.mShirt = 30;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 11:
            case 37:
                kit.mShirt = 19;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 12:
            case 55:
                kit.mShirt = 57;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 13:
                kit.mShirt = 21;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 14:
                kit.mShirt = 4;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mOutline };
                break;
            case 15:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 16:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 17:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 18:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 19:
                kit.mShirt = 60;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 20:
            case 39:
                kit.mShirt = 39;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 21:
                kit.mShirt = 22;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 22:
            case 23:
            case 31:
            case 32:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 24:
            case 33:
                kit.mShirt = 12;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 25:
                kit.mShirt = 9;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 26:
            case 27:
                kit.mShirt = 52;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 28:
                kit.mShirt = 47;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 29:
                kit.mShirt = 11;
                kit.mShirtColors = { set->mBackground, set->mBackground, set->mForeground };
                break;
            case 30:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 34:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 35:
                kit.mShirt = 8;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 36:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mBackground };
                break;
            case 40:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 41:
                kit.mShirt = 63;
                kit.mShirtColors = { set->mForeground, set->mBackground, set->mOutline };
                break;
            case 42:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 43:
                kit.mShirt = 13;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 44:
                kit.mShirt = 65;
                kit.mShirtColors = { set->mBackground, set->mOutline, set->mForeground };
                break;
            case 45:
                kit.mShirt = 15;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 46:
            case 47:
                kit.mShirt = 54;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mForeground };
                break;
            case 50:
                kit.mShirt = 62;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 52:
                kit.mShirt = 58;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 53:
                kit.mShirt = 17;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 54:
                kit.mShirt = 53;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            case 56:
                kit.mShirt = 12;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mOutline };
                break;
            }
        }

        // (if club has no FIFA kit)
        if (!exists(L"D:\\Games\\FIFA Manager 13\\data\\kits\\" + Utils::Format(L"%08X", dst->mUniqueID) + L"_h.tga")
            && !exists(L"D:\\Projects\\fifam\\content\\fm13\\art_05\\data\\kits\\" + Utils::Format(L"%08X", dst->mUniqueID) + L"_h.tga"))
        {
            Color shirtBackColor = FifamKit::GetShirtBackColor(kit.mShirt, kit.mShirtColors[0], kit.mShirtColors[1], kit.mShirtColors[2]);
            if (Color::Distance(kit.mShirtNumberColor, shirtBackColor) < 100) {
                if (Color::Distance(kit.mShirtNumberColor, foregroundClr) < 100)
                    kit.mShirtNumberColor = backgroundClr;
                else
                    kit.mShirtNumberColor = foregroundClr;
            }
        }
    }
}

void Converter::FixPersonName(String &name) {
    String originalName = name;
    name.clear();
    for (auto c : originalName) {
        if (c != L',' && c != '|')
            name += c;
    }
}

void Converter::ConvertPersonAttributes(FifamPerson *person, foom::person *p) {
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

void Converter::CreateStaffMembersForClub(UInt gameId, foom::team * team, FifamClub * dst, Bool isNationalTeam) {
    std::sort(team->mVecContractedNonPlayers.begin(), team->mVecContractedNonPlayers.end(),
        [](foom::non_player *a, foom::non_player *b) {
        return a->mCurrentAbility > b->mCurrentAbility;
    });

    foom::non_player *staffManager = nullptr;
    foom::non_player *staffOwner = nullptr;
    foom::non_player *staffChairman = nullptr;
    foom::non_player *staffDirector = nullptr;
    foom::non_player *staffManagingDirector = nullptr;
    foom::non_player *staffSportsDirector = nullptr;
    foom::non_player *staffReserveCoach = nullptr;
    foom::non_player *staffYouthCoach = nullptr;
    foom::non_player *staffGoalkeeperCoach = nullptr;
    foom::non_player *staffFitnessCoach = nullptr;
    foom::non_player *staffTeamDoctor = nullptr;
    foom::non_player *staffPhysio = nullptr;

    Vector<foom::non_player *> staffAssistantCoaches;
    Vector<foom::non_player *> staffCoaches;
    Vector<foom::non_player *> staffChiefScouts;
    Vector<foom::non_player *> staffScouts;

    UInt maxAssistantCoaches = 1;
    UInt maxScouts = 1;

    UChar maxReserveCoachAgeGroup = 0;
    UChar maxYouthCoachAgeGroup = 0;

    Bool hasHeadPhysio = false;
    Bool hasChiefDoctor = false;
    Bool hasOwner = false;

    if (gameId >= 9) {
        maxAssistantCoaches = 2;
        maxScouts = 6;
    }

    // reserve coach
    if (!isNationalTeam) {
        foom::club *reserveClub = ((foom::club *)team)->mConverterData.mMainChildClubInDB;
        if (reserveClub) {
            for (auto &p : reserveClub->mVecContractedNonPlayers) {
                if (IsConvertable(p, gameId) && p->mClubContract.mJob == 5) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 23;
                    break;
                }
            }
        }
    }

    for (auto &p : team->mVecContractedNonPlayers) {
        Int j = -1;
        if (isNationalTeam) {
            if (p->mClubContract.mClub)
                continue;
            j = p->mNationContract.mJob;
        }
        else
            j = p->mClubContract.mJob;

        if (j == 157) { // owner
            if (!hasOwner) {
                staffOwner = p;
                hasOwner = IsConvertable(p, gameId);
            }
        }
        else if (IsConvertable(p, gameId)) {
            switch (j) {
            case 5: // used as manager
                if (!staffManager)
                    staffManager = p;
                break;
            case 1: // used as president or chief-exec
                if (!staffChairman)
                    staffChairman = p;
                break;
            case 38: // used as chief-exec
                if (!staffDirector)
                    staffDirector = p;
                break;
            case 2: // used as general manager
                if (!staffManagingDirector)
                    staffManagingDirector = p;
                break;
            case 4: // used as director of football (sports director)
                if (!staffSportsDirector)
                    staffSportsDirector = p;
                break;
            case 6: // assistant manager first team
                staffAssistantCoaches.push_back(p);
                break;
            case 8: // coach first team
                staffCoaches.push_back(p);
                break;
            case 34: // chief scout
                staffChiefScouts.push_back(p);
                break;
            case 9: // scout
                staffScouts.push_back(p);
                break;
            case 26: // gk coach
                if (!staffGoalkeeperCoach)
                    staffGoalkeeperCoach = p;
                break;
            case 55: // fitness coach
                if (!staffFitnessCoach)
                    staffFitnessCoach = p;
                break;
            case 36: // head physio
                if (!hasHeadPhysio) {
                    staffPhysio = p;
                    hasHeadPhysio = true;
                }
                break;
            case 10: // physio
                if (!staffPhysio)
                    staffPhysio = p;
                break;
            case 172: // chief doctor
                if (!hasChiefDoctor) {
                    staffTeamDoctor = p;
                    hasChiefDoctor = true;
                }
                break;
            case 178: // doctor
                if (!staffTeamDoctor)
                    staffTeamDoctor = p;
                break;
            case 101: // U23 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 23) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 23;
                }
                break;
            case 107: // U21 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 21) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 21;
                }
                break;
            case 113: // U20 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 20) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 20;
                }
                break;
            case 119: // U19 manager
                if (!staffYouthCoach || maxYouthCoachAgeGroup < 19) {
                    staffYouthCoach = p;
                    maxYouthCoachAgeGroup = 19;
                }
                break;
            case 24: // U18 manager
                if (!staffYouthCoach || maxYouthCoachAgeGroup < 18) {
                    staffYouthCoach = p;
                    maxYouthCoachAgeGroup = 18;
                }
                break;
            }
        }
    }

    if (!staffCoaches.empty())
        staffAssistantCoaches.insert(staffAssistantCoaches.end(), staffCoaches.begin(), staffCoaches.end());
    if (!staffScouts.empty())
        staffChiefScouts.insert(staffChiefScouts.end(), staffScouts.begin(), staffScouts.end());

    if (staffManager)
        CreateAndConvertStaff(staffManager, dst, FifamClubStaffPosition::Manager);
    if (staffOwner) {
        if (IsConvertable(staffOwner, gameId))
            CreateAndConvertStaff(staffOwner, dst, FifamClubStaffPosition::President);
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::ChiefExec);
        else if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec);
    }
    else {
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::President);
        if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec);
    }
    if (staffManagingDirector)
        CreateAndConvertStaff(staffManagingDirector, dst, FifamClubStaffPosition::GeneralManager);
    if (gameId >= 9) {
        if (staffSportsDirector)
            CreateAndConvertStaff(staffSportsDirector, dst, FifamClubStaffPosition::SportsDirector);
    }
    for (UInt i = 0; i < Utils::Min(maxAssistantCoaches, staffAssistantCoaches.size()); i++)
        CreateAndConvertStaff(staffAssistantCoaches[i], dst, FifamClubStaffPosition::AssistantCoach);
    for (UInt i = 0; i < Utils::Min(maxScouts, staffChiefScouts.size()); i++)
        CreateAndConvertStaff(staffChiefScouts[i], dst, FifamClubStaffPosition::GeneralScout);
    if (staffReserveCoach)
        CreateAndConvertStaff(staffReserveCoach, dst, FifamClubStaffPosition::AmateurCoach);
    if (staffYouthCoach)
        CreateAndConvertStaff(staffYouthCoach, dst, FifamClubStaffPosition::YouthCoach);
    if (staffGoalkeeperCoach)
        CreateAndConvertStaff(staffGoalkeeperCoach, dst, FifamClubStaffPosition::GoalkeeperCoach);
    if (gameId >= 9) {
        if (staffFitnessCoach)
            CreateAndConvertStaff(staffFitnessCoach, dst, FifamClubStaffPosition::FitnessCoach);
    }
    if (staffPhysio)
        CreateAndConvertStaff(staffPhysio, dst, FifamClubStaffPosition::Masseur);
    if (staffTeamDoctor)
        CreateAndConvertStaff(staffTeamDoctor, dst, FifamClubStaffPosition::TeamDoctor);
}

UChar Converter::GetPlayerLevelFromCA(Int ca) {
    static Pair<UChar, UChar> playerCAtoLvlAry[99] = {
        { 99, 199 },
        { 98, 198 },
        { 97, 197 },
        { 96, 196 },
        { 95, 195 },
        { 94, 192 },
        { 93, 190 },
        { 92, 188 },
        { 91, 184 },
        { 90, 180 },
        { 89, 177 },
        { 88, 174 },
        { 87, 170 },
        { 86, 166 },
        { 85, 163 },
        { 84, 160 },
        { 83, 157 },
        { 82, 154 },
        { 81, 151 },
        { 80, 149 },
        { 79, 146 }, // 147?
        { 78, 143 },
        { 77, 141 },
        { 76, 139 },
        { 75, 136 }, // 73, 74, 75
        { 74, 133 },
        { 73, 130 },
        { 72, 127 },
        { 71, 124 },
        { 70, 122 },
        { 69, 119 }, // 66, 67
        { 68, 115 },
        { 67, 111 },
        { 66, 108 },
        { 65, 105 },
        { 64, 102 },
        { 63,  99 },
        { 62,  96 },
        { 61,  93 },
        { 60,  90 },
        { 59,  88 },
        { 58,  86 },
        { 57,  84 },
        { 56,  82 },
        { 55,  80 },
        { 54,  78 },
        { 53,  76 },
        { 52,  74 },
        { 51,  72 },
        { 50,  70 },
        { 49,  68 },
        { 48,  66 },
        { 47,  64 },
        { 46,  62 },
        { 45,  60 },
        { 44,  58 },
        { 43,  56 },
        { 42,  54 },
        { 41,  52 },
        { 40,  50 },
        { 39,  48 },
        { 38,  46 },
        { 37,  44 },
        { 36,  42 },
        { 35,  40 },
        { 34,  38 },
        { 33,  36 },
        { 32,  34 },
        { 31,  32 },
        { 30,  30 },
        { 29,  29 },
        { 28,  28 },
        { 27,  27 },
        { 26,  26 },
        { 25,  25 },
        { 24,  24 },
        { 23,  23 },
        { 22,  22 },
        { 21,  21 },
        { 20,  20 },
        { 19,  19 },
        { 18,  18 },
        { 17,  17 },
        { 16,  16 },
        { 15,  15 },
        { 14,  14 },
        { 13,  13 },
        { 12,  12 },
        { 11,  11 },
        { 10,  10 },
        { 9,    9 },
        { 8,    8 },
        { 7,    7 },
        { 6,    6 },
        { 5,    5 },
        { 4,    4 },
        { 3,    3 },
        { 2,    2 },
        { 1,    1 }
    };
    for (UInt i = 0; i < 99; i++) {
        if (ca >= playerCAtoLvlAry[i].second)
            return playerCAtoLvlAry[i].first;
    }
    return 1;
}

Bool Converter::IsConvertable(foom::person *p, UInt gameId) {
    return gameId >= 13 || !p->mFemale;
}

Bool Converter::IsConvertable(foom::official *o, UInt gameId) {
    return gameId >= 13 || !o->mFemale;
}

Int OriginalAttrValue(Int attr) {
    return (Int)ceilf((Float)attr / 5.0f);
}

Int Converter::ConvertPlayerAttribute(Int attr, UInt gameId) {
    Int originalAttr = OriginalAttrValue(attr);
    if (originalAttr <= 0)
        originalAttr = 1;
    if (originalAttr > 20)
        originalAttr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        {  5, 15 }, // 1
        { 16, 29 }, // 2
        { 30, 38 }, // 3
        { 39, 44 }, // 4
        { 45, 50 }, // 5
        { 51, 53 }, // 6
        { 54, 57 }, // 7
        { 58, 61 }, // 8
        { 62, 64 }, // 9
        { 65, 67 }, // 10
        { 68, 70 }, // 11
        { 71, 74 }, // 12
        { 75, 78 }, // 13
        { 79, 82 }, // 14
        { 83, 84 }, // 15
        { 85, 86 }, // 16
        { 87, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    static Pair<Int, Int> fmRatingAry_12[20] = {
        {  5, 15 }, // 1
        { 16, 29 }, // 2
        { 30, 39 }, // 3
        { 40, 49 }, // 4
        { 50, 59 }, // 5
        { 60, 64 }, // 6
        { 65, 67 }, // 7
        { 68, 70 }, // 8
        { 71, 72 }, // 9
        { 73, 74 }, // 10
        { 75, 76 }, // 11
        { 77, 78 }, // 12
        { 79, 80 }, // 13
        { 81, 82 }, // 14
        { 83, 84 }, // 15
        { 85, 86 }, // 16
        { 87, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    if (gameId >= 13) {
        if (fmRatingAry[originalAttr - 1].first == fmRatingAry[originalAttr - 1].second)
            return fmRatingAry[originalAttr - 1].first;
        return Random::Get(fmRatingAry[originalAttr - 1].first, fmRatingAry[originalAttr - 1].second);
    }
    else {
        if (fmRatingAry_12[originalAttr - 1].first == fmRatingAry_12[originalAttr - 1].second)
            return fmRatingAry_12[originalAttr - 1].first;
        return Random::Get(fmRatingAry_12[originalAttr - 1].first, fmRatingAry_12[originalAttr - 1].second);
    }
}

Int Converter::LinearConvertPlayerAttribute(Int attr, UInt gameId) {
    Int originalAttr = OriginalAttrValue(attr);
    if (originalAttr <= 0)
        originalAttr = 1;
    if (originalAttr > 20)
        originalAttr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        {  0,  4 }, // 1
        {  5,  9 }, // 2
        { 10, 14 }, // 3
        { 15, 19 }, // 4
        { 20, 24 }, // 5
        { 25, 29 }, // 6
        { 30, 34 }, // 7
        { 35, 39 }, // 8
        { 40, 44 }, // 9
        { 45, 49 }, // 10
        { 50, 54 }, // 11
        { 55, 59 }, // 12
        { 60, 64 }, // 13
        { 65, 69 }, // 14
        { 70, 74 }, // 15
        { 75, 79 }, // 16
        { 80, 84 }, // 17
        { 85, 89 }, // 18
        { 90, 94 }, // 19
        { 95, 99 }  // 20
    };
    if (fmRatingAry[originalAttr - 1].first == fmRatingAry[originalAttr - 1].second)
        return fmRatingAry[originalAttr - 1].first;
    return Random::Get(fmRatingAry[originalAttr - 1].first, fmRatingAry[originalAttr - 1].second);
}

FifamPlayer *Converter::CreateAndConvertPlayer(UInt gameId, foom::player *p, FifamClub *club) {
    if (!p->mNation) {
        Error(L"Player without nation\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *playerCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!playerCountry) {
        Error(L"Player without associated country\nPlayerId: %d\nPlayerName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamPlayer *player = mFifamDatabase->CreatePlayer(club, mPersonIdCounter++);
    ConvertPersonAttributes(player, p);
    player->SetProperty(L"foom::player", p);
    player->mIsRealPlayer = true;
    player->mIsBasque = p->mIsBasque;
    
    UInt age = player->GetAge(GetCurrentDate());

    // foot
    player->mLeftFoot = Utils::MapTo(p->mLeftFoot, 1, 100, 0, 4);
    player->mRightFoot = Utils::MapTo(p->mRightFoot, 1, 100, 0, 4);
    if (player->mLeftFoot != 4 && player->mRightFoot != 4)
        player->mRightFoot = 4;

    // appearance
    player->mHeight = Utils::Clamp(p->mHeight, 150, 220);
    if (player->mHeight <= 0) {
        if (playerCountry->mAverageHeight > 10)
            player->mHeight = Utils::Clamp(playerCountry->mAverageHeight - 10 + Random::Get(0, 20), 150, 220);
        else
            player->mHeight = 175 + Random::Get(0, 10);
    }
    player->mWeight = Utils::Clamp(p->mWeight, 50, 150);
    if (player->mWeight == 50) {
        if (playerCountry->mAverageWeight > 10)
            player->mWeight = Utils::Clamp(playerCountry->mAverageWeight - 10 + Random::Get(0, 20), 50, 150);
        else
            player->mWeight = 70 + Random::Get(0, 10);
    }

    UInt randomShoeType = Random::Get(1, 99);
    if (randomShoeType > 66)
        player->mShoeType = FifamShoeType::Blue;
    else if (randomShoeType > 33)
        player->mShoeType = FifamShoeType::White;
    else
        player->mShoeType = FifamShoeType::Black;

    AppearanceGenerator::Type ethnicity = AppearanceGenerator::EasternEuropeDark;

    if (p->mEthnicity == 0) {
        if (p->mSkinTone <= 7)
            ethnicity = AppearanceGenerator::EasternEuropeLight;
        else
            ethnicity = AppearanceGenerator::EasternEuropeDark;
    }
    else if (p->mEthnicity == 1) {
        if (p->mSkinTone < 0 || p->mSkinTone >= 8)
            ethnicity = AppearanceGenerator::WesternEuropeDark;
        else
            ethnicity = AppearanceGenerator::WesternEuropeLight;
    }
    else if (p->mEthnicity == 2) {
        if (p->mSkinTone <= 10)
            ethnicity = AppearanceGenerator::Arabic2;
        else
            ethnicity = AppearanceGenerator::Arabic1;
    }
    else if (p->mEthnicity == 3) {
        if (p->mSkinTone < 0 || p->mSkinTone >= 16)
            ethnicity = AppearanceGenerator::African1;
        else
            ethnicity = AppearanceGenerator::African2;
    }
    else if (p->mEthnicity == 4 || p->mEthnicity == 10)
        ethnicity = AppearanceGenerator::Asian;
    else if (p->mEthnicity == 5)
        ethnicity = AppearanceGenerator::Indian;
    else if (p->mEthnicity == 6 || p->mEthnicity == 8) {
        if (p->mSkinTone <= 7)
            ethnicity = AppearanceGenerator::WesternEuropeLight;
        else
            ethnicity = AppearanceGenerator::WesternEuropeDark;
    }
    else if (p->mEthnicity == 7 || p->mEthnicity == 9)
        ethnicity = AppearanceGenerator::Latin;

    // first - generate random appearance for ethnicity
    appearanceGenerator.Generate(player, ethnicity);

    if (player->mAppearance.mHairStyle <= 1)
        player->mAppearance.mHairStyle = 28;

    // second - apply skin color and hair color (if available)
    bool skinColorSet = false;
    if (p->mSkinTone >= 0 && p->mSkinTone <= 20) {
        if (p->mSkinTone <= 6) // 1, 2, 3, 4, 5, 6, 7
            player->mAppearance.mSkinColor = FifamSkinColor::White;
        else if (p->mSkinTone <= 9) { // 8, 9, 10
            if (p->mEthnicity < 0 || p->mEthnicity == 2 || p->mEthnicity == 4 || p->mEthnicity == 5 || p->mEthnicity == 9 || p->mEthnicity == 10)
                player->mAppearance.mSkinColor = FifamSkinColor::Asian;
            else
                player->mAppearance.mSkinColor = FifamSkinColor::Latin1;
        }
        else if (p->mSkinTone <= 10)
            player->mAppearance.mSkinColor = FifamSkinColor::Latin1; // 11
        else if (p->mSkinTone <= 12)
            player->mAppearance.mSkinColor = FifamSkinColor::Latin2; // 12, 13
        else if (p->mSkinTone <= 15)
            player->mAppearance.mSkinColor = FifamSkinColor::African1; // 14, 15, 16
        else if (p->mSkinTone <= 17)
            player->mAppearance.mSkinColor = FifamSkinColor::African2; // 17, 18
        else
            player->mAppearance.mSkinColor = FifamSkinColor::African3; // 19, 20
        skinColorSet = true;
    }

    bool hairSet = false;
    if (p->mHairLength == 0) {
        player->mAppearance.mHairStyle = 1;
        hairSet = true;
    }

    bool hairColorSet = false;
    if (p->mHairColour >= 1 && p->mHairColour <= 6) {
        if (p->mHairColour == 1)
            player->mAppearance.mHairColor = FifamHairColor::Blonde;
        else if (p->mHairColour == 2)
            player->mAppearance.mHairColor = FifamHairColor::Lightbrown;
        else if (p->mHairColour == 3)
            player->mAppearance.mHairColor = FifamHairColor::Darkbrown;
        else if (p->mHairColour == 4)
            player->mAppearance.mHairColor = FifamHairColor::Red;
        else if (p->mHairColour == 5)
            player->mAppearance.mHairColor = FifamHairColor::Black;
        else if (p->mHairColour == 6)
            player->mAppearance.mHairColor = FifamHairColor::Black;
        hairColorSet = true;
    }

    // third - convert appearance from FIFA (if available)
    if (p->mConverterData.mFifaPlayerId > 0) {
        FifaPlayer *fifaPlayer = mFifaDatabase->GetPlayer(p->mConverterData.mFifaPlayerId);
        if (fifaPlayer)
            appearanceGenerator.SetFromFifaPlayer(player, fifaPlayer);
    }

    // fourth - apply custom appearance (if available)
    bool hasCustomFace = false;
    bool hasCustomHair = false;
    if (p->mConverterData.mEditorFace >= 1 && p->mConverterData.mEditorFace <= 135) {
        player->mAppearance.mGenericFace = p->mConverterData.mEditorFace - 1;
        hasCustomFace = true;
    }
    if (p->mConverterData.mEditorHair >= 1 && p->mConverterData.mEditorHair <= 98) {
        player->mAppearance.mHairStyle = FifamPlayerAppearance::GetHairIdFromEditor14Id(p->mConverterData.mEditorHair);
        hasCustomHair = true;
    }
    if (p->mConverterData.mEditorBeard >= 0 && p->mConverterData.mEditorBeard <= 8)
        player->mAppearance.mBeardType.SetFromInt(p->mConverterData.mEditorBeard);
    else if (hasCustomFace && hasCustomHair) {
        player->mAppearance.mBeardType = FifamBeardType::None;
        player->mAppearance.mBeardColor = FifamBeardColor::Black;
    }
    if (p->mConverterData.mEditorEye >= 0 && p->mConverterData.mEditorEye <= 6)
        player->mAppearance.mEyeColour.SetFromInt(p->mConverterData.mEditorEye);
    else if (hasCustomFace && hasCustomHair)
        player->mAppearance.mEyeColour = FifamEyeColor::Brown;
    if (p->mConverterData.mEditorSkin >= 1 && p->mConverterData.mEditorSkin <= 7)
        player->mAppearance.mSkinColor.SetFromInt(p->mConverterData.mEditorSkin - 1);
    if (hasCustomFace && hasCustomHair)
        player->mAppearance.mFaceVariation = FifamFaceVariation::Normal;

    // experience
    UInt totalExperiencePoints = 0;
    totalExperiencePoints += p->mInternationalApps * 10;
    for (auto &h : p->mVecPlayingHistory) {
        if (!h.mYouthTeam && h.mApps > 0) {
            if (h.mDivision) {
                switch (h.mDivision->mCompetitionLevel) {
                case 1:
                    totalExperiencePoints += h.mApps * 5;
                    break;
                case 2:
                    totalExperiencePoints += h.mApps * 2;
                    break;
                default:
                    totalExperiencePoints += h.mApps;
                }
            }
            else
                totalExperiencePoints += h.mApps;
        }
    }
    UInt minExperiencePoints = 0;
    //if (totalExperiencePoints == 0) {
        if (age >= 35)
            minExperiencePoints = 2500; // 18 | 3
        else if (age >= 34)
            minExperiencePoints = 2150; // 15 | 2
        else if (age >= 33)
            minExperiencePoints = 2000; // 14 | 1
        else if (age >= 32)
            minExperiencePoints = 1850; // 13 | 1
        else if (age >= 31)
            minExperiencePoints = 1700; // 12 | 1
        else if (age >= 30)
            minExperiencePoints = 1550; // 11 | 0
        else if (age >= 29)
            minExperiencePoints = 1400; // 10 | 0
        else if (age >= 28)
            minExperiencePoints = 1250; // 9 | 0
        else if (age >= 27)
            minExperiencePoints = 950; // 7 | -1
        else if (age >= 26)
            minExperiencePoints = 800; // 6 | -1
        else if (age >= 25)
            minExperiencePoints = 700; // 5 | -2
        else if (age >= 24)
            minExperiencePoints = 550; // 4 | -2
        else if (age >= 23)
            minExperiencePoints = 400; // 3 | -2
        else if (age >= 22)
            minExperiencePoints = 250; // 2 | -4
        else if (age >= 21)
            minExperiencePoints = 100; // 1 | -4
    //}
    if (totalExperiencePoints < minExperiencePoints)
        totalExperiencePoints = minExperiencePoints;
    player->mGeneralExperience = Utils::MapTo(totalExperiencePoints, 0, 2500, 0, 18);

    // potential
    player->mPotential = GetPlayerLevelFromCA(p->mPotentialAbility);

    // talent
    if (gameId >= 9) {
        if (p->mOriginalPA == -10) // 170-200
            player->mTalent = 9; // 5 stars
        else if (p->mOriginalPA == -95) // 160-190
            player->mTalent = 8; // 4,5 stars
        else if (p->mOriginalPA == -9) { // 150-180
            if (p->mOriginalCA >= 130)
                player->mTalent = 8; // 4,5 stars
            else
                player->mTalent = 7; // 4 stars
        }
        else if (p->mOriginalPA == -85) // 140-170
            player->mTalent = 7; // 4 stars
        else if (p->mOriginalPA == -8) { // 130-160
            if (p->mOriginalCA >= 135)
                player->mTalent = 7; // 4 stars
            else
                player->mTalent = 6; // 3.5 stars
        }
        else if (p->mOriginalPA == -75) // 120-150
            player->mTalent = 6; // 3.5 stars
        else {
            UInt maxTalent = 9;
            UChar *potantialAbilityRanges = nullptr;
            static UChar potentialAbilityRanges1to10[9] = { 35, 60, 80, 100, 119, 136, 155, 170, 190 };
            potantialAbilityRanges = potentialAbilityRanges1to10;
            player->mTalent = 0;
            for (UInt i = 0; i < maxTalent; i++) {
                if (p->mPotentialAbility >= potentialAbilityRanges1to10[maxTalent - 1 - i]) {
                    player->mTalent = maxTalent - i;
                    break;
                }
            }
        }
        //if (gameId >= 13) { // 6-star talents
        //    if (p->mID == 7458500 || p->mID == 735216)
        //        player->mTalent = 10;
        //}
    }
    else {
        if (player->mPotential >= 86) // Gigantic
            player->mTalent = 8;
        if (player->mPotential >= 76) // Outstanding
            player->mTalent = 6;
        else if (player->mPotential >= 66) // Very Big
            player->mTalent = 4;
        else if (player->mPotential >= 56) // Big
            player->mTalent = 2;
        else // Normal
            player->mTalent = 0;
    }

    Vector<Pair<Int, FifamPlayerPosition>> playerPositions = {
        { p->mGoalkeeper, FifamPlayerPosition::GK },
        { p->mDefenderLeft, FifamPlayerPosition::LB },
        { p->mDefenderCentral, FifamPlayerPosition::CB },
        { p->mDefenderRight, FifamPlayerPosition::RB },
        { p->mDefensiveMidfielder, FifamPlayerPosition::DM },
        { p->mWingBackLeft, FifamPlayerPosition::LWB },
        { p->mWingBackRight, FifamPlayerPosition::RWB },
        { p->mMidfielderLeft, FifamPlayerPosition::LM },
        { p->mMidfielderCentral, FifamPlayerPosition::CM },
        { p->mMidfielderRight, FifamPlayerPosition::RM },
        { p->mAttackingMidfielderLeft, FifamPlayerPosition::LW },
        { p->mAttackingMidfielderCentral, FifamPlayerPosition::AM },
        { p->mAttackingMidfielderRight, FifamPlayerPosition::RW },
        { p->mStriker, FifamPlayerPosition::ST }
    };
    // position
    auto bestPos = Utils::GetMaxElementId<Int, FifamPlayerPosition>(playerPositions);
    if (bestPos.first != 20)
        Error(L"Player has no preferred position\nPlayer: %s\nBest pos: %s (%d)", p->mFullName.c_str(), bestPos.second.ToCStr(), bestPos.first);

    player->mMainPosition = bestPos.second;
    player->mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(player->mMainPosition, gameId);
    player->mPositionBias[player->mMainPosition.ToInt()] = 100;
    for (UInt i = 0; i < playerPositions.size(); i++) {
        Int value = playerPositions[i].first;
        Int bias = 40;
        if (gameId >= 13) {
            if (value == 20)
                bias = 100;
            else if (value == 19)
                bias = 99;
            else if (value == 18)
                bias = 98;
            else if (value == 17)
                bias = Random::Get(96, 97);
            else if (value == 16)
                bias = Random::Get(94, 95);
            else if (value == 15)
                bias = Random::Get(92, 93);
            else if (value == 14)
                bias = Random::Get(90, 91);
            else if (value == 13)
                bias = Random::Get(86, 89);
            else if (value == 12)
                bias = Random::Get(82, 85);
            else if (value == 11)
                bias = Random::Get(78, 81);
            else if (value == 10)
                bias = Random::Get(74, 77);
            else if (value == 9)
                bias = Random::Get(70, 73);
            else if (value == 8)
                bias = Random::Get(66, 69);
            else if (value == 7)
                bias = Random::Get(62, 65);
            else if (value == 6)
                bias = Random::Get(58, 61);
            else if (value == 5)
                bias = Random::Get(54, 57);
            else if (value == 4)
                bias = Random::Get(50, 53);
            else if (value == 3)
                bias = Random::Get(46, 49);
            else if (value == 2)
                bias = Random::Get(41, 45);
            else if (value == 1)
                bias = 40;
        }
        else {
            bias = 20;
            if (value == 20)
                bias = 100;
            else if (value == 19)
                bias = 99;
            else if (value == 18)
                bias = 98;
            else if (value == 17)
                bias = Random::Get(96, 97);
            else if (value == 16)
                bias = Random::Get(91, 95);
            else if (value == 15)
                bias = Random::Get(86, 90);
            else if (value == 14)
                bias = Random::Get(81, 85);
            else if (value == 13)
                bias = Random::Get(76, 80);
            else if (value == 12)
                bias = Random::Get(71, 75);
            else if (value == 11)
                bias = Random::Get(66, 70);
            else if (value == 10)
                bias = Random::Get(61, 65);
            else if (value == 9)
                bias = Random::Get(56, 60);
            else if (value == 8)
                bias = Random::Get(51, 55);
            else if (value == 7)
                bias = Random::Get(46, 50);
            else if (value == 6)
                bias = Random::Get(41, 45);
            else if (value == 5)
                bias = Random::Get(36, 40);
            else if (value == 4)
                bias = Random::Get(31, 35);
            else if (value == 3)
                bias = Random::Get(26, 30);
            else if (value == 2)
                bias = Random::Get(21, 25);
            else if (value == 1)
                bias = 20;
        }
        if (player->mPositionBias[playerPositions[i].second.ToInt()] < bias)
            player->mPositionBias[playerPositions[i].second.ToInt()] = (Float)bias;
    }

    // attributes
    struct ConvertedAttributes {
        Int mAggression = 0;
        Int mAnticipation = 0;
        Int mBravery = 0;
        Int mComposure = 0;
        Int mConcentration = 0;
        Int mConsistency = 0;
        Int mVision = 0;
        Int mDecisions = 0;
        Int mDetermination = 0;
        Int mDirtiness = 0;
        Int mFlair = 0;
        Int mImportantMatches = 0;
        Int mLeadership = 0;
        Int mMovement = 0;
        Int mPositioning = 0;
        Int mTeamWork = 0;
        Int mWorkRate = 0;
        Int mAcceleration = 0;
        Int mAgility = 0;
        Int mBalance = 0;
        Int mInjuryProneness = 0;
        Int mJumpingReach = 0;
        Int mNaturalFitness = 0;
        Int mPace = 0;
        Int mStamina = 0;
        Int mStrength = 0;
        Int mCorners = 0;
        Int mCrossing = 0;
        Int mDribbling = 0;
        Int mFinishing = 0;
        Int mFirstTouch = 0;
        Int mFreeKicks = 0;
        Int mHeading = 0;
        Int mLongShots = 0;
        Int mLongThrows = 0;
        Int mMarking = 0;
        Int mPassing = 0;
        Int mPenaltyTaking = 0;
        Int mTackling = 0;
        Int mTechnique = 0;
        Int mVersatility = 0;
        Int mAerialAbility = 0;
        Int mCommandOfArea = 0;
        Int mCommunication = 0;
        Int mEccentricity = 0;
        Int mHandling = 0;
        Int mKicking = 0;
        Int mOneOnOnes = 0;
        Int mReflexes = 0;
        Int mRushingOut = 0;
        Int mTendencyToPunch = 0;
        Int mThrowing = 0;
    } attr;

    /*
    UInt playerLevel = 1;
    static Int playerLevelAry[20] = {
          0, //  1 -1
         50, //  2 
         70, //  3 -2
         90, //  4 
        100, //  5 -3
        105, //  6 
        110, //  7 -4
        115, //  8 
        125, //  9 -5
        130, // 10 
        135, // 11 -6
        140, // 12 
        145, // 13 -7
        150, // 14 
        155, // 15 -8
        160, // 16 
        165, // 17 -9
        170, // 18 
        188, // 19 -10
        190, // 20 
    };
    for (Int i = 19; i >= 0; i--) {
        if (p->mCurrentAbility >= playerLevelAry[i]) {
            playerLevel = i + 2;
            break;
        }
    }
    playerLevel /= 2;
    */

    attr.mAggression = ConvertPlayerAttribute(p->mAggression, gameId);
    attr.mAnticipation = ConvertPlayerAttribute(p->mAnticipation, gameId);
    attr.mBravery = ConvertPlayerAttribute(p->mBravery, gameId);
    attr.mComposure = ConvertPlayerAttribute(p->mComposure, gameId);
    attr.mConcentration = ConvertPlayerAttribute(p->mConcentration, gameId);
    attr.mConsistency = ConvertPlayerAttribute(p->mConsistency, gameId);
    attr.mVision = ConvertPlayerAttribute(p->mVision, gameId);
    attr.mDecisions = ConvertPlayerAttribute(p->mDecisions, gameId);
    attr.mDetermination = ConvertPlayerAttribute(p->mDetermination, gameId);
    attr.mDirtiness = ConvertPlayerAttribute(p->mDirtiness, gameId);
    attr.mFlair = ConvertPlayerAttribute(p->mFlair, gameId);
    attr.mImportantMatches = ConvertPlayerAttribute(p->mImportantMatches, gameId);
    attr.mLeadership = LinearConvertPlayerAttribute(p->mLeadership, gameId);
    attr.mMovement = ConvertPlayerAttribute(p->mMovement, gameId);
    attr.mPositioning = ConvertPlayerAttribute(p->mPositioning, gameId);
    attr.mTeamWork = ConvertPlayerAttribute(p->mTeamWork, gameId);
    attr.mWorkRate = ConvertPlayerAttribute(p->mWorkRate, gameId);
    attr.mAcceleration = ConvertPlayerAttribute(p->mAcceleration, gameId);
    attr.mAgility = ConvertPlayerAttribute(p->mAgility, gameId);
    attr.mBalance = ConvertPlayerAttribute(p->mBalance, gameId);
    attr.mInjuryProneness = ConvertPlayerAttribute(p->mInjuryProneness, gameId);
    attr.mJumpingReach = ConvertPlayerAttribute(p->mJumpingReach, gameId);
    attr.mNaturalFitness = ConvertPlayerAttribute(p->mNaturalFitness, gameId);
    attr.mPace = ConvertPlayerAttribute(p->mPace, gameId);
    attr.mStamina = ConvertPlayerAttribute(p->mStamina, gameId);
    attr.mStrength = ConvertPlayerAttribute(p->mStrength, gameId);
    attr.mCorners = ConvertPlayerAttribute(p->mCorners, gameId);
    attr.mCrossing = ConvertPlayerAttribute(p->mCrossing, gameId);
    attr.mDribbling = ConvertPlayerAttribute(p->mDribbling, gameId);
    attr.mFinishing = ConvertPlayerAttribute(p->mFinishing, gameId);
    attr.mFirstTouch = ConvertPlayerAttribute(p->mFirstTouch, gameId);
    attr.mFreeKicks = ConvertPlayerAttribute(p->mFreeKicks, gameId);
    attr.mHeading = ConvertPlayerAttribute(p->mHeading, gameId);
    attr.mLongShots = ConvertPlayerAttribute(p->mLongShots, gameId);
    attr.mLongThrows = ConvertPlayerAttribute(p->mLongThrows, gameId);
    attr.mMarking = ConvertPlayerAttribute(p->mMarking, gameId);
    attr.mPassing = ConvertPlayerAttribute(p->mPassing, gameId);
    attr.mPenaltyTaking = ConvertPlayerAttribute(p->mPenaltyTaking, gameId);
    attr.mTackling = ConvertPlayerAttribute(p->mTackling, gameId);
    attr.mTechnique = ConvertPlayerAttribute(p->mTechnique, gameId);
    attr.mVersatility = ConvertPlayerAttribute(p->mVersatility, gameId);
    attr.mAerialAbility = ConvertPlayerAttribute(p->mAerialAbility, gameId);
    attr.mCommandOfArea = ConvertPlayerAttribute(p->mCommandOfArea, gameId);
    attr.mCommunication = ConvertPlayerAttribute(p->mCommunication, gameId);
    attr.mEccentricity = ConvertPlayerAttribute(p->mEccentricity, gameId);
    attr.mHandling = ConvertPlayerAttribute(p->mHandling, gameId);
    attr.mKicking = ConvertPlayerAttribute(p->mKicking, gameId);
    attr.mOneOnOnes = ConvertPlayerAttribute(p->mOneOnOnes, gameId);
    attr.mReflexes = ConvertPlayerAttribute(p->mReflexes, gameId);
    attr.mRushingOut = ConvertPlayerAttribute(p->mRushingOut, gameId);
    attr.mTendencyToPunch = ConvertPlayerAttribute(p->mTendencyToPunch, gameId);
    attr.mThrowing = ConvertPlayerAttribute(p->mThrowing, gameId);

    //attributes: general
    player->mAttributes.BallControl = attr.mTechnique;
    player->mAttributes.Volleys = (UChar)ceilf((attr.mTechnique + attr.mFinishing + attr.mLongShots) / 3.0f);
    player->mAttributes.Dribbling = attr.mDribbling;
    player->mAttributes.Finishing = attr.mFinishing;
    Int shotPowerBaseAttr = attr.mLongShots;
    if (attr.mFinishing > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mFinishing;
    if (attr.mKicking > shotPowerBaseAttr)
        shotPowerBaseAttr = attr.mKicking;
    player->mAttributes.ShotPower = Utils::Clamp((UChar)ceilf((shotPowerBaseAttr * 3 + attr.mStrength) / 4.0f) +
        (p->mShootsWithPower ? 10 : 0), 1, 99);
    player->mAttributes.LongShots = attr.mLongShots;
    player->mAttributes.Crossing = attr.mCrossing;
    player->mAttributes.Passing = attr.mPassing;
    player->mAttributes.LongPassing = (UChar)ceilf((attr.mPassing + attr.mVision) / 2.0f);
    player->mAttributes.Header = attr.mHeading;
    player->mAttributes.TacklingGeneral = attr.mTackling;
    player->mAttributes.TackleStanding = (UChar)ceilf((attr.mTackling + attr.mMarking) / 2.0f);
    player->mAttributes.TackleSliding = (UChar)ceilf((attr.mTackling + attr.mDecisions) / 2.0f);
    if (p->mDivesIntoTackles)
        player->mAttributes.TackleSliding += 5;
    else if (p->mDoesNotDiveIntoTackles)
        player->mAttributes.TackleSliding -= 10;
    player->mAttributes.TackleSliding = Utils::Clamp(player->mAttributes.TackleSliding, 1, 99);
    player->mAttributes.ManMarking = attr.mMarking;
    player->mAttributes.Technique = attr.mTechnique;
    player->mAttributes.Creativity = (UChar)ceilf((attr.mFlair + attr.mVision) / 2.0f);
    player->mAttributes.Flair = attr.mFlair;
    player->mAttributes.Touch = attr.mFirstTouch;
    // attributes: physical
    player->mAttributes.Balance = attr.mBalance;
    player->mAttributes.Acceleration = attr.mAcceleration;
    player->mAttributes.Pace = attr.mPace;
    player->mAttributes.Agility = attr.mAgility;
    player->mAttributes.Jumping = attr.mJumpingReach;
    player->mAttributes.Strength = attr.mStrength;
    player->mAttributes.Stamina = attr.mStamina;
    player->mAttributes.WorkRate = attr.mWorkRate;
    player->mAttributes.ForwardRuns = (UChar)ceilf((attr.mMovement + (UChar)ceilf((attr.mPace + attr.mAcceleration) / 2.0f)) / 2.0f);
    // attributes: mental
    player->mAttributes.PosOffensive = attr.mMovement;
    player->mAttributes.PosDefensive = attr.mPositioning;
    player->mAttributes.Vision = attr.mVision;
    player->mAttributes.Reactions = (UChar)ceilf((attr.mAnticipation + attr.mFirstTouch) / 2.0f);
    player->mAttributes.TacticAwareness = attr.mTeamWork;
    player->mAttributes.Aggression = attr.mAggression;
    player->mAttributes.Composure = attr.mComposure;
    player->mAttributes.Consistency = attr.mConsistency;
    player->mAttributes.Leadership = attr.mLeadership;
    player->mAttributes.Anticipation = attr.mAnticipation;
    player->mAttributes.Concentration = attr.mConcentration;
    player->mAttributes.Decision = attr.mDecisions;
    player->mAttributes.Determination = attr.mDetermination;
    player->mAttributes.TeamWork = attr.mTeamWork;
    player->mAttributes.Intimidation = attr.mAggression;
    // atributes : set pieces
    player->mAttributes.FreeKicks = attr.mFreeKicks;
    player->mAttributes.Corners = attr.mCorners;
    player->mAttributes.PenaltyShot = attr.mPenaltyTaking;
    // attributes: goalkeeping
    Int gkJumping = attr.mJumpingReach;
    if (attr.mAerialAbility > gkJumping)
        gkJumping = p->mAerialAbility;
    if (player->mMainPosition == FifamPlayerPosition::GK)
        player->mAttributes.Diving = (UChar)ceilf((attr.mReflexes + gkJumping) / 2.0f);
    else
        player->mAttributes.Diving = attr.mAerialAbility;
    player->mAttributes.GkCrosses = (UChar)ceilf((attr.mPassing + attr.mKicking) / 2.0f);
    player->mAttributes.Handling = attr.mHandling;
    player->mAttributes.Kicking = attr.mKicking;
    player->mAttributes.Positioning = (UChar)ceilf((attr.mRushingOut + attr.mCommandOfArea) / 2.0f);
    player->mAttributes.OneOnOne = attr.mOneOnOnes;
    player->mAttributes.Reflexes = attr.mReflexes;
    player->mAttributes.Punching = attr.mTendencyToPunch;
    player->mAttributes.ShotStopping = (UChar)ceilf((attr.mReflexes + attr.mOneOnOnes) / 2.0f);
    player->mAttributes.Throwing = attr.mThrowing;

    Int cfPosFirst = p->mStriker;
    Int cfPosSecond = p->mAttackingMidfielderCentral;
    if (cfPosSecond > cfPosFirst)
        std::swap(cfPosFirst, cfPosSecond);
    if (cfPosFirst == 20) {
        if (cfPosSecond >= 19)
            player->mPositionBias[FifamPlayerPosition::CF] = 100;
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(98, 99);
        else if (cfPosSecond == 18)
            player->mPositionBias[FifamPlayerPosition::CF] = (Float)Random::Get(96, 97);
    }
    if (player->mPositionBias[FifamPlayerPosition::CF] == 100) {
        UChar currentLevel = GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId);
        auto savedPos = player->mMainPosition;
        player->mMainPosition = FifamPlayerPosition::CF;
        if (GetPlayerLevel(player, player->mMainPosition, FifamPlayerLevel::GetBestStyleForPlayer(player), false, gameId) <= currentLevel)
            player->mMainPosition = savedPos;
    }

    // styles table
    auto GetPlayerStylePoints = [](foom::player *a, FifamPlayerPlayingStyle style) -> Int {
        UChar s = style.ToInt();
        if (s == FifamPlayerPlayingStyle::AttackingFB)
            return (a->mCutsInsideFromLeftWing || a->mCutsInsideFromRightWing || a->mCutsInsideFromBothWings) * 100 + Utils::Max(a->mJumpingReach, a->mAcceleration);
        else if (s == FifamPlayerPlayingStyle::DefenceFB)
            return a->mStaysBackAtAllTimes * 100 + (a->mMarking + a->mTackling) / 2;
        else if (s == FifamPlayerPlayingStyle::Libero) // Builds up
            return a->mTeamWork + a->mPositioning;
        else if (s == FifamPlayerPlayingStyle::SimplePasser)
            return a->mPlaysShortSimplePasses * 100 + a->mPassing;
        else if (s == FifamPlayerPlayingStyle::BallWinner)
            return a->mDivesIntoTackles * 100 + a->mTackling;
        else if (s == FifamPlayerPlayingStyle::HardMan) // Fighter
            return a->mDivesIntoTackles * 100 + a->mAggression;
        else if (s == FifamPlayerPlayingStyle::Holding) // Closes gaps
            return a->mTeamWork + a->mWorkRate;
        else if (s == FifamPlayerPlayingStyle::BallWinnerMidfield)
            return a->mMarksOpponentTightly * 100 + (a->mTackling + a->mPositioning) / 2;
        else if (s == FifamPlayerPlayingStyle::BoxToBox)
            return (a->mWorkRate + a->mStamina);
        else if (s == FifamPlayerPlayingStyle::Busy)
            return a->mWorkRate * 2;
        else if (s == FifamPlayerPlayingStyle::PlayMaker)
            return a->mDictatesTempo * 100 + (a->mFlair + a->mVision) / 2;
        else if (s == FifamPlayerPlayingStyle::Dribbler)
            return (a->mRunsWithBallOften || a->mTriesToPlayWayOutOfTrouble) * 100 + a->mDribbling;
        else if (s == FifamPlayerPlayingStyle::Winger)
            return (a->mCutsInsideFromLeftWing || a->mCutsInsideFromRightWing || a->mCutsInsideFromBothWings || a->mHugsLine) * 100 + (a->mAcceleration + a->mPace + a->mCrossing) / 3;
        else if (s == FifamPlayerPlayingStyle::TargetMan)
            return a->mPenaltyBoxPlayer * 100 + (a->mFinishing + a->mHeading) / 2;
        else if (s == FifamPlayerPlayingStyle::PenaltyBox)
            return a->mPenaltyBoxPlayer * 100 + a->mFinishing;
        else if (s == FifamPlayerPlayingStyle::RunsChannels)
            return (a->mMovesIntoChannels) * 100 + (a->mAcceleration + a->mPace) / 2;
        else if (s == FifamPlayerPlayingStyle::PullsWideLeft)
            return (a->mRunsWithBallDownLeft && !a->mRunsWithBallDownRight) * 100 + a->mMovement;
        else if (s == FifamPlayerPlayingStyle::PullsWideRight)
            return (a->mRunsWithBallDownRight && !a->mRunsWithBallDownLeft) * 100 + a->mMovement;
        else if (s == FifamPlayerPlayingStyle::DribblerAttack)
            return (a->mRunsWithBallOften || a->mTriesToPlayWayOutOfTrouble) * 100 + a->mDribbling;
        else if (s == FifamPlayerPlayingStyle::HoldsUp)
            return a->mStopsPlay * 100 + (a->mDribbling + a->mTechnique) / 2;
        else if (s == FifamPlayerPlayingStyle::BusyAttacker)
            return a->mWorkRate * 2;
        else if (s == FifamPlayerPlayingStyle::TowerStrength)
            return a->mStrength * 2;
        else if (s == FifamPlayerPlayingStyle::DistanceShooter)
            return a->mShootsFromDistance * 100 + a->mLongShots;
        return 0;
    };

    auto bestStyles = FifamPlayerLevel::GetBestStylesForPlayer(player, gameId >= 13);
    if (bestStyles.empty())
        player->mPlayingStyle = FifamPlayerPlayingStyle::None;
    else if (bestStyles.size() == 1)
        player->mPlayingStyle = bestStyles[0];
    else {
        Int bestStyleScore = GetPlayerStylePoints(p, bestStyles[0]);
        FifamPlayerPlayingStyle bestStyle = bestStyles[0];
        for (UInt i = 1; i < bestStyles.size(); i++) {
            Int stylePoints = GetPlayerStylePoints(p, bestStyles[i]);
            if (stylePoints > bestStyleScore) {
                bestStyle = bestStyles[i];
                bestStyleScore = stylePoints;
            }
        }
        player->mPlayingStyle = bestStyle;
    }

    //std::wcout << p->mID << std::endl;

    UChar desiredLevel = GetPlayerLevelFromCA(p->mCurrentAbility > 0 ? p->mCurrentAbility : 1);
    if (gameId == 11 || gameId == 12) {
        Int modifier = 0;
        if (player->mGeneralExperience >= 18)
            modifier = 3;
        else if (player->mGeneralExperience >= 15)
            modifier = 2;
        else if (player->mGeneralExperience >= 12)
            modifier = 1;
        else if (player->mGeneralExperience >= 9)
            modifier = 0;
        else if (player->mGeneralExperience >= 6)
            modifier = -1;
        else if (player->mGeneralExperience >= 3)
            modifier = -2;
        else
            modifier = -4;
        player->mMaxBias = (Float)(Utils::Clamp(desiredLevel - modifier, 1, 99));
    }
    else {
        UChar currLevel = GetPlayerLevel(player, true, gameId);
        Int levelDiff = desiredLevel - currLevel;
        if (levelDiff != 0) {
            Int levelCorrection = 1;
            Int levelIteration = levelDiff * 2;
            if (levelDiff < 0) {
                levelCorrection = -1;
                levelIteration *= -1;
            }
            do {
                player->ForAllAttributes([&](UChar &attr, FifamPlayerAbilityID const &attrId) {
                    if (!FifamPlayerAttributes::IsSetPiecesAttribute(attrId)) {
                        if (FifamPlayerAttributes::IsGoalkeeperAttribute(attrId)) {
                            if (player->mMainPosition == FifamPlayerPosition::GK || (levelCorrection == -1 && desiredLevel <= 20)) {
                                attr = Utils::Clamp((Int)attr + levelCorrection, 1, 99);
                                if (attr == 99 && Random::Get(0, 1) == 1)
                                    attr = 98;
                            }
                        }
                        else {
                            if (player->mMainPosition != FifamPlayerPosition::GK || (levelCorrection == -1 && desiredLevel <= 20)) {
                                attr = Utils::Clamp((Int)attr + levelCorrection, 1, 99);
                                if (attr == 99 && Random::Get(0, 1) == 1)
                                    attr = 98;
                            }
                        }
                    }
                });
                levelIteration--;
                currLevel = GetPlayerLevel(player, true, gameId);
            } while (levelIteration && ((levelCorrection == -1 && currLevel > desiredLevel) || (levelCorrection == 1 && currLevel < desiredLevel)));
        }
    }

    player->mTacticalEducation = Utils::MapTo(player->mAttributes.TacticAwareness, 1, 99, 0, 7);

    // update potential if needed
    UChar finalLevel = GetPlayerLevel(player, true, gameId);
    if (player->mPotential < finalLevel)
        player->mPotential = finalLevel;

    // contract
    if (club) {
        if (p->mContract.mDateJoined > FmEmptyDate() && p->mContract.mDateJoined < GetCurrentDate())
            player->mContract.mJoined = GetDateAlignedToSeasonStart(p->mContract.mDateJoined, true, false);
        else
            player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        if (p->mContract.mWage > 0)
            player->mContract.mBasicSalary = foom::db::convert_money(p->mContract.mWage) * 52;
        if (p->mContract.mContractExpires.year <= CURRENT_YEAR)
            player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR);
        else
            player->mContract.mValidUntil = GetDateAlignedToSeasonEnd(p->mContract.mContractExpires, false, true);

        if (p->mContract.mWillLeaveAtEndOfContract)
            player->mNoContractExtension = true;
        if (p->mContract.mMatchHighestEarnerClause)
            player->mContract.mHighestPaidPlayer = true;
        if (p->mContract.mAppearanceFee > 0)
            player->mContract.mMatchBonus = foom::db::convert_money(p->mContract.mAppearanceFee);
        if (player->mMainPosition == FifamPlayerPosition::GK) {
            if (p->mContract.mCleanSheetBonus > 0)
                player->mContract.mGoalsCleanSheetBonus = foom::db::convert_money(p->mContract.mCleanSheetBonus);
        }
        else {
            if (p->mContract.mGoalBonus > 0)
                player->mContract.mGoalsCleanSheetBonus = foom::db::convert_money(p->mContract.mGoalBonus);
        }
        if (p->mContract.mYearlyWageRise > 0)
            player->mContract.mSalaryIncrease = Utils::Clamp(p->mContract.mYearlyWageRise, 1, 100);
        if (p->mContract.mRelegationWageDrop > 0)
            player->mContract.mSalaryDecreaseOnRelegation = Utils::Clamp(p->mContract.mRelegationWageDrop, 1, 100);
        if (p->mContract.mOneYearExtensionAfterLeagueGamesFinalSeason > 0)
            player->mContract.mAutoExtend = Utils::Clamp(p->mContract.mOneYearExtensionAfterLeagueGamesFinalSeason, 1, 34);
        if (p->mContract.mOptionalContractExtensionByClub > 0)
            player->mContract.mOptionClub = Utils::Clamp(p->mContract.mOptionalContractExtensionByClub, 1, 2);
        if (p->mContract.mMinimumFeeReleaseClause > 0) {
            if (p->mContract.mMinimumFeeReleaseClauseExpiryDate == FmEmptyDate() || p->mContract.mMinimumFeeReleaseClauseExpiryDate > GetCurrentDate())
                player->mContract.mFixTransferFee = foom::db::convert_money(p->mContract.mMinimumFeeReleaseClause);
        }
        if (p->mContract.mSellOnFeePercentage > 0) {
            player->mContract.mSellOnPercentage = Utils::Clamp(p->mContract.mSellOnFeePercentage, 1, 100);
            player->mContract.mSellOnFee = 100;
        }
    }
    else {
        player->mContract.mJoined.Set(1, 7, CURRENT_YEAR);
        player->mContract.mValidUntil.Set(30, 6, CURRENT_YEAR);
    }

    // player agent
    if (p->mCurrentAbility >= 100) {
        Int randVal = Random::Get(0, 100);
        if (randVal >= 65) {
            if (randVal >= 90)
                player->mPlayerAgent = FifamPlayerAgent::Lawyer;
            else
                player->mPlayerAgent = FifamPlayerAgent::Agent;
        }
        else
            player->mPlayerAgent = FifamPlayerAgent::Agency;
    }
    else
        player->mPlayerAgent = FifamPlayerAgent::None;

    // national team
    FifamCountry *playerOriginalCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerID);
    if (IsPlayerRetiredFromNationalTeam(p->mID))
        player->mRetiredFromNationalTeam = true;
    else {
        if (p->mInternationalRetirement || (p->mInternationalRetirementDate > Date(1, 1, 2000) && p->mInternationalRetirementDate <= GetCurrentDate()))
            player->mRetiredFromNationalTeam = true;
    }
    if (!player->mRetiredFromNationalTeam && playerOriginalCountry && p->mDeclaredForNation)
        mNationalTeamPlayers[playerOriginalCountry->mId - 1].push_back(p);
    player->mNationalTeamMatches = p->mInternationalApps;
    player->mNationalTeamGoals = p->mInternationalGoals;

    // character
    if (p->mPressure >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::StrongNerves, true);
    else if (p->mPressure <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::WeakNerves, true);

    if (p->mProfessionalism >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Professionalism, true);
    else if (p->mProfessionalism <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::ScandalProne, true);

    if (p->mAmbition >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Ambition, true);
    else if (p->mAmbition <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::LacksAmbition, true);

    if (p->mAdaptability >= 18)
        player->mCharacter.Set(FifamPlayerCharacter::Adaptability, true);

    if (player->mLanguages[3] != FifamLanguage::None)
        player->mCharacter.Set(FifamPlayerCharacter::LanguageGenius, true);
    else if (player->mLanguages[1] == FifamLanguage::None && p->mAdaptability <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::DoesntLikeNewLang, true);

    if (p->mSportsmanship >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::Fairness, true);
    else if (p->mSportsmanship <= 6)
        player->mCharacter.Set(FifamPlayerCharacter::HardMan, true);

    if (OriginalAttrValue(p->mInjuryProneness) >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::InjuryProne, true);
    else if (OriginalAttrValue(p->mInjuryProneness) <= 4) // maybe 3
        player->mCharacter.Set(FifamPlayerCharacter::LittleInjuryProne, true);

    if (p->mControversy >= 15)
        player->mCharacter.Set(FifamPlayerCharacter::MediaDarling, true);
    else if (p->mControversy <= 1)
        player->mCharacter.Set(FifamPlayerCharacter::UninterestedInMedia, true);

    if (p->mLoyalty >= 19) // maybe 17-18
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationHigh, true);
    else if (p->mLoyalty <= 5) // maybe 4
        player->mCharacter.Set(FifamPlayerCharacter::IdentificationLow, true);

    if (p->mHitsFreekicksWithPower)
        player->mCharacter.Set(FifamPlayerCharacter::DrivenFreeKicks, true);

    if (player->mMainPosition == FifamPlayerPosition::GK) {
        if (p->mUsesLongThrowToStartCounterAttacks || OriginalAttrValue(p->mThrowing) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrowOuts, true);
    }
    else {
        if (p->mPossessesLongFlatThrow || OriginalAttrValue(p->mLongThrows) >= 15)
            player->mCharacter.Set(FifamPlayerCharacter::LongThrows, true);

        if (OriginalAttrValue(p->mVersatility) >= 17) // maybe 15-16
            player->mCharacter.Set(FifamPlayerCharacter::Flexibility, true);
        else if (OriginalAttrValue(p->mVersatility) <= 4) // maybe 3
            player->mCharacter.Set(FifamPlayerCharacter::Inflexibility, true);

        Int teamWork = OriginalAttrValue(p->mTeamWork);
        if (teamWork > 0 && teamWork <= 8) {
            Float teamPlayerRating = (Float)p->mCurrentAbility / teamWork;
            if (teamPlayerRating >= 13.4f) // magic
                player->mCharacter.Set(FifamPlayerCharacter::Egoist, true);
        }
    }

    if (!player->mCharacter.Check(FifamPlayerCharacter::Egoist)) {
        if (OriginalAttrValue(p->mTeamWork) >= 16) // maybe 15/17
            player->mCharacter.Set(FifamPlayerCharacter::Teamplayer, true);
    }

    if (OriginalAttrValue(p->mWorkRate) >= 18) // maybe 17/19
        player->mCharacter.Set(FifamPlayerCharacter::HighTrainingWorkRate, true);
    else {
        if (!player->mCharacter.Check(FifamPlayerCharacter::Professionalism)) {
            Int workRate = OriginalAttrValue(p->mWorkRate);
            if (workRate > 0 && workRate <= 8) {
                Float workRateRating = (Float)p->mCurrentAbility / workRate;
                if (workRateRating >= 17.0f) // magic
                    player->mCharacter.Set(FifamPlayerCharacter::LazyInTraining, true);
            }
        }
    }

    if (p->mTemperament >= 20) {
        if (p->mCurrentAbility >= 80)
            player->mCharacter.Set(FifamPlayerCharacter::Composure, true);
    }
    else if (p->mTemperament <= 5)
        player->mCharacter.Set(FifamPlayerCharacter::Temperament, true);

    player->mCharacter.Set(FifamPlayerCharacter::LifestyleIcon, IsIconicPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Introvert, IsIntrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Extrovert, IsExtrovertPlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::FansFavorite, IsFansFavouritePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::Diva, IsSensitivePlayer(p->mID));
    player->mCharacter.Set(FifamPlayerCharacter::NeedsAttention, IsInsecurePlayer(p->mID));

    // hero
    UChar heroStatus = 0;
    if (OriginalAttrValue(p->mLeadership) > 17)
        heroStatus += OriginalAttrValue(p->mLeadership) - 17;
    if (OriginalAttrValue(p->mBravery) > 17)
        heroStatus += OriginalAttrValue(p->mBravery) - 17;
    if (player->mGeneralExperience >= 17)
        heroStatus += player->mGeneralExperience - 2;
    if (p->mWorldReputation >= 9500)
        heroStatus += 9;
    else if (p->mWorldReputation >= 8500)
        heroStatus += 3;
    else if (p->mWorldReputation >= 8000)
        heroStatus += 1;
    if (heroStatus > 0) {
        if (heroStatus >= 8)
            player->mHeroStatus = 4;
        else if (heroStatus == 7)
            player->mHeroStatus = 3;
        else if (heroStatus == 6) {
            if (Random::Get(0, 1) == 1)
                player->mHeroStatus = 6;
            else
                player->mHeroStatus = 10;
        }
        else if (heroStatus == 5)
            player->mHeroStatus = 2;
        else if (heroStatus == 4) {
            if (Random::Get(0, 1) == 1)
                player->mHeroStatus = 5;
            else
                player->mHeroStatus = 9;
        }
        else if (heroStatus == 3)
            player->mHeroStatus = 1;
        else if (heroStatus == 2)
            player->mHeroStatus = 8;
        else if (heroStatus == 1)
            player->mHeroStatus = 7;
    }

    // starting conditions

    // retirement
    for (auto &i : p->mVecRetirements) {
        if (i.mType == 1 && i.mDate > GetCurrentDate() && i.mDate.year <= (CURRENT_YEAR + 1)) {
            player->mStartingConditions.mRetirement.Setup();
            break;
        }
    }
    // bans
    for (auto &b : p->mVecBans) {
        // league ban
        if (!player->mStartingConditions.mLeagueBan.mEnabled && b.mNumberMatches > 0 && b.mStartDate == FmEmptyDate() && b.mEndDate == FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 1 || b.mBanType == 3)
                player->mStartingConditions.mLeagueBan.Setup(b.mNumberMatches);
        }
        // ban until
        else if (!player->mStartingConditions.mBanUntil.mEnabled && b.mNumberMatches == 0 && b.mStartDate != FmEmptyDate() && b.mEndDate != FmEmptyDate()) {
            if (b.mBanType == 1 || b.mBanType == 1 || b.mBanType == 3 || b.mBanType == 17 || b.mBanType == 18) {
                if (b.mStartDate <= GetCurrentDate() && b.mEndDate > GetCurrentDate())
                    player->mStartingConditions.mBanUntil.Setup(b.mEndDate);
            }
        }
    }

    // injury
    for (auto &i : p->mVecInjuries) {
        if (i.mInjury && i.mStartDate != FmEmptyDate() && i.mStartDate <= GetCurrentDate() && i.mEndDate > GetCurrentDate()) {
            if (i.mInjury->mConverterData.mFIFAManagerID != 0) {
                FifamPlayerInjuryType injuryType = FifamPlayerInjuryType::MakeFromInt(i.mInjury->mConverterData.mFIFAManagerID);
                player->mStartingConditions.mInjury.Setup(i.mStartDate, i.mEndDate, injuryType);
                break;
            }
        }
    }

    // debug
    player->mEmpicsId = p->mID;

    return player;
}

FifamPlayer *Converter::CreateAndConvertFifaPlayer(UInt gameId, FifaPlayer *p, FifamClub *club) {
    return nullptr;
}

UChar GetManagerLevelFromCA(Int ca) {
    static Pair<UChar, UChar> managerCAtoLvlAry[15] = {
        { 15,  185 },
        { 14,  170 },
        { 13,  160 },
        { 12,  151 },
        { 11,  140 },
        { 10,  130 },
        { 9,   120 },
        { 8,   110 },
        { 7,   100 },
        { 6,    90 },
        { 5,    80 },
        { 4,    70 },
        { 3,    50 },
        { 2,    25 },
        { 1,     0 }
    };
    for (UInt i = 0; i < 15; i++) {
        if (ca >= managerCAtoLvlAry[i].second)
            return managerCAtoLvlAry[i].first;
    }
    return 1;
}

UChar GetStaffLevelFromCA(Int ca) {
    static Pair<UChar, UChar> staffCAtoLvlAry[99] = {
        { 99, 190 },
        { 98, 185 },
        { 97, 180 },
        { 96, 175 },
        { 95, 170 },
        { 94, 167 },
        { 93, 164 },
        { 92, 161 },
        { 91, 158 },
        { 90, 155 },
        { 89, 152 },
        { 88, 149 },
        { 87, 146 },
        { 86, 143 },
        { 85, 140 },
        { 84, 138 },
        { 83, 136 },
        { 82, 134 },
        { 81, 132 },
        { 80, 130 },
        { 79, 128 },
        { 78, 126 },
        { 77, 124 },
        { 76, 122 },
        { 75, 120 },
        { 74, 118 },
        { 73, 116 },
        { 72, 114 },
        { 71, 112 },
        { 70, 110 },
        { 69, 108 },
        { 68, 106 },
        { 67, 104 },
        { 66, 102 },
        { 65, 100 },
        { 64,  98 },
        { 63,  96 },
        { 62,  94 },
        { 61,  92 },
        { 60,  90 },
        { 59,  88 },
        { 58,  86 },
        { 57,  84 },
        { 56,  82 },
        { 55,  80 },
        { 54,  78 },
        { 53,  76 },
        { 52,  74 },
        { 51,  72 },
        { 50,  70 },
        { 49,  68 },
        { 48,  66 },
        { 47,  64 },
        { 46,  62 },
        { 45,  60 },
        { 44,  58 },
        { 43,  56 },
        { 42,  54 },
        { 41,  52 },
        { 40,  50 },
        { 39,  48 },
        { 38,  46 },
        { 37,  44 },
        { 36,  42 },
        { 35,  40 },
        { 34,  38 },
        { 33,  36 },
        { 32,  34 },
        { 31,  32 },
        { 30,  30 },
        { 29,  29 },
        { 28,  28 },
        { 27,  27 },
        { 26,  26 },
        { 25,  25 },
        { 24,  24 },
        { 23,  23 },
        { 22,  22 },
        { 21,  21 },
        { 20,  20 },
        { 19,  19 },
        { 18,  18 },
        { 17,  17 },
        { 16,  16 },
        { 15,  15 },
        { 14,  14 },
        { 13,  13 },
        { 12,  12 },
        { 11,  11 },
        { 10,  10 },
        { 9,    9 },
        { 8,    8 },
        { 7,    7 },
        { 6,    6 },
        { 5,    5 },
        { 4,    4 },
        { 3,    3 },
        { 2,    2 },
        { 1,    1 }
    };
    for (UInt i = 0; i < 99; i++) {
        if (ca >= staffCAtoLvlAry[i].second)
            return staffCAtoLvlAry[i].first;
    }
    return 1;
}

Int ConvertStaffAttribute_20_15(Int attr) {
    if (attr <= 0)
        attr = 1;
    if (attr > 20)
        attr = 20;
    static UChar fmRatingAry[20] = {
        1, // 1
        2, // 2
        3, // 3
        4, // 4
        4, // 5
        5, // 6
        5, // 7
        6, // 8
        6, // 9
        7, // 10
        8, // 11
        9, // 12
        10, // 13
        11, // 14
        12, // 15
        13, // 16
        14, // 17
        14, // 18
        15, // 19
        15, // 20
    };
    return fmRatingAry[attr];
}

Int ConvertStaffAttribute_20_100(Int attr) {
    if (attr <= 0)
        attr = 1;
    if (attr > 20)
        attr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        { 5, 29 }, // 1
        { 30, 39 }, // 2
        { 40, 49 }, // 3
        { 50, 54 }, // 4
        { 55, 59 }, // 5
        { 60, 62 }, // 6
        { 63, 64 }, // 7
        { 64, 65 }, // 8
        { 66, 67 }, // 9
        { 68, 69 }, // 10
        { 70, 71 }, // 11
        { 72, 73 }, // 12
        { 74, 76 }, // 13
        { 77, 79 }, // 14
        { 82, 83 }, // 15
        { 84, 85 }, // 16
        { 86, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    if (fmRatingAry[attr - 1].first == fmRatingAry[attr - 1].second)
        return fmRatingAry[attr - 1].first;
    return Random::Get(fmRatingAry[attr - 1].first, fmRatingAry[attr - 1].second);
}

Int ConvertStaffAttribute(Int attr) {
    return ConvertStaffAttribute_20_100(OriginalAttrValue(attr));
}

Int ConvertStaffAttribute_100_15(Int attr) {
    return ConvertStaffAttribute_20_15(OriginalAttrValue(attr));
}

FifamStaff *Converter::CreateAndConvertStaff(foom::non_player *p, FifamClub *club, FifamClubStaffPosition position) {
    if (!p->mNation) {
        Error(L"Staff without nation\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *staffCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!staffCountry) {
        Error(L"Staff without associated country\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamStaff *staff = mFifamDatabase->CreateStaff(club, mPersonIdCounter++);
    
    ConvertPersonAttributes(staff, p);
    staff->SetProperty(L"foom::non_player", p);

    staff->mLinkedCountry = staff->mNationality[0];

    // experience
    UInt age = staff->GetAge(GetCurrentDate());
    if (age >= 60)
        staff->mExperience = 4;
    else if (age >= 50)
        staff->mExperience = 3;
    else if (age >= 40)
        staff->mExperience = 2;
    else if (age >= 30)
        staff->mExperience = 1;
    else
        staff->mExperience = 0;

    // contract
    if (club) {
        if (club->mIsNationalTeam) {
            if (p->mNationContract.mDateJoined > FmEmptyDate())
                staff->mJoinedClubDate = p->mNationContract.mDateJoined;
            else
                staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
        }
        else {
            if (p->mClubContract.mDateJoined > FmEmptyDate())
                staff->mJoinedClubDate = p->mClubContract.mDateJoined;
            else
                staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
        }
        
    }
    else
        staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);

    if (staff->mJoinedClubDate.year > CURRENT_YEAR)
        staff->mJoinedClubDate.year = CURRENT_YEAR;

    staff->mClubPosition = position;

    if (staff->mClubPosition == FifamClubStaffPosition::Manager)
        staff->mPersonType = FifamPersonType::Manager;
    else if (staff->mClubPosition == FifamClubStaffPosition::ChiefExec)
        staff->mPersonType = FifamPersonType::Chairman;
    else if (staff->mClubPosition == FifamClubStaffPosition::President)
        staff->mPersonType = FifamPersonType::President;
    else
        staff->mPersonType = FifamPersonType::Staff;

    // talent
    Int potential = p->mPotentialAbility;
    if (potential == 180 && p->mOriginalPA != 180)
        potential = p->mCurrentAbility;

    //static UChar potantialAbilityRanges[9] = { 35, 55, 75, 100, 115, 130, 145, 160, 175 };
    static UChar potantialAbilityRanges[9] = { 30, 50, 70, 90, 110, 130, 140, 160, 180 };
    staff->mTalent = 0;
    for (UInt i = 0; i < 9; i++) {
        if (potential >= potantialAbilityRanges[9 - 1 - i]) {
            staff->mTalent = 9 - i;
            break;
        }
    }
    // stability of board of directors
    if (p->mPatience >= 18)
        staff->mChairmanStability = FifamChairmanStability::LongTermPlaning;
    else if (p->mPatience >= 15)
        staff->mChairmanStability = FifamChairmanStability::Patient;
    else if (p->mPatience >= 7)
        staff->mChairmanStability = FifamChairmanStability::Average;
    else if (p->mPatience >= 4)
        staff->mChairmanStability = FifamChairmanStability::Impatient;
    else
        staff->mChairmanStability = FifamChairmanStability::Insane;

    // formation
    if (p->mPreferredFormation > 0)
        staff->mManagerFavouriteFormation = ConvertFormationId(p->mPreferredFormation);
    else
        staff->mManagerFavouriteFormation = FifamFormation::None;

    // preferred orientation
    if (p->mExpectAttackingFootball && (!p->mWillLookToPlayOutOfDefence || (p->mExpectAttackingFootball / 3) > p->mWillLookToPlayOutOfDefence)) {
        if (p->mExpectAttackingFootball >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryOffensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
    }
    else if (p->mWillLookToPlayOutOfDefence && (!p->mExpectAttackingFootball || (p->mWillLookToPlayOutOfDefence / 3) > p->mExpectAttackingFootball)) {
        if (p->mWillLookToPlayOutOfDefence >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryDefensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
    }
    else
        staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Normal;

    // coaching style
    Int bestAttacking = Utils::Min(p->mAttacking, p->mExpectAttackingFootball) * 5;
    Int bestDefending = p->mWillLookToPlayOutOfDefence * 5;
    Int bestTactical = BestFrom_Avg<Int>({ p->mTacticalKnowledge, p->mWillMakeEarlyTacticalChanges * 5, p->mWillFitPlayersIntoPreferredTactic * 5 }, 1);
    Int bestYouth = BestFrom_Avg<Int>({ p->mSignsYoungPlayersForTheFirstTeam, p->mSignsALotOfYouthPlayers, p->mExpectYoungSigningsForTheFirstTeam }, 1) * 5;
    auto bestStyle = Utils::GetMaxElementId<Int, FifamManagerFocus>({
        { bestAttacking > bestDefending ? (bestAttacking - bestDefending) : 0, FifamManagerFocus::Offensive },
        { bestDefending > bestAttacking ? (bestDefending - bestAttacking) : 0, FifamManagerFocus::Defensive },
        { bestTactical, FifamManagerFocus::TacticalEducation },
        { p->mLevelOfDiscipline * 5, FifamManagerFocus::Discipline },
        { (p->mCoachingMental + p->mMotivating) / 2, FifamManagerFocus::PlayerCharacter },
        { p->mCoachingFitness * 5, FifamManagerFocus::Fitness },
        { bestYouth, FifamManagerFocus::YoungPlayers }
    });

    if (bestStyle.first > 50)
        staff->mManagerFocus = bestStyle.second;
    else {
        // UPDATE: update this if languages count was changed
        if (staff->mLanguages[3] != FifamLanguage::None)
            staff->mManagerFocus = FifamManagerFocus::Language;
        else {
            Int randVal = Random::Get(0, 100);
            if (randVal < 35)
                staff->mManagerFocus = FifamManagerFocus::None;
            else if (randVal < 45)
                staff->mManagerFocus = FifamManagerFocus::ExperiencedPlayers;
            else if (randVal < 55)
                staff->mManagerFocus = FifamManagerFocus::KeepsTeamTogether;
            else if (randVal < 65)
                staff->mManagerFocus = FifamManagerFocus::LongContracts;
            else
                staff->mManagerFocus.SetFromInt(Random::Get(1, 11));
        }
    }

    // manager skills
    UInt overallSkillPart = 0;
    if (p->mCurrentAbility >= 170)
        overallSkillPart = 8;
    else if (p->mCurrentAbility >= 160)
        overallSkillPart = 7;
    else if (p->mCurrentAbility >= 150)
        overallSkillPart = 6;
    else if (p->mCurrentAbility >= 140)
        overallSkillPart = 5;
    else if (p->mCurrentAbility >= 130)
        overallSkillPart = 4;
    else if (p->mCurrentAbility >= 120)
        overallSkillPart = 3;
    else if (p->mCurrentAbility >= 110)
        overallSkillPart = 2;
    else if (p->mCurrentAbility >= 100)
        overallSkillPart = 1;

    UChar staffLevel = (UChar)ConvertStaffAttribute(p->mCurrentAbility / 2);
    Int staffFitness = Utils::Clamp((Int)((Float)p->mCoachingFitness * (Float)p->mJobFitnessCoach / 20.0f), 1, 20) * 5;

    // manager skills
    auto motivationSkillPart = Utils::Clamp(ConvertStaffAttribute_100_15(p->mMotivating) / 2, 1, 7);
    staff->mManagerMotivationSkills = overallSkillPart + motivationSkillPart;
    
    Vector<Int> coachingSkills = {
        p->mCoachingAttacking, p->mCoachingDefending, staffFitness, p->mCoachingMental, p->mCoachingTactical, p->mCoachingTechnical
    };
    std::sort(coachingSkills.begin(), coachingSkills.end(), std::greater<Int>());
    auto coachingSkillsPart = Utils::Clamp(ConvertStaffAttribute_100_15((coachingSkills[0] + coachingSkills[1] + coachingSkills[2]) / 3) / 2, 1, 7);
    staff->mManagerCoachingSkills = overallSkillPart + coachingSkillsPart;
    
    auto gkSkillPart = Utils::GetMaxElement<Int>({ p->mCoachingGKDistribution, p->mCoachingGKHandling, p->mCoachingGKShotStopping });
    gkSkillPart = Utils::Clamp(ConvertStaffAttribute_100_15(gkSkillPart) / 2, 1, 7);
    staff->mManagerGoalkeepersTraining = overallSkillPart + gkSkillPart;
    
    auto negotiationSkillPart = Utils::GetMaxElement<Int>({ p->mBuyingPlayers, p->mBusiness });
    negotiationSkillPart = Utils::Clamp(ConvertStaffAttribute_20_15(negotiationSkillPart) / 2, 1, 7);
    staff->mManagerNegotiationSkills = overallSkillPart + negotiationSkillPart;

    // fix overall rating
    Int  overallRating = (staff->mManagerMotivationSkills + staff->mManagerCoachingSkills + staff->mManagerNegotiationSkills) / 3;
    if (overallRating > 1 && (staff->mManagerGoalkeepersTraining + 1) < overallRating) {
        staff->mManagerGoalkeepersTraining = overallRating - 1;
    }

    overallRating = staff->GetManagerLevel();
    Int desiredOverall = GetManagerLevelFromCA(p->mCurrentAbility);
    Bool dir = desiredOverall > overallRating;
    while ((dir && overallRating < desiredOverall) || (!dir && overallRating > desiredOverall)) {
        Int ratingDiff = dir ? 1 : -1;
        staff->mManagerMotivationSkills = Utils::Clamp(staff->mManagerMotivationSkills + ratingDiff, 1, 15);
        staff->mManagerCoachingSkills = Utils::Clamp(staff->mManagerCoachingSkills + ratingDiff, 1, 15);
        staff->mManagerNegotiationSkills = Utils::Clamp(staff->mManagerNegotiationSkills + ratingDiff, 1, 15);
        staff->mManagerGoalkeepersTraining = Utils::Clamp(staff->mManagerGoalkeepersTraining + ratingDiff, 1, 15);
        overallRating = staff->GetManagerLevel();
    }

    // personality attributes
    staff->mPersonalityAttributes.WillingnessToLearn = ConvertStaffAttribute_20_100(p->mAmbition);
    staff->mPersonalityAttributes.Effort = ConvertStaffAttribute(p->mDetermination);
    staff->mPersonalityAttributes.Resilence = ConvertStaffAttribute(p->mVersatility);

    staff->mSkills.Tactics = ConvertStaffAttribute(p->mTacticalKnowledge);

    staff->mSkills.FieldPlayerTraining = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mCoachingAttacking, p->mCoachingDefending, p->mCoachingMental, p->mCoachingTactical, p->mCoachingTechnical }, 2, true)
    );

    staff->mSkills.GoalkeeperTraining = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mCoachingGKDistribution, p->mCoachingGKHandling, p->mCoachingGKShotStopping }, 3, true)
    );

    staff->mSkills.FitnessTraining = Utils::Clamp((Int)((Float)ConvertStaffAttribute(staffFitness) * (Float)staffLevel * 1.1f / 100.0f), 1, 99);
    staff->mSkills.MotivationAbility = ConvertStaffAttribute(p->mMotivating);

    staff->mSkills.RegenerationAbility = ConvertStaffAttribute(p->mPhysiotherapy);

    staff->mSkills.InjuryPrevention = ConvertStaffAttribute(p->mSportsScience);
    staff->mSkills.BoneInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.KneeInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MuscleInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);

    staff->mSkills.Negotiation = ConvertStaffAttribute(p->mBuyingPlayers);
    staff->mSkills.Sponsoring = ConvertStaffAttribute(p->mBusiness);

    staff->mSkills.GeneralScouting = ConvertStaffAttribute(Utils::Max(p->mJudgingPlayerAbility, p->mJudgingTeamData));
    staff->mSkills.TalentEstimation = ConvertStaffAttribute(p->mJudgingPlayerPotential);

    staff->mSkills.FieldSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.GoalkeeperSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MentalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.PhysicalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.Networking = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mPresentingData, p->mJudgingPlayerData, p->mJudgingTeamData }, 1, true)
    );

    if (staff->mClubPosition == FifamClubStaffPosition::SportsDirector) {
        staff->mSkills.SportsLaw = Random::Get(10, 25);
        staff->mSkills.LaborLaw = Random::Get(10, 25);
        staff->mSkills.PR = Random::Get(10, 25);
        staff->mSkills.FanContact = Random::Get(10, 25);
        staff->mSkills.Arbitrate = Random::Get(10, 25);
        staff->mSkills.Marketing = Random::Get(10, 25);
        staff->mSkills.Construction = Random::Get(10, 25);
    }
    else if (staff->mClubPosition == FifamClubStaffPosition::GeneralManager) {
        staff->mSkills.SportsLaw = Random::Get(15, 30);
        staff->mSkills.LaborLaw = Random::Get(15, 30);
        staff->mSkills.PR = Random::Get(15, 30);
        staff->mSkills.FanContact = Random::Get(15, 30);
        staff->mSkills.Arbitrate = Random::Get(15, 30);
        staff->mSkills.Marketing = Random::Get(15, 30);
        staff->mSkills.Construction = Random::Get(15, 30);
    }
    else {
        staff->mSkills.SportsLaw = Random::Get(5, 20);
        staff->mSkills.LaborLaw = Random::Get(5, 20);
        staff->mSkills.PR = Random::Get(5, 20);
        staff->mSkills.FanContact = Random::Get(5, 20);
        staff->mSkills.Arbitrate = Random::Get(5, 20);
        staff->mSkills.Marketing = Random::Get(5, 20);
        staff->mSkills.Construction = Random::Get(5, 20);
    }

    staff->ForAllAttributes([=](UChar &attr, Float weight) {
        float c = 0.0f;
        if (weight >= 1.0f)
            c = 1.0f;
        else if (weight >= 0.75f) {
            if (staff->mClubPosition == FifamClubStaffPosition::TeamDoctor)
                c = 1.0f;
            else
                c = 0.95f;
        }
        else if (weight >= 0.5f) {
            if (staff->mClubPosition == FifamClubStaffPosition::TeamDoctor)
                c = 1.0f;
            else
                c = 0.9f;
        }
        else if (weight >= 0.25f)
            c = 0.8f;
        if (c > 0.0f) {
            Int newAttrValue = Utils::Clamp((Int)((Float)staffLevel * c) + 10 - Random::Get(0, 20), 1, 99);
            if (newAttrValue > (Int)attr)
                attr = (UChar)newAttrValue;
        }
    });

    Int staffCurrLevel = staff->GetStaffLevel();
    Int staffDesiredLevel = GetStaffLevelFromCA(p->mCurrentAbility);
    if (staffCurrLevel > 0 && staffCurrLevel != staffDesiredLevel) {
        Bool dir = staffDesiredLevel > staffCurrLevel;
        while ((dir && staffCurrLevel < staffDesiredLevel) || (!dir && staffCurrLevel > staffDesiredLevel)) {
            staff->ForAllAttributes([=](UChar &attr, Float weight) {
                if (weight > 0.0f) {
                    if (dir) {
                        if (attr < 99)
                            attr++;
                    }
                    else {
                        if (attr > 1)
                            attr--;
                    }
                }
            });
            staffCurrLevel = staff->GetStaffLevel();
        }
    }

    return staff;
}

Bool Converter::IsIconicPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        735216, // Cristiano Ronaldo
        7458500, // Messi
        19024412, // Neymar
        11133, // Buffon
        703963, // Pirlo
        142173, // Ibrahimovic
        7458272, // Sergio Ramos
        5124470 // Pique
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsIntrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        85081880, // Kante
        653054 // Modric
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsExtrovertPlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        19024412, // Neymar
        8832853, // Marcelo
        71000324, // Konoplyanka
        71081391, // Zinchenko
        8085570 // Dzyuba
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsInsecurePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        43001238 // Balotelli
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsFansFavouritePlayer(Int playerId) {
    static Vector<Int> playerIDs = {
        7458500, // Messi
        735216, // Cristiano Ronaldo
        19024412, // Neymar
        11133, // Buffon
        142173, // Ibrahimovic
        98028755, // Salah
        76002390, // James
        5132312, // Bale
        78000335, // Luis Suarez
        8832853, // Marcelo
        8435089, // Benzema
        7458272, // Sergio Ramos
        824041, // Iniesta
        85028014, // Pogba
        2114068, // David Luiz
        315542, // Dani Alves
        35002219, // Ozil
        156772, // Kroos
        5124470, // Pique
        67086656, // Griezmann
        14044150, // Dybala
        8833628, // Thiago Silva
        310625, // Kaka
        6700151 // Casillas
    };
    return Utils::Contains(playerIDs, playerId);
}

Bool Converter::IsSensitivePlayer(Int playerId) {
    return false;
}

UChar Converter::GetPlayerLevel(FifamPlayer * player, Bool includeExperience, UInt gameId) {
    return GetPlayerLevel(player, player->mMainPosition, player->mPlayingStyle, includeExperience, gameId);
}

UChar Converter::GetPlayerLevel(FifamPlayer * player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool includeExperience, UInt gameId) {
    if (gameId >= 13)
        return FifamPlayerLevel::GetPlayerLevel13(player, position, style, includeExperience);
    return FifamPlayerLevel::GetPlayerLevel12(player, position, style, includeExperience);
}

Bool Converter::IsPlayerRetiredFromNationalTeam(Int playerId) {
    return
        playerId == 8435089 || // Benzema
        playerId == 8169332 || // Nainggolan
        playerId == 11133 || // Buffon
        playerId == 8053234; // Akinfeev
}

FifamClubLink Converter::GetCompWinner(foom::comp *c, Bool checkPreviousSeason) {
    Int latestYear = 0;
    FifamClubLink result;
    for (auto &h : c->mVecHistory) {
        if (h.mYear <= CURRENT_YEAR && h.mYear > latestYear && h.mFirstPlaced) {
            latestYear = h.mYear;
            result.Clear();
            if (!h.mFirstPlaced->mIsNation)
                result = GetTeamClubLink(h.mFirstPlaced);
        }
    }
    Int minYearToCheck = CURRENT_YEAR;
    if (checkPreviousSeason)
        minYearToCheck -= 1;
    if (latestYear >= minYearToCheck)
        return result;
    return FifamClubLink();
}

void Converter::GetCompWinnerAndRunnerUp(foom::comp * c, FifamClubLink & outWinner, FifamClubLink & outRunnerUp, Bool checkPreviousSeason) {
    Int latestYear = 0;
    foom::comp::history *latestHistory = nullptr;
    for (auto &h : c->mVecHistory) {
        if (h.mYear <= CURRENT_YEAR && h.mYear > latestYear && (h.mFirstPlaced || h.mSecondPlaced)) {
            latestYear = h.mYear;
            latestHistory = &h;
        }
    }
    outWinner.Clear();
    outRunnerUp.Clear();
    Int minYearToCheck = CURRENT_YEAR;
    if (checkPreviousSeason)
        minYearToCheck -= 1;
    if (latestHistory && latestYear >= minYearToCheck) {
        if (latestHistory->mFirstPlaced && !latestHistory->mFirstPlaced->mIsNation)
            outWinner = GetTeamClubLink(latestHistory->mFirstPlaced);
        if (latestHistory->mSecondPlaced && !latestHistory->mSecondPlaced->mIsNation)
            outRunnerUp = GetTeamClubLink(latestHistory->mSecondPlaced);
    }
}

FifamClubLink Converter::GetCompHost(foom::comp *c, Bool checkPreviousSeason) {
    Int latestYear = 0;
    FifamClubLink result;
    for (auto &h : c->mVecHistory) {
        if (h.mYear <= (CURRENT_YEAR + 1) && h.mYear > latestYear && !h.mFirstPlaced) {
            latestYear = h.mYear;
            result.Clear();
            if (h.mHostStadium)
                result = GetTeamClubLink(h.mHostStadium->mOwner);
        }
    }
    Int minYearToCheck = CURRENT_YEAR + 1;
    if (checkPreviousSeason)
        minYearToCheck -= 1;
    if (latestYear >= minYearToCheck)
        return result;
    return FifamClubLink();
}

FifamClubLink Converter::GetTeamClubLink(foom::team * t, Bool allowReserveTeam) {
    if (t) {
        if (t->mIsNation) {
            FifamCountry *country = mFifamDatabase->GetCountry(((foom::nation *)t)->mConverterData.mFIFAManagerID);
            if (country)
                return FifamClubLink(&country->mNationalTeam);
        }
        else {
            foom::club *foomClub = (foom::club *)t;
            if (foomClub->mConverterData.mFifamClub)
                return FifamClubLink((FifamClub *)foomClub->mConverterData.mFifamClub);
            else if (allowReserveTeam && foomClub->mConverterData.mParentClub && foomClub->mConverterData.mParentClub->mConverterData.mFifamClub)
                return FifamClubLink((FifamClub *)foomClub->mConverterData.mParentClub->mConverterData.mFifamClub, FifamClubTeamType::Reserve);
        }
    }
    return FifamClubLink();
}

Map<UShort, foom::team*> Converter::GetWinnersList(Vector<foom::comp*> const & inputComps, bool isSupercup) {
    Map<UShort, foom::team *> result;
    if (inputComps.empty())
        return result;
    auto comps = inputComps;
    std::sort(comps.begin(), comps.end(), [](foom::comp *a, foom::comp *b) {
        if (b->mIsExtinct && !a->mIsExtinct) return true;
        if (a->mIsExtinct && !b->mIsExtinct) return false;
        Int levelA = a->mCompetitionLevel;
        if (levelA == 0)
            levelA = 255;
        Int levelB = b->mCompetitionLevel;
        if (levelB == 0)
            levelB = 255;
        if (levelA < levelB) return true;
        if (levelB < levelA) return false;
        if (a->mReputation > b->mReputation) return true;
        if (b->mReputation > a->mReputation) return false;
        if (a->mName.find(L"Clausura") != String::npos && b->mName.find(L"Apertura") != String::npos) return true;
        if (b->mName.find(L"Clausura") != String::npos && a->mName.find(L"Apertura") != String::npos) return false;
        return false;
    });
    for (auto i = comps.rbegin(); i != comps.rend(); i++) {
        if (!(*i)->mVecHistory.empty()) {
            Vector<foom::comp::history> h;
            std::copy_if((*i)->mVecHistory.begin(), (*i)->mVecHistory.end(), std::back_inserter(h), [](foom::comp::history const &a) {
                return a.mYear > 0 && a.mYear <= CURRENT_YEAR && a.mFirstPlaced;
            });
            if (!h.empty()) {
                std::sort(h.begin(), h.end(), [](foom::comp::history const &a, foom::comp::history const &b) {
                    if (a.mYear < b.mYear) return true;
                    if (b.mYear < a.mYear) return false;
                    if (a.mYearOrder < b.mYearOrder) return true;
                    if (b.mYearOrder < a.mYearOrder) return false;
                    return false;
                });
                bool yearsShift = 0;
                if (!(*i)->mIsExtinct && !isSupercup && h.back().mYear == (CURRENT_YEAR - 1))
                    yearsShift = 1;
                for (auto &e : h)
                    result[e.mYear + yearsShift] = e.mFirstPlaced;
            }
        }
    }
    return result;
}

FifamFormation Converter::ConvertFormationId(Int id) {
    switch (id) {
    case 2:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
        return FifamFormation::_5_3_2_Attacking;
    case 3:
        return FifamFormation::_4_4_2_Normal;
    case 32:
        return FifamFormation::_4_1_4_1;
    case 7:
    case 20:
    case 52:
        return FifamFormation::_4_4_2_Diamond;
    case 8:
        return FifamFormation::_4_5_1_Normal;
    case 12:
        return FifamFormation::_4_3_3_Normal;
    case 14:
        return FifamFormation::_4_4_2_Diamond_Attacking;
    case 15:
        return FifamFormation::_5_4_1;
    case 16:
    case 19:
    case 33:
        return FifamFormation::_4_4_1_1_Defensive;
    case 18:
    case 21:
    case 23:
        return FifamFormation::_4_3_3_Attacking;
    case 22:
    case 24:
        return FifamFormation::_4_4_2_Attacking;
    case 29:
    case 54:
    case 55:
        return FifamFormation::_4_2_4;
    case 25:
    case 34:
    case 60:
        return FifamFormation::_3_5_2;
    case 4:
    case 26:
    case 27:
        return FifamFormation::_4_3_3_Wings;
    case 28:
        return FifamFormation::_4_2_2_1_1_V2;
    case 30:
        return FifamFormation::_5_2_2_1;
    case 31:
    case 35:
        return FifamFormation::_3_4_3_Defensive;
    case 36:
    case 40:
        return FifamFormation::_4_3_3_Defensive;
    case 41:
    case 56:
        return FifamFormation::_5_2_1_2;
    case 42:
    case 59:
    case 61:
        return FifamFormation::_5_2_2_1;
    case 47:
        return FifamFormation::_5_3_2;
    }
    return FifamFormation::None;
}

Bool Converter::GenerateCalendar(FifamNation const & countryId, FifamDatabase * database, Vector<FifamCompLeague*> const & leagues, Vector<FifamCompCup*> const & cups)
{

    if (leagues.empty() && cups.empty())
        return false;

    Array<Array<UInt, 366>, 2> calendar = {};

    FifamCountry *country = database->GetCountry(countryId.ToInt());

    // is valid country
    if (!country) {
        Error(L"Calendar generator: unknown country (ID %d)", countryId.ToInt());
        return false;
    }

    Bool veryDifficultCalendar = country->mId == FifamNation::England;

    auto PutOneMatch = [](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId) {
        if (matchdayId >= 1 && matchdayId <= 365) {
            if (c[season][matchdayId] < 1002)
                c[season][matchdayId] = 1002;
            if (matchdayId >= 2) {
                if (c[season][matchdayId - 1] < 1001)
                    c[season][matchdayId - 1] = 1001;
            }
            if (matchdayId <= 364) {
                if (c[season][matchdayId + 1] < 1001)
                    c[season][matchdayId + 1] = 1001;
            }
        }
    };

    auto MarkPossibleMatchday = [&](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId, UInt &index) {
        if (matchdayId >= 1 && matchdayId <= 365 && c[season][matchdayId] == 0) {
            PutOneMatch(c, season, matchdayId);
            c[season][matchdayId] = ++index;
        }
    };

    auto MarkPossibleWeekEndMatchday = [&](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId, Bool bSunday, UInt &index) {
        if (matchdayId >= 1) {
            if (bSunday)
                matchdayId++;
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0 && (!bSunday || c[season][matchdayId - 1] == 0)) {
                    PutOneMatch(c, season, matchdayId);
                    c[season][matchdayId] = ++index;
                }
            }
            /// additional matches movement
            //else if (bSunday) {
            //    if (c[season][matchdayId - 1] == 0) {
            //        PutOneMatch(c, season, matchdayId - 1);
            //        c[season][matchdayId - 1] = ++index;
            //    }
            //}
            //else {
            //    if (matchdayId <= 364 && c[season][matchdayId + 1] == 0) {
            //        PutOneMatch(c, season, matchdayId + 1);
            //        c[season][matchdayId + 1] = ++index;
            //    }
            //}
        }
    };

    auto MarkPossibleWeekEndMatchdayForCup = [&](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId, UInt &index) {
        if (matchdayId >= 1) {
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId);
                    c[season][matchdayId] = ++index;
                }
                else {
                    if (matchdayId <= 364 && c[season][matchdayId + 1] == 0) {
                        PutOneMatch(c, season, matchdayId + 1);
                        c[season][matchdayId + 1] = ++index;
                    }
                }
            }
        }
    };

    auto MarkPossibleMidWeekMatchday = [&](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId, Bool bSunday, UInt &index) {
        matchdayId += 3;
        if (matchdayId >= 1) {
            if (bSunday)
                matchdayId++;
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId);
                    c[season][matchdayId] = ++index;
                }
            }
        }
    };

    auto MarkPossibleMidWeekMatchdayForCup = [&](Array<Array<UInt, 366>, 2> &c, Bool season, UShort matchdayId, UInt &index) {
        if (veryDifficultCalendar) {
            MarkPossibleMatchday(c, season, matchdayId + 3, index);
            MarkPossibleMatchday(c, season, matchdayId + 5, index);
        }
        matchdayId += 4;
        if (matchdayId >= 1) {
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId);
                    c[season][matchdayId] = ++index;
                }
                else {
                    if (matchdayId <= 364 && c[season][matchdayId + 1] == 0) {
                        PutOneMatch(c, season, matchdayId + 1);
                        c[season][matchdayId + 1] = ++index;
                    }
                }
            }
        }
    };

    auto PutCompToCalendar = [&](Array<Array<UInt, 366>, 2> &c, FifamCompetition *comp) {
        bool isInternational = comp->mID.mType == FifamCompType::WorldCup ||
            comp->mID.mType == FifamCompType::EuroCup ||
            comp->mID.mType == FifamCompType::QualiEC ||
            comp->mID.mType == FifamCompType::QualiWC ||
            comp->mID.mType == FifamCompType::CopaAmerica ||
            comp->mID.mType == FifamCompType::ConfedCup;
        bool firstSeason = comp->TakesPlaceInSeason(CURRENT_YEAR) || comp->TakesPlaceInSeason(CURRENT_YEAR + 2);
        bool secondSeason = comp->TakesPlaceInSeason(CURRENT_YEAR - 1) || comp->TakesPlaceInSeason(CURRENT_YEAR + 1);
        if (comp->GetDbType() == FifamCompDbType::League) {
            if (firstSeason) {
                for (auto &m : comp->AsLeague()->mFirstSeasonMatchdays) {
                    if (m <= 365)
                        PutOneMatch(c, 0, m);
                    else
                        PutOneMatch(c, 1, m - 365);
                }
            }
            if (secondSeason) {
                if (isInternational) {
                    for (auto &m : comp->AsLeague()->mFirstSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m);
                        else
                            PutOneMatch(c, 0, m - 365);
                    }
                }
                else {
                    for (auto &m : comp->AsLeague()->mSecondSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m);
                        else
                            PutOneMatch(c, 0, m - 365);
                    }
                }
            }
        }
        else if (comp->GetDbType() == FifamCompDbType::Round) {
            if (firstSeason) {
                for (auto &m : comp->AsRound()->mFirstSeasonMatchdays)
                    PutOneMatch(c, 0, m);
            }
            if (secondSeason) {
                if (isInternational) {
                    for (auto &m : comp->AsRound()->mFirstSeasonMatchdays)
                        PutOneMatch(c, 1, m);
                }
                else {
                    for (auto &m : comp->AsRound()->mSecondSeasonMatchdays)
                        PutOneMatch(c, 1, m);
                }
            }
        }
        else if (comp->GetDbType() == FifamCompDbType::Cup) {
            for (auto &m : comp->AsCup()->mFirstSeasonMatchdays)
                PutOneMatch(c, 0, m);
            for (auto &m : comp->AsCup()->mSecondSeasonMatchdays)
                PutOneMatch(c, 1, m);
        }
    };

    Int winterBreakStart = 0, winterBreakEnd = 0;

    if (!leagues.empty()) {
        winterBreakStart = leagues[0]->GetProperty<Int>(L"winterBreakStart", 0);
        winterBreakEnd = leagues[0]->GetProperty<Int>(L"winterBreakEnd", 0);
    }

    // winter break
    if (winterBreakStart > 0 && winterBreakEnd > 0 && winterBreakStart < winterBreakEnd) {
        for (UInt s = 0; s < 2; s++) {
            for (Int m = winterBreakStart; m <= winterBreakEnd; m++) {
                Int mid = m;
                if (s == 1)
                    mid -= 7;
                if (calendar[s][mid] < 1000)
                    calendar[s][mid] = 1000;
            }
        }
    }

    // generate calendar for leagues

    Array<UInt, 2> matchdayIndex = { 0, 0 };

    for (FifamCompLeague *l : leagues) {

        auto cc = calendar;

        if (l->mLeagueLevel == 0) {
            // put international matches to calendar (for highest league level only)
            for (auto &m : database->mRules.mInternationalFriendliesFirstSeason)
                PutOneMatch(cc, 0, m);
            for (auto &m : database->mRules.mInternationalFriendliesSecondSeason)
                PutOneMatch(cc, 1, m);
        }

        for (auto[compID, comp] : database->mCompMap) {
            if (compID.mRegion == FifamCompRegion::International || compID.mType == FifamCompType::WorldClubChamp ||
                (country->mContinent.ToInt() <= 5 && compID.mRegion.ToInt() == (249 + country->mContinent.ToInt())))
            {
                if (compID.mType != FifamCompType::U20WorldCup) {
                    if (l->mLeagueLevel == 0 || (compID.mType != FifamCompType::WorldCup && compID.mType != FifamCompType::EuroCup &&
                        compID.mType != FifamCompType::QualiWC && compID.mType != FifamCompType::QualiEC &&
                        compID.mType != FifamCompType::CopaAmerica && compID.mType != FifamCompType::ConfedCup))
                    {
                        PutCompToCalendar(cc, comp);
                    }
                }
            }
        }

        // play matches in Sunday or Saturday?
        Bool bSunday = l->mLeagueLevel == 0; // play matches for first league on Sunday

        Int startMatchday1 = 48; // End of August
        Int startMatchday2 = 27; // End of July
        Int startMatchday3 = 13; // Middle of July
        Int endMatchday = 315; // May

        Int startDate = l->GetProperty<Int>(L"startDate", 0);
        Int endDate = l->GetProperty<Int>(L"endDate", 0);

        if ((endDate > 0 && endDate < startDate) || startDate > 365 || endDate > 365) {
            Error(L"Incorrect startDate/endDate in league\nLeague: %s\nstartDate: %d\nendDate: %d", FifamTr(l->mName).c_str(), startDate, endDate);
            startDate = 0;
            endDate = 0;
        }

        Bool customStartDate = startDate > 0;

        if (customStartDate) {
            Int dayOfWeek = startDate % 7;
            if (dayOfWeek != 6) { // Saturday
                if (dayOfWeek == 0) // Sunday
                    startDate -= 1;
                else if (dayOfWeek == 1) // Monday
                    startDate -= 2;
                else if (dayOfWeek == 2) // Tuesday
                    startDate -= 3;
                else if (dayOfWeek == 5) // Friday
                    startDate += 1;
                else if (dayOfWeek == 4) // Thursday
                    startDate += 2;
                else if (dayOfWeek == 3) // Wednesday
                    startDate += 3;
            }
            startMatchday1 = startDate;
            startMatchday2 = startDate;
            startMatchday3 = startDate;
        }
        if (endDate > 0) {
            if ((!customStartDate || endDate > startDate) && (!l->mLeagueLevel || endDate < endMatchday))
                endMatchday = endDate;
        }

        matchdayIndex = { 0, 0 };

        for (UInt s = 0; s < 2; s++) {
            // phase 1 - add possible match every 2 weeks
            for (Int m = startMatchday1; m <= endMatchday; m += 14)
                MarkPossibleWeekEndMatchday(cc, s, m, bSunday, matchdayIndex[s]);

            // phase 2 - add possible match every week
            for (Int m = startMatchday1; m <= endMatchday; m += 7)
                MarkPossibleWeekEndMatchday(cc, s, m, bSunday, matchdayIndex[s]);

            // phase 3 - add possible matches in August (reverse direction)
            if (!customStartDate) {
                for (Int m = startMatchday1 - 7; m >= startMatchday2; m -= 7)
                    MarkPossibleWeekEndMatchday(cc, s, m, bSunday, matchdayIndex[s]);
            }

            // phase 4 - add possible mid-week matches every 2 weeks
            for (Int m = startMatchday2 + 14; m <= endMatchday; m += 14)
                MarkPossibleMidWeekMatchday(cc, s, m, bSunday, matchdayIndex[s]);

            // phase 5 - add possible matches in July (reverse direction)
            if (!customStartDate) {
                for (Int m = startMatchday2 - 7; m >= startMatchday3; m -= 7)
                    MarkPossibleWeekEndMatchday(cc, s, m, bSunday, matchdayIndex[s]);
            }

            // phase 6 - add possible mid-week matches every week
            for (Int m = startMatchday1; m <= endMatchday; m += 7)
                MarkPossibleMidWeekMatchday(cc, s, m, bSunday, matchdayIndex[s]);

            // phase 7 - add possible mid-week matches every week in July and August (reverse direction)
            if (!customStartDate) {
                for (Int m = startMatchday1 - 7; m >= startMatchday3; m -= 7)
                    MarkPossibleMidWeekMatchday(cc, s, m, bSunday, matchdayIndex[s]);
            }
        }

        for (UInt s = 0; s < 2; s++) {
            Vector<Pair<UInt, UInt>> availableMatchdays;
            for (UInt i = 1; i < cc[s].size(); i++) {
                if (cc[s][i] > 0 && cc[s][i] < 1000)
                    availableMatchdays.emplace_back(cc[s][i], i);
            }
            UInt requiredMatchdays = l->GetNumMatchdays();
            if (availableMatchdays.size() < requiredMatchdays) {
                Error(L"Not enough available matches in calendar for league\nLeague: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
                    FifamTr(l->mName).c_str(), s + 1, requiredMatchdays, availableMatchdays.size());
            }
            else {
                std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                    return a.first < b.first;
                });
                if (s == 0) {
                    l->mFirstSeasonMatchdays.resize(requiredMatchdays);
                    for (UInt i = 0; i < requiredMatchdays; i++)
                        l->mFirstSeasonMatchdays[i] = availableMatchdays[i].second;
                    std::sort(l->mFirstSeasonMatchdays.begin(), l->mFirstSeasonMatchdays.end());
                }
                else {
                    l->mSecondSeasonMatchdays.resize(requiredMatchdays);
                    for (UInt i = 0; i < requiredMatchdays; i++)
                        l->mSecondSeasonMatchdays[i] = availableMatchdays[i].second;
                    std::sort(l->mSecondSeasonMatchdays.begin(), l->mSecondSeasonMatchdays.end());
                }
            }
        }
    }

    // generate calendar for cups

    // add international and continental matches to calendar

    for (auto &m : database->mRules.mInternationalFriendliesFirstSeason)
        PutOneMatch(calendar, 0, m);
    for (auto &m : database->mRules.mInternationalFriendliesSecondSeason)
        PutOneMatch(calendar, 1, m);

    for (auto[compID, comp] : database->mCompMap) {
        if (compID.mRegion == FifamCompRegion::International || compID.mType == FifamCompType::WorldClubChamp ||
            (country->mContinent.ToInt() <= 5 && compID.mRegion.ToInt() == (249 + country->mContinent.ToInt())))
        {
            if (compID.mType != FifamCompType::U20WorldCup)
                PutCompToCalendar(calendar, comp);
        }
    }

    // add league matches to calendar

    Array<Int, 2> lastLeagueMatchday = { 0, 0 };

    for (FifamCompLeague *l : leagues) {
        PutCompToCalendar(calendar, l);
        if (!l->mFirstSeasonMatchdays.empty()) {
            if (l->mFirstSeasonMatchdays.back() > lastLeagueMatchday[0])
                lastLeagueMatchday[0] = l->mFirstSeasonMatchdays.back();
        }
        if (!l->mSecondSeasonMatchdays.empty()) {
            if (l->mSecondSeasonMatchdays.back() > lastLeagueMatchday[1])
                lastLeagueMatchday[1] = l->mSecondSeasonMatchdays.back();
        }
    }

    // collect available matchdays for cups

    Int cupsStartMatchday1 = 20;
    Int cupsStartMatchday2 = 13;
    Int cupsEndMatchday2 = 321;

    matchdayIndex = { 0, 0 };

    UInt numFaCups = 0;
    UInt numLeCups = 0;
    UInt numSuperCups = 0;
    FifamCompCup *superCup = nullptr;
    UInt numMatchesForLeAndFaCups = 0;

    for (FifamCompCup *c : cups) {
        if (c->mID.mType == FifamCompType::SuperCup) {
            if (!superCup)
                superCup = c;
            numSuperCups++;
        }
        else if (c->mID.mType == FifamCompType::FaCup)
            numFaCups++;
        else if (c->mID.mType == FifamCompType::LeagueCup)
            numLeCups++;

        if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
            for (auto &r : c->mRounds) {
                if (r.mFlags.Check(FifamBeg::_2ndLeg) || r.mFlags.Check(FifamBeg::WithReplay))
                    numMatchesForLeAndFaCups += 2;
                else
                    numMatchesForLeAndFaCups += 1;
            }
        }
    }

    UInt numFaAndLeCups = numFaCups + numLeCups;

    auto cupcc = calendar;

    if (numFaAndLeCups > 0) {

        for (UInt s = 0; s < 2; s++) {

            Int cupsEndMatchday1 = cupsEndMatchday2;
            if (lastLeagueMatchday[s] > 0 && (lastLeagueMatchday[s] + 7) < cupsEndMatchday2)
                cupsEndMatchday1 = lastLeagueMatchday[s] + 7;

            MarkPossibleWeekEndMatchdayForCup(cupcc, s, cupsEndMatchday1, matchdayIndex[s]);

            if (matchdayIndex[s] == 0 || numFaAndLeCups > 1)
                MarkPossibleMidWeekMatchdayForCup(cupcc, s, cupsEndMatchday1 - 7, matchdayIndex[s]);

            if (numFaAndLeCups > 1) {
                if (matchdayIndex[s] <= 1)
                    MarkPossibleWeekEndMatchdayForCup(cupcc, s, cupsEndMatchday1 - 7, matchdayIndex[s]);
                if (matchdayIndex[s] <= 1)
                    MarkPossibleMidWeekMatchdayForCup(cupcc, s, cupsEndMatchday1 - 14, matchdayIndex[s]);
            }

            // phase 1 - add possible match every 2 weeks (reverse direction)
            for (Int m = cupsEndMatchday1 - 7; m >= cupsStartMatchday1; m -= 14)
                MarkPossibleWeekEndMatchdayForCup(cupcc, s, m, matchdayIndex[s]);
            // phase 2 - add possible match every week (reverse direction)
            for (Int m = cupsEndMatchday1 - 7; m >= cupsStartMatchday1; m -= 7)
                MarkPossibleWeekEndMatchdayForCup(cupcc, s, m, matchdayIndex[s]);

            // phase 3 - add possible mid-week matches every 2 weeks (reverse direction)
            for (Int m = cupsEndMatchday1 - 7; m >= cupsStartMatchday1; m -= 14)
                MarkPossibleMidWeekMatchdayForCup(cupcc, s, m, matchdayIndex[s]);
            // phase 4 - add possible mid-week matches every week
            for (Int m = cupsEndMatchday1 - 7; m >= cupsStartMatchday1; m -= 7)
                MarkPossibleMidWeekMatchdayForCup(cupcc, s, m, matchdayIndex[s]);

            // phase 6 - add possible matches in June (reverse direction)
            for (Int m = cupsStartMatchday1 - 7; m >= cupsStartMatchday2; m -= 7)
                MarkPossibleWeekEndMatchdayForCup(cupcc, s, m, matchdayIndex[s]);
            // phase 7 - add possible mid-week matches in June (reverse direction)
            for (Int m = cupsStartMatchday1 - 7; m >= cupsStartMatchday2; m -= 7)
                MarkPossibleMidWeekMatchdayForCup(cupcc, s, m, matchdayIndex[s]);

            if (cupsEndMatchday1 != cupsEndMatchday2) {
                // phase 8 - add possible matches in May
                for (Int m = cupsEndMatchday1; m < cupsEndMatchday2; m += 7)
                    MarkPossibleWeekEndMatchdayForCup(cupcc, s, m, matchdayIndex[s]);
                // phase 9 - add possible mid-week matches in May
                for (Int m = cupsEndMatchday1; m < cupsEndMatchday2; m += 7)
                    MarkPossibleMidWeekMatchdayForCup(cupcc, s, m, matchdayIndex[s]);
            }

            if (matchdayIndex[s] < numMatchesForLeAndFaCups) {

                Message(Utils::Format(L"Too difficult calendar in %s (season %d)", FifamTr(country->mName).c_str(), s + 1));

                // phase 10 - add matches when possible
                for (Int m = cupsStartMatchday2; m < cupsEndMatchday2; m += 3)
                    MarkPossibleMatchday(cupcc, s, m, matchdayIndex[s]);
                for (Int m = cupsStartMatchday2; m < cupsEndMatchday2; m++)
                    MarkPossibleMatchday(cupcc, s, m, matchdayIndex[s]);
            }
        }

        for (UInt s = 0; s < 2; s++) {
            Vector<Pair<UInt, UInt>> availableMatchdays;
            for (UInt i = 1; i < cupcc[s].size(); i++) {
                if (cupcc[s][i] > 0 && cupcc[s][i] < 1000)
                    availableMatchdays.emplace_back(cupcc[s][i], i);
            }
            if (availableMatchdays.size() < numMatchesForLeAndFaCups) {
                Error(L"Not enough available matches in calendar for cups\nCountry: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
                    FifamTr(country->mName).c_str(), s + 1, numMatchesForLeAndFaCups, availableMatchdays.size());
            }
            else {
                // sort by priority
                std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                    return a.first < b.first;
                });
                availableMatchdays.resize(numMatchesForLeAndFaCups);
                // sort by dates
                std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                    return a.second > b.second;
                });
                // clear matchdays
                for (FifamCompCup *c : cups) {
                    if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
                        if (s == 0)
                            c->mFirstSeasonMatchdays.clear();
                        else
                            c->mSecondSeasonMatchdays.clear();
                    }
                }
                // fill matchdays
                UInt nextMatchdayIndex = 0;
                for (UInt r = 0; r < 8; r++) {
                    for (UInt m = 0; m < 2; m++) {
                        for (FifamCompCup *c : cups) {
                            if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
                                UInt numRounds = c->mRounds.size();
                                if (numRounds > r) {
                                    if (m == 0 || (c->mRounds[numRounds - 1 - r].mFlags.Check(FifamBeg::_2ndLeg) ||
                                        c->mRounds[numRounds - 1 - r].mFlags.Check(FifamBeg::WithReplay)))
                                    {
                                        if (s == 0)
                                            c->mFirstSeasonMatchdays.push_back(availableMatchdays[nextMatchdayIndex++].second);
                                        else
                                            c->mSecondSeasonMatchdays.push_back(availableMatchdays[nextMatchdayIndex++].second);
                                    }
                                }
                            }
                        }
                    }
                }
                // sort matchdays
                for (FifamCompCup *c : cups) {
                    if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
                        if (s == 0)
                            std::sort(c->mFirstSeasonMatchdays.begin(), c->mFirstSeasonMatchdays.end());
                        else
                            std::sort(c->mSecondSeasonMatchdays.begin(), c->mSecondSeasonMatchdays.end());
                    }
                }
            }
        }
    }

    // add FA/League cup matches to calendar
    for (FifamCompCup *c : cups) {
        if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup)
            PutCompToCalendar(calendar, c);
    }

    // generate matchdays for supercup

    if (superCup) {
        if (numSuperCups > 1)
            Error(L"More than 1 supercup in %s", FifamTr(country->mName).c_str());
        else {
            if (superCup->mRounds.empty())
                Error(L"Supercup has no rounds\nCountry : %s", FifamTr(country->mName).c_str());
            else if (superCup->mRounds.size() > 1)
                Error(L"Supercups with more than 1 round are not supported\nCountry : %s", FifamTr(country->mName).c_str());
            else {

                auto supercupcc = calendar;

                matchdayIndex = { 0, 0 };

                for (UInt s = 0; s < 2; s++) {
                    // find first matchday in country
                    Int firstMatchday = 0;
                    for (FifamCompLeague *l : leagues) {
                        if (s == 0) {
                            if (!l->mFirstSeasonMatchdays.empty() && l->mFirstSeasonMatchdays[0] < firstMatchday)
                                firstMatchday = l->mFirstSeasonMatchdays[0];
                        }
                        else {
                            if (!l->mSecondSeasonMatchdays.empty() && l->mSecondSeasonMatchdays[0] < firstMatchday)
                                firstMatchday = l->mSecondSeasonMatchdays[0];
                        }
                    }
                    for (FifamCompCup *c : cups) {
                        if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
                            if (s == 0) {
                                if (!c->mFirstSeasonMatchdays.empty() && c->mFirstSeasonMatchdays[0] < firstMatchday)
                                    firstMatchday = c->mFirstSeasonMatchdays[0];
                            }
                            else {
                                if (!c->mSecondSeasonMatchdays.empty() && c->mSecondSeasonMatchdays[0] < firstMatchday)
                                    firstMatchday = c->mSecondSeasonMatchdays[0];
                            }
                        }
                    }
                    Int supercupMax = 13;
                    Int supercupMin = 6;

                    bool cantCreate = false;

                    if (firstMatchday > 0) {
                        if (firstMatchday <= supercupMin) {
                            Error(L"No available matchdays for supercup in %s (season %d)", FifamTr(country->mName).c_str(), s + 1);
                            cantCreate = true;
                        }
                        else
                            supercupMax = firstMatchday - (firstMatchday % 7) - 1;
                    }
                    if (!cantCreate) {
                        // phase 1 - add possible match every week (reverse direction)
                        for (Int m = supercupMax; m >= supercupMin; m -= 7)
                            MarkPossibleWeekEndMatchdayForCup(supercupcc, s, m, matchdayIndex[s]);

                        // phase 2 - add possible mid-week matches every week (reverse direction)
                        for (Int m = supercupMax; m >= supercupMin; m -= 7)
                            MarkPossibleMidWeekMatchdayForCup(supercupcc, s, m, matchdayIndex[s]);

                        UInt numMatches = (superCup->mRounds[0].mFlags.Check(FifamBeg::_2ndLeg) || superCup->mRounds[0].mFlags.Check(FifamBeg::WithReplay)) ? 2 : 1;

                        Vector<Pair<UInt, UInt>> availableMatchdays;
                        for (UInt i = 1; i < supercupcc[s].size(); i++) {
                            if (supercupcc[s][i] > 0 && supercupcc[s][i] < 1000)
                                availableMatchdays.emplace_back(supercupcc[s][i], i);
                        }
                        if (availableMatchdays.size() < numMatches) {
                            Error(L"Not enough available matches in calendar for SuperCup\nCountry: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
                                FifamTr(country->mName).c_str(), s + 1, numMatches, availableMatchdays.size());
                        }
                        else {
                            // sort by priority
                            std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                return a.first < b.first;
                            });
                            if (s == 0) {
                                superCup->mFirstSeasonMatchdays.clear();
                                for (UInt m = 0; m < numMatches; m++)
                                    superCup->mFirstSeasonMatchdays.push_back(availableMatchdays[m].second);
                                std::sort(superCup->mFirstSeasonMatchdays.begin(), superCup->mFirstSeasonMatchdays.end());
                            }
                            else {
                                superCup->mSecondSeasonMatchdays.clear();
                                for (UInt m = 0; m < numMatches; m++)
                                    superCup->mSecondSeasonMatchdays.push_back(availableMatchdays[m].second);
                                std::sort(superCup->mSecondSeasonMatchdays.begin(), superCup->mSecondSeasonMatchdays.end());
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

void Converter::ConvertReferee(FifamReferee *dst, foom::official *official) {
    official->mConverterData.mFifamReferee = dst;
    dst->SetProperty(L"foom::official", official);
    dst->mFirstName = FifamNames::LimitPersonName(official->mFirstName, 19);
    dst->mLastName = FifamNames::LimitPersonName(official->mSecondName, 19);
    FixPersonName(dst->mFirstName);
    FixPersonName(dst->mLastName);
    if (official->mFIFACategory && official->mContinentalOfficial && official->mCurrentAbility > 140)
        dst->mType = FifamRefereeType::WorldClassReferee;
    else {
        dst->mType = FifamRefereeType::None;
        Vector<Pair<Int, FifamRefereeType>> elements;
        if (official->mPressure != 0) {
            if (official->mContinentalOfficial)
                elements.emplace_back(20 - official->mPressure, FifamRefereeType::Coward);
            elements.emplace_back(official->mPressure, FifamRefereeType::BraveDecisionMaker);
        }
        if (official->mDiscipline != 0)
            elements.emplace_back(official->mDiscipline, FifamRefereeType::SeverePerson);
        if (official->mAllowingFlow != 0)
            elements.emplace_back(official->mAllowingFlow, FifamRefereeType::DoesNotInterfereMuchInTheGame);
        if (!elements.empty()) {
            if (elements.size() == 1) {
                if (elements[0].first >= 15)
                    dst->mType = elements[0].second;
            }
            else {
                std::sort(elements.begin(), elements.end(), [](Pair<Int, FifamRefereeType> const &a, Pair<Int, FifamRefereeType> const &b) {
                    return a.first < b.first;
                });
                Int diff = elements[1].first - elements[0].first;
                if (diff >= 3)
                    dst->mType = elements[0].second;
            }
        }
    }
}
