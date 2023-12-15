#include <stdio.h>
#include <string.h>

#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/core.h>
#include <sampgdk/sdk.h>
#include <fmt/format.h>

#include "game/Player.h"
#include "game/Dialog.h"
#include "game/Items.h"
#include "game/Message.h"
#include "managers/PlayerManager.h"
#include "managers/CommandManager.h"
#include "commands/index.h"
#include "dialogs/PlayerDialog.h"
#include "structures/Inventory.h"
#include "utils/Constants.h"
#include "utils/Util.h"
#include "utils/Objects.h"
#include "utils/Player.h"


using namespace std;
static CommandManager* commandManager = new CommandManager;

void SAMPGDK_CALL AccessServer(int timerid, void* params) {
    SendRconCommand("password 0");
    SendRconCommand("hostname United Capital Roleplay - Juego de Rol");

    sampgdk::logprintf("Server Open!");
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
    sampgdk::logprintf("----------------------------------");
    sampgdk::logprintf("    United Capital: Roleplay");
    sampgdk::logprintf("----------------------------------");

    UsePlayerPedAnims();
    AllowInteriorWeapons(true);
    SetNameTagDrawDistance(10);
    EnableStuntBonusForAll(false);
    DisableInteriorEnterExits();
    ShowPlayerMarkers(1);
    ShowNameTags(false);

    SetGameModeText("Hello, World!");
    SendRconCommand("password loadserverwaitplease");
    SendRconCommand("mapname San Fierro");
    SendRconCommand("language Spanish");
    SendRconCommand("hostname United Capital Roleplay - Cargando...");

    RegisterAllModulesCommands();

    AddPlayerClass(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    SetTimer(10000, false, AccessServer, 0);
    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeExit() {
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
    Player* player = new Player(playerid);
    PlayerManager::AddPlayer(player);

    ResetPlayerVariables(player);

    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason) {
    Player* player = PlayerManager::GetPlayer(playerid);

    PlayerManager::SavePlayer(player);
    PlayerManager::RemovePlayer(player);

    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid, int classid) {
    Player* player = PlayerManager::GetPlayer(playerid);

    if (!player->IsSpawned()) {
        SpawnPlayer(playerid);
        TogglePlayerSpectating(playerid, true);
        TogglePlayerControllable(playerid, false);
        SetPlayerName(playerid, fmt::format("Conectando_{}", playerid).c_str());

        Dialog* SelectMenu = new Dialog("UserSelectMenu", DIALOG_STYLE_LIST);

        SelectMenu->Caption("{FFFFFF}Selecciona una opcione.");
        SelectMenu->Information("{062037}> {FFFFFF}Iniciar Sesion\n{062037}> {FFFFFF}Registrarse\n{062037}> {FFFFFF}Notas de version");
        SelectMenu->Buttons("Siguiente", "Cancelar");
        SelectMenu->OnResponse = SelectMenuResponse;
        SelectMenu->ShowToPlayer(player->GetID());
    }

  	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestSpawn(int playerid) {
    return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid) {
    Player* player = PlayerManager::GetPlayer(playerid);

    if (IsPlayerNPC(playerid)) return false;

    if (player->GetCrack() == 1) {
        TogglePlayerSpectating(playerid, false);
        TogglePlayerControllable(playerid, false);

        SetPlayerHealth(playerid, 25.0f);
        SetPlayerArmour(playerid, 0);

        player->SetHealth(25.0f);
        player->SetArmor(25.0f);

        Point3D position = player->GetPosition();

        SetPlayerPos(playerid, position.x, position.y, position.z);
        SetPlayerFacingAngle(playerid, player->GetFacingAngle());
        SetPlayerInterior(playerid, player->GetInterior());
        SetPlayerVirtualWorld(playerid, player->GetVirtualWorld());

        RemovePlayerAttachedObject(player->GetID(), 7);
        RemovePlayerAttachedObject(player->GetID(), 8);

        ResetPlayerWeapons(player->GetID());

        if (player->Hand[0].item->ID != 0) {
            if (player->Hand[0].item->IDGun > 0) {
                if (player->Hand[0].amount > 0) {
                    GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
                }
            }

            PutObject(player, 0, player->Hand[0].item->IDGun);
        }
        if (player->Hand[1].item->ID != 0) {
            PutObject(player, 1, player->Hand[1].item->IDGun);
        }

        ApplyAnimation(playerid, "WUZI", "CZ_Dead_Guy", 4.0f, false, true, true, true, 0, true);
    }

    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDeath(int playerid, int killerid, int reason) {
    Player* player = PlayerManager::GetPlayer(playerid);

    float x, y, z, r;

    GetPlayerPos(playerid, &x, &y, &z);

    Point3D position = Point3DMake(x, y, z);
    GetPlayerFacingAngle(playerid, &r);

    player->SetPosition(position);
    player->SetFacingAngle(r);
    player->SetInterior(GetPlayerInterior(playerid));
    player->SetVirtualWorld(GetPlayerVirtualWorld(playerid));

    player->SetCrack(1);

    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerText(int playerid, const char* txt) {
    Player* player = PlayerManager::GetPlayer(playerid);

    if (player->IsSpawned()) {
        std::string text = std::string(txt);

        if (text.length() > 64) {
            Message::Proximity(player, fmt::format("{} dice: {}...", player->ParseName(), text.substr(0, 64)), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
            Message::Proximity(player, fmt::format("...{}", text.substr(64)), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
        
            return 0;
        }

        Message::Proximity(player, fmt::format("{} dice: {}", player->ParseName(), text), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);

        return 0;
    }

    return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char* cmdtext) {
    return commandManager->OnPlayerCommandText(PlayerManager::GetPlayer(playerid), string(cmdtext));
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char *inputtext) {
    Dialog* dialog = Dialog::Get(dialogid);
    
    if (dialog != nullptr) {
        dialog->Execute(response, listitem, inputtext);

        delete dialog;
        dialog = nullptr;
    } else {
        return false;
    }

    return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
  	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
  	return sampgdk::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
  	sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
  	sampgdk::ProcessTick();
}