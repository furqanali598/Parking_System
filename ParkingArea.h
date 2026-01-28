#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <string>
#include <vector>
#include "ParkingSlot.h"

struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;
    SlotNode(int id) { slot = new ParkingSlot(id); next = nullptr; }
};

class ParkingArea {
private:
    std::string areaName;
    SlotNode* head;
    int totalSlots;

public:
    ParkingArea(std::string name) : areaName(name), head(nullptr), totalSlots(0) {}

    void addSlots(int count) {
        for (int i = 1; i <= count; i++) {
            totalSlots++;
            SlotNode* newNode = new SlotNode(totalSlots);
            if (!head) head = newNode;
            else {
                SlotNode* temp = head;
                while (temp->next) temp = temp->next;
                temp->next = newNode;
            }
        }
    }

    ParkingSlot* findEmptySlot() {
        SlotNode* temp = head;
        while (temp) {
            // FIXED: Using 0 instead of the undeclared AVAILABLE
            if (temp->slot->getStatus() == 0) return temp->slot;
            temp = temp->next;
        }
        return nullptr;
    }

    std::string getAreaName() { return areaName; }
    SlotNode* getHead() { return head; }
};

#endif