#pragma once
#include <functional>
#include <string>
#include "Player.h"

class Dialog
{
public:
	explicit Dialog(char* _name, int _style);

	std::function<int(Player*, int, int, const char*)> OnResponse;

	Dialog* Caption(const char* _caption);
	Dialog* Information(const char* _info);
	Dialog* Buttons(const char* _button1, const char* _button2);

	static Dialog* Get(int dialogid);
	static Dialog* GetByName(char* _name);
	inline int GetID() const { return dialogid; };
	int Execute(int response, int listitem, const char* inputtext);
	int ShowToPlayer(int _playerid);
	static void Delete(int id);

private:
	int dialogid;
	char* name;
	int style;
	int playerid;
	const char* caption;
	const char* info;
	const char* button1;
	const char* button2;

	inline static std::unordered_map<int, Dialog*> dialogMap;
	inline static std::unordered_map<char*, int> dialogNameMap;
};