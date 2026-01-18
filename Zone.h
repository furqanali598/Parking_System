#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include <ctime>
#include "ParkingArea.h"
#include "Vehicle.h"

// Node for the Linked List of Areas
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
    AreaNode* areaHead; // Head of Areas Linked List
    double totalEarnings;

public:
    Zone(std::string name) : zoneName(name), areaHead(nullptr), totalEarnings(0.0) {}

    // Getter for GUI
    AreaNode* getAreaHead() { return areaHead; }

    void addArea(std::string name) {
        AreaNode* newNode = new AreaNode(name);
        if (!areaHead) {
            areaHead = newNode;
        } else {
            AreaNode* temp = areaHead;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

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

    bool parkVehicle(Vehicle* v) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            ParkingSlot* spot = tempArea->area->findEmptySlot();
            if (spot) {
                spot->occupy(v);
                std::cout << "\n[SUCCESS] " << v->getPlate() << " parked in " 
                          << tempArea->area->getAreaName() << std::endl;
                return true;
            }
            tempArea = tempArea->next;
        }
        std::cout << "\n[!] ERROR: All areas full!" << std::endl;
        delete v; 
        return false;
    }

    bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == OCCUPIED && tempSlot->slot->getPlate() == plate) {
                    time_t exitTime = time(0);
                    double seconds = difftime(exitTime, tempSlot->slot->getVehicle()->getEntryTime());
                    
                    // Fee: 50 for Car, 20 for Bike per hour (minimum 1 hour)
                    double hours = (seconds / 3600.0);
                    if (hours < 1.0) hours = 1.0; 
                    double rate = (tempSlot->slot->getVehicle()->getType() == "Car") ? 50.0 : 20.0;
                    
                    double fee = hours * rate;
                    totalEarnings += fee;

                    std::cout << "\n--- Receipt ---\nPlate: " << plate << "\nFee: " << fee << " PKR\n--------------" << std::endl;
                    tempSlot->slot->release();
                    return true;
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false;
    }

    void displayZoneSummary() {
        std::cout << "\n--- Zone: " << zoneName << " ---" << std::endl;
        AreaNode* temp = areaHead;
        while (temp) {
            temp->area->displayAreaStatus();
            temp = temp->next;
        }
        std::cout << "Total Revenue: " << totalEarnings << " PKR" << std::endl;
    }
};

#endif