#pragma warning(disable: 4305)
#pragma once

#include <fmt/format.h>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <sampgdk/a_objects.h>
#include "../game/Player.h"
#include "../game/Items.h"
#include "../managers/FloorManager.h"
#include "Constants.h"
#include "Player.h"

static int PutObject(Player* player, int hand, int itemid) {
    int model = GetItem(itemid)->Model;

    switch (hand) {
        case 0: // mano derecha 
        {
            if (itemid >= 1 && itemid <= 36) SetPlayerAttachedObject(player->GetID(), 7, model, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // weapons
            else if (itemid >= 50 && itemid <= 61) SetPlayerAttachedObject(player->GetID(), 7, model, 6, 0.093999, 0.012000, -0.010999, -81.199974, 10.000000, 179.099945, 0.990999, 0.829000, 0.903000, 0, 0); // telefonos
            
            return 0;
        }
        case 1: // mano izquierda
        {
            switch(itemid) {
                case 1: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.024999, 0.000000, -0.005999, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Nudilleras
				case 2: SetPlayerAttachedObject(player->GetID(), 8, model, 5, -0.040000, 0.067000, -0.003000, -29.899999, 152.000000, 176.000030, 1.000000, 1.000000, 1.000000, 0, 0); // Palo de golf
				case 3: SetPlayerAttachedObject(player->GetID(), 8, model, 5, -0.115999, 0.113999, 0.087999, 0.000000, 44.500000, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Porra
				case 4: SetPlayerAttachedObject(player->GetID(), 8, model, 5, -0.007000, 0.060999, -0.013000, 156.600051, 9.100000, -14.800011, 1.000000, 1.000000, 1.000000, 0, 0); // Navaja
				case 5: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.135999, 0.030999, 0.053999, -17.900001, 153.800018, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Bate de madera
				case 6: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.041999, 0.051999, -0.080000, -27.899986, 155.800018, -167.499938, 1.000000, 1.000000, 1.000000, 0, 0); // Pala
				case 7: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.156999, 0.092000, 0.071999, -25.099998, 172.899963, -19.899995, 1.000000, 1.000000, 1.000000, 0, 0); // Palo de pool
				case 8: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.058000, 0.008000, -0.109000, -27.999998, 155.100036, 172.699966, 1.000000, 1.000000, 1.000000, 0, 0); // Katana
				case 9: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.000000, 0.078999, -0.009000, 155.599990, 7.200000, 4.800000, 1.000000, 1.000000, 1.000000, 0, 0); // Motosierra
				case 10: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.015000, 0.041000, -0.068000, -25.599971, 162.900054, -175.300018, 1.000000, 1.000000, 1.000000, 0, 0); // Dildo purpura
				case 11: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.023999, 0.052999, -0.041999, 157.899978, 2.499999, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Dildo
				case 12: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.176000, 0.000000, -0.011000, -23.399999, 160.599990, -6.399995, 1.000000, 1.000000, 1.000000, 0, 0); // Flowers
				case 13: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.019000, 0.047000, -0.021000, 164.499984, 0.000000, -17.599994, 1.000000, 1.000000, 1.000000, 0, 0); // Granada
				case 14: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.034000, 0.063000, -0.037999, 164.500000, 14.599996, -4.700009, 1.000000, 1.000000, 1.000000, 0, 0); // Granada de gas
				case 15: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.022999, 0.039000, -0.125999, 165.700012, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Molotov
				case 16: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.033000, 0.057999, -0.017999, 146.299987, 14.999998, -4.700002, 1.000000, 1.000000, 1.000000, 0, 0); // Glock17
				case 17: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.050999, 0.038000, -0.021999, 144.500015, 9.100000, -11.600002, 1.000000, 1.000000, 1.000000, 0, 0); // Glock17 Silenciada
				case 18: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.041000, 0.038000, -0.026999, 173.899963, 0.000000, 1.799999, 1.000000, 1.000000, 1.000000, 0, 0); // Desert Eagle
				case 19: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.000000, 0.089000, 0.000000, 153.500015, 10.000001, 4.800000, 1.000000, 1.000000, 1.000000, 0, 0); // Remington 870 Express
				case 20: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.040000, 0.026000, 0.011000, -156.300003, 0.000000, 2.600000, 1.000000, 1.000000, 1.000000, 0, 0); // Winchester 24
				case 21: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.023000, 0.048999, -0.013000, 172.200012, 14.400005, 0.599999, 1.000000, 1.000000, 1.000000, 0, 0); // Spas12
				case 22: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.024999, 0.048000, -0.012000, 172.000076, -3.099988, -2.099991, 1.000000, 1.000000, 1.000000, 0, 0); // UZI
				case 23: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.005000, 0.065000, -0.016000, 162.399963, 9.599999, 8.400000, 1.000000, 1.000000, 1.000000, 0, 0); // HK MP5
				case 24: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.057999, 0.076999, -0.023999, 162.799942, 14.399999, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // AK47
				case 25: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.043999, 0.028999, -0.038000, 167.499969, 15.999998, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // AR15
				case 26: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.048000, 0.049999, 0.011000, -172.600006, -2.299995, 5.999999, 1.000000, 1.000000, 1.000000, 0, 0); // TEC9
				case 27: SetPlayerAttachedObject(player->GetID(), 8, model, 5, -0.043000, 0.065999, -0.044999, 167.499954, 17.399999, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Rifle
				case 28: SetPlayerAttachedObject(player->GetID(), 8, model, 5, -0.018000, 0.085000, 0.033000, 167.399978, 12.200000, -0.299997, 1.000000, 1.000000, 1.000000, 0, 0); // Sniper
				case 29: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.036999, 0.026999, -0.079999, 152.899993, 14.900004, -0.400000, 1.000000, 1.000000, 1.000000, 0, 0); // Spray
				case 30: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.004000, 0.061999, -0.063999, 126.599990, 33.099998, 2.200000, 1.000000, 1.000000, 1.000000, 0, 0); // Extintor
				case 31: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.018000, 0.062999, 0.074999, -109.400039, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000, 0, 0); // Camara T50
				case 32: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.050999, 0.038000, -0.021999, 144.500015, 9.100000, -11.600002, 1.000000, 1.000000, 1.000000, 0, 0); // Taser
                case 33: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.033000, 0.057999, -0.017999, 146.299987, 14.999998, -4.700002, 1.000000, 1.000000, 1.000000, 0, 0); // Sig Sauer P226
                case 34: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.033000, 0.057999, -0.017999, 146.299987, 14.999998, -4.700002, 1.000000, 1.000000, 1.000000, 0, 0); // Sig Sauer P250 Compact
                case 35: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.033000, 0.057999, -0.017999, 146.299987, 14.999998, -4.700002, 1.000000, 1.000000, 1.000000, 0, 0); // Glock 17
                case 36: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.041000, 0.038000, -0.026999, 173.899963, 0.000000, 1.799999, 1.000000, 1.000000, 1.000000, 0, 0); // Glock 22

                case 50: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 51: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 52: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 53: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 54: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 55: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 56: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 57: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 58: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 59: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 60: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
                case 61: SetPlayerAttachedObject(player->GetID(), 8, model, 5, 0.056000, 0.000000, 0.007000, -97.100006, 0.000000, 0.000000, 0.990999, 0.829000, 0.903000, 0, 0); // telefono
            }
        }

        return 0;
    }

    return 1;
}

static int ResetHands(Player* player) {
    RemovePlayerAttachedObject(player->GetID(), 7);
    RemovePlayerAttachedObject(player->GetID(), 8);

    if (player->Hand[0].item->ID != 0) {
        if (player->Hand[0].item->IDGun > 0) {
            if (player->Hand[0].amount > 0) {
                ResetPlayerWeapons(player->GetID());
                GivePlayerWeapon(player->GetID(), player->Hand[0].item->IDGun, player->Hand[0].amount);
            }
        }

        PutObject(player, 0, player->Hand[0].item->ID);
    }
    if (player->Hand[1].item->ID != 0) {
        if (player->Hand[1].item->IDGun > 0) {
            if (player->Hand[1].amount > 0) {
                ResetPlayerWeapons(player->GetID());
                GivePlayerWeapon(player->GetID(), player->Hand[1].item->IDGun, player->Hand[1].amount);
            }
        }

        PutObject(player, 0, player->Hand[1].item->ID);
    }

    return 1;
}

static int TakeSlotItem(Player* player, int slot) {
    IItem* Slot = player->Inventory[slot].item;
    int SlotAmount = player->Inventory[slot].amount;

    if (Slot->ID == 0) {
        SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No hay nada en este slot.");
        return 0;
    }

    ResetHands(player);

    if (player->Hand[0].item->ID == 0) {
        if (Slot->IDGun > 0) {
            if (SlotAmount > 0) {
                GivePlayerWeapon(player->GetID(), Slot->IDGun, SlotAmount);
            }
        }

        player->Hand[0].item = Slot;
        player->Hand[0].amount = SlotAmount;

        PutObject(player, 0, player->Hand[0].item->ID);

        player->Inventory[slot].item = new Empty();
        player->Inventory[slot].amount = 0;

        RolAme(player, fmt::format("saca {} del inventario", player->Hand[0].item->Name));

        return 0;
    } else if (player->Hand[1].item->ID == 0) {
        player->Hand[1].item = Slot;
        player->Hand[1].amount = SlotAmount;

        IItem* item = player->Hand[1].item;

        PutObject(player, 1, item->ID);

        player->Inventory[slot].item = new Empty();
        player->Inventory[slot].amount = 0;

        RolAme(player, fmt::format("saca {} del inventario", player->Hand[1].item->Name));

        return 0;
    } else {
        SendClientMessage(player->GetID(), COLOR_WHITE, "{C60004}x {FFFFFF}No tienes ninguna mano vacia.");
        return 0;
    }

    return 1;
}

struct Params {
    int floorid;
    Params(int id) : floorid(id) {}
};

void SAMPGDK_CALL ClearFloorItem(int timerid, void* params) {
    Params* p = (Params*) params;

    IFloor* floor = FloorManager::GetFloor(p->floorid);

    if (IsValidObject(floor->Object)) DestroyObject(floor->Object);
    
    if(floor->Label != INVALID_3DTEXT_ID) Delete3DTextLabel(floor->Label);

    KillTimer(floor->Timer);

    FloorManager::RemoveFloor(floor);
}

static int DropItem(int objectid, int amount, float x, float y, float z, int vw, int interior) {
    if (objectid != 0) {
        float rotx, roty, rotz;

        z -= 1.0f;

        IFloor* floor = new IFloor();
        floor->Item = objectid;
        floor->Amount = amount;
        floor->X = x;
        floor->Y = y;
        floor->Z = z;
        floor->VirtualWorld = vw;
        floor->Interior = interior;

        int id = FloorManager::AddFloor(floor);

        if (objectid >= 1 && objectid <= 5) { rotx = 85; roty = 1; rotz = 8; z += 0; } // nudillera, palo de golf, porra, cuchillo y bate
        else if (objectid == 6) { rotx = 8.5; roty = 90; rotz = -82; z += 0.1; } // pala
        else if (objectid == 7 || objectid == 8) { rotx = 85; roty = 1; rotz = 8; z += 0; } // palo de pool y katana
        else if (objectid == 9) { rotx = 95; roty = -69; rotz = 8; z += 0.08; } //motosierra
        else if (objectid == 10 || objectid == 11) { rotx = 90; roty = 1; rotz =  8; z += 0; } // dildos
        else if (objectid == 12) { rotx = 70; roty = 1; rotz =  8; z += 0; } // flores
        else if (objectid == 13) { rotx = 0; roty = -17; rotz = 0; z += 0.05; } // granada
        else if (objectid == 14) { rotx = 0; roty = 0.3; rotz = -7; z += 0.06; } // granada de humo
        else if (objectid == 15) { rotx = -5.5; roty = -1.8; rotz = -7; z += 0.2; } // molotov
        else if (objectid == 16 || objectid == 33 || objectid == 34 || objectid == 35) { rotx = -85; roty = 3; rotz = 3; z += 0.04; } // glock19, p226, p250, glock17
        else if (objectid == 17) { rotx = -85; roty = 3; rotz = 3; z += 0.05; } //glock19 silenciada
        else if (objectid == 18 || objectid == 36) { rotx = -85; roty = 3; rotz = 3; z += 0.04; } //glock22, desert eagle
        else if (objectid == 19) { rotx = -82; roty = 58; rotz = 64; z += 0.06; } // remington 870
        else if (objectid == 20) { rotx = -90; roty = 58; rotz =  64; z += 0.05; } // winchester 24
        else if (objectid == 21) { rotx = -82; roty = 58; rotz =  64; z += 0.05; } // spas 12
        else if (objectid == 22 || objectid == 23) { rotx = -90; roty = 0; rotz = 0; z += 0.04; } // uzi, mp5
        else if (objectid == 24 || objectid == 25) { rotx = -92.6; roty = 0.8; rotz = -3.1; z += 0.06; } // ak47, ar15
        else if (objectid == 26) { rotx = -92.6; roty = 0.8; rotz = -3.1; z += 0.06; } // tec 9
        else if (objectid == 27) { rotx = -112; roty = -150; rotz = -159; z += 0.04; } // rifle de caza
        else if (objectid == 28) { rotx = -105; roty = -150; rotz = -159; z += 0.04; } // rifle francotirador
        else if (objectid == 29) { rotx = 0.5; roty = -5.5; rotz = -4.7; z += 0.15; } // spray
        else if (objectid == 30) { rotx = -3.2; roty = 40.3; rotz = 0; z += 0.59; } // extintor
        else if (objectid == 31) { rotx = 0; roty = -1.8; rotz = -30; z += 0.02; } // camera
        else if (objectid == 32) { rotx = 0; roty = 0; rotz = 9.7; z += 0.19; } // paracaidas
        else if (objectid >= 50 || objectid <= 61) { rotx = 0; roty = 0; rotz = 0; z += 0.02; } // telefonos

        std::string text = fmt::format("{} ({})", GetItem(objectid)->Name, amount);

        floor->Label = Create3DTextLabel(text.c_str(), COLOR_ORANGE, x, y, z, 3.0, vw, 0);
        floor->Object = CreateObject(GetItem(objectid)->Model, x, y, z, rotx, roty, rotz, 15.0f);

        floor->Timer = SetTimer(3600000*2, false, ClearFloorItem, new Params(id));

        return 1;
    }

    return 1;
}