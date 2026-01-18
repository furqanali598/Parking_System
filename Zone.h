#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include <ctime>     // <--- ADD THIS for time functions
#include "ParkingArea.h"
#include "Vehicle.h" // <--- ADD THIS for Vehicle*

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
bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead(); // You'll need a getter for head in ParkingArea
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == OCCUPIED && tempSlot->slot->getVehicle()->getPlate() == plate) {
                    
                    // Calculate Time & Fee
                    time_t exitTime = time(0);
                    double seconds = difftime(exitTime, tempSlot->slot->getVehicle()->getEntryTime());
                    double hours = (seconds / 3600.0) + 1; // Minimum 1 hour charge
                    double rate = (tempSlot->slot->getVehicle()->getType() == "Car") ? 50.0 : 20.0;
                    
                    std::cout << "\n--- Receipt ---" << std::endl;
                    std::cout << "Vehicle: " << plate << " (" << tempSlot->slot->getVehicle()->getType() << ")" << std::endl;
                    std::cout << "Total Fee: " << hours * rate << " PKR" << std::endl;

                    tempSlot->slot->release();
                    return true;
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false;
    }
    Zone(std::string name) : zoneName(name), areaHead(nullptr) {}

   bool parkVehicle(Vehicle* v) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            ParkingSlot* spot = tempArea->area->findEmptySlot();
            if (spot) {
                spot->occupy(v); // Now passes the whole vehicle object
                std::cout << "Successfully parked " << v->getPlate() << " in " 
                          << tempArea->area->getAreaName() << std::endl;
                return true;
            }
            tempArea = tempArea->next;
        }
        std::cout << "Parking Full!" << std::endl;
        delete v; // Clean up memory if parking failed
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
