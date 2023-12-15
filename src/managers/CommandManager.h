#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../game/Player.h"
#include "../utils/Flags.h"

class CommandManager
{
public:
	CommandManager();
	~CommandManager();

	static int RegisterCommand(std::vector<std::string> names, const IFlag flag, std::function<int(Player*, std::vector<std::string>)>);

	bool OnPlayerCommandText(Player* player, std::string commandText);

	std::function<int(Player*, std::vector<std::string>)> GetCommand(std::string command);
	IFlag GetFlag(std::string name);

	static Player* GetPlayerFromParams(std::vector<std::string> params, std::vector<std::string>::size_type index = 0);
	static Player* GetPlayerFromParams(Player* player, std::vector<std::string> params, std::vector<std::string>::size_type index);

private:
	inline static std::unordered_map<std::string, std::function<int(Player*, std::vector<std::string>)>> commandMap;
	inline static std::unordered_map<std::string, IFlag> commandFlag;
};