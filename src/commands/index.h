#include "users/Chat.h"
#include "users/Inventory.h"
#include "admin/Moderator.h"

static int RegisterAllModulesCommands() {
    RegisterModeratorCommands();
    RegisterChatCommands();
    RegisterInventoryCommands();

    return 1;
}