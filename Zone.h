#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include "ParkingArea.h" // No "include/" here!

// This is the correct structure for Zone.h
struct AreaNode {
    ParkingArea* area;
    AreaNode* next;

    AreaNode(std::string name) {
        area = new ParkingArea(name);
        next = nullptr;
    }
};

class Zone {
private:
    std::string zoneName;
    AreaNode* areaHead;

public:
    Zone(std::string name) : zoneName(name), areaHead(nullptr) {}

    bool parkVehicle(std::string plate) {
    AreaNode* tempArea = areaHead;
    while (tempArea) {
        ParkingSlot* spot = tempArea->area->findEmptySlot();
        if (spot) {
            spot->occupy(plate);
            // Inside parkVehicle after spot->occupy(plate);
double baseRate = 50.0; 
std::cout << "Allocation Cost: " << baseRate << " PKR" << std::endl;
            std::cout << "Vehicle " << plate << " parked in Area: " 
                      << tempArea->area->getAreaName() << " Slot ID: " 
                      << spot->getSlotId() << std::endl;
            return true;
        }
        tempArea = tempArea->next;
    }
    std::cout << "Error: All areas in this zone are full!" << std::endl;
    return false;
}

    void addArea(std::string name) {
        AreaNode* newNode = new AreaNode(name);
        if (!areaHead) {
            areaHead = newNode;
        } else {
            AreaNode* temp = areaHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void addSlotsToArea(std::string areaName, int count) {
    AreaNode* temp = areaHead;
    while (temp) {
        if (temp->area->getAreaName() == areaName) {
            for (int i = 1; i <= count; i++) {
                temp->area->addSlot(i); // Adds slots with IDs 1, 2, 3...
            }
            return;
        }
        temp = temp->next;
    }
}

    void displayZoneSummary() {
        std::cout << "--- Zone: " << zoneName << " ---" << std::endl;
        AreaNode* temp = areaHead;
        while (temp) {
            temp->area->displayAreaStatus();
            temp = temp->next;
        }
    }
};

#endif
