#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include <ctime>
#include "ParkingArea.h"
#include "Vehicle.h"

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
    AreaNode* areaHead;
    double totalEarnings; // Track total revenue

public:
    Zone(std::string name) : zoneName(name), areaHead(nullptr), totalEarnings(0.0) {}

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
                          << tempArea->area->getAreaName() << " (Slot " << spot->getSlotId() << ")" << std::endl;
                return true;
            }
            tempArea = tempArea->next;
        }
        std::cout << "\n[!] ERROR: Parking is full in all areas of " << zoneName << std::endl;
        delete v; 
        return false;
    }

    bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = areaHead;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == OCCUPIED && tempSlot->slot->getPlate() == plate) {
                    
                    // Fee Calculation logic
                    time_t exitTime = time(0);
                    double seconds = difftime(exitTime, tempSlot->slot->