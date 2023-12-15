#pragma once

#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <fmt/format.h>
#include "../game/Player.h"

static void RolAme(Player* player, std::string str) {
    if (player->GetCrack() == 0) {
        SendClientMessage(player->GetID(), 0xD0AEEBFF, fmt::format("> {}", str).c_str());
        SetPlayerChatBubble(player->GetID(), str.c_str(), 0xD0AEEBFF, 15.0, 10000);
    }
}

static void ResetPlayerVariables(Player* player) {
	player->SetAccountID(-1);
	player->SetIP("127.0.0.1");
	player->SetHealth(100);
	player->SetArmor(0);
	player->SetSkin(0);
	player->SetColor(0);
	player->SetSpawned(false);
	player->SetFacingAngle(0.0);
	player->SetVirtualWorld(0);
	player->SetInterior(0);
	player->StopAnimations();
	player->SetEmail("none");
	player->SetPassword("none");
	player->SetNick("none");
}