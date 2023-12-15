#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <fmt/format.h>
#include "../../game/Player.h"
#include "../../game/Message.h"
#include "../../managers/CommandManager.h"
#include "../../utils/Flags.h"
#include "../../utils/Util.h"

static int RegisterChatCommands() {
    FLAG_User userFlag;

    CommandManager::RegisterCommand({"me"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { Message::Usage(player, "/me <accion>"); return 0; }

        
        std::string text = VectorToString(params);

        if (text.length() > 128) {
            Message::Usage(player, "/me <accion>");

            return 0;
        }

        if (text.length() > 64) {
            Message::Proximity(player, fmt::format("* {} {}...", player->ParseName(), text.substr(0, 64)), 20.0f, COLOR_EMOTE);
            Message::Proximity(player, fmt::format("...{}", text.substr(64)), 20.0f, COLOR_EMOTE);
        
            return 0;
        }

        Message::Proximity(player, fmt::format("* {} {}", player->ParseName(), text), 20.0f, COLOR_EMOTE);

        return 1;
    });

    CommandManager::RegisterCommand({"do"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { Message::Usage(player, "/do <accion>"); return 0; }

        std::string text = VectorToString(params);

        if (text.length() > 128) {
            Message::Usage(player, "/do <accion>");

            return 0;
        }

        if (text.length() > 64) {
            Message::Proximity(player, fmt::format("* {}...", text.substr(0, 64)), 20.0f, COLOR_LIGHTGREEN);
            Message::Proximity(player, fmt::format("* ...{} (( {} ))", text.substr(64), player->ParseName()), 20.0f, COLOR_LIGHTGREEN);
        
            return 0;
        }

        Message::Proximity(player, fmt::format("* {} (( {} ))", text, player->ParseName()), 20.0f, COLOR_LIGHTGREEN);

        return 1;
    });


    CommandManager::RegisterCommand({"b", "ooc"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) {
            Message::Usage(player, "/b <text>"); 
            return 0;
        }

        std::string text = VectorToString(params);

        if (text.length() > 128) {
            Message::Usage(player, "/b <text>"); 
            return 0;
        }

        if (text.length() > 64) {
            Message::Proximity(player, fmt::format("[OOC] {} ({}): {}...", player->GetNick(), player->GetID(), text.substr(0, 64)), 40.0f, COLOR_FADE1);
            Message::Proximity(player, fmt::format("...{}", text.substr(64), player->ParseName()), 40.0f, COLOR_FADE1);
        
            return 0;
        }

        Message::Proximity(player, fmt::format("[OOC] {} ({}): {}", player->GetNick(), player->GetID(), text), 40.0f, COLOR_FADE1);

        return 1;
    });

    return 1;
}