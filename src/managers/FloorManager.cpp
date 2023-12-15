#include "FloorManager.h"
#include "../structures/Inventory.h"

IFloor* FloorManager::GetFloor(int floorid) {
    auto index = floorMap.find(floorid);
	if (index != floorMap.end()) {
		return index->second;
	}
	return NULL;
}

const std::unordered_map<int, IFloor*>::const_iterator FloorManager::BeginFloor() {
    return floorMap.cbegin();
}

const std::unordered_map<int, IFloor*>::const_iterator FloorManager::EndFloor() {
    return floorMap.cend();
}

int FloorManager::AddFloor(IFloor* floor) {
    int id = count+1;

    floor->ID = id;
    floorMap[id] = floor;
    count++;

    return id;
}

void FloorManager::RemoveFloor(IFloor* floor) {
    floorMap.erase(floor->ID);

    delete floor;
    floor = nullptr;
}