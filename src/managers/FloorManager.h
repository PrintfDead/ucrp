#pragma once

#include <unordered_map>
#include "../structures/Inventory.h"

class FloorManager
{
public:
    static IFloor* GetFloor(int floorid);

    static const std::unordered_map<int, IFloor*>::const_iterator BeginFloor();
	static const std::unordered_map<int, IFloor*>::const_iterator EndFloor();

    static int AddFloor(IFloor* floor);
	static void RemoveFloor(IFloor* floor);

private:
    FloorManager();
	~FloorManager();

	static inline std::unordered_map<int, IFloor*> floorMap;
    static inline int count = 0;
};