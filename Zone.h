#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include <ctime>
#include "ParkingArea.h"
#include "Vehicle.h"

// Node for the Linked List of Parking Areas
class AreaNode {
public:
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
    AreaNode* areaHead;      // Head of the Areas Linked List
    double totalEarnings;    // Revenue tracker

public:
    Zone(std::string name) : zoneName(name), areaHead(nullptr), totalEarnings(0.0) {}

    // Add a new Area (Block) to the Zone
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

    // Add multiple slots to a specific Area
    void addSlotsToArea(std::string areaName, int count) {
        AreaNode* temp = areaHead;
        while (temp) {
            if (temp->area->getAreaName() == areaName) {
                for (int i = 1; i <= count; i++) {
                    temp->area->addSlot(i);
                }
                return;
            }
            temp = temp->next;
        }
    }

    // Allocation Logic: Find first available slot in any area
    bool parkVehicle(Vehicle* v) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            ParkingSlot* spot = tempArea->area->findEmptySlot();
            if (spot) {
                spot->occupy(v);
                std::cout << "\n[SUCCESS] " << v->getPlate() << " parked in " 
                          << tempArea->area->getAreaName() << " (Slot " << spot->getSlotId() << ")" << std::endl;
                return true;
            }
            tempArea = tempArea->next;
        }
        std::cout << "\n[!] ERROR: Parking is full in all areas of " << zoneName << std::endl;
        delete v; // Clean up memory if parking fails
        return false;
    }

    // Release Logic: Search for plate, calculate fee, and free slot
    bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            // Traverse the slots within this area
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == OCCUPIED && tempSlot->slot->getPlate() == plate) {
                    
                    // Fee Calculation
                    time_t exitTime = time(0);
                    double seconds = difftime(exitTime, tempSlot->slot->getVehicle()->getEntryTime());
                    
                    // Logic: Charge for at least 1 hour, then hourly
                    double hours = (seconds / 3600.0);
                    if (hours < 1.0) hours = 1.0; 
                    
                    double rate = (tempSlot->slot->getVehicle()->getType() == "Car") ? 50.0 : 20.0;
                    double fee = hours * rate;
                    totalEarnings += fee;

                    std::cout << "\n========== RECEIPT ==========" << std::endl;
                    std::cout << "Plate: " << plate << std::endl;
                    std::cout << "Type:  " << tempSlot->slot->getVehicle()->getType() << std::endl;
                    std::cout << "Time:  " << (int)seconds << " seconds" << std::endl;
                    std::cout << "Fee:   " << fee << " PKR" << std::endl;
                    std::cout << "=============================" << std::endl;

                    tempSlot->slot->release(); // Frees the Vehicle object and resets status
                    return true;
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false; // Vehicle not found in any area
    }

    // Display the current state of the entire Zone
    void displayZoneSummary() {
        std::cout << "\n--- Zone Summary: " << zoneName << " ---" << std::endl;
        AreaNode* temp = areaHead;
        while (temp) {
            temp->area->displayAreaStatus();
            temp = temp->next;
        }
        std::cout << "------------------------------------" << std::endl;
        std::cout << "Total Revenue Collected: " << totalEarnings << " PKR" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
};

#endif // ZONE_H