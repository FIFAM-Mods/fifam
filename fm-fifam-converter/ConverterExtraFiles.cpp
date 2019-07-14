#include "Converter.h"
#include "FifaConverter.h"

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
                        OptionalInt(d.mReserveTeamsAllowed), sorting, d.mAttendanceMp, d.mTransfersMp, d.mTvBonus, d.mWinBouns, d.mPlaceBonus,
                        d.mOneYearCalendar);

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
                    reader.ReadLineWithSeparator(L'\t', strDummy, c.mNationID, c.mName, c.mShortName, c.mID, c.mLevel, strType, strDummy, c.mReputation, c.mPriority, strTemplateType, c.mRoundDesc[7], c.mRoundDesc[6], c.mRoundDesc[5], c.mRoundDesc[4], c.mRoundDesc[3], c.mRoundDesc[2], c.mRoundDesc[1], c.mRoundDesc[0], c.mStructure, c.mMaxLevel, c.mNumSubs, c.mBonus, c.mTvBonus, c.mStartDate, c.mEndDate, c.mOneYearCalendar);
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
        std::wcout << L"Reading league tables..." << std::endl;
        for (auto const &p : recursive_directory_iterator(infoPath / L"leagues")) {
            if (is_regular_file(p.path()) && p.path().extension() == ".txt") {
                FifamReader reader(p.path(), 0);
                if (reader.Available()) {
                    foom::comp *league = nullptr;
                    DivisionInfo *div = nullptr;
                    foom::club *cupWinner = nullptr;
                    foom::club *cupRunnerUp = nullptr;
                    foom::team *leagueWinner = nullptr;
                    Set<UInt> usedTeamIDs;
                    foom::nation *nation = nullptr;

                    auto finishLeague = [&] {
                        if (league && div) {
                            if (!nation)
                                nation = league->mNation;
                            if (league->mVecTeams.size() != div->mTeams) {
                                return Error(L"League tables:\nincorrect league teams count\nin %s\n%d - %d", div->mName.c_str(), league->mVecTeams.size(), div->mTeams);
                            }
                            if (!leagueWinner && !league->mVecTeams.empty())
                                leagueWinner = league->mVecTeams[0];
                            league->mConverterData.mUsesTableData = true;
                        }
                        league = nullptr;
                        div = nullptr;
                        return true;
                    };

                    while (!reader.IsEof()) {
                        if (!reader.EmptyLine()) {
                            String line = reader.ReadFullLine();
                            if (Utils::StartsWith(line, L";"))
                                continue;
                            if (Utils::StartsWith(line, L"LEAGUE") || Utils::StartsWith(line, L"league")) {
                                finishLeague();
                                Int leagueId = -1;
                                if (swscanf(line.c_str(), L"%*s %d", &leagueId) != 1) {
                                    Error(L"League tables:\nincorrect league line format\nin %s\n%s", div->mName.c_str(), line.c_str());
                                    break;
                                }
                                league = mFoomDatabase->get<foom::comp>(leagueId);
                                if (!league) {
                                    Error(L"League tables:\nunable to find league by id\nin %s\nid %u", div->mName.c_str(), leagueId);
                                    break;
                                }
                                for (auto &d : mDivisions) {
                                    if (d.mID == leagueId) {
                                        div = &d;
                                        break;
                                    }
                                }
                                if (!div) {
                                    Error(L"League tables:\nunable to find division by id\nin %s\nid %u", div->mName.c_str(), leagueId);
                                    break;
                                }
                                league->mVecTeams.clear();
                            }
                            else if (Utils::StartsWith(line, L"NON") || Utils::StartsWith(line, L"non"))
                                break;
                            else {
                                Int teamId = -1;
                                WideChar status[64];
                                if (league && div) {
                                    if (swscanf(line.c_str(), L"%d %s", &teamId, status) == 2) {
                                        foom::club *team = mFoomDatabase->get<foom::club>(teamId);
                                        if (!team) {
                                            Error(L"League tables:\nunable to find team by id\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                        }
                                        else {
                                            if (team->mExtinct) {
                                                Error(L"League tables:\nextinct team in the league\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                            }
                                            else {
                                                if (team->mNation != league->mNation) {
                                                    Error(L"League tables:\nteam from incorrect country in the league\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                                }
                                                else {
                                                    if (Utils::Contains(usedTeamIDs, teamId)) {
                                                        Error(L"League tables:\nteam id was already registered in the league\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                                    }
                                                    else {
                                                        league->mVecTeams.push_back(team);
                                                        usedTeamIDs.insert(teamId);
                                                        String strStatus = status;
                                                        foom::club *possibleCupWinner = nullptr;
                                                        foom::club *possibleCupRunnerUp = nullptr;
                                                        if (strStatus == L"P") {
                                                            team->mConverterData.mTablePromoted = true;
                                                        }
                                                        else if (strStatus == L"R") {
                                                            team->mConverterData.mTableRelegated = true;
                                                        }
                                                        else if (strStatus == L"W") {
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"F") {
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"PW" || strStatus == L"WP") {
                                                            team->mConverterData.mTablePromoted = true;
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"PF" || strStatus == L"FP") {
                                                            team->mConverterData.mTablePromoted = true;
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"RW" || strStatus == L"WR") {
                                                            team->mConverterData.mTableRelegated = true;
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"RF" || strStatus == L"FR") {
                                                            team->mConverterData.mTableRelegated = true;
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"-") {

                                                        }
                                                        else {
                                                            Error(L"League tables:\nunknown status\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                                        }
                                                        if (possibleCupWinner) {
                                                            if (cupWinner) {
                                                                Error(L"League tables:\ncup winner was already set\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                                            }
                                                            else {
                                                                cupWinner = possibleCupWinner;
                                                            }
                                                        }
                                                        if (possibleCupRunnerUp) {
                                                            if (cupRunnerUp) {
                                                                Error(L"League tables:\ncup runner-up was already set\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                                            }
                                                            else {
                                                                cupRunnerUp = possibleCupRunnerUp;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        Error(L"League tables:\nunable to parse team id line\nin %s\nid %u\n%s", div->mName.c_str(), teamId, line.c_str());
                                    }
                                }
                            }
                        }
                        else
                            reader.SkipLine();
                    }
                    finishLeague();
                    if (cupWinner)
                        cupWinner->mConverterData.mTableCupWinner = true;
                    if (cupRunnerUp)
                        cupRunnerUp->mConverterData.mTableCupRunnerUp = true;
                    if (nation) {
                        nation->mConverterData.mDomesticComps.cupWinner = cupWinner;
                        nation->mConverterData.mDomesticComps.cupRunnerUp = cupRunnerUp;
                    }
                    //if (nation && (cupWinner || cupRunnerUp)) {
                    //    for (auto &entry : mFoomDatabase->mComps) {
                    //        auto &c = entry.second;
                    //        if (c.mNation) {
                    //            if (c.mType == 0) { // domestic main league
                    //                c.mVecHistory.erase(std::remove_if(c.mVecHistory.begin(), c.mVecHistory.end(), [](const foom::comp::history &h) {
                    //                    return h.mYear >= CURRENT_YEAR;
                    //                    }), c.mVecHistory.end());
                    //                if (!c.mIsExtinct) {
                    //                    foom::comp::history newEntry;
                    //                    newEntry.mYear = CURRENT_YEAR;
                    //                    newEntry.mFirstPlaced = cupWinner;
                    //                    newEntry.mSecondPlaced = cupRunnerUp;
                    //                    c.mVecHistory.push_back(newEntry);
                    //                }
                    //            }
                    //            else if (c.mType == 2) { // domestic main cup
                    //                c.mVecHistory.erase(std::remove_if(c.mVecHistory.begin(), c.mVecHistory.end(), [](const foom::comp::history & h) {
                    //                    return h.mYear >= CURRENT_YEAR;
                    //                    }), c.mVecHistory.end());
                    //                if (!c.mIsExtinct) {
                    //                    foom::comp::history newEntry;
                    //                    newEntry.mYear = CURRENT_YEAR;
                    //                    newEntry.mFirstPlaced = cupWinner;
                    //                    newEntry.mSecondPlaced = cupRunnerUp;
                    //                    c.mVecHistory.push_back(newEntry);
                    //                }
                    //            }
                    //        }
                    //    }
                    //}
                }
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
                        Int playerId = Utils::SafeConvertInt<Int>(foomId);
                        if (playerId != -1) {
                            foom::player *foomPlayer = mFoomDatabase->get<foom::player>(playerId);
                            if (foomPlayer) {
                                auto SetPlayerParameter = [](auto & param, String const &str) {
                                    if (!str.empty()) {
                                        Int paramId = Utils::SafeConvertInt<Int>(str);
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
