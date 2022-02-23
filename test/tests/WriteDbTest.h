#pragma once
#include "FifamDbEnvironment.h"

class WriteDbTest {
public:
    static Bool IsCorrectTranslation(UChar const &val, UInt translation) {
        switch (translation) {
        case FifamTranslation::German:
            return val <= 3;
        case FifamTranslation::English:
            return val <= 1;
        case FifamTranslation::French:
            return val <= 4;
        case FifamTranslation::Spanish:
            return val <= 4;
        case FifamTranslation::Italian:
            return val <= 7;
        case FifamTranslation::Polish:
            return val == 0;
        }
        return false;
    }

    static void Check(FifamTrArray<UChar> &ary, FifamTrArray<String> const &namesAry, FifamWriter &w, String const &checkName, FifamTrArray<String> const &clubName, String const &countryName) {
        static Array<String, 6> translationName = {
            L"German",
            L"English",
            L"French",
            L"Spanish",
            L"Italian",
            L"Polish"
        };
        for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
            if (!IsCorrectTranslation(ary[i], i)) {
                w.WriteLineWithSeparator(L'\t', clubName[FifamTranslation::English], countryName,
                    translationName[i] + L" (" + std::to_wstring(i + 1) + L")", checkName, ary[i], clubName[i]);
                ary[i] = 0;
            }
        }
    }

    WriteDbTest() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        //FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = new FifamDatabase(13, "db2");
        FifamWriter w("articles_check.txt");
        w.WriteLineWithSeparator(L'\t', L"Club", L"Country", L"Language", L"Check", L"Value", L"ClubName");
        for (auto country : db->mCountries) {
            for (FifamClub *c : country->mClubs) {
                Check(c->mAbbreviationArticle, c->mAbbreviation, w, L"AbbreviationArticle (1)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mPlayerInTextArticle, c->mPlayerInText, w, L"PlayerInTextArticle (2)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mTermForTeam1Article, c->mTermForTeam1, w, L"TermForTeam1Article (3)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mTermForTeam2Article, c->mTermForTeam2, w, L"TermForTeam2Article (4)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mFanName1Article, c->mFanName1, w, L"FanName1Article (5)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mFanName2Article, c->mFanName2, w, L"FanName2Article (6)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mClubNameUsageInPhrase, c->mName, w, L"ClubNameUsageInPhrase (7)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mClubNameUsageInPhrase2, c->mName2, w, L"ClubNameUsageInPhrase2 (8)", c->mName, FifamTr(c->mCountry->mName));
            }
            {
                FifamClub *c = &country->mNationalTeam;
                Check(c->mAbbreviationArticle, c->mAbbreviation, w, L"AbbreviationArticle (1)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mPlayerInTextArticle, c->mPlayerInText, w, L"PlayerInTextArticle (2)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mTermForTeam1Article, c->mTermForTeam1, w, L"TermForTeam1Article (3)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mTermForTeam2Article, c->mTermForTeam2, w, L"TermForTeam2Article (4)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mFanName1Article, c->mFanName1, w, L"FanName1Article (5)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mFanName2Article, c->mFanName2, w, L"FanName2Article (6)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mClubNameUsageInPhrase, c->mName, w, L"ClubNameUsageInPhrase (7)", c->mName, FifamTr(c->mCountry->mName));
                Check(c->mClubNameUsageInPhrase2, c->mName2, w, L"ClubNameUsageInPhrase2 (8)", c->mName, FifamTr(c->mCountry->mName));
            }
        }
        db->Write(13, FifamDatabase::GetGameDbVersion(13), "database_fixed_articles");
    }
};
