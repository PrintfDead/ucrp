#include <fmt/format.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "sqlite3.h"
#include "../../game/Player.h"
#include "../../managers/PlayerManager.h"
#include "../../game/Dialog.h"
#include "../Constants.h"
#include "../Util.h"
#include "../Point.h"

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
        dialog->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario!\nIngresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
        dialog->Buttons("Siguiente", "Cancelar");
        dialog->OnResponse = CreateCharacterInputResponse;
        dialog->ShowToPlayer(player->GetID());

        return 0;
    } else if (std::string(inputtext).length() > 24) {
        Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
        dialog->Caption("Crear Personaje.");
        dialog->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario con no mas de 24 caracteres!\nIngresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
        dialog->Buttons("Siguiente", "Cancelar");
        dialog->OnResponse = CreateCharacterInputResponse;
        dialog->ShowToPlayer(player->GetID());

        return 0;
    } else if (!player->HasValidName(std::string(inputtext))) {
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

    PlayerManager::SavePlayer(player);
    SpawnCharacter(player);

    return 1;
}

static int SelectCharacterMenuResponse(Player* player, int response, int listitem, const char *inputtext) {
    if (response) {
        if (player->charactersMap[listitem] == -1) {
            Dialog* dialog = new Dialog("CreateCharacterInput", DIALOG_STYLE_INPUT);
            dialog->Caption("Crear Personaje.");
            dialog->Information("Ingresa un nombre y apellido para tu personaje.\nFormato: Nombre_Apellido");
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
            sqlite3_close(db);

            SpawnCharacter(player);
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
            LoginInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar una contraseña!\nIngresa la contraseña para encontrar tu cuenta.");
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
            LoginInput->Information("{C60004}ERROR: {FFFFFF}La contraseña es incorrecta!\nIngresa la contraseña para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = PasswordInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        GetCharacters(player, player->GetAccountID());

        std::string characters[3];
        characters[0] = fmt::format("({}) {}", player->charactersMap[0], player->charactersNameMap[0]);
        characters[1] = fmt::format("({}) {}", player->charactersMap[1], player->charactersNameMap[1]);
        characters[2] = fmt::format("({}) {}", player->charactersMap[2], player->charactersNameMap[2]);

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
            LoginInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un correo!\nIngresa el correo electronico para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = LoginInputResponse;
            LoginInput->ShowToPlayer(player->GetID());

            return 0;
        }

        if (!player->HasValidEmail(std::string(inputtext))) {
            Dialog *LoginInput = new Dialog("LoginInput", DIALOG_STYLE_INPUT);
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("{C60004}ERROR: {FFFFFF}Cuenta no encontrada.\nIngresa el correo electronico para encontrar tu cuenta.");
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
                LoginInput->Information("{C60004}ERROR: {FFFFFF}Cuenta no encontrada.\nIngresa el correo electronico para encontrar tu cuenta.");
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
        LoginInput->Information("Ingresa la contraseña para encontrar tu cuenta.");
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
            PasswordRegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar una contraseña!\nIngresa una contraseña para registrarte en el PCU.");
            PasswordRegisterInput->Buttons("Siguiente", "Cancelar");
            PasswordRegisterInput->OnResponse = PasswordRegisterInputResponse;
            PasswordRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 32) {
            Dialog *PasswordRegisterInput = new Dialog("PasswordRegisterInput", DIALOG_STYLE_PASSWORD);
            PasswordRegisterInput->Caption("Registro");
            PasswordRegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar una contraseña no mayor a 32 caracteres!\nIngresa una contraseña para registrarte en el PCU.");
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

        Dialog *dialog = new Dialog("SelectCharacterMenu", DIALOG_STYLE_LIST);
        dialog->Caption("Selecciona un personaje");
        dialog->Information(fmt::format("({}) {}\n({}) {}\n({}) {}", player->charactersMap[0], player->charactersNameMap[0], player->charactersMap[1], player->charactersNameMap[1], player->charactersMap[2], player->charactersNameMap[2]).c_str());
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
            EmailRegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un correo electronico!\nIngresa un correo electronico para registrarte en el PCU.");
            EmailRegisterInput->Buttons("Siguiente", "Cancelar");
            EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
            EmailRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 256) {
            Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
            EmailRegisterInput->Caption("Registro");
            EmailRegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un correo electronico no mayor 256 caracteres!\nIngresa un correo electronico para registrarte en el PCU.");
            EmailRegisterInput->Buttons("Siguiente", "Cancelar");
            EmailRegisterInput->OnResponse = EmailRegisterInputResponse;
            EmailRegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (!player->HasValidEmail(std::string(inputtext))) {
            Dialog *EmailRegisterInput = new Dialog("EmailRegisterInput", DIALOG_STYLE_INPUT);
            EmailRegisterInput->Caption("Registro");
            EmailRegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un correo electronico valido!\nIngresa un correo electronico para registrarte en el PCU.");
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
                EmailRegisterInput->Information("{C60004}ERROR: {FFFFFF}Este correo electronico ya esta registrado en el PCU!\nIngresa un correo electronico para registrarte en el PCU.");
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
        PasswordRegisterInput->Information("Ingresa una contraseña para registrarte en el PCU.");
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
            RegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario!\nIngresa un nombre de usuario para registrarte en el PCU.");
            RegisterInput->Buttons("Siguiente", "Cancelar");
            RegisterInput->OnResponse = RegisterInputResponse;
            RegisterInput->ShowToPlayer(player->GetID());

            return 0;
        } else if (std::string(inputtext).length() >= 64) {
            Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
            RegisterInput->Caption("Registro");
            RegisterInput->Information("{C60004}ERROR: {FFFFFF}Debes ingresar un nombre de usuario no mayor a 64 caracteres!\nIngresa un nombre de usuario para registrarte en el PCU.");
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
                RegisterInput->Information("{C60004}ERROR: {FFFFFF}Este nombre de usuario ya esta registrado en el PCU!\nIngresa un nombre de usuario para registrarte en el PCU.");
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
        EmailRegisterInput->Caption("Registro");
        EmailRegisterInput->Information("Ingresa un correo electronico para registrarte en el PCU.");
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
            LoginInput->Caption("Iniciar Sesion");
            LoginInput->Information("Ingresa el correo electronico para encontrar tu cuenta.");
            LoginInput->Buttons("Siguiente", "Cancelar");
            LoginInput->OnResponse = LoginInputResponse;
            LoginInput->ShowToPlayer(player->GetID());
        }
        else if (listitem == 1)
        {
            Dialog *RegisterInput = new Dialog("RegisterInput", DIALOG_STYLE_INPUT);
            RegisterInput->Caption("Registro");
            RegisterInput->Information("Ingresa un nombre de usuario para registrarte en el PCU.");
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