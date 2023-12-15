#pragma once

#include <string>
#include "../game/Player.h"

class Message
{
public:

	static int Message::Usage(Player* player, std::string message);
	static int Message::System(Player* player, std::string message);
    static int Message::Error(Player* player, std::string message);
    static int Message::Success(Player* player, std::string message);
    static int Message::AdminSuccess(Player* player, std::string message);

	static void Message::Proximity(Player *player, std::string message, float distance, int color1, int color2, int color3, int color4, int color5);
	static void Message::Proximity(Player *player, std::string message, float distance, int color);

private:
	Message();
	~Message();
};