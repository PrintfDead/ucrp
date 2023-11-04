#include "users/index.h"
#include "users/Chat.h"

static int RegisterAllModulesCommands() {
    RegisterUserCommands();
    RegisterChatCommands();

    return 1;
}