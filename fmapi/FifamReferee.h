#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

class FifamCountry;

// @since FM07
class FifamReferee {
public:
    // @since FM07
    // @maxsize 19
    String mFirstName;

    // @since FM07
    // @maxsize 19
    String mLastName;

    // @since FM07
    enum class Type : Char {
        None,
        WorldClassReferee,
        SeverePerson,
        DoesNotInterfereMuchInTheGame,
        BraveDecisionMaker,
        Coward,
        DoesNotLikeManagersInterfering
    } mType = Type::None;

    FifamCountry *mCountry = nullptr;

private:
    const Type m07TypeIds[7] = {
        Type::WorldClassReferee,
        Type::SeverePerson,
        Type::DoesNotInterfereMuchInTheGame,
        Type::DoesNotLikeManagersInterfering,
        Type::BraveDecisionMaker,
        Type::Coward,
        Type::None
    };

    const Type m09TypeIds[6] = {
        Type::None,
        Type::WorldClassReferee,
        Type::SeverePerson,
        Type::DoesNotInterfereMuchInTheGame,
        Type::BraveDecisionMaker,
        Type::Coward
    };

public:
    FifamReferee();
    FifamReferee(FifamReferee const &rhs);
    FifamReferee(String const &firstName, String const &lastName, Type type);
    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
