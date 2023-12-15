#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sqlite3.h>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <fmt/format.h>
#include <typeinfo>
#include <type_traits>
#include "../../game/Player.h"
#include "../../game/Message.h"
#include "../../managers/CommandManager.h"
#include "../../utils/Flags.h"
#include "../../utils/Util.h"

static int RegisterModeratorCommands() {
    FLAG_Moderator modFlag;

    CommandManager::RegisterCommand({"changename"}, modFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { Message::Usage(player, "/changename <ID> <nombre>"); return 0; }

        int id;

        sampgdk::logprintf(params[0].c_str());

        if (!ParseInt(params[0], id)) { Message::Error(player, "Debes introducir una id correcta."); return 0; }
        if (!HasValidName(params[1])) { Message::Usage(player, "/changename <ID> <nombre>"); return 0; }
        if (!IsPlayerConnected(id)) { Message::Error(player, "Este usuario no esta conectado."); return 0; }
        
        Player* newplayer = PlayerManager::GetPlayer(id);

        SetPlayerName(newplayer->GetID(), params[1].c_str());

        newplayer->SetName(params[1]);

        sqlite3 *db;
	    sqlite3_stmt *stmt;

        std::string sql = fmt::format("UPDATE Characters SET Nick = '{}' WHERE ID = '{}'", newplayer->GetName(), newplayer->GetCharacterID());

        if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
            sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);
            int rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE) {
                sampgdk::logprintf("Error en la consulta: %s", sqlite3_errmsg(db));
            }
	    }

        Message::AdminSuccess(player, fmt::format("El usuario {} se le cambio el nombre de su personaje a {} ({})", newplayer->GetNick(), newplayer->GetName(), player->GetNick()));
        Message::Success(newplayer, fmt::format("El administrador {} te cambio el nombre de personaje a {}.\nSi crees que fue un error, reportalo en foro.", player->GetNick(), newplayer->GetName(), player->GetNick()));

        return 1;
    });

    return 1;
}