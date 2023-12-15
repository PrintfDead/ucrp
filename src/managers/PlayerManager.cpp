#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <sampgdk/core.h>
#include <fmt/format.h>
#include "PlayerManager.h"
#include "../game/Player.h"

using namespace std;

unordered_map<int, Player*> PlayerManager::playerMap;
unordered_map<string, Player*> PlayerManager::playerNameMap;

void PlayerManager::SavePlayer(Player* player)
{
	if (!player->IsSpawned()) return;

	sqlite3 *db;
	sqlite3_stmt *stmt;

	float x, y, z, angle, health, kevlar;

	GetPlayerPos(player->GetID(), &x, &y, &z);
	GetPlayerFacingAngle(player->GetID(), &angle);
	GetPlayerHealth(player->GetID(), &health);
	GetPlayerArmour(player->GetID(), &kevlar);

	int interior = GetPlayerInterior(player->GetID());
	int vw = GetPlayerVirtualWorld(player->GetID());

	Point3D position = Point3DMake(x, y, z);

	std::string sql = fmt::format("UPDATE Characters SET PositionX = '{}', PositionY = '{}', PositionZ = '{}', PositionR = '{}', Interior = '{}', VirtualWorld = '{}', Health = '{}', Kevlar = '{}', Skin = '{}', Crack = '{}', HandRight = '{}', HandRightAmount = '{}', HandLeft = '{}', HandLeftAmount = '{}' WHERE ID = '{}'",
		position.x, position.y, position.z,
		angle,
		interior, vw,
		health, kevlar,
		player->GetSkin(), player->GetCrack(),
		player->Hand[0].item->ID, player->Hand[0].amount,
		player->Hand[1].item->ID, player->Hand[1].amount,
		player->GetCharacterID()
	);

	std::string sql2 = fmt::format("UPDATE Inventories SET Slot1 = '{}', SlotAmount1 = '{}', Slot2 = '{}', SlotAmount2 = '{}', Slot3 = '{}', SlotAmount3 = '{}', Slot4 = '{}', SlotAmount4 = '{}', Slot5 = '{}', SlotAmount5 = '{}' WHERE ID = '{}'", 
		player->Inventory[0].item->ID, player->Inventory[0].amount,
		player->Inventory[1].item->ID, player->Inventory[1].amount,
		player->Inventory[2].item->ID, player->Inventory[2].amount,
		player->Inventory[3].item->ID, player->Inventory[3].amount,
		player->Inventory[4].item->ID, player->Inventory[4].amount,
		player->GetInventoryID()
	);

	if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
		sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);
		int rc = sqlite3_step(stmt);

		if (rc != SQLITE_DONE) {
			sampgdk::logprintf("Error en la consulta: %s", sqlite3_errmsg(db));
		}

		sqlite3_finalize(stmt);
	}

	sqlite3_stmt *newstmt;

	sqlite3_prepare(db, sql.c_str(), -1, &newstmt, NULL);
	int rc = sqlite3_step(newstmt);

	if (rc != SQLITE_DONE) {
		sampgdk::logprintf("Error en la consulta: %s", sqlite3_errmsg(db));
	}

	sqlite3_finalize(newstmt);

    sqlite3_close(db);
}

Player* PlayerManager::GetPlayer(int playerID)
{
	auto index = playerMap.find(playerID);
	if (index != playerMap.end()) {
		return index->second;
	}
	return NULL;
}

Player* PlayerManager::GetPlayerByName(string name)
{
	auto index = playerNameMap.find(name);
	if (index != playerNameMap.end()) {
		return index->second;
	}
	return NULL;
}

const unordered_map<int, Player*>::const_iterator PlayerManager::BeginPlayer()
{
	return playerMap.cbegin();
}

const unordered_map<int, Player*>::const_iterator PlayerManager::EndPlayer()
{
	return playerMap.cend();
}

void PlayerManager::AddPlayer(Player* player)
{
	playerMap[player->GetID()] = player;
	playerNameMap[player->GetName()] = player;
}

void PlayerManager::RemovePlayer(Player* player)
{
	playerMap.erase(player->GetID());

	delete player;
	player = nullptr;
}

string PlayerManager::GetName(int playerID)
{
	char name[MAX_PLAYER_NAME];
	GetPlayerName(playerID, name, sizeof(name));
	return string(name);
}

int PlayerManager::GetNumPlayers()
{
	return playerMap.size();
}