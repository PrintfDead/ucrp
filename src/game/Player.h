#pragma once
#include <fmt/format.h>
#include <string>
#include <unordered_map>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <sampgdk/core.h>
#include "sqlite3.h"
#include "../utils/Point.h"

class Player
{
public:
	template <class Archive>
	Player();
	Player(int id);
	~Player();

	inline std::string GetName() const { return name; };
	inline void SetName(std::string _name) { name = _name; };
	std::string ParseName();
	bool HasValidName(std::string _nick);

	inline int GetID() const { return id; };
	inline int GetCharacterID() const { return cID; };
	inline void SetCharacterID(int id) { cID = id; };

	// Account
	void SetAccountID(int id);
	int  GetAccountID();
	void SetNick(std::string _nick);
	std::string GetNick();
	void SetEmail(std::string _email);
	std::string GetEmail();
	bool HasValidEmail(std::string _email);
	void SetPassword(std::string _password);
	std::string GetPassword();
	void SetSalt(std::string _salt);
	std::string GetSalt();
	void SetIP(std::string _ip);
	std::string GetIP();
	inline void SetAdminLevel(int level) { adminlevel = level; };
	inline int GetAdminLevel() const { return adminlevel; };

	// Character

	inline void SetHealth(float _health) { health = _health; };
	inline float GetHealth() const { return health;  };
	inline void SetArmor(float armor) { kevlar = armor; };
	inline float GetArmor() const { return kevlar; };
	inline void SetSkin(int skinID) { skin = skinID; };
	inline int GetSkin() const { return skin; };
	inline void SetColor(int color) const { SetPlayerColor(id, color); };
	inline int GetColor() const { return GetPlayerColor(id); };
	inline bool IsSpawned() const {  return isSpawned; };
	inline void SetSpawned(bool spawned) { isSpawned = spawned; };
	inline Point3D GetPosition() const { return position; };
	inline void SetPosition(Point3D _position) { position = _position; };
	inline float GetFacingAngle() const { return angle; };
	inline void SetFacingAngle(float _angle) { angle = _angle; };
	inline int GetVirtualWorld() const { return virtualworld; };
	inline void SetVirtualWorld(int virtualWorldID) { virtualworld = virtualWorldID; };
	inline int GetInterior() const { return interior; };
	inline void SetInterior(int interiorID) { interior = interiorID; };
	inline int GetCrack() const { return crack; };
	inline void SetCrack(int _crack) { crack = _crack; };

	// Functions

	void GiveWeapon(int weaponID);
	void GiveWeapon(int weaponID, int ammo);

	void Kill();

	bool IsInRangeOfPlayer(float range, Player* player);
	bool IsInRangeOfPoint(float range, Point3D point);
	float GetDistanceFromPoint(Point3D point);

	bool IsInVehicle();
	bool IsInVehicleID(int vehicleID);
	int GetVehicleID();
	int GetVehicleModelID();

	void StartAnimation(const char* animationLibrary, const char* animationName, float delta, bool loop, bool lockX, bool lockY, bool freeze, int time, bool forceSync);

	void StartWalkAnimation(int index);
	void StartRunAnimation(int index);
	void StopAnimations();
	bool IsLoopingAnimation();

	// Temp Maps

	std::unordered_map<int, int> charactersMap;
	std::unordered_map<int, std::string> charactersNameMap;

protected:
	int id;
	int cID;
	std::string name;

	// Account
	int aID;
	std::string nick;
	std::string email;
	std::string password;
	std::string salt;
	std::string ip;
	int adminlevel;

	// Character
	int skin;
	Point3D position;
	float angle;
	int interior;
	int virtualworld;
	int crack;
	float health;
	float kevlar;

	float connectedTime;

	bool isSpawned;
	bool isLoopingAnimation;

	std::unordered_map<int, Player*> inventoryMap;
};

static void SpawnCharacter(Player *player) {
	SpawnPlayer(player->GetID());

	TogglePlayerSpectating(player->GetID(), false);
    TogglePlayerControllable(player->GetID(), true);

    SetPlayerHealth(player->GetID(), player->GetHealth());
    SetPlayerArmour(player->GetID(), player->GetArmor());
    SetPlayerSkin(player->GetID(), player->GetSkin());

    Point3D position = player->GetPosition();

    SetPlayerPos(player->GetID(), position.x, position.y, position.z);
    SetPlayerFacingAngle(player->GetID(), player->GetFacingAngle());
    SetPlayerInterior(player->GetID(), player->GetInterior());
    SetPlayerVirtualWorld(player->GetID(), player->GetVirtualWorld());

	player->SetSpawned(true);
}

static void GetCharacters(Player *player, int accountid) {
	sqlite3 *db;
    sqlite3_stmt *stmt;

    std::string sql = fmt::format("SELECT ID, Nick FROM Characters WHERE Account = '{}'", accountid);
    int count = 0;

	if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
		sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			if (count >= 3) break;

			player->charactersMap[count] = sqlite3_column_int(stmt, 0);
			player->charactersNameMap[count] = std::string((char*) sqlite3_column_text(stmt, 1));
			count++;
		}

		if (count == 0) {
			for (int i = 0; i < 3; i++) {
				player->charactersMap[i] = -1;
				player->charactersNameMap[i] = "Crear Personaje";
			}
		} else if (count == 1) {
			for (int i = 0; i <= 2; i++) {
				player->charactersMap[i+1] = -1;
				player->charactersNameMap[i+1] = "Crear Personaje";
			}
		} else if (count == 2) {
			player->charactersMap[2] = -1;
			player->charactersNameMap[2] = "Crear Personaje";
		}
    } else {
        sampgdk::logprintf("Error, Failed tu open db.");
        Kick(player->GetID());
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

	return;
}