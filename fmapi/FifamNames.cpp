#include "FifamNames.h"

String FifamNames::LimitName(String const &name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    UInt len = name.length();
    if (len <= maxSize)
        return name;
    if (maxSize == 1)
        return name.substr(0, 1);
    String s = name.substr(0, maxSize);
    {
        Int depth = 0;
        Int unclosedStart = -1;
        for (Int i = 0; i < (Int)s.size(); i++) {
            if (s[i] == L'(') {
                if (depth == 0)
                    unclosedStart = i;
                depth++;
            }
            else if (s[i] == L')') {
                if (depth > 0) {
                    depth--;
                    if (depth == 0)
                        unclosedStart = -1;
                }
            }
        }
        if (unclosedStart >= 0) {
            s = s.substr(0, unclosedStart);
            while (!s.empty() && s.back() == L' ')
                s.pop_back();
            return s;
        }
    }
    Bool midWord = iswalpha(name[maxSize]);
    Int pos = (Int)s.size() - 1;
    while (pos >= 0 && s[pos] == L' ') {
        pos--;
        midWord = false;
    }
    if (pos < 0)
        return String();
    if (s[pos] == L'.')
        return s.substr(0, pos + 1);
    if (Utils::IsPunctuationMark(s[pos]) && s[pos] != L'.') {
        while (pos >= 0 && Utils::IsPunctuationMark(s[pos]) && s[pos] != L'.')
            pos--;
        midWord = false;
        while (pos >= 0 && s[pos] == L' ')
            pos--;
        if (pos < 0)
            return String();
        if (s[pos] == L'.')
            return s.substr(0, pos + 1);
    }
    if (pos >= 0 && iswdigit(s[pos])) {
        while (pos >= 0 && iswdigit(s[pos]))
            pos--;
        midWord = false;
        while (pos >= 0 && s[pos] == L' ')
            pos--;
        if (pos < 0)
            return String();
    }
    if (!midWord)
        return s.substr(0, pos + 1);
    Int i = pos;
    while (i >= 0) {
        WideChar c = s[i];
        if (c == L' ') {
            while (i >= 0 && s[i] == L' ')
                i--;
            return (i >= 0) ? s.substr(0, i + 1) : String();
        }
        if (Utils::IsVowel(c)) {
            i--;
            continue;
        }
        if (iswdigit(c)) {
            while (i >= 0 && (iswdigit(s[i]) || iswalpha(s[i]))) i--;
            continue;
        }
        if (Utils::IsPunctuationMark(c) && c != L'.') { 
            i--;
            continue; 
        }
        if (c == L'.')
            return s.substr(0, i + 1);
        Bool oneLetterWord = (i == 0 || s[i - 1] == L' ' || s[i - 1] == L'.');
        if (oneLetterWord) {
            i--;
            while (i >= 0 && s[i] == L' ')
                i--;
            return (i >= 0) ? s.substr(0, i + 1) : String();
        }
        return s.substr(0, i + 1) + L'.';
    }
    return String();
}

Bool IsJunior(String const &word) {
    return word == L"Junior" || word == L"Júnior";
}

String TruncateWord(String const &word, UInt maxSize) {
    if (word.empty() || maxSize == 0)
        return String();
    if (word.size() <= maxSize) 
        return word;
    if (maxSize == 1)
        return word.substr(0, 1);
    auto hyphenPos = word.find(L'-');
    if (hyphenPos != String::npos && hyphenPos > 0) {
        String candidate = word.substr(0, 1) + L'.' + word.substr(hyphenPos);
        if (candidate.size() <= maxSize)
            return candidate;
    }
    for (Int i = (Int)maxSize - 2; i >= 0; i--) {
        if (!Utils::IsVowel(word[i]))
            return word.substr(0, i + 1) + L'.';
    }
    return word.substr(0, maxSize);
}

String JoinParts(std::vector<String> const &parts) {
    String result;
    for (auto const &p : parts)
        result += (result.empty() ? L"" : L" ") + p;
    return result;
}

String FifamNames::LimitPersonNameWithTruncation(String const &name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    if (name.size() <= maxSize)
        return name;
    auto parts = Utils::Split(name, L' ', true, true);
    if (parts.empty())
        return String();
    if (parts.size() == 1)
        return TruncateWord(parts[0], maxSize);
    for (UInt i = 0; i < (UInt)parts.size(); i++) {
        if (IsJunior(parts[i])) {
            parts[i] = L"J.";
            break;
        }
    }
    if (JoinParts(parts).size() <= maxSize)
        return JoinParts(parts);
    for (UInt i = 0; i < (UInt)parts.size() - 1; i++) {
        if (parts[i].size() <= 2)
            continue;
        auto hyphenPos = parts[i].find(L'-');
        if (hyphenPos != String::npos && hyphenPos > 0 &&
            parts[i].substr(0, hyphenPos).back() != L'.') {
            parts[i] = parts[i].substr(0, 1) + L'.' + parts[i].substr(hyphenPos);
            if (JoinParts(parts).size() <= maxSize)
                return JoinParts(parts);
        }
        parts[i] = parts[i].substr(0, 1) + L'.';
        if ((UInt)JoinParts(parts).size() <= maxSize)
            return JoinParts(parts);
    }
    Int truncIdx = (Int)parts.size() - 1;
    if (parts[truncIdx] == L"J." && truncIdx > 0)
        truncIdx--;
    String prefix, suffix;
    for (Int i = 0; i < truncIdx; i++)
        prefix += (prefix.empty() ? L"" : L" ") + parts[i];
    for (Int i = truncIdx + 1; i < (Int)parts.size(); i++)
        suffix += L" " + parts[i];
    UInt available = maxSize;
    if (!prefix.empty())
        available -= prefix.size() + 1;
    if (!suffix.empty())
        available -= suffix.size();

    if (available >= 2) {
        String truncated = TruncateWord(parts[truncIdx], available);
        if (!truncated.empty())
            return (prefix.empty() ? L"" : prefix + L' ') + truncated + suffix;
    }
    return TruncateWord(JoinParts(parts), maxSize);
}

String FifamNames::LimitPersonName(String const &name, UInt maxSize) {
    if (maxSize == 0)
        return String();
    UInt len = name.length();
    if (len <= maxSize)
        return name;
    auto nameParts = Utils::Split(name, L' ', true, true);
    if (nameParts.empty())
        return String();
    String result = nameParts.back();
    if (maxSize == 1)
        return result.substr(0, 1);
    if (result.size() <= maxSize) {
        if (nameParts.size() > 1) {
            for (Int i = (Int)nameParts.size() - 2; i >= 0; i--) {
                if (result.size() + 1 + nameParts[i].size() > maxSize)
                    break;
                result = nameParts[i] + L' ' + result;
            }
        }
        return result;
    }
    for (Int i = maxSize - 2; i >= 0; i--) {
        if (!Utils::IsVowel(result[i]))
            return result.substr(0, i + 1) + L'.';
    }
    return result.substr(0, maxSize);
}

String FifamNames::LimitClubName(String const &name, UInt maxSize, String const &cityName) {
    if (maxSize == 0)
        return String();
    if (name.length() <= maxSize)
        return name;

    // =========================================================================
    //  Local helpers
    // =========================================================================

    // Trim leading/trailing spaces in-place
    auto Trim = [](String &s) {
        size_t b = s.find_first_not_of(L' ');
        if (b == String::npos) { s.clear(); return; }
        size_t e = s.find_last_not_of(L' ');
        s = s.substr(b, e - b + 1);
    };

    // Collapse multiple consecutive spaces into one
    auto CollapseSpaces = [](String &s) {
        String out;
        out.reserve(s.size());
        bool prevSp = false;
        for (wchar_t c : s) {
            if (c == L' ') { if (!prevSp) out += c; prevSp = true; }
            else { out += c;              prevSp = false; }
        }
        s = out;
    };

    // Check whether a wide-string is an all-uppercase Latin abbreviation
    // of exactly len characters (only A-Z, no accented chars)
    auto IsAllCapsLatin = [](const String &w, int minL, int maxL) -> bool {
        int len = (int)w.size();
        if (len < minL || len > maxL) return false;
        for (wchar_t c : w)
            if (c < L'A' || c > L'Z') return false;
        return true;
    };

    // Check if a string contains at least one digit
    auto ContainsDigit = [](const String &w) -> bool {
        for (wchar_t c : w)
            if (iswdigit(c)) return true;
        return false;
    };

    // Case-insensitive wide-char comparison
    auto CIEq = [](const String &a, const String &b) -> bool {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++)
            if (towupper((wint_t)a[i]) != towupper((wint_t)b[i])) return false;
        return true;
    };

    // Replace all whole-word occurrences of 'from' with 'to' (case-insensitive)
    // "whole-word" = surrounded by space or string boundary
    auto ReplaceWholeWord = [&](String &s, const wchar_t *from, const wchar_t *to) {
        String f(from);
        size_t flen = f.size();
        size_t tlen = wcslen(to);
        size_t pos = 0;
        while (pos + flen <= s.size()) {
            bool match = true;
            for (size_t i = 0; i < flen && match; i++)
                match = (towupper((wint_t)s[pos + i]) == towupper((wint_t)f[i]));
            if (match) {
                bool leftOk = (pos == 0 || s[pos - 1] == L' ');
                bool rightOk = (pos + flen == s.size() || s[pos + flen] == L' ');
                if (leftOk && rightOk) {
                    s.replace(pos, flen, to);
                    pos += tlen;
                    continue;
                }
            }
            pos++;
        }
    };

    // Remove all whole-word occurrences of 'word' (case-insensitive),
    // eating one adjacent space so gaps don't accumulate.
    auto RemoveWholeWord = [&](String &s, const wchar_t *word) {
        String w(word);
        size_t wlen = w.size();
        size_t pos = 0;
        while (pos + wlen <= s.size()) {
            bool match = true;
            for (size_t i = 0; i < wlen && match; i++)
                match = (towupper((wint_t)s[pos + i]) == towupper((wint_t)w[i]));
            if (match) {
                bool leftOk = (pos == 0 || s[pos - 1] == L' ');
                bool rightOk = (pos + wlen == s.size() || s[pos + wlen] == L' ');
                if (leftOk && rightOk) {
                    size_t eStart = pos;
                    size_t eLen = wlen;
                    if (eStart > 0 && s[eStart - 1] == L' ') { eStart--; eLen++; }
                    else if (eStart + wlen < s.size() && s[eStart + wlen] == L' ') eLen++;
                    s.erase(eStart, eLen);
                    pos = (eStart > 0) ? eStart : 0;
                    continue;
                }
            }
            pos++;
        }
    };

    // Split string on spaces into a vector of tokens
    auto Split = [](const String &s) -> std::vector<String> {
        std::vector<String> v;
        String tok;
        for (wchar_t c : s) {
            if (c == L' ') { if (!tok.empty()) { v.push_back(tok); tok.clear(); } }
            else            tok += c;
        }
        if (!tok.empty()) v.push_back(tok);
        return v;
    };

    // Join vector of tokens with spaces
    auto Join = [](const std::vector<String> &v) -> String {
        String s;
        for (size_t i = 0; i < v.size(); i++) {
            if (i) s += L' ';
            s += v[i];
        }
        return s;
    };

    // Fits check
    auto Fits = [&](const String &s) -> bool { return s.size() <= maxSize; };

    // =========================================================================
    //  Working string
    // =========================================================================
    String s = name;

    // =========================================================================
    // STEP 1 – Remove content in square brackets: [anything]
    // =========================================================================
    {
        bool changed = true;
        while (changed) {
            changed = false;
            size_t ob = s.find(L'[');
            size_t cb = s.find(L']');
            if (ob != String::npos && cb != String::npos && cb > ob) {
                size_t eStart = ob;
                size_t eEnd = cb + 1;
                // eat one adjacent space
                if (eStart > 0 && s[eStart - 1] == L' ') eStart--;
                else if (eEnd < s.size() && s[eEnd] == L' ') eEnd++;
                s.erase(eStart, eEnd - eStart);
                changed = true;
            }
        }
    }
    Trim(s); CollapseSpaces(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 2 – Remove commas
    // =========================================================================
    for (size_t i = 0; i < s.size(); )
        s[i] == L',' ? (void)s.erase(i, 1) : (void)++i;
    Trim(s); CollapseSpaces(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 3 – Remove dots from abbreviations (e.g. F.C. => FC, U.S.A. => USA)
    //          Strategy: a space-separated token that matches (letter dot)+ is
    //          an abbreviation → strip all dots from it.
    // =========================================================================
    {
        auto IsAbbrToken = [](const String &tok) -> bool {
            // Must be non-empty and consist of alternating (alpha)(dot) pairs,
            // optionally ending without a final dot.
            if (tok.empty()) return false;
            size_t i = 0;
            while (i < tok.size()) {
                if (!iswalpha(tok[i])) return false;
                i++;
                if (i < tok.size()) {
                    if (tok[i] != L'.') return false;
                    i++;
                }
            }
            return true; // all groups matched
        };

        auto words = Split(s);
        for (auto &w : words) {
            if (IsAbbrToken(w)) {
                String stripped;
                for (wchar_t c : w)
                    if (c != L'.') stripped += c;
                w = stripped;
            }
        }
        s = Join(words);
    }
    Trim(s); CollapseSpaces(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 4 – Strip ( ) brackets; MARK the text that was encapsulated so we
    //          can remove it later (step 10).
    // =========================================================================
    String markedContent;
    {
        String out;
        size_t i = 0;
        while (i < s.size()) {
            if (s[i] == L'(') {
                size_t close = s.find(L')', i + 1);
                if (close != String::npos) {
                    String inner = s.substr(i + 1, close - i - 1);
                    Trim(inner);
                    if (!inner.empty()) {
                        if (!markedContent.empty()) markedContent += L' ';
                        markedContent += inner;
                    }
                    i = close + 1;
                    if (i < s.size() && s[i] == L' ') i++;
                    continue;
                }
                // unmatched '(' — just drop it
                i++;
                continue;
            }
            if (s[i] == L')') { i++; continue; }
            out += s[i++];
        }
        s = out;
    }
    Trim(s); CollapseSpaces(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 5 – Remove  '  and  &  characters
    // =========================================================================
    for (size_t i = 0; i < s.size(); )
        (s[i] == L'\'' || s[i] == L'&') ? (void)s.erase(i, 1) : (void)++i;
    Trim(s); CollapseSpaces(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 6 – Remove duplicate words (keep first occurrence, case-insensitive)
    // =========================================================================
    {
        auto words = Split(s);
        std::vector<String> unique;
        for (auto &w : words) {
            bool dup = false;
            for (auto &u : unique) if (CIEq(w, u)) { dup = true; break; }
            if (!dup) unique.push_back(w);
        }
        s = Join(unique);
    }
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 7 – Remove all-caps abbreviations of length 4→2
    //          (process larger sizes first; preserve II and III)
    // =========================================================================
    for (int abbLen = 4; abbLen >= 2; abbLen--) {
        auto words = Split(s);
        std::vector<String> kept;
        for (auto &w : words) {
            if (w == L"II" || w == L"III") { kept.push_back(w); continue; }
            if (IsAllCapsLatin(w, abbLen, abbLen)) continue;
            kept.push_back(w);
        }
        s = Join(kept);
        Trim(s); CollapseSpaces(s);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 8 – Textual substitutions (longer words first to avoid partial hits)
    //   Juniors/Junior → Jrs.   Athletic → Ath.   Atletic → Atl.
    //   Sportiva/Sportiv → Sp.  Academy → Ac.     United → Utd
    //   Utd. → Utd               Real → R.
    // =========================================================================
    ReplaceWholeWord(s, L"Juniors", L"Jrs.");
    ReplaceWholeWord(s, L"Junior", L"Jrs.");
    ReplaceWholeWord(s, L"Athletic", L"Ath.");
    ReplaceWholeWord(s, L"Atletic", L"Atl.");
    ReplaceWholeWord(s, L"Sportiva", L"Sp.");
    ReplaceWholeWord(s, L"Sportiv", L"Sp.");
    ReplaceWholeWord(s, L"Academy", L"Ac.");
    ReplaceWholeWord(s, L"United", L"Utd");
    // Utd. → Utd
    ReplaceWholeWord(s, L"Utd.", L"Utd");
    ReplaceWholeWord(s, L"Real", L"R.");
    CollapseSpaces(s); Trim(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 9 – Remove years (stand-alone 4-digit numbers)
    // =========================================================================
    {
        auto words = Split(s);
        std::vector<String> kept;
        for (auto &w : words) {
            if (w.size() == 4) {
                bool allDigit = true;
                for (wchar_t c : w) if (!iswdigit(c)) { allDigit = false; break; }
                if (allDigit) continue;
            }
            kept.push_back(w);
        }
        s = Join(kept);
        Trim(s); CollapseSpaces(s);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 10 – Remove the text previously captured from () blocks (step 4)
    // =========================================================================
    if (!markedContent.empty()) {
        for (auto &mw : Split(markedContent))
            RemoveWholeWord(s, mw.c_str());
        Trim(s); CollapseSpaces(s);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 11 – Remove Romance/Germanic filler words
    //   de del delle della degli di da do das
    //   la le el los las y e of the
    // =========================================================================
    {
        // Ordered: longer variants before shorter ones to avoid partial removal
        static const wchar_t *fillers[] = {
            L"delle", L"della", L"degli", L"dels",
            L"das", L"del", L"los", L"las",
            L"de", L"di", L"da", L"do",
            L"la", L"le", L"el",
            L"of", L"the",
            L"y", L"e",
            nullptr
        };
        for (int fi = 0; fillers[fi]; fi++) {
            RemoveWholeWord(s, fillers[fi]);
            Trim(s); CollapseSpaces(s);
            if (Fits(s)) return s;
        }
    }

    // =========================================================================
    // STEP 12 – Second round of abbreviation substitutions
    //   Jrs. → J.   Ath. → A.   Atl. → A.   Ac. → A.   Sp. → S.   Utd → U
    // =========================================================================
    ReplaceWholeWord(s, L"Jrs.", L"J.");
    ReplaceWholeWord(s, L"Ath.", L"A.");
    ReplaceWholeWord(s, L"Atl.", L"A.");
    ReplaceWholeWord(s, L"Ac.", L"A.");
    ReplaceWholeWord(s, L"Sp.", L"S.");
    ReplaceWholeWord(s, L"Utd", L"U");
    CollapseSpaces(s); Trim(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 13 – Words containing hyphen: truncate first segment to initial
    //           e.g. "Garmisch-Partenkirchen" → "G.-Partenkirchen"
    // =========================================================================
    {
        auto words = Split(s);
        for (auto &w : words) {
            size_t hyp = w.find(L'-');
            if (hyp != String::npos && hyp > 0) {
                String first = w.substr(0, hyp);
                String rest = w.substr(hyp + 1);
                if (first.size() > 1 && !ContainsDigit(first))
                    w = String(1, first[0]) + L".-" + rest;
            }
        }
        s = Join(words);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 14 – Words containing /: truncate first segment to initial
    //           e.g. "Garmisch/Partenkirchen" → "G./Partenkirchen"
    //           (done BEFORE replacing / with space)
    // =========================================================================
    {
        auto words = Split(s);
        for (auto &w : words) {
            size_t sl = w.find(L'/');
            if (sl != String::npos && sl > 0) {
                String first = w.substr(0, sl);
                String rest = w.substr(sl + 1);
                if (first.size() > 1 && !ContainsDigit(first))
                    w = String(1, first[0]) + L"./" + rest;
            }
        }
        s = Join(words);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 15 – Replace / with space
    // =========================================================================
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] == L'/') s[i] = L' ';
    CollapseSpaces(s); Trim(s);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 16 – Prepare the city name and remove "Club" if not the sole candidate
    // =========================================================================
    std::vector<String> words = Split(s);

    // Clean cityName: strip anything in parentheses
    String cleanCity = cityName;
    {
        size_t ob = cleanCity.find(L'(');
        if (ob != String::npos) cleanCity = cleanCity.substr(0, ob);
        Trim(cleanCity);
    }

    // A word in the club name matches the city if it equals the whole city string
    // (case-insensitive). Multi-word city names like "San Jose" are treated as a
    // single unit — only a word that exactly matches the full city string qualifies.
    auto IsCityWord = [&](const String &w) -> bool {
        return !cleanCity.empty() && CIEq(w, cleanCity);
    };

    // Helper: is this word a valid main-word candidate (ignoring city constraint)?
    auto IsCandidate = [&](const String &w) -> bool {
        return w.size() > 3 && !ContainsDigit(w);
    };

    // Remove "Club" if at least one other candidate exists
    {
        bool hasOtherCandidate = false;
        for (auto &w : words)
            if (!CIEq(w, L"Club") && IsCandidate(w)) { hasOtherCandidate = true; break; }
        if (hasOtherCandidate) {
            words.erase(
                std::remove_if(words.begin(), words.end(),
                    [&](const String &w) { return CIEq(w, L"Club"); }),
                words.end());
        }
        s = Join(words);
        if (Fits(s)) return s;
        words = Split(s); // refresh after possible removal
    }

    // =========================================================================
    // STEP 16b – Find the "main" word
    //            Walk right-to-left; first word with >3 chars and no digits
    //            that is NOT a city word.  If no such word exists, fall back
    //            and allow a city word to be main.
    // =========================================================================
    int mainIdx = -1;

    // Pass 1: prefer non-city candidates
    for (int i = (int)words.size() - 1; i >= 0; i--) {
        if (IsCandidate(words[i]) && !IsCityWord(words[i])) {
            mainIdx = i;
            break;
        }
    }
    // Pass 2: fall back to city-word candidates if nothing else qualifies
    if (mainIdx < 0) {
        for (int i = (int)words.size() - 1; i >= 0; i--) {
            if (IsCandidate(words[i])) {
                mainIdx = i;
                break;
            }
        }
    }

    // =========================================================================
    // STEP 16c – Replace non-main city words with their first letter (no dot)
    //            e.g. "Praha" → "P"  (only when Praha is not the main word)
    // =========================================================================
    for (int i = 0; i < (int)words.size(); i++) {
        if (i == mainIdx)             continue;
        if (!IsCityWord(words[i]))    continue;
        if (words[i].size() <= 1)     continue;
        words[i] = String(1, words[i][0]); // single letter, no dot
    }
    s = Join(words);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 17 – Abbreviate non-main words to "X." (capital + dot)
    //           Skip: words with ≤2 chars, words with digits, the main word.
    //           City words were already reduced to a single letter in step 16c,
    //           so they naturally fall into the ≤2 chars skip condition.
    // =========================================================================
    for (int i = 0; i < (int)words.size(); i++) {
        if (i == mainIdx)                 continue;
        if (words[i].size() <= 2)         continue;
        if (ContainsDigit(words[i]))      continue;
        words[i] = String(1, words[i][0]) + L'.';
    }
    s = Join(words);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 18 – If ≥2 abbreviated "X." words exist, merge consecutive runs
    //           e.g. "R. S. Gijon"    → "RS Gijon"
    //                "A. B. C. Madrid" → "ABC Madrid"
    //                "A. B. Madrid C." → "AB Madrid C."
    // =========================================================================
    {
        // Count single-letter abbreviations
        int abbCount = 0;
        for (auto &w : words)
            if (w.size() == 2 && iswalpha(w[0]) && w[1] == L'.') abbCount++;

        if (abbCount >= 2) {
            // Remember the main word's content so we can re-find it after merging
            String mainContent = (mainIdx >= 0 && mainIdx < (int)words.size())
                ? words[mainIdx] : String();

            std::vector<String> merged;
            size_t i = 0;
            while (i < words.size()) {
                // Detect start of a run of "X." tokens
                if (words[i].size() == 2 && iswalpha(words[i][0]) && words[i][1] == L'.') {
                    String run;
                    run += words[i][0];
                    size_t j = i + 1;
                    while (j < words.size() &&
                        words[j].size() == 2 && iswalpha(words[j][0]) && words[j][1] == L'.') {
                        run += words[j][0];
                        j++;
                    }
                    // Only merge if the run contains at least 2 letters
                    merged.push_back(run.size() >= 2 ? run : words[i]);
                    i = (run.size() >= 2) ? j : i + 1;
                }
                else {
                    merged.push_back(words[i++]);
                }
            }
            words = merged;

            // Re-locate mainIdx by matching the saved content
            mainIdx = -1;
            for (int i = 0; i < (int)words.size(); i++)
                if (words[i] == mainContent) { mainIdx = i; break; }
        }

        s = Join(words);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 19 – Progressively shorten the main word
    //           Each iteration: strip any trailing dot first (so we always make
    //           progress), then search right-to-left for the rightmost consonant
    //           that is NOT the last char of the stripped word.  Truncate there
    //           and re-attach a trailing dot.
    // =========================================================================
    if (mainIdx >= 0 && mainIdx < (int)words.size()) {
        String &main = words[mainIdx];

        while ((int)main.size() > 1) {
            // Strip trailing dot before every search so we never spin in place
            if (!main.empty() && main.back() == L'.') main.pop_back();
            if ((int)main.size() <= 1) break;

            // Search from second-to-last char backwards for a consonant
            bool truncated = false;
            for (Int i = (Int)main.size() - 2; i >= 0; i--) {
                if (!Utils::IsVowel(main[i]) &&
                    main[i] != L' ' &&
                    !Utils::IsPunctuationMark(main[i])) {
                    main = main.substr(0, i + 1) + L'.';
                    truncated = true;
                    break;
                }
            }
            // No consonant boundary found – just drop the last character
            if (!truncated) main.pop_back();

            s = Join(words);
            if (Fits(s)) return s;
        }
    }

    // Final check after all main-word truncation attempts
    s = Join(words);
    if (Fits(s)) return s;

    // =========================================================================
    // STEP 20 – Remove all dots
    // =========================================================================
    {
        String noDots;
        noDots.reserve(s.size());
        for (wchar_t c : s)
            if (c != L'.') noDots += c;
        s = noDots;
        CollapseSpaces(s); Trim(s);
        if (Fits(s)) return s;
    }

    // =========================================================================
    // STEP 21 – Hard truncation: cut to maxSize, then strip trailing junk
    //           (digits, spaces, hyphens) to avoid "Madrid-" or "Madrid 190"
    // =========================================================================
    s = s.substr(0, maxSize);
    Trim(s);
    while (!s.empty() && (s.back() == L' ' || s.back() == L'-' || iswdigit(s.back())))
        s.pop_back();
    Trim(s);
    return s;
}

String FifamNames::GetClubAbbr(String const &name) {
    auto nameNorm = Utils::GetStringWithoutUnicodeChars(name);
    if (nameNorm.size() == 4) {
        Bool is4LetterAbbr = true;
        for (auto c : nameNorm) {
            Bool isLetterOrNumber = (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
            if (!isLetterOrNumber) {
                is4LetterAbbr = false;
                break;
            }
        }
        if (is4LetterAbbr)
            return nameNorm;
    }
    String nameUpper = Utils::ToUpper(nameNorm);
    String abbr;
    UInt abbrSize = 0;
    for (auto &c : nameUpper) {
        if (c != L' ' && !Utils::IsPunctuationMark(c)) {
            abbr += c;
            abbrSize++;
            if (abbrSize == 3)
                break;
        }
    }
    return abbr;
}

FifamTrArray<String> FifamNames::TransformTrArray(FifamTrArray<String> const &ary, Function<String(String const &, UInt)> callback, UInt maxSize) {
    FifamTrArray<String> result;
    for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++)
        result[i] = callback(ary[i], maxSize);
    return result;
}

String FifamNames::FindCommonPrefix(Vector<String> const &strings, bool trim) {
    if (strings.empty())
        return String();
    String prefix;
    for (String::size_type i = 0; i < strings[0].size(); i++) {
        for (String::size_type j = 1; j < strings.size(); j++) {
            if (i >= strings[j].size() || strings[j][i] != strings[0][i]) {
                if (trim)
                    Utils::Trim(prefix);
                return prefix;
            }
        }
        prefix += strings[0][i];
    }
    if (trim)
        Utils::Trim(prefix);
    return prefix;
}

String FifamNames::GetLeagueLevelName(Vector<String> const &leagueNames, UInt level) {
    auto stripTrailingSeparators = [](String const &s) -> String {
        String::size_type pos = s.find_last_not_of(L" .-");
        return (pos != String::npos) ? s.substr(0, pos + 1) : String();
    };
    String levelName = stripTrailingSeparators(FindCommonPrefix(leagueNames, true));
    if (levelName.empty())
        return Utils::Format(L"League Level %d", level);
    auto words = Utils::Split(levelName, L' ', true, true);
    if (words.empty())
        return Utils::Format(L"League Level %d", level);
    auto lastWord = Utils::ToLower(words.back());
    if (lastWord == L"group" || lastWord == L"grupo" || lastWord == L"gr" || lastWord == L"gr." || lastWord == L"grp") {
        words.pop_back();
        levelName = stripTrailingSeparators(Utils::Join(words, L' '));
        if (!levelName.empty())
            return levelName;
    }
    else
        return levelName;
    return Utils::Format(L"League Level %d", level);
}

String FifamNames::GetPersonStringId(UInt gameId, String const &firstName, String const &lastName, String const &pseudonym, FifamDate const &birthday, UInt empicsId) {
    if (empicsId != 0 && gameId <= 9)
        return Utils::Format(L"emp-%d", empicsId);
    String nameId;
    if (!pseudonym.empty())
        nameId = FifamNames::LimitPersonName(pseudonym, gameId > 7 ? 29 : 19);
    else
        nameId = FifamNames::LimitPersonName(lastName, 19) + firstName.substr(0, 2);
    nameId = Utils::GetStringWithoutUnicodeChars(nameId);
    String finalName;
    for (WideChar c : nameId) {
        if (c != L' ' && c != L'.' && c != '_' && c != 0xA0) {
            switch (c) {
            case 0xD8:
                c = L'O';
                break;
            case 0xDE:
            case 0xFE:
                c = L'P';
                break;
            case 0xDF:
                c = L's';
                break;
            case 0xE6:
                c = L'a';
                break;
            case 0xF0:
            case 0xF8:
                c = L'o';
                break;
            case 0x110:
                c = L'D';
                break;
            case 0x111:
                c = L'd';
                break;
            case 0x131:
                c = L'i';
                break;
            case 0x141:
                c = L'L';
                break;
            case 0x142:
                c = L'l';
                break;
            case 0x153:
                c = L'c';
                break;
            case 0x2019:
                c = L'\'';
                break;
            case 0x21A:
                c = L'T';
                break;
            case 0x21B:
                c = L't';
                break;
            }
            finalName += c;
        }
    }
    if (!birthday.IsEmpty())
        finalName += Utils::Format(L"%02d%02d%04d", birthday.day, birthday.month, birthday.year);
    if (empicsId != 0)
        finalName += (gameId >= 13 ? L"-" : L"_") + Utils::Format(L"%d", empicsId);
    return finalName;
}
