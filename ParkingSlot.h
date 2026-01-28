#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>
#include "Vehicle.h" // Ensure this is included

class Vehicle; // Forward declaration

class ParkingSlot {
private:
    int slotId;
    int status; 
    Vehicle* vehicle; // Now the compiler knows this is a pointer to 'something' called Vehicle

public:
    ParkingSlot(int id) : slotId(id), status(0), vehicle(nullptr) {}

    int getStatus() { return status; }
    int getSlotId() { return slotId; }

    std::string getVehicleType() {
        if (status == 1 && vehicle != nullptr) {
            return vehicle->getType(); 
        }
        return "";
    }

    std::string getPlate() {
        if (status == 1 && vehicle != nullptr) return vehicle->getPlate();
        return "";
    }

    void occupy(Vehicle* v) {
        vehicle = v;
        status = 1;
    }

    void release() {
        delete vehicle;
        vehicle = nullptr;
        status = 0;
    }
};

#endif