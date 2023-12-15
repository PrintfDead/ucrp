#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "streamer.hpp"
#include <fmt/format.h>
#include "../../game/Player.h"
#include "../../game/Items.h"
#include "../../game/Dialog.h"
#include "../../game/Message.h"
#include "../../managers/CommandManager.h"
#include "../../utils/Flags.h"
#include "../../utils/Player.h"
#include "../../utils/Util.h"
#include "../../structures/Inventory.h"
#include "../../dialogs/InventoryDialog.h"

static int RegisterInventoryCommands() {
    FLAG_User userFlag;
    FLAG_Moderator modFlag;
    FLAG_Helper helperFlag;

    CommandManager::RegisterCommand({"recoger", "levantar"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (GetPlayerState(player->GetID()) != PLAYER_STATE_ONFOOT) return Message::Error(player, "No puedes usar este comando en esta situacion.");
        if (player->Hand[0].item->ID != 0 && player->Hand[1].item->ID != 0) return Message::Error(player, "Tienes ambas manos ocupadas.");
        if (player->GetCrack() == 1) return Message::Error(player, "No puedes usar este comando en este estado.");

        int objectexists = 0;
        IFloor* floor;

        for (auto i = FloorManager::BeginFloor(); i != FloorManager::EndFloor() && objectexists == 0; i++) {
            IFloor* flooritem = i->second;

            if (!IsPlayerInRangeOfPoint(player->GetID(), 2.0, flooritem->X, flooritem->Y, flooritem->Z)) return 0;
            if (GetPlayerVirtualWorld(player->GetID()) != flooritem->VirtualWorld && GetPlayerInterior(player->GetID()) != flooritem->Interior) return 0;
            if (objectexists == 1) break;

            floor = flooritem;
            objectexists = 1;
        }

        if (objectexists == 0) return Message::Error(player, "No estas cerca de ningun objeto.");

        if (IsValidObject(floor->Object)) DestroyObject(floor->Object);
        if (floor->Label != INVALID_3DTEXT_ID) Delete3DTextLabel(floor->Label);

        if (player->Hand[0].item->ID == 0) {
            player->Hand[0].item = GetItem(floor->Item);
            player->Hand[0].amount = floor->Amount;

            if (player->Hand[0].item->IDGun > 0 && player->Hand[0].amount > 0) {
                GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
            }

            PutObject(player, 0, player->Hand[0].item->ID);
        } else if (player->Hand[1].item->ID == 0) {
            player->Hand[1].item = GetItem(floor->Item);
            player->Hand[1].amount = floor->Amount;

            PutObject(player, 0, player->Hand[1].item->ID);
        }

        RolAme(player, fmt::format("recoge {} del suelo.", GetItem(floor->Item)->Name));

        KillTimer(floor->Timer);
        FloorManager::RemoveFloor(floor);

        return 1;
    });

    CommandManager::RegisterCommand({"tirar", "soltar"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (GetPlayerState(player->GetID()) != PLAYER_STATE_ONFOOT) return Message::Error(player, "No puedes usar este comando en esta situacion.");
        if (player->Hand[0].item->ID == 0 && player->Hand[1].item->ID == 0) return Message::Error(player, "No tienes nada en las manos.");
        if (player->GetCrack() == 1) return Message::Error(player, "No puedes usar este comando en este estado.");

        if (player->Hand[0].item->ID != 0) {
            RolAme(player, fmt::format("tira {}", player->Hand[0].item->Name));

            ResetPlayerWeapons(player->GetID());
            RemovePlayerAttachedObject(player->GetID(), 7);

            float x, y, z;
            GetPlayerPos(player->GetID(), &x, &y, &z);
            int vw = GetPlayerVirtualWorld(player->GetID());
            int interior = GetPlayerInterior(player->GetID());
            DropItem(player->Hand[0].item->ID, player->Hand[0].amount, x, y, z, vw, interior);

            player->Hand[0].item = new Empty();
            player->Hand[0].amount = 0;

            return 1;
        } else if (player->Hand[1].item->ID != 0) {
            RolAme(player, fmt::format("tira {}", player->Hand[1].item->Name));

            ResetPlayerWeapons(player->GetID());
            RemovePlayerAttachedObject(player->GetID(), 8);

            float x, y, z;
            GetPlayerPos(player->GetID(), &x, &y, &z);
            int vw = GetPlayerVirtualWorld(player->GetID());
            int interior = GetPlayerInterior(player->GetID());
            DropItem(player->Hand[1].item->ID, player->Hand[1].amount, x, y, z, vw, interior);

            player->Hand[1].item = new Empty();
            player->Hand[1].amount = 0;

            return 1;
        }

        return 1;
    });

    CommandManager::RegisterCommand({"inv", "inventario", "bolso"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (player->GetCrack() == 1) {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No puedes abrir el inventario en este estado.");
            return 0;
        }
        
        Dialog* InventoryMenu = new Dialog("InventoryMenu", DIALOG_STYLE_LIST);

        std::vector<std::string> information;

        for (int i = 0; i<=4; i++) {
            ISlot slot = player->Inventory[i];

            if (slot.item->ID == 0)
                information.push_back("{062037}> {FFFFFF}" + fmt::format("{}", slot.item->Name, slot.amount));
            else
                information.push_back("{062037}> {FFFFFF}" + fmt::format("{} ({})", slot.item->Name, slot.amount));
        }
        information.push_back("{062037}---------------");
        if (player->Hand[0].item->ID == 0)
            information.push_back("{062037}> {FFFFFF}Mano Derecha: " + fmt::format("{}", player->Hand[0].item->Name, player->Hand[0].amount));
        else
            information.push_back("{062037}> {FFFFFF}Mano Derecha: " + fmt::format("{} ({})", player->Hand[0].item->Name, player->Hand[0].amount));

        if (player->Hand[1].item->ID == 0)
            information.push_back("{062037}> {FFFFFF}Mano Izquierda: " + fmt::format("{}", player->Hand[1].item->Name, player->Hand[1].amount));
        else
            information.push_back("{062037}> {FFFFFF}Mano Izquierda: " + fmt::format("{} ({})", player->Hand[1].item->Name, player->Hand[1].amount));

        std::string info = fmt::format("{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}", information[0], information[1], information[2], information[3], information[4], information[5], information[6], information[7]);

        InventoryMenu->Caption("{FFFFFF}Inventario.");
        InventoryMenu->Information(info.c_str());
        InventoryMenu->Buttons("Equipar", "Cerrar");
        InventoryMenu->OnResponse = InventoryMenuResponse;
        InventoryMenu->ShowToPlayer(player->GetID());

        return 1;
    });

    CommandManager::RegisterCommand({"sacar"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { Message::Usage(player, "/sacar <Slot>"); return 0; }
        if (params[0].empty()) { Message::Usage(player, "/giveitem <Item ID> <Amount>"); return 0; }
        
        if (player->GetCrack() == 1) {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No puedes abrir el inventario en este estado.");
            return 0;
        }

        int pos;

        if (!ParseInt(params[0], pos)) { SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Debes introducir un slot correcto."); return 0; }
        
        if (pos > 5 || pos <= 0) {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Debes introducir un slot correcto.");
            return 0;
        }

        TakeSlotItem(player, pos-1);

        ResetHands(player);

        return 1;
    });

    CommandManager::RegisterCommand({"guardar"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (player->GetCrack() == 1) {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No puedes abrir el inventario en este estado.");
            return 0;
        }

        if (player->Hand[0].item->ID == 0 && player->Hand[1].item->ID == 0)
        {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes nada en las manos.");
            return 0;
        }

        if (player->Hand[0].item->ID > 0) {
            if (player->Hand[0].item->Size >= 2) {
                SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Este objeto es demasiado grande para guardar en el inventario.");
                return 0;
            }

            int count = 0;
            bool save = false;

            for (int i = 0; i<=5; i++) {
                if (count > 4) {
                    SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes espacio en el inventario.");
                    return 0;
                }

                if (save) return 0;

                if (player->Inventory[i].item->ID == 0) {
                    player->Inventory[i].item = player->Hand[0].item;
                    player->Inventory[i].amount = player->Hand[0].amount;

                    RemovePlayerAttachedObject(player->GetID(), 7);

                    if (player->Hand[0].item->IDGun > 0) {
                        ResetPlayerWeapons(player->GetID());
                    }

                    player->Hand[0].item = new Empty();
                    player->Hand[0].amount = 0;

                    save = true;
                } else count++;
            }
        } else if (player->Hand[1].item->ID > 0) {
            if (player->Hand[1].item->Size >= 2) {
                SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Este objeto es demasiado grande para guardar en el inventario.");
                return 0;
            }

            int count = 0;
            bool save = false;

            for (int i = 0; i<=4; i++) {
                if (count > 4) {
                    SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes espacio en el inventario.");
                    return 0;
                }

                if (save) return 0;

                if (player->Inventory[i].item->ID == 0) {
                    player->Inventory[i].item = player->Hand[1].item;
                    player->Inventory[i].amount = player->Hand[1].amount;

                    RemovePlayerAttachedObject(player->GetID(), 7);

                    if (player->Hand[1].item->IDGun > 0) {
                        ResetPlayerWeapons(player->GetID());
                    }

                    player->Hand[1].item = new Empty();
                    player->Hand[1].amount = 0;

                    save = true;
                } else count++;
            }
        }

        ResetHands(player);
        
        return 1;
    });

    CommandManager::RegisterCommand({"giveitem"}, modFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (params.empty()) { Message::Usage(player, "/giveitem <Item ID> <Amount>"); return 0; }
        if (params[0].empty()) { Message::Usage(player, "/giveitem <Item ID> <Amount>"); return 0; }
        if (params[1].empty()) { Message::Usage(player, "/giveitem <Item ID> <Amount>"); return 0; }
        
        int itemid, amount;

        if (!ParseInt(params[0], itemid)) { Message::Error(player, "Debes introducir un id correcto."); return 0; }
        if (!ParseInt(params[1], amount)) { Message::Error(player, "Debes introducir una cantidad correcta."); return 0; }

        if (player->Hand[0].item->ID != 0 && player->Hand[1].item->ID != 0) { Message::Error(player, "No tienes ninguna mano vacia."); return 0; }

        IItem* item = GetItem(itemid);

        if(item->ID == 0) {
            Message::Error(player, "Este item no existe.");
            return 0;
        }

        if(amount == 0) {
            Message::Error(player, "Debes introducir una cantidad mayor a 0.");
            return 0;
        }

        if (item->MaxAmount < amount) {
            Message::Error(player, fmt::format("Sobre pasaste la cantidad maxima de este item. ({})", item->MaxAmount));
            return 0;
        }

        if (player->Hand[0].item->ID == 0) {
            if (item->IDGun > 0) {
                if (amount > 0) {
                    GivePlayerWeapon(player->GetID(), item->IDGun, amount);
                }
            }

            player->Hand[0].item = item;
            player->Hand[0].amount = amount;

            PutObject(player, 0, player->Hand[0].item->ID);
        } else if (player->Hand[1].item->ID == 0) {
            player->Hand[1].item = item;
            player->Hand[1].amount = amount;

            PutObject(player, 1, player->Hand[1].item->ID);
        }

        Message::AdminSuccess(player, fmt::format("Recibiste \"{}\" ({})", item->Name, amount));

        return 1;
    });

    CommandManager::RegisterCommand({"listitem", "listaitems"}, helperFlag, [](Player* player, std::vector<std::string> params) -> int {
        Dialog* ItemsList = new Dialog("ItemsList", DIALOG_STYLE_TABLIST_HEADERS);
        
        std::string items = "Item\tID\n"
        "{062037}> {FFFFFF}Nudillera\t1\n"
        "{062037}> {FFFFFF}Palo de Golf\t2\n"
        "{062037}> {FFFFFF}Porra\t3\n"
        "{062037}> {FFFFFF}Navaja\t4\n"
        "{062037}> {FFFFFF}Bate de Madera\t5\n"
        "{062037}> {FFFFFF}Pala \t6\n"
        "{062037}> {FFFFFF}Palo de Pool\t7\n"
        "{062037}> {FFFFFF}Katana\t8\n"
        "{062037}> {FFFFFF}Motosierra\t9\n"
        "{062037}> {FFFFFF}Dildo purpura\t10\n"
        "{062037}> {FFFFFF}Dildo\t11\n"
        "{062037}> {FFFFFF}Flores\t12\n"
        "{062037}> {FFFFFF}Granada\t13\n"
        "{062037}> {FFFFFF}Granada de Humo\t14\n"
        "{062037}> {FFFFFF}Botella Molotov\t15\n"
        "{062037}> {FFFFFF}Glock-19 9mm\t16\n"
        "{062037}> {FFFFFF}Glock-19 9mm Silenciada\t17\n"
        "{062037}> {FFFFFF}Desert Eagle .50\t18\n"
        "{062037}> {FFFFFF}Remingtn 870 Express\t19\n"
        "{062037}> {FFFFFF}Winchester Model 24 .12\t20\n"
        "{062037}> {FFFFFF}SPAS-12 18,5mm\t21\n"
        "{062037}> {FFFFFF}UZI 9mm Parabellum\t22\n"
        "{062037}> {FFFFFF}HK MP5 .22\t23\n"
        "{062037}> {FFFFFF}Kalashnikov Rifle 7,62mm\t24\n"
        "{062037}> {FFFFFF}AR-15 5,56mm\t25\n"
        "{062037}> {FFFFFF}TEC-DC9 9mm\t26\n"
        "{062037}> {FFFFFF}ZKB 680 Fox 16,5mm\t27\n"
        "{062037}> {FFFFFF}Springfield M1 Garand .308\t28\n"
        "{062037}> {FFFFFF}Aerosol de pintura\t29\n"
        "{062037}> {FFFFFF}Matafuego\t30\n"
        "{062037}> {FFFFFF}Camara T50\t31\n"
        "{062037}> {FFFFFF}Taser\t32\n"
        "{062037}> {FFFFFF}Sig Sauer P226 9mm\t33\n"
        "{062037}> {FFFFFF}Sig Sauer P250 Compact 9mm\t34\n"
        "{062037}> {FFFFFF}Glock-17 9mm\t35\n"
        "{062037}> {FFFFFF}Glock-22 .45 ACP\t36\n"
        "{062037}> {FFFFFF}Samsung Naranja\t50\n"
        "{062037}> {FFFFFF}Samsung Celeste\t51\n"
        "{062037}> {FFFFFF}Samsung Negro\t52\n"
        "{062037}> {FFFFFF}Samsung Rosa\t53\n"
        "{062037}> {FFFFFF}Samsung Rojo\t54\n"
        "{062037}> {FFFFFF}Samsung Verde\t55\n"
        "{062037}> {FFFFFF}Samsung Azul\t56\n"
        "{062037}> {FFFFFF}Samsung Amarillo\t57\n"
        "{062037}> {FFFFFF}Samsung Blanco\t58\n"
        "{062037}> {FFFFFF}Iphone Negro\t59\n"
        "{062037}> {FFFFFF}Iphone Azul\t60\n"
        "{062037}> {FFFFFF}Ihpone Blanco\t61\n";

        ItemsList->Caption("{FFFFFF}Weapons Items.");
        ItemsList->Information(items.c_str());
        ItemsList->Buttons("Obtener", "Cerrar");
        ItemsList->OnResponse = ItemsListResponse;
        ItemsList->ShowToPlayer(player->GetID());
        
        return 1;
    });

    CommandManager::RegisterCommand({"deleteitem", "delitem"}, modFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (player->Hand[0].item->ID == 0 && player->Hand[1].item->ID == 0)
        {
            Message::Error(player, "No tienes nada en las manos.");
            return 0;
        }

        int hand = -1;
        int amount = 0;
        std::string name;

        if (player->Hand[0].item->ID != 0) {
            name = player->Hand[0].item->Name;
            amount = player->Hand[0].amount;

            if (player->Hand[0].item->IDGun > 0) {
                ResetPlayerWeapons(player->GetID());
            }

            player->Hand[0].item = new Empty();
            player->Hand[0].amount = 0;

            hand = 0;

            RemovePlayerAttachedObject(player->GetID(), 7);
        } else if (player->Hand[1].item->ID != 0) {
            name = player->Hand[1].item->Name;
            amount = player->Hand[1].amount;

            player->Hand[1].item = new Empty();
            player->Hand[1].amount = 0;

            hand = 1;

            RemovePlayerAttachedObject(player->GetID(), 8);
        }
        
        Message::AdminSuccess(player, fmt::format("Se elimino el item \"{}\" ({}) de la mano {}", name, amount, hand == 0 ? "derecha" : "izquierda"));

        return 1;
    });

    CommandManager::RegisterCommand({"mano", "cambiarmano", "cmano"}, userFlag, [](Player* player, std::vector<std::string> params) -> int {
        if (player->GetCrack() == 1) {
            Message::Error(player, "No puedes abrir el inventario en este estado.");
            return 0;
        }

        if (player->Hand[0].item->ID == 0 && player->Hand[1].item->ID == 0)
        {
            Message::Error(player, "No tienes nada en las manos.");
            return 0;
        }

        std::string itemName;

        if (player->Hand[0].item->ID == 0) {
            player->Hand[0].item = player->Hand[1].item;
            player->Hand[0].amount = player->Hand[1].amount;

            player->Hand[1].item = new Empty();
            player->Hand[1].amount = 0;

            RemovePlayerAttachedObject(player->GetID(), 7);
            RemovePlayerAttachedObject(player->GetID(), 8);

            if (player->Hand[0].item->IDGun > 0) {
                if (player->Hand[0].amount > 0) {
                    GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
                }
            }

            PutObject(player, 0, player->Hand[0].item->ID);

            itemName = player->Hand[0].item->Name;
        } else if (player->Hand[1].item->ID == 0) {
            player->Hand[1].item = player->Hand[0].item;
            player->Hand[1].amount = player->Hand[0].amount;

            player->Hand[0].item = new Empty();
            player->Hand[0].amount = 0;

            ResetPlayerWeapons(player->GetID());
            RemovePlayerAttachedObject(player->GetID(), 7);
            RemovePlayerAttachedObject(player->GetID(), 8);

            PutObject(player, 1, player->Hand[1].item->ID);

            itemName = player->Hand[1].item->Name;
        } else if (player->Hand[0].item->ID != 0 && player->Hand[1].item->ID != 0) {
            RemovePlayerAttachedObject(player->GetID(), 7);
            RemovePlayerAttachedObject(player->GetID(), 8);

            PutObject(player, 0, player->Hand[1].item->ID);
            PutObject(player, 1, player->Hand[0].item->ID);

            IItem* HandR = player->Hand[0].item;
            int HandRAmount = player->Hand[0].amount;
            IItem* HandL = player->Hand[1].item;
            int HandLAmount = player->Hand[1].amount;

            player->Hand[0].item = HandL;
            player->Hand[0].amount = HandLAmount;
            player->Hand[1].item = HandR;
            player->Hand[1].amount = HandRAmount;

            itemName = player->Hand[0].item->Name;
        } else {
            Message::Error(player, "Ocurrio un error..");
            return 0;
        }

        RolAme(player, fmt::format("cambia {} de mano", itemName));

        return 1;
    });

    return 1;
}