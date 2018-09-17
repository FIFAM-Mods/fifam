#include "FifamInstructionList.h"
#include "FifamUtils.h"
#include "FifamCompID.h"
#include "FifamDatabase.h"

FifamInstructionsList::FifamInstructionsList() {

}

FifamInstructionsList::FifamInstructionsList(FifamInstructionsList const &rhs) {
    operator=(rhs);
}

FifamInstructionsList &FifamInstructionsList::operator=(FifamInstructionsList const &rhs) {
    stack.resize(rhs.stack.size());
    for (UInt i = 0; i < rhs.stack.size(); i++)
        stack[i] = rhs.stack[i]->Clone();
    return *this;
}

UInt FifamInstructionsList::Size() {
    return stack.size();
}

Bool FifamInstructionsList::Empty() {
    return stack.empty();
}

void FifamInstructionsList::Clear() {
    stack.clear();
}

void FifamInstructionsList::PushBack(FifamAbstractInstruction *instruction) {
    stack.push_back(instruction);
}

void FifamInstructionsList::Insert(FifamAbstractInstruction *instruction, Int position) {
    if (position == -1)
        PushBack(instruction);
    else
        stack.insert(stack.begin() + position, instruction);
}

Bool FifamInstructionsList::Contains(FifamInstructionID instructionID) {
    return std::any_of(stack.begin(), stack.end(), [=](FifamAbstractInstruction *instruction) {
        return instruction->GetID() == instructionID;
    });
}

UInt FifamInstructionsList::Count(FifamInstructionID instructionID) {
    return std::count_if(stack.begin(), stack.end(), [=](FifamAbstractInstruction *instruction) {
        return instruction->GetID() == instructionID;
    });
}

FifamAbstractInstruction *FifamInstructionsList::FindFirstOccurence(FifamInstructionID instructionID) {
    for (UInt i = 0; i < stack.size(); i++) {
        if (stack[i]->GetID() == instructionID)
            return stack[i];
    }
    return nullptr;
}

Int FifamInstructionsList::FindFirstOccurencePosition(FifamInstructionID instructionID) {
    for (UInt i = 0; i < stack.size(); i++) {
        if (stack[i]->GetID() == instructionID)
            return i;
    }
    return -1;
}

void FifamInstructionsList::ForAll(Function<void(FifamAbstractInstruction *)> callback) {
    for (UInt i = 0; i < stack.size(); i++)
        callback(stack[i]);
}

void FifamInstructionsList::ForAll(Function<void(FifamAbstractInstruction *, UInt)> callback) {
    for (UInt i = 0; i < stack.size(); i++)
        callback(stack[i], i);
}

template<typename T = UInt>
T IntParam(Vector<String> const &params, UInt index) {
    return params.size() > index ? Utils::SafeConvertInt<T>(params[index]) : 0;
}

String StrParam(Vector<String> const &params, UInt index) {
    return params.size() > index ? params[index] : String();
}

FifamCompetition *CompParam(Vector<String> const &params, UInt index, FifamNation nationId) {
    if (params.size() > index) {
        FifamCompID compID;
        compID.SetFromStr(params[index], FifamCompRegion::MakeFromInt(nationId.ToInt()));
        return reinterpret_cast<FifamCompetition *>(compID.ToInt());
    }
    return nullptr;
}

FifamCompID CompIdParam(Vector<String> const &params, UInt index, FifamNation nationId) {
    FifamCompID compID;
    if (params.size() > index)
        compID.SetFromStr(params[index], FifamCompRegion::MakeFromInt(nationId.ToInt()));
    return compID;
}

FifamNation CountryParam(Vector<String> const &params, UInt index, UInt gameId) {
    FifamNation result;
    if (params.size() > index) {
        result.SetFromInt(FifamUtils::GetConvertedRegion(Utils::SafeConvertInt<UChar>(params[index]), gameId,
            FifamDatabase::LATEST_GAME_VERSION));
    }
    return result;
}

FifamContinent ContinentParam(Vector<String> const &params, UInt index) {
    FifamContinent result;
    if (params.size() > index)
        result.SetFromInt(Utils::SafeConvertInt<UChar>(params[index]));
    return result;
}

FifamClubTeamType TeamTypeParam(Vector<String> const &params, UInt index) {
    FifamClubTeamType result;
    if (params.size() > index)
        result.SetFromInt(Utils::SafeConvertInt<UChar>(params[index]));
    return result;
}

void FifamInstructionsList::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
    Clear();
    UInt numInstructions = reader.ReadLine<UInt>();
    for (UInt i = 0; i < numInstructions; i++) {
        auto firstLine = reader.ReadFullLine();
        for (UInt i = 0; i < firstLine.length(); i++) {
            if (firstLine[i] == L'{' || firstLine[i] == L'}')
                firstLine[i] = L'"';
        }
        auto p = Utils::Split(firstLine, L',');
        FifamInstructionID instructionID = StrParam(p, 0);
        if (instructionID.GetWasSetFromUnknown()) {
            reader.SkipLines(2);
            continue;
        }
        else if (instructionID == FifamInstructionID::ID_END_OF_ENTRY)
            break;
        if (instructionID == FifamInstructionID::ID_BUILD_COUNTER)
            PushBack(new FifamInstruction::BUILD_COUNTER(IntParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS)
            PushBack(new FifamInstruction::RESERVE_ASSESSMENT_TEAMS(IntParam(p, 1), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_BUILD_UEFA5)
            PushBack(new FifamInstruction::BUILD_UEFA5(IntParam(p, 1), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_GET_CHAMP)
            PushBack(new FifamInstruction::GET_CHAMP(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS)
            PushBack(new FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS(IntParam(p, 1), IntParam(p, 2), IntParam(p, 3)));
        else if (instructionID == FifamInstructionID::ID_GET_UEFA5_TAB)
            PushBack(new FifamInstruction::GET_UEFA5_TAB(IntParam(p, 1), IntParam(p, 2), IntParam(p, 3)));
        else if (instructionID == FifamInstructionID::ID_FILL_ASSESSMENT_RESERVES)
            PushBack(new FifamInstruction::FILL_ASSESSMENT_RESERVES());
        else if (instructionID == FifamInstructionID::ID_GET_UEFA5_SURE_TAB)
            PushBack(new FifamInstruction::GET_UEFA5_SURE_TAB(IntParam(p, 1), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP)
            PushBack(new FifamInstruction::GET_CHAMP_OR_RUNNER_UP(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_UEFA5_SURE_UIC)
            PushBack(new FifamInstruction::GET_UEFA5_SURE_UIC(IntParam(p, 1), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_GET_TAB_X_TO_Y) {
            auto p2 = reader.ReadLine<UShort>();
            auto p3 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_TAB_X_TO_Y(CompParam(p, 1, nationId), p2, p3));
        }
        else if (instructionID == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z) {
            auto p2 = reader.ReadLine<UChar>();
            auto p3 = reader.ReadLine<UChar>();
            auto p4 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_TAB_SURE_X_TO_Y_Z(CompParam(p, 1, nationId), p2, p3, p4));
        }
        else if (instructionID == FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y) {
            auto p1 = reader.ReadLine<UChar>();
            auto p2 = reader.ReadLine<UShort>();
            auto p3 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_TAB_LEVEL_X_TO_Y(p1, p2, p3));
        }
        else if (instructionID == FifamInstructionID::ID_GET_TAB_SPARE)
            PushBack(new FifamInstruction::GET_TAB_SPARE());
        else if (instructionID == FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y) {
            auto p1 = reader.ReadLine<UChar>();
            auto p2 = reader.ReadLine<UShort>();
            auto p3 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y(p1, p2, p3));
        }
        else if (instructionID == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER)
            PushBack(new FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER(IntParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST) {
            auto compID = CompIdParam(p, 2, nationId);
            PushBack(new FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST(IntParam(p, 1), compID.mType, compID.mIndex));
        }
        else if (instructionID == FifamInstructionID::ID_GET_WINNER)
            PushBack(new FifamInstruction::GET_WINNER(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_LOSER)
            PushBack(new FifamInstruction::GET_LOSER(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_POOL) {
            auto p2 = reader.ReadLine<UShort>();
            auto p3 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_POOL(CompParam(p, 1, nationId), p2, p3));
        }
        else if (instructionID == FifamInstructionID::ID_GET_NAT_UEFA5_WITH_HOST)
            PushBack(new FifamInstruction::GET_NAT_UEFA5_WITH_HOST(IntParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_GET_NAT_UEFA5_WITHOUT_HOST)
            PushBack(new FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST(IntParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA)
            PushBack(new FifamInstruction::GET_NAT_SOUTH_AMERICA(CountryParam(p, 1, reader.GetGameId())));
        else if (instructionID == FifamInstructionID::ID_GET_NAT_AMERICA)
            PushBack(new FifamInstruction::GET_NAT_AMERICA());
        else if (instructionID == FifamInstructionID::ID_GET_NAT_AFRICA)
            PushBack(new FifamInstruction::GET_NAT_AFRICA());
        else if (instructionID == FifamInstructionID::ID_GET_NAT_ASIA)
            PushBack(new FifamInstruction::GET_NAT_ASIA());
        else if (instructionID == FifamInstructionID::ID_GET_NAT_OCEANIA)
            PushBack(new FifamInstruction::GET_NAT_OCEANIA());
        else if (instructionID == FifamInstructionID::ID_GET_HOST)
            PushBack(new FifamInstruction::GET_HOST());
        else if (instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y) {
            FifamNation p1;
            p1.SetFromInt(FifamUtils::GetConvertedRegion(CompIdParam(p, 1, nationId).mRegion.ToInt(), reader.GetGameId(), FifamDatabase::LATEST_GAME_VERSION));
            auto p2 = reader.ReadLine<UChar>();
            auto p3 = reader.ReadLine<UShort>();
            auto p4 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y(p1, p2, p3, p4));
        }
        else if (instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE) {
            FifamNation p1;
            p1.SetFromInt(FifamUtils::GetConvertedRegion(CompIdParam(p, 1, nationId).mRegion.ToInt(), reader.GetGameId(), FifamDatabase::LATEST_GAME_VERSION));
            auto p2 = reader.ReadLine<UInt>();
            PushBack(new FifamInstruction::GET_INTERNATIONAL_SPARE(p1, p2));
        }
        else if (instructionID == FifamInstructionID::ID_GET_RUNNER_UP)
            PushBack(new FifamInstruction::GET_RUNNER_UP(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR) {
            auto p1 = reader.ReadLine<UChar>();
            auto p2 = reader.ReadLine<UShort>();
            auto p3 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_TAB_LEVEL_INDOOR(p1, p2, p3));
        }
        else if (instructionID == FifamInstructionID::ID_GET_RELEGATED_TEAMS)
            PushBack(new FifamInstruction::GET_RELEGATED_TEAMS(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS)
            PushBack(new FifamInstruction::GET_INTERNATIONAL_TEAMS(CountryParam(p, 1, reader.GetGameId()), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_GET_CC_FA_WINNER)
            PushBack(new FifamInstruction::GET_CC_FA_WINNER(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_CC_SPARE)
            PushBack(new FifamInstruction::GET_CC_SPARE());
        else if (instructionID == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM)
            PushBack(new FifamInstruction::GET_CHAMP_COUNTRY_TEAM(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM)
            PushBack(new FifamInstruction::GET_RANDOM_NATIONAL_TEAM(ContinentParam(p, 1), IntParam(p, 2)));
        else if (instructionID == FifamInstructionID::ID_CHANGE_TEAM_TYPES)
            PushBack(new FifamInstruction::CHANGE_TEAM_TYPES(TeamTypeParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_GET_FAIRNESS_TEAM)
            PushBack(new FifamInstruction::GET_FAIRNESS_TEAM(IntParam(p, 1)));
        else if (instructionID == FifamInstructionID::ID_COPY_LEAGUE_DATA)
            PushBack(new FifamInstruction::COPY_LEAGUE_DATA(CompParam(p, 1, nationId)));
        else if (instructionID == FifamInstructionID::ID_GET_NATIONAL_TEAM)
            PushBack(new FifamInstruction::GET_NATIONAL_TEAM(CountryParam(p, 1, reader.GetGameId())));
        else if (instructionID == FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST)
            PushBack(new FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST(CountryParam(p, 1, reader.GetGameId())));
        else if (instructionID == FifamInstructionID::ID_SHUFFLE_TEAMS)
            PushBack(new FifamInstruction::SHUFFLE_TEAMS());
    }
}

struct InstructionWriteableData {
    Bool mWriteable = true;
    Vector<String> mWarnings;
    Vector<String> mErrors;
};

void WriteInstruction(FifamAbstractInstruction *instruction, Vector<UInt> additionalPparams, InstructionWriteableData &writeableData) {

}



void FifamInstructionsList::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
    UInt gameId = writer.GetGameId();
    Vector<InstructionWriteableData> writeableData(Size());
    bool addBuildCounterInstruction = false;
    if (gameId <= 7 && !Contains(FifamInstructionID::ID_BUILD_COUNTER)) {
        ForAll([&](FifamAbstractInstruction *instruction, UInt index) {
            auto id = instruction->GetID();
            if (id == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS ||
                id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS ||
                id == FifamInstructionID::ID_FILL_ASSESSMENT_RESERVES ||
                id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER)
            {
                addBuildCounterInstruction = true;
            }
        });
    }
    // Disable instructions with null or unknown competitions
    ForAllCompetitionLinks([&](FifamCompetition *&competition, UInt index, FifamAbstractInstruction *instruction) {
        if (!competition) {
            writeableData[index].mWriteable = false;
            writeableData[index].mErrors.push_back(L"Competition is not available");
        }
        else {
            if (!competition->GetWriteableID()) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"Competition (original ID %s) is not available (disabled)", competition->GetCompIDStr().c_str()));
            }
            if (nationId != FifamNation::None) {
                if (competition->mID.mRegion != FifamCompRegion::None && competition->mID.mRegion != nationId.ToInt()) {
                    writeableData[index].mWriteable = false;
                    writeableData[index].mErrors.push_back(
                        Utils::Format(L"Can refer only to country competitions in country script", competition->GetCompIDStr().c_str()));
                }
            }
        }
    });
    // Disable instructions with Montenegro or 53rd assessment position for FM07
    if (gameId <= 7) {
        ForAllCountryLinks([&](FifamNation &countryId, UInt index, FifamAbstractInstruction *instruction) {
            if (countryId == FifamNation::Montenegro) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"This country (%s) is not available (disabled)", countryId.ToCStr()));
            }
        });
        ForAllAssessmentPositions([&](UInt &assessmentTablePosition, UInt index, FifamAbstractInstruction *instruction) {
            if (assessmentTablePosition == 53) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"This assessment position (%d) is not available", assessmentTablePosition));
            }
        });
    }
    // Disable instructions with disabled league level
    if (nationId != FifamNation::None && database->mCountries[nationId.ToInt()]) {
        ForAllLeagueLevels([&](UInt &leagueLevel, UInt index, FifamAbstractInstruction *instruction) {
            if (leagueLevel <= database->mCountries[nationId.ToInt()]->GetNumWriteableLeagueLevels()) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"This league level (%d) is not available (disabled)", leagueLevel));
            }
        });
    }
    bool prevInstructionGetUefa5SureTab = false;
    ForAll([&](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        // GET_UEFA5_SURE_TAB can't be used since FM08, but can be replaced with FILL_ASSESSMENT_RESERVES
        if (gameId >= 8) {
            if (id == FifamInstructionID::ID_GET_UEFA5_SURE_TAB) {
                if (prevInstructionGetUefa5SureTab)
                    writeableData[index].mWriteable = false;
                else
                    prevInstructionGetUefa5SureTab = true;
            }
            else
                prevInstructionGetUefa5SureTab = false;
        }
        // BUILD_COUNTER is not needed since FM08
        if (id == FifamInstructionID::ID_BUILD_COUNTER) {
            if (gameId >= 8)
                writeableData[index].mWriteable = false;
        }
        // GET_UEFA5_SURE_UIC can't be used since FM08 and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_UIC) {
            if (gameId >= 8)
                writeableData[index].mWriteable = false;
        }
        // GET_TAB_LEVEL_INDOOR can' be used since FM10 and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR) {
            if (gameId >= 10)
                writeableData[index].mWriteable = false;
        }
        // GET_CHAMP_OR_RUNNER_UP can't be used in FM12 and earlier, but can be replaced with GET_CHAMP
        // (but only if GET_CHAMP is not already present in the script)
        else if (id == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP) {
            if (gameId <= 12 && Contains(FifamInstructionID::ID_GET_CHAMP))
                writeableData[index].mWriteable = false;
        }
        // GET_UEFA5_CHAMP_OR_FINALIST can't be used since FM08, but can be replaced with GET_EUROPEAN_ASSESSMENT_CUPWINNER
        // (but only if GET_UEFA5_CHAMP_OR_FINALIST gets the team from FA_CUP competition)
        else if (id == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST) {
            if (gameId >= 8 && ((FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *)instruction)->mCompType != FifamCompType::FaCup)
                writeableData[index].mWriteable = false;
        }
        // GET_RUNNER_UP can' be used in FM09 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_RUNNER_UP) {
            if (gameId <= 9)
                writeableData[index].mWriteable = false;
        }
        // GET_CHAMP_COUNTRY_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM) {
            if (gameId <= 7)
                writeableData[index].mWriteable = false;
        }
        // GET_RANDOM_NATIONAL_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM) {
            if (gameId <= 7)
                writeableData[index].mWriteable = false;
        }
        // CHANGE_TEAM_TYPES can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_CHANGE_TEAM_TYPES) {
            if (gameId <= 7)
                writeableData[index].mWriteable = false;
        }
        // GET_FAIRNESS_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_FAIRNESS_TEAM) {
            if (gameId <= 7)
                writeableData[index].mWriteable = false;
        }
        // COPY_LEAGUE_DATA can' be used in FM08 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA) {
            if (gameId <= 8)
                writeableData[index].mWriteable = false;
        }
        // SHUFFLE_TEAMS can' be used in FM12 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_SHUFFLE_TEAMS) {
            if (gameId <= 12)
                writeableData[index].mWriteable = false;
        }
    });
    if (gameId <= 11) {

    }
    else {

    }
    writer.WriteLine(0);
    //writer.WriteLine(size());
    //for (UInt i = 0; i < size(); i++)
    //    mInstructions[i].Write(writer);
}

void FifamInstructionsList::ForAllCompetitionLinks(Function<void(FifamCompetition *&, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt id) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_CHAMP)
            callback(((FifamInstruction::GET_CHAMP *)instruction)->mCompetition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP)
            callback(((FifamInstruction::GET_CHAMP_OR_RUNNER_UP *)instruction)->mCompetition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_X_TO_Y *)instruction)->mLeague, id, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z)
            callback(((FifamInstruction::GET_TAB_SURE_X_TO_Y_Z *)instruction)->mLeague, id, instruction);
        else if (id == FifamInstructionID::ID_GET_WINNER)
            callback(((FifamInstruction::GET_WINNER *)instruction)->mRound, id, instruction);
        else if (id == FifamInstructionID::ID_GET_LOSER)
            callback(((FifamInstruction::GET_LOSER *)instruction)->mRound, id, instruction);
        else if (id == FifamInstructionID::ID_GET_POOL)
            callback(((FifamInstruction::GET_POOL *)instruction)->mPool, id, instruction);
        else if (id == FifamInstructionID::ID_GET_RUNNER_UP)
            callback(((FifamInstruction::GET_RUNNER_UP *)instruction)->mCompetition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_RELEGATED_TEAMS)
            callback(((FifamInstruction::GET_RELEGATED_TEAMS *)instruction)->mLeague, id, instruction);
        else if (id == FifamInstructionID::ID_GET_CC_FA_WINNER)
            callback(((FifamInstruction::GET_CC_FA_WINNER *)instruction)->mCompetition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM)
            callback(((FifamInstruction::GET_CHAMP_COUNTRY_TEAM *)instruction)->mCompetition, id, instruction);
        else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA)
            callback(((FifamInstruction::COPY_LEAGUE_DATA *)instruction)->mLeague, id, instruction);
    });
}

void FifamInstructionsList::ForAllCountryLinks(Function<void(FifamNation &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt id) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA)
            callback(((FifamInstruction::GET_NAT_SOUTH_AMERICA *)instruction)->mCountryId, id, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *)instruction)->mCountryId, id, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE)
            callback(((FifamInstruction::GET_INTERNATIONAL_SPARE *)instruction)->mCountryId, id, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS)
            callback(((FifamInstruction::GET_INTERNATIONAL_TEAMS *)instruction)->mCountryId, id, instruction);
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM)
            callback(((FifamInstruction::GET_NATIONAL_TEAM *)instruction)->mCountryId, id, instruction);
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST)
            callback(((FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST *)instruction)->mCountryId, id, instruction);
    });
}

void FifamInstructionsList::ForAllAssessmentPositions(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt id) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS)
            callback(((FifamInstruction::RESERVE_ASSESSMENT_TEAMS *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_BUILD_UEFA5)
            callback(((FifamInstruction::BUILD_UEFA5 *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS)
            callback(((FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_TAB)
            callback(((FifamInstruction::GET_UEFA5_TAB *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_TAB)
            callback(((FifamInstruction::GET_UEFA5_SURE_TAB *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_UIC)
            callback(((FifamInstruction::GET_UEFA5_SURE_UIC *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER)
            callback(((FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST)
            callback(((FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITH_HOST)
            callback(((FifamInstruction::GET_NAT_UEFA5_WITH_HOST *)instruction)->mAssessmentPosition, id, instruction);
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITHOUT_HOST)
            callback(((FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST *)instruction)->mAssessmentPosition, id, instruction);
    });
}

void FifamInstructionsList::ForAllLeagueLevels(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt id) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_LEVEL_X_TO_Y *)instruction)->mLeagueLevel, id, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y *)instruction)->mLeagueLevel, id, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *)instruction)->mLeagueLevel, id, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR)
            callback(((FifamInstruction::GET_TAB_LEVEL_INDOOR *)instruction)->mLeagueLevel, id, instruction);
    });
}
