#pragma once

#include "Fifa07Enums.h"
#include "Fifa07Country.h"
#include "Fifa07Date.h"
#include "Fifa07Team.h"
#include "Fifa07League.h"
#include "Fifa07Tournament.h"
#include "Fifa07Player.h"
#include "Fifa07Referee.h"
#include "Fifa07Stadium.h"
#include "Fifa07Formation.h"
#include <filesystem>

class Fifa07Database {
public:
    static int m_currentGameVersion;


    Fifa07Database(std::filesystem::path const &dir, int version = 7) {

    }
};
