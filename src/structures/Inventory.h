#pragma once

#include <fmt/format.h>
#include <sampgdk/core.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include "../game/Items.h"

struct IHand {
    IItem* item;
    int amount;
};

struct ISlot {
    int ID;
    IItem* item;
    int amount;
};

struct IFloor {
    int ID;
    int Item;
    int Amount;
    float X, Y, Z;
    int VirtualWorld;
    int Interior;
    int Object;
    int Label;
    int Timer;
};

static inline ISlot MakeSlot(int id, IItem* item, int amount) {
    return ISlot {id, item, amount};
}