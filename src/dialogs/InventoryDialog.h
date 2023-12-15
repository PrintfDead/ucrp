#include <fmt/format.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "sqlite3.h"
#include "../game/Player.h"
#include "../game/Items.h"
#include "../structures/Inventory.h"
#include "../utils/Objects.h"

static int ItemsListResponse(Player *player, int response, int listitem, const char *inputtext)
{
    if (!response)
        return 0;

    if (player->Hand[0].item->ID != 0 && player->Hand[1].item->ID != 0) return Message::Error(player, "Tienes las manos ocupadas.");

    IItem* item = GetItem(listitem+1);
    int amount = item->MaxAmount; 

    if (player->Hand[0].item->ID == 0)
    {
        if (item->IDGun > 0)
        {
            if (amount > 0)
            {
                GivePlayerWeapon(player->GetID(), item->IDGun, amount);
            }
        }

        player->Hand[0].item = item;
        player->Hand[0].amount = amount;

        PutObject(player, 0, player->Hand[0].item->ID);
    }
    else if (player->Hand[1].item->ID == 0)
    {
        player->Hand[1].item = item;
        player->Hand[1].amount = amount;

        PutObject(player, 1, player->Hand[1].item->ID);
    }

    Message::AdminSuccess(player, fmt::format("Recibiste \"{}\" ({})", item->Name, amount));

    return 1;
}

static int InventoryMenuResponse(Player *player, int response, int listitem, const char *inputtext)
{
    if (!response)
        return 0;

    if (listitem >= 0 && listitem <= 4)
        TakeSlotItem(player, listitem);
    else if (listitem == 5)
        return 0;
    else if (listitem == 6)
    {
        if (player->Hand[0].item->Size >= 2)
        {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Este objeto es demasiado grande para guardar en el inventario.");
            return 0;
        }

        int count = 0;
        bool save = false;

        for (int i = 0; i <= 5; i++)
        {
            if (count > 4)
            {
                SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes espacio en el inventario.");
                return 0;
            }

            if (save)
                return 0;

            if (player->Inventory[i].item->ID == 0)
            {
                player->Inventory[i].item = player->Hand[0].item;
                player->Inventory[i].amount = player->Hand[0].amount;

                RemovePlayerAttachedObject(player->GetID(), 7);

                if (player->Hand[0].item->IDGun > 0)
                {
                    ResetPlayerWeapons(player->GetID());
                }

                player->Hand[0].item = new Empty();
                player->Hand[0].amount = 0;

                save = true;
            }
            else
                count++;
        }
    }
    else if (listitem == 7)
    {
        if (player->Hand[1].item->Size >= 2)
        {
            SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}Este objeto es demasiado grande para guardar en el inventario.");
            return 0;
        }

        int count = 0;
        bool save = false;

        for (int i = 0; i <= 4; i++)
        {
            if (count > 4)
            {
                SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes espacio en el inventario.");
                return 0;
            }

            if (save)
                return 0;

            if (player->Inventory[i].item->ID == 0)
            {
                player->Inventory[i].item = player->Hand[1].item;
                player->Inventory[i].amount = player->Hand[1].amount;

                RemovePlayerAttachedObject(player->GetID(), 7);

                if (player->Hand[1].item->IDGun > 0)
                {
                    ResetPlayerWeapons(player->GetID());
                }

                player->Hand[1].item = new Empty();
                player->Hand[1].amount = 0;

                save = true;
            }
            else
                count++;
        }
    }

    return 0;
}