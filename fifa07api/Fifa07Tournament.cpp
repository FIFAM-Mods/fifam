#include "Fifa07Tournament.h"
#include <sstream>

unsigned int Fifa07Tournament::GetId() {
    return rtsg;
}

void Fifa07Tournament::Parse(Fifa07DataFile::Line &line) {
    line >> rtsg >> asset_id >> country_id >> tournament_type >> start_next_year >> start_month >> start_week >> end_next_year >> end_month >> end_week >> monday >> tuesday >> wednesday >> thursday >> friday >> saturday >> sunday >> stage_tournament_index >> qualify_from_this_year >> is_play_off_tournament >> num_yellows_for_red >> num_subs >> appears_in_tournament >> appears_in_career >> num_games_per_team >> use_away_goal_rule;
}

Fifa07Tournament::Fifa07Tournament(Fifa07DataFile::Line &line) {
    Parse(line);
}
