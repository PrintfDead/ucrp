#pragma once

#include <unordered_map>
#include <string>

class Player;

class PlayerManager
{
public:
	static Player* GetPlayer(int playerID);
	static std::string GetName(int playerID);

	static const std::unordered_map<int, Player*>::const_iterator BeginPlayer();
	static const std::unordered_map<int, Player*>::const_iterator EndPlayer();

	static int GetNumPlayers();
	static Player* GetPlayerByName(std::string name);

	static void AddPlayer(Player* player);
	static void RemovePlayer(Player* player);
	static void SavePlayer(Player* player);

private:
	PlayerManager();
	~PlayerManager();

	static std::unordered_map<int, Player*> playerMap;
	static std::unordered_map<std::string, Player*> playerNameMap;
};