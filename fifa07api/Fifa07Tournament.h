#pragma once
#include <string>
#include <vector>
#include "Fifa07DataFile.h"
#include "Fifa07Propertied.h"

class Fifa07Tournament : public Fifa07Propertied {
public:
    int rtsg = 0;
    int asset_id = 0;
    int country_id = 0;
    int tournament_type = 0;
    int start_next_year = 0;
    int start_month = 0;
    int start_week = 0;
    int end_next_year = 0;
    int end_month = 0;
    int end_week = 0;
    int monday = 0;
    int tuesday = 0;
    int wednesday = 0;
    int thursday = 0;
    int friday = 0;
    int saturday = 0;
    int sunday = 0;
    int stage_tournament_index = 0;
    int qualify_from_this_year = 0;
    int is_play_off_tournament = 0;
    int num_yellows_for_red = 0;
    int num_subs = 0;
    int appears_in_tournament = 0;
    int appears_in_career = 0;
    int num_games_per_team = 0;
    int use_away_goal_rule = 0;

    struct TeamLookup {
        int rtsgt = 0;
        int team_id = 0;
        int move_from_rtsgt = 0;
        int move_to_rtsgt = 0;
        int qualify_from_rtsgt = 0;
        int num_wins = 0;
        int num_draws = 0;
        int num_losses = 0;
        int num_goals_for = 0;
        int num_goals_against = 0;
        int num_clean_sheets = 0;
        int num_red_cards = 0;
        int num_yellow_cards = 0;
    };

    std::vector<TeamLookup> teams;

    void Parse(Fifa07DataFile::Line &line);
    Fifa07Tournament() {}
    Fifa07Tournament(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
