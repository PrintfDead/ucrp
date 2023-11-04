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
#include "../../managers/ChatManager.h"
#include "../../utils/Util.h"

static int RegisterChatCommands() {
    FLAG_User userFlag;

    std::vector<std::string> meNames = {"me"}; 
    std::vector<std::string> doNames = {"do"};
    std::vector<std::string> oocNames = {"b", "ooc"}; 

    CommandManager::RegisterCommand(meNames, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { ChatManager::UsageMessage(player, "/me <accion>"); return 0; }

        
        std::string text = VectorToString(params);

        if (text.length() > 128) {
            ChatManager::UsageMessage(player, "/me <accion>");

            return 0;
        }

        if (text.length() > 64) {
            ChatManager::EmoteMessage(player, fmt::format("* {} {}...", player->ParseName(), text.substr(0, 64)), 0);
            ChatManager::EmoteMessage(player, fmt::format("...{}", text.substr(64)), 0);
        
            return 0;
        }

        ChatManager::EmoteMessage(player, fmt::format("* {} {}", player->ParseName(), text), 0);

        return 1;
    });

    CommandManager::RegisterCommand(doNames, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { ChatManager::UsageMessage(player, "/do <accion>"); return 0; }

        std::string text = VectorToString(params);

        if (text.length() > 128) {
            ChatManager::UsageMessage(player, "/do <accion>");

            return 0;
        }

        if (text.length() > 64) {
            ChatManager::ProximityMessage(player, fmt::format("* {}...", text.substr(0, 64)), 20.0f, COLOR_LIGHTGREEN);
            ChatManager::ProximityMessage(player, fmt::format("* ...{} ({})", text.substr(64), player->ParseName()), 20.0f, COLOR_LIGHTGREEN);
        
            return 0;
        }

        ChatManager::ProximityMessage(player, fmt::format("* {} ({})", text, player->ParseName()), 20.0f, COLOR_LIGHTGREEN);

        return 1;
    });

    CommandManager::RegisterCommand(oocNames, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) {
            ChatManager::UsageMessage(player, "/b <text>"); 
            return 0;
        }

        std::string text = VectorToString(params);

        if (text.length() > 128) {
            ChatManager::UsageMessage(player, "/b <text>"); 
            return 0;
        }

        if (text.length() > 64) {
            ChatManager::ProximityMessage(player, fmt::format("[OOC] {} ({}): {}...", player->GetNick(), player->GetID(), text.substr(0, 64)), 40.0f, COLOR_FADE1);
            ChatManager::ProximityMessage(player, fmt::format("...{}", text.substr(64), player->ParseName()), 40.0f, COLOR_FADE1);
        
            return 0;
        }

        ChatManager::ProximityMessage(player, fmt::format("[OOC] {} ({}): {}", player->GetNick(), player->GetID(), text), 40.0f, COLOR_FADE1);

        return 1;
    });

    return 1;
}