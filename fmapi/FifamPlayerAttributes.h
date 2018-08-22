#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

// @since FM07
class FifamPlayerAttributes {
public:
    UChar BallControl = 0; // FM13
    UChar Volleys = 0; // FM13
    UChar Dribbling = 0; // Influences the number of possible movements and also the chance of success
    UChar Finishing = 0;
    UChar ShotPower = 0; // Influences the strength of a shot
    UChar LongShots = 0; // Influences the accuracy of a shot on goal from more than 20 meters away
    UChar Crossing = 0;
    UChar Passing = 0;
    UChar LongPassing = 0;
    UChar Header = 0;
    UChar TacklingGeneral = 0; // FM07
    UChar TackleStanding = 0; // FM13
    UChar TackleSliding = 0; // FM13
    UChar ManMarking = 0;
    UChar Technique = 0; // FM07; Secondary value for all movements with the ball
    UChar Creativity = 0; // FM07
    UChar Flair = 0; // FM07
    UChar Touch = 0; // FM07

    // Physical skills

    UChar Balance = 0; // FM13
    UChar Acceleration = 0;
    UChar Pace = 0;
    UChar Agility = 0;
    UChar Jumping = 0;
    UChar Strength = 0;
    UChar Stamina = 0;
    UChar WorkRate = 0; // FM07
    UChar ForwardRuns = 0; // FM07

    // Mental skills

    UChar PosOffensive = 0; // FM13
    UChar PosDefensive = 0; // FM13
    UChar Vision = 0; // FM13
    UChar Reactions = 0; // FM13
    UChar TacticAwareness = 0; // FM13*
    UChar Aggression = 0;
    UChar Composure = 0;
    UChar Consistency = 0;
    UChar Leadership = 0; // FM07*
    UChar Anticipation = 0; // FM07
    UChar Concentration = 0; // FM07
    UChar Decision = 0; // FM07
    UChar Determination = 0; // FM07
    UChar TeamWork = 0; // FM07
    UChar Intimidation = 0; // FM07

    // Set pieces

    UChar FreeKicks = 0;
    UChar Corners = 0;
    UChar PenaltyShot = 0;

    // Goalkeeper skills

    UChar Diving = 0; // FM13
    UChar Reflexes = 0; // FM13
    UChar Handling = 0;
    UChar Positioning = 0;
    UChar OneOnOne = 0;
    UChar Kicking = 0;
    UChar Punching = 0; // FM07
    UChar ShotStopping = 0; // FM07
    UChar Throwing = 0; // FM07

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
