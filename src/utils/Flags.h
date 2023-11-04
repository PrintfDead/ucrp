#include <stdio.h>
#include <functional>
#include <fmt/format.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "../game/Player.h"
#include "../managers/PlayerManager.h"
#include "Constants.h"

#ifndef FLAGS_H
#define FLAGS_H

class AdminFlag {
public:
    int admin_level;
    std::function<int(int)> error_message;
};

struct FLAG_User : public AdminFlag {
    FLAG_User() {
        admin_level = 0;
        error_message = [](int playerid) -> int {
            SendClientMessage(playerid, COLOR_WHITE, fmt::format("* Probablemente ocurrio un error.").c_str());
            return 1;
        };
    }
};

struct FLAG_Helper : public AdminFlag {
    FLAG_Helper() {
        admin_level = 1;
        error_message = [](int playerid) -> int {
            SendClientMessage(playerid, COLOR_WHITE, fmt::format("* No posees el nivel de administrador {} Helper {FFFFFF}para poder ejecutar este comando.").c_str());
            return 1;
        };
    }
};

struct FLAG_Moderator : public AdminFlag {
    FLAG_Moderator() {
        admin_level = 2;
        error_message = [](int playerid) -> int {
            SendClientMessage(playerid, COLOR_WHITE, fmt::format("* No posees el nivel de administrador {} Moderator {FFFFFF}para poder ejecutar este comando.").c_str());
            return 1;
        };
    }
};

#endif // FLAGS_H