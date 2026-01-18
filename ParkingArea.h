#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <iostream>
#include <string>
#include "ParkingSlot.h"

// Put the color codes at the top so they are available to the whole file
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

class SlotNode {
public:
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

public:
    ParkingArea(std::string name) : areaName(name), head(nullptr) {}

    std::string getAreaName() const { return areaName; }
    SlotNode* getHead() { return head; }

    void addSlot(int id) {
        SlotNode* newNode = new SlotNode(id);
        if (!head) {
            head = newNode;
        } else {
            SlotNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    ParkingSlot* findEmptySlot() {
        SlotNode* temp = head;
        while (temp) {
            if (temp->slot->getStatus() == AVAILABLE) return temp->slot;
            temp = temp->next;
        }
        return nullptr;
    }

    // PASTE THE NEW FUNCTION HERE
    void displayAreaStatus() {
        std::cout << "\nArea: " << YELLOW << areaName << RESET << std::endl;
        SlotNode* temp = head;
        while (temp) {
            std::cout << "  Slot " << temp->slot->getSlotId() << ": ";
            if (temp->slot->getStatus() == OCCUPIED) {
                // Shows plate number in Red
                std::cout << RED << "[OCCUPIED by " << temp->slot->getPlate() << "]" << RESET << std::endl;
            } else {
                // Shows Available in Green
                std::cout << GREEN << "[AVAILABLE]" << RESET << std::endl;
            }
            temp = temp->next;
        }
    }
};

#endif