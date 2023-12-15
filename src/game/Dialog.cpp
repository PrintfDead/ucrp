#include <string>
#include <sampgdk/a_samp.h>
#include "Dialog.h"
#include "Player.h"
#include "../managers/PlayerManager.h"

using namespace std;

Dialog::Dialog(char* _name, int _style) {
	name = _name;
	style = _style;

	dialogid = dialogMap.size()+1;

	dialogMap[dialogid] = this;
	dialogNameMap[_name] = dialogid;
}

Dialog* Dialog::Caption(const char* _caption) {
	caption = _caption;

	dialogMap[dialogid] = this;

	return this;
}

Dialog* Dialog::Information(const char* _info) {
	info = _info;

	dialogMap[dialogid] = this;

	return this;
}

Dialog* Dialog::Buttons(const char* _button1, const char* _button2) {
	button1 = _button1;
	button2 = _button2;

	dialogMap[dialogid] = this;

	return this;
}

Dialog* Dialog::Get(int id) {
	auto index = dialogMap.find(id);

	if (index != dialogMap.end()) {
		return index->second;
	}

	return NULL;
}

Dialog* Dialog::GetByName(char* _name) {
	auto index = dialogNameMap.find(_name);

	if (index != dialogNameMap.end()) {
		return Dialog::Get(index->second);
	}

	return NULL;
}

int Dialog::Execute(int response, int listitem, const char* inputtext) {
	return OnResponse(PlayerManager::GetPlayer(playerid), response, listitem, inputtext);
}

int Dialog::ShowToPlayer(int _playerid) {
	if (style == NULL || caption == NULL || info == NULL || button1 == NULL || button2 == NULL) {
		return 0;
	}

	playerid = _playerid;
	ShowPlayerDialog(_playerid, dialogid, style, caption, info, button1, button2);

	return 1;
}

void Dialog::Delete(int id) {
	dialogMap.erase(id);
}