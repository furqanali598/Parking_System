#ifndef ZONE_H
#define ZONE_H

#include <string>
#include <ctime>
#include <windows.h>
#include "ParkingArea.h"

struct AreaNode {
    ParkingArea* area;
    AreaNode* next;
    AreaNode(std::string name) { area = new ParkingArea(name); next = nullptr; }
};

class Zone {
private:
    std::string zoneName;
    AreaNode* head;

public:
    Zone(std::string name) : zoneName(name), head(nullptr) {}

    void addArea(std::string name) {
        AreaNode* newNode = new AreaNode(name);
        if (!head) head = newNode;
        else {
            AreaNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    void addSlotsToArea(std::string areaName, int count) {
        AreaNode* temp = head;
        while (temp) {
            if (temp->area->getAreaName() == areaName) {
                temp->area->addSlots(count);
                return;
            }
            temp = temp->next;
        }
    }

    AreaNode* getAreaHead() { return head; }

    bool isAlreadyParked(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) {
                    return true; 
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false;
    }

    bool parkVehicle(Vehicle* v) {
        if (isAlreadyParked(v->getPlate())) {
            std::string msg = "Error: Vehicle " + v->getPlate() + " is already parked!";
            MessageBoxA(NULL, msg.c_str(), "Security Alert", MB_OK | MB_ICONWARNING);
            delete v; 
            return false;
        }
        AreaNode* temp = head;
        while (temp) {
            ParkingSlot* spot = temp->area->findEmptySlot();
            if (spot) {
                spot->occupy(v);
                return true;
            }
            temp = temp->next;
        }
        MessageBoxA(NULL, "Parking Lot Full!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) {
                    time_t exitTime = time(0);
                    double seconds = difftime(exitTime, tempSlot->slot->getVehicle()->getEntryTime());
                    double rate = (tempSlot->slot->getVehicle()->getType() == "Car") ? 50.0 : 20.0;
                    double total = (seconds / 3600.0) * rate; 
                    if (total < rate) total = rate; 

                    std::string receipt = "Plate: " + plate + "\nFee: Rs. " + std::to_string((int)total);
                    MessageBoxA(NULL, receipt.c_str(), "Unparking Successful", MB_OK | MB_ICONINFORMATION);
                    
                    tempSlot->slot->release();
                    return true;
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        MessageBoxA(NULL, "Vehicle not found!", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
};

#endif