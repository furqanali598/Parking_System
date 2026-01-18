#ifndef PARKING_AREA_H
#define PARKING_AREA_H

#include <string>
#include <iostream>
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

public:
    ParkingArea(std::string name) : areaName(name), head(nullptr) {}

    void addSlots(int count) {
        for (int i = 1; i <= count; i++) {
            SlotNode* newNode = new SlotNode(i);
            if (!head) head = newNode;
            else {
                SlotNode* temp = head;
                while (temp->next) temp = temp->next;
                temp->next = newNode;
            }
        }
    }

    std::string getAreaName() { return areaName; }
    SlotNode* getHead() { return head; }

    ParkingSlot* findEmptySlot() {
        SlotNode* temp = head;
        while (temp) {
            if (temp->slot->getStatus() == AVAILABLE) return temp->slot;
            temp = temp->next;
        }
        return nullptr;
    }
};

#endif