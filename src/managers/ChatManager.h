#pragma once
#pragma once

#include <string>

class Player;

class ChatManager
{
public:

	static void ChatManager::UsageMessage(Player* player, std::string message);
	static void ChatManager::SystemMessage(Player* player, std::string message);
	static void ChatManager::EmoteMessage(Player* player, std::string message, int color);

	static void ChatManager::ErrorMessageInvalidPlayer(Player* player);
	static void ChatManager::ProximityMessage(Player *player, std::string message, float distance, int color1, int color2, int color3, int color4, int color5);
	static void ChatManager::ProximityMessage(Player *player, std::string message, float distance, int color);

private:
	ChatManager();
	~ChatManager();
};