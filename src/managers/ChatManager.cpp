#include <sampgdk/a_samp.h>
#include "ChatManager.h"
#include "PlayerManager.h"
#include "../game/Player.h"
#include "../utils/Constants.h"

using namespace std;

static const float MESSAGE_DISTANCE = 20.f;
static const float SHOUT_DISTANCE = 40.f;
static const float EMOTE_DISTANCE = MESSAGE_DISTANCE - 2.f;
void ChatManager::SystemMessage(Player* player, string message)
{
	SendClientMessage(player->GetID(), COLOR_GRAD1, message.c_str());
}

void ChatManager::UsageMessage(Player* player, string message)
{
	string fullMessage = "Usage: \"" + message + "\"";
	SendClientMessage(player->GetID(), COLOR_GRAD1, fullMessage.c_str());
}

void ChatManager::ErrorMessageInvalidPlayer(Player* player)
{
	ChatManager::SystemMessage(player, "Invalid {FFFFFF}player name or ID.");
}

void ChatManager::EmoteMessage(Player *player, string message, int color)
{
	ChatManager::ProximityMessage(player, message, EMOTE_DISTANCE, COLOR_EMOTE);
}

void ChatManager::ProximityMessage(Player *player, string message, float distance, int color)
{
	ProximityMessage(player, message, distance, color, color, color, color, color);
}

void ChatManager::ProximityMessage(Player *player, string message, float distance, int color1, int color2, int color3, int color4, int color5)
{
	Point3D position = player->GetPosition();
	bool inVehicle = player->IsInVehicle();

	for (auto i = PlayerManager::BeginPlayer(); i != PlayerManager::EndPlayer(); i++)
	{
		Player *otherPlayer = i->second;
		if (player->GetID() == otherPlayer->GetID())
		{
			SendClientMessage(player->GetID(), color1, message.c_str());
			continue;
		}

		if (player->GetVirtualWorld() == otherPlayer->GetVirtualWorld())
		{
			Point3D otherPlayerPosition = otherPlayer->GetPosition();
			if (Point3DDistance(position, otherPlayerPosition) < distance / 16)
			{
				SendClientMessage(otherPlayer->GetID(), color1, message.c_str());
			}
			else if (Point3DDistance(position, otherPlayerPosition) < distance / 8)
			{
				SendClientMessage(otherPlayer->GetID(), color2, message.c_str());
			}
			else if (Point3DDistance(position, otherPlayerPosition) < distance / 4)
			{
				SendClientMessage(otherPlayer->GetID(), color3, message.c_str());
			}
			else if (Point3DDistance(position, otherPlayerPosition) < distance / 2)
			{
				SendClientMessage(otherPlayer->GetID(), color4, message.c_str());
			}
			else if (Point3DDistance(position, otherPlayerPosition) < distance)
			{
				SendClientMessage(otherPlayer->GetID(), color5, message.c_str());
			}
		}
	}
}

ChatManager::~ChatManager() {}