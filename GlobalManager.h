#ifndef ZONE_H
#define ZONE_H
#include "Zone.h"
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
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
    int posX, posY;

public:
    Zone(std::string name, int x, int y) : zoneName(name), head(nullptr), posX(x), posY(y) {}

    int getX() { return posX; }
    int getY() { return posY; }
    std::string getName() { return zoneName; }
    AreaNode* getAreaHead() { return head; }

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

    bool isAlreadyParked(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) return true;
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false;
    }

    bool parkVehicle(Vehicle* v) {
        if (isAlreadyParked(v->getPlate())) {
            MessageBoxA(NULL, "Vehicle already in building!", "Error", MB_ICONERROR);
            delete v;
            return false;
        }
        AreaNode* temp = head;
        while (temp) {
            ParkingSlot* spot = temp->area->findEmptySlot();
            if (spot) { spot->occupy(v); return true; }
            temp = temp->next;
        }
        return false;
    }

    bool releaseVehicle(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) {
                    tempSlot->slot->release();
                    return true;
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return false;
    }
};

class GlobalManager {
private:
    std::vector<Zone*> buildings;
public:
    void addBuilding(Zone* z) { buildings.push_back(z); }
    const std::vector<Zone*>& getBuildings() { return buildings; }

    Zone* findNearestAvailable(int userX, int userY) {
        Zone* nearest = nullptr;
        double minDistance = 1e18;
        for (Zone* b : buildings) {
            double dist = std::sqrt(std::pow(b->getX() - userX, 2) + std::pow(b->getY() - userY, 2));
            if (dist < minDistance) {
                AreaNode* temp = b->getAreaHead();
                while(temp) {
                    if(temp->area->findEmptySlot()) { minDistance = dist; nearest = b; break; }
                    temp = temp->next;
                }
            }
        }
        return nearest;
    }
};

#endif