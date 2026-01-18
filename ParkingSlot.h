#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>

enum SlotStatus { AVAILABLE, OCCUPIED, RESERVED };

class ParkingSlot {
private:
    int slotId;
    SlotStatus status;
    std::string vehicleNumber;

public:
// Get the plate to find the car later
    std::string getPlate() const { return vehicleNumber; }
bool isOccupied() const { return status == OCCUPIED; }
    ParkingSlot(int id) : slotId(id), status(AVAILABLE), vehicleNumber("") {}

    int getSlotId() const { return slotId; }
    SlotStatus getStatus() const { return status; }
    void setStatus(SlotStatus s) { status = s; }
    
    void occupy(std::string plate) {
        vehicleNumber = plate;
        status = OCCUPIED;
    }
    void release() {
        vehicleNumber = "";
        status = AVAILABLE;
    }
};

#endif