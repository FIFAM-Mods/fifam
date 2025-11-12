#include "Converter.h"

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

FifamClubLink Converter::GetCompHost(foom::comp * c, Bool checkPreviousSeason) {
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

Map<UShort, foom::team *> Converter::GetWinnersList(Vector<foom::comp *> const &inputComps, bool isSupercup) {
    Map<UShort, foom::team *> result;
    if (inputComps.empty())
        return result;
    auto comps = inputComps;
    std::sort(comps.begin(), comps.end(), [](foom::comp * a, foom::comp * b) {
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

Bool IsArabCountry(FifamNation const &nationId) {
    return nationId == FifamNation::Saudi_Arabia
        || nationId == FifamNation::United_Arab_Emirates
        || nationId == FifamNation::Bahrain
        || nationId == FifamNation::Iraq
        || nationId == FifamNation::Kuwait
        || nationId == FifamNation::Jordan
        || nationId == FifamNation::Lebanon
        || nationId == FifamNation::Oman
        || nationId == FifamNation::Syria
        || nationId == FifamNation::Palestinian_Authority
        || nationId == FifamNation::Qatar
        || nationId == FifamNation::Yemen
        || nationId == FifamNation::Tunisia
        || nationId == FifamNation::Egypt
        || nationId == FifamNation::Algeria
        || nationId == FifamNation::Morocco
        || nationId == FifamNation::Libya
        || nationId == FifamNation::Sudan
        || nationId == FifamNation::Mauritania
        || nationId == FifamNation::Djibouti
        || nationId == FifamNation::Somalia;
}

Bool Converter::GenerateCalendar(FifamNation const &countryId, FifamDatabase * database, Vector<FifamCompLeague *> const &leagues,
    Vector<FifamCompCup *> const &cups, Pair<FifamCompLeague *, FifamCompLeague *> const &split, Vector<PlayOffInfo *> const &playOffs)
{
    struct MatchdaysOption {
        Int level = 0;
        Int matchdaysType = 0;
    };

    if (leagues.empty() && cups.empty())
        return false;

    Array<Array<UInt, 366>, 2> calendar = {};

    FifamCountry *country = database->GetCountry(countryId.ToInt());

    // is valid country
    if (!country) {
        Alert(AL_ERROR, L"Calendar generator: unknown country (ID %d)", countryId.ToInt());
        return false;
    }

    Bool veryDifficultCalendar = country->mId == FifamNation::England;

    auto PutOneMatch = [](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, UShort days = 1) {
        auto MarkMatch = [](Array<Array<UInt, 366>, 2> & _c, Bool _season, Short _matchdayId, UInt _value) {
            if (_matchdayId >= 1 && _matchdayId <= 365) {
                if (_c[_season][_matchdayId] < _value)
                    _c[_season][_matchdayId] = _value;
            }
        };
        Short m = (Short)matchdayId;
        MarkMatch(c, season, m, 1002);
        for (UShort d = 1; d <= days; d++) {
            MarkMatch(c, season, m + d, 1001);
            MarkMatch(c, season, m - d, 1001);
        }
    };

    auto MarkPossibleMatchday = [&](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, UInt & index, UShort days = 1) {
        if (matchdayId >= 1 && matchdayId <= 365 && c[season][matchdayId] == 0) {
            PutOneMatch(c, season, matchdayId, days);
            c[season][matchdayId] = ++index;
        }
    };

    auto MarkPossibleWeekEndMatchday = [&](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, MatchdaysOption &option, UInt & index, UShort days = 1) {
        if (matchdayId >= 1) {

            //Bool bSunday = false;
            //if (option.matchdaysType == 0) // default
            //    bSunday = option.level == 0;
            //if (option.matchdaysType == 1) // prior_saturday
            //    bSunday = option.level == 0 && matchdayId >= 2 && c[season][matchdayId - 2] != 0;
            //if (option.matchdaysType == 2 || option.matchdaysType == 4) // prior_sunday, force sunday
            //    bSunday = true;
            //if (option.matchdaysType == 3) // force saturday
            //    bSunday = false;

            Bool bSunday = option.matchdaysType == 4;
            if (option.matchdaysType != 3 && option.matchdaysType != 4) {
                bSunday = option.matchdaysType == 2 || (option.matchdaysType == 0 && option.level == 0);
                if (option.level == 0 && !bSunday && matchdayId >= 2 && c[season][matchdayId - 2] != 0)
                    bSunday = true;
            }
            if (bSunday)
                matchdayId++;
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0 && (!bSunday || c[season][matchdayId - 1] == 0)) {
                    PutOneMatch(c, season, matchdayId, days);
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

    auto MarkPossibleWeekEndMatchdayForCup = [&](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, UInt & index, UShort days = 1) {
        if (matchdayId >= 1) {
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId, days);
                    c[season][matchdayId] = ++index;
                }
                else {
                    if (matchdayId <= 364 && c[season][matchdayId + 1] == 0) {
                        PutOneMatch(c, season, matchdayId + 1, days);
                        c[season][matchdayId + 1] = ++index;
                    }
                }
            }
        }
    };

    auto MarkPossibleMidWeekMatchday = [&](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, MatchdaysOption &option, UInt & index, UShort days = 1) {
        matchdayId += 3;
        if (matchdayId >= 1) {
            Bool bSunday = option.matchdaysType == 4;
            if (option.matchdaysType != 3 && option.matchdaysType != 4) {
                bSunday = option.matchdaysType == 2 || (option.matchdaysType == 0 && option.level == 0);
                if (option.level == 0 && !bSunday && matchdayId >= 2 && c[season][matchdayId - 2] != 0)
                    bSunday = true;
            }
            if (bSunday)
                matchdayId++;
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId, days);
                    c[season][matchdayId] = ++index;
                }
            }
        }
    };

    auto MarkPossibleMidWeekMatchdayForCup = [&](Array<Array<UInt, 366>, 2> & c, Bool season, UShort matchdayId, UInt & index, UShort days = 1) {
        if (veryDifficultCalendar) {
            MarkPossibleMatchday(c, season, matchdayId + 3, index, days);
            MarkPossibleMatchday(c, season, matchdayId + 5, index, days);
        }
        matchdayId += 4;
        if (matchdayId >= 1) {
            if (matchdayId <= 365) {
                if (c[season][matchdayId] == 0) {
                    PutOneMatch(c, season, matchdayId, days);
                    c[season][matchdayId] = ++index;
                }
                else {
                    if (matchdayId <= 364 && c[season][matchdayId + 1] == 0) {
                        PutOneMatch(c, season, matchdayId + 1, days);
                        c[season][matchdayId + 1] = ++index;
                    }
                }
            }
        }
    };

    auto PutCompToCalendar = [&](Array<Array<UInt, 366>, 2> & c, FifamCompetition * comp) {
        Bool isInternational = comp->mID.mRegion == FifamCompRegion::International;
        Bool firstSeason = comp->TakesPlaceInSeason(2018) || comp->TakesPlaceInSeason(2020);
        Bool secondSeason = comp->TakesPlaceInSeason(2019) || comp->TakesPlaceInSeason(2021);
        UShort days = isInternational ? 3 : 1;
        if (comp->GetDbType() == FifamCompDbType::League) {
            if (firstSeason) {
                for (auto &m : comp->AsLeague()->mFirstSeasonMatchdays) {
                    if (m <= 365)
                        PutOneMatch(c, 0, m, days);
                    else
                        PutOneMatch(c, 1, m - 365, days);
                }
            }
            if (secondSeason) {
                if (isInternational) {
                    for (auto &m : comp->AsLeague()->mFirstSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m, days);
                        else
                            PutOneMatch(c, 0, m - 365, days);
                    }
                }
                else {
                    for (auto &m : comp->AsLeague()->mSecondSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m, days);
                        else
                            PutOneMatch(c, 0, m - 365, days);
                    }
                }
            }
        }
        else if (comp->GetDbType() == FifamCompDbType::Round) {
            if (firstSeason) {
                for (auto &m : comp->AsRound()->mFirstSeasonMatchdays) {
                    if (m <= 365)
                        PutOneMatch(c, 0, m, days);
                    else
                        PutOneMatch(c, 1, m - 365, days);
                }
            }
            if (secondSeason) {
                if (isInternational) {
                    for (auto &m : comp->AsRound()->mFirstSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m, days);
                        else
                            PutOneMatch(c, 0, m - 365, days);
                    }
                }
                else {
                    for (auto &m : comp->AsRound()->mSecondSeasonMatchdays) {
                        if (m <= 365)
                            PutOneMatch(c, 1, m, days);
                        else
                            PutOneMatch(c, 0, m - 365, days);
                    }
                }
            }
        }
        else if (comp->GetDbType() == FifamCompDbType::Cup) {
            for (auto &m : comp->AsCup()->mFirstSeasonMatchdays)
                PutOneMatch(c, 0, m, days);
            for (auto &m : comp->AsCup()->mSecondSeasonMatchdays)
                PutOneMatch(c, 1, m, days);
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

        if (!l->GetProperty<Bool>(L"custom_calendar", false)) {

            auto cc = calendar;

            if (l->mLeagueLevel == 0) {
                // put international matches to calendar (for highest league level only)
                for (auto &m : database->mRules.mInternationalFriendliesFirstSeason)
                    PutOneMatch(cc, 0, m);
                for (auto &m : database->mRules.mInternationalFriendliesSecondSeason)
                    PutOneMatch(cc, 1, m);
            }

            for (auto [compID, comp] : database->mCompMap) {
                if (compID.mType == FifamCompType::WorldCup || compID.mType == FifamCompType::QualiWC || compID.mType == FifamCompType::ConfedCup) {
                    if (l->mLeagueLevel == 0)
                        PutCompToCalendar(cc, comp);
                }
                else if (compID.mType == FifamCompType::WorldClubChamp && compID.mRegion == FifamCompRegion::Europe) {
                    if (comp->GetDbType() != FifamCompDbType::Round || (comp->AsRound()->mRoundType == FifamRoundID::Final
                        || comp->AsRound()->mRoundType == FifamRoundID::Final3rdPlace || comp->AsRound()->mRoundType == FifamRoundID::Semifinal)
                        || (country->mContinent != FifamContinent::Europe && country->mContinent != FifamContinent::SouthAmerica))
                    {
                        PutCompToCalendar(cc, comp);
                    }
                }
                else {
                    switch (country->mContinent.ToInt()) {
                    case FifamContinent::Europe:
                        if (compID.mType == FifamCompType::EuroCup || compID.mType == FifamCompType::QualiEC
                            || compID.mType == FifamCompType::EuroNL || compID.mType == FifamCompType::EuroNLQ)
                        {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::Europe
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2 || compID.mType == FifamCompType::ICC
                                || compID.mType == FifamCompType::ConferenceLeague))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    case FifamContinent::SouthAmerica:
                        if (compID.mType == FifamCompType::CopaAmerica) {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::SouthAmerica
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2)
                            || (compID.mRegion == FifamCompRegion::Asia && compID.mType == FifamCompType::Continental1))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    case FifamContinent::NorthAmerica:
                        if (compID.mType == FifamCompType::NamCup || compID.mType == FifamCompType::NamNL || compID.mType == FifamCompType::NamNLQ) {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::NorthAmerica
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    case FifamContinent::Africa:
                        if (compID.mType == FifamCompType::AfricaCup || compID.mType == FifamCompType::AfricaCupQ) {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::Africa
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    case FifamContinent::Asia:
                        if (compID.mType == FifamCompType::AsiaCup || compID.mType == FifamCompType::AsiaCupQ) {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::Asia
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2)
                            || (IsArabCountry(countryId) && compID.mRegion == FifamCompRegion::Africa && compID.mType == FifamCompType::Continental1))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    case FifamContinent::Oceania:
                        if (compID.mType == FifamCompType::OfcCup) {
                            if (l->mLeagueLevel == 0)
                                PutCompToCalendar(cc, comp);
                        }
                        else if (compID.mRegion == FifamCompRegion::Oceania
                            && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                                || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                                || compID.mType == FifamCompType::Continental2))
                        {
                            PutCompToCalendar(cc, comp);
                        }
                        break;
                    default:
                        if (compID.mType != FifamCompType::League
                            && comp->GetProperty<Bool>(L"custom_calendar", false)
                            && comp->GetProperty<UChar>(L"min_level", 0) <= l->mLeagueLevel
                            && comp->GetProperty<UChar>(L"max_level", 255) >= l->mLeagueLevel)
                        {
                            if (comp->GetDbType() == FifamCompDbType::League && compID.mType == FifamCompType::Relegation && comp->mCompetitionLevel == 0) {
                                if (l->mLeagueLevel == 0)
                                    PutCompToCalendar(cc, comp);
                            }
                            else
                                PutCompToCalendar(cc, comp);
                        }
                        break;
                    }
                }
            }

            //create_directories(L"calendars");
            //FifamWriter calendarWriter(L"calendars\\calendar" + Utils::Format(L"%d", countryId.ToInt()) + L".csv", 14, FifamVersion());
            //for (auto const &wc : cc) {
            //    calendarWriter.WriteLine(L"SEASON");
            //    UInt wday = 0;
            //    for (auto const &wd : wc) {
            //        calendarWriter.WriteLine(Utils::Format(L"%d. %d", wday++, wd));
            //    }
            //}

            // play matches in Sunday or Saturday?
            MatchdaysOption option;
            option.level = l->mLeagueLevel;
            option.matchdaysType = l->GetProperty<Int>(L"calendarMatches", 0);
            if (l->mID.mRegion == FifamCompRegion::India && l->mLeagueLevel == 1 && l->mID.mIndex == 1)
                option.level = 0;

            Int startMatchday1 = 48; // End of August
            Int startMatchday2 = 27; // End of July
            Int startMatchday3 = 13; // Middle of July
            Int endMatchday = 315; // May

            Int startDate = l->GetProperty<Int>(L"startDate", 0);
            Int endDate = l->GetProperty<Int>(L"endDate", 0);

            if ((endDate > 0 && endDate < startDate) || startDate > 365 || endDate > 365) {
                Alert(AL_ERROR, L"Incorrect startDate/endDate in league\nLeague: %s\nstartDate: %d\nendDate: %d", FifamTr(l->mName).c_str(), startDate, endDate);
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
                    MarkPossibleWeekEndMatchday(cc, s, m, option, matchdayIndex[s]);

                // phase 2 - add possible match every week
                for (Int m = startMatchday1; m <= endMatchday; m += 7)
                    MarkPossibleWeekEndMatchday(cc, s, m, option, matchdayIndex[s]);

                // phase 3 - add possible matches in August (reverse direction)
                if (!customStartDate) {
                    for (Int m = startMatchday1 - 7; m >= startMatchday2; m -= 7)
                        MarkPossibleWeekEndMatchday(cc, s, m, option, matchdayIndex[s]);
                }

                // phase 4 - add possible mid-week matches every 2 weeks
                for (Int m = startMatchday2 + 14; m <= endMatchday; m += 14)
                    MarkPossibleMidWeekMatchday(cc, s, m, option, matchdayIndex[s]);

                // phase 5 - add possible matches in July (reverse direction)
                if (!customStartDate) {
                    for (Int m = startMatchday2 - 7; m >= startMatchday3; m -= 7)
                        MarkPossibleWeekEndMatchday(cc, s, m, option, matchdayIndex[s]);
                }

                // phase 6 - add possible mid-week matches every week
                for (Int m = startMatchday1; m <= endMatchday; m += 7)
                    MarkPossibleMidWeekMatchday(cc, s, m, option, matchdayIndex[s]);

                // phase 7 - add possible mid-week matches every week in July and August (reverse direction)
                if (!customStartDate) {
                    for (Int m = startMatchday1 - 7; m >= startMatchday3; m -= 7)
                        MarkPossibleMidWeekMatchday(cc, s, m, option, matchdayIndex[s]);
                }
            }

            for (UInt s = 0; s < 2; s++) {
                Vector<Pair<UInt, UInt>> availableMatchdays;
                for (UInt i = 1; i < cc[s].size(); i++) {
                    if (cc[s][i] > 0 && cc[s][i] < 1000)
                        availableMatchdays.emplace_back(cc[s][i], i);
                }
                UInt requiredMatchdaysForLeague = l->GetNumMatchdays();
                UInt requiredMatchdaysForSplit = (split.first && l->mLeagueLevel == 0) ? Utils::Max(split.first->GetNumMatchdays(), split.second->GetNumMatchdays()) : 0;
                UInt requiredMatchdays = requiredMatchdaysForLeague + requiredMatchdaysForSplit;
                if (availableMatchdays.size() < requiredMatchdays) {
                    Alert(AL_ERROR, L"Not enough available matches in calendar for league\nLeague: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
                        FifamTr(l->mName).c_str(), s + 1, requiredMatchdays, availableMatchdays.size());
                }
                else {
                    std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                        return a.first < b.first;
                        });
                    availableMatchdays.resize(requiredMatchdays);
                    std::sort(availableMatchdays.begin(), availableMatchdays.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                        return a.second < b.second;
                        });
                    if (s == 0) {
                        l->mFirstSeasonMatchdays.resize(requiredMatchdaysForLeague);
                        for (UInt i = 0; i < requiredMatchdaysForLeague; i++)
                            l->mFirstSeasonMatchdays[i] = availableMatchdays[i].second;
                        if (split.first && l->mLeagueLevel == 0) {
                            Vector<Pair<UInt, UInt>> matchdaysForFirstSplit(availableMatchdays.begin() + requiredMatchdaysForLeague, availableMatchdays.end());
                            if (matchdaysForFirstSplit.size() > split.first->GetNumMatchdays()) {
                                std::sort(matchdaysForFirstSplit.begin(), matchdaysForFirstSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.first < b.first;
                                    });
                                matchdaysForFirstSplit.resize(split.first->GetNumMatchdays());
                                std::sort(matchdaysForFirstSplit.begin(), matchdaysForFirstSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.second < b.second;
                                    });
                            }
                            split.first->mFirstSeasonMatchdays.resize(split.first->GetNumMatchdays());
                            for (UInt i = 0; i < split.first->GetNumMatchdays(); i++)
                                split.first->mFirstSeasonMatchdays[i] = matchdaysForFirstSplit[i].second;

                            Vector<Pair<UInt, UInt>> matchdaysForSecondSplit(availableMatchdays.begin() + requiredMatchdaysForLeague, availableMatchdays.end());
                            if (matchdaysForSecondSplit.size() > split.second->GetNumMatchdays()) {
                                std::sort(matchdaysForSecondSplit.begin(), matchdaysForSecondSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.first < b.first;
                                    });
                                matchdaysForSecondSplit.resize(split.second->GetNumMatchdays());
                                std::sort(matchdaysForSecondSplit.begin(), matchdaysForSecondSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.second < b.second;
                                    });
                            }
                            split.second->mFirstSeasonMatchdays.resize(split.second->GetNumMatchdays());
                            for (UInt i = 0; i < split.second->GetNumMatchdays(); i++)
                                split.second->mFirstSeasonMatchdays[i] = matchdaysForSecondSplit[i].second;
                        }
                    }
                    else {
                        l->mSecondSeasonMatchdays.resize(requiredMatchdaysForLeague);
                        for (UInt i = 0; i < requiredMatchdaysForLeague; i++)
                            l->mSecondSeasonMatchdays[i] = availableMatchdays[i].second;
                        if (split.first && l->mLeagueLevel == 0) {
                            Vector<Pair<UInt, UInt>> matchdaysForFirstSplit(availableMatchdays.begin() + requiredMatchdaysForLeague, availableMatchdays.end());
                            if (matchdaysForFirstSplit.size() > split.first->GetNumMatchdays()) {
                                std::sort(matchdaysForFirstSplit.begin(), matchdaysForFirstSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.first < b.first;
                                    });
                                matchdaysForFirstSplit.resize(split.first->GetNumMatchdays());
                                std::sort(matchdaysForFirstSplit.begin(), matchdaysForFirstSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.second < b.second;
                                    });
                            }
                            split.first->mSecondSeasonMatchdays.resize(split.first->GetNumMatchdays());
                            for (UInt i = 0; i < split.first->GetNumMatchdays(); i++)
                                split.first->mSecondSeasonMatchdays[i] = matchdaysForFirstSplit[i].second;

                            Vector<Pair<UInt, UInt>> matchdaysForSecondSplit(availableMatchdays.begin() + requiredMatchdaysForLeague, availableMatchdays.end());
                            if (matchdaysForSecondSplit.size() > split.second->GetNumMatchdays()) {
                                std::sort(matchdaysForSecondSplit.begin(), matchdaysForSecondSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.first < b.first;
                                    });
                                matchdaysForSecondSplit.resize(split.second->GetNumMatchdays());
                                std::sort(matchdaysForSecondSplit.begin(), matchdaysForSecondSplit.end(), [](Pair<UInt, UInt> const &a, Pair<UInt, UInt> const &b) {
                                    return a.second < b.second;
                                    });
                            }
                            split.second->mSecondSeasonMatchdays.resize(split.second->GetNumMatchdays());
                            for (UInt i = 0; i < split.second->GetNumMatchdays(); i++)
                                split.second->mSecondSeasonMatchdays[i] = matchdaysForSecondSplit[i].second;
                        }
                    }
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

    for (auto [compID, comp] : database->mCompMap) {
        if (compID.mType == FifamCompType::WorldCup || compID.mType == FifamCompType::QualiWC || compID.mType == FifamCompType::ConfedCup) {
            PutCompToCalendar(calendar, comp);
        }
        else if (compID.mType == FifamCompType::WorldClubChamp && compID.mRegion == FifamCompRegion::Europe) {
            if (comp->GetDbType() != FifamCompDbType::Round || (comp->AsRound()->mRoundType == FifamRoundID::Final
                || comp->AsRound()->mRoundType == FifamRoundID::Final3rdPlace || comp->AsRound()->mRoundType == FifamRoundID::Semifinal)
                || (country->mContinent != FifamContinent::Europe && country->mContinent != FifamContinent::SouthAmerica))
            {
                PutCompToCalendar(calendar, comp);
            }
        }
        else {
            switch (country->mContinent.ToInt()) {
            case FifamContinent::Europe:
                if (compID.mType == FifamCompType::EuroCup || compID.mType == FifamCompType::QualiEC
                    || compID.mType == FifamCompType::EuroNL || compID.mType == FifamCompType::EuroNLQ
                    || (compID.mRegion == FifamCompRegion::Europe
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2 || compID.mType == FifamCompType::ICC
                            || compID.mType == FifamCompType::ConferenceLeague))
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            case FifamContinent::SouthAmerica:
                if (compID.mType == FifamCompType::CopaAmerica
                    || (compID.mRegion == FifamCompRegion::SouthAmerica
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2 || compID.mType == FifamCompType::ICC))
                    || (compID.mRegion == FifamCompRegion::Asia && compID.mType == FifamCompType::Continental1)
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            case FifamContinent::NorthAmerica:
                if (compID.mType == FifamCompType::NamCup || compID.mType == FifamCompType::NamNL || compID.mType == FifamCompType::NamNLQ
                    || (compID.mRegion == FifamCompRegion::NorthAmerica
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2))
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            case FifamContinent::Africa:
                if (compID.mType == FifamCompType::AfricaCup || compID.mType == FifamCompType::AfricaCupQ
                    || (compID.mRegion == FifamCompRegion::Africa
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2))
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            case FifamContinent::Asia:
                if (compID.mType == FifamCompType::AsiaCup || compID.mType == FifamCompType::AsiaCupQ
                    || (compID.mRegion == FifamCompRegion::Asia
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2))
                    || (IsArabCountry(countryId) && compID.mRegion == FifamCompRegion::Africa && compID.mType == FifamCompType::Continental1)
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            case FifamContinent::Oceania:
                if (compID.mType == FifamCompType::OfcCup
                    || (compID.mRegion == FifamCompRegion::Oceania
                        && (compID.mType == FifamCompType::ChampionsLeague || compID.mType == FifamCompType::UefaCup
                            || compID.mType == FifamCompType::EuroSuperCup || compID.mType == FifamCompType::Continental1
                            || compID.mType == FifamCompType::Continental2))
                    )
                {
                    PutCompToCalendar(calendar, comp);
                }
                break;
            default:
                if (comp->GetProperty<Bool>(L"custom_calendar", false))
                    PutCompToCalendar(calendar, comp);
                break;
            }
        }
    }

    // add league matches to calendar

    Array<Int, 2> lastLeagueMatchday = { 0, 0 };

    for (FifamCompLeague *l : leagues) {
        if (!l->GetProperty<Bool>(L"custom_calendar", false))
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
    if (split.first) {
        if (!split.first->GetProperty<Bool>(L"custom_calendar", false))
            PutCompToCalendar(calendar, split.first);
        if (!split.first->mFirstSeasonMatchdays.empty()) {
            if (split.first->mFirstSeasonMatchdays.back() > lastLeagueMatchday[0])
                lastLeagueMatchday[0] = split.first->mFirstSeasonMatchdays.back();
        }
        if (!split.first->mSecondSeasonMatchdays.empty()) {
            if (split.first->mSecondSeasonMatchdays.back() > lastLeagueMatchday[1])
                lastLeagueMatchday[1] = split.first->mSecondSeasonMatchdays.back();
        }
        if (!split.second->GetProperty<Bool>(L"custom_calendar", false))
            PutCompToCalendar(calendar, split.second);
        if (!split.second->mFirstSeasonMatchdays.empty()) {
            if (split.second->mFirstSeasonMatchdays.back() > lastLeagueMatchday[0])
                lastLeagueMatchday[0] = split.second->mFirstSeasonMatchdays.back();
        }
        if (!split.second->mSecondSeasonMatchdays.empty()) {
            if (split.second->mSecondSeasonMatchdays.back() > lastLeagueMatchday[1])
                lastLeagueMatchday[1] = split.second->mSecondSeasonMatchdays.back();
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
        if (!c->GetProperty<Bool>(L"custom_calendar", false)) {
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
                Alert(AL_WARNING, L"Too difficult calendar in %s (season %d)", FifamTr(country->mName).c_str(), s + 1);

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
                Alert(AL_ERROR, L"Not enough available matches in calendar for cups\nCountry: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
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
                    if (!c->GetProperty<Bool>(L"custom_calendar", false)) {
                        if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup) {
                            if (s == 0)
                                c->mFirstSeasonMatchdays.clear();
                            else
                                c->mSecondSeasonMatchdays.clear();
                        }
                    }
                }
                // fill matchdays
                UInt nextMatchdayIndex = 0;
                for (UInt r = 0; r < 8; r++) {
                    for (UInt m = 0; m < 2; m++) {
                        for (FifamCompCup *c : cups) {
                            if (!c->GetProperty<Bool>(L"custom_calendar", false)) {
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
                }
                // sort matchdays
                for (FifamCompCup *c : cups) {
                    if (!c->GetProperty<Bool>(L"custom_calendar", false)) {
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
    }

    // add FA/League cup matches to calendar
    for (FifamCompCup *c : cups) {
        if (!c->GetProperty<Bool>(L"custom_calendar", false)) {
            if (c->mID.mType == FifamCompType::FaCup || c->mID.mType == FifamCompType::LeagueCup)
                PutCompToCalendar(calendar, c);
        }
    }

    // generate matchdays for supercup

    if (superCup && !superCup->GetProperty<Bool>(L"custom_calendar", false)) {
        if (numSuperCups > 1) {
            Alert(AL_ERROR, L"More than 1 supercup in %s", FifamTr(country->mName).c_str());
        }
        else {
            if (superCup->mRounds.empty()) {
                Alert(AL_ERROR, L"Supercup has no rounds\nCountry : %s", FifamTr(country->mName).c_str());
            }
            else if (superCup->mRounds.size() > 1) {
                if (countryId != FifamNation::Spain) {
                    Alert(AL_ERROR, L"Supercups with more than 1 round are not supported\nCountry : %s", FifamTr(country->mName).c_str());
                }
            }
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
                            Alert(AL_ERROR, L"No available matchdays for supercup in %s (season %d)", FifamTr(country->mName).c_str(), s + 1);
                            cantCreate = true;
                        }
                        else
                            supercupMax = firstMatchday - 7;
                    }
                    if (!cantCreate) {
                        // phase 1 - add possible match every week (reverse direction)
                        for (Int m = supercupMax; m >= supercupMin; m -= 7)
                            MarkPossibleWeekEndMatchdayForCup(supercupcc, s, m, matchdayIndex[s]);

                        // phase 2 - add possible mid-week matches every week (reverse direction)
                        for (Int m = supercupMax; m >= supercupMin; m -= 7)
                            MarkPossibleMidWeekMatchdayForCup(supercupcc, s, m, matchdayIndex[s]);

                        for (Int m = supercupMax; m >= supercupMin; m--)
                            MarkPossibleMidWeekMatchdayForCup(supercupcc, s, m, matchdayIndex[s]);

                        for (Int m = supercupMax; m <= supercupMax + 7; m++)
                            MarkPossibleMatchday(supercupcc, s, m, matchdayIndex[s]);

                        UInt numMatches = (superCup->mRounds[0].mFlags.Check(FifamBeg::_2ndLeg) || superCup->mRounds[0].mFlags.Check(FifamBeg::WithReplay)) ? 2 : 1;

                        Vector<Pair<UInt, UInt>> availableMatchdays;
                        for (UInt i = 1; i < supercupcc[s].size(); i++) {
                            if (supercupcc[s][i] > 0 && supercupcc[s][i] < 1000)
                                availableMatchdays.emplace_back(supercupcc[s][i], i);
                        }
                        if (availableMatchdays.size() < numMatches) {
                            Alert(AL_ERROR, L"Not enough available matches in calendar for SuperCup\nCountry: %s\nSeason: %d\nRequired matches: %d\nAvailable matches: %d",
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

    for (auto &po : playOffs) {
        UInt numPlayOffMatches = 0;
        if (po->mIsLeague) {
            if (po->mLeague.mFifamLeague)
                numPlayOffMatches = po->mLeague.mFifamLeague->GetNumMatchdays();
        }
        else {
            for (auto &r : po->mRounds) {
                if (r.mFifamRound && r.mFifamRound->GetDbType() == FifamCompDbType::Round)
                    numPlayOffMatches += r.mFifamRound->m1stLegFlags.Check(FifamBeg::_2ndLeg) ? 2 : 1;
            }
        }
        UInt dist = numPlayOffMatches > 4 ? 3 : 4;
        UInt matchDate = 331;
        if (po->mIsLeague) {
            if (po->mLeague.mFifamLeague && !po->mLeague.mFifamLeague->GetProperty<Bool>(L"custom_calendar", false)) {
                for (UInt i = 0; i < numPlayOffMatches; i++)
                    po->mLeague.mFifamLeague->mFirstSeasonMatchdays.push_back(matchDate + i * dist);
                po->mLeague.mFifamLeague->mSecondSeasonMatchdays = po->mLeague.mFifamLeague->mFirstSeasonMatchdays;
            }
        }
        else {
            for (auto &r : po->mRounds) {
                if (r.mFifamRound && !r.mFifamRound->GetProperty<Bool>(L"custom_calendar", false)) {
                    if (r.mFifamRound->m1stLegFlags.Check(FifamBeg::_2ndLeg)) {
                        r.mFifamRound->mFirstSeasonMatchdays[0] = matchDate;
                        r.mFifamRound->mFirstSeasonMatchdays[1] = matchDate + dist;
                        matchDate += dist * 2;
                    }
                    else {
                        r.mFifamRound->mFirstSeasonMatchdays[0] = matchDate;
                        matchDate += dist;
                    }
                    r.mFifamRound->mSecondSeasonMatchdays = r.mFifamRound->mFirstSeasonMatchdays;
                }
            }
        }
    }

    // autocorrect league calendars

    struct LeagueSpecialInfo {
        Bool mHasGroup = false;
        Set<FifamCompLeague *> mConnections;
    };

    Map<FifamCompLeague *, LeagueSpecialInfo> leagueSpecialInfo;

    for (auto &p : playOffs) {
        Set<FifamCompLeague *> playOffConnectedLeagues;
        if (p->mIsLeague) {
            for (auto &t : p->mLeague.mTeamEntries)
                playOffConnectedLeagues.insert(t.mLeague);
        } else {
            for (auto &r : p->mRounds) {
                for (auto &t : r.mTeamEntries)
                    playOffConnectedLeagues.insert(t.mLeague);
            }
        }
        if (playOffConnectedLeagues.size() > 1) {
            for (FifamCompLeague *l1 : playOffConnectedLeagues) {
                for (FifamCompLeague *l2 : playOffConnectedLeagues) {
                    if (l1 != l2)
                        leagueSpecialInfo[l1].mConnections.insert(l2);
                }
            }
        }
    }

    if (!leagueSpecialInfo.empty()) {
        Vector<Vector<FifamCompLeague *>> groups;

        Function<void(FifamCompLeague *, FifamCompLeague *)> ProcessLeague = [&](FifamCompLeague *base, FifamCompLeague *current) {
            if (!leagueSpecialInfo[current].mHasGroup) {
                if (current == base)
                    groups.resize(groups.size() + 1);
                groups.back().push_back(current);
                leagueSpecialInfo[current].mHasGroup = true;
                for (auto l : leagueSpecialInfo[current].mConnections)
                    ProcessLeague(base, l);
            }
        };

        for (auto &[l, i] : leagueSpecialInfo)
            ProcessLeague(l, l);

        for (auto &g : groups) {
            if (g.size() > 1) {
                //String leaguesInGroup;
                //for (auto &l : g)
                //    leaguesInGroup += FifamTr(l->mName) + L" ";
                //Error(L"Group in %s: %s", countryId.ToCStr(), leaguesInGroup.c_str());
                UInt latestMatchDay[2] = { 0, 0 };
                for (auto &l : g) {
                    if (!l->mFirstSeasonMatchdays.empty() && l->mFirstSeasonMatchdays.back() > latestMatchDay[0])
                        latestMatchDay[0] = l->mFirstSeasonMatchdays.back();
                    if (!l->mSecondSeasonMatchdays.empty() && l->mSecondSeasonMatchdays.back() > latestMatchDay[1])
                        latestMatchDay[1] = l->mSecondSeasonMatchdays.back();
                }
                for (auto &l : g) {
                    if (!l->mFirstSeasonMatchdays.empty())
                        l->mFirstSeasonMatchdays.back() = latestMatchDay[0];
                    if (!l->mSecondSeasonMatchdays.empty())
                        l->mSecondSeasonMatchdays.back() = latestMatchDay[1];
                }
            }
        }
    }

    return true;
}
