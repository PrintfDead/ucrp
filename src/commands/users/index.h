#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <fmt/format.h>
#include "../../utils/Constants.h"
#include "../../game/Player.h"
#include "../../managers/CommandManager.h"
#include "../../utils/Flags.h"

static int RegisterUserCommands() {
    FLAG_User userFlag;

    std::vector<std::string> HiNames = {"hi", "hello"}; 

    CommandManager::RegisterCommand(HiNames, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        SendClientMessage(player->GetID(), COLOR_WHITE, "Hi!");

        return 1;
    });

    return 1;
}