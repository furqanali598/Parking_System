#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <iostream>
#include <string>
#include "ParkingSlot.h" // No "include/" here!

struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;
    SlotNode(int id) {
        slot = new ParkingSlot(id);
        next = nullptr;
    }
};

class ParkingArea {
private:

    std::string areaName;
    SlotNode* head;
    int totalSlots;

public:
std::string getAreaName() { return areaName; }


ParkingSlot* findEmptySlot() {
    SlotNode* temp = head;
    while (temp) {
        if (temp->slot->getStatus() == AVAILABLE) {
            return temp->slot;
        }
        temp = temp->next;
    }
    return nullptr; // All slots in this area are full
}
    ParkingArea(std::string name) : areaName(name), head(nullptr), totalSlots(0) {}

    void addSlot(int id) {
        SlotNode* newNode = new SlotNode(id);
        if (!head) head = newNode;
        else {
            SlotNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        totalSlots++;
    }
    

    void displayAreaStatus() {
        std::cout << "Area: " << areaName << " | Slots: " << totalSlots << std::endl;
    }
};

#endif