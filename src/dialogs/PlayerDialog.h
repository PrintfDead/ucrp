#include <fmt/format.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "sqlite3.h"
#include "../game/Player.h"
#include "../game/Items.h"
#include "../game/Dialog.h"
#include "../managers/PlayerManager.h"
#include "../utils/Objects.h"
#include "../utils/Constants.h"
#include "../utils/Util.h"
#include "../utils/Point.h"
#include "../structures/Inventory.h"

static sqlite3* db;

static int CreateCharacterInputResponse(Player* player, int response, int listitem, const char *inputtext) {
    if (!response) {
        sampgdk::logprintf("no se respondio xd");
        Kick(player->GetID());

        return 0;
    }

    if (std::string(inputtext).length() <= 0) {
        Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
        dialog->Caption("Crear Personaje.");
        dialog->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario!\n{FFFFFF}Ingresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
        dialog->Buttons("Siguiente", "Cancelar");
        dialog->OnResponse = CreateCharacterInputResponse;
        dialog->ShowToPlayer(player->GetID());

        return 0;
    } else if (std::string(inputtext).length() > 24) {
        Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
        dialog->Caption("Crear Personaje.");
        dialog->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario con no mas de 24 caracteres!\n{FFFFFF}Ingresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
        dialog->Buttons("Siguiente", "Cancelar");
        dialog->OnResponse = CreateCharacterInputResponse;
        dialog->ShowToPlayer(player->GetID());

        return 0;
    } else if (!HasValidName(std::string(inputtext))) {
        Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
        dialog->Caption("Crear Personaje.");
        dialog->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario con el formato indicado!\nIngresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
        dialog->Buttons("Siguiente", "Cancelar");
        dialog->OnResponse = CreateCharacterInputResponse;
        dialog->ShowToPlayer(player->GetID());

        return 0;
    }

    player->SetName(std::string(inputtext));

    sqlite3_stmt *stmt;

    std::string sql = fmt::format("INSERT INTO Characters (Nick, Account) VALUES ('{}', '{}')", player->GetName(), player->GetAccountID());

    if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
        sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            player->SetCharacterID((int) sqlite3_last_insert_rowid(db));

            SetPlayerName(player->GetID(), player->GetName().c_str());
            Point3D position = Point3DMake(2106.7546f, -1771.4769f, 13.3936f);

            player->SetPosition(position);
            player->SetFacingAngle(284.3643f);
            player->SetInterior(0);
            player->SetVirtualWorld(0);
            player->SetHealth(100.0f);
            player->SetArmor(0.0f);
            player->SetSkin(289);
            player->SetCrack(0);

            player->Hand.push_back(IHand {new Empty(), 0});
            player->Hand.push_back(IHand {new Empty(), 0});
        } else {
            sampgdk::logprintf("Error, Failed Query.");
            Kick(player->GetID());

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }
    } else {
        sampgdk::logprintf("Error, Failed tu open db.");
        Kick(player->GetID());

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return 0;
    }

    sqlite3_finalize(stmt);

    sqlite3_stmt *newstmt;

    sql = fmt::format("INSERT INTO Inventories (IDCharacter, Slot1, SlotAmount1) VALUES ('{}', '{}', '{}')", player->GetCharacterID(), 52, 1);

    sqlite3_prepare(db, sql.c_str(), -1, &newstmt, NULL);

    if (sqlite3_step(newstmt) == SQLITE_DONE) {
        player->SetInventoryID((int) sqlite3_last_insert_rowid(db));
        player->Inventory.push_back(MakeSlot(0, new SamsungBlack(), 1));

        for (int i = 1; i<=4; i++) {
            player->Inventory.push_back(MakeSlot(i, new Empty(), 0));
        }
    } else {
        sampgdk::logprintf("Error, Failed Query.");
        Kick(player->GetID());

        sqlite3_finalize(newstmt);
        sqlite3_close(db);

        return 0;
    }

    sqlite3_finalize(newstmt);
    sqlite3_close(db);

    PlayerManager::SavePlayer(player);
    
    TogglePlayerSpectating(player->GetID(), false);
    TogglePlayerControllable(player->GetID(), true);

    SetPlayerHealth(player->GetID(), player->GetHealth());
    SetPlayerArmour(player->GetID(), player->GetArmor());
    SetPlayerSkin(player->GetID(), player->GetSkin());

    Point3D position = player->GetPosition();

    SetPlayerPos(player->GetID(), position.x, position.y, position.z);
    SetPlayerFacingAngle(player->GetID(), player->GetFacingAngle());
    SetPlayerInterior(player->GetID(), player->GetInterior());
    SetPlayerVirtualWorld(player->GetID(), player->GetVirtualWorld());

	SetPlayerSkillLevel(player->GetID(), 0, 1);
	SetPlayerSkillLevel(player->GetID(), 1, 1);
	SetPlayerSkillLevel(player->GetID(), 2, 1);
	SetPlayerSkillLevel(player->GetID(), 3, 1);
	SetPlayerSkillLevel(player->GetID(), 4, 1);
	SetPlayerSkillLevel(player->GetID(), 5, 1);
	SetPlayerSkillLevel(player->GetID(), 6, 1);
	SetPlayerSkillLevel(player->GetID(), 7, 1);


	RemovePlayerAttachedObject(player->GetID(), 7);
    RemovePlayerAttachedObject(player->GetID(), 8);

    ResetPlayerWeapons(player->GetID());

    if (player->Hand[0].item->ID != 0) {
        if (player->Hand[0].item->IDGun > 0) {
            if (player->Hand[0].amount > 0) {
                GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
            }
        }

        PutObject(player, 0, player->Hand[0].item->IDGun);
    }

    if (player->Hand[1].item->ID != 0) {
        PutObject(player, 1, player->Hand[1].item->IDGun);
    }

	player->SetSpawned(true);

    return 1;
}

static int SelectCharacterMenuResponse(Player* player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (player->charactersMap[listitem] == -1) {
            Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
            dialog->Caption("Crear Personaje.");
            dialog->Information("{062037}> {FFFFFF}Ingresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
            dialog->Buttons("Siguiente", "Cancelar");
            dialog->OnResponse = CreateCharacterInputResponse;
            dialog->ShowToPlayer(player->GetID());

            return 1;
        } else {
            sqlite3_stmt *stmt;

            std::string sql = fmt::format("SELECT * FROM Characters WHERE ID = '{}'", player->charactersMap[listitem]);

            if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
                sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    player->SetCharacterID(sqlite3_column_int(stmt, 0));
                    player->SetName(std::string((const char*) sqlite3_column_text(stmt, 1)));
                    SetPlayerName(player->GetID(), player->GetName().c_str());
                    
                    Point3D position = Point3DMake((float) sqlite3_column_double(stmt, 3), (float) sqlite3_column_double(stmt, 4), (float) sqlite3_column_double(stmt, 5));
                    player->SetPosition(position);
                    player->SetFacingAngle((float) sqlite3_column_double(stmt, 6));
                    player->SetInterior(sqlite3_column_int(stmt, 7));
                    player->SetVirtualWorld(sqlite3_column_int(stmt, 8));
                    player->SetHealth((float) sqlite3_column_double(stmt, 9));
                    player->SetArmor((float) sqlite3_column_double(stmt, 10));
                    player->SetSkin(sqlite3_column_int(stmt, 11));
                    player->SetCrack(sqlite3_column_int(stmt, 12));

                    player->Hand.push_back(IHand { GetItem(sqlite3_column_int(stmt, 13)), sqlite3_column_int(stmt, 14) });
                    player->Hand.push_back(IHand { GetItem(sqlite3_column_int(stmt, 15)), sqlite3_column_int(stmt, 16) });
                } else {
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    Kick(player->GetID());
                    
                    return 0;
                }
            } else {
                sampgdk::logprintf("Error, Failed tu open db.");
                Kick(player->GetID());

                return 0;
            }

            sqlite3_finalize(stmt);

            sqlite3_stmt *newstmt;
            sql = fmt::format("SELECT * FROM Inventories WHERE IDCharacter = '{}'", player->GetCharacterID());

            sqlite3_prepare(db, sql.c_str(), -1, &newstmt, NULL);

            if (sqlite3_step(newstmt) == SQLITE_ROW) {
                player->SetInventoryID(sqlite3_column_int(newstmt, 0));

                for (int i = 0; i<=11; i++) {
                    sampgdk::logprintf(fmt::format("{}: {}", i, sqlite3_column_int(newstmt, i)).c_str());
                }

                sampgdk::logprintf(fmt::format("{}", GetItem(52)->Name).c_str());

                player->Inventory.push_back(MakeSlot(0, GetItem(sqlite3_column_int(newstmt, 2)), sqlite3_column_int(newstmt, 3)));
                player->Inventory.push_back(MakeSlot(1, GetItem(sqlite3_column_int(newstmt, 4)), sqlite3_column_int(newstmt, 5)));
                player->Inventory.push_back(MakeSlot(2, GetItem(sqlite3_column_int(newstmt, 6)), sqlite3_column_int(newstmt, 7)));
                player->Inventory.push_back(MakeSlot(3, GetItem(sqlite3_column_int(newstmt, 8)), sqlite3_column_int(newstmt, 9)));
                player->Inventory.push_back(MakeSlot(4, GetItem(sqlite3_column_int(newstmt, 10)), sqlite3_column_int(newstmt, 11)));
            } else {
                sqlite3_finalize(newstmt);
                sqlite3_close(db);
                Kick(player->GetID());
                    
                return 0;
            }

            sqlite3_finalize(newstmt);
            sqlite3_close(db);
    
            TogglePlayerSpectating(player->GetID(), false);
            TogglePlayerControllable(player->GetID(), true);

            SetPlayerHealth(player->GetID(), player->GetHealth());
            SetPlayerArmour(player->GetID(), player->GetArmor());
            SetPlayerSkin(player->GetID(), player->GetSkin());

            Point3D position = player->GetPosition();

            SetPlayerPos(player->GetID(), position.x, position.y, position.z);
            SetPlayerFacingAngle(player->GetID(), player->GetFacingAngle());
            SetPlayerInterior(player->GetID(), player->GetInterior());
            SetPlayerVirtualWorld(player->GetID(), player->GetVirtualWorld());

            SetPlayerSkillLevel(player->GetID(), 0, 1);
            SetPlayerSkillLevel(player->GetID(), 1, 1);
            SetPlayerSkillLevel(player->GetID(), 2, 1);
            SetPlayerSkillLevel(player->GetID(), 3, 1);
            SetPlayerSkillLevel(player->GetID(), 4, 1);
            SetPlayerSkillLevel(player->GetID(), 5, 1);
            SetPlayerSkillLevel(player->GetID(), 6, 1);
            SetPlayerSkillLevel(player->GetID(), 7, 1);


            RemovePlayerAttachedObject(player->GetID(), 7);
            RemovePlayerAttachedObject(player->GetID(), 8);

            ResetPlayerWeapons(player->GetID());

            if (player->Hand[0].item->ID != 0) {
                if (player->Hand[0].item->IDGun > 0) {
                    if (player->Hand[0].amount > 0) {
                        GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
                    }
                }

                PutObject(player, 0, player->Hand[0].item->IDGun);
            }

            if (player->Hand[1].item->ID != 0) {
                PutObject(player, 1, player->Hand[1].item->IDGun);
            }

            player->SetSpawned(true);
        }

    } else {
        Kick(player->GetID());
    }

    return 1;
}

static int PasswordInputResponse(Player* player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (std::string(inputtext).length() <= 0) {
            Dialog *LoginInput = new Dialog("PasswordInput", DIALOG_STYLE_PASSWORD);
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("{C60004}x {FFFFFF}Debes ingresar una contrase�a!\n{062037}> {FFFFFF}Ingresa la contrase�a para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = PasswordInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        char hash[64];

        SHA256_PassHash(inputtext, player->GetSalt().c_str(), hash, sizeof(hash));

        if (hash != player->GetPassword()) {
            Dialog *LoginInput = new Dialog("PasswordInput", DIALOG_STYLE_PASSWORD);
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("{C60004}x {FFFFFF}La contrase�a es incorrecta!\n{062037}> {FFFFFF}Ingresa la contrase�a para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = PasswordInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        GetCharacters(player, player->GetAccountID());

        std::string characters[3];
        if (player->charactersMap[0] == -1)
            characters[0] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[0]);
        else
            characters[0] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[0], player->charactersNameMap[0]);
        
        if (player->charactersMap[1] == -1)
            characters[1] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[1]);
        else
            characters[1] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[1], player->charactersNameMap[1]);

        if (player->charactersMap[2] == -1)
            characters[2] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[2]);
        else
            characters[2] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[2], player->charactersNameMap[2]);

        std::string info = fmt::format("{}\n{}\n{}", characters[0], characters[1], characters[2]);

        Dialog *dialog = new Dialog("SelectCharacterMenu", DIALOG_STYLE_LIST);
        dialog->Caption("Selecciona un personaje");
        dialog->Information(info.c_str());
        dialog->Buttons("Seleccionar", "Cancelar");
        dialog->OnResponse = SelectCharacterMenuResponse;
        dialog->ShowToPlayer(player->GetID());
    } else {
        Kick(player->GetID());
    }

    return 1;
}

static int LoginInputResponse(Player *player, int response, int listitem, const char *inputtext)
{
    if (response)
    {
        if (std::string(inputtext).length() <= 0)
        {
            Dialog *LoginInput = new Dialog("LoginInput", DIALOG_STYLE_INPUT);
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("{C60004}x {FFFFFF}Debes ingresar un correo!\n{062037}> {FFFFFF}Ingresa el correo electronico para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = LoginInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        if (!player->HasValidEmail(std::string(inputtext))) {
            Dialog *LoginInput = new Dialog("LoginInput", DIALOG_STYLE_INPUT);
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("{C60004}x {FFFFFF}Cuenta no encontrada.\n{062037}> {FFFFFF}Ingresa el correo electronico para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = LoginInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        sqlite3_stmt *stmt;

        int id;
        std::string email;
        std::string pass;
        std::string salt;
        std::string name;
        std::string ip;
        int admin;

        std::string _query = fmt::format("SELECT * FROM Accounts WHERE Email = '{}'", inputtext);

        if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
            sqlite3_prepare(db, _query.c_str(), -1, &stmt, NULL);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                id = sqlite3_column_int(stmt, 0);
                name = std::string((const char*) sqlite3_column_text(stmt, 1));
                pass = std::string((const char*) sqlite3_column_text(stmt, 2));
                email = std::string((const char*) sqlite3_column_text(stmt, 3));
                ip = std::string((const char*) sqlite3_column_text(stmt, 4));
                salt = std::string((const char*) sqlite3_column_text(stmt, 5));
                admin = sqlite3_column_int(stmt, 6);
            } else {
                Dialog *LoginInput = new Dialog("LoginInput", DIALOG_STYLE_INPUT);
                LoginInput->Caption("Iniciar Sesion");
                LoginInput->Information("{C60004}x {FFFFFF}Cuenta no encontrada.\n{062037}> {FFFFFF}Ingresa el correo electronico para encontrar tu cuenta.");
                LoginInput->Buttons("Siguiente", "Cancelar");
                LoginInput->OnResponse = LoginInputResponse;
                LoginInput->ShowToPlayer(player->GetID());

                sqlite3_finalize(stmt);
                sqlite3_close(db);

                return 0;
            }
        } else {
            sampgdk::logprintf("Error, Failed tu open db.");
            Kick(player->GetID());

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        char currentip[36];

        GetPlayerIp(player->GetID(), currentip, sizeof(36));

        /*if (currentip != ip) {
            SendClientMessage(player->GetID(), COLOR_WHITE, fmt::format("Se detecto un nuevo inicio de sesion desde otra ip. IP Detectada: {}{}", "C60004", currentip).c_str());
            SendClientMessage(player->GetID(), COLOR_WHITE, "Lo siento! Seras expulsado por seguridad.");
            
            Kick(player->GetID());

            return 0;
        }*/

        player->SetAccountID(id);
        player->SetNick(name);
        player->SetEmail(email);
        player->SetPassword(pass);
        player->SetSalt(salt);
        player->SetIP(ip);
        player->SetAdminLevel(admin);

        Dialog *LoginInput = new Dialog("PasswordInput", DIALOG_STYLE_PASSWORD);
        LoginInput->Caption("Iniciar Sesion");
        LoginInput->Information("{062037}> {FFFFFF}Ingresa la contrase�a para encontrar tu cuenta.");
        LoginInput->Buttons("Siguiente", "Cancelar");
        LoginInput->OnResponse = PasswordInputResponse;
        LoginInput->ShowToPlayer(player->GetID());
    }
    else
    {
        Kick(player->GetID());
    }

    return 1;
};

static int PasswordRegisterInputResponse(Player *player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (std::string(inputtext).length() <= 0) {
            Dialog *PasswordRegisterInput = new Dialog("PasswordRegisterInput", DIALOG_STYLE_PASSWORD);
            PasswordRegisterInput->Caption("Registro");
            PasswordRegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar una contrase�a!\n{062037}> {FFFFFF}Ingresa una contrase�a para registrarte en el PCU.");
            PasswordRegisterInput->Buttons("Siguiente", "Cancelar");
            PasswordRegisterInput->OnResponse = PasswordRegisterInputResponse;
            PasswordRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 32) {
            Dialog *PasswordRegisterInput = new Dialog("PasswordRegisterInput", DIALOG_STYLE_PASSWORD);
            PasswordRegisterInput->Caption("Registro");
            PasswordRegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar una contrase�a no mayor a 32 caracteres!\n{062037}> {FFFFFF}Ingresa una contrase�a para registrarte en el PCU.");
            PasswordRegisterInput->Buttons("Siguiente", "Cancelar");
            PasswordRegisterInput->OnResponse = PasswordRegisterInputResponse;
            PasswordRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        }

        std::string salt = generateRandomSalt();
        char hash[64];

        SHA256_PassHash(inputtext, salt.c_str(), hash, sizeof(hash));

        player->SetPassword(std::string(hash));
        player->SetSalt(salt);

        sqlite3_stmt *stmt;
        char ip[36];

        GetPlayerIp(player->GetID(), ip, sizeof(hash));
        player->SetIP(std::string(ip));

        std::string sql = fmt::format("INSERT INTO Accounts (Email, Password, Salt, Name, IP) VALUES('{}', '{}', '{}', '{}', '{}')", player->GetEmail(), player->GetPassword(), player->GetSalt(), player->GetNick(), player->GetIP());

        if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
            sqlite3_prepare(db, sql.c_str(), -1, &stmt, NULL);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                player->SetAccountID((int) sqlite3_last_insert_rowid(db));
            } else {
                sampgdk::logprintf("Error, Failed Query.");
                Kick(player->GetID());

                sqlite3_finalize(stmt);
                sqlite3_close(db);

                return 0;
            }
        } else {
            sampgdk::logprintf("Error, Failed tu open db.");
            Kick(player->GetID());

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        GetCharacters(player, player->GetAccountID());

        std::string characters[3];
        if (player->charactersMap[0] == -1)
            characters[0] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[0]);
        else
            characters[0] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[0], player->charactersNameMap[0]);
        
        if (player->charactersMap[1] == -1)
            characters[1] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[1]);
        else
            characters[1] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[1], player->charactersNameMap[1]);

        if (player->charactersMap[2] == -1)
            characters[2] = "{062037}> {FFFFFF}" + fmt::format("{}", player->charactersNameMap[2]);
        else
            characters[2] = "{062037}> {FFFFFF}" + fmt::format("(#{}) {}", player->charactersMap[2], player->charactersNameMap[2]);

        std::string info = fmt::format("{}\n{}\n{}", characters[0], characters[1], characters[2]);

        Dialog *dialog = new Dialog("SelectCharacterMenu", DIALOG_STYLE_LIST);
        dialog->Caption("Selecciona un personaje");
        dialog->Information(info.c_str());
        dialog->Buttons("Seleccionar", "Cancelar");
        dialog->OnResponse = SelectCharacterMenuResponse;
        dialog->ShowToPlayer(player->GetID());
    } else {
        Kick(player->GetID());
    }

    return 1;
}

static int EmailRegisterInputResponse(Player *player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (std::string(inputtext).length() <= 0) {
            Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
            EmailRegisterInput->Caption("Registro");
            EmailRegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar un correo electronico!\n{062037}> {FFFFFF}Ingresa un correo electronico para registrarte en el PCU.");
            EmailRegisterInput->Buttons("Siguiente", "Cancelar");
            EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
            EmailRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 256) {
            Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
            EmailRegisterInput->Caption("Registro");
            EmailRegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar un correo electronico no mayor 256 caracteres!\n{062037}> {FFFFFF}Ingresa un correo electronico para registrarte en el PCU.");
            EmailRegisterInput->Buttons("Siguiente", "Cancelar");
            EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
            EmailRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (!player->HasValidEmail(std::string(inputtext))) {
            Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
            EmailRegisterInput->Caption("Registro");
            EmailRegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar un correo electronico valido!\n{062037}> {FFFFFF}Ingresa un correo electronico para registrarte en el PCU.");
            EmailRegisterInput->Buttons("Siguiente", "Cancelar");
            EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
            EmailRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        }

        sqlite3_stmt* stmt;

        const char* sql = fmt::format("SELECT Email FROM Accounts WHERE Email = '{}'", inputtext).c_str();

        if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
                EmailRegisterInput->Caption("Registro");
                EmailRegisterInput->Information("{C60004}x {FFFFFF}Este correo electronico ya esta registrado en el PCU!\n{062037}> {FFFFFF}Ingresa un correo electronico para registrarte en el PCU.");
                EmailRegisterInput->Buttons("Siguiente", "Cancelar");
                EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
                EmailRegisterInput->ShowToPlayer(player->GetID());

                return 0;
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        player->SetEmail(std::string(inputtext));

        Dialog *PasswordRegisterInput = new Dialog("PasswordRegisterInput", DIALOG_STYLE_PASSWORD);
        PasswordRegisterInput->Caption("Registro");
        PasswordRegisterInput->Information("{062037}x {FFFFFF}Ingresa una contrase�a para registrarte en el PCU.");
        PasswordRegisterInput->Buttons("Siguiente", "Cancelar");
        PasswordRegisterInput->OnResponse = PasswordRegisterInputResponse;
        PasswordRegisterInput->ShowToPlayer(player->GetID());
    } else {
        Kick(player->GetID());
    }

    return 1;
}

static int RegisterInputResponse(Player *player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (std::string(inputtext).length() <= 0) {
            Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
            RegisterInput->Caption("Registro");
            RegisterInput->Information("{C60004}x {FFFFFF}Debes ingresar un nombre de usuario!\n{062037}> {FFFFFF}Ingresa un nombre de usuario para registrarte en el PCU.");
            RegisterInput->Buttons("Siguiente", "Cancelar");
            RegisterInput->OnResponse = RegisterInputResponse;
            RegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 64) {
            Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
            RegisterInput->Caption("Registro");
            RegisterInput->Information("{C60004}x: {FFFFFF}Debes ingresar un nombre de usuario no mayor a 64 caracteres!\n{062037}> {FFFFFF}Ingresa un nombre de usuario para registrarte en el PCU.");
            RegisterInput->Buttons("Siguiente", "Cancelar");
            RegisterInput->OnResponse = RegisterInputResponse;
            RegisterInput->ShowToPlayer(player->GetID());

            return 0;
        }

        sqlite3_stmt* stmt;

        const char* sql = fmt::format("SELECT Nick FROM Accounts WHERE Nick = '{}'", inputtext).c_str();

        if (sqlite3_open("ucrp.db", &db) == SQLITE_OK) {
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
                RegisterInput->Caption("Registro");
                RegisterInput->Information("{C60004}ERROR: {FFFFFF}Este nombre de usuario ya esta registrado en el PCU!\n{062037}> {FFFFFF}Ingresa un nombre de usuario para registrarte en el PCU.");
                RegisterInput->Buttons("Siguiente", "Cancelar");
                RegisterInput->OnResponse = RegisterInputResponse;
                RegisterInput->ShowToPlayer(player->GetID());

                return 0;
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        player->SetNick(std::string(inputtext));

        Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
        EmailRegisterInput->Caption("{FFFFFF}Registro");
        EmailRegisterInput->Information("{062037}> {FFFFFF}Ingresa un correo electronico para registrarte en el PCU.");
        EmailRegisterInput->Buttons("Siguiente", "Cancelar");
        EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
        EmailRegisterInput->ShowToPlayer(player->GetID());
    } else {
        Kick(player->GetID());
    }

    return 1;
}

static int SelectMenuResponse(Player *player, int response, int listitem, const char *inputtext)
{
    if (response)
    {
        if (listitem == 0)
        {
            Dialog *LoginInput = new Dialog("LoginInput", DIALOG_STYLE_INPUT);
            LoginInput->Caption("{FFFFFF}Iniciar Sesion");
            LoginInput->Information("{062037}> {FFFFFF}Ingresa el correo electronico para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = LoginInputResponse;
            LoginInput->ShowToPlayer(player->GetID());
        }
        else if (listitem == 1)
        {
            Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
            RegisterInput->Caption("{FFFFFF}Registro");
            RegisterInput->Information("{062037}> {FFFFFF}Ingresa un nombre de usuario para registrarte en el PCU.");
            RegisterInput->Buttons("Siguiente", "Cancelar");
            RegisterInput->OnResponse = RegisterInputResponse;
            RegisterInput->ShowToPlayer(player->GetID());
        }
        else if (listitem == 2)
        {
            sampgdk::logprintf("notas de version");
            Kick(player->GetID());

            return 0;
        }
    }
    else
    {
        Kick(player->GetID());
    }

    return 1;
};