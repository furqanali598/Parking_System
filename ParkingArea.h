#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include <iostream>
#include <string>
#include "ParkingSlot.h"

// Color codes for Terminal UI
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
    SlotNode* head; // Head of Slots Linked List

public:
    ParkingArea(std::string name) : areaName(name), head(nullptr) {}

    // Getters
    std::string getAreaName() const { return areaName; }
    SlotNode* getHead() { return head; } // Needed for GUI to see slots

    // Add a slot to the linked list
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

    // Find first available slot (Logic)
    ParkingSlot* findEmptySlot() {
        SlotNode* temp = head;
        while (temp) {
            if (temp->slot->getStatus() == AVAILABLE) return temp->slot;
            temp = temp->next;
        }
        return nullptr;
    }

    // Terminal Display with Colors
    void displayAreaStatus() {
        std::cout << "\nArea: " << YELLOW << areaName << RESET << std::endl;
        SlotNode* temp = head;
        while (temp) {
            std::cout << "  Slot " << temp->slot->getSlotId() << ": ";
            if (temp->slot->getStatus() == OCCUPIED) {
                std::cout << RED << "[OCCUPIED by " << temp->slot->getPlate() << "]" << RESET << std::endl;
            } else {
                std::cout << GREEN << "[AVAILABLE]" << RESET << std::endl;
            }
            temp = temp->next;
        }
    }
};

#endif