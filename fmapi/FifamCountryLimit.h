#pragma once

// The extended FM13/UCP format. IDs 225..248 remain available for future additions.
constexpr unsigned FifamLegacyCountryCount = 207;
constexpr unsigned FifamCountryCount = 224;
constexpr unsigned FifamMaxCountryId = 248;
constexpr unsigned char FifamCountryDatabaseVersion = 0x13;
static_assert(FifamCountryCount <= FifamMaxCountryId, "Country IDs overlap competition regions");

struct FifamAddedCountry {
    wchar_t const *name;
    wchar_t const *abbr;
    unsigned continent, language1, language2, territory;
};
// Football regions: French Guiana is CONCACAF; Northern Mariana Islands is AFC.
// Name-pool languages are editable starter values, not new language IDs.
inline constexpr FifamAddedCountry FifamAddedCountries[] = {
    { L"Anguilla", L"AIA", 2, 14, 14, 3 },
    { L"Bonaire", L"BON", 2, 13, 13, 3 },
    { L"French Guiana", L"GUF", 2, 17, 17, 11 },
    { L"Guadeloupe", L"GLP", 2, 17, 17, 3 },
    { L"Martinique", L"MTQ", 2, 17, 17, 3 },
    { L"Saint Martin", L"MAF", 2, 17, 14, 3 },
    { L"Sint Maarten", L"SXM", 2, 13, 14, 3 },
    { L"Eritrea", L"ERI", 3, 3, 4, 1 },
    { L"Reunion", L"REU", 3, 17, 17, 1 },
    { L"South Sudan", L"SSD", 3, 14, 4, 1 },
    { L"Zanzibar", L"ZAN", 3, 27, 27, 1 },
    { L"Northern Mariana Islands", L"NMI", 4, 14, 14, 9 },
    { L"Timor-Leste", L"TLS", 4, 37, 32, 2 },
    { L"Kiribati", L"KIR", 5, 14, 14, 9 },
    { L"Tuvalu", L"TUV", 5, 14, 14, 9 },
    { L"Micronesia", L"FSM", 5, 14, 14, 9 },
    { L"Greenland", L"GRL", 2, 12, 14, 8 }
};
static_assert(sizeof(FifamAddedCountries) / sizeof(FifamAddedCountries[0]) == FifamCountryCount - 207);

