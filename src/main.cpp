#include <stdio.h>
#include <string.h>

#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/core.h>
#include <sampgdk/sdk.h>
#include <fmt/format.h>

#include "game/Player.h"
#include "game/Dialog.h"
#include "managers/PlayerManager.h"
#include "managers/CommandManager.h"
#include "maps/Alhambra.h"
#include "maps/Cemetery.h"
#include "maps/SkatePark.h"
#include "commands/index.h"
#include "utils/dialogs/PlayerConnect.h"
#include "utils/Constants.h"
#include "utils/Util.h"

using namespace std;
static CommandManager* commandManager = new CommandManager;

void SAMPGDK_CALL AccessServer(int timerid, void* params) {
    SendRconCommand("password 0");
    SendRconCommand("hostname [0.3DL] United Capital Roleplay - Juego de Rol");

    sampgdk::logprintf("Server Open!");
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
    sampgdk::logprintf("----------------------------------");
    sampgdk::logprintf(" United Capital: Roleplay");
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
    SendRconCommand("mapname Washington");
    SendRconCommand("language Spanish");
    SendRconCommand("hostname [0.3DL] United Capital Roleplay - Cargando...");

    RegisterAllModulesCommands();

    Alhambra();
    Cemetery();
    SkatePark();

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

    AlhambraRemoveBuilding(playerid);
    CemeteryRemoveBuilding(playerid);
    SkateParkRemoveBuilding(playerid);

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
        TogglePlayerSpectating(playerid, true);
        TogglePlayerControllable(playerid, false);
        SetPlayerName(playerid, fmt::format("Conectando_{}", playerid).c_str());

        Dialog* SelectMenu = new Dialog("UserSelectMenu", DIALOG_STYLE_LIST);

        SelectMenu->Caption("Selecciona una opcione.");
        SelectMenu->Information("Iniciar Sesion\nRegistrarse\nNotas de version");
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

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerText(int playerid, const char* text) {
    Player* player = PlayerManager::GetPlayer(playerid);

    if (player->IsSpawned()) {
        sampgdk::logprintf(text);
        std::string text = std::string(text);
        sampgdk::logprintf(text.c_str());

        if (text.length() > 64) {
            ChatManager::ProximityMessage(player, fmt::format("{}: {}...", player->ParseName(), text.substr(0, 64)), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
            ChatManager::ProximityMessage(player, fmt::format("...{}", text.substr(64)), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
            
            return 0;
        }

        ChatManager::ProximityMessage(player, fmt::format("{}: {}", player->ParseName(), std::string(text)), 20.0f, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);

        return 0;
    }

    return 0;
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