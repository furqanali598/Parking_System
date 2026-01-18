#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>
#include "Vehicle.h" // <--- ADD THIS LINE

enum SlotStatus { AVAILABLE, OCCUPIED, RESERVED };

class ParkingSlot {
private:
    int slotId;
    SlotStatus status;
    Vehicle* parkedVehicle; // Now the compiler will know what this is

public:
    ParkingSlot(int id) : slotId(id), status(AVAILABLE), parkedVehicle(nullptr) {}

    int getSlotId() const { return slotId; }
    SlotStatus getStatus() const { return status; } // Make sure this is public
    void setStatus(SlotStatus s) { status = s; }
    
    // Add these helper functions that the errors said were missing
    Vehicle* getVehicle() { return parkedVehicle; }
    
    std::string getPlate() const { 
        if (parkedVehicle) return parkedVehicle->getPlate();
        return "";
    }

    void occupy(Vehicle* v) {
        parkedVehicle = v;
        status = OCCUPIED;
    }

    void release() {
        if (parkedVehicle) {
            delete parkedVehicle;
            parkedVehicle = nullptr;
        }
        status = AVAILABLE;
    }
};
#endif