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

FifamCompID CompParam(Vector<String> const &params, UInt index, FifamNation nationId) {
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

String CompStr(FifamCompID const &competition, UInt gameId) {
    String result = L"{}";
    if (!competition.IsNull()) {
        FifamCompID compID;
        UInt writeableID = FifamUtils::GetWriteableID(competition, gameId);
        if (writeableID != 0)
            compID.SetFromInt(writeableID);
        else
            compID = competition;
        result = Utils::Format(L"{ %d, %s, %d }", compID.mRegion.ToInt(), compID.mType.ToCStr(), compID.mIndex);
    }
    return result;
}

void FifamInstructionsList::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
    Clear();
    UInt numInstructions = reader.ReadLine<UInt>();
    for (UInt i = 0; i < numInstructions; i++) {
        auto firstLine = reader.ReadFullLine();
        for (UInt i = 0; i < firstLine.length(); i++) {
            if (firstLine[i] == L';') {
                firstLine = firstLine.substr(0, i);
                break;
            }
            if (firstLine[i] == L'{' || firstLine[i] == L'}')
                firstLine[i] = L'"';
        }
        auto p = Utils::Split(firstLine, L',');
        FifamInstructionID instructionID = StrParam(p, 0);
        if (instructionID.GetWasSetFromUnknown()) {
            Error(L"Set from unknown: %s", StrParam(p, 0).c_str());
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
        else if (instructionID == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST)
            PushBack(new FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST(IntParam(p, 1), CompParam(p, 0, nationId)));
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
            p1.SetFromInt(FifamUtils::GetConvertedRegion(CompParam(p, 1, nationId).mRegion.ToInt(), reader.GetGameId(), FifamDatabase::LATEST_GAME_VERSION));
            auto p2 = reader.ReadLine<UChar>();
            auto p3 = reader.ReadLine<UShort>();
            auto p4 = reader.ReadLine<UChar>();
            PushBack(new FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y(p1, p2, p3, p4));
        }
        else if (instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE) {
            FifamNation p1;
            p1.SetFromInt(FifamUtils::GetConvertedRegion(CompParam(p, 1, nationId).mRegion.ToInt(), reader.GetGameId(), FifamDatabase::LATEST_GAME_VERSION));
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
    Bool mNotSupported = false;
    Vector<String> mWarnings;
    Vector<String> mErrors;
};

void WriteInstruction(FifamWriter &writer, FifamInstructionID instructionID, InstructionWriteableData &writeableData,
    Vector<UInt> inlineParams, Vector<UInt> additionalPparams, String compIdParam = L"", UInt compIdIndex = 0)
{
    for (auto const &err : writeableData.mErrors)
        writer.WriteLine(L";; ERROR: " + err);
    for (auto const &warn : writeableData.mWarnings)
        writer.WriteLine(L";; WARNING: " + warn);
    if (!writeableData.mWriteable)
        writer.Write(L"; ");
    writer.Write(instructionID.ToCStr());
    if (inlineParams.size() > 0) {
        for (UInt i = 0; i < inlineParams.size(); i++) {
            writer.Write(L", ");
            if (!compIdParam.empty() && i == compIdIndex)
                writer.Write(compIdParam + L", ");
            writer.Write(inlineParams[i]);
        }
        if (instructionID == FifamInstructionID::ID_GET_NATIONAL_TEAM
            || instructionID == FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST
            || instructionID == FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA
            || instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS)
        {
            if (inlineParams[0] >= 1 && inlineParams[0] <= 207) {
                if (inlineParams[0] >= 100)
                    writer.Write(L" ; ");
                else if (inlineParams[0] >= 10)
                    writer.Write(L"  ; ");
                else
                    writer.Write(L"   ; ");
            #ifdef UCP_EXTENSIONS
                if (inlineParams[0] == 207)
                    writer.Write(L"Kosovo");
                else
                    writer.Write(FifamNation::MakeFromInt(inlineParams[0]).ToStr());
            #else
                writer.Write(FifamNation::MakeFromInt(inlineParams[0]).ToStr());
            #endif
            }
        }
        else if (instructionID == FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM) {
            if (inlineParams[0] >= 0 && inlineParams[0] <= 5) {
                writer.Write(L" ; ");
                writer.Write(FifamContinent::MakeFromInt(inlineParams[0]).ToStr());
            }
        }
        else if (instructionID == FifamInstructionID::ID_CHANGE_TEAM_TYPES) {
            if ((inlineParams[0] >= 0 && inlineParams[0] <= 2) || inlineParams[0] == 4) {
                writer.Write(L" ; ");
                writer.Write(FifamClubTeamType::MakeFromInt(inlineParams[0]).ToStr());
            }
        }
    }
    else if (!compIdParam.empty()) {
        writer.Write(L", " + compIdParam);
        if (instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y
            || instructionID == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE)
        {
            writer.Write(L" ;");
            FifamCompID compId;
            compId.SetFromStr(compIdParam);
            if (!compId.IsNull() && (compId.mRegion.ToInt() >= 1 && compId.mRegion.ToInt() <= 207)) {
                writer.Write(L" ");
                writer.Write(FifamNation::MakeFromInt(compId.mRegion.ToInt()).ToStr());
            }
        }
    }
    
    writer.WriteNewLine();
    for (UInt param : additionalPparams) {
        if (!writeableData.mWriteable)
            writer.Write(L"; ");
        writer.WriteLine(param);
    }
}

void FifamInstructionsList::Write(FifamWriter &writer, FifamDatabase *database, FifamCompDbType compDbType, FifamNation nationId, Bool useEndOfEntry) {
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
            if (gameId >= 8) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_TAB_LEVEL_INDOOR can' be used since FM10 and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR) {
            if (gameId >= 10) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_CHAMP_OR_RUNNER_UP can't be used in FM12 and earlier, but can be replaced with GET_CHAMP
        // (but only if GET_CHAMP is not already present in the script)
        else if (id == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP) {
            if (gameId <= 12 && Contains(FifamInstructionID::ID_GET_CHAMP)) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_UEFA5_CHAMP_OR_FINALIST can't be used since FM08, but can be replaced with GET_EUROPEAN_ASSESSMENT_CUPWINNER
        // (but only if GET_UEFA5_CHAMP_OR_FINALIST gets the team from FA_CUP competition)
        else if (id == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST) {
            if (gameId >= 8) {
                auto getUefa5ChampOrFinalist = ((FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *)instruction);
                if (getUefa5ChampOrFinalist->mCompetition.mType != FifamCompType::FaCup || getUefa5ChampOrFinalist->mCompetition.mIndex != 0) {
                    writeableData[index].mWriteable = false;
                    writeableData[index].mNotSupported = true;
                    writeableData[index].mErrors.push_back(L"This instruction is not supported");
                }
            }
        }
        // GET_RUNNER_UP can' be used in FM09 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_RUNNER_UP) {
            if (gameId <= 9) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_CHAMP_COUNTRY_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM) {
            if (gameId <= 7) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_RANDOM_NATIONAL_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM) {
            if (gameId <= 7) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // CHANGE_TEAM_TYPES can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_CHANGE_TEAM_TYPES) {
            if (gameId <= 7) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // GET_FAIRNESS_TEAM can' be used in FM07, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_GET_FAIRNESS_TEAM) {
            if (gameId <= 7) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // COPY_LEAGUE_DATA can' be used in FM08 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA) {
            if (gameId <= 8 || (gameId <= 12 && nationId != FifamNation::None)) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
        // SHUFFLE_TEAMS can' be used in FM12 and earlier, and can't be replaced by any other command
        else if (id == FifamInstructionID::ID_SHUFFLE_TEAMS) {
            if (gameId <= 12) {
                writeableData[index].mWriteable = false;
                writeableData[index].mNotSupported = true;
                writeableData[index].mErrors.push_back(L"This instruction is not supported");
            }
        }
    });
    // Disable instructions with null or unknown competitions
    ForAllCompetitionLinks([&](FifamCompID &competition, UInt index, FifamAbstractInstruction *instruction) {
        if (competition.IsNull()) {
            writeableData[index].mWriteable = false;
            writeableData[index].mErrors.push_back(L"Competition is not available");
        }
        else {
            if (!FifamUtils::GetWriteableID(competition, writer.GetGameId())) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"Competition (original ID %s) is not available (disabled)", competition.ToStr().c_str()));
            }
            if (nationId != FifamNation::None) {
                if (competition.mRegion != FifamCompRegion::None && competition.mRegion != nationId.ToInt()) {
                    writeableData[index].mWriteable = false;
                    writeableData[index].mErrors.push_back(
                        Utils::Format(L"Can refer only to country competitions in country script", competition.ToStr().c_str()));
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
    if (nationId != FifamNation::None && database->mCountries[nationId.ToInt() - 1]) {
        ForAllLeagueLevels([&](UInt &leagueLevel, UInt index, FifamAbstractInstruction *instruction) {
            if (leagueLevel >= database->mCountries[nationId.ToInt() - 1]->GetNumWriteableLeagueLevels()) {
                writeableData[index].mWriteable = false;
                writeableData[index].mErrors.push_back(
                    Utils::Format(L"This league level (%d) is not available (disabled)", leagueLevel));
            }
        });
    }
    // Validate instructions
    ForAll([&](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (!writeableData[index].mNotSupported) {
            if (gameId <= 11) {
                if (compDbType == FifamCompDbType::Pool) {
                    if (id == FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y ||
                        id == FifamInstructionID::ID_GET_CC_FA_WINNER)
                    {
                        writeableData[index].mWarnings.push_back(
                            Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                    }
                }
                else if (compDbType == FifamCompDbType::League) {
                    if (id != FifamInstructionID::ID_GET_TAB_X_TO_Y &&
                        id != FifamInstructionID::ID_GET_TAB_SPARE &&
                        id != FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y &&
                        id != FifamInstructionID::ID_GET_POOL &&
                        id != FifamInstructionID::ID_GET_CC_SPARE &&
                        id != FifamInstructionID::ID_CHANGE_TEAM_TYPES &&
                        id != FifamInstructionID::ID_COPY_LEAGUE_DATA)
                    {
                        writeableData[index].mWarnings.push_back(
                            Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                    }
                }
                else if (compDbType == FifamCompDbType::Round) {
                    if (id == FifamInstructionID::ID_GET_TAB_SPARE) {
                        if (gameId <= 10) {
                            writeableData[index].mWarnings.push_back(
                                Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                        }
                    }
                    else if (id != FifamInstructionID::ID_GET_CHAMP &&
                        id != FifamInstructionID::ID_GET_TAB_X_TO_Y &&
                        id != FifamInstructionID::ID_GET_WINNER &&
                        id != FifamInstructionID::ID_GET_LOSER &&
                        id != FifamInstructionID::ID_GET_POOL &&
                        id != FifamInstructionID::ID_GET_RUNNER_UP &&
                        id != FifamInstructionID::ID_CHANGE_TEAM_TYPES)
                    {
                        writeableData[index].mWarnings.push_back(
                            Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                    }
                }
                else if (compDbType == FifamCompDbType::Cup) {
                    if (id != FifamInstructionID::ID_GET_CHAMP &&
                        id != FifamInstructionID::ID_GET_TAB_X_TO_Y &&
                        id != FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y &&
                        id != FifamInstructionID::ID_GET_TAB_SPARE  &&
                        id != FifamInstructionID::ID_GET_RUNNER_UP  &&
                        id != FifamInstructionID::ID_GET_CC_FA_WINNER)
                    {
                        writeableData[index].mWarnings.push_back(
                            Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                    }
                }
            }
            else {
                if (compDbType != FifamCompDbType::League) {
                    if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA) {
                        writeableData[index].mWarnings.push_back(
                            Utils::Format(L"This instruction has no effect inside %s", compDbType.ToCStr()));
                    }
                }
                if (gameId >= 12 && id == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z)
                    writeableData[index].mWarnings.push_back(L"This instruction may not work as expected");
            }
            FifamCompetition *compToCheck = nullptr;
            FifamCompDbType compDbTypeToCheck;
            if (id == FifamInstructionID::ID_GET_TAB_X_TO_Y) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_TAB_X_TO_Y *)instruction)->mLeague);
                compDbTypeToCheck = FifamCompDbType::League;
            }
            else if (id == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_TAB_SURE_X_TO_Y_Z *)instruction)->mLeague);
                compDbTypeToCheck = FifamCompDbType::League;
            }
            else if (id == FifamInstructionID::ID_GET_WINNER) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_WINNER *)instruction)->mRound);
                compDbTypeToCheck = FifamCompDbType::Round;
            }
            else if (id == FifamInstructionID::ID_GET_LOSER) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_LOSER *)instruction)->mRound);
                compDbTypeToCheck = FifamCompDbType::Round;
            }
            else if (id == FifamInstructionID::ID_GET_POOL) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_POOL *)instruction)->mPool);
                compDbTypeToCheck = FifamCompDbType::Pool;
            }
            else if (id == FifamInstructionID::ID_GET_RELEGATED_TEAMS) {
                compToCheck = database->GetCompetition(((FifamInstruction::GET_RELEGATED_TEAMS *)instruction)->mLeague);
                compDbTypeToCheck = FifamCompDbType::League;
            }
            else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA) {
                compToCheck = database->GetCompetition(((FifamInstruction::COPY_LEAGUE_DATA *)instruction)->mLeague);
                compDbTypeToCheck = FifamCompDbType::League;
            }
            if (compToCheck && compToCheck->GetDbType() != compDbTypeToCheck) {
                writeableData[index].mWarnings.push_back(
                    Utils::Format(L"Competition must be a %s", compDbTypeToCheck.ToCStr()));
            }
        }
    });
    UInt numInstructionsToWrite = addBuildCounterInstruction? 1 : 0;
    Set<UInt> assessmentFillTable;
    ForAll([&](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (writeableData[index].mWriteable) {
            if (gameId <= 7) {
                if (id == FifamInstructionID::ID_FILL_ASSESSMENT_RESERVES) {
                    numInstructionsToWrite += assessmentFillTable.size() + 1;
                    assessmentFillTable.clear();
                    return;
                }
                else if (id == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS)
                    assessmentFillTable.insert(((FifamInstruction::RESERVE_ASSESSMENT_TEAMS *)instruction)->mAssessmentPosition);
            }
            numInstructionsToWrite++;
        }
    });
    if (useEndOfEntry)
        writer.WriteLine(Utils::Max(255u, numInstructionsToWrite));
    else
        writer.WriteLine(numInstructionsToWrite);
    assessmentFillTable.clear();
    if (addBuildCounterInstruction)
        writer.WriteLine(L"BUILD_COUNTER, 53");
    ForAll([&](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_BUILD_COUNTER) {
            auto i = (FifamInstruction::BUILD_COUNTER *)instruction;
            if (gameId <= 7)
                WriteInstruction(writer, id, writeableData[index], { i->mNumAssessmentPositions }, {});
        }
        else if (id == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS) {
            auto i = (FifamInstruction::RESERVE_ASSESSMENT_TEAMS *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->mNumOfReservedSpaces }, {});
            else {
                WriteInstruction(writer, FifamInstructionID::ID_BUILD_UEFA5, writeableData[index], { i->mAssessmentPosition, i->mNumOfReservedSpaces }, {});
                if (writeableData[index].mWriteable)
                    assessmentFillTable.insert(i->mAssessmentPosition);
            }
        }
        else if (id == FifamInstructionID::ID_BUILD_UEFA5) {
            auto i = (FifamInstruction::BUILD_UEFA5 *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS, writeableData[index], { i->mAssessmentPosition, i->mNumOfReservedSpaces }, {});
            else
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->mNumOfReservedSpaces }, {});
        }
        else if (id == FifamInstructionID::ID_GET_CHAMP) {
            auto i = (FifamInstruction::GET_CHAMP *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS) {
            auto i = (FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->mLeagueStartPosition, i->mNumTeams }, {});
            else
                WriteInstruction(writer, FifamInstructionID::ID_GET_UEFA5_TAB, writeableData[index], { i->mAssessmentPosition, i->mLeagueStartPosition, i->mNumTeams }, {});
        }
        else if (id == FifamInstructionID::ID_GET_UEFA5_TAB) {
            auto i = (FifamInstruction::GET_UEFA5_TAB *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS, writeableData[index], { i->mAssessmentPosition, i->mLeagueStartPosition, i->mNumTeams }, {});
            else
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->mLeagueStartPosition, i->mNumTeams }, {});
        }
        else if (id == FifamInstructionID::ID_FILL_ASSESSMENT_RESERVES) {
            auto i = (FifamInstruction::FILL_ASSESSMENT_RESERVES *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, id, writeableData[index], {}, {});
            else {
                for (UInt assessment : assessmentFillTable)
                    writer.WriteLine(Utils::Format(L"GET_UEFA5_SURE_TAB, %d, 8", assessment));
                assessmentFillTable.clear();
                writer.WriteLine(L"BUILD_COUNTER, 53");
            }
        }
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_TAB) {
            auto i = (FifamInstruction::GET_UEFA5_SURE_TAB *)instruction;
            if (gameId >= 8) {
                if (writeableData[index].mWriteable)
                    writer.WriteLine(L"FILL_ASSESSMENT_RESERVES");
            }
            else
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->unknown1 }, {});
        }
        else if (id == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP) {
            auto i = (FifamInstruction::GET_CHAMP_OR_RUNNER_UP *)instruction;
            if (gameId >= 13)
                WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
            else if (writeableData[index].mWriteable)
                WriteInstruction(writer, FifamInstructionID::ID_GET_CHAMP, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
            else
                WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_UIC) {
            auto i = (FifamInstruction::GET_UEFA5_SURE_UIC *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition, i->unknown1 }, {});
        }
        else if (id == FifamInstructionID::ID_GET_TAB_X_TO_Y) {
            auto i = (FifamInstruction::GET_TAB_X_TO_Y *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mLeagueStartPosition, i->mNumTeams }, CompStr(i->mLeague, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z) {
            auto i = (FifamInstruction::GET_TAB_SURE_X_TO_Y_Z *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mLeagueStartPosition, i->mLeagueMaxPosition, i->mNumTeams }, CompStr(i->mLeague, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y) {
            auto i = (FifamInstruction::GET_TAB_LEVEL_X_TO_Y *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mLeagueLevel, i->mLeagueStartPosition, i->mNumTeamsFromEachLeague });
        }
        else if (id == FifamInstructionID::ID_GET_TAB_SPARE) {
            auto i = (FifamInstruction::GET_TAB_SPARE *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER) {
            auto i = (FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER *)instruction;
            if (gameId >= 8)
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition }, {});
            else
                WriteInstruction(writer, FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST, writeableData[index], { i->mAssessmentPosition }, {}, L"{0, FA_CUP, 0}", 1);
        }
        else if (id == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST) {
            auto i = (FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *)instruction;
            if (gameId >= 8) {
                if (writeableData[index].mWriteable)
                    WriteInstruction(writer, FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER, writeableData[index], { i->mAssessmentPosition }, {});
                else
                    WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition }, {}, CompStr(i->mCompetition, writer.GetGameId()), 1);
            }
            else
                WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition }, {}, CompStr(i->mCompetition, writer.GetGameId()), 1);
        }
        else if (id == FifamInstructionID::ID_GET_WINNER) {
            auto i = (FifamInstruction::GET_WINNER *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mRound, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_LOSER) {
            auto i = (FifamInstruction::GET_LOSER *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mRound, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_POOL) {
            auto i = (FifamInstruction::GET_POOL *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mPoolStartPosition, i->mNumTeams }, CompStr(i->mPool, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITH_HOST) {
            auto i = (FifamInstruction::GET_NAT_UEFA5_WITH_HOST *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition }, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITHOUT_HOST) {
            auto i = (FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mAssessmentPosition }, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA) {
            auto i = (FifamInstruction::GET_NAT_SOUTH_AMERICA *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mCountryId.ToInt() }, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_AMERICA) {
            auto i = (FifamInstruction::GET_NAT_AMERICA *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_AFRICA) {
            auto i = (FifamInstruction::GET_NAT_AFRICA *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_ASIA) {
            auto i = (FifamInstruction::GET_NAT_ASIA *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_NAT_OCEANIA) {
            auto i = (FifamInstruction::GET_NAT_OCEANIA *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_HOST) {
            auto i = (FifamInstruction::GET_HOST *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y) {
            auto i = (FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mLeagueLevel, i->mLeagueStartPosition, i->mNumTeamsFromEachLeague }, FifamCompID(i->mCountryId.ToInt(), FifamCompType::League, 0).ToStr());
        }
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE) {
            auto i = (FifamInstruction::GET_INTERNATIONAL_SPARE *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mNumTeams }, FifamCompID(i->mCountryId.ToInt(), FifamCompType::League, 0).ToStr());
        }
        else if (id == FifamInstructionID::ID_GET_RUNNER_UP) {
            auto i = (FifamInstruction::GET_RUNNER_UP *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR) {
            auto i = (FifamInstruction::GET_TAB_LEVEL_INDOOR *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, { i->mLeagueLevel, i->mLeagueStartPosition, i->mNumTeamsFromEachLeague });
        }
        else if (id == FifamInstructionID::ID_GET_RELEGATED_TEAMS) {
            auto i = (FifamInstruction::GET_RELEGATED_TEAMS *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mLeague, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS) {
            auto i = (FifamInstruction::GET_INTERNATIONAL_TEAMS *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mCountryId.ToInt(), i->mNumTeams }, {});
        }
        else if (id == FifamInstructionID::ID_GET_CC_FA_WINNER) {
            auto i = (FifamInstruction::GET_CC_FA_WINNER *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_CC_SPARE) {
            auto i = (FifamInstruction::GET_CC_SPARE *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
        else if (id == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM) {
            auto i = (FifamInstruction::GET_CHAMP_COUNTRY_TEAM *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mCompetition, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM) {
            auto i = (FifamInstruction::GET_RANDOM_NATIONAL_TEAM *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mContinentId.ToInt(), i->mNumTeams }, {});
        }
        else if (id == FifamInstructionID::ID_CHANGE_TEAM_TYPES) {
            auto i = (FifamInstruction::CHANGE_TEAM_TYPES *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mNewTeamType.ToInt() }, {});
        }
        else if (id == FifamInstructionID::ID_GET_FAIRNESS_TEAM) {
            auto i = (FifamInstruction::GET_FAIRNESS_TEAM *)instruction;
            WriteInstruction(writer, id, writeableData[index], { i->mNumTeams }, {});
        }
        else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA) {
            auto i = (FifamInstruction::COPY_LEAGUE_DATA *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {}, CompStr(i->mLeague, writer.GetGameId()));
        }
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM) {
            auto i = (FifamInstruction::GET_NATIONAL_TEAM *)instruction;
            if (gameId >= 13 || (gameId >= 12 && nationId == FifamNation::None))
                WriteInstruction(writer, id, writeableData[index], { i->mCountryId.ToInt() }, {});
            else
                WriteInstruction(writer, FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA, writeableData[index], { i->mCountryId.ToInt() }, {});
        }
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST) {
            auto i = (FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST *)instruction;
            if (gameId >= 13 || (gameId >= 12 && nationId == FifamNation::None))
                WriteInstruction(writer, id, writeableData[index], { i->mCountryId.ToInt() }, {});
            else
                WriteInstruction(writer, FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA, writeableData[index], { i->mCountryId.ToInt() }, {});
        }
        else if (id == FifamInstructionID::ID_SHUFFLE_TEAMS) {
            auto i = (FifamInstruction::SHUFFLE_TEAMS *)instruction;
            WriteInstruction(writer, id, writeableData[index], {}, {});
        }
    });
    if (useEndOfEntry)
        writer.WriteLine(L"END_OF_ENTRY");
}

void FifamInstructionsList::ForAllCompetitionLinks(Function<void(FifamCompID &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_CHAMP)
            callback(((FifamInstruction::GET_CHAMP *)instruction)->mCompetition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP)
            callback(((FifamInstruction::GET_CHAMP_OR_RUNNER_UP *)instruction)->mCompetition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_X_TO_Y *)instruction)->mLeague, index, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z)
            callback(((FifamInstruction::GET_TAB_SURE_X_TO_Y_Z *)instruction)->mLeague, index, instruction);
        else if (id == FifamInstructionID::ID_GET_WINNER)
            callback(((FifamInstruction::GET_WINNER *)instruction)->mRound, index, instruction);
        else if (id == FifamInstructionID::ID_GET_LOSER)
            callback(((FifamInstruction::GET_LOSER *)instruction)->mRound, index, instruction);
        else if (id == FifamInstructionID::ID_GET_POOL)
            callback(((FifamInstruction::GET_POOL *)instruction)->mPool, index, instruction);
        else if (id == FifamInstructionID::ID_GET_RUNNER_UP)
            callback(((FifamInstruction::GET_RUNNER_UP *)instruction)->mCompetition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_RELEGATED_TEAMS)
            callback(((FifamInstruction::GET_RELEGATED_TEAMS *)instruction)->mLeague, index, instruction);
        else if (id == FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM)
            callback(((FifamInstruction::GET_CHAMP_COUNTRY_TEAM *)instruction)->mCompetition, index, instruction);
        else if (id == FifamInstructionID::ID_COPY_LEAGUE_DATA)
            callback(((FifamInstruction::COPY_LEAGUE_DATA *)instruction)->mLeague, index, instruction);
    });
}

void FifamInstructionsList::ForAllCountryLinks(Function<void(FifamNation &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA)
            callback(((FifamInstruction::GET_NAT_SOUTH_AMERICA *)instruction)->mCountryId, index, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *)instruction)->mCountryId, index, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_SPARE)
            callback(((FifamInstruction::GET_INTERNATIONAL_SPARE *)instruction)->mCountryId, index, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS)
            callback(((FifamInstruction::GET_INTERNATIONAL_TEAMS *)instruction)->mCountryId, index, instruction);
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM)
            callback(((FifamInstruction::GET_NATIONAL_TEAM *)instruction)->mCountryId, index, instruction);
        else if (id == FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST)
            callback(((FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST *)instruction)->mCountryId, index, instruction);
    });
}

void FifamInstructionsList::ForAllAssessmentPositions(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS)
            callback(((FifamInstruction::RESERVE_ASSESSMENT_TEAMS *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_BUILD_UEFA5)
            callback(((FifamInstruction::BUILD_UEFA5 *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS)
            callback(((FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_TAB)
            callback(((FifamInstruction::GET_UEFA5_TAB *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_TAB)
            callback(((FifamInstruction::GET_UEFA5_SURE_TAB *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_SURE_UIC)
            callback(((FifamInstruction::GET_UEFA5_SURE_UIC *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER)
            callback(((FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST)
            callback(((FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITH_HOST)
            callback(((FifamInstruction::GET_NAT_UEFA5_WITH_HOST *)instruction)->mAssessmentPosition, index, instruction);
        else if (id == FifamInstructionID::ID_GET_NAT_UEFA5_WITHOUT_HOST)
            callback(((FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST *)instruction)->mAssessmentPosition, index, instruction);
    });
}

void FifamInstructionsList::ForAllLeagueLevels(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback) {
    ForAll([=](FifamAbstractInstruction *instruction, UInt index) {
        auto id = instruction->GetID();
        if (id == FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_LEVEL_X_TO_Y *)instruction)->mLeagueLevel, index, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y)
            callback(((FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y *)instruction)->mLeagueLevel, index, instruction);
        else if (id == FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y)
            callback(((FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *)instruction)->mLeagueLevel, index, instruction);
        else if (id == FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR)
            callback(((FifamInstruction::GET_TAB_LEVEL_INDOOR *)instruction)->mLeagueLevel, index, instruction);
    });
}
