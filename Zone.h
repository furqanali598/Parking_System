#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include <string>
#include "ParkingArea.h" // No "include/" here!

// This is the correct structure for Zone.h
struct AreaNode {
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

public:
    Zone(std::string name) : zoneName(name), areaHead(nullptr) {}

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

    void displayZoneSummary() {
        std::cout << "--- Zone: " << zoneName << " ---" << std::endl;
        AreaNode* temp = areaHead;
        while (temp) {
            temp->area->displayAreaStatus();
            temp = temp->next;
        }
    }
};

#endif