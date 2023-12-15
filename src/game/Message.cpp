#include <sampgdk/a_samp.h>
#include "../game/Player.h"
#include "Message.h"
#include "../managers/PlayerManager.h"
#include "../utils/Constants.h"

using namespace std;

static const float MESSAGE_DISTANCE = 20.f;
static const float SHOUT_DISTANCE = 40.f;
static const float EMOTE_DISTANCE = MESSAGE_DISTANCE - 2.f;

int Message::System(Player* player, string message)
{
    string fullMessage = "{5b5b5b}* {FFFFFF}" + message;
	SendClientMessage(player->GetID(), COLOR_GRAD1, fullMessage.c_str());

	return 1;
}

int Message::Usage(Player* player, string message)
{
	string fullMessage = "{C60004}x {FFFFFF} Uso: " + message;
	SendClientMessage(player->GetID(), COLOR_WHITE, fullMessage.c_str());

	return 0;
}

int Message::Error(Player* player, string message)
{
    string fullMessage = "{C60004}x {FFFFFF}" + message;
    SendClientMessage(player->GetID(), COLOR_WHITE, fullMessage.c_str());

	return 0;
}

int Message::Success(Player* player, string message)
{
    string fullMessage = "{062037}> {FFFFFF}" + message;
    SendClientMessage(player->GetID(), COLOR_WHITE, fullMessage.c_str());

	return 1;
}

int Message::AdminSuccess(Player* player, string message)
{
    string fullMessage = "{FF0000}> {FFFFFF}" + message;
    SendClientMessage(player->GetID(), COLOR_WHITE, fullMessage.c_str());

	return 1;
}

void Message::Proximity(Player *player, string message, float distance, int color)
{
	Proximity(player, message, distance, color, color, color, color, color);
}

void Message::Proximity(Player *player, string message, float distance, int color1, int color2, int color3, int color4, int color5)
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

Message::~Message() {}