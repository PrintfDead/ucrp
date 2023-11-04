project(ucrp)

set(SAMPSDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/sampsdk)
set(SAMPSDK_ROOT_DIR ${SAMPSDK_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/sampgdk)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_GENERATOR_PLATFORM x86)
endif(MSVC)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(AMXConfig)
include(AddSAMPPlugin)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${SAMPSDK_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/fmt/include
	${CMAKE_CURRENT_SOURCE_DIR}/build/include/sampgdk/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/sampgdk/include
)

set(UCRP_SOURCES
	include/sqlite3.c
	include/sqlite3.h

	src/main.cpp
	src/main.def

	src/game/Player.cpp
	src/game/Player.h
	src/game/Dialog.cpp
	src/game/Dialog.h

	src/managers/PlayerManager.cpp
	src/managers/PlayerManager.h
	src/managers/CommandManager.cpp
	src/managers/CommandManager.h
	src/managers/ChatManager.cpp
	src/managers/ChatManager.h

	src/maps/SkatePark.h
	src/maps/Cemetery.h
	src/maps/Alhambra.h

	src/commands/index.h
	src/commands/users/index.h
	src/commands/users/Chat.h

	src/utils/dialogs/PlayerConnect.h

	src/utils/Constants.h
	src/utils/Point.h
	src/utils/Util.h
	
	${SAMPSDK_DIR}/amxplugin.cpp
	${SAMPSDK_DIR}/plugin.h
	${SAMPSDK_DIR}/plugincommon.h
)

add_samp_plugin(ucrp ${UCRP_SOURCES})

target_link_libraries(ucrp fmt::fmt)
target_link_libraries(ucrp sampgdk)