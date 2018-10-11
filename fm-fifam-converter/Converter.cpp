#include "Converter.h"
#include "FifamCompLeague.h"
#include "FifamNames.h"
#include "Random.h"

Converter::~Converter() {
    delete mReferenceDatabase;
    delete mFifamDatabase;
    delete mFoomDatabase;
}

void Converter::ReadAdditionalInfo(Path const &infoPath) {
    {
        std::wcout << L"Reading fifam-uids.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam-uids.csv", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::club::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, dummy, dummy, dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAID);
                    map_find(mFoomDatabase->mClubs, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_countries.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_countries.csv", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::nation::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, dummy, dummy, FootballManagerID, info.mFIFAID, dummy, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mNations, FootballManagerID).mConverterData = info;
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
        FifamReader reader(infoPath / L"fifam_divisions.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    DivisionInfo d;
                    reader.ReadLineWithSeparator(L'\t', d.mNationName, d.mNationID, d.mName, d.mShortName, d.mID, d.mIsLvl, d.mLevel, d.mIsTop, d.mParent, d.mTeams, d.mRep, d.mPriority);
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
}

void Converter::Convert(UInt gameId, Bool writeToGameFolder) {

}

void Converter::Convert(UInt gameId, Path const &originalDb, Path const &referenceDb, Bool writeToGameFolder) {
    // read reference database here

    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadPersons = false;
    mFifamDatabase = new FifamDatabase(gameId, originalDb);
    mFoomDatabase = new foom::db(L"D:\\Projects\\fifam\\db\\foom");
    ReadAdditionalInfo(L"D:\\Projects\\fifam\\db");

    // convert nations, national teams, leagues
    for (auto &entry : mFoomDatabase->mNations) {
        auto &nation = entry.second;
        auto country = mFifamDatabase->GetCountry(nation.mConverterData.mFIFAManagerID);
        if (country) {
            ConvertNationInfo(country, &nation);

            Map<UInt, DivisionInfo *> leagueLevels;
            Vector<DivisionInfo *> comps;
            Vector<DivisionInfo *> unusedComps;

            // find all division competitions for this country
            for (auto &divInfo : mDivisions) {
                if (divInfo.mNationID == nation.mID) {
                    Int minPriority = (gameId <= 7) ? 2 : 1;
                    if (divInfo.mPriority >= minPriority) {
                        if (divInfo.mIsLvl)
                            leagueLevels[divInfo.mLevel] = &divInfo;
                        else
                            comps.push_back(&divInfo);
                    }
                    else
                        unusedComps.push_back(&divInfo);
                }
            }
            // sort comps
            std::sort(comps.begin(), comps.end(), [](DivisionInfo *a, DivisionInfo *b) {
                return a->mLevel < b->mLevel;
            });
            // add league levels
            for (UInt i = 0; i < 16; i++) {
                DivisionInfo *level = nullptr;
                String levelName;
                if (leagueLevels.find(i) != leagueLevels.end()) {
                    level = leagueLevels[i];
                    if (level->mName.length() < 29)
                        levelName = level->mName;
                    else
                        levelName = level->mShortName;
                }
                else {
                    Vector<String> divsOnLevelNames;
                    for (UInt c = 0; c < comps.size(); c++) {
                        if (comps[c]->mLevel == i) {
                            if (!level)
                                level = comps[c];
                            divsOnLevelNames.push_back(comps[c]->mName);
                        }
                    }
                    if (!divsOnLevelNames.empty())
                        levelName = FifamNames::FindCommonPrefix(divsOnLevelNames, true);
                }
                if (level) {
                    auto &levelNames = country->mLeagueLevelNames.emplace_back();
                    FifamTrSetAll<String>(levelNames, FifamNames::LimitName(levelName, 28));
                    country->mLeagueLevels[i].mEqualPointsSorting = FifamEqualPointsSorting::GoalDiff;
                    country->mLeagueLevels[i].mNumNonEUPlayersAllowed = 0;
                    country->mLeagueLevels[i].mNumRelegatedTeams = 0;


                    country->mLeagueLevels[i].mRating = (UChar)(roundf((
                        (Float)level->mRep / 370.0f +
                        (Float)nation.mYouthRating / 1200.0f +
                        0.15f) * 100.0f));
                }
                if (i == 0)
                    country->mAveragePlayerRating = country->mLeagueLevels[i].mRating;
            }
            // add leagues
            UInt clubCounter = 1;
            for (UShort i = 0; i < comps.size(); i++) {
                auto div = comps[i];
                FifamCompID compID = { (UChar)country->mId, FifamCompType::League, i };
                String compName;
                if (div->mName.length() < 64)
                    compName = div->mName;
                else
                    compName = div->mShortName;
                auto league = mFifamDatabase->CreateCompetition(FifamCompDbType::League, compID, compName)->AsLeague();
                league->mCompetitionLevel = div->mLevel;
                league->mLeagueLevel = div->mLevel;
                league->mNumRounds = 2;
                league->mNumSubsAllowed = 3;
                league->mNumTeams = div->mTeams;
                league->mRoundType = FifamRoundID::Group1;
                league->mAttendanceMp = 50;
                league->mTransferMarketMp = 50;
                league->mTeams.resize(div->mTeams);

                // add teams
                auto comp = mFoomDatabase->get<foom::comp>(div->mID);
                if (!comp)
                    continue;
                UInt leagueClubCounter = 0;
                for (auto entry : comp->mVecTeams) {
                    foom::club *team = (foom::club *)entry;
                    auto club = mFifamDatabase->CreateClub(country);
                    // uid
                    club->mUniqueID = (country->mId << 16) | clubCounter;
                    
                    ConvertClub(club, team, country);

                    mFifamDatabase->AddClubToMap(club);
                    country->mClubsMap[club->mUniqueID] = club;
                    clubCounter++;

                    // put team to league
                    //if (team->mLastPosition > 0 && team->mLastPosition <
                    league->mTeams[leagueClubCounter].mPtr = club;
                    leagueClubCounter++;
                }

                league->GenerateFixtures();
            }
        }
    }
    // convert referees
    for (auto &entry : mFoomDatabase->mOfficials) {
        auto &official = entry.second;
        if (!official.mFemale && official.mNation) {
            auto country = mFifamDatabase->GetCountry(official.mNation->mConverterData.mFIFAManagerReplacementID);
            if (country) {
                auto referee = country->AddReferee();
                ConvertReferee(referee, &official);
            }
        }
    }

    FifamVersion version = FifamDatabase::GetGameDbVersion(gameId);
    mFifamDatabase->Write(gameId, version.GetYear(), version.GetNumber(),
        Utils::Format(writeToGameFolder ?
            L"D:\\Games\\FIFA Manager %02d\\database" :
            L"fm_test\\database",
            gameId).c_str());

    delete mFifamDatabase;
    delete mFoomDatabase;
}



void Converter::ConvertNationInfo(FifamCountry *dst, foom::nation *nation) {
    dst->mAssessmentData[5] = nation->mEuroCoeff1;
    dst->mAssessmentData[4] = nation->mEuroCoeff2;
    dst->mAssessmentData[3] = nation->mEuroCoeff3;
    dst->mAssessmentData[2] = nation->mEuroCoeff4;
    dst->mAssessmentData[1] = nation->mEuroCoeff5;
    dst->mAssessmentData[0] = nation->mEuroCoeff6;
    dst->mFifaRanking = nation->mRankingPoints;
    dst->mYearsForNaturalization = nation->mYearsToGainNationality;
    // national team
    if (nation->mYearAssociationFormed != 0)
        dst->mNationalTeam.mYearOfFoundation = nation->mYearAssociationFormed;
    Int prestige = (Int)ceilf((Float)nation->mReputation / 500.0f);
    dst->mNationalTeam.mInternationalPrestige = prestige;
    dst->mNationalTeam.mNationalPrestige = prestige;

    ConvertKitsAndColors(&dst->mNationalTeam, nation->mVecKits);
}

void Converter::ConvertClub(FifamClub *dst, foom::club *team, FifamCountry *country) {
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
    // city
    if (team->mCity)
        FifamTrSetAll(dst->mCityName, FifamNames::LimitName(team->mCity->mName, 29));
    else
        dst->mCityName = country->mNationalTeam.mCityName;
    // latitude/longitude
    if (team->mLatitude != 0 && team->mLongitude != 0)
        dst->mGeoCoords.SetFromFloat(team->mLatitude, team->mLongitude);
    else
        dst->mGeoCoords = country->mNationalTeam.mGeoCoords;
    // club prestige
    Int prestige = team->mReputation / 450;
    if (prestige > 20)
        prestige = 20;
    dst->mInternationalPrestige = prestige;
    // TODO change this
    dst->mNationalPrestige = prestige;
    // media pressure
    // TODO: read from reference file
    dst->mMediaPressure = FifamClubMediaPressure::Normal;
    // individual TV money

    // capital
    dst->mInitialCapital = foom::db::convert_money(team->mBalance);
    // transfer budget
    if (team->mTransferBudget > 0)
        dst->mTransferBudget = foom::db::convert_money(team->mTransferBudget);
    // joint-stock company
    dst->mJointStockCompany = team->mOwnershipType == 2; // Public Limited Company
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
    // Fan Base 1st League

    ConvertKitsAndColors(dst, team->mVecKits);
}

void Converter::ConvertKitsAndColors(FifamClub *dst, Vector<foom::kit> const &kits) {
    enum KitPart { Shirt, Icon, Text, Shorts, Socks };

    const foom::kit *tmpKitSets[5][2] = {};
    const foom::kit *tmpCompKitSets[5][2] = {};

    for (auto &k : kits) {
        if (k.mKit == 0 || k.mKit == 1) {
            if (k.mType >= 1 && k.mType <= 5 && k.mAlternativeKitNumber == 0) {
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

    const foom::kit *clubClrs = nullptr;
    for (UInt i = 0; i < 5; i++) {
        if (kitSets[i][0]) {
            clubClrs = kitSets[i][0];
            break;
        }
    }

    Color backgroundClr;
    Color foregroundClr;

    // badge and colors
    if (clubClrs) {
        backgroundClr = Color(clubClrs->mBackground.r, clubClrs->mBackground.g, clubClrs->mBackground.b);
        foregroundClr = Color(clubClrs->mForeground.r, clubClrs->mForeground.g, clubClrs->mForeground.b);
        Color outlineClr = Color(clubClrs->mOutline.r, clubClrs->mOutline.g, clubClrs->mOutline.b);
        Color secondColor = foregroundClr;
        if (Color::Distance(backgroundClr, foregroundClr) < 100) {
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
        dst->mClubColour.Set(backgroundClr, secondColor);
    }
    else {
        UInt numTeamColors = FifamClub::mTeamColorsTable.size();
        if (numTeamColors > 0) {
            dst->mClubColour = FifamClub::mTeamColorsTable[Random::Get(0, numTeamColors - 1)];
            backgroundClr = dst->mClubColour.first;
            foregroundClr = dst->mClubColour.second;
        }
    }
    dst->mClubColour2 = dst->mClubColour;
    dst->mMerchandiseColour = backgroundClr;
    dst->mBackgroundColour = backgroundClr;
    dst->mHeaderColour = foregroundClr;
    dst->mBadge.SetColorDefault(dst->mClubColour);

    // home & away kits
    for (UInt i = 0; i < 2; i++) {
        auto &kit = dst->mKit.mSets[i];
        // kit - number
        if (kitSets[KitPart::Shirt][i])
            kit.mShirtNumberColor = kitSets[KitPart::Shirt][i]->mNumberColour;
        else
            kit.mShirtNumberColor = { 0, 0, 0 };
        // kit - armband
        kit.mCaptainArmbandColor = kit.mShirtNumberColor;
        // kit - socks
        kit.mSocks = 1;
        kit.mSocksColors = { backgroundClr, backgroundClr };
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
                if (set->mBackOfShirtStyle == 2) // Plain
                    kit.mShirt = 14;
                else
                    kit.mShirt = 41;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 4:
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
                kit.mShirt = 19;
                kit.mShirtColors = { set->mForeground, set->mOutline, set->mBackground };
                break;
            case 12:
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
            case 37:
                kit.mShirt = 26;
                kit.mShirtColors = { set->mBackground, set->mForeground, set->mBackground };
                break;
            }
        }
    }
}

void Converter::ConvertReferee(FifamReferee *dst, foom::official *official) {
    dst->mFirstName = FifamNames::LimitPersonName(official->mFirstName, 19);
    dst->mLastName = FifamNames::LimitPersonName(official->mSecondName, 19);
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
