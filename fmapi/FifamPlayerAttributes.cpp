#include "FifamPlayerAttributes.h"

void FifamPlayerAttributes::Read(FifamReader &reader) {
    if (reader.IsVersionGreaterOrEqual(0x2013, 0x01)) {
        reader.ReadLine(
            BallControl,
            Dribbling,
            Finishing,
            ShotPower,
            LongShots,
            Volleys,
            Crossing,
            Passing,
            LongPassing,
            Header,
            TackleStanding,
            TackleSliding,
            ManMarking,
            Acceleration,
            Pace,
            Agility,
            Jumping,
            Strength,
            Stamina,
            Balance,
            PosOffensive,
            PosDefensive,
            Vision,
            Aggression,
            Reactions,
            Composure,
            Consistency,
            TacticAwareness,
            FreeKicks,
            Corners,
            PenaltyShot,
            Diving,
            Handling,
            Positioning,
            OneOnOne,
            Reflexes,
            Kicking
        );
    }
    else if (reader.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
        reader.ReadLine(
            ShotPower,
            LongShots,
            Dribbling,
            Technique,
            Crossing,
            Creativity,
            Finishing,
            FreeKicks,
            Corners,
            PenaltyShot,
            Passing,
            LongPassing,
            Flair,
            TacklingGeneral,
            ManMarking,
            Header,
            Touch,
            ForwardRuns,
            Acceleration,
            Pace,
            WorkRate,
            Strength,
            Agility,
            Jumping,
            Stamina,
            Anticipation,
            Aggression,
            Composure,
            Concentration,
            Decision,
            Determination,
            TeamWork,
            Consistency,
            Intimidation,
            Leadership,
            GkCrosses,
            Handling,
            Kicking,
            OneOnOne,
            Positioning,
            Punching,
            ShotStopping,
            Throwing
        );
    }
    else if (reader.IsVersionGreaterOrEqual(0x2006, 0x00)) {
        reader.ReadLine(ShotPower);
        reader.ReadLine(LongShots);
        reader.ReadLine(Dribbling);
        reader.ReadLine(Technique);
        reader.ReadLine(Crossing);
        reader.ReadLine(Creativity);
        reader.ReadLine(Finishing);
        reader.ReadLine(FreeKicks);
        reader.ReadLine(Corners);
        reader.ReadLine(PenaltyShot);
        reader.ReadLine(Passing);
        reader.ReadLine(LongPassing);
        reader.ReadLine(Flair);
        reader.ReadLine(TacklingGeneral);
        reader.ReadLine(ManMarking);
        reader.ReadLine(Header);
        reader.ReadLine(Touch);
        reader.ReadLine(ForwardRuns);
        reader.ReadLine(Acceleration);
        reader.ReadLine(Pace);
        reader.ReadLine(WorkRate);
        reader.ReadLine(Strength);
        reader.ReadLine(Agility);
        reader.ReadLine(Jumping);
        reader.ReadLine(Stamina);
        reader.ReadLine(Anticipation);
        reader.ReadLine(Aggression);
        reader.ReadLine(Composure);
        reader.ReadLine(Concentration);
        reader.ReadLine(Decision);
        reader.ReadLine(Determination);
        reader.ReadLine(TeamWork);
        reader.ReadLine(Consistency);
        reader.ReadLine(Intimidation);
        reader.ReadLine(Leadership);
        reader.ReadLine(GkCrosses);
        reader.ReadLine(Handling);
        reader.ReadLine(Kicking);
        reader.ReadLine(OneOnOne);
        reader.ReadLine(Positioning);
        reader.ReadLine(Punching);
        reader.ReadLine(ShotStopping);
        reader.ReadLine(Throwing);
    }
    else {
        
    }
}

void FifamPlayerAttributes::Write(FifamWriter &writer) {
    if (writer.IsVersionGreaterOrEqual(0x2013, 0x01)) {
        writer.WriteLine(
            BallControl,
            Dribbling,
            Finishing,
            ShotPower,
            LongShots,
            Volleys,
            Crossing,
            Passing,
            LongPassing,
            Header,
            TackleStanding,
            TackleSliding,
            ManMarking,
            Acceleration,
            Pace,
            Agility,
            Jumping,
            Strength,
            Stamina,
            Balance,
            PosOffensive,
            PosDefensive,
            Vision,
            Aggression,
            Reactions,
            Composure,
            Consistency,
            TacticAwareness,
            FreeKicks,
            Corners,
            PenaltyShot,
            Diving,
            Handling,
            Positioning,
            OneOnOne,
            Reflexes,
            Kicking
        );
    }
    else if (writer.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
        writer.WriteLine(
            ShotPower,
            LongShots,
            Dribbling,
            Technique,
            Crossing,
            Creativity,
            Finishing,
            FreeKicks,
            Corners,
            PenaltyShot,
            Passing,
            LongPassing,
            Flair,
            TacklingGeneral,
            ManMarking,
            Header,
            Touch,
            ForwardRuns,
            Acceleration,
            Pace,
            WorkRate,
            Strength,
            Agility,
            Jumping,
            Stamina,
            Anticipation,
            Aggression,
            Composure,
            Concentration,
            Decision,
            Determination,
            TeamWork,
            Consistency,
            Intimidation,
            Leadership,
            GkCrosses,
            Handling,
            Kicking,
            OneOnOne,
            Positioning,
            Punching,
            ShotStopping,
            Throwing
        );
    }
    else {
        writer.WriteLine(ShotPower);
        writer.WriteLine(LongShots);
        writer.WriteLine(Dribbling);
        writer.WriteLine(Technique);
        writer.WriteLine(Crossing);
        writer.WriteLine(Creativity);
        writer.WriteLine(Finishing);
        writer.WriteLine(FreeKicks);
        writer.WriteLine(Corners);
        writer.WriteLine(PenaltyShot);
        writer.WriteLine(Passing);
        writer.WriteLine(LongPassing);
        writer.WriteLine(Flair);
        writer.WriteLine(TacklingGeneral);
        writer.WriteLine(ManMarking);
        writer.WriteLine(Header);
        writer.WriteLine(Touch);
        writer.WriteLine(ForwardRuns);
        writer.WriteLine(Acceleration);
        writer.WriteLine(Pace);
        writer.WriteLine(WorkRate);
        writer.WriteLine(Strength);
        writer.WriteLine(Agility);
        writer.WriteLine(Jumping);
        writer.WriteLine(Stamina);
        writer.WriteLine(Anticipation);
        writer.WriteLine(Aggression);
        writer.WriteLine(Composure);
        writer.WriteLine(Concentration);
        writer.WriteLine(Decision);
        writer.WriteLine(Determination);
        writer.WriteLine(TeamWork);
        writer.WriteLine(Consistency);
        writer.WriteLine(Intimidation);
        writer.WriteLine(Leadership);
        writer.WriteLine(GkCrosses);
        writer.WriteLine(Handling);
        writer.WriteLine(Kicking);
        writer.WriteLine(OneOnOne);
        writer.WriteLine(Positioning);
        writer.WriteLine(Punching);
        writer.WriteLine(ShotStopping);
        writer.WriteLine(Throwing);
    }
}

bool FifamPlayerAttributes::IsSetPiecesAttribute(FifamPlayerAbilityID const & attrId) {
    return attrId == FifamPlayerAbilityID::FreeKicks ||
        attrId == FifamPlayerAbilityID::Corners ||
        attrId == FifamPlayerAbilityID::PenaltyShot;
}

bool FifamPlayerAttributes::IsGoalkeeperAttribute(FifamPlayerAbilityID const & attrId) {
    return attrId == FifamPlayerAbilityID::Diving ||
        attrId == FifamPlayerAbilityID::Reflexes ||
        attrId == FifamPlayerAbilityID::Handling ||
        attrId == FifamPlayerAbilityID::Positioning ||
        attrId == FifamPlayerAbilityID::OneOnOne ||
        attrId == FifamPlayerAbilityID::GoalKicks ||
        attrId == FifamPlayerAbilityID::Punching ||
        attrId == FifamPlayerAbilityID::ShotStopping ||
        attrId == FifamPlayerAbilityID::Throwing ||
        attrId == FifamPlayerAbilityID::GkCrosses;
}
