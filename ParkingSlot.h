#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include "Vehicle.h"

// Defined these so ParkingArea.h stops complaining
#define AVAILABLE 0
#define OCCUPIED 1

class ParkingSlot {
private:
    int slotId;
    int status; 
    Vehicle* vehicle;

public:
    ParkingSlot(int id) : slotId(id), status(AVAILABLE), vehicle(nullptr) {}

    int getSlotId() const { return slotId; }
    int getStatus() const { return status; }
    
    // Helper for GUI and Receipt logic
    Vehicle* getVehicle() { return vehicle; }

    std::string getPlate() const {
        return (vehicle != nullptr) ? vehicle->getPlate() : "";
    }

    std::string getType() const {
        if (vehicle != nullptr) return vehicle->getType();
        return "Empty";
    }

    // Renamed to occupy to match your Zone.h
    void occupy(Vehicle* v) {
        vehicle = v;
        status = OCCUPIED;
    }

    void release() {
        delete vehicle;
        vehicle = nullptr;
        status = AVAILABLE;
    }
};

#endif