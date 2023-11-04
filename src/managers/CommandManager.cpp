#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "PlayerManager.h"
#include "../game/Player.h"
#include "../utils/Constants.h"
#include "CommandManager.h"
#include "../utils/Util.h"
#include "../utils/Flags.h"

using namespace std;

int CommandManager::RegisterCommand(std::vector<std::string> names, const AdminFlag flag, std::function<int(Player*, std::vector<std::string>)> callback) {
	std::for_each(names.begin(), names.end(), [callback, flag](std::string name) {
		commandMap[name] = callback;
		commandFlag[name] = flag;
	});

	return 1;
}

std::function<int(Player*, std::vector<std::string>)> CommandManager::GetCommand(std::string commandInput)
{
	string command = ToLowerCase(commandInput);

	auto find = commandMap.find(command);
	if (find != commandMap.end()) {
		return find->second;
	}
	return NULL;
}

AdminFlag CommandManager::GetFlag(std::string name) {
	FLAG_User userFlag;

	auto find = commandFlag.find(name);
	if (find != commandFlag.end()) {
		return find->second;
	}

	return userFlag;
}

bool CommandManager::OnPlayerCommandText(Player* player, string text)
{
	if(!player->IsSpawned()) return 0;

	vector<string> params = split(text);

	if (params.size() > 0)
	{
		string remainingtext;
		string name = params[0].substr(1);
		if (params.size() > 1)
		{
			remainingtext = text.substr(params[0].size() + 1);
		}

		auto command = GetCommand(name);
		if (command != nullptr)
		{
			AdminFlag flag = GetFlag(name);

			if (player->GetAdminLevel() < flag.admin_level) { flag.error_message(player->GetID()); return 0; }

			params.erase(params.begin());
			command(player, params);	

			return true;
		}
	}

	return false;
}

CommandManager::CommandManager() {}

CommandManager::~CommandManager() {}